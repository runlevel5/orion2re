#@author
#@category AI
#@runtime PyGhidra
#@keybinding
#@menupath Tools.AI.Clean up selected function (Gemini)
#@toolbar

"""
Ghidra 12.x (PyGhidra / Python 3) script:
- Gets selected/current function
- Extracts:
  1) function disassembly
     - left side: "raw" instruction text (ins.toString()) which often includes absolute addresses
     - comment: human-readable version with variable/symbol names (Listing-like formatting)
  2) Ghidra decompiler output
- Calls Gemini REST API (generateContent) with API key authentication
- Prints cleaned-up equivalent C/C++ code to the Ghidra Console

Changes:
1) Namespace support:
   - Extracts namespace path from the selected function in Ghidra
   - Sanitizes it into valid C/C++ identifiers
   - Includes it in the prompt context (so generated symbols/functions can be nested correctly)

2) External/global references:
   - Instructs model to emit absolute-address globals as C++ namespace-scoped inline references:
       namespace X { inline T& sym = *reinterpret_cast<T*>(std::uintptr_t{0x...}); }
"""

import os
import json
import ssl
import urllib.request
import urllib.error
import re

from ghidra.app.decompiler import DecompInterface
from ghidra.program.model.listing import CodeUnitFormat, CodeUnitFormatOptions
from ghidra.program.model.data import DataTypeWriter, Pointer, Array, TypeDef, Composite, Enum
from ghidra.program.model.data import Pointer, Array, TypeDef, Composite, Enum
from java.io import StringWriter
from java.util import ArrayList

