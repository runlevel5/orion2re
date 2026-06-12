---
name: asan-symbolize
description: Use this WSL skill to recover symbolic names and source file lines from a Windows ASAN log using the matching EXE and PDB from the same directory.
---

# ASAN symbolization skill

Use this skill when you have:

- a Windows ASAN text log
- the matching `.exe`
- the matching `.pdb`

This is a WSL-oriented skill: the agent runs in WSL, but the symbolization target files and `llvm-symbolizer.exe` live on the Windows side.

The goal is to turn stack frames like:

```text
(C:\path\app.exe+0x47cfd6)
```

into:

```text
FunctionName
C:\path\file.cpp:123:0
```

## Verified workflow for this project

For the verified case:

- log: `C:\Projects\moo2debug\2\asan_20260610_223646.1544`
- exe: `C:\Projects\moo2debug\2\orion2re.exe`
- pdb: `C:\Projects\moo2debug\2\orion2re.pdb`

Important: for this binary and ASAN log, use the `orion2re.exe+0x...` values exactly as they appear in the log with `llvm-symbolizer --obj=...exe`.

Do not use `--relative-address` for this case.

## When to use this skill

Use this skill for tasks such as:

- resolving every stack frame in an ASAN log to symbol and source line
- producing reproducible symbolization instructions
- verifying that a given ASAN log matches the EXE and PDB in the same directory
- mapping the crashing frame and thread-creation frames back to source

## Required behavior

Before symbolizing:

- identify the ASAN log path
- identify the matching `.exe` and `.pdb` in the same directory
- extract stack offsets only from frames belonging to the target executable
- preserve the exact `0x...` offsets from the log

When reporting results:

- include the ASAN log line numbers where the offsets came from
- include resolved function names
- include resolved source file names and line numbers
- call out any frames that belong to other modules such as ASAN runtime or Windows system DLLs

## Address extraction rule

From an ASAN frame like:

```text
#0 0x00a4cfd6  (C:\Games\MOO2-131\orion2re.exe+0x47cfd6)
```

extract:

```text
0x47cfd6
```

Do not use the left-hand PC value unless you have separately verified a tool wants that form.

## Preferred tool

Use Visual Studio's `llvm-symbolizer.exe` if present.

Typical locations:

```text
C:\Program Files\Microsoft Visual Studio\<ver>\<edition>\VC\Tools\MSVC\<toolset>\bin\Hostx64\x64\llvm-symbolizer.exe
C:\Program Files\Microsoft Visual Studio\<ver>\<edition>\VC\Tools\MSVC\<toolset>\bin\Hostx64\x86\llvm-symbolizer.exe
```

## Reproducible command pattern

```bat
"C:\Path\To\llvm-symbolizer.exe" --obj=C:\path\app.exe 0x47cfd6 0x47e2e2 0x53b369
```

For the verified project case:

```bat
"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\llvm-symbolizer.exe" --obj=C:\Projects\moo2debug\2\orion2re.exe 0x47cfd6 0x47e2e2 0x53b369 0x51f665 0x540a39 0x52a9b8 0x52d45e 0x523c76 0x6abc39 0x6ab8d4 0x679c63 0x67a479 0x67a587 0x7b56fe 0x7e10ec 0x7e114b 0xa5ceb7 0xa5d4d5 0x7e0cf0 0x7b4d46 0x7b4beb 0x73ff8f 0x6c965a 0x73bb01 0x8cf5f5 0x7fa72e 0x756a20 0x74c30b 0x73b8b2 0xa0141c 0xa01329 0xa011cc 0xa01487
```

## Verified frame locations for the current log

Offsets came from these exact log lines in `C:\Projects\moo2debug\2\asan_20260610_223646.1544`:

- lines 4-20 for the crashing thread's `orion2re.exe` frames
- lines 61-76 for the thread-creation stack's `orion2re.exe` frames
- line 26 for the ASAN-reported global location

Resolved examples from the verified run:

- `0x47cfd6` -> `CMBTDRW1::Draw_Main_Combat_Screen_(void)` -> `C:\Projects\orion2re\orion2re\src\game\cmbtdrw1.cpp:647:0`
- `0x47e2e2` -> `CMBTDRW1::Refresh_Combat_Screen_Full_(void)` -> `C:\Projects\orion2re\orion2re\src\game\cmbtdrw1.cpp:1142:0`
- `0x53b369` -> `COMBINIT::Scroll_Combat_Screen_(void)` -> `C:\Projects\orion2re\orion2re\src\game\combinit.cpp:2310:0`
- `0x51f665` -> `COMBAT1::Do_Combat_Turn_(short, short)` -> `C:\Projects\orion2re\orion2re\src\game\combat1.cpp:539:0`
- `0xa5d4d5` -> `_beginthreadex` -> `minkernel\crts\ucrt\src\appcrt\startup\thread.cpp:209:0`
- line 26 of the ASAN log already reports `MOX::_combat_grid` at `mox.cpp:165:12`

## Validation checklist

After symbolization, verify that:

- every `app.exe+0x...` frame from the target module resolved to a function and `file:line`
- resolved source paths are plausible for the build
- unrelated module frames remain attributed to ASAN runtime or Windows DLLs
- the first crashing frame matches the ASAN summary module offset

## Common pitfall

If `llvm-symbolizer` returns obviously wrong SDL, CRT, or unrelated symbols for the first frame, retry by changing address mode assumptions.

For this verified `orion2re.exe` case, the correct mode was:

```text
--obj=<exe> <orion2re.exe+0x... offsets from the log>
```

and the incorrect mode was:

```text
--obj=<exe> --relative-address <offsets>
```