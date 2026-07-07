# Config system design

## Goal

Add support for the 1.50 config format as a native feature of this project.

The config system should:

- parse CFG files, including includes and commands
- decode symbolic names and values using the external schema
- interpret config operations directly against live globals and global tables
- generate the decode layer from `config.json`
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

### 3. Decoded selectors must become numeric and resolve to logical positions

After schema decode, every selector after the primary parameter name should resolve to a numeric selector id or numeric literal.

For table-like parameters, decode should continue one step further and resolve the lhs selector sequence to a schema logical start position.

For a regular row/column table, the logical position is the flattened schema index:

```text
logical_position = row_index * num_columns + column_index
```

For rowless flat tables, the logical position is just the column index.

For single-column row-grouped tables, the logical position naturally advances through successive rows.

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

Additional validation against the full mod corpus confirmed:

- parameter names are schema-known
- symbolic lhs selectors are schema-decodable
- numeric lhs selectors are also used in real configs and must be accepted

Examples seen in real mod files:

```text
building 21 maintenance = 1;
weapon 3 13 = hv,pd,co,nr,af;
```

So the decode layer must support both symbolic selector names and numeric selector addressing.

The important boundary is:

- Stage 2 resolves selector syntax into schema-level numeric identities, logical positions, and stable `schema_field_id` values
- Stage 3 consumes those decoded writes and maps `schema_field_id` values to actual field writes

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

Additional validation against mod CFG files showed that decode must accept more than one surface encoding for the same schema type:

- symbolic enum names
- numeric enum ids
- symbolic mask codes
- symbolic mask value `none`
- numeric mask values
- binary integer mask notation

So `config.json` still defines the type, but decode must allow both symbolic and numeric forms when mods use them.

## Validation findings from existing mod CFG files

The current mod corpus under `C:\Projects\150\150\mods` was scanned with `tools/validate_config_lhs_numeric.py`.

Validated corpus summary:

- 36 CFG files scanned
- 6712 assignments checked
- 6073 table assignments
- 3121 bulk/table-list assignments
- 141 skip-aware assignments using `-`
- 0 unknown parameter names
- 0 selector decode errors

This confirms that the design direction is correct:

- schema-known parameter names are sufficient
- schema-driven selector decoding is sufficient
- bulk notation is heavily used in real mods
- skip-aware traversal is required
- generated layout-driven interpretation is the right execution model

It also revealed several important requirements that were not fully captured in the first draft:

- numeric lhs selectors are used in real configs
- enum-valued fields may be written using symbolic names or numeric ids
- mask-valued fields may be written using symbolic masks, symbolic `none`, numeric masks, or binary integer literals
- ratio-valued fields are used in real configs and need dedicated readers
- single-column row-grouped tables may still use bulk traversal across successive rows

Example of real bulk traversal across successive rows in a single-column table:

```text
skill_cost famous = 3 6 - - 2 4;
```

So the interpreter must not assume that bulk traversal only walks successive columns inside one row. Depending on schema shape, traversal may walk the flattened logical ordering of multiple row entries as well.

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

Validation against the real mod corpus confirmed that this is common and not just a theoretical/manual feature.

Bulk traversal must be defined against generated logical layout, not against handwritten table-specific rules.

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

Validation against existing mods confirmed active use of skip markers in multiple tables.

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

This same mechanism must also support other real-world cases such as:

```text
weapon crystal_ray 8 = 80 - - - - 2;
building warp_interdictor 3 = - 4;
skill_cost famous = 3 6 - - 2 4;
```

The common rule is:

- resolve starting logical position from the supplied lhs selectors
- walk forward through generated logical layout
- for each rhs item, either skip or decode-and-assign the current logical field
- advance logical position after every rhs item

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
- decoded lhs selector shape to a schema logical start position
- logical positions to stable `schema_field_id` values
- enum values to numeric values
- mask codes to numeric bitmasks
- numeric enum ids to enum values where legal
- numeric selector addressing to selector positions where legal
- raw value shape to decoded operation structure

This layer is generated from `config.json`.

It should not depend on project-owned semantic bindings, because decode is an external-schema concern rather than a live-target concern.

Output is a flat decoded write stream using:

- numeric parameter ids
- resolved schema logical indices
- resolved `schema_field_id` values
- typed decoded values

A single parsed assignment may expand into zero or more decoded logical writes.

Examples:

- a scalar assignment expands to one decoded write
- a bulk table assignment expands to one decoded write per non-skip rhs item
- skip markers affect logical traversal during Stage 2 but do not need to be emitted as writes