SYSTEM_PROMPT = r"""
You are given Ghidra disassembly + Ghidra decompiler output. Produce cleaned-up C/C++ that is
semantically equivalent to the assembly.

IMPORTANT INPUT CONVENTION:
- Each disassembly line is formatted like:
    <addr>  <raw_instruction>  ; <human_readable_instruction>
- Treat the COMMENT portion after ';' as the authoritative human-readable version that
contains Ghidra variable/symbol names and types (e.g. DAT_..., FUN_..., local vars).

NAMESPACE CONTEXT:
- The input may include a line like:
    ;; Namespace (C++): A::B::C
  This is the Ghidra function namespace path, already sanitized to valid C++ identifiers.
- When emitting the decompiled function and symbols without a namespace, prefer placing them inside
  that C++ namespace block. 
- Symbols might also have :: in their name. Text to the left of the :: is the namespace and
  text to the right is the symbol name. These symbols MUST go to their own namespace blocks. Examples:

  _current_position - no namespace, use one provided via the comment:

  namespace A::B::C {
    char _current_position;
  }

  For "MOX2::Func" - MOX2 is namespace, Func is the function name:

  namespace MOX2 {
    // ... Func definition here
  }

  For "HAROLD::Load_Strings_" - HAROLD is namespace, Load_Strings_ is function name. Should look 
  similar to MOX2::Func.
 
  For "mouse::current_x" - mouse is namespace, current_x is variable name. Generate namespace for the symbol:

  namespace mouse {
    // ... current_x definition here 
  }

- Keep in mind that code might reference symbols from other namespaces. When generating
  stubs, you must correctly put symbols into correct namespace blocks deriving namespace
  name from a symbol name.
- Namespaces might have similar names (MOX vs MOX) - these should be treated
  as different namespaces.
- Combine all external symbols (variables and functions) that have same namespace name
  into a single namespace block. Example to follow:

   namespace video {
     // ... all "video" namespace symbols
     inline uint8_t& is_visible =
             *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00123456});
   }

   namespace MOX2 {
     // ... all "MOX2" namespace variables and functions
     inline uint8_t& is_visible =
             *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00123456});

     inline void __stdcall Lookup_Something() {
       using Fn = void(*)();
       auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005287A0});
       fn();
     }
   }

RULES:
* Correctness first: match control flow, calling convention, side effects, flags-dependent
   behavior, and memory accesses.
* Cleanup:
  - You are allowed to rename internal variables.
  - You are allowed to remove superfluous variables, control logic, loops, etc.
  - Generated code should resemble code written by a developer and not look like it was decompiled,
     but the execution flow should be as close to original code as possible.
* Never introduce or call C stdlib helpers (memcpy/memset/strlen/strcmp/printf/etc).
  If the binary calls an imported/external function, call that symbol directly as it exists
  in the comment/symbols. Otherwise, implement the logic inline.
* External/global references by absolute address:
  - Never inline absolute-address globals directly inside function bodies.
  - Assembly might include both address and a name. You must use name, which will be in the comment
    or decompiler output.
  - For external variables, emit following structure with the address:

   mov eax, [MOX2::is_visible]

   should generate following stub:

   namespace MOX2 {
     inline uint8_t& is_visible =
             *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00123456});
   }

  - For external functions, emit inline wrapper function with namespace (if available) using the following patterns.
    Make sure to include explicit calling convention. Do not change calling convention
    in generated stubs.

     Function with namespace, namespace block MUST be generated: 

     call [MOX2::SomeFunc]

     should produce:

     namespace MOX2 {
       inline void __cdecl FUN_5287A0() {
         using Fn = void(_cdecl*)();
         auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005287A0});
         fn();
       }
     }

     Function without namespace, no namespace block: 

     call [FUN_123456]
   
     should produce:

     inline void __cdecl FUN_123456() {
       using Fn = void(__cdecl*)();
       auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00123456});
       fn();
     }

  - The alias must match the best available symbol name from the human-readable comment.
  - Use std::uintptr_t{0x...} (C++), and reinterpret_cast.
  - For CALL/JMP targets, infer the correct function signature from usage and the human-readable comment.
  - We use naming convention from the project - mixed Snake_Case for functions and lowercase snake_case for variables.
* Use explicit binary-faithful types (uint8_t/uint16_t/uint32_t/int32_t, pointers, structs) and casts where needed.
* Also output the following code that ties decompiled function to its base address, where 0x123456 is the raw function
  start address and decompiled_func is the address of the decompiled function:

  // { 0x123456, &Decompiled_Func },

* If you see automatically generated name starting with "sub_", "addr_", "DAT_", "LAB_", "FUN_" and similar - keep original
  name, but you MUST suggest a new name in the comment. If name already matches, do no generate the comment. Examples:

     namespace MOX2 {
         // Suggested name: MOX2::is_player_selected
         inline uint32_t& DAT_00578E04 =
             *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00578E08});

         inline uint32_t& g_exampleVar =
             *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00578E18});

         // Suggested name: MOX2::Cleanup_Game
         inline void __stdcall FUN_5287A0() {
           using Fn = void(__stdcall*)();
           auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005287A0});
           fn();
         }

         // Suggested name: Find_Needle
         void __cdecl FUN_00123456(char *input_str) {
           ...
         }
     }

* If you see loops resembling standard functions, such as memset, strcmp and others,
  prefer standard library functions over custom loop implementation.
* If you see string literals as pointers, reference original string address and 
  provide potential text as a comment. Do not put address inline, create external
  variable reference. 
* You might encounter a following pattern in this project: [var] + offset * size_t.
  This usually represents a pointer a continuous region of space containing an
  array of records. Examples:

   MOVSX      EAX,word ptr [MOX::_PLAYER_NUM]                  = FFFFh
   IMUL       EDX,EAX,0xea9
   MOV        EAX,[MOX::_player]                               = ffffffff
   ADD        EAX,EDX
   
   This is equivalent to having MOX::_player is a pointer to an array of
   player structures of 0xea9 size and can be expressed in C++ code as:

   plr = &MOX::_player[MOX::_PLAYER_NUM];

   Please note that MOX::_player will have a correct type set and there
   is NO need to multiply offset by structure size. Just add a comment about
   structure size.

   You also might see a variation of this pattern where offset is calculated via
   bit shifting operations. It is exactly same behavior as a previous example
   with more operations.

        MOVSX      ECX,word ptr [MOX::_PLAYER_NUM]
        MOV        EAX,ECX
        LEA        EDX,[ECX + ECX*0x2]
        LEA        ECX,[EAX + EDX*0x4]
        MOV        EDX,dword ptr [MOX::_player]                     = 00000000
        SHL        ECX,0x2
        LEA        EAX,[EAX + ECX*0x8]
        LEA        ECX,[EAX + EAX*0x8]
        ADD        ECX,EDX
 
* Output only the cleaned-up code (no extra explanation).
"""

