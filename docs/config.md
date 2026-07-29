# Configuration and mod system design

## Purpose

This document proposes a native configuration and mod system for this project. It is influenced by the unofficial 1.50 patch, but it does not reproduce 1.50's binary patching architecture, generated memory offsets, variable names, or mod-loader behavior.

The design has two complementary formats:

- a terse, user-facing assignment language for mods and overrides
- a commented JSON5 schema that describes types, logical structure, traversal, defaults, and native bindings without embedding executable code

The assignment language preserves the strongest part of the 1.50 format: one statement can update a scalar, a record, an array element, or a larger table without repeating every field name. The JSON5 schema replaces 1.50's Perl declarations, flattened field names, binary offsets, and weakly typed bindings.

## Goals

- Preserve a compact configuration syntax suitable for hand-authored mods.
- Support scalars, records, arrays, nested arrays, enums, masks, ratios, strings, and partial updates.
- Apply RHS values sequentially from the field selected by the LHS.
- Bound sequential traversal to the aggregate explicitly selected by the LHS.
- Represent arrays as arrays instead of expanding them into names such as `weapon_1_type`.
- Use cleaned, stable external names and cleaned native bindings.
- Define finite value domains for semantic IDs, enums, and masks whenever the engine has a finite domain.
- Accept symbolic values while optionally preserving known numeric forms for compatibility.
- Generate typed C++ bindings and validation from a no-code JSON5 schema.
- Load configuration transactionally: validate the complete operation before changing live state.
- Make `enable` load a complete mod package, including dependencies, configuration, and resources.
- Integrate resource replacement through the project's VFS rather than through individual binary-era filename substitutions.
- Keep configuration deterministic and suitable for save games and multiplayer synchronization.
- Provide precise diagnostics with source file, line, path, expected type, and traversal boundary.

## Non-goals

- Reproducing 1.50's fixed executable offsets or `conf_apply` mechanism.
- Rewriting native instructions when a configuration option changes.
- Preserving 1.50's internal variable names or flattened table layouts.
- Using JSON5 itself as the normal mod override format.
- Embedding C++ expressions or executable scripts in the schema.
- Treating Lua or another scripting language as a substitute for declarative configuration.
- Implementing build-list files as part of the main configuration grammar.

## Design principles

### 1. Preserve behavior, not patch machinery

The unofficial patch is a reference for supported rules, fixes, defaults, and mod use cases. Its implementation had to modify a DOS executable, so it expresses many settings as writes to fixed offsets or as alternative instruction sequences.

The native implementation should instead use:

- typed configuration state
- restored game structures and arrays
- named C++ fields
- normal control flow at the semantic use site
- ordinary VFS mounts
- versioned save and network serialization

An original patch offset can help locate behavior in Ghidra, but it is not a native configuration binding.

### 2. Syntax and semantics are separate

The parser knows how to read statements and values. It does not know what `guardian`, `weapon`, or `plasma_cannon` mean.

The generated schema layer resolves paths, types values, determines traversal boundaries, and produces an atomic set of writes.

The generated binding layer maps those writes to typed native state.

### 3. The logical schema is not the C++ memory layout

The user-facing shape should describe game concepts naturally even when restored structures use different names or layouts.

For example, the logical schema should expose:

```text
guardian.weapon[1].type
guardian.weapon[1].arc
guardian.weapon[1].mods
guardian.weapon[1].ammo
guardian.weapon[1].amount
```

rather than preserving the 1.50 fields:

```text
weapon_1_type
weapon_1_arc
weapon_1_mods
weapon_1_ammo
weapon_1_amount
```

### 4. Sequential assignment must respect structure

Terse assignment is safe only when traversal cannot silently escape the object selected by the author.

The LHS selects both:

- the first leaf to update
- the aggregate that bounds sequential propagation

The complete statement is decoded and validated before any write is committed.

### 5. Finite domains should be explicit

