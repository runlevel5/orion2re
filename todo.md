# Configuration and mod system implementation plan

This plan turns the design in `docs/config.md` into incremental, testable work. No implementation should begin until the decisions in Step 1 are reviewed.

## Proposed repository layout

```text
specs/
  config/
    main.json5                 Canonical configuration schema entry point
  mod/
    manifest.json5             Mod-manifest schema added with the mod loader

tools/
  config/                      Build-time schema validation and generation tools
    config_tool.cpp            Single C++ command-line entry point
    json5_reader.cpp/.h        Project adapter around vendored yyjson
    schema_model.cpp/.h        JSON5 loading, normalization, and validation
    generate_cpp.cpp/.h        C++ schema and binding generator
    generate_docs.cpp/.h       Reference documentation generator

src/
  config/                      Runtime parser, traversal, transactions, and loading
    generated/                 Checked-in generated C++ files; never hand-edited
  game/                        Gameplay-owned state and semantic use sites

tests/
  unit/                        Runtime C++ tests using the existing test setup
  data/config/                 Schema, CFG, manifest, and diagnostic fixtures

<build>/generated-check/       Temporary outputs used only for drift verification
```

Build-time tools belong in `tools/config/`, not `src/`, because they consume specifications and produce source or documentation without being linked into the game. They are C++ executables built and tested through CMake, using the same compiler and project conventions as the rest of the repository; no Python or other scripting runtime is required. Generated C++ is committed under `src/config/generated/`, and normal builds compile it directly without building or running the generator. Runtime configuration code belongs in `src/config/`. Native settings remain owned by the relevant game subsystem rather than being collected into an unrelated generated global structure.

## Step 1: approve foundation decisions

- [x] Confirm `specs/config/main.json5` as the single canonical schema entry point.
- [x] Confirm `tools/config/` for build-time validation and generation, and `src/config/` for runtime code.
- [x] Start `main.json5` with only a few existing primitive scalar variables. Do not bootstrap records, arrays, enums, masks, ratios, or imported 1.50 catalogs.
- [x] Keep `main.json5` as one file while it remains manageable; define deterministic splitting and inclusion rules only when migration growth requires them.
- [x] Use a pinned yyjson release as the JSON5 parser. Vendor its single `.c` and `.h` implementation, link it only to `config-tool`, and do not add JsonCpp or `mortie/json5cpp`.
- [x] Implement all schema validators, generators, documentation helpers, and related build tools in C++ and integrate them directly with CMake; do not add a Python dependency.
- [x] Apply `docs/styleguide.md` to runtime code, tools, generated files, and tests: C-with-namespaces organization, project naming conventions, and no project-owned template declarations.
- [x] Commit generated C++ under `src/config/generated/`; normal builds must not run the generator. Regeneration is explicit, and CI checks committed files for drift.
- [x] Use an opt-in `add_custom_command(OUTPUT ...)` rule for regeneration. Declare `main.json5` and generator implementation files as dependencies; do not use `PRE_BUILD`, `POST_BUILD`, or an always-run custom command.
- [x] Decide whether generated user documentation is committed under `docs/` or produced only by an explicit documentation target.
- [x] Define stable field identity policy. Prefer explicit reviewed IDs derived from canonical paths, with collision and change checks, rather than declaration-order IDs.
- [x] Resolve the remaining syntax decisions from `docs/config.md`: quoted escapes, optional includes, trailing aggregate values, array ranges, regular-expression dialect, string-length units, interface synchronization, and deterministic resource hashing.

Deliverable: a short decision record appended to this file, with every choice required by the first vertical slice settled.

### Foundation decision record

