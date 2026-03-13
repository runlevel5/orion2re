#@author
#@category AI
#@runtime PyGhidra
#@keybinding
#@menupath Tools.AI.Clean up selected function (HTTP POST)
#@toolbar

"""
Ghidra 12.x (PyGhidra / Python 3) script:
- Gets selected/current function
- Extracts:
  1) function disassembly
     - left side: "raw" instruction text (ins.toString()) which often includes absolute addresses
     - comment: human-readable version with variable/symbol names (Listing-like formatting)
  2) Ghidra decompiler output
  3) externals/globals + type definitions (via DataTypeWriter)
- Sends a single HTTP POST to http://127.0.0.1:5050/input with JSON payload:
    { "text": "<whole input>" }
- Prints HTTP status + response body to the Ghidra console (response is expected to be "OK")
"""

import json
import re
import urllib.request
import urllib.error

from ghidra.app.decompiler import DecompInterface
from ghidra.program.model.listing import CodeUnitFormat, CodeUnitFormatOptions
from ghidra.program.model.data import DataTypeWriter, Pointer, Array, TypeDef, Composite, Enum
from java.io import StringWriter
from java.util import ArrayList
from ghidra.app.decompiler import DecompInterface, DecompileOptions
from ghidra import GhidraOptions

POST_URL = "http://127.0.0.1:5050/input"


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
    try:
        ns = func.getParentNamespace()
    except Exception:
        return ""

    parts = []
    cur = ns
    while cur is not None:
        try:
            name = cur.getName()
        except Exception:
            break

        if name is None:
            break
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
        opts = CodeUnitFormatOptions(
            CodeUnitFormatOptions.ShowBlockName.NEVER,
            CodeUnitFormatOptions.ShowNamespace.ALWAYS
        )
        _CODE_UNIT_FMT = CodeUnitFormat(opts)
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


# ---------------------------
# Externals/globals + datatype definitions
# ---------------------------

def _get_namespace_cpp_from_namespace(ns) -> str:
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
    try:
        ns_cpp = _get_namespace_cpp_from_namespace(sym.getParentNamespace())
    except Exception:
        ns_cpp = ""
    try:
        name = sym.getName()
    except Exception:
        name = "<noname>"
    return (ns_cpp + "::" + name) if ns_cpp else name


def _escape_string_for_comment(s: str, max_len: int = 200) -> str:
    """
    Escape a string so it is safe to embed in a single-line comment as: ; "..."
    """
    if s is None:
        return ""
    try:
        s = str(s)
    except Exception:
        return ""
    s = s.replace("\r", "\\r").replace("\n", "\\n").replace("\t", "\\t")
    # Escape backslash first, then quotes
    s = s.replace("\\", "\\\\").replace('"', '\\"')
    if len(s) > max_len:
        s = s[:max_len] + "..."
    return s


def _try_get_string_at_address(addr):
    """
    If 'addr' resolves to a defined string in the Listing, return its value as Python str.
    Otherwise return None.
    """
    listing = currentProgram.getListing()
    try:
        data = listing.getDataAt(addr)
        if (data is None) or (not data.isDefined()):
            data = listing.getDataContaining(addr)
        if data is None or (not data.isDefined()):
            return None

        if hasattr(data, "hasStringValue") and data.hasStringValue():
            v = data.getValue()
            if v is None:
                return None
            return str(v)
    except Exception:
        return None
    return None


def _collect_referenced_global_data_symbols(func):
    listing = currentProgram.getListing()
    rm = currentProgram.getReferenceManager()
    symtab = currentProgram.getSymbolTable()
    fm = currentProgram.getFunctionManager()

    # map int(addr)->(to, sym, dt, str_value_or_none)
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

            # Skip code references
            if listing.getInstructionAt(to) is not None:
                continue
            if fm.getFunctionAt(to) is not None:
                continue

            sym = None
            try:
                sym = symtab.getPrimarySymbol(to)
            except Exception:
                sym = None

            # Skip symbols local to the function namespace (rare)
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

            s_val = _try_get_string_at_address(to)

            by_addr[int(to.getOffset())] = (to, sym, dt, s_val)

    return [by_addr[k] for k in sorted(by_addr.keys())]


def _is_complex_datatype(dt) -> bool:
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

    try:
        if isinstance(dt, TypeDef):
            base = dt.getDataType()
            if base is not None:
                out_set.add(dt)
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
    if not datatypes:
        return ""

    sw = StringWriter()
    try:
        dtm = currentProgram.getDataTypeManager()
        writer = DataTypeWriter(dtm, sw, True)

        jlist = ArrayList()

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


