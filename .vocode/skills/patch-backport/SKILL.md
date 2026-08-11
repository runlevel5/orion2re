---
name: patch-backport
description: Analyze and semantically backport one MOO2 1.50 asm.pl patch into the restored source tree using the matching DOS orion2.exe in Ghidra, with user approval before applying changes.
---

# MOO2 1.50 patch backport skill

Use this skill to backport one change from `C:\Projects\MOO2150\src\asm.pl` into the current restored codebase.

The goal is not to translate patched machine code literally. Recover the defect, intended behavior, and relevant invariants, then implement the clearest semantic fix in the restored source.

Every proposed implementation must also evaluate whether the restored architecture permits a simpler, safer, or more maintainable source-level solution than the original patch mechanism. Do not introduce helpers, public APIs, persistent state, or binary-shaped control flow solely to resemble or test the assembly patch when an existing source abstraction can enforce the same invariant directly.

Use the `grpc` skill for all Ghidra access and follow its read-only and assembly-validation rules.

## Helper-agent delegation

Use of `agent-helper` is optional. Delegate bounded, read-only evidence collection only when it reduces parent-agent
work; there is no requirement to invoke it in any phase or at every affected location. Suitable tasks include:

- locating and inventorying the selected topic in `docs/patches.md`, `asm.pl`, `patch.c`, symbols, and history
- calculating candidate file-offset-to-address mappings from patcher section metadata
- collecting Ghidra program metadata, bytes, disassembly, cross-references, callers, callees, and decompiler output
- locating semantic equivalents, style requirements, configuration definitions, generated-code workflows, and tests
- comparing evidence or triaging build and test failures

Every delegation must request one directly scoped evidence-collection task. State the exact files, line ranges,
symbols, addresses, or supplied output the helper may inspect; the specific evidence it must return; what it must not
inspect or decide; and the condition at which it must stop. Do not delegate the patch topic as a whole, combine
dependent workflow phases, issue overlapping requests, or ask the helper to choose its own investigation scope.
Complete and validate prerequisite phases in the parent before delegating later-phase evidence collection.

For Ghidra tasks, give the helper the RPC host, port, bearer token, exact addresses or symbols, and the evidence to
return. Require it to follow the `grpc` skill, keep every `/exec` request at `write=false`, and validate decompiler
claims against disassembly. The helper may use shell `curl` to reach Ghidra and may inspect the MOO2150 files
outside the current repository, but it must not modify files, mutate the Ghidra program, or call `/shutdown`.

Keep each delegation narrow and request file paths, line numbers, addresses, bytes or instructions, uncertainties,
and unresolved mismatches in the report. Factual evidence returned within the delegated scope may be trusted without
repeating the same collection. The parent agent must still reconcile conflicting or incomplete evidence and remains
solely responsible for understanding and validating the complete 1.50 change, drawing semantic conclusions, deciding
stop conditions, reading complete files before proposing diffs, selecting the implementation, obtaining approval,
applying changes, and validating the result. Never adopt a helper's interpretation, design choice, or final decision
without parent-agent validation.

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
- Compilation is allowed and required for validation. After implementation and any required parser-code generation, build the project and run the applicable tests using the repository's canonical native Linux Debug test profile.

### Required Linux Debug test profile

Before proposing validation commands, read and analyze the repository's build-profile definitions, including
`CMakePresets.json` and any directly referenced build/test scripts. Identify the native Linux Debug profile that
enables tests and record its configure, build, and test presets, generator, binary directory, sanitizer settings,
and test-output behavior. Do not choose an ad hoc build directory or a different preset for individual runs.

For the current repository, the canonical profile is `linux-debug-asan-tests`. Use it consistently for generation,
focused builds and tests, the complete build, and the complete test suite:

```bash
cmake --preset linux-debug-asan-tests
cmake --build --preset linux-debug-asan-tests
ctest --preset linux-debug-asan-tests
```

Focused validation may add `--target` to the build command and `-R` to the test command, but must retain the same
configure, build, and test preset. If the profile definition changes, follow the profile proven from the current
repository rather than copying stale settings from this document.