- `specs/config/main.json5` is the single canonical schema entry point and remains one file until schema growth requires a reviewed inclusion design.
- Build-time code lives under `tools/config/`; runtime code lives under `src/config/`; checked-in generated C++ lives under `src/config/generated/`.
- Generated reference documentation is written only to the build directory by the explicit `config-docs` target. It is not committed under `docs/`.
- Generated in-memory schema IDs are derived from canonical paths. They are not serialization identities and may change when a canonical path changes.
- JSON5 source is valid UTF-8. Logical game strings are restricted to text representable in CP850, with limits measured in encoded bytes. Unicode-only text is rejected because the game cannot render it.
- JSON5 schema inclusion and CFG array-range selectors are out of scope. CFG `include` statements are supported; file resolution, cycle detection, and transactional loading are implemented in Step 10. Individual array indexes and whole-array selection remain supported by the later traversal phase.
- Aggregate and scalar-start assignments may end before their traversal boundary. They update only the supplied suffix values; excess values beyond the boundary remain errors.
- Regular-expression constraints are out of scope. Enums use exact symbolic validation, and strings use explicit encoding, byte-length, and approved named-format checks only when required.
- Interface settings remain local by default. Multiplayer configuration sharing, mod-list exchange, and resource identity comparison are deferred to the networking phase.
- Deterministic resource identity hashes are deferred with native mod loading and multiplayer support. That phase must define normalized mounted paths, file-content hashes, package IDs and versions, and deterministic mount order before peers exchange and compare identities.

## Step 2: define the schema contract and bootstrap specification

- [x] Write the top-level contract for types, records, groups, and parameters without a schema-version field.
- [x] Define built-in explicit-width primitives: `bool`, signed and unsigned 8/16/32-bit integers, and `string`.
- [ ] Define normalized node representations for aliases, enums, masks, ratios, records, arrays, inline types, parameters, metadata, constraints, defaults, groups, and bindings.
- [x] Specify declaration-order preservation as part of the public traversal contract.
- [x] Specify required metadata and the inheritance/ambiguity rules for `title`, `description`, `remarks`, `units`, and `examples`.
- [x] Specify structured native bindings for the bootstrap's root header, symbol, C++ type, and direct member paths. Replicated targets, fixed arrays, and index-base conversion remain deferred with aggregate types.
- [x] Create `specs/config/main.json5` with a few verified existing scalar variables using only primitive types, scalar defaults, metadata, groups, constraints, and direct native bindings.
- [x] Choose bootstrap variables that are already understood in restored source and can be consumed at clear semantic use sites; do not add speculative values for the sake of exercising schema features.
- [x] Defer records, arrays, enums, masks, ratios, catalogs, aggregate defaults, and sequential aggregate traversal until the migration reaches variables that require them.

Deliverable: a minimal canonical schema for basic scalar variables that contains no executable expressions, offsets, scripts, native initializer strings, or prematurely modeled 1.50 catalogs.

## Step 3: implement the build-time schema model and validator

- [x] Add a CMake-built `config-tool validate specs/config/main.json5` C++ executable as the stable command-line interface.
- [x] Keep parsing, normalized schema data, validation, C++ generation, and documentation generation in reusable C++ components behind the command-line entry point.
- [x] Implement a small yyjson adapter that reads the source buffer with only approved JSON5 flags, preserves object order, rejects duplicate keys, enforces a project depth limit during traversal, and produces deterministic errors.
- [x] Convert yyjson syntax-error byte offsets to file/line/column with `yyjson_locate_pos`; report semantic validation failures with file and canonical schema path because successful yyjson values do not retain source offsets.
- [x] Keep invalid-Unicode support disabled and reject `NaN`/`Infinity`; do not use the full `YYJSON_READ_JSON5` convenience flag when it enables syntax outside the schema contract.
- [x] Do not expose `yyjson_doc` or `yyjson_val` outside the build-tool adapter and schema model.
- [ ] Normalize named and inline types into one internal model without losing declaration order.
- [ ] Resolve references and reject duplicate names, unknown types, recursive value layouts, invalid array sizes, and unsupported schema shapes.
- [ ] Validate aliases and field constraints so fields may narrow but never broaden referenced types.
- [ ] Validate enum base types, numeric ranges, duplicate values, and unknown-numeric policy.
- [ ] Validate mask values, independent bits, composites, unknown bits, and canonical declaration order.
- [ ] Normalize and validate ratios, string/path constraints, record defaults, exact-array defaults, repeated element defaults, and complete default coverage.
- [ ] Reject overlapping aggregate and descendant defaults and defaults that fail the same type rules as CFG input.
- [x] Validate metadata requirements, group references, and canonical-path in-memory identities for the primitive bootstrap.
- [x] Validate binding structure without pretending to resolve C++ members; compiler-backed binding checks are added in Step 7.
- [x] Emit deterministic diagnostics with file and schema path, plus line and column for syntax errors.

