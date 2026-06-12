# Fields documentation

## Purpose

This document is the explicit behavior map and runtime reference for the field system.

It is intended to answer:

- which field types are active,
- which constructors create them,
- which code paths read and write their payloads,
- which payload members are actually used,
- which screens exercise them,
- and how far each type has progressed through refactoring.

During the refactor, this file is the working source of truth for field behavior.

## How to use this document

For each active field type, record:

- the field type name,
- constructor entry points,
- draw paths,
- mouse input paths,
- keyboard input paths,
- payload members read,
- payload members written,
- known screens or callers,
- refactor notes,
- migration status.

## Migration status legend

- `unverified`: behavior not yet confirmed
- `mapped`: behavior and payload usage confirmed
- `migrating`: code is actively being moved to semantic payload types
- `migrated`: active code uses the new semantic payload model
- `removed`: dead legacy type or path deleted

## Field type behavior map

### FIELD_TYPE_BUTTON

- Status: mapped
- Constructors:
  - `Add_Button_Field_`
  - `Add_Clipped_Button_Field_`
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Visible_Fields_Back_`
  - `Draw_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Push_Field_Down_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.button.help`
  - `detail.button.pic`
  - `detail.button.draw_style`
- Payload members written:
  - `detail.button.help`
  - `detail.button.pic`
  - `detail.button.draw_style`
- Known screens/callers:
  - `colrefit.cpp`
  - `colony_main.cpp`
  - `cmbtdrw1.cpp`
  - `plntsum.cpp`
  - `raceopt.cpp`
  - `racerprt.cpp`
- Refactor notes:
  - base button payload family

### FIELD_TYPE_RADIO

- Status: mapped
- Constructors:
  - `Add_Radio_Button_Field_`
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Visible_Fields_Back_`
  - `Draw_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Push_Field_Down_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.radio.help`
  - `detail.radio.pic`
  - `detail.radio.draw_style`
  - `detail.radio.selection`
- Payload members written:
  - `detail.radio.help`
  - `detail.radio.pic`
  - `detail.radio.draw_style`
  - `detail.radio.selection`
  - `*detail.radio.selection`
- Known screens/callers:
  - `racesel.cpp`
- Refactor notes:
  - distinct from locked buttons even if visuals overlap

### FIELD_TYPE_LOCKED

- Status: mapped
- Constructors:
  - `Add_Locked_Button_Field_`
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Visible_Fields_Back_`
  - `Draw_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Push_Field_Down_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.radio.help`
  - `detail.radio.pic`
  - `detail.radio.draw_style`
  - `detail.radio.selection`
- Payload members written:
  - `detail.radio.help`
  - `detail.radio.pic`
  - `detail.radio.draw_style`
  - `detail.radio.selection`
  - `*detail.radio.selection`
- Known screens/callers:
  - `colsum.cpp`
- Refactor notes:
  - should receive its own semantic contract even if it shares some radio behavior

### FIELD_TYPE_MULTI_BUTTON

- Status: mapped
- Constructors:
  - `Add_Multi_Button_Field_`
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Visible_Fields_Back_`
  - `Draw_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Push_Field_Down_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.multibtn.help`
  - `detail.multibtn.pic`
  - `detail.multibtn.draw_style`
  - `detail.multibtn.selection`
  - `detail.multibtn.button_value`
  - `*detail.multibtn.selection`
- Payload members written:
  - `detail.multibtn.help`
  - `detail.multibtn.pic`
  - `detail.multibtn.draw_style`
  - `detail.multibtn.selection`
  - `detail.multibtn.button_value`
  - `*detail.multibtn.selection`
- Known screens/callers:
  - `design_main.cpp`
- Refactor notes:
  - shared selection variable plus per-field selected value

### FIELD_TYPE_SCROLL

