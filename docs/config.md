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
- Apply RHS values sequentially beginning at the first leaf selected by the LHS.
- Allow a scalar LHS to act as a starting field within its nearest containing aggregate.
- Bound traversal to the selected aggregate, or to a scalar's nearest containing aggregate, so assignment cannot spill into a sibling object.
- Represent arrays as arrays instead of expanding them into names such as `weapon_1_type`.
- Use cleaned, stable external names and cleaned native bindings.
- Define finite value domains for semantic IDs, enums, and masks whenever the engine has a finite domain.
- Support explicit-width primitive types and reusable semantic type aliases with stricter constraints.
- Store human-readable titles, descriptions, remarks, units, and examples as schema data for generated documentation.
- Store typed logical defaults, including record and array defaults, without embedding native initializer text.
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

An aggregate LHS selects both:

- the first leaf to update
- the aggregate that bounds sequential propagation

A scalar LHS selects its leaf as the first leaf to update. Its nearest containing aggregate is the traversal boundary. For example, a field inside one array element may update the remaining fields of that element, but it may not continue into the next element. A scalar parameter with no containing aggregate remains a one-value assignment.

Partial updates may start directly at a scalar field or use skip items from the beginning of an aggregate.

The complete statement is decoded and validated before any write is committed.

### 5. Finite domains should be explicit

If a field identifies one of a finite set of weapons, specials, buildings, drives, or other game concepts, its schema type should list those values. It should not be represented merely as an unrestricted `uint8` because the original patch did so.

Numeric syntax and semantic validity are separate policies. A type may accept numeric IDs while still rejecting numbers that do not identify a declared value.

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

Canonical output uses explicit `0b` for binary.

### Scalar-start assignment

```text
combat fighters_use_point_defense = true;
```

The LHS resolves to a scalar parameter with no containing aggregate, so exactly one RHS value is allowed.

A scalar field inside an aggregate acts as a bounded starting cursor:

```text
guardian weapon 1 mods = heavy,eccm 0 2;
```