Deliverable: malformed schemas fail before C++ generation, and repeated validation produces byte-for-byte stable normalized output.

## Step 4: add generator tests and build integration

- [x] Add focused fixtures for every accepted bootstrap schema shape and representative validation failures under `tests/data/config/schema/`.
- [x] Test the C++ schema model and generators with the repository's existing C++ test framework; keep command-line integration tests focused on process results and generated files.
- [x] Add opt-in CMake support for building the native `config-tool`; keep it out of the dependency graph for normal game and test targets.
- [x] Add explicit `config-schema-validate`, `config-regenerate`, `config-generated-check`, and `config-docs` targets.
- [x] Implement `config-regenerate` with a stamped `add_custom_command`, an explicit complete `BYPRODUCTS` list, `COMMAND config-tool generate`, and `DEPENDS specs/config/main.json5` plus all generator/parser sources and headers.
- [x] Put the custom command behind an explicit `config-regenerate` target that depends on its declared outputs; do not add that target to `ALL` or as a dependency of game/test targets.
- [x] Rely on the selected CMake backend's timestamp/dependency tracking so `cmake --build <build> --target config-regenerate` does nothing when every output is present and no declared input is newer.
- [x] Generate output only when bytes differ, preventing unchanged content from triggering recompilation.
- [x] List generated outputs explicitly rather than using a glob, and update the CMake list in the same change whenever generator output topology changes.
- [x] Make `config-generated-check` write to `<build>/generated-check/`, compare byte-for-byte with committed outputs, and fail on missing, stale, extra, or nondeterministic files.
- [ ] Run `config-generated-check` in CI after relevant JSON5, generator, or CMake changes.
- [ ] Compile checked-in generated C++ normally during cross-compilation. Regeneration requires a separate native host build of `config-tool` and never executes a target binary on the build host.

Deliverable: normal and cross-compiled builds use checked-in generated C++ without running generators, while explicit validation and CI detect stale or nondeterministic outputs.

## Step 5: generate schema IDs, descriptors, defaults, and documentation

- [x] Generate canonical-path C++ identifiers and typed descriptors for the initial schema slice.
- [ ] Generate leaf order, parent relationships, index metadata, writable status, and stable serialization identity needed by path resolution.
- [x] Generate normalized typed scalar defaults without native expression strings.
- [x] Generate declaration-ordered tables for canonical parameter names and groups; enum and mask tables remain deferred with those types.
- [x] Generate Markdown containing every bootstrap parameter, path, type, constraint, default, group, unit, example, and native binding.
- [x] Keep generated file banners and ordering deterministic, and identify the canonical input without introducing a schema version.
- [x] Decide and implement the approved documentation publication policy from Step 1.

Deliverable: the initial basic variables have inspectable generated C++ metadata and complete generated reference documentation.

## Step 6: implement the standalone CFG lexical parser

- [x] Add runtime value and source-location types under `src/config/`.
- [x] Parse semicolon-terminated assignment, `include`, `enable`, and `stop` statements without game-specific names.
- [x] Support comments, whitespace-separated paths, quoted strings and JSON escapes, bare identifiers, decimal/hex/binary integers, ratios, comma-separated masks, and explicit skip tokens.
- [x] Preserve raw path and RHS tokens plus file, line, column, and byte-offset information.
- [x] Define and test malformed-number boundaries so a lexical error is not misreported as a schema error.
- [x] Add unit tests for valid syntax, invalid escapes, unterminated strings/statements, missing RHS values, comments, directives, and source locations.

Deliverable: parser tests do not depend on the game schema or native bindings.

## Step 7: generate path resolution, bounded traversal, and typed decode

Bootstrap scalar progress:

- [x] Generate exact canonical-name lookup for the initial root-scalar parameters.
- [x] Resolve primitive root-scalar assignments and reject selectors on scalar parameters.
- [x] Decode logical booleans and decimal, hexadecimal, and binary integers using generated primitive types and field constraints.
- [x] Enforce the one-leaf scalar boundary, reject overflow and skips-only assignments, and clear temporary writes on every failure.
- [x] Produce typed temporary scalar writes containing the canonical-path identity and source provenance.
- [x] Add focused tests for lookup, type mismatch, range validation, scalar overflow, unknown paths, provenance, and atomic failure.
- [ ] Extend resolution, traversal, and typed decoding to records, arrays, aliases, enums, masks, ratios, strings, and aggregate skips when those schema shapes are introduced.

- [ ] Resolve raw path tokens through records, arrays, and any approved keyed collections.
- [ ] Enforce index bases and produce a canonical resolved path.
- [ ] Return the selected node, first writable leaf, nearest valid traversal boundary, and remaining writable leaves.
- [ ] Implement depth-first declaration-order traversal exactly as specified in `docs/config.md`.
- [ ] Implement scalar-root, scalar-record-field, scalar-array-element, record, array, row, and whole-table boundary rules.
- [ ] Decode every RHS token against its current leaf, including enum names/numbers, masks, ratios, integers, strings, and skips.
- [ ] Reject overflow before writes, and reject statements containing only skips.
- [ ] Produce an immutable temporary write set containing stable field identity, resolved indexes, typed value, and provenance.
- [ ] Add exhaustive table-driven tests for scalar starts, records, arrays, nested arrays, partial updates, skips, boundaries, and overflow diagnostics.

Deliverable: complete statements resolve and validate atomically without accessing live game state.

## Step 8: generate typed native bindings for the basic variables

- [x] Generate includes and typed accessors from declared binding roots and member paths.
- [x] Generate direct one-to-one writes and allowed logical-to-native conversions for the initial scalar variables; add replicated writes and fixed-array indexing later when migration requires them.
- [x] Emit static assertions for bootstrap root/member compatibility; fixed array extents remain deferred until arrays are introduced.
- [x] Compile generated bindings against the declared project headers so missing symbols, bad members, and incompatible types fail the build.
- [x] Apply writes to temporary typed state or a transaction overlay, never through byte offsets or untyped pointer arithmetic.
- [ ] Add cross-field validator registration for rules that require the complete candidate state.
- [x] Commit only after all bootstrap writes succeed; verify candidate and native rollback behavior with tests.
- [x] Commit the typed bootstrap state to the existing audio setting fields already consumed at their gameplay semantic use sites.

Deliverable: a few basic existing variables can load defaults and CFG overrides transactionally through generated typed bindings.

## Step 9: add configuration lifecycle, groups, extraction, and provenance

- [ ] Define group policies for save participation, multiplayer synchronization, and derived-state refresh when those services are implemented. Do not introduce mutability phases or reload-timing policy without concrete subsystem requirements.
- [x] Initialize candidate state from generated logical defaults before applying base or mod configuration.
- [ ] Add lifecycle services that rebuild derived caches only after successful commit when a migrated setting requires a derived cache.
- [x] Track source package, file, line, statement, and prior-value provenance per effective bootstrap leaf.
- [x] Implement canonical extraction for the primitive bootstrap with stable schema order and optional provenance comments. Symbolic values and bounded aggregate assignments remain deferred until those schema types exist.
- [x] Add round-trip tests: defaults/overrides -> canonical CFG -> equivalent effective state.

Bootstrap lifecycle progress:

- [x] Decode a complete CFG document into temporary typed state and commit effective and native state only after every applicable statement succeeds.
- [x] Make `stop;` end the current document and leave `include` and `enable` execution to their dedicated loader steps.
- [x] Preserve effective state, native state, current provenance, and prior-value history when any document statement fails.

Deliverable: effective configuration is explainable, extractable, reload-controlled, and deterministic.

## Step 10: implement includes

- [ ] Resolve include paths relative to the current file/package using the approved VFS path policy.
- [ ] Maintain include stacks and reject cycles with the complete chain in diagnostics.
- [ ] Make `stop;` return only from the current file.
- [ ] Implement optional includes only if Step 1 approves explicit syntax.
- [ ] Apply an included configuration as part of the caller's transaction; any failure leaves prior live state unchanged.
- [ ] Add tests for nested includes, cycles, missing versus malformed files, `stop;`, path normalization, and rollback.