Run only native Linux builds from Linux. Do not configure, build, or use MinGW, mingw64, Windows cross-compilation,
Wine, or another platform's profile as validation for a backport. A cross-platform build requested separately by the
user is outside this skill's validation workflow and cannot replace the native Linux Debug build and tests.

Never pass `-j`, `--jobs`, or a numeric `--parallel` limit to a compile command. Let the selected build tool use all
available cores. A repository script's unrestricted `--parallel` is acceptable, but the canonical preset workflow is
preferred.

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

Also locate `vars.pl` in the MOO2150 tree and search it for variables related to the topic. Search by more than exact
references from the selected `asm.pl` block: include the topic words and abbreviations, affected functions and
symbols, patched offsets and nearby immediates/data, subsystem terminology, and names discovered in comments,
configuration, and history. A variable can be an inseparable part of the topic even when the selected patch block
does not name it directly.

If `vars.pl` is absent, search the complete MOO2150 tree for it and record the absence explicitly. Then perform the
same topic-based variable inventory in the available variable and configuration registries, including `conf.pl`,
`mtype.pl`, generated configuration declarations, and related non-Lua C definitions. Absence of `vars.pl` is not
permission to skip variable discovery or to conclude that the topic has no configuration.

Record:

- exact topic name and `asm.pl` line
- all patch blocks belonging to the topic
- original locations and symbols
- expected original chunks (`cchunk`)
- replacement chunks (`pchunk`)
- added code/data and diversion targets
- every variable and configuration variable related to the selected topic, including definitions found in `vars.pl`
  or its available replacements; record its external and internal names, type, default value, limits, parser metadata,
  storage or patch offsets, synchronization/save behavior, and every reference or binary write site
- referenced `src/c/patch.c` functions
- related later C fixes or history that materially change the final behavior

Do not stop at the first patch block. The complete topic can contain multiple edits, helper procedures, data changes, and call-site diversions.

Do not conclude that no variables exist merely because `asm.pl` has no explicit variable reference. Reconcile every
patched branch destination and configurable immediate/data location with the variable inventory. If a patch only
separates previously shared storage or control flow, determine whether a variable registry supplies the value at the
new destination before reconstructing the resulting behavior.

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

The parent agent must personally understand or validate the complete 1.50 change before using it to determine
migration status, recommend an implementation, invoke a stop condition, or present conclusions. Helper-collected
facts may support this validation, but a helper's semantic reconstruction or recommendation is not a substitute for it.

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

Before designing or proposing source changes, read the repository's style documentation under `docs/`, including `docs/styleguide.md` when present. Treat those conventions as implementation requirements for source, tests, generated-code handling, naming, visibility, and organization.

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

Before selecting an implementation, compare at least these source-level options where applicable:

- a direct fix at the owner of the violated invariant
- reuse of an existing helper, state transition, or subsystem abstraction
- deriving state from authoritative data instead of adding duplicated persistent state
- updating state at the event that invalidates it instead of checking or repairing it in a hot path
- keeping logic file-local instead of adding a public helper used only by tests
- removing an obsolete faulty mechanism instead of reproducing its patched control flow

Choose the smallest option that fully preserves the proven behavior, including all required configuration, AI, UI, network, save, and compatibility effects. Reject an apparently simpler option if it bypasses existing consumers or changes observable behavior. Document the alternatives considered and why the selected design is better for the restored source.

If 1.50 introduces configuration variables as part of the selected topic, restore every one of them through the project's configuration-variable definitions. Do not omit a variable merely because the main behavioral fix can be approximated without it. Preserve its proven name, type, default value, parser metadata, and use sites. If a configuration variable is genuinely unnecessary because the restored architecture represents the same setting elsewhere, treat that as a material implementation difference and obtain explicit approval during pre-apply review.

Identify suitable existing tests. Add a regression test when the project already has an applicable test pattern and the defect can be reproduced deterministically.

