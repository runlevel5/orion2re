---
name: patch-backport
description: Analyze and semantically backport one MOO2 1.50 asm.pl patch into the restored source tree using the matching DOS orion2.exe in Ghidra, with user approval before applying changes.
---

# MOO2 1.50 patch backport skill

Use this skill to backport one change from `C:\Projects\MOO2150\src\asm.pl` into the current restored codebase.

The goal is not to translate patched machine code literally. Recover the defect, intended behavior, and relevant invariants, then implement the clearest semantic fix in the restored source.

Use the `grpc` skill for all Ghidra access and follow its read-only and assembly-validation rules.

## Required inputs

Before analysis, obtain:

1. The specific MOO2 1.50 change to backport:
   - preferably an exact `docs/patches.md` topic name
   - otherwise a description, `asm.pl` line, function, address, or patch name

For Orion2, this workflow runs in WSL. Unless the user supplies different Ghidra RPC connection information, use the Windows gateway, port `47633`, and bearer token `None`. Test the default connection with:

```bash
WIN_HOST=$(ip route show default | awk '{print $3}')
curl -H "Authorization: Bearer None" http://$WIN_HOST:47633/status
```

If the specific patch is missing, ask the user for it before proceeding. Do not guess the desired patch. If the default Ghidra connection fails, report the failure and ask the user for alternate host, port, or token information.

Treat connection information already supplied in the conversation as valid input. If only host is omitted, explain that WSL commonly needs the Windows gateway rather than `127.0.0.1`, as documented by the `grpc` skill.

## Environment constraints

- The Orion2 binary open in Ghidra is a slightly different 1.40 version, not the exact executable used as the MOO2 1.50 patch base. It should be very close to that base, but this expected version difference must be recorded rather than treated as exact equivalence.
- Do not perform a full project build in WSL; WSL builds are too slow. CMake parser-code generation is a required exception when configuration variables are added. After implementation, generation, and static checks, ask the user to build the project in Visual Studio and report the result.

## Scope

Include:

- original-game correctness and stability fixes
- fixes implemented directly in `asm.pl`
- fixes whose definition and hooks are in `asm.pl` but whose implementation is in `src/c/patch.c`
- later non-Lua C corrections that refine the selected `asm.pl` topic
- relevant patch history and changelog notes when they clarify intent

Exclude unless the user explicitly requests them:

- the Lua engine and Lua API implementation
- scripting support
- unrelated new features
- broad configuration or balance changes outside the selected topic
- 1.50-specific infrastructure that the restored codebase does not use

Stay within one selected topic unless analysis proves that another topic is inseparable. Ask before expanding scope.

## Source authority

Use these sources in this order:

1. Original DOS `orion2.exe` disassembly and bytes in Ghidra.
2. The selected `asm.pl` topic, including each `patch`, `patch_add`, `patch_divert`, original `cchunk`, replacement `pchunk`, comments, and symbols.
3. Non-Lua C implementation in MOO2150, especially `src/c/patch.c`.
4. MOO2150 history and changelog as supporting evidence.
5. The current restored source tree as the integration target.

Decompiler output is only a hint. Validate behavior against disassembly, control flow, loads, stores, call setup, extension instructions, and original bytes.

The patcher's `off` value may be an executable file offset rather than a loaded Ghidra address. Never use it as a Ghidra address without proving the mapping. Resolve it through program memory-block source information, known symbols, patch tooling, or a verified file-offset-to-address mapping.

### Resolving `asm.pl` offsets to Ghidra addresses

Use the patcher's section metadata whenever available. In MOO2150 this is commonly defined in `src/binary-header.pl` through `section_offset`, `section_base`, and `section_length`.

For an `asm.pl` file offset `off`:

1. Find the section whose file range contains it:

   ```text
   section_offset <= off < section_offset + section_length
   ```

2. Convert it to the loaded address:

   ```text
   address = section_base + (off - section_offset)
   ```

3. Prove the mapping rather than relying on the formula alone:
   - map at least one nearby symbol from `src/symbols.pl` to the symbol address in Ghidra
   - compare the `cchunk` bytes or instructions at the calculated address
   - confirm that the containing function and call targets match the patch description
   - inspect references to relevant globals and callees

Additional tips:

- First identify the patcher's actual base executable from `source_input_file`; do not infer it from the currently open program's filename.
- If Ghidra memory blocks expose file-byte source information, use it as independent confirmation of the mapping. XML or reconstructed imports may have no file-byte mappings, in which case patcher metadata plus symbols and bytes are required.
- Distinguish executable file offsets, section-relative offsets, loaded addresses, and runtime pointers. Document which kind each value represents.
- LE loader relocation can change loaded address operands even when the underlying code is the same. Validate relocated instructions semantically and compare stable opcode bytes or the original file when necessary.
- A version-label or digest difference is not sufficient to declare code equivalent. Require documented version equivalence or exact matches for every affected instruction sequence and its surrounding control flow.
- If more than one mapping fits, or any affected `cchunk` still fails to match, stop and ask the user instead of choosing the nearest address.