GEMINI_MODEL = "gemini-3-flash-preview"
GEMINI_URL_TEMPLATE = "https://generativelanguage.googleapis.com/v1beta/models/{model}:generateContent"


def _get_selected_or_current_function():
    func = None
    try:
        func = currentFunction
    except Exception:
        func = None

    if func is None:
        addr = currentAddress
        if addr is None:
            return None
        func = currentProgram.getFunctionManager().getFunctionContaining(addr)
    return func


# ---------------------------
# Namespace extraction + sanitization
# ---------------------------

_CPP_KEYWORDS = {
    # common C/C++ keywords (not exhaustive, but enough to avoid obvious collisions)
    "alignas","alignof","and","and_eq","asm","auto","bitand","bitor","bool","break","case","catch","char","char8_t",
    "char16_t","char32_t","class","compl","concept","const","consteval","constexpr","constinit","const_cast","continue",
    "co_await","co_return","co_yield","decltype","default","delete","do","double","dynamic_cast","else","enum","explicit",
    "export","extern","false","float","for","friend","goto","if","inline","int","long","mutable","namespace","new","noexcept",
    "not","not_eq","nullptr","operator","or","or_eq","private","protected","public","register","reinterpret_cast","requires",
    "return","short","signed","sizeof","static","static_assert","static_cast","struct","switch","template","this","thread_local",
    "throw","true","try","typedef","typeid","typename","union","unsigned","using","virtual","void","volatile","wchar_t","while",
    "xor","xor_eq",
}

_ident_re = re.compile(r"[^0-9A-Za-z_]+")

def _sanitize_cpp_ident(name: str) -> str:
    """
    Convert an arbitrary Ghidra namespace/symbol segment into a valid C++ identifier:
    - Replace invalid chars with '_'
    - If starts with digit, prefix '_'
    - Avoid empty segments
    - Avoid obvious keyword collisions by appending '_'
    """
    if name is None:
        return ""
    s = name.strip()
    if not s:
        return ""
    s = _ident_re.sub("_", s)
    s = s.strip("_")
    if not s:
        s = "ns"
    if s[0].isdigit():
        s = "_" + s
    if s in _CPP_KEYWORDS:
        s = s + "_"
    return s

def _get_function_namespace_cpp(func) -> str:
    """
    Returns a sanitized C++ namespace path like 'A::B::C' derived from Ghidra's parent namespace chain.
    Returns '' if function is in the global namespace.
    """
    try:
        ns = func.getParentNamespace()
    except Exception:
        return ""

    # Walk up until global
    parts = []
    cur = ns
    while cur is not None:
        try:
            name = cur.getName()
        except Exception:
            break

        if name is None:
            break

        # Ghidra typically uses 'global' for the root namespace name.
        # Some programs may use 'Global' or similar.
        if name.lower() == "global":
            break

        san = _sanitize_cpp_ident(name)
        if san:
            parts.append(san)

        try:
            cur = cur.getParentNamespace()
        except Exception:
            break

    parts.reverse()
    return "::".join(parts)


# ---------------------------
# Disassembly formatting
# ---------------------------

_CODE_UNIT_FMT = None

def _get_code_unit_format():
    global _CODE_UNIT_FMT
    if _CODE_UNIT_FMT is None:
        _CODE_UNIT_FMT = CodeUnitFormat(CodeUnitFormatOptions())
    return _CODE_UNIT_FMT

def _format_ins_human(ins) -> str:
    fmt = _get_code_unit_format()
    return fmt.getRepresentationString(ins)

def _format_disassembly(func, ns_cpp: str):
    listing = currentProgram.getListing()
    lines = []
    lines.append(";; Function: %s @ %s" % (func.getName(), func.getEntryPoint()))
    if ns_cpp:
        lines.append(";; Namespace (C++): %s" % ns_cpp)
    else:
        lines.append(";; Namespace (C++): <global>")
    lines.append(";; Disassembly:")

    instr_iter = listing.getInstructions(func.getBody(), True)
    for ins in instr_iter:
        addr = ins.getAddress()
        raw = ins.toString()
        human = _format_ins_human(ins)
        lines.append("%s  %s  ; %s" % (addr, raw, human))

    return "\n".join(lines)