This updates `mods`, `ammo`, and `amount`. The boundary is the containing weapon 1 record, so another value is an error rather than an update to weapon 2.

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
guardian weapon 1 = - - heavy,eccm - 2;
```

The LHS selects the complete weapon 1 record. The first two skips preserve `type` and `arc`, `mods` and `amount` are changed, and `ammo` is preserved. The scalar-start form can express the same suffix more directly:

```text
guardian weapon 1 mods = heavy,eccm - 2;
```

It may carry values only through the end of weapon 1.

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

Selecting one scalar array element starts within the containing array:

```text
guardian special 3 = battle_scanner augmented_engines damper_field;
```

This updates elements 3 through 5. Traversal may continue through the remaining `special` elements, but it cannot continue into the field after the array.

### Table assignment

Nested arrays and records use the same rules. Selecting a table permits traversal over the entire table. Selecting a row limits traversal to that row.

```text
mount_damage = 50 100 150 75 150 225;
mount_damage high_energy_focus = 75 150 225;
mount_damage high_energy_focus point_defense = 75;
```

The first statement selects the complete table. The second selects one row and may update every field in that row. The third starts at one scalar cell; with one RHS value it changes only that cell, while additional values may update later cells in the same row. It cannot continue into the next row. To update the final two fields of a row, selecting the row and skipping the first field remains valid:

```text
mount_damage high_energy_focus = - 150 225;
```

The schema defines each aggregate's declaration order and therefore its sequential order.

### Skip items

`-` advances one leaf without changing it:

```text
mount_damage no_high_energy_focus = 50 - -;
```

A skip advances one leaf from the current cursor and still counts toward the boundary. It is valid for aggregate and scalar-start assignments. A statement containing only skips is rejected because it performs no update.

### Atomic behavior

Before applying a statement, the decoder must:

1. Resolve the complete LHS.
2. Determine the first leaf and traversal boundary.
3. Walk all RHS items from the first leaf without crossing the boundary.
4. Type-check and range-check every non-skip item.
5. Verify that no item exceeds the boundary.
6. Produce a temporary write list.
7. Commit all writes only if every step succeeds.

There must never be a partially applied statement.

## Bounded sequential traversal

### Boundary rule

Every assignment resolves to a first writable leaf and a traversal boundary.

- If the LHS resolves to a record, that record is the boundary.
- If it resolves to an array, that array is the boundary.
- If it explicitly selects an array element that is a record or array, that element is the boundary.
- If it resolves to a scalar field, the nearest aggregate containing that field is the boundary.
- A record-valued array element contains its own fields, so a scalar inside that element is bounded to the element record rather than the containing array.
- A scalar array element is bounded to its containing array and may update that element and later elements.
- If a scalar parameter has no containing aggregate within the parameter, the scalar itself is the boundary and exactly one RHS value is allowed.

Traversal always begins at the selected scalar or at the first leaf of the selected aggregate. It proceeds only forward and never wraps to the beginning of a boundary.

Examples:

| Assignment prefix | First leaf | Boundary |
| --- | --- | --- |
| `guardian =` | `appearance` | Guardian record |
| `guardian weapon =` | `weapon[1].type` | Complete weapon array |
| `guardian weapon 1 =` | `weapon[1].type` | Weapon 1 record |
| `guardian weapon 1 mods =` | `weapon[1].mods` | Weapon 1 record |
| `guardian special =` | `special[1]` | Complete special array |
| `guardian special 3 =` | `special[3]` | Complete special array, starting at element 3 |
| `combat fighters_use_point_defense =` | selected scalar | Scalar parameter; one value only |

### Leaf order

Sequential order is depth-first declaration order:

- record fields follow their order in JSON5
- array elements follow ascending external index
- nested aggregates are recursively flattened into leaves
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

The same rule applies to scalar starts:

```text
guardian weapon 1 mods = heavy 0 2 4;
```

The fourth item exceeds the weapon 1 boundary even though weapon 2 follows it in native or logical storage.

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

- explicit-width primitive types and ranges
- reusable semantic type aliases and field-level constraints
- enums and their stable numeric values
- masks and their stable bit values
- ratios and strings
- reusable records
- arrays and index bases
- parameter roots
- defaults
- declaration and traversal order
- titles, descriptions, remarks, units, and examples for generated documentation
- mutability and synchronization groups
- native headers, symbols, C++ types, and member paths
- replicated writes to more than one native field

The documents do not contain:

- executable C++ expressions
- binary offsets
- arbitrary pointer arithmetic
- embedded scripts
- instruction bytes

### Documentation metadata

JSON5 comments explain the schema source to its maintainers, but comments are not documentation data and are not consumed by generators. Human-facing text must therefore be stored explicitly on schema nodes.

Public types, parameters, records, fields, arrays, enum values, mask flags, constraints, and groups define human-facing metadata where it adds meaning. Public parameters and reusable semantic types require at least `title` and `description`. Other nodes may inherit sufficient context from their parent, but schema tooling should require explicit text whenever generated documentation would otherwise be ambiguous.

Available metadata fields are:

- `title`: a short heading or table label
- `description`: game-facing Markdown suitable for generated manuals
- `remarks`: optional technical details or caveats
- `units`: a structured or textual unit such as `percent`, `turns`, or `BC`
- `examples`: canonical assignment examples

Example:

```json5
{
  id: "weapon",
  title: "Weapon slots",
  description: "The Guardian's five weapon slots in firing priority order.",
  remarks: "External slot indexes are one-based.",
  examples: [
    "guardian weapon 1 = plasma_cannon all heavy 0 2;",
  ],
}
```

### Defaults

`default` is a property of the parameter, record field, or array element it initializes; defaults are not stored in a separate table. It appears beside `type`. Defaults use logical schema values rather than C++ initializer strings, native enum spellings, or memory representations.

Scalar defaults use the JSON5 representation appropriate to their type. Quoted semantic values are decoded by the referenced type, so defaults and CFG assignments use the same names and mask grammar:

- booleans, integers, and strings use JSON5 scalars
- enums use a quoted canonical value name, or a numeric ID when the enum permits numeric values
- masks use one quoted comma-separated string of canonical flag names, such as `"forward,aft"`; named zero or composite values such as `"none"` and `"all"` must be declared by the mask type
- masks may use a numeric value when the mask permits numeric input; canonical schema defaults should prefer names
- ratios use a quoted canonical ratio such as `"1/3"`

For example, a weapon-slot default may be written as:

```json5
{
  type: "missile",
  arc: "forward,aft",
  mods: "continuous",
  ammo: 0,
  amount: 2,
}
```

Each string is checked against the selected field's type. It is not an untyped string stored in native state.

Record defaults normally come from the defaults on their field definitions. A record value may instead declare a complete object default keyed by stable field ID when one use of a reusable record needs different values.

An array with one default for every element declares it on the `element` definition. An array whose elements have different defaults declares an exact JSON5 array on the array definition. An exact array must contain the declared number of elements; its first item corresponds to the array's lowest external index.

```json5
{
  parameters: {
    hit_threshold_fixed_value: {
      title: "Fixed hit threshold value",
      description: "The fixed term in the beam hit-threshold formula.",
      type: "vtype_lucky_divisor",
      default: 40,
    },

    anti_missile_rocket_chance_to_hit: {
      title: "Anti-missile rocket accuracy",
      description: "Terms used by the anti-missile rocket hit formula.",
      type: {
        kind: "record",
        fields: [
          { id: "base", type: "uint8", default: 70 },
          { id: "range_multiplier", type: "uint8", default: 10 },
          { id: "range_divisor", type: "uint8", default: 3 },
        ],
      },
    },

    research_percent: {
      title: "Research percentages",
      description: "Research multipliers for all seven difficulty levels.",
      type: {
        kind: "array",
        count: 7,
        index_base: 1,
        element: {
          type: "uint8",
          default: 100,
        },
      },
    },

    terrain_food_bonus: {
      title: "Terrain food bonuses",
      description: "Food bonuses ordered by the terrain array's declared indexes.",
      type: {
        kind: "array",
        count: 10,
        index_base: 0,
        element: { type: "int8" },
      },
      default: [0, 0, 0, 2, 2, 4, 4, 2, 4, 6],
    },
  },
}
```

An aggregate default is always complete: record objects contain every writable field and exact arrays contain every element. There is no skip marker or partial aggregate default. If an aggregate has no `default`, its field or element definitions provide defaults recursively. An `element.default` is repeated for every array element. Every writable leaf must receive exactly one effective default. Schema validation rejects missing defaults, an exact array of the wrong length, and overlapping aggregate and descendant defaults.

Defaults are decoded, normalized, type-checked, range-checked, and cross-field validated using the same rules as configuration input. They establish the initial candidate state before base-game configuration or mod overrides are applied. Defaults are logical data: the generator converts them to typed native initialization and never accepts declarations such as `"{ .base = 70 }"`.

Generated documentation must not depend on comments adjacent to a declaration.

### Value type example

```json5
{
  types: {
    // Built-in explicit-width primitives are supplied by the schema runtime:
    // bool, int8, uint8, int16, uint16, int32, and uint32.

    officer_id: {
      kind: "alias",
      type: "int16",
      title: "Officer ID",
      description: "Identifies an entry in the fixed officer table.",
      constraints: {
        min: 0,
        max: 66,
      },
    },

    weapon_count: {
      kind: "alias",
      type: "uint8",
      title: "Weapon count",
      description: "Number of weapon units installed in one slot.",
      constraints: {
        min: 0,
        max: 99,
      },
    },

    weapon_id: {
      kind: "enum",
      base_type: "uint8",
      allow_numeric: true,
      reject_unknown_numeric: true,
      values: {
        none: {
          value: 0,
          title: "No weapon",
          description: "Leaves the weapon slot empty.",
        },
        laser_cannon: { value: 3, title: "Laser Cannon" },
        mass_driver: { value: 4, title: "Mass Driver" },
        plasma_cannon: { value: 11, title: "Plasma Cannon" },
      },
    },

    special_id: {
      kind: "enum",
      base_type: "uint8",
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
      values: {
        none: 0,
        all: 15,
      },
      flags: {
        forward: { value: 1, title: "Forward" },
        aft: { value: 2, title: "Aft" },
        left: { value: 4, title: "Left" },
        right: { value: 8, title: "Right" },
      },
    },

    weapon_mod_mask: {
      kind: "mask",
      allow_numeric: true,
      reject_unknown_bits: true,
      values: {
        none: 0,
      },
      flags: {
        heavy: {
          value: 0x0002,
          title: "Heavy mount",
          description: "Uses the heavy weapon mount variant.",
        },
        point_defense: { value: 0x0004, title: "Point defense" },
        armor_piercing: { value: 0x0008, title: "Armor piercing" },
        continuous: { value: 0x0010, title: "Continuous" },
        enveloping: { value: 0x0100, title: "Enveloping" },
        eccm: { value: 0x0200, title: "ECCM" },
        overloaded: { value: 0x4000, title: "Overloaded" },
      },
    },

    ratio16: {
      kind: "ratio",
      component_type: "int16",
      title: "16-bit ratio",
      description: "An exact rational value stored as a numerator and denominator.",
      constraints: {
        denominator_nonzero: true,
      },
      cpp: {
        header: "game/config_types.h",
        type: "s_ratio16",
        numerator_member: "numerator",
        denominator_member: "denominator",
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
      title: "Weapon slot",
      description: "One installed weapon type and its firing properties.",
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
          title: "Amount",
          description: "Number of weapon units installed in this slot.",
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
      title: "Orion Guardian",
      description: "Defines the equipment and combat design used by the Guardian at Orion.",
      group: "gameplay",
      bind_root: {
        header: "game/ships.h",
        symbol: "SHIPDATA::_guardian_design",
        cpp_type: "s_ship_design",
      },
      type: {
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
            title: "Weapon slots",
            description: "The Guardian's five weapon slots in firing priority order.",
            bind: ["weapons"],
            type: {
              kind: "array",
              count: 5,
              index_base: 1,
              element: { type: "weapon_slot" },
            },
            default: [
              { type: "plasma_cannon", arc: "all", mods: "heavy", ammo: 0, amount: 2 },
              { type: "mass_driver", arc: "all", mods: "point_defense", ammo: 0, amount: 10 },
              { type: 20, arc: 15, mods: 17664, ammo: 0, amount: 1 },
              { type: 20, arc: 15, mods: 17664, ammo: 0, amount: 1 },
              { type: 13, arc: "all", mods: "none", ammo: 0, amount: 2 },
            ],
          },

          {
            id: "special",
            title: "Special systems",
            description: "Special systems installed on the Guardian.",
            bind: ["specials"],
            type: {
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

- include the declared native header
- resolve the declared root symbol and expected C++ type
- bind one logical leaf to one native field
- bind one leaf to several native fields
- bind an array to a native fixed-size array
- apply a fixed index-base conversion
- map an enum or mask to the same underlying numeric type
- bind a parameter to project-owned configuration state consumed by gameplay code

If changing a setting requires procedural behavior, the preferred design is to store typed configuration state and let the owning C++ subsystem consult it. The schema should not invoke arbitrary setter code merely to imitate a binary patch.

Derived caches and resources may be rebuilt by explicit configuration lifecycle services after a successful transaction. Those services are engine code, not field mappings.

Generated code must compile against the declared native headers and types. The generator should emit static checks for fixed array extents and compatible field types where possible; the C++ compiler is the final authority on binding validity. Generated initialization must preserve the same logical-to-native conversions and replicated bindings used by ordinary writes.

## Strong typing policy

### Explicit-width primitives

The built-in scalar types are:

- `bool`
- `int8` and `uint8`
- `int16` and `uint16`
- `int32` and `uint32`
- `string`

The schema does not expose an ambiguous platform-sized `int`. Numeric literals are decoded into a sufficiently wide temporary representation and then checked against the selected explicit-width type and field constraints.

### Semantic type aliases

A semantic alias refines a primitive or another alias without changing its storage representation:

```json5
{
  star_id: {
    kind: "alias",
    type: "int16",
    title: "Star ID",
    description: "Identifies a star slot in the galaxy arrays.",
    constraints: {
      min: 0,
      max: 70,
    },
  },
}
```

Aliases provide reusable constraints, documentation, units, and stronger generated diagnostics. Use an enum instead when every valid identity has a stable finite catalog.

### Field types

Every parameter, record field, and array element has a `type`. The property has one of two forms:

- a string references a previously defined built-in, alias, enum, mask, ratio, record, or other named type
- an object defines the type directly using the same properties accepted by a named type definition

Both forms have identical decoding, validation, binding, save, network, and documentation rules.

For example, the design-screen CLEAR button behavior has a local three-value enum:

```json5
{
  parameters: {
    clear_button_computer: {
      title: "CLEAR button computer behavior",
      description: "Controls how the design-screen CLEAR button changes the selected computer.",
      group: "interface",
      bind_root: {
        header: "game/config.h",
        symbol: "config::clear_button_computer",
        cpp_type: "uint8_t",
      },
      type: {
        kind: "enum",
        base_type: "uint8",
        values: {
          update: {
            value: 0,
            title: "Update",
            description: "Select the best available computer.",
          },
          clear: {
            value: 1,
            title: "Clear",
            description: "Remove the selected computer.",
          },
          intact: {
            value: 2,
            title: "Leave intact",
            description: "Keep the currently selected computer.",
          },
        },
      },
      default: "update",
    },
  },
}
```

The assignment remains semantic:

```text
clear_button_computer = update;
```

The decoder maps `update`, `clear`, and `intact` to integer values 0, 1, and 2. Generated binding code stores the selected value using the enum's `base_type`. An object-valued field type follows the same uniqueness, numeric-input, default-validation, save, network, and documentation rules as a referenced named type.

Define a named type under `types` when the same domain is used by more than one field or is independently meaningful in generated APIs or documentation. Otherwise, define the type directly on the field.

### Field-level constraints

A field may narrow, but never broaden, its referenced type:

```json5
{
  id: "starting_officer",
  type: "officer_id",
  constraints: {
    max: 20,
  },
}
```

Numeric constraints include `min` and `max`. String constraints include `min_length`, `max_length`, `encoding`, and either a predefined `format` or an explicit `pattern`.

```json5
{
  id: "id",
  type: "string",
  constraints: {
    min_length: 1,
    max_length: 63,
    encoding: "utf8",
    format: "identifier",
  },
}
```

`pattern` denotes a regular expression and is unrelated to bit masks. The schema version must define the supported regular-expression dialect and whether matching covers the complete string. Reusable formats such as `identifier`, `mod_id`, `semantic_version`, and `vfs_path` are preferred over repeated patterns.

### Enums

Enums declare every recognized semantic value and its stable numeric identity.

```json5
{
  kind: "enum",
  base_type: "uint8",
  allow_numeric: true,
  reject_unknown_numeric: true,
  values: {
    none: 0,
    laser_cannon: 3,
    plasma_cannon: 11,
  },
}
```

`base_type` is required and must reference an integer primitive or integer alias capable of representing every declared value. It controls generated storage and native compatibility; it does not change the user-facing symbolic syntax.

Policies are independent:

- `allow_numeric` permits `11` in addition to `plasma_cannon`.
- `reject_unknown_numeric` rejects numeric values absent from the catalog.

Canonical configs should prefer names. Numeric IDs remain useful for incomplete reverse engineering and values whose names are not yet stable.

### Masks

Masks declare every known bit:

```json5
{
  kind: "mask",
  allow_numeric: true,
  reject_unknown_bits: true,
  values: {
    none: 0,
  },
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

The same value is a quoted string in JSON5 because a comma is structural JSON5 syntax:

```json5
{ default: "heavy,point_defense" }
```

`values` declares canonical names for zero or useful composite masks. Its values must contain no unknown bits. A `values` entry may duplicate a combination of `flags`, but `flags` must still declare each independently addressable bit. Canonical output may use a declared composite name only when it represents the complete value; otherwise it emits the individual flag names in declaration order.

### Integers

Integers are used only when every value in a numeric range is meaningful, or when the domain has not yet been recovered well enough to define an enum.

Semantically finite IDs should not remain generic integers permanently. Schema review should track such weak types as restoration work.

### Ratios

Ratios are one logical scalar even though their native representation contains two components. A ratio consumes one RHS position:

```text
government_bonus feudal_reduction = 1/3;
```

The built-in ratio family declares an explicit-width signed or unsigned component type and binds to a native ratio structure:

```cpp
struct s_ratio16 {
    int16_t numerator;
    int16_t denominator;
};
```

The JSON5 type identifies the C++ type and its numerator and denominator members, allowing generated typed binding code. A field may alternatively map the two components to two declared native members, but a dedicated ratio structure is preferred because it cannot be updated partially.

Ratios are normalized during decoding:

- the denominator must be nonzero
- the denominator is made positive
- numerator and denominator are reduced by their greatest common divisor
- zero is represented as `0/1`
- an integer token such as `2` is accepted as `2/1`

### Strings and paths

Strings have explicit length, encoding, format, and pattern constraints. Paths are semantic string aliases with VFS-aware normalization and package-relative resolution.

## Names and schema evolution

### Canonical names

Canonical names should:

- describe game concepts rather than binary variables
- use consistent singular names for one selected element
- represent repeated structures as arrays
- avoid embedded indexes such as `weapon_1_type`
- remain stable once published

There is no legacy alias layer for old 1.50 field names or flattened positions. Numeric selectors are reserved for actual arrays and keyed numeric collections.

### Schema versioning

The schema has its own version independent of the executable version. A mod manifest declares the schema compatibility range it targets.

Changes are classified as:

- additive: new parameter or new trailing field with a default
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
- first writable leaf and traversal boundary
- canonical path

### Stage 3: sequential decode

The decoder walks forward from the resolved first leaf to the boundary and converts each RHS token according to the current leaf's type. A root scalar has a one-leaf boundary; a scalar inside an aggregate may consume the remaining leaves within that boundary.

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

The manifest is separate from the CFG entry point because package discovery and dependency resolution happen before game configuration can be applied. The loader must know a package's stable ID, version, requirements, dependencies, conflicts, resource roots, and entry point before it can establish load order or resolve package-relative files.

Keeping this metadata in ordinary configuration variables would require partially executing every installed mod merely to discover it, would allow includes to overwrite package identity, and would create a cycle between resource mounting and configuration parsing. A JSON5 manifest is directly inspectable by launchers, package managers, multiplayer compatibility checks, and dependency tools without loading the game configuration schema.

CFG remains responsible for concise game-state overrides. JSON5 remains responsible for describing the package that contains those overrides.

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

The native model uses package resource mounts rather than binary-era per-file substitution parameters.

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

### Schema tooling

The build should provide validation for:

- duplicate names
- duplicate enum values or mask bits
- unknown type references
- invalid defaults
- invalid native binding paths
- array-size mismatches
- missing, overlapping, or invalid defaults
- exact array defaults with the wrong length
- replicated targets with incompatible types
- unstable field-order changes
- undocumented weak integer IDs
- save/network fields lacking stable identities
- missing human-facing titles or descriptions on public nodes

Generated documentation can list every parameter, path, type, constraint, default, enum value, mask bit, group, unit, example, and native binding.

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
- Define explicit-width primitive, semantic alias, enum, mask, ratio, string, record, array, and parameter nodes.
- Define typed scalar, record, exact-array, and repeated element defaults.
- Define documentation metadata and field-level constraints.
- Import and verify a small set of value catalogs from 1.50 and restored source.
- Generate schema IDs and documentation.

### Phase 2: assignment parser

- Implement comments, statements, paths, values, masks, ratios, and skips.
- Preserve source locations.
- Add parser tests independent of game semantics.

### Phase 3: bounded traversal

- Resolve records and arrays.
- Implement the boundary rules in this document.
- Implement scalar-start traversal within the nearest containing aggregate.
- Decode complete statements atomically.
- Add exhaustive tests for record, array, nested-array, scalar-start, skip, and overflow behavior.

### Phase 4: generated native bindings

- Bind a small gameplay subsystem using cleaned names.
- Generate typed writes without offsets or expressions.
- Add compile-time and generator-time type validation.
- Add cross-field validation and transactional commit.

### Phase 5: includes, extraction, and provenance

- Add include stacks and cycle detection.
- Add canonical extraction.
- Track the source mod, file, line, and statement responsible for each value.

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
- Exact quoted-string escape rules for canonical files.
- Whether optional include syntax is retained or replaced with an explicit statement form.
- Whether whole-record assignment may omit trailing values without warnings.
- Whether explicit array ranges are needed after whole-array assignment and skips are available.
- Exact regular-expression dialect and string-length unit.
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

A scalar selection starts at that field but remains structurally contained:

```text
guardian weapon 1 mods = heavy,eccm 0 2;
```

may update the remaining fields of weapon 1, but cannot spill into weapon 2. Root scalar parameters still accept exactly one value because they have no containing aggregate.