## Mandatory process

### 1. Confirm Ghidra and target program

Use the `grpc` skill to:

- call `/status`
- read `/docs` if helper behavior is unknown
- confirm the open program is the DOS `orion2.exe` 1.40 variant expected for this workflow
- record the program name, image format, language/compiler specification, and image base

Default every Ghidra request to `write=false`.

The known 1.40-versus-1.50-base version difference is expected and is not by itself a stop condition. If the program is missing, is not the expected Orion2 1.40 variant, or the server cannot be reached, stop and report the exact issue. Do not continue by assuming addresses or bytes.

### 2. Locate the authoritative patch topic

Search `docs/patches.md` and `C:\Projects\MOO2150\src\asm.pl` for the requested change.

Record:

- exact topic name and `asm.pl` line
- all patch blocks belonging to the topic
- original locations and symbols
- expected original chunks (`cchunk`)
- replacement chunks (`pchunk`)
- added code/data and diversion targets
- every configuration variable introduced by 1.50 for the selected topic, including its name, type, default value, parser metadata, and references
- referenced `src/c/patch.c` functions
- related later C fixes or history that materially change the final behavior

Do not stop at the first patch block. The complete topic can contain multiple edits, helper procedures, data changes, and call-site diversions.

If the user supplied only a description and multiple topics match, present the candidates and ask which one to use.

### 3. Verify original code in Ghidra

For every affected original location:

- resolve the patch offset to a verified Ghidra address
- read and compare the original bytes where literal bytes are available
- disassemble enough surrounding instructions to establish control flow and register state
- inspect containing functions, callers, callees, cross-references, and relevant global data
- use decompilation only alongside disassembly
- verify operand width, signedness, packed offsets, and branch conditions

If `cchunk` contains assembly rather than literal bytes, compare the expected instructions and effects rather than pretending it is a byte string.

If original bytes or instructions do not match:

- retry the file-offset/address mapping
- confirm and account for the expected 1.40-versus-1.50-base executable difference
- account for relocations only with evidence
- stop and ask the user if the mismatch remains

Never silently analyze a near match. Document any version-related differences, and proceed only when the affected sequence and surrounding control flow can still be mapped with evidence sufficient to reconstruct the same behavior.

### 4. Reconstruct the complete semantic change

Explain:

- in plain language, what the actual bug is, including its trigger and observable incorrect result
- what the patch fixes and why the patched behavior resolves that bug
- if the topic is not a bug fix, state that explicitly and explain the behavior or feature change instead
- the original behavior and defect
- the exact trigger or violated invariant
- what each 1.50 edit changes
- why all edits in the topic are needed
- which configuration variables 1.50 adds and how each participates in the fix
- whether the final MOO2150 C implementation supersedes an earlier assembly workaround
- any observable behavior, save, network, or compatibility implications

Distinguish facts proven by assembly from interpretations inferred from comments or changelog text.

### 5. Analyze the restored source tree

Locate the semantic equivalent of the original functions and data in the current project.

Follow repository read policy:

- search and summarize first
- read full files only when they will be changed or quoted
- follow explicit imports when more context is required

Determine whether the selected fix is:

- already migrated completely
- partially migrated
- absent
- obsolete because the restored implementation removed the faulty mechanism
- implemented differently but equivalently

Prefer a root fix in the restored abstraction over binary-compatible instruction mimicry. Preserve original behavior outside the selected defect. Avoid importing unrelated patch-specific configuration, globals, hooks, or code caves when ordinary source logic is sufficient.

If 1.50 introduces configuration variables as part of the selected topic, restore every one of them through the project's configuration-variable definitions. Do not omit a variable merely because the main behavioral fix can be approximated without it. Preserve its proven name, type, default value, parser metadata, and use sites. If a configuration variable is genuinely unnecessary because the restored architecture represents the same setting elsewhere, treat that as a material implementation difference and obtain explicit approval during pre-apply review.

Identify suitable existing tests. Add a regression test when the project already has an applicable test pattern and the defect can be reproduced deterministically.

### 6. Mandatory pre-apply review

Do not change any project file yet, including `docs/patches.md`.

Present the user with:

1. A concise analysis summary.
2. A plain-language statement of the actual bug and what the patch fixes, including the trigger and incorrect result. If the topic is not a bug fix, identify it as a behavior or feature change and explain that change instead.
3. The verified original and patched behavior.
4. Migration status in the restored source.
5. A file-by-file implementation plan.
6. Focused suggested diffs for every proposed source, test, and documentation edit.
7. When configuration variables will be added, the repository-defined CMake command or target that will regenerate the C++ parser, plus the expected generated outputs.
8. Assumptions, compatibility effects, and validation commands.