def _get_namespace_cpp_from_namespace(ns) -> str:
    """
    Returns a sanitized C++ namespace path like 'A::B::C' from a Ghidra Namespace object.
    Returns '' if global.
    """
    if ns is None:
        return ""
    parts = []
    cur = ns
    while cur is not None:
        try:
            name = cur.getName()
        except Exception:
            break
        if not name or name.lower() == "global":
            break
        san = _sanitize_cpp_ident(name)
        if san:
            parts.append(san)
        try:
            cur = cur.getParentNamespace()
        except Exception:
            break
    parts.reverse()
    return "::".join(parts)


def _get_symbol_fullname_cpp(sym) -> str:
    """
    Returns 'A::B::name' (sanitized namespaces) or just 'name' if global.
    """
    try:
        ns_cpp = _get_namespace_cpp_from_namespace(sym.getParentNamespace())
    except Exception:
        ns_cpp = ""
    try:
        name = sym.getName()
    except Exception:
        name = "<noname>"
    return (ns_cpp + "::" + name) if ns_cpp else name


def _collect_referenced_global_data_symbols(func):
    """
    Heuristically collects global/external *data* symbols referenced by the function body.
    Skips:
      - code refs (refs that land on an instruction)
      - function entry points (refs that land on a Function)
      - labels scoped under the function namespace
    Returns list of tuples: (to_addr, symbol_or_None, dataType_or_None)
    """
    listing = currentProgram.getListing()
    rm = currentProgram.getReferenceManager()
    symtab = currentProgram.getSymbolTable()
    fm = currentProgram.getFunctionManager()

    by_addr = {}

    instr_iter = listing.getInstructions(func.getBody(), True)
    for ins in instr_iter:
        from_addr = ins.getAddress()
        try:
            refs = rm.getReferencesFrom(from_addr)
        except Exception:
            continue

        for ref in refs:
            try:
                to = ref.getToAddress()
            except Exception:
                continue
            if to is None or (not to.isMemoryAddress()):
                continue

            # Skip refs into code
            if listing.getInstructionAt(to) is not None:
                continue

            # Skip refs to function entry points (calls/jmps)
            if fm.getFunctionAt(to) is not None:
                continue

            sym = None
            try:
                sym = symtab.getPrimarySymbol(to)
            except Exception:
                sym = None

            # Skip local labels scoped under this function
            if sym is not None:
                try:
                    if sym.getParentNamespace() == func:
                        continue
                except Exception:
                    pass

            dt = None
            try:
                data = listing.getDataAt(to)
                if (data is None) or (not data.isDefined()):
                    # Handle references into the middle of a defined array/struct
                    data = listing.getDataContaining(to)
                if data is not None and data.isDefined():
                    dt = data.getDataType()
            except Exception:
                dt = None

            by_addr[int(to.getOffset())] = (to, sym, dt)

    return [by_addr[k] for k in sorted(by_addr.keys())]


def _is_complex_datatype(dt) -> bool:
    """
    Complex = struct/union (Composite) or Enum, including typedefs that wrap those.
    """
    if dt is None:
        return False
    try:
        if isinstance(dt, Composite) or isinstance(dt, Enum):
            return True
        if isinstance(dt, TypeDef):
            base = dt.getDataType()
            return isinstance(base, Composite) or isinstance(base, Enum)
    except Exception:
        return False
    return False