- Status: mapped
- Constructors:
  - `Add_Scroll_Field_`
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Visible_Fields_Back_`
  - `Draw_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Scan_Input_`
  - `Find_Bar_Position_`
  - `Increment_Scroll_Bar_`
  - `Decrement_Scroll_Bar_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.scroll.return_value`
  - `detail.scroll.set_value`
  - `detail.scroll.range_min`
  - `detail.scroll.range_max`
  - `detail.scroll.is_vertical`
  - `detail.scroll.value`
  - `*detail.scroll.value`
- Payload members written:
  - `detail.scroll.return_value`
  - `detail.scroll.set_value`
  - `detail.scroll.range_min`
  - `detail.scroll.range_max`
  - `detail.scroll.is_vertical`
  - `detail.scroll.value`
  - `*detail.scroll.value`
- Known screens/callers:
  - `colrefit.cpp`
  - `cmbtdrw1.cpp`
- Refactor notes:
  - constructor initializes only `FIELD_TYPE_SCROLL`
  - normal scroll drag behavior is handled by `Find_Bar_Position_`, `Increment_Scroll_Bar_`, and `Decrement_Scroll_Bar_`

### FIELD_TYPE_SCROLL_RETURN

- Status: mapped
- Constructors:
  - no direct constructor found in the current tree
  - `Scan_Input_` and the input handlers expect an already-initialized scroll payload with `field_type` retagged to `FIELD_TYPE_SCROLL_RETURN`
- Draw paths:
  - `Draw_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Scan_Input_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.scroll.return_value`
  - `detail.scroll.set_value`
  - `detail.scroll.value`
- Payload members written:
  - `*detail.scroll.value`
- Known screens/callers:
  - no direct caller found in the current tree
- Refactor notes:
  - behaves as a scroll-derived action mode, not as a separately constructed payload family in current code

### FIELD_TYPE_HIDDEN

- Status: mapped
- Constructors:
  - `Add_Hidden_Field_`
  - `Add_Clipped_Hidden_Field_`
  - `Add_Hot_Key_`
- Draw paths:
  - none expected
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Scan_Field_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - none
- Payload members written:
  - none
- Known screens/callers:
  - widely used as generic clickable regions in `antaroom.cpp`, `colony_main.cpp`, `science.cpp`, `score.cpp`, `sys.cpp`, and many others
  - `Add_Hot_Key_` creates off-screen hidden fields for keyboard-only activation
- Refactor notes:
  - may not need a payload struct at all

### FIELD_TYPE_MULTI_HOTKEY

- Status: mapped
- Constructors:
  - `Add_Multi_Hot_Key_Field_`
- Draw paths:
  - none expected
- Mouse input paths:
  - `Interpret_Mouse_Input_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
  - `Check_Multi_Hot_Key_Fields_`
  - `Clear_Multi_Hot_Key_Offsets_`
- Payload members read:
  - `detail.multi_hotkey.help`
  - `detail.multi_hotkey.help_start`
  - `detail.multi_hotkey.help_len`
- Payload members written:
  - `detail.multi_hotkey.help`
  - `detail.multi_hotkey.help_start`
  - `detail.multi_hotkey.help_len`
- Known screens/callers:
  - `colony_main.cpp`
  - `score.cpp`
- Refactor notes:
  - should become a very small dedicated payload

### FIELD_TYPE_STRING_LIST

- Status: mapped
- Constructors:
  - `Add_String_List_Field_`
  - indirectly through list-building helpers
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Visible_Fields_Back_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Scan_Input_`
  - list field helpers
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.strings.help`
  - `detail.strings.value`
  - `detail.strings.help_start`
  - `detail.strings.is_selectable`
  - `detail.strings.highlight_color`
  - `detail.strings.gradient_colors`
  - `detail.strings.gradient_mode`
  - `detail.strings.gradient_palette`
  - `detail.strings.gradient_extra`
  - `detail.strings.gradient_pic`
  - `*detail.strings.value`
- Payload members written:
  - `detail.strings.help`
  - `detail.strings.value`
  - `detail.strings.help_start`
  - `detail.strings.is_selectable`
  - `detail.strings.highlight_color`
  - `detail.strings.gradient_colors`
  - `detail.strings.gradient_mode`
  - `detail.strings.gradient_palette`
  - `detail.strings.gradient_extra`
  - `detail.strings.gradient_pic`
  - `*detail.strings.value`
- Known screens/callers:
  - internal list-building paths in `fields.cpp` via `Add_List_Field_` style helpers
- Refactor notes:
  - dedicated row payload; do not treat as generic string storage

### FIELD_TYPE_CONTINUOUS_INPUT