Read and analyze the canonical native Linux Debug test profile before selecting any validation command. All focused
and complete builds and tests proposed for the backport must use that one profile; do not mix an existing ad hoc build
directory, a release profile, or a cross-compilation profile into the workflow.

### 6. Mandatory pre-apply review

Do not change any project file yet, including `docs/patches.md`.

Present the user with:

1. A concise analysis summary.
2. A plain-language statement of the actual bug and what the patch fixes, including the trigger and incorrect result. If the topic is not a bug fix, identify it as a behavior or feature change and explain that change instead.
3. The verified original and patched behavior.
4. Migration status in the restored source.
5. A source-level design comparison describing the faithful patch-shaped approach, any simpler alternatives, and why the recommended option best fits the restored architecture.
6. A file-by-file implementation plan.
7. Focused suggested diffs for every proposed source, test, and documentation edit.
8. When configuration variables will be added, the repository-defined CMake command or target that will regenerate the C++ parser, plus the expected generated outputs.
9. Assumptions, compatibility effects, and validation commands.

Validation commands must use the proven native Linux Debug test presets. Do not propose MinGW or other
cross-platform builds, and do not add compile job limits.

Before the suggested diffs, include two short implementation paragraphs:

- **Original patch implementation:** state concretely what the selected `asm.pl` edits do, including saved/restored state, changed instructions, wrappers, diversions, and added code or data that matter to the behavior.
- **Proposed source implementation:** compare the viable source-level designs, including whether state can be derived, an existing abstraction can be reused, cleanup can occur at the invalidating event, or logic can remain local. Explain why the recommended design is simpler, safer, or more maintainable without losing any proven behavior. When it differs from the original binary implementation, explain how it enforces the same invariant and why the binary mechanism is unnecessary or inappropriate. If the patch-shaped approach is still best, say why explicitly.

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
- run formatting and static checks
- configure, build, and run focused tests with the canonical native Linux Debug test presets
- build the complete project and run the complete test suite with those same presets
- never substitute a MinGW or other cross-platform build, and never pass `-j`, `--jobs`, or a numeric parallel limit

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
- run applicable static checks and formatting
- confirm the canonical native Linux Debug test profile was used consistently for configure, focused validation, the
  complete build, and the complete test suite
- build the project and run the relevant tests; tests are required validation and must not be deferred solely because they compile code
- confirm no cross-platform build was used as validation and no compile command limited the available build threads
- verify the catalog entry describes the implemented fix accurately
- confirm no unrelated files changed

Report:

- the actual bug and what the patch fixes, or the behavior or feature change when the topic is not a bug fix
- the restored semantic fix
- static checks, build, and tests run, including their results
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
- [ ] `vars.pl` located and searched by topic, symbol, function, and offset, or its repository-wide absence recorded and equivalent variable/configuration registries searched instead.
- [ ] Original locations and bytes/instructions verified.
- [ ] Patched behavior reconstructed, including C implementation refinements.
- [ ] The parent agent understood or validated the complete 1.50 semantic change before drawing conclusions.
- [ ] Every topic-related variable and configuration variable inventoried even when not explicitly referenced by `asm.pl`, with proven names, types, defaults, limits, parser metadata, synchronization behavior, storage or patch offsets, and use sites.
- [ ] Current semantic source path analyzed.
- [ ] Existing migration status determined.
- [ ] Plan and focused suggested diffs shown before modification.
- [ ] Explicit user approval received.
- [ ] Approved source and tests implemented.
- [ ] If configuration variables were added, CMake used to regenerate the C++ parser code and generated changes reviewed.
- [ ] Canonical native Linux Debug test profile read, analyzed, and used for all configure, build, and test commands.
- [ ] No MinGW or other cross-platform build was used as validation, and no compile command used `-j`, `--jobs`, or a numeric parallel limit.
- [ ] Static checks, full native Linux Debug project build, focused and complete tests, and `git diff --check` passed.
- [ ] Matching `docs/patches.md` item completed with a short semantic descriptor.