def _collect_complex_datatypes_recursive(dt, out_set, seen_set):
    """
    Collect Composite/Enum/TypeDef reachable under dt.
    Ensures typedef-wrapped Composite/Enum also pulls in the base definition.
    Handles pointers/arrays/typedef chains.
    """
    if dt is None:
        return

    try:
        uid = dt.getUniversalID()
    except Exception:
        uid = None

    try:
        path = dt.getPathName()
    except Exception:
        path = str(dt)

    key = (path, str(uid))
    if key in seen_set:
        return
    seen_set.add(key)

    # If it's a typedef, also pull in the base definition (if complex)
    try:
        if isinstance(dt, TypeDef):
            base = dt.getDataType()
            if base is not None:
                # include typedef itself
                out_set.add(dt)
                # include base if it's complex, and keep walking
                if isinstance(base, Composite) or isinstance(base, Enum):
                    out_set.add(base)
                _collect_complex_datatypes_recursive(base, out_set, seen_set)
                return
    except Exception:
        pass

    if _is_complex_datatype(dt):
        out_set.add(dt)

    try:
        if isinstance(dt, Pointer):
            _collect_complex_datatypes_recursive(dt.getDataType(), out_set, seen_set)
            return
        if isinstance(dt, Array):
            _collect_complex_datatypes_recursive(dt.getDataType(), out_set, seen_set)
            return
    except Exception:
        pass



def _render_datatypes_as_c(datatypes):
    """
    Render ANSI-C-ish pseudocode for complex datatypes using Ghidra DataTypeWriter.
    This is the reliable way to get full struct/enum bodies and typedefs.
    """
    if not datatypes:
        return ""

    sw = StringWriter()
    try:
        dtm = currentProgram.getDataTypeManager()
        # Third arg typically controls whether to emit dependencies as needed
        writer = DataTypeWriter(dtm, sw, True)

        # DataTypeWriter wants a Java List
        jlist = ArrayList()
        # stable order helps prompt diffs
        def _key(dt):
            try:
                return dt.getPathName()
            except Exception:
                return str(dt)

        for dt in sorted(datatypes, key=_key):
            jlist.add(dt)

        writer.write(jlist, monitor)
        return sw.toString()
    except Exception as e:
        return "/* DataTypeWriter failed: %s */\n" % (e,)


def _format_externals_for_prompt(func):
    """
    Builds a prompt section describing referenced globals + their types, plus C pseudocode
    for complex types.
    """
    refs = _collect_referenced_global_data_symbols(func)
    if not refs:
        return "=== EXTERNALS / GLOBALS ===\n<none>\n"

    lines = []
    lines.append("=== EXTERNALS / GLOBALS ===")
    lines.append(";; Referenced global/external data symbols (address + Ghidra type)")

    complex_set = set()
    seen = set()

    # cap to keep prompt sane
    MAX_ITEMS = 64
    for (to, sym, dt) in refs[:MAX_ITEMS]:
        try:
            addr_hex = "0x%08X" % int(to.getOffset())
        except Exception:
            addr_hex = str(to)

        name = "<no_symbol>"
        if sym is not None:
            name = _get_symbol_fullname_cpp(sym)

        type_str = "<unknown>"
        if dt is not None:
            try:
                type_str = dt.getDisplayName()
            except Exception:
                type_str = str(dt)

        lines.append("- %s @ %s : %s" % (name, addr_hex, type_str))

        if dt is not None:
            _collect_complex_datatypes_recursive(dt, complex_set, seen)

    if complex_set:
        lines.append("")
        lines.append("=== TYPE DEFINITIONS (C-ish, from Ghidra DataType.getDefinition) ===")
        c_defs = _render_datatypes_as_c(list(complex_set))

        MAX_CHARS = 12000
        if len(c_defs) > MAX_CHARS:
            c_defs = c_defs[:MAX_CHARS] + "\n/* ... truncated ... */\n"

        lines.append(c_defs.strip() if c_defs else "<none>")

    return "\n".join(lines) + "\n"


def _get_decompiler_output(func, timeout_seconds=60):
    decomp = DecompInterface()
    decomp.openProgram(currentProgram)

    res = decomp.decompileFunction(func, timeout_seconds, monitor)
    if not res.decompileCompleted():
        raise RuntimeError("Decompilation failed or timed out: %s" % res.getErrorMessage())

    df = res.getDecompiledFunction()
    if df is None:
        raise RuntimeError("Decompiler returned no DecompiledFunction.")
    return df.getC()


