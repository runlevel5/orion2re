# Config system design

## Goal

Add support for the 1.50 config format as a native feature of this project.

The config system should:

- parse CFG files, including includes and commands
- decode symbolic names and values using the external schema
- interpret config operations directly against live globals and global tables
- generate the interpreter from schema plus project-owned semantic mappings
- support bulk notation, partial bulk notation, and skip markers correctly
- avoid handwritten field-by-field loaders for regular tables

This document records the current design direction.

## Core decisions

### 1. The CFG parser is syntax-only

The parser does not know table semantics.

It only understands:

- comments beginning with `#`
- semicolon-terminated statements
- commands: `include`, `enable`, `stop`
- assignment syntax: `<lhs> = <rhs>;`
- token kinds: integer, ratio, quoted string, identifier, skip marker

The parser output is an opaque raw config object containing operations in source order.

Each assignment is stored as:

- primary parameter name token
- zero or more lhs selector tokens
- one or more rhs value tokens

Example:

```text
building autolab cost = 200;
```

parses as:

- name: `building`
- lhs selectors: `autolab`, `cost`
- rhs values: `200`

The parser does not decide whether `autolab` is a row, a field, or something else.

### 2. `config.json` is the external schema

`c:\Projects\150\150\docs\config.json` defines the external config schema.

It provides:

- parameter names
- parameter kind: integer, string, enum, table
- row selector domains
- column selector domains
- logical field ordering
- per-field value kind
- min/max bounds
- enum values
- mask flag definitions
- defaults

It does not provide project-specific semantic mapping such as:

- which global variable to mutate
- which C++ field corresponds to a config column
- whether a config table maps to a struct, array, or custom helper

So `config.json` is sufficient for parsing support, decode, and validation, but not sufficient by itself to mutate live project memory.

### 3. Decoded selectors must become numeric

After schema decode, every selector after the primary parameter name should resolve to a numeric selector id or numeric literal.

Example:

```text
field a 1 = 10;
```

becomes conceptually:

```text
name = FIELD
lhs = [A, 1]
rhs = [10]
```

There should be no unresolved symbolic selector left in decoded operations.

This assumption was validated against the current schema and sample config with `tools/validate_config_lhs_numeric.py`.

### 4. The runtime is an interpreter, not a byte-offset patch VM

Config application should change live memory immediately.

However, generated code should not be expressed as raw byte-offset patching like:

```cpp
*(uint16_t*)((uint8_t*)base + row * stride + field_offset) = value;
```

Instead, generated code should look like ordinary typed C++ assignments against actual globals and fields:

```cpp
MOX::_weapon_table[Weapon_Id].cost = Read_Uint16_(Value);
MOX::_building_table[Building_Id].group = Read_Uint8_(Value);
```

This still mutates live memory, but keeps the interpreter readable, typed, and debuggable.

### 5. Types come from `config.json`

The generated interpreter should derive value readers from schema.

The project mapping file should not duplicate external field types.

If generated code does not compile because the schema-implied type does not match the actual internal field type, that is a useful correctness signal.

## External config semantics that must be supported

The 1.50 config format supports more than just scalar assignments and simple table cell updates.

The important model is:

```text
<name> <lhs selectors...> = <rhs items...>;
```

where the schema determines how lhs selectors and rhs items are interpreted.

### Bulk notation

The manual defines bulk notation for tables.

Examples:

```text
mount_dmg = 50 100 150 75 150 225;
mount_dmg hef pd = 75 150 225;
mount_dmg hef = 75 150 225;
```

This means a config operation can start at some logical table position and then walk forward through the schema-defined logical order.

### Skip marker

The manual also defines `-` as a skip marker inside bulk notation.

Example from the manual:

```text
mount_dmg nohef pd = 50 - - 75;
```

This means:

- assign the first provided value to the first selected logical position
- advance logical position after every rhs item
- if an rhs item is `-`, do not modify the current logical field, only advance

This must be supported by the interpreter directly.

### Example: skip-aware partial bulk notation

Example based on existing mod files:

```text
antaran_titan___defensive weapon_1_type = 11 - 00000000000000010 - 8;
```

Using schema order, this means:

- `weapon_1_type = 11`
- `weapon_1_arc = skip`
- `weapon_1_mods = 00000000000000010`
- `weapon_1_ammo = skip`
- `weapon_1_amount = 8`

The interpreter must derive this by walking logical schema order from the supplied starting selector. It must not rely on handwritten special cases for this particular parameter.

## High-level architecture

The config system is split into four stages.

### Stage 1: CFG parser

Runtime code in `config.cpp` parses text CFG files into opaque raw operations.

Responsibilities:

- tokenization
- include processing
- optional include handling
- stop command handling
- source location tracking
- preserving source order
- representing skip markers explicitly

No schema knowledge is required here.

### Stage 2: generated decode layer

Generated code maps:

- parameter names to numeric ids
- selector identifiers to numeric selector ids
- enum values to numeric values
- mask codes to numeric bitmasks
- raw value shape to decoded operation structure

This layer is driven by `config.json`.

Output is a decoded operation stream using:

- numeric parameter ids
- numeric selectors
- typed primitive values
- explicit skip values where `-` was used

### Stage 3: generated layout-driven interpreter

Generated code interprets decoded operations against live globals.

This layer is driven by:

- `config.json`
- a project-owned semantic bindings file

The key rule is:

- traversal order comes from generated logical layout metadata
- actual field assignment targets come from generated semantic mapping

### Stage 4: optional handwritten custom helpers

Most regular parameters and tables should be generated.

Some irregular cases may still require custom helpers, for example:

- variable-length encoded payloads
- special packing rules
- application logic not expressible as regular field traversal

These should be explicitly marked as custom in the bindings file.

## Decoded runtime model

The generated interpreter should work on decoded operations.

Conceptually:

```cpp
struct s_selector {
    int32_t id;
};

struct s_value {
    int32_t kind;
    int32_t int_value;
    const char* string_value;
};

struct s_operation {
    int32_t name_id;
    const s_selector* lhs;
    int32_t lhs_count;
    const s_value* rhs;
    int32_t rhs_count;
};
```

At decode time, `-` should become an explicit skip value kind.

For example:

```text
weapon_1_type = 11 - 00000000000000010 - 8
```

should decode to five rhs items, two of which are skip values.

## Generated logical layout

The interpreter must not hardcode table traversal shape by hand.

Instead, the generator should emit a logical layout for every generated table-like parameter.

Conceptually:

```cpp
struct s_layout_item {
    int32_t logical_index;
    int32_t schema_field_id;
    int32_t target_field_id;
};
```

The purpose of `s_layout_item` is:

- preserve external logical field order from `config.json`
- connect external logical field positions to internal semantic target fields

It should not duplicate the field type, because the type already comes from the schema.

### Example layout use

For a flat design table like `antaran_titan___defensive`, generated layout would preserve the schema order:

- `appearance`
- `hull_size`
- `drive`
- ...
- `weapon_1_type`
- `weapon_1_arc`
- `weapon_1_mods`
- `weapon_1_ammo`
- `weapon_1_amount`
- ...

If an operation starts at `weapon_1_type`, the generated interpreter resolves that to a starting logical index and then walks the layout forward across rhs items.

## Semantic bindings file

Because `config.json` has no knowledge of project globals or struct fields, a second input file is required.

This document calls it `config_bindings.json`.

Its purpose is to define semantic mapping from external config fields to actual internal globals and fields.

It should not duplicate value types from `config.json`.

It should define:

- which parameters are generated
- which parameters are custom
- target global/table expression for generated parameters
- mapping from external field names to internal field names or target field ids

Example direction:

```json
{
  "parameters": {
    "disable_vsync": {
      "kind": "scalar",
      "target_expr": "MOX::_disable_vsync_flag"
    },
    "building": {
      "kind": "generated_layout_table",
      "target_expr": "MOX::_building_table",
      "fields": {
        "id": "id",
        "tech_id": "tech_id",
        "cost": "cost",
        "maintenance": "maintenance",
        "group": "group"
      }
    },
    "antaran_titan___defensive": {
      "kind": "generated_layout_table",
      "target_expr": "MOX::_antaran_titan_defensive",
      "fields": {
        "appearance": "appearance",
        "hull_size": "hull_size",
        "drive": "drive",
        "armor": "armor",
        "shield": "shield",
        "computer": "computer",
        "weapon_1_type": "weapon_1_type",
        "weapon_1_arc": "weapon_1_arc",
        "weapon_1_mods": "weapon_1_mods",
        "weapon_1_ammo": "weapon_1_ammo",
        "weapon_1_amount": "weapon_1_amount"
      }
    },
    "ai_race_variant_table": {
      "kind": "custom",
      "apply_function": "Apply_Ai_Race_Variant_Table_"
    }
  }
}
```

This file answers:

- which live object to mutate
- which generated interpretation strategy to use
- which internal field each external field maps to
- whether a parameter requires a handwritten helper

## Generated interpreter behavior

The generated interpreter should contain:

- primitive typed readers chosen from schema
- parameter dispatch by generated parameter id
- generated field write dispatch by generated target field id
- generic traversal for bulk notation using generated layout items

### Top-level dispatch

Conceptually:

```cpp
switch (Operation.name_id) {
case CONFIG_NAME_BUILDING:
    Apply_Building_(Operation);
    break;
case CONFIG_NAME_ANTARAN_TITAN_DEFENSIVE:
    Apply_Antaran_Titan_Defensive_(Operation);
    break;
}
```

### Generated traversal for layout-driven tables

Conceptually:

```cpp
static void Apply_Layout_Driven_Operation_(
    const s_parameter_binding& Binding,
    const s_operation& Operation)
{
    int32_t Logical_Index = Resolve_Start_Logical_Index_(Binding, Operation.lhs, Operation.lhs_count);

    for (int32_t Rhs_Index = 0; Rhs_Index < Operation.rhs_count; ++Rhs_Index, ++Logical_Index) {
        const s_value& Value = Operation.rhs[Rhs_Index];

        if (Value.kind == CONFIG_VALUE_SKIP) {
            continue;
        }

        const s_layout_item& Item = Binding.layout[Logical_Index];
        Apply_Layout_Item_Value_(Binding, Item, Value);
    }
}
```

### Generated typed field writes

Conceptually:

```cpp
static void Apply_Layout_Item_Value_(
    const s_parameter_binding& Binding,
    const s_layout_item& Item,
    const s_value& Value)
{
    switch (Item.target_field_id) {
    case FIELD_WEAPON_1_TYPE:
        MOX::_antaran_titan_defensive.weapon_1_type = Read_Uint16_(Value);
        break;
    case FIELD_WEAPON_1_ARC:
        MOX::_antaran_titan_defensive.weapon_1_arc = Read_Uint8_(Value);
        break;
    case FIELD_WEAPON_1_MODS:
        MOX::_antaran_titan_defensive.weapon_1_mods = Read_Mask_Uint16_(Value);
        break;
    case FIELD_WEAPON_1_AMMO:
        MOX::_antaran_titan_defensive.weapon_1_ammo = Read_Int16_(Value);
        break;
    case FIELD_WEAPON_1_AMOUNT:
        MOX::_antaran_titan_defensive.weapon_1_amount = Read_Uint8_(Value);
        break;
    }
}
```

This gives immediate live-memory updates without exposing a low-level byte-offset patch VM.

## Supported config operation shapes

The interpreter should support the following generic shapes.

### Scalar assignment

```text
disable_vsync = 1;
```

### Full bulk override

```text
mount_dmg = 50 100 125 75;
```

### Partial bulk override from a starting selector

```text
mount_dmg hef = 75 150 225;
```

### Skip-aware partial bulk override

```text
mount_dmg nohef pd = 50 - - 75;
```

### Cell patch

```text
building autolab cost = 200;
```

### Variable-length keyed payloads

```text
ai_race_variant_table Alkari hard v1 = 1 7 2;
```

These may require a custom helper if not expressible as layout-driven traversal.

## Why not a raw patch engine

The system does mutate live memory, but we do not want implementation to be expressed as raw storage patching.

Reasons:

- poor readability
- poor debuggability
- type knowledge moves out of the C++ type system
- higher risk of accidental misuse

Generated typed assignments provide the same runtime effect in a safer and clearer form.

## Why not replace config with Lua

Lua is not a replacement for this system.

Config files are declarative schema-driven updates. Lua would expose much more engine behavior and would weaken validation for data-only changes.

Lua may still be useful later for procedural behavior, but config remains the right mechanism for static table and rule mutation.

## Generator tool

The generator should be a native project executable built by CMake.

No new runtime dependencies should be added.

Recommended tool:

```text
tools/config_codegen.cpp
```

Inputs:

- external `config.json`
- project `config_bindings.json`

Outputs:

- generated ids and enums header
- generated decode implementation
- generated apply implementation
- generated layout metadata

Suggested files:

```text
src/generated/config_ids.h
src/generated/config_decode.cpp
src/generated/config_apply.cpp
```

## CMake integration

The intended flow is:

1. Build the generator executable.
2. Run it as a custom command.
3. Emit generated `.h` and `.cpp` files.
4. Compile those files into the main target.

This makes the interpreter a static generated loader.

## Error handling

The config system should report errors in three phases.

### Parse errors

- malformed statements
- unterminated string
- invalid integer syntax
- include failure

### Decode errors

- unknown parameter name
- invalid selector name
- invalid enum or mask token
- wrong selector count
- wrong rhs arity
- out-of-range numeric value

### Generation/apply errors

- missing semantic binding for known parameter or field
- unsupported generated layout shape
- compile-time type mismatch between schema and internal target field

Compile-time type mismatch is considered useful and should not be hidden.

## Implementation order

1. Add syntax-only CFG parser and opaque raw operation container.
2. Keep the selector validation helper script.
3. Define `config_bindings.json` schema.
4. Implement native code generator.
5. Generate a small subset of parameters first.
6. Add support for skip-aware bulk notation in decoded operations.
7. Wire generated interpreter into startup.
8. Expand parameter coverage gradually.

## Non-goals for the first version

- full support for every parameter immediately
- Lua exposure for internal data tables
- a generic byte-offset patch VM
- a semantic object-loader framework unrelated to config

The first version should focus on:

- correct parsing
- schema-driven decode
- generated logical layouts
- generated semantic field mapping
- direct typed live-memory updates