Deliverable: multi-file base configuration works without introducing package activation semantics.

## Step 11: define and generate mod-manifest support

- [ ] Add `specs/mod/manifest.json5` only after the core configuration vertical slice is stable.
- [ ] Define manifest fields for stable ID, display name, semantic version, supported engine range, dependencies, optional dependencies, conflicts, ordering, config entry point, resources, and capabilities.
- [ ] Reuse the build-time JSON5 model and diagnostics where practical, while keeping manifest and variable schemas distinct.
- [ ] Generate or implement a typed manifest reader with strict path, identifier, and semantic-version constraints.
- [ ] Add fixtures for compatible, incompatible, malformed, cyclic, conflicting, and ordered package graphs.

Deliverable: tools and runtime can inspect package identity and requirements without executing CFG files.

## Step 12: implement atomic mod activation and VFS layering

- [ ] Discover manifests by stable ID and reject ambiguous packages.
- [ ] Resolve version constraints, dependencies, optional dependencies, conflicts, cycles, and deterministic load ordering.
- [ ] Prepare resource mounts without exposing them to live VFS consumers.
- [ ] Parse package CFG entry points and recursively resolve `enable` requests in the same activation transaction.
- [ ] Validate required resources and define deterministic collision/override policy.
- [ ] Commit resource mounts, configuration writes, provenance, and enabled-package state together.
- [ ] Add integration tests proving that any manifest, dependency, CFG, validation, or mount failure rolls back the entire activation.

Deliverable: `enable <mod-id>;` activates a complete package atomically rather than acting as an include alias.

## Step 13: add save-game and multiplayer representations

- [ ] Define save and network wire-format versioning independent of JSON5 source layout, CFG syntax, C++ memory layout, and declaration position; this versions serialized formats, not the configuration schema.
- [ ] Store effective deterministic fields by stable identity plus mod IDs/versions, resource identity, and engine versions.
- [ ] Define optional-field skipping, missing-field defaults, and explicit migrations.
- [ ] Restore configuration before dependent save sections.
- [ ] Define canonical host synchronization for gameplay groups and deterministic resources while excluding approved local interface settings.
- [ ] Define multiplayer exchange and comparison of enabled mod lists, package versions, deterministic resource hashes, mount order, and synchronized configuration groups.
- [ ] Add compatibility fixtures and mismatch diagnostics for save/wire formats, packages, resources, and effective configuration differences.

Deliverable: deterministic configuration survives saves and can be compared and synchronized across peers.

## Step 14: expand schema coverage subsystem by subsystem

- [ ] Treat migration and exposure of existing variables as an ongoing project, selecting one cohesive gameplay subsystem at a time until community 1.50 configuration parity is achieved.
- [ ] Add records, arrays, enums, masks, ratios, aggregate defaults, and richer traversal only when a migrated subsystem needs them.
- [ ] Replace weak integer IDs with verified enums as restoration establishes finite catalogs.
- [ ] Preserve published names, numeric identities, stable field IDs, and declaration order; classify every change as additive, behavioral, or breaking.
- [ ] Use Git history and review to track schema changes; migrate repository-owned CFG files in the same change when paths, types, or traversal order change.
- [ ] Add domain validators learned from restored gameplay rules.
- [ ] Track coverage and unresolved native bindings in generated reports.
- [ ] Evaluate scripting separately only after declarative configuration and package loading are stable.

Deliverable: configuration coverage grows through small reviewed vertical slices without weakening type safety or compatibility.

## Completion criteria

- [ ] Clean native and cross-compiled builds validate the canonical schema and deterministically generate required target sources.
- [ ] Generated C++ compiles against all declared bindings with no offsets, arbitrary expressions, or untyped writes.
- [ ] Parser, traversal, schema, transaction, include, manifest, VFS, save, and network tests cover success and rollback paths.
- [ ] Every writable leaf has exactly one valid effective default and a stable identity.
- [ ] Generated documentation is complete without relying on JSON5 comments.
- [ ] Canonical extraction round-trips to the same deterministic effective configuration.
- [ ] Failed statements, files, includes, or package activations never partially modify live configuration or resource mounts.