def _gemini_call(api_key, system_prompt, user_text, model=GEMINI_MODEL):
    url = GEMINI_URL_TEMPLATE.format(model=model)

    payload = {
        "system_instruction": {"parts": [{"text": system_prompt}]},
        "contents": [{"role": "user", "parts": [{"text": user_text}]}],
        # NOTE: No generationConfig.maxOutputTokens -> no explicit token limit set
    }

    data = json.dumps(payload).encode("utf-8")
    req = urllib.request.Request(
        url,
        data=data,
        method="POST",
        headers={
            "Content-Type": "application/json",
            "x-goog-api-key": api_key,
        },
    )

    ctx = ssl.create_default_context()

    try:
        with urllib.request.urlopen(req, context=ctx) as resp:
            raw = resp.read().decode("utf-8", errors="replace")
            obj = json.loads(raw)
    except urllib.error.HTTPError as e:
        body = e.read().decode("utf-8", errors="replace") if hasattr(e, "read") else ""

        # Some models/setups reject system_instruction in REST.
        # Retry by folding system prompt into the user text.
        if "system_instruction" in body or "systemInstruction" in body:
            payload.pop("system_instruction", None)
            payload["contents"][0]["parts"][0]["text"] = (
                "SYSTEM INSTRUCTION:\n" + system_prompt + "\n\n" + user_text
            )

            data2 = json.dumps(payload).encode("utf-8")
            req2 = urllib.request.Request(
                url,
                data=data2,
                method="POST",
                headers={
                    "Content-Type": "application/json",
                    "x-goog-api-key": api_key,
                },
            )

            with urllib.request.urlopen(req2, context=ctx) as resp2:
                raw2 = resp2.read().decode("utf-8", errors="replace")
                obj = json.loads(raw2)
        else:
            raise RuntimeError("Gemini HTTPError %s: %s" % (getattr(e, "code", "???"), body))
    except urllib.error.URLError as e:
        raise RuntimeError("Gemini URLError: %s" % (e,))

    candidates = obj.get("candidates") or []
    if not candidates:
        return json.dumps(obj, indent=2)

    content = (candidates[0].get("content") or {})
    parts = content.get("parts") or []

    texts = []
    for p in parts:
        t = p.get("text")
        if t:
            texts.append(t)

    if texts:
        return "\n".join(texts).strip()

    return json.dumps(obj, indent=2)


def main():
    api_key = os.getenv("GEMINI_API_KEY")
    if not api_key:
        api_key = askString("Gemini API Key", "GEMINI_API_KEY not set. Paste your Gemini API key:")
        if not api_key:
            printerr("No API key provided. Aborting.")
            return

    func = _get_selected_or_current_function()
    if func is None:
        printerr("No function selected and no function at cursor.")
        return

    ns_cpp = _get_function_namespace_cpp(func)

    monitor.setMessage("Collecting disassembly...")
    disasm = _format_disassembly(func, ns_cpp=ns_cpp)

    monitor.setMessage("Decompiling function...")
    decomp_c = _get_decompiler_output(func, timeout_seconds=60)

    monitor.setMessage("Collecting externals/globals + types...")
    externals_section = _format_externals_for_prompt(func)

    user_text = (
        "=== NAMESPACE CONTEXT ===\n"
        f"Function namespace (C++): {ns_cpp if ns_cpp else '<global>'}\n\n"
        f"{externals_section}\n"
        "=== DISASSEMBLY ===\n"
        f"{disasm}\n\n"
        "=== GHIDRA DECOMPILER OUTPUT ===\n"
        f"{decomp_c}\n"
    )

    #print(user_text)

    monitor.setMessage("Calling Gemini (%s)..." % GEMINI_MODEL)
    cleaned = _gemini_call(
        api_key=api_key,
        system_prompt=SYSTEM_PROMPT,
        user_text=user_text,
        model=GEMINI_MODEL,
    )

    print("\n" + "=" * 80)
    if ns_cpp:
        print("GEMINI CLEANED-UP CODE (%s) for %s::%s @ %s" % (GEMINI_MODEL, ns_cpp, func.getName(), func.getEntryPoint()))
    else:
        print("GEMINI CLEANED-UP CODE (%s) for %s @ %s" % (GEMINI_MODEL, func.getName(), func.getEntryPoint()))
    print("=" * 80 + "\n")
    print(cleaned)
    print("\n" + "=" * 80 + "\n")


main()