- Status: mapped
- Constructors:
  - `Add_Continuous_String_Input_Field_`
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Visible_Fields_Back_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Copy_Continuous_String_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.cont_input.highlight_color`
  - `detail.cont_input.max_length`
  - `detail.cont_input.buffer`
  - `detail.cont_input.full_store_flag`
  - `detail.cont_input.pic`
  - `detail.cont_input.background_color`
  - `detail.cont_input.font_shadow_style`
- Payload members written:
  - `detail.cont_input.highlight_color`
  - `detail.cont_input.max_length`
  - `detail.cont_input.buffer`
  - `detail.cont_input.full_store_flag`
  - `detail.cont_input.pic`
  - `detail.cont_input.background_color`
  - `detail.cont_input.font_shadow_style`
  - `strcpy(detail.cont_input.buffer, ...)`
- Known screens/callers:
  - `cmbtdrw1.cpp`
  - `raceopt.cpp`
- Refactor notes:
  - separate from legacy inline input

### FIELD_TYPE_GRID

- Status: mapped
- Constructors:
  - `Add_Grid_Field_`
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Scan_Input_`
  - `Draw_Field_`
- Keyboard input paths:
  - none found beyond generic hotkey dispatch
- Payload members read:
  - `detail.grid.step_x`
  - `detail.grid.step_y`
  - `detail.grid.grid_x_ptr`
  - `detail.grid.grid_y_ptr`
  - `*detail.grid.grid_x_ptr`
  - `*detail.grid.grid_y_ptr`
- Payload members written:
  - `detail.grid.step_x`
  - `detail.grid.step_y`
  - `detail.grid.grid_x_ptr`
  - `detail.grid.grid_y_ptr`
  - `*detail.grid.grid_x_ptr`
  - `*detail.grid.grid_y_ptr`
- Known screens/callers:
  - `combat1.cpp`
  - `mainscr.cpp`
  - `movebox.cpp`
  - `sys.cpp`
- Refactor notes:
  - dedicated coordinate-conversion payload

### FIELD_TYPE_IRREGULAR_BUTTON

- Status: mapped
- Constructors:
  - `Add_Irregular_Button_Field_`
- Draw paths:
  - `Draw_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Push_Field_Down_`
- Keyboard input paths:
  - `Interpret_Keyboard_Input_`
- Payload members read:
  - `detail.button.help`
  - `detail.button.pic`
  - `detail.button.draw_style`
  - `detail.button.help_start`
  - `detail.button.background_color`
- Payload members written:
  - `detail.button.help`
  - `detail.button.pic`
  - `detail.button.draw_style`
  - `detail.button.help_start`
  - `detail.button.background_color`
- Known screens/callers:
  - `mainscr.cpp`
- Refactor notes:
  - currently reuses button payload storage and repurposes `help_start` and `background_color` as draw origin
  - explicit draw origin; do not reuse button payload members semantically in the refactor

### FIELD_TYPE_CIRCLE

- Status: mapped
- Constructors:
  - `Add_Clipped_Circle_Field_`
- Draw paths:
  - none expected for payload
- Mouse input paths:
  - `Interpret_Mouse_Input_`
- Keyboard input paths:
  - none found beyond generic hotkey dispatch
- Payload members read:
  - `detail.circle.center_x`
  - `detail.circle.center_y`
  - `detail.circle.radius_squared`
- Payload members written:
  - `detail.circle.center_x`
  - `detail.circle.center_y`
  - `detail.circle.radius_squared`
- Known screens/callers:
  - `mainscr.cpp`
- Refactor notes:
  - dedicated hit-test payload

### FIELD_TYPE_LEGACY_INPUT

- Status: mapped
- Constructors:
  - no direct constructor found in the current tree
- Draw paths:
  - `Draw_Visible_Fields_`
  - `Draw_Visible_Fields_Back_`
  - `Draw_Field_`
  - `Display_Input_Field_`
- Mouse input paths:
  - `Interpret_Mouse_Input_`
  - `Push_Field_Down_`
  - `Input_String_`
- Keyboard input paths:
  - handled indirectly by `Input_String_` once activated
- Payload members read:
  - `detail.input.max_length`
  - `detail.input.buffer`
  - `detail.input.cursor_color_ramp`
  - `detail.input.background_color`
  - `detail.input.input_mode`
- Payload members written:
  - `strcpy(detail.input.buffer, ...)`
- Known screens/callers:
  - no direct caller found in the current tree
- Refactor notes:
  - handler code is still live in `fields.cpp`, but the active constructor path was not located
  - verify reachability before migration; delete if dead

## Notes

- This file should become more concrete as each field family is verified.
- Once a family is fully mapped, replace `pending` entries with exact payload-member and call-site information.
- When a field family is migrated, update its status here before deleting the old runtime representation.