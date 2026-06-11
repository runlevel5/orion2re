#pragma once

namespace fields {
    inline constexpr int32_t MAX_FIELDS_COUNT = 250;

    extern MOX2::action_func_t _update_function;
    extern uint16_t _auto_active_flag;
    extern uint16_t _list_field_on;
    extern int16_t _auto_function_delay;
    extern uint16_t _list_field_scan_flag;
    extern int16_t _down_mouse_button;
    extern int16_t _auto_input_variable;
    extern uint16_t _input_field_active;
    extern int16_t _active_input_field_number;
    extern int16_t _cursor_count;
    extern int16_t _cursor_position;
    extern int16_t _cursor_on;
    extern int16_t _help_list_active;
    extern int16_t _help_list_count;
    extern int16_t _multi_hotkey_active_field;
    extern uint16_t _draw_fields_flag;
    extern char s__00551100[1];
    extern char* DAT_0055afc0;
    extern int32_t _max_fields_count;
    extern uint8_t _exit_the_list_field;
    extern s_field* _fields;
    extern s_field* _old_fields;
    extern int16_t _pointer_offset;
    extern uint16_t _old_input_field_active;
    extern int16_t _old_active_input_field_number;
    extern int16_t _old_down_mouse_button;
    extern int16_t _old_fields_count;
    extern int16_t _old_input_delay;
    extern int16_t _old_auto_input_variable;
    extern int16_t _fields_count;
    extern int16_t _selected_count;
    extern int16_t mouse_saved_center_y;
    extern int16_t mouse_saved_center_x;
    extern uint16_t _pressed_mouse_x_value;
    extern uint16_t _pressed_mouse_y_value;
    extern int16_t _pressed_button_flag;
    extern int16_t _last_button_number;
    extern int16_t _last_button_x;
    extern int16_t _last_button_y;
    extern s_colors _list_normal_color;
    extern s_colors _list_highlight_colors;
    extern s_colors _list_special_colors;
    extern int16_t _input_delay;
    extern MOX2::action_func_t _function;
    extern MOX2::action_func_t _function2;
    extern s_help_box* _help_struct_pointer;
    extern char _continuous_string[256];
    extern uint8_t _active_input_field_first;
    extern uint16_t _field_outline_color;
    extern uint16_t _help_field_outline_color;
    extern int16_t _selected_button_text_x_offset;
    extern int16_t _selected_button_text_y_offset;
    extern uint16_t _mouse_cancel_disabled;
    extern uint16_t _dynamic_field_refresh;
    extern uint16_t _field_box_mode;
    extern uint16_t _help_box_mode;
    extern int16_t _mouse_auto_exit;
    extern uint32_t _draw_override_flag;

    void __cdecl Assign_Update_Function_(MOX2::action_func_t fn);
    void __cdecl Save_Auto_Function_();
    void __cdecl Deactivate_Auto_Function_();
    void __cdecl Activate_Auto_Function_();
    void __cdecl Restore_Auto_Function_();
    void __cdecl Clear_Fields_();

    int16_t __cdecl Add_Hot_Key_(const char* hotkey_str);

    void __cdecl Assign_Auto_Function_(MOX2::action_func_t fn, int16_t delay);

    int16_t __cdecl Get_Input_();

    void __cdecl Enable_Mouse_Wait_();
    void __cdecl Disable_Mouse_Wait_();

    int16_t __cdecl Interpret_Mouse_Input_();

    int16_t __cdecl Scan_Input_();