Decoded numeric selectors may still be retained for diagnostics, but Stage 3 should not need to re-resolve selector syntax or replay rhs traversal.

Decode must support the following surface forms observed in real configs:

- symbolic selector names
- numeric selector ids
- symbolic enum names
- numeric enum ids
- symbolic mask codes such as `hv,pd,co`
- symbolic mask value `none`
- numeric masks such as `18`
- binary numeric masks such as `00000000000010010`
- ratios such as `1/2`

These mask forms are not theoretical. Existing mod CFG files use all of the following forms today:

- `none`
- `hv`
- `pd`
- `hv,pd,co,nr`
- `00000000000010010`

### Stage 3: generated layout-driven interpreter

Generated code interprets decoded operations against live globals.

This layer is driven by:

- `config.json`
- a project-owned semantic bindings file

So the generation split is:

- Stage 2 generation uses `config.json` as its source
- Stage 3 generation uses `config.json` plus semantic bindings as its sources

Stage 3 should treat decoded logical writes as its input.

For each decoded write, Stage 3 may derive:

```text
row_index = logical_index / num_columns
column_index = logical_index % num_columns
```

Then:

- `row_index` selects the row/object/array entry to mutate
- `column_index` identifies the per-row field slot within schema order
- `schema_field_id` identifies the stable schema field to map
- semantic bindings map `schema_field_id` to the actual typed target write

No raw memory offsets are involved in this model.

The key rule is:

- traversal order and `schema_field_id` generation come from `config.json`
- actual field assignment targets come from generated semantic mapping

### Stage 4: optional handwritten custom helpers

Most regular parameters and tables should be generated.

Some irregular cases may still require custom helpers, for example:

- variable-length encoded payloads
- special packing rules
- application logic not expressible as regular field traversal

These should be explicitly marked as custom in the bindings file.

## Decoded runtime model

The generated interpreter should work on decoded logical writes.

Conceptually:

```cpp
struct s_value {
    int32_t kind;
    int32_t int_value;
    const char* string_value;
};

struct s_decoded_write {
    int32_t parameter_id;
    int32_t logical_index;
    int32_t schema_field_id;
    s_value value;
};
```

Stage 2 is responsible for:

- resolving the starting logical index from lhs selectors
- walking logical positions across rhs items
- applying skip semantics
- resolving `schema_field_id` for each emitted logical write
- decoding each non-skip rhs item to a typed value
- emitting one `s_decoded_write` per actual write

For scalar parameters, `logical_index` can be `0`.

For example:

```text
weapon_1_type = 11 - 00000000000000010 - 8
```

should decode to five rhs items, two of which are skip values.

The decoded value model must also be able to represent:

- integer values
- ratio values
- string values
- explicit skip values

Enum and mask values should normally be decoded into integer form before application.

## Generated schema field ids and apply layout

The interpreter must not hardcode table traversal shape by hand.

Instead, the generator should emit stable schema field ids from `config.json` and apply metadata from `config_bindings.json`.

Conceptually:

```cpp
struct s_layout_item {
    int32_t schema_field_id;
    int32_t target_field_id;
};
```

The purpose of `s_layout_item` is:

- connect stable external schema fields to internal semantic target fields

It should not duplicate the field type, because the type already comes from the schema.

`s_layout_item` is generated from both:

- external schema field definitions from `config.json`
- internal field mapping from `config_bindings.json`

Stage 2 traversal still comes from generated schema ordering in `config.json`. Stage 3 uses generated layout items to map `schema_field_id` values to concrete write targets.

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

If an operation starts at `weapon_1_type`, Stage 2 resolves that to a starting logical index, derives the corresponding `schema_field_id`, and then walks forward across rhs items.

For some schema shapes, the generated logical layout will flatten more than one conceptual axis.

Example: `skill_cost` has one row group and one column, but real mod files use bulk notation starting from one row and then continue into successive rows. So generated schema traversal for such parameters must flatten row-major logical order and not assume bulk traversal is column-only.

## Semantic bindings file

Because `config.json` has no knowledge of project globals or struct fields, a second input file is required.

This document calls it `config_bindings.json`.

Its purpose is to define semantic mapping from external config fields to actual internal globals and fields.

It should not duplicate value types from `config.json`.

It should define:

- which parameters are generated
- which parameters are custom
- target global/table expression for generated parameters
- mapping from generated `schema_field_id` values to internal field names or target field ids

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
- generated mapping from `schema_field_id` to target field id

The generated interpreter must not hardcode semantic chunks like "weapon_1_type implies exactly 5 following fields" as handwritten behavior. Those traversal relationships belong to Stage 2 traversal plus generated layout metadata.

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