If a field identifies one of a finite set of weapons, specials, buildings, drives, or other game concepts, its schema type should list those values. It should not be represented merely as an unrestricted `uint8` because the original patch did so.

Numeric compatibility and semantic validity are separate policies. A type may accept numeric IDs while still rejecting numbers that do not identify a declared value.

### 6. Mod activation is package loading

`enable <mod-id>;` is not a marker and is not only an indirect include. It requests that the loader resolve and activate a complete mod package.

Activation includes dependency resolution, conflict checks, resource mounting, configuration loading, and atomic commit.

## User-facing assignment language

### Statements

Configuration consists of semicolon-terminated statements:

```text
<path> = <value>...;
include <path>;
enable <mod-id>;
stop;
```

Comments begin with `#` and continue to the end of the line.

Examples:

```text
# Replace one complete Guardian weapon slot.
guardian weapon 1 = plasma_cannon all heavy 0 2;

# Change one field.
guardian weapon 1 mods = heavy,eccm;

# Update all Guardian specials.
guardian special = battle_scanner augmented_engines damper_field - - - - -;

# Load a complete mod package.
enable example_balance_mod;
```

### Path syntax

A path is a whitespace-separated sequence beginning with a parameter name. Subsequent selectors traverse records and arrays.

```text
guardian weapon 1 mods
building automated_factory cost
weapon plasma_cannon available_mods
```

Record fields use stable symbolic names. Arrays use explicit numeric indexes. External indexes are one-based unless a schema node explicitly declares another base.

The parser retains raw path tokens. The schema decides whether a token is:

- a record field
- an array index
- a symbolic key in a keyed collection
- a compatibility alias

### Value syntax

The language supports:

- decimal integers: `18`, `-5`
- hexadecimal integers: `0x12`
- binary integers: `0b10010`
- ratios: `1/3`
- bare identifiers: `plasma_cannon`
- masks: `heavy,eccm,overloaded`
- quoted strings
- `-` as a skip item during sequential assignment

Canonical output uses explicit `0b` for binary. A compatibility reader may accept 1.50's leading-zero binary notation, but new files should not depend on it.

### Scalar assignment

```text
combat fighters_use_point_defense = true;
```

The LHS resolves to a scalar. Exactly one non-skip RHS value is allowed.

### Record assignment

Given a weapon-slot record ordered as:

```text
type arc mods ammo amount
```

the following updates one complete record:

```text
guardian weapon 1 = plasma_cannon all heavy 0 2;
```

The selected aggregate is `guardian.weapon[1]`. The RHS cannot overflow into weapon 2.

### Partial record assignment

```text
guardian weapon 1 mods = heavy,eccm - 2;
```

Traversal starts at `mods` and continues through `ammo` and `amount`. The skip leaves `ammo` unchanged. A fourth item would be an error because it would exceed weapon 1.

### Array assignment

Selecting an array allows traversal across its elements:

```text
guardian weapon =
    plasma_cannon all heavy 0 2
    mass_driver   all point_defense,continuous 0 10;
```

The selected aggregate is the entire `weapon` array. Traversal may continue from one weapon element to the next, but it cannot continue into the following `special` field.

Similarly:

```text
guardian special = 13 1 5 25 19 21 18 3;
```

may update the complete special array.

Selecting one scalar array element does not select the array:

```text
guardian special 3 = battle_scanner;
```

Only one value is allowed because `special[3]` is scalar.

### Table assignment

Nested arrays and records use the same rules. Selecting a table permits traversal over the entire table. Selecting a row limits traversal to that row.

```text
mount_damage = 50 100 150 75 150 225;
mount_damage high_energy_focus = 75 150 225;
mount_damage high_energy_focus point_defense = 75 150 225;
```

The schema defines each aggregate's declaration order and therefore its sequential order.

### Skip items

`-` advances one leaf without changing it:

```text
mount_damage no_high_energy_focus point_defense = 50 - - 75;
```

A skip is valid only in a sequential assignment. It still counts toward the selected aggregate's boundary.

### Atomic behavior