    int16_t __cdecl Add_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* text, int8_t key);

    int16_t __cdecl Add_Clipped_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Clipped_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int8_t sound);

    // Type 13 irregular buttons reuse the button payload, but the two legacy short
    // fields at detail+0x1c/+0x20 are alternate sprite draw coordinates rather than
    // help metadata.
    int16_t __cdecl Add_Irregular_Button_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* help, s_animation_header* pic, int16_t draw_x, int16_t draw_y, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Multi_Hot_Key_Field_(const char* help_text);
    int16_t __cdecl Check_Multi_Hot_Key_Fields_(int16_t hotkey);

    int16_t __cdecl Auto_Input_();

    uint16_t __cdecl Pressed_Mouse_X_();

    uint16_t __cdecl Pressed_Mouse_Y_();

    /* Adds a grid field that converts mouse position into grid coordinates.
     * grid_x: Receives the selected column.
     * grid_y: Receives the selected row.
     */
    int16_t __cdecl Add_Grid_Field_(int16_t x, int16_t y, int16_t step_x, int16_t step_y, int16_t cols, int16_t rows, int16_t* grid_x, int16_t* grid_y, int8_t sound);

    /* Adds a scroll field that updates a bound value within the supplied range.
     * value: Pointer to the value controlled by the scroll field.
     * hotkey_str: Optional hotkey string; only the first character is used.
     */
    int16_t __cdecl Add_Scroll_Field_(int16_t x, int16_t y, int16_t range_min, int16_t range_max, int16_t return_value, int16_t set_value, int16_t width, int16_t height, int16_t* value, const char* hotkey_str, int8_t sound);

    // Type 11 is the newer continuous editor used by screen text boxes.
    // It uses detail.cont_input and is distinct from the older type 4 inline editor.
    int16_t __cdecl Add_Continuous_String_Input_Field_(int16_t x, int16_t y, int16_t width, int16_t height, char* value, int16_t max_length, int16_t background_color, s_animation_header* pic, int8_t sound, int16_t font_shadow_style);

    // Type 3 multi-buttons share one selection variable across several button fields.
    // The payload stores the shared selection pointer plus this field's selected value.
    int16_t __cdecl Add_Multi_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* value, int16_t button_value, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Clipped_Circle_Field_(int16_t center_x, int16_t center_y, int16_t radius, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Radio_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* variable, const char* hotkey, int8_t sound);

    void __cdecl Disable_Draw_Field_(int16_t field_idx);
    void __cdecl Enable_Draw_Field_(int16_t field_idx);

    void __cdecl Disable_Draw_All_Fields_();
    void __cdecl Enable_Draw_All_Fields_();

    void __cdecl Disable_Cancel_();

    /* Truncates the field list above the supplied field and clears pending mouse input.
     * field_idx: Highest field index to keep.
     */
    void __cdecl Clear_Fields_Above_(int16_t field_idx);

    int16_t __cdecl Get_Field_Counter_();

    void __cdecl Enable_Mouse_Pressed_Field_Update_();
    void __cdecl Disable_Mouse_Pressed_Field_Update_();

    void __cdecl Deactivate_Help_List_();

    /* Registers the active help-box array used for right-click help lookup.
     * help_data: Pointer to the help-box records.
     * count: Number of help-box records.
     */
    void __cdecl Set_Help_List_(s_help_box* help_data, int16_t count);

    void __cdecl Invoke_Auto_Function_();
    void __cdecl Invoke_Saved_Auto_Function_();

    void __cdecl F11_Key_();

    void __cdecl Push_Field_Down_(int16_t field_idx, int16_t x, int16_t y);

    void __cdecl Copy_Continuous_String_();

    int16_t __cdecl Interpret_Keyboard_Input_(int16_t* field_idx);

    void __cdecl Increment_Scroll_Bar_(int16_t field_idx);
    void __cdecl Decrement_Scroll_Bar_(int16_t field_idx);

    // Type 4 is the older inline input field renderer/editor.
    // It uses detail.input, including input_mode and a cursor color ramp.
    void __cdecl Display_Input_Field_(int16_t field_idx, char* input_buffer);

    void __cdecl Input_String_(int16_t field_idx);

    void __cdecl Find_Bar_Position_(int16_t field_idx);

    /* Checks whether the mouse is inside any registered help box and draws that help entry.
     * Returns 0 when a help entry was shown, or 1 when no help box matched.
     */
    int16_t __cdecl Check_Help_List_();

    void __cdecl Clear_Multi_Hot_Key_Offsets_();

    uint8_t __cdecl Get_Alt_Alpha_Key_(int16_t keycode);

    void __cdecl Draw_Field_(int16_t field_idx, int16_t action);

    void __cdecl Call_Auto_Function_();

    int16_t __cdecl Scan_Field_();

    void __cdecl Clear_Mouse_Buffer_();

    void __cdecl Quick_Call_Auto_Function_();

    void __cdecl Timed_Wait_For_Input_(int32_t timeout);

    void __cdecl Draw_Visible_Fields_();

    void __cdecl Draw_All_Fields_();
    void __cdecl Set_Input_Delay_(int16_t delay);
    void __cdecl Set_Field_Outline_Color_(uint16_t color);

    void __cdecl Draw_Visible_Fields_Back_();

    int16_t __cdecl Add_Locked_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* selection, const char* hotkey, int8_t sound);

    // Type 10 list rows store a per-row selectability flag at detail+0x2.
    int16_t __cdecl Add_String_List_Field_(int16_t x, int16_t y, int16_t width, const char* help, int16_t is_selectable, int8_t* value, int16_t help_start, int16_t gradient_mode, uint8_t* gradient_palette, int32_t gradient_extra, s_animation_header* gradient_pic, int32_t unused_param_12, const char* hotkey_str, const char* unused_param_14);

    void __cdecl Mark_Fields_();

    void __cdecl Clipped_Dot_(int16_t x, int16_t y, uint8_t color);

    void __cdecl Exit_List_Field_();

    void __cdecl Release_Fields_();

    /* Builds a temporary selectable list from fixed-size records and returns the chosen row.
     * list_ptr: Pointer to the first list record.
     * item_size: Byte stride between records.
     * selected_index: In/out selected row index.
     */
    int16_t __cdecl Get_List_Field_(int16_t x, int16_t y, int16_t width, const char* help, const char* list_ptr, int32_t item_size, int8_t* selected_index, uint8_t* item_flags, int16_t gradient_mode, uint8_t* gradient_palette, int32_t gradient_extra, s_animation_header* gradient_pic, int32_t unused_param_13, const char* unused_param_14);
}