Before the suggested diffs, include two short implementation paragraphs:

- **Original patch implementation:** state concretely what the selected `asm.pl` edits do, including saved/restored state, changed instructions, wrappers, diversions, and added code or data that matter to the behavior.
- **Proposed source implementation:** when the source-level fix differs from the original binary implementation, explain how it enforces the same invariant in the restored code and why the binary mechanism is unnecessary or inappropriate. If it is equivalent in structure, say so explicitly.

Suggested diffs must be based on complete current file contents, not summaries. Keep them focused; do not print complete files.

End by explicitly asking the user to approve, revise, or reject the proposal.

Do not apply changes until the user clearly approves. Analysis requests, clarifying questions, or general agreement with the bug do not count as approval of the proposed diffs.

### 7. Apply the approved implementation

After approval:

- apply only the approved semantic changes
- restore every approved 1.50-added configuration variable before regenerating parser code
- after adding configuration variables, use the repository's CMake generation command or target to regenerate the C++ parser code; do not hand-edit generated parser files
- inspect the generated changes and stop if they contain unrelated output or do not reflect the restored variables
- retain existing project style and abstractions
- avoid unrelated cleanup
- add the approved regression tests
- run formatting and static checks that do not build the project
- do not run full builds or compiled tests in WSL; the required CMake parser-generation target is allowed
- ask the user to build and run relevant tests in Visual Studio, and request the result

If implementation requires a material departure from the approved diffs, stop and obtain approval for the revised proposal before continuing.

### 8. Update the migration catalog

If the selected change has an entry in `docs/patches.md`, update that entry only after the source fix is implemented and validated.

Change its checkbox to complete and add a short descriptor that states:

- where the restored fix lives
- the semantic invariant or behavior corrected
- any important difference from the 1.50 binary implementation

Example form:

```markdown
- [x] `asm.pl:1234` - `topic name`: fixed in `MODULE::Function_()` by validating the real target index before use; this replaces the 1.50 call-site guard with a shared source-level invariant.
```

If analysis proves the current code already contains an equivalent complete fix, propose a documentation-only completion descriptor during the pre-apply review and wait for approval before editing it.

If the topic is absent from `docs/patches.md`, do not invent a duplicate topic automatically. Mention the absence and propose an entry only if it is needed for catalog completeness.

### 9. Final verification and report

Before finishing:

- run `git diff --check`
- confirm every configuration variable added by 1.50 for the topic was restored with the correct definition and use sites
- when configuration variables changed, confirm CMake regenerated the C++ parser code and that the generated output is current
- run applicable non-build static checks and formatting in WSL
- ask the user to run the relevant build and tests in Visual Studio; do not build in WSL
- verify the catalog entry describes the implemented fix accurately
- confirm no unrelated files changed

Report:

- the actual bug and what the patch fixes, or the behavior or feature change when the topic is not a bug fix
- the restored semantic fix
- static checks run and the Visual Studio build/test result, or clearly state that user validation is pending
- catalog status update
- any unresolved uncertainty or follow-up topic

Do not show a final diff after applying; summarize the result concisely.

## Stop conditions

Stop and request user input when:

- Ghidra connection information is missing
- the default WSL-to-Windows Ghidra connection fails and no alternate connection information is available
- the desired 1.50 topic is missing or ambiguous
- Ghidra has the wrong or no program open
- original bytes/instructions do not match `asm.pl`
- an offset cannot be mapped reliably
- the proposed change crosses into another topic or feature
- the restored source has multiple materially different integration choices
- requested implementation differs materially from the approved proposal

Do not weaken a stop condition merely to make progress.

## Completion checklist

- [ ] Default WSL Ghidra connection verified, or alternate host/base URL, port, and bearer token obtained.
- [ ] Specific 1.50 topic selected.
- [ ] Open Ghidra program confirmed as the expected Orion2 1.40 variant and the difference from the 1.50 patch base recorded.
- [ ] Every patch block in the topic inventoried.
- [ ] Original locations and bytes/instructions verified.
- [ ] Patched behavior reconstructed, including C implementation refinements.
- [ ] Every configuration variable introduced by 1.50 for the selected topic inventoried and restored with its proven name, type, default, parser metadata, and use sites.
- [ ] Current semantic source path analyzed.
- [ ] Existing migration status determined.
- [ ] Plan and focused suggested diffs shown before modification.
- [ ] Explicit user approval received.
- [ ] Approved source and tests implemented.
- [ ] If configuration variables were added, CMake used to regenerate the C++ parser code and generated changes reviewed.
- [ ] WSL static checks and `git diff --check` passed; Visual Studio build and test results requested from the user.
- [ ] Matching `docs/patches.md` item completed with a short semantic descriptor.