Before applying a statement, the decoder must:

1. Resolve the complete LHS.
2. Determine the selected aggregate and first writable leaf.
3. Walk every RHS item within that boundary.
4. Type-check and range-check every non-skip item.
5. Verify that no item exceeds the boundary.
6. Produce a temporary write list.
7. Commit all writes only if every step succeeds.

There must never be a partially applied statement.

## Bounded sequential traversal

### Boundary rule

The traversal boundary is the most specific aggregate explicitly selected by the LHS.

- If the LHS resolves to a record, that record is the boundary.
- If it resolves to an array, that array is the boundary.
- If it explicitly selects an array element that is a record or array, that element is the boundary.
- If it resolves to a field inside an explicitly selected array element, that element is the boundary.
- If it resolves to a scalar array element, only that scalar is writable.
- If it resolves to a scalar field outside an array element, the containing record is the boundary, starting at that field.

Examples:

| Assignment prefix | First leaf | Boundary |
| --- | --- | --- |
| `guardian =` | `appearance` | Guardian record |
| `guardian weapon =` | `weapon[1].type` | Complete weapon array |
| `guardian weapon 1 =` | `weapon[1].type` | Weapon 1 record |
| `guardian weapon 1 mods =` | `weapon[1].mods` | Weapon 1 record |
| `guardian special =` | `special[1]` | Complete special array |
| `guardian special 3 =` | `special[3]` | Scalar element 3 |

### Leaf order

Sequential order is depth-first declaration order:

- record fields follow their order in JSON5
- array elements follow ascending external index
- nested aggregates are recursively flattened into leaves
- aliases do not affect order
- computed or read-only fields are omitted

Declaration order is therefore part of the public schema contract. Reordering fields can change the meaning of terse assignments and requires a schema compatibility decision.

### Overflow diagnostics

Overflow is an error, not an implicit transition to a sibling aggregate:

```text
guardian weapon 1 = 11 15 heavy 0 2 4;
```

Diagnostic direction:

```text
guardian weapon 1: value 6 exceeds the selected weapon record;
5 writable fields are available: type, arc, mods, ammo, amount
```

## JSON5 schema and binding format

### Why JSON5

JSON5 provides:

- comments
- trailing commas
- quoted or identifier-like object keys
- predictable scalar behavior
- straightforward parsing and code generation
- cleaner diffs than a code-based declaration system

YAML is not preferred because implicit scalar conversions and its larger syntax surface make strict schema tooling harder.

### Schema responsibilities

The JSON5 documents define:

- primitive numeric types and ranges
- enums and their stable numeric values
- masks and their stable bit values
- ratios and strings
- reusable records
- arrays and index bases
- parameter roots
- field aliases
- defaults
- declaration and traversal order
- mutability and synchronization groups
- native binding roots and member paths
- replicated writes to more than one native field
- compatibility aliases and deprecation metadata

The documents do not contain:

- executable C++ expressions
- binary offsets
- arbitrary pointer arithmetic
- embedded scripts
- instruction bytes

### Value type example

```json5
{
  types: {
    uint8: {
      kind: "integer",
      min: 0,
      max: 255,
    },

    weapon_count: {
      kind: "integer",
      min: 0,
      max: 99,
    },

    weapon_id: {
      kind: "enum",
      allow_numeric: true,
      reject_unknown_numeric: true,
      values: {
        none: 0,
        laser_cannon: 3,
        mass_driver: 4,
        plasma_cannon: 11,
      },
    },

    special_id: {
      kind: "enum",
      allow_numeric: true,
      reject_unknown_numeric: true,
      values: {
        none: 0,
        augmented_engines: 1,
        battle_scanner: 5,
        damper_field: 7,
      },
    },

    weapon_arc_mask: {
      kind: "mask",
      allow_numeric: true,
      reject_unknown_bits: true,
      flags: {
        forward: 1,
        aft: 2,
        left: 4,
        right: 8,
      },
      aliases: {
        all: ["forward", "aft", "left", "right"],
      },
    },

    weapon_mod_mask: {
      kind: "mask",
      allow_numeric: true,
      reject_unknown_bits: true,
      flags: {
        heavy: 0x0002,
        point_defense: 0x0004,
        armor_piercing: 0x0008,
        continuous: 0x0010,
        enveloping: 0x0100,
        eccm: 0x0200,
        overloaded: 0x4000,
      },
      aliases: {
        hv: ["heavy"],
        pd: ["point_defense"],
        ap: ["armor_piercing"],
        co: ["continuous"],
        env: ["enveloping"],
        ovr: ["overloaded"],
      },
    },
  },
}
```

