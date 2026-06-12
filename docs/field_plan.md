# Field refactor plan

## Goal

Refactor the field system away from decompiler-shaped, offset-guessing payload structs while staying with C-style types and unions.

The explicit field behavior map for this work lives in `docs/fields.md`.

The new design should:

- keep `s_field` as a plain runtime struct,
- keep a union-based payload model,
- give each active field behavior its own dedicated payload type,
- remove accidental cross-type aliasing,
- be safe on x86 and x64 native builds,
- avoid preserving 32-bit binary layout assumptions.

## Constraints and decisions

- Do not use C++ class hierarchies or `std::variant` for field payloads.
- Use C-style structs and a union for the payload.
- Keep common field metadata in `s_field`.
- Each active field type gets its own semantic payload struct.
- Add typed `as_...` helpers to centralize payload access.
- Migrate incrementally instead of rewriting the full subsystem at once.

## Current findings

The current field payload model is based on decompiler-labeled members such as `field14_0x18` and on union overlays that assume x86-era pointer placement.

That is unsafe for native x64 because pointer-bearing payload structs are relaid out by the compiler, so unrelated union members no longer share the same offsets in any meaningful way.

The active field behaviors currently appear to be:

- button,
- radio,
- locked button,
- multi-button,
- scroll,
- hidden,
- multi-hotkey,
- string-list,
- continuous-input,
- grid,
- irregular-button,
- circle hit-test.

Legacy input behavior and any remaining unnamed field types should be verified explicitly before deletion.

## Target runtime model

Keep `s_field` for common metadata only:

- bounds,
- field type,
- font style,
- font color,
- hotkey,
- sound.

Replace the current guessed payload structs with one dedicated payload struct per active field family.

Planned payload families:

- `s_field_button_data`
- `s_field_radio_data`
- `s_field_multi_button_data`
- `s_field_scroll_data`
- `s_field_string_list_data`
- `s_field_continuous_input_data`
- `s_field_grid_data`
- `s_field_multi_hotkey_data`
- `s_field_circle_data`
- `s_field_irregular_button_data`
- `s_field_hidden_data` only if hidden fields need extra payload, otherwise none
- `s_field_legacy_input_data` only if type 4 is confirmed live

The union should contain only semantic payload structs that match current runtime behavior.

## Behavioral cleanup rules

### 1. No cross-type payload reuse

Do not model one field type by reusing another type's payload with repurposed members.

Examples:

- irregular buttons should have explicit draw-origin members instead of reusing button members,
- any return-only scroll action should use either a dedicated flag in scroll payload or a dedicated payload type,
- list rows should use their own payload rather than relying on overlay compatibility.

### 2. One field type maps to one payload contract

Every active `FIELD_TYPE_*` value must have exactly one expected payload contract.

If two field types share behavior, that should be represented explicitly with:

- the same payload struct plus a mode flag, or
- separate field types with separate payloads,

but not by relying on union overlap.

### 3. Keep dead or unknown types quarantined

If a field type is not yet fully understood but is still reachable, isolate it behind a dedicated payload type and helper rather than folding it into another type prematurely.

## Helper strategy

Introduce typed access helpers for every active payload family.

Examples:

- `Field_Button_Data_(s_field*)`
- `Field_Radio_Data_(s_field*)`
- `Field_Scroll_Data_(s_field*)`
- `Field_String_List_Data_(s_field*)`
- `Field_Grid_Data_(s_field*)`

Helper rules:

- helpers should assert or validate that the field type matches the expected payload,
- helpers should return pointers to the dedicated payload struct,
- direct access to raw union members outside helper boundaries should be reduced steadily during migration.

These helpers are intended to be the transition seam that lets the code move away from the current direct `detail.xxx` access pattern.

## Migration phases

### Phase 1: freeze the current behavior map

Create a table of:

- field type,
- constructor entry points,
- draw paths,
- mouse input paths,
- keyboard input paths,
- payload fields actually read and written,
- known screens that exercise the field type.

Store and maintain that map in `docs/fields.md`.

This phase is complete only when each active type has a concrete behavior summary.

### Phase 2: define the new semantic payload structs

Add the new dedicated payload structs and a new union layout in the headers.

Do not remove the old code paths yet.

At this phase:

- field type names should be explicit,
- payload member names should describe behavior, not guessed offsets,
- old decompiler-style members should stop spreading into new code.

### Phase 3: add helper accessors

Implement `as_...` or `Field_..._Data_` access helpers for each payload family.

Update a small number of call sites first to prove the access pattern.

### Phase 4: migrate constructors family by family

Recommended migration order:

1. hidden
2. button
3. radio and locked button
4. multi-button
5. scroll
6. string-list
7. continuous-input
8. grid
9. multi-hotkey
10. circle
11. irregular-button
12. legacy input if still active

For each family:

- migrate constructor code,
- migrate draw logic,
- migrate input logic,
- replace direct union access with helper access,
- verify all references compile and behave consistently.

### Phase 5: normalize special-case behaviors

Resolve any remaining cases where a field type is represented as an action mode of another payload.

The end state should make those choices explicit instead of implicit.

### Phase 6: delete the old decompiler-shaped payload structs

Once all active code uses the new semantic payloads, remove:

- guessed `fieldN_0x..` members,
- inactive compatibility aliases,
- dead field type aliases,
- unused legacy payload structs.

### Phase 7: tighten guardrails

After migration, add guardrails that prevent regression.

## Guardrails

### Type and payload checks

- Every payload helper should verify the field type.
- Switches over field type should be exhaustive wherever practical.
- Constructor functions should initialize only the payload that belongs to their field type.

### Naming rules

- New payload members must use semantic names.
- Do not add new members named after guessed offsets.
- Do not add new cross-type alias members for convenience.

### Code organization rules

- All new runtime field access should go through helper functions or a very small set of localized direct accesses.
- Avoid spreading raw union knowledge across unrelated files.

### Validation rules

- Build on both x86 and x64.
- Exercise representative screens for every active field family.
- Re-scan for decompiler-style payload members after each migration group.

## Validation checklist

For each migrated family, validate:

- constructor initializes the correct payload,
- draw path reads only that payload,
- mouse interaction reads and writes only that payload,
- keyboard interaction reads and writes only that payload,
- no unrelated union member access remains for that family,
- x86 and x64 both compile cleanly.

Suggested runtime coverage:

- normal buttons,
- radio groups,
- locked buttons,
- multi-button groups,
- scroll bars,
- list selection rows,
- text entry screens,
- grid hit tests,
- irregular button screens,
- multi-hotkey behavior,
- circle hit-test behavior.

## End state

The desired end state is a runtime field system that still uses C-style structs and a union, but where the union is semantic rather than reconstructive.

`s_field` should describe a UI object in native C terms, not a guessed 32-bit memory image. The resulting code should be readable, type-directed, and stable across x86 and x64 builds.