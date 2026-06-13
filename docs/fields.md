# Fields module

This document is a developer-facing guide to the `fields` module implemented in `src/game/fields.cpp` and declared in `src/game/fields.h`.

The field system is the game's generic UI/input layer. It provides a shared runtime for clickable regions, toggle buttons, list rows, scroll bars, text entry widgets, grid hit targets, circular hit regions, help overlays, and hotkey-only actions.

At a high level, the module does four jobs:

- stores a global list of active interactive regions
- renders those regions in a few different passes
- maps keyboard and mouse activity onto field ids or higher-level return values
- maintains transient UI state such as pressed-button visuals, active text input, and multi-key progress

The code is old-style and heavily stateful. It is best understood as a small UI runtime driven by global arrays and flags rather than as a modern widget tree.

## Source layout

- implementation: `src/game/fields.cpp`
- public API and typed payload helpers: `src/game/fields.h`
- core type definitions: `src/game/orion2.h`

## Core concepts

### The field list

All active interactive objects live in the global array:

- `fields::_fields`

The active count is:

- `fields::_fields_count`

Index `0` is effectively reserved. Most runtime code iterates from `1` to `_fields_count - 1`.

Each element is an `s_field`:

```cpp
struct s_field {
    int16_t x;
    int16_t y;
    int16_t x_end;
    int16_t y_end;
    int16_t field_type;
    int16_t font_style;
    s_colors* font_color;
    s_field_detail detail;
    char hotkey;
    int16_t sound;
};
```

The common header contains:

- a rectangular hit box: `x`, `y`, `x_end`, `y_end`
- the runtime type tag: `field_type`
- text style: `font_style`, `font_color`
- type-specific payload: `detail`
- an optional hotkey: `hotkey`
- a sound id to play on activation: `sound`

### Typed payloads

The module used to rely on raw union access patterns. It now exposes typed accessors in `fields.h` such as:

- `Field_Data_Button_()`
- `Field_Data_Radio_()`
- `Field_Data_Scroll_()`
- `Field_Data_String_List_()`

These helpers validate the runtime `field_type` before returning the relevant union member. If a caller requests the wrong payload type, the helper terminates through `Field_Type_Mismatch_()`.

That makes the current code much easier to reason about than the original anonymous-union style.

### Global runtime state

The field system is driven by a set of globals. The most important ones are:

- `_fields`, `_fields_count`: active field storage
- `_draw_fields_flag`: global render enable/disable switch
- `_down_mouse_button`: field currently considered pressed for visual feedback
- `_auto_input_variable`: most recently tracked field during some drag/hold flows
- `_input_field_active`: whether a continuous input field is active
- `_active_input_field_number`: active continuous input field index
- `_continuous_string[256]`: scratch buffer for continuous input editing
- `_cursor_count`, `_cursor_position`, `_cursor_on`: cursor/blink state used by text rendering
- `_multi_hotkey_active_field`: enables multi-key sequence handling
- `_help_struct_pointer`, `_help_list_count`, `_help_list_active`: active help-box overlay state
- `_field_box_mode`, `_help_box_mode`: debug-style outline overlays
- `_mouse_cancel_disabled`: disables normal right-click/escape cancel behavior
- `_mouse_auto_exit`: changes some hold-loop behavior to exit immediately

These values are not isolated per screen. Callers are expected to manage lifecycle explicitly by clearing or rebuilding fields when screen state changes.

## Lifecycle and ownership model

The normal lifecycle is:

1. call `Clear_Fields_()`
2. add field entries with one or more `Add_*` functions
3. optionally configure help boxes or auto functions
4. draw via `Draw_Visible_Fields_()` / `Draw_Visible_Fields_Back_()` / `Draw_All_Fields_()`
5. poll input via `Get_Input_()` or lower-level helpers
6. interpret the returned field id or action code in the caller

There is no ownership abstraction for per-field payload memory. Most payloads store pointers back into caller-owned state:

- text strings
- selection variables
- scroll values
- list selection variables
- animation headers

Callers must ensure those pointed-to objects remain valid while the field exists.

## Field types

The system uses a tagged union. The main active types are described below.

### FIELD_TYPE_BUTTON

Payload:

```cpp
struct s_field_button {
    const char* help;
    s_animation_header* pic;
    int16_t draw_style;
};
```

Purpose:

- standard rectangular button with sprite and centered text

Constructors:

- `Add_Button_Field_()`
- `Add_Clipped_Button_Field_()`

Behavior:

- hit-tested by the common rectangle logic
- drawn in normal or pressed style depending on context
- may be activated by mouse or hotkey

### FIELD_TYPE_IRREGULAR_BUTTON

Payload:

```cpp
struct s_field_irregular_button {
    const char* help;
    s_animation_header* pic;
    int16_t draw_style;
    int16_t draw_x;
    int16_t draw_y;
};
```

Purpose:

- button whose clickable rectangle and sprite draw origin are different

Constructor:

- `Add_Irregular_Button_Field_()`

Behavior:

- hit box still uses the shared `s_field` rectangle
- sprite draw uses `draw_x` / `draw_y`
- only some draw paths need the irregular payload directly

### FIELD_TYPE_RADIO

Payload:

```cpp
struct s_field_radio {
    const char* help;
    s_animation_header* pic;
    int16_t draw_style;
    int16_t* selection;
};
```

Purpose:

- two-state toggle button

Constructor:

- `Add_Radio_Button_Field_()`

Behavior:

- activation toggles `*selection` between `0` and `1`
- render path changes according to selection state

### FIELD_TYPE_LOCKED

Payload:

```cpp
struct s_field_locked {
    const char* help;
    s_animation_header* pic;
    int16_t draw_style;
    int16_t* selection;
};
```

Purpose:

- one-way latch button used like a lock/confirm toggle

Constructor:

- `Add_Locked_Button_Field_()`

Behavior:

- activation sets `*selection` to `1` if it was `0`
- unlike radio, it does not toggle back off through normal activation logic

### FIELD_TYPE_MULTI_BUTTON

Payload:

```cpp
struct s_field_multi_button {
    const char* help;
    s_animation_header* pic;
    int16_t draw_style;
    int16_t* selection;
    int16_t button_value;
};
```

Purpose:

- group of buttons sharing one selection variable

Constructor:

- `Add_Multi_Button_Field_()`

Behavior:

- pressed/selected state is based on `*selection == button_value`
- `Draw_Field_()` writes the shared selection directly for some action paths

### FIELD_TYPE_SCROLL

Payload:

```cpp
struct s_field_scroll {
    int16_t return_value;
    int16_t set_value;
    int16_t range_min;
    int16_t range_max;
    int16_t is_vertical;
    int16_t* value;
};
```

Purpose:

- scroll bar / drag-adjustable numeric control

Constructor:

- `Add_Scroll_Field_()`

Behavior:

- `*value` is clamped between `return_value` and `set_value`
- geometry mapping is handled by `Find_Bar_Position_()`
- keyboard `+` and `-` paths call `Increment_Scroll_Bar_()` / `Decrement_Scroll_Bar_()`

### FIELD_TYPE_SCROLL_RETURN

Purpose:

- action-like variant of a scroll field using the same payload

Construction model:

- no separate constructor
- the code expects an existing scroll payload with `field_type` changed to `FIELD_TYPE_SCROLL_RETURN`

Behavior:

- mouse/keyboard activation returns `detail.scroll.return_value`
- some paths also force `*value = set_value`

### FIELD_TYPE_HIDDEN

Payload:

```cpp
struct s_field_hidden {
    uint8_t unused;
};
```

Purpose:

- invisible clickable or keyboard-only region

Constructors:

- `Add_Hidden_Field_()`
- `Add_Clipped_Hidden_Field_()`
- `Add_Hot_Key_()`

Behavior:

- has no meaningful visual payload
- commonly used for generic rectangular actions or off-screen hotkey bindings

### FIELD_TYPE_MULTI_HOTKEY

Payload:

```cpp
struct s_field_multi_hotkey {
    const char* help;
    int16_t help_start;
    int16_t help_len;
};
```

Purpose:

- stores progress for multi-key sequences like `AB`

Constructor:

- `Add_Multi_Hot_Key_Field_()`

Behavior:

- `help` points to the sequence string
- `help_start` is the current matched offset
- matching logic lives in `Check_Multi_Hot_Key_Fields_()`

### FIELD_TYPE_STRING_LIST

Payload:

```cpp
struct s_field_string_list {
    s_colors* highlight_color;
    const char* help;
    int8_t* value;
    int16_t help_start;
    int16_t is_selectable;
    s_colors* gradient_colors;
    int16_t gradient_mode;
    uint8_t* gradient_palette;
    int32_t gradient_extra;
    s_animation_header* gradient_pic;
};
```

Purpose:

- selectable or informational row in a list popup

Constructor:

- `Add_String_List_Field_()`

Behavior:

- `help` is the row text
- `value` points to the caller-owned current selection variable
- `help_start` stores the row's logical index/value
- `is_selectable` gates whether scan/input should treat the row as clickable
- optional gradient fields control special text/row styling

### FIELD_TYPE_CONTINUOUS_INPUT

Payload:

```cpp
struct s_field_cont_input {
    s_colors* highlight_color;
    int16_t max_length;
    char* buffer;
    int16_t full_store_flag;
    s_animation_header* pic;
    int16_t background_color;
    int16_t font_shadow_style;
};
```

Purpose:

- modern continuously-active text input widget

Constructor:

- `Add_Continuous_String_Input_Field_()`

Behavior:

- live edit state is mirrored into `_continuous_string`
- render passes may add or remove a trailing `_` cursor marker
- click and keyboard flows switch active field ownership and commit back into `buffer`

### FIELD_TYPE_GRID

Payload:

```cpp
struct s_field_grid {
    int16_t step_x;
    int16_t step_y;
    int16_t* grid_x_ptr;
    int16_t* grid_y_ptr;
};
```

Purpose:

- rectangular region that converts pointer position into grid coordinates

Constructor:

- `Add_Grid_Field_()`

Behavior:

- pointer coordinates are converted into column/row indexes
- values are clamped into valid range

### FIELD_TYPE_CIRCLE

Payload:

```cpp
struct s_field_circle {
    int16_t center_x;
    int16_t center_y;
    int32_t radius_squared;
};
```

Purpose:

- circle hit region stored inside a clipped rectangle

Constructor:

- `Add_Clipped_Circle_Field_()`

Behavior:

- first passes rectangle bounds test
- then checks squared distance against `radius_squared`

### FIELD_TYPE_LEGACY_INPUT

Payload:

```cpp
struct s_field_input_string {
    s_colors* highlight_color;
    char* buffer;
    int16_t background_color;
    int16_t draw_style;
    int16_t input_mode;
    int16_t max_length;
    char* cursor_color_ramp;
};
```

Purpose:

- older modal text entry field

Construction status:

- active editing and draw logic still exists
- direct constructor path is not obvious in the current tree

Behavior:

- drawn by `Display_Input_Field_()`
- edited by `Input_String_()`
- unlike continuous input, it owns a local modal edit loop

## Rendering architecture

There are four important render entry points.

### `Draw_Field_(field_idx, action)`

Purpose:

- draw a single field, usually in normal or pressed state

Typical `action` meaning:

- `0`: normal draw
- `1`: pressed/active draw or interaction update, depending on field type

This function is also used as a small behavior dispatcher for some types:

- scroll fields call `Find_Bar_Position_()` on action `1`
- grid fields update grid coordinates on action `1`
- legacy input fields enter `Input_String_()` on action `1`

### `Draw_Visible_Fields_()`

Purpose:

- draw all active fields to the off page

Behavior summary:

- saves the current clip window
- resets the graphics window
- switches to the off page
- iterates active fields
- handles a pressed-field shortcut for `_down_mouse_button`
- draws each field according to type
- optionally draws debug field and help outlines
- restores the previous clip window

It also contains special rendering logic for:

- string lists
- continuous input fields
- scroll value clamping

### `Draw_Visible_Fields_Back_()`

Purpose:

- same overall role as `Draw_Visible_Fields_()`, but for the back page

It mirrors most of the off-page rendering behavior, including overlays and continuous-input rendering, but is implemented as a separate function rather than a shared helper.

### `Draw_All_Fields_()`

Purpose:

- force a full visible-field draw pass by temporarily setting `_draw_override_flag`

## Event handling architecture

The input side is layered rather than fully centralized.

### `Get_Input_()`

This is the common top-level polling entry point used by many callers.

It:

- respects `_input_delay`
- returns `0` when there are not enough active fields
- delegates to `Interpret_Mouse_Input_()`
- plays the selected field sound when the result is nonzero

Despite the name, it handles both mouse and keyboard activity because `Interpret_Mouse_Input_()` internally checks keyboard state first.

### `Interpret_Mouse_Input_()`

This is the main event router.

It handles:

- keyboard hotkeys that map to fields
- command keys like function keys and cancel keys
- continuous-input editing state
- buffered mouse clicks
- held-button drag/release behavior
- help-box right-click behavior
- circle hit-testing
- scroll return semantics

Its return conventions are important:

- positive value: activated field id or action value
- negative value: cancel or right-click variant of a field/action
- `0`: no action yet

### `Interpret_Keyboard_Input_()`

This helper translates a raw key into either:

- a field hotkey match
- a negative command code
- a multi-hotkey completion
- or the original key value when no match exists

The output field id is written through the pointer argument.

### `Scan_Field_()` and `Scan_Input_()`

`Scan_Field_()`:

- returns the first rectangular field under the pointer

`Scan_Input_()` adds type-specific behavior on top:

- scroll-return fields write `set_value` and return `return_value`
- string-list rows may be rejected when not selectable
- grid fields update coordinate outputs

### Text input flows

There are two distinct text systems.

#### Legacy input

- draw: `Display_Input_Field_()`
- edit loop: `Input_String_()`

This path is modal and self-contained.

#### Continuous input

- activation and editing state are managed in `Interpret_Mouse_Input_()`
- render passes mutate `_continuous_string` to add/remove a trailing `_`
- commit path uses `Copy_Continuous_String_()`

This path is more stateful and distributed.

## List popup flow

`Get_List_Field_()` builds a temporary interactive list out of fixed-size caller-owned records.

It does the following:

1. clears the current field list
2. creates one `FIELD_TYPE_STRING_LIST` field per row
3. creates an additional help/header row
4. normalizes the initial selection
5. enters a polling loop using `Get_Input_()`
6. optionally updates highlighted selection through `Scan_Input_()`
7. exits on accepted input, explicit list exit, or cancel
8. clears temporary fields before returning

Return value conventions:

- `-1`: cancel or explicit exit without accepted row selection
- non-negative: accepted row index as `input - 1`

## Help boxes

Help boxes are separate from fields.

Definition:

```cpp
struct s_help_box {
    int16_t help_id;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
};
```

The active list is managed by:

- `Set_Help_List_()`
- `Deactivate_Help_List_()`
- `Check_Help_List_()`

Behavior:

- right-click or help-trigger flows call `Check_Help_List_()`
- matching `help_id` is forwarded to `TEXTBOX::Draw_Help_Entry_()`
- render passes may also draw help-box outlines when `_help_box_mode` is enabled