The values above illustrate the schema shape. Imported numeric catalogs must be verified against the restored source and original binary data before becoming authoritative.

### Reusable record example

```json5
{
  records: {
    weapon_slot: {
      fields: [
        {
          id: "type",
          type: "weapon_id",
          bind: ["type"],
        },
        {
          id: "arc",
          type: "weapon_arc_mask",
          bind: ["arc"],
        },
        {
          id: "mods",
          type: "weapon_mod_mask",
          bind: ["mods"],
        },
        {
          id: "ammo",
          type: "uint8",
          bind: ["ammo"],
        },
        {
          id: "amount",
          type: "weapon_count",
          bind: ["amount", "current_amount"],
        },
      ],
    },
  },
}
```

The `amount` field demonstrates a declarative replicated write. The schema says that one logical value initializes two native fields; it does not provide code for doing so.

### Parameter and array example

```json5
{
  parameters: {
    guardian: {
      group: "gameplay",
      bind_root: "game_data.guardian",
      value: {
        kind: "record",
        fields: [
          { id: "appearance", type: "ship_appearance_id", bind: ["appearance"] },
          { id: "hull_size", type: "hull_id", bind: ["hull_size"] },
          { id: "drive", type: "drive_id", bind: ["drive"] },
          { id: "armor", type: "armor_id", bind: ["armor"] },
          { id: "shield", type: "shield_id", bind: ["shield"] },
          { id: "computer", type: "computer_id", bind: ["computer"] },

          {
            id: "weapon",
            aliases: ["weapons"],
            bind: ["weapons"],
            value: {
              kind: "array",
              count: 5,
              index_base: 1,
              element: { record: "weapon_slot" },
            },
          },

          {
            id: "special",
            aliases: ["specials"],
            bind: ["specials"],
            value: {
              kind: "array",
              count: 8,
              index_base: 1,
              element: { type: "special_id" },
            },
          },
        ],
      },
    },
  },
}
```

### Binding restrictions

Bindings are structured member paths, not arbitrary source expressions. The generator may support only a small set of declarative binding operations:

- bind one logical leaf to one native field
- bind one leaf to several native fields
- bind an array to a native fixed-size array
- apply a fixed index-base conversion
- map an enum or mask to the same underlying numeric type
- bind a parameter to project-owned configuration state consumed by gameplay code

If changing a setting requires procedural behavior, the preferred design is to store typed configuration state and let the owning C++ subsystem consult it. The schema should not invoke arbitrary setter code merely to imitate a binary patch.

Derived caches and resources may be rebuilt by explicit configuration lifecycle services after a successful transaction. Those services are engine code, not field mappings.

## Strong typing policy

### Enums

Enums declare every recognized semantic value and its stable numeric identity.

```json5
{
  kind: "enum",
  allow_numeric: true,
  reject_unknown_numeric: true,
  values: {
    none: 0,
    laser_cannon: 3,
    plasma_cannon: 11,
  },
}
```

Policies are independent:

- `allow_numeric` permits `11` in addition to `plasma_cannon`.
- `reject_unknown_numeric` rejects numeric values absent from the catalog.

New canonical configs should prefer names. Numeric IDs remain useful for imported 1.50 files, incomplete reverse engineering, and values whose names are not yet stable.

### Masks

Masks declare every known bit:

```json5
{
  kind: "mask",
  allow_numeric: true,
  reject_unknown_bits: true,
  flags: {
    heavy: 0x0002,
    point_defense: 0x0004,
  },
}
```

Symbolic masks use comma-separated names:

```text
heavy,point_defense
```

The schema may define short compatibility aliases such as `hv` and `pd`, while extraction uses canonical names.

### Integers

Integers are used only when every value in a numeric range is meaningful, or when the domain has not yet been recovered well enough to define an enum.

Semantically finite IDs should not remain generic integers permanently. Schema review should track such weak types as restoration work.

### Ratios

Ratios are represented as a numerator and denominator with explicit bounds. A zero denominator is rejected unless a particular type assigns it a documented meaning.

### Strings and paths

Strings have explicit length limits and synchronization policy. Paths are a distinct type with VFS-aware normalization and package-relative resolution.

## Names, aliases, and compatibility

### Canonical names

Canonical names should:

- describe game concepts rather than binary variables
- use consistent singular names for one selected element
- represent repeated structures as arrays
- avoid embedded indexes such as `weapon_1_type`
- remain stable once published

### Legacy aliases

1.50 names may be accepted through explicit aliases or an import tool:

```text
weapon_1_type   -> weapon 1 type
weapon_1_arc    -> weapon 1 arc
weapon_1_mods   -> weapon 1 mods
weapon_1_ammo   -> weapon 1 ammo
weapon_1_amount -> weapon 1 amount
```

Aliases do not create additional schema fields and do not affect traversal order.

Deprecated aliases should produce a diagnostic that includes the canonical replacement. Canonical extraction never emits deprecated names.

### Flat numeric positions

Original 1.50 supports flattened numeric positions such as:

```text
guardian 16 = 13 15 none 0 2;
```

This is not the same as structured array indexing and is fragile when fields are inserted or reordered.

The native format should reserve numeric selectors for actual arrays and keyed numeric collections. A dedicated 1.50 importer may decode flattened positions using the pinned 1.50 schema and rewrite them into canonical paths.

### Schema versioning

The schema has its own version independent of the executable version. A mod manifest declares the schema compatibility range it targets.

Changes are classified as:

- additive: new parameter or new trailing field with a default
- aliasing: old path remains valid through an alias
- behavioral: same syntax receives changed semantics
- breaking: field removal, field reordering, type change, or numeric identity change

Because declaration order controls terse traversal, field reordering is a breaking change even when all names remain present.

## Parsing and application architecture

### Stage 1: lexical parser

The parser produces source-located statements containing:

- statement kind
- raw path tokens
- raw RHS tokens
- explicit skip tokens
- source file, line, and column

It handles comments, quoting, escapes, semicolons, and includes no game-specific lookup tables.

### Stage 2: path resolution

Generated schema code resolves the path through records, arrays, and keyed collections. It returns:

- selected node
- first writable leaf
- traversal boundary
- canonical path
- alias/deprecation diagnostics

### Stage 3: sequential decode

The decoder walks leaves inside the boundary and converts each RHS token according to the current leaf's type.

Its output is an atomic write set containing stable schema field identities, resolved indexes, typed values, and source provenance.

### Stage 4: typed apply

Generated binding code applies the write set to temporary typed state or a transaction overlay. No raw byte offsets or untyped pointer arithmetic are used.

### Stage 5: validation and commit

Cross-field validators inspect the complete candidate state. Examples include:

- an amount requiring a non-`none` weapon type
- an enum value unsupported by a selected hull
- a mod dependency requiring a registered resource
- a table whose weights must have a valid total

After validation, the transaction commits and affected subsystems rebuild declared derived state.

## Configuration groups and lifecycle

Every parameter belongs to a group, for example:

- `gameplay`: deterministic rules and game data
- `pregame`: settings needed before game creation or race selection
- `interface`: local presentation and controls
- `resource`: VFS and asset selection
- `metadata`: mod names, descriptions, and compatibility declarations

Groups control:

- when values may be changed
- whether a reload is permitted
- whether values are stored in a save
- whether values are synchronized in multiplayer
- which derived systems must be refreshed

Configuration loading is transactional at the requested group set. A failed file, include, mod, resource mount, or validator leaves the prior state active.

## Include behavior

`include` composes configuration source files without creating a mod package boundary:

```text
include balance/weapons.cfg;
```

Paths resolve relative to the current file or package according to the path type's rules. The loader detects include cycles and reports the include stack.

`stop;` ends only the current file and returns to its includer.

Optional includes may be supported explicitly, but absence should never be inferred from a malformed file. Missing and invalid are different outcomes.

## Native mod packages

### Manifest

Each mod has a JSON5 manifest containing at least:

- stable mod ID
- display name and version
- supported engine and schema ranges
- dependencies
- optional dependencies
- conflicts
- load ordering constraints
- configuration entry point
- resource roots or archives
- declared capabilities

Example direction:

```json5
{
  id: "example_balance_mod",
  name: "Example Balance Mod",
  version: "1.2.0",

  requires: {
    engine: ">=0.1.0",
    config_schema: ">=1 <2",
  },

  dependencies: [
    { id: "community_resources", version: ">=2.0.0" },
  ],

  conflicts: ["other_total_conversion"],
  load_after: ["base_game"],

  config: "config/main.cfg",
  resources: [
    { path: "resources", mount: "/" },
  ],
}
```

### `enable` semantics

```text
enable example_balance_mod;
```

performs the following operation:

1. Resolve the manifest by stable ID.
2. Validate engine and schema compatibility.
3. Resolve dependencies and ordering.
4. Detect cycles and conflicts.
5. Prepare VFS mounts in dependency order.
6. Parse the mod's configuration entry point.
7. Resolve any nested `enable` requests.
8. Validate all configuration and required resources.
9. Commit resource mounts and configuration together.
10. Record provenance for every resulting value and mounted resource.

If any step fails, the mod is not partially enabled.

### Resource layering

Mods provide resources through VFS layers. Higher-priority mods may override lower-priority paths according to deterministic ordering.

Diagnostics should be able to answer:

- which package supplies a resource
- which package it overrides
- which config statement selected it
- whether multiplayer peers have the same deterministic resource set

Individual parameters such as `help.lbx` may be imported for compatibility, but the native model should prefer package resource mounts.

## Save games and multiplayer

### Save games

Save files store the effective deterministic configuration using stable field identities, not JSON5 text, native memory dumps, or positional schema order.

The save should also record:

- configuration schema version
- enabled mod IDs and versions
- deterministic package/resource identity
- creation engine version
- last-saved engine version

Unknown optional fields should be skippable. Missing fields receive schema defaults or explicit migrations. Configuration is restored before dependent game-state sections.

### Multiplayer

The host synchronizes a canonical deterministic configuration and mod manifest. Peers compare:

- schema version
- mod IDs and versions
- deterministic resource hashes where required
- canonical gameplay configuration

Local interface settings may remain unsynchronized unless a game mode explicitly requires them.

Wire representation is versioned independently from the user-facing text syntax and JSON5 schema source files.

## Extraction and tooling

### Canonical extraction

The engine can write the effective configuration using:

- canonical names
- symbolic enum and mask values
- explicit `0b` and `0x` numeric prefixes
- stable declaration order
- bounded aggregate assignments where concise
- source provenance in optional comments

Extraction should favor readability. It need not minimize line count at the expense of understanding.

### Importing 1.50 configuration

A separate import tool should:

- parse known 1.50 syntax
- use a pinned 1.50 schema
- resolve flattened numeric positions
- translate flattened names into records and arrays
- translate short enum and mask names into canonical names
- identify values that 1.50 accepted only because of weak integer typing
- emit canonical native configuration with warnings for ambiguous cases

The runtime does not need to preserve every historical ambiguity if the importer can make it explicit.

### Schema tooling

The build should provide validation for:

- duplicate names and aliases
- duplicate enum values or mask bits
- unknown type references
- invalid defaults
- invalid native binding paths
- array-size mismatches
- replicated targets with incompatible types
- unstable field-order changes
- undocumented weak integer IDs
- save/network fields lacking stable identities

Generated documentation can list every parameter, path, type, default, enum value, mask bit, group, and compatibility alias.

## Error handling

Errors should identify the relevant phase.

### Syntax errors

- unterminated statement or string
- malformed numeric value or ratio
- invalid escape
- missing RHS

### Resolution errors

- unknown parameter or field
- indexing a non-array
- missing or out-of-range array index
- ambiguous alias
- use of a removed field

### Sequential decode errors

- too many RHS items for the selected boundary
- invalid skip use
- invalid enum name
- unknown numeric enum ID
- unknown mask bit
- out-of-range integer
- value type mismatch

### Mod-loading errors

- missing manifest
- incompatible engine or schema version
- missing or cyclic dependency
- conflict
- resource mount collision forbidden by policy
- missing required resource
- nested mod activation failure

### Binding and generation errors

- missing native root or member
- C++ type mismatch
- invalid replicated target
- unsupported schema shape
- unstable or duplicate stable field identity

## Implementation phases

### Phase 1: schema foundation

- Define JSON5 document structure.
- Implement JSON5 loading for build tools.
- Define primitive, enum, mask, record, array, and parameter nodes.
- Import and verify a small set of value catalogs from 1.50 and restored source.
- Generate schema IDs and documentation.

### Phase 2: assignment parser

- Implement comments, statements, paths, values, masks, ratios, and skips.
- Preserve source locations.
- Add parser tests independent of game semantics.

### Phase 3: bounded traversal

- Resolve records and arrays.
- Implement the boundary rules in this document.
- Decode complete statements atomically.
- Add exhaustive tests for record, array, nested-array, skip, and overflow behavior.

### Phase 4: generated native bindings

- Bind a small gameplay subsystem using cleaned names.
- Generate typed writes without offsets or expressions.
- Add compile-time and generator-time type validation.
- Add cross-field validation and transactional commit.

### Phase 5: includes, extraction, and provenance

- Add include stacks and cycle detection.
- Add canonical extraction.
- Track the source mod, file, line, and statement responsible for each value.
- Add a 1.50 config importer.

### Phase 6: native mod loader

- Define and validate mod manifests.
- Resolve dependencies, conflicts, and ordering.
- Integrate atomic VFS resource mounting.
- Make `enable` activate complete packages.

### Phase 7: saves and networking

- Assign stable serialized field identities.
- Save effective deterministic configuration and mod identity.
- Add migrations and compatibility fixtures.
- Synchronize canonical gameplay configuration and deterministic resources.

### Phase 8: expand coverage

- Port configuration and fixes subsystem by subsystem.
- Replace weak integer IDs with verified enums.
- Add additional validation learned from restored gameplay rules.
- Evaluate scripting separately after the declarative system is stable.

## Open decisions

- Exact JSON5 parser/library or whether to implement the required subset in-tree.
- Exact quoted-string escape rules for canonical files and 1.50 compatibility mode.
- Whether optional include syntax is retained or replaced with an explicit statement form.
- Whether whole-record assignment may omit trailing values without warnings.
- Whether explicit array ranges are needed after whole-array assignment and skips are available.
- How schema stable field identities are assigned and reviewed.
- Which interface settings belong in saves or multiplayer synchronization.
- Whether unknown deterministic resource files are rejected or merely excluded from identity hashes.

## Final direction

The terse language is a schema-aware override language, not a general serialization format. JSON5 describes the valid logical game model and its native bindings; CFG statements describe ordered changes to that model.

The critical safety rule is structural containment:

```text
guardian weapon = ...;
```

may traverse every weapon, while:

```text
guardian weapon 1 = ...;
```

may modify only weapon 1. Terseness is retained without allowing a missing or extra value to spill into an unrelated object.