def _render_composite_layouts(datatypes):
    """
    Emit a separate block with byte offsets for Composite types (struct/union).
    """
    comps = []
    for dt in datatypes:
        try:
            if isinstance(dt, TypeDef):
                dt = dt.getDataType()
            if isinstance(dt, Composite):
                comps.append(dt)
        except Exception:
            pass

    if not comps:
        return ""

    def _dt_name(dt):
        try:
            return dt.getPathName()
        except Exception:
            try:
                return dt.getName()
            except Exception:
                return str(dt)

    lines = []
    lines.append("=== STRUCT LAYOUT (OFFSETS) ===")

    for comp in sorted(comps, key=_dt_name):
        try:
            comp_name = _dt_name(comp)
            comp_size = comp.getLength()
            is_union = False
            try:
                is_union = comp.isUnion()
            except Exception:
                # some versions expose union-ness via category/name; ignore if unavailable
                pass

            lines.append("")
            lines.append(";; %s  (size=%d / 0x%X)%s" % (
                comp_name, comp_size, comp_size, " [union]" if is_union else ""
            ))

            try:
                components = comp.getComponents()
            except Exception:
                # fallback: build list by index
                components = [comp.getComponent(i) for i in range(comp.getNumComponents())]

            for c in components:
                try:
                    off = c.getOffset()
                except Exception:
                    off = 0
                try:
                    ln = c.getLength()
                except Exception:
                    ln = -1
                try:
                    fdt = c.getDataType()
                    fdt_name = fdt.getDisplayName()
                except Exception:
                    fdt_name = "<unknown>"
                try:
                    fname = c.getFieldName() or c.getName()
                except Exception:
                    fname = None
                if not fname:
                    fname = "<unnamed>"

                bit_suffix = ""
                try:
                    if hasattr(c, "isBitField") and c.isBitField():
                        bit_suffix = "  (bit %d..%d)" % (
                            c.getBitOffset(),
                            c.getBitOffset() + c.getBitSize() - 1
                        )
                except Exception:
                    pass

                # format: 0x0010 (+4) field : type
                if ln >= 0:
                    lines.append("  - 0x%04X (+%d)  %s : %s%s" % (off, ln, fname, fdt_name, bit_suffix))
                else:
                    lines.append("  - 0x%04X         %s : %s%s" % (off, fname, fdt_name, bit_suffix))

        except Exception as e:
            lines.append("")
            lines.append(";; <error rendering composite>: %s" % (e,))

    return "\n".join(lines) + "\n"


def _format_externals_for_input(func):
    refs = _collect_referenced_global_data_symbols(func)
    if not refs:
        return "=== EXTERNALS / GLOBALS ===\n<none>\n"

    lines = []
    lines.append("=== EXTERNALS / GLOBALS ===")
    lines.append(";; Referenced global/external data symbols (address + Ghidra type)")

    complex_set = set()
    seen = set()

    MAX_ITEMS = 64
    for (to, sym, dt, s_val) in refs[:MAX_ITEMS]:
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

        comment = ""
        if s_val:
            comment = ' ; "%s"' % _escape_string_for_comment(s_val)

        lines.append("- %s @ %s : %s%s" % (name, addr_hex, type_str, comment))

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

        lines.append("")
        lines.append(_render_composite_layouts(list(complex_set)).strip())

    return "\n".join(lines) + "\n"


def _get_decompiler_output(func, timeout_seconds=60):
    decomp = DecompInterface()

    opts = DecompileOptions()
    tool = state.getTool()
    if tool is not None:
        field_opts = tool.getOptions(GhidraOptions.CATEGORY_BROWSER_FIELDS)
        decomp_opts = tool.getOptions(GhidraOptions.CATEGORY_DECOMPILER_POPUPS)
        opts.grabFromToolAndProgram(field_opts, decomp_opts, currentProgram)
    else:
        opts.grabFromProgram(currentProgram)

    decomp.setOptions(opts)
    decomp.openProgram(currentProgram)

    res = decomp.decompileFunction(func, timeout_seconds, monitor)
    if not res.decompileCompleted():
        raise RuntimeError("Decompilation failed or timed out: %s" % res.getErrorMessage())

    df = res.getDecompiledFunction()
    if df is None:
        raise RuntimeError("Decompiler returned no DecompiledFunction.")
    return df.getC()


# ---------------------------
# HTTP POST
# ---------------------------

def _http_post_text(url: str, text: str) -> str:
    payload = {"text": text}
    data = json.dumps(payload).encode("utf-8")

    req = urllib.request.Request(
        url,
        data=data,
        method="POST",
        headers={"Content-Type": "application/json"},
    )

    try:
        with urllib.request.urlopen(req) as resp:
            status = getattr(resp, "status", None) or resp.getcode()
            body = resp.read().decode("utf-8", errors="replace")
            print("HTTP %s" % status)
            return body
    except urllib.error.HTTPError as e:
        body = ""
        try:
            body = e.read().decode("utf-8", errors="replace")
        except Exception:
            pass
        raise RuntimeError("HTTPError %s: %s" % (getattr(e, "code", "???"), body))
    except urllib.error.URLError as e:
        raise RuntimeError("URLError: %s" % (e,))


def main():
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
    externals_section = _format_externals_for_input(func)

    user_text = (
        "=== NAMESPACE CONTEXT ===\n"
        f"Function namespace (C++): {ns_cpp if ns_cpp else '<global>'}\n\n"
        f"{externals_section}\n"
        "=== DISASSEMBLY ===\n"
        f"{disasm}\n\n"
        "=== GHIDRA DECOMPILER OUTPUT ===\n"
        f"{decomp_c}\n"
    )

    monitor.setMessage("HTTP POST to localhost...")
    resp_text = _http_post_text(POST_URL, user_text)

    print("\n" + "=" * 80)
    if ns_cpp:
        print("POSTED INPUT for %s::%s @ %s" % (ns_cpp, func.getName(), func.getEntryPoint()))
    else:
        print("POSTED INPUT for %s @ %s" % (func.getName(), func.getEntryPoint()))
    print("=" * 80 + "\n")

    # print what the server returned (expected "OK")
    print(resp_text.strip())


main()