## Auto functions

The module supports a global callback used during some wait, drag, input, and redraw flows.

Relevant functions:

- `Assign_Auto_Function_()`
- `Activate_Auto_Function_()`
- `Deactivate_Auto_Function_()`
- `Save_Auto_Function_()`
- `Restore_Auto_Function_()`
- `Invoke_Auto_Function_()`
- `Invoke_Saved_Auto_Function_()`
- `Quick_Call_Auto_Function_()`
- `Call_Auto_Function_()`

This mechanism is used for screen-specific refresh logic while the field system is waiting for input or tracking button holds.

## Debug and overlay modes

Two overlay modes are built into the renderer.

### Field outline mode

Controlled by:

- `_field_box_mode`
- `_field_outline_color`

When enabled, render passes draw a rectangle around each field except multi-hotkey fields.

### Help outline mode

Controlled by:

- `_help_box_mode`
- `_help_field_outline_color`

When enabled, render passes draw rectangles around active help boxes with valid `help_id` values.

## Construction helpers

The main field constructors are:

- `Add_Button_Field_()`
- `Add_Clipped_Button_Field_()`
- `Add_Hidden_Field_()`
- `Add_Clipped_Hidden_Field_()`
- `Add_Irregular_Button_Field_()`
- `Add_Radio_Button_Field_()`
- `Add_Locked_Button_Field_()`
- `Add_Multi_Button_Field_()`
- `Add_Scroll_Field_()`
- `Add_Grid_Field_()`
- `Add_Continuous_String_Input_Field_()`
- `Add_String_List_Field_()`
- `Add_Clipped_Circle_Field_()`
- `Add_Hot_Key_()`
- `Add_Multi_Hot_Key_Field_()`

Most constructors:

- append at `_fields[_fields_count]`
- populate the common header
- fill the relevant typed payload
- normalize lowercase ASCII hotkeys to uppercase
- return the new field index

## State save/restore helpers

The module contains some legacy save/restore support:

- `Mark_Fields_()`
- `Release_Fields_()`
- `Clear_Fields_Above_()`

These are used to temporarily build nested field sets and then restore the previous state.

## Practical guidance for developers

When modifying this module:

- prefer typed payload helpers in `fields.h` over direct union access
- treat `_fields_count` index `0` as reserved
- remember that many payload pointers are caller-owned, not copied
- keep hotkey normalization behavior consistent with existing constructors
- be careful with negative return values: they often mean cancel or right-click variants, not generic failure
- for new text behavior, first decide whether it belongs to legacy input or continuous input
- when touching draw logic, check both `Draw_Visible_Fields_()` and `Draw_Visible_Fields_Back_()` because they intentionally duplicate behavior

When debugging input bugs, start from this path:

1. `Get_Input_()`
2. `Interpret_Mouse_Input_()`
3. `Interpret_Keyboard_Input_()` / `Scan_Field_()` / `Scan_Input_()`
4. type-specific handlers such as `Find_Bar_Position_()`, `Input_String_()`, or `Copy_Continuous_String_()`

When debugging render bugs, start from:

1. constructor setup of `s_field`
2. `Draw_Field_()` for the single-field behavior
3. `Draw_Visible_Fields_()` and `Draw_Visible_Fields_Back_()` for pass-specific differences

## Summary

The `fields` module is a global, data-driven UI runtime built around an array of tagged interactive regions. It mixes rendering, event interpretation, editing state, and helper overlays in one subsystem. The modernized typed payload helpers now make the codebase much easier to navigate, but the runtime model is still fundamentally based on global state and explicit per-screen rebuilding.

If you understand:

- `s_field`
- the per-type payload structs
- `Draw_Visible_Fields_()` / `Draw_Visible_Fields_Back_()`
- `Interpret_Mouse_Input_()` / `Interpret_Keyboard_Input_()`

then you understand most of how the game's UI interaction layer works.