Stage 2 logical-start resolution must be generated from schema shape. It must support:

The equivalent logical-start resolution belongs to Stage 2 decode, not Stage 3 apply. It must support:

- full-table start
- row start
- row-plus-column start
- column-only start for rowless tables
- numeric selector forms where mods use numeric addressing

### Stage 2 traversal for layout-driven tables

Conceptually:

```cpp
static void Decode_Operation_To_Writes_(
    const s_schema_parameter& parameter,
    const s_raw_operation& operation,
    std::vector<s_decoded_write>& out_writes)
{
    int32_t logical_index = Resolve_Start_Logical_Index_(parameter, operation.lhs, operation.lhs_count);

    for (int32_t rhs_index = 0; rhs_index < operation.rhs_count; ++rhs_index, ++logical_index) {
        const s_raw_value& raw_value = operation.rhs[rhs_index];

        if (raw_value.kind == CONFIG_VALUE_SKIP) {
            continue;
        }

        out_writes.push_back({
            parameter.id,
            logical_index,
            Resolve_Schema_Field_Id_(parameter, logical_index),
            Decode_Value_For_Logical_Index_(parameter, logical_index, raw_value),
        });
    }
}
```

### Stage 3 generated typed field writes

Conceptually:

```cpp
static void Apply_Decoded_Write_(
    const s_parameter_binding& binding,
    const s_decoded_write& write)
{
    int32_t row_index = write.logical_index / binding.column_count;
    int32_t column_index = write.logical_index % binding.column_count;
    int32_t target_field_id = binding.schema_field_to_target_field[write.schema_field_id];

    switch (target_field_id) {
    case FIELD_WEAPON_1_TYPE:
        MOX::_antaran_titan_defensive.weapon_1_type = Read_Uint16_(write.value);
        break;
    case FIELD_WEAPON_1_ARC:
        MOX::_antaran_titan_defensive.weapon_1_arc = Read_Uint8_(write.value);
        break;
    case FIELD_WEAPON_1_MODS:
        MOX::_antaran_titan_defensive.weapon_1_mods = Read_Mask_Uint16_(write.value);
        break;
    case FIELD_WEAPON_1_AMMO:
        MOX::_antaran_titan_defensive.weapon_1_ammo = Read_Int16_(write.value);
        break;
    case FIELD_WEAPON_1_AMOUNT:
        MOX::_antaran_titan_defensive.weapon_1_amount = Read_Uint8_(write.value);
        break;
    }
}
```

This gives immediate live-memory updates without exposing a low-level byte-offset patch VM.

For rowless flat tables, `binding.column_count` is the full logical field count and `row_index` remains `0`. For regular row tables, `row_index` selects the target entry and `schema_field_id` identifies the stable schema field to map.

## Supported config operation shapes

The interpreter should support the following generic shapes.

### Scalar assignment

```text
disable_vsync = 1;
```

Also observed in real configs with enum names and numeric enum ids.

### Full bulk override

```text
mount_dmg = 50 100 125 75;
```

### Partial bulk override from a starting selector

```text
mount_dmg hef = 75 150 225;
```

This was confirmed to be a real required pattern conceptually and matches the documented manual behavior.

### Skip-aware partial bulk override

```text
mount_dmg nohef pd = 50 - - 75;
```

Confirmed in real mod files with many variations.

### Cell patch

```text
building autolab cost = 200;
```

Real mod files also use numeric selector forms such as:

```text
building 21 maintenance = 1;
building recyclotron 5 = 4;
```

### Variable-length keyed payloads

```text
ai_race_variant_table Alkari hard v1 = 1 7 2;
```

These may require a custom helper if not expressible as layout-driven traversal.

At the moment, most real-world irregularity appears to be representable by generated layout plus typed readers. Custom helpers should remain the fallback, not the default.

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

- external `config.json` for generated ids, decode tables, selector decode, enum decode, mask decode, and schema-driven value readers
- project `config_bindings.json` for generated live-target bindings and apply logic

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

In other words:

- Stage 2 should be emitted into generated decode artifacts such as `src/generated/config_decode.cpp`
- Stage 3 should be emitted into generated apply artifacts such as `src/generated/config_apply.cpp`

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
- invalid enum, mask, or ratio token
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
5. Generate Stage 2 decode code from `config.json`.
6. Generate a small subset of Stage 3 apply bindings first.
7. Add support for skip-aware bulk notation in decoded operations.
8. Wire generated interpreter into startup.
9. Expand parameter coverage gradually.

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
