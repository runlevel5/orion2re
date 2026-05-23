---
name: grpc
description: Use this skill when you need to inspect or analyze the currently open Ghidra program through the Adhoc PyGhidra RPC server, with read-only defaults and reusable request patterns.
---

# Ghidra RPC skill

Use this skill whenever you need to inspect the currently open Ghidra program through the Adhoc PyGhidra RPC server.

The RPC server executes arbitrary Python code inside the running PyGhidra process, so snippets can use normal Python plus the available Ghidra APIs and any helpers exposed by the server.

## When to use this skill

Use this skill for tasks such as:

- checking whether the RPC server is reachable
- discovering the currently open program and cursor state
- reading live server docs and helper availability
- decompiling or disassembling code through Ghidra
- reading memory, cross-references, symbols, or function metadata
- creating labels or comments only when mutation is explicitly justified

Ask for the port and bearer token if they are not already provided.

## Connectivity

Base URL form:

```text
http://<host>:<port>
```

If the client is running inside WSL and Ghidra is running on Windows, use the Windows gateway host instead of `127.0.0.1`:

```sh
WIN_HOST=$(ip route show default | awk '{print $3}')
BASE_URL="http://$WIN_HOST:<port>"
```

Otherwise use the local server address directly:

```text
http://127.0.0.1:<port>
```

Every request requires:

```text
Authorization: Bearer <token>
```

## Required behavior

Before making any RPC call, show a short preamble stating what you are trying to check or inspect.

Default to read-only access.

Use `write=false` unless there is strong evidence that mutation is required and appropriate for the task.

Do not send JSON unless the server documentation explicitly requires it. The `/exec` endpoint expects raw Python source in the HTTP request body.

Prefer the built-in `web_fetch` tool if it is available in the current environment. If it is not available, use `curl` through the shell.

When recovering jump tables, pointer tables, or individual code/data pointers, resolve each recovered address to an existing symbol in the current project instead of leaving it as a raw address.

If a recovered target does not have a corresponding local project symbol, treat that as an error and report it explicitly rather than guessing, inventing a name, or silently returning the address.

When reporting recovered targets, prefer the existing symbol name and include the raw address only as supporting detail.

## Core endpoints

### Check server status

Use this first when you need to confirm connectivity or identify the current program.

```sh
curl -H "Authorization: Bearer <token>" \
  "<base-url>/status"
```

### Read live server documentation

Use this when helper availability or endpoint details might have changed.

```sh
curl -H "Authorization: Bearer <token>" \
  "<base-url>/docs"
```

### Execute read-only Python inside Ghidra

Use raw Python in the request body and keep `write=false` by default.

```sh
cat script.py | curl \
  -H "Authorization: Bearer <token>" \
  -H "Content-Type: text/plain" \
  --data-binary @- \
  "<base-url>/exec?mode=exec&write=false&fresh=false&timeout_s=300"
```

### Stop the server

Only use this when the task explicitly calls for stopping the RPC server.

```sh
curl -X POST \
  -H "Authorization: Bearer <token>" \
  "<base-url>/shutdown"
```

## Safe execution defaults

For `/exec`, start with these query parameters unless the task requires something else:

```text
mode=exec&write=false&fresh=false&timeout_s=300
```

Guidance:

- `mode=exec`: use for normal multi-line snippets
- `write=false`: default for analysis and inspection
- `fresh=false`: preserve snippet globals unless you specifically need a clean environment
- `timeout_s=300`: aligns with the live server docs for longer decompilation or analysis calls

Important timeout rule: a timed-out request may continue running on the Ghidra side. Do not assume the underlying Python execution was canceled.

## Short helper examples

The exact helper set comes from `/docs`, but common examples include:

- `program()`, `here()`, `A(...)`
- `curfunc()`, `func_at(...)`, `func_containing(...)`
- `xrefs_to(...)`, `xrefs_from(...)`
- `bytes_at(...)`, `u8(...)`, `u16(...)`, `u32(...)`
- `decomp(...)`, `disasm(...)`
- `label(...)`, `tx(...)`

Treat this as a convenience layer, not a full API boundary. The server can run any Python you send inside PyGhidra, with access to normal Ghidra APIs.

## Reusable snippet patterns

### Current function

```python
print(curfunc())
```

### Decompile current function

```python
result = decomp()
print(result)
```

### Decompile a specific address

```python
result = decomp(0x401000)
print(result)
```

### Disassemble from an address

```python
print(disasm(0x401000, 20))
```

### Read bytes from memory

```python
print(bytes_at(0x401000, 16))
```

### Inspect cross-references

```python
addr = A("0x401000")
print(xrefs_to(addr))
print(xrefs_from(addr))
```

### Find a function and decompile it

```python
f = func_at(0x401000)
print(f)
print(decomp(f))
```

## Mutation rules

Only mutate the program when the task explicitly requires it, for example adding a label or comment.

When mutating:

- explain why mutation is needed
- switch to `write=true`
- keep the snippet minimal and targeted
- prefer one focused change at a time

Example:

```python
label(0x401000, "My_Function_Name")
```

Use it with:

```text
/exec?mode=exec&write=true&fresh=false&timeout_s=300
```

## Response handling guidance

When using this skill:

- summarize the intent before the request
- keep snippets small and inspectable
- prefer printing structured intermediate values for clarity
- when inspecting jump tables or pointers, validate that each recovered target maps to an existing local symbol before returning results
- if any recovered target lacks a local symbol, surface it as an error instead of returning unresolved addresses
- if the server times out, note that the remote snippet may still be running
- if connectivity fails, verify the host, port, and bearer token first

## Quick copy-paste templates

### Inline read-only exec

```sh
cat <<'PY' | curl \
  -H "Authorization: Bearer <token>" \
  -H "Content-Type: text/plain" \
  --data-binary @- \
  "<base-url>/exec?mode=exec&write=false&fresh=false&timeout_s=300"
print(curfunc())
print(disasm(here(), 10))
PY
```

### Inline write exec

```sh
cat <<'PY' | curl \
  -H "Authorization: Bearer <token>" \
  -H "Content-Type: text/plain" \
  --data-binary @- \
  "<base-url>/exec?mode=exec&write=true&fresh=false&timeout_s=300"
label(0x401000, "My_Function_Name")
PY
```