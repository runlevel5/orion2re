#pragma once

namespace fields {
    inline MOX2::action_func_t& _update_function = *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{ 0x0055AFD8 });
    inline uint16_t& _auto_active_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFDC });
    inline uint16_t& _list_field_on = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055AFF0});
    inline int16_t& _auto_function_delay = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055AFF4 });
    inline uint16_t& _list_field_scan_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B008});
    inline int16_t& _down_mouse_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFE4});
    inline int16_t& _auto_input_variable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B004});
    inline uint16_t& _input_field_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B040});
    inline int16_t& _active_input_field_number = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B04C});
    inline int16_t& _cursor_count =\
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B034});
    inline int16_t& _cursor_position = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055b038});
    inline int16_t& _cursor_on = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055b03c});
    inline int16_t& _help_list_active = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B05C});
    inline int16_t& _help_list_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B060});
    inline int16_t& _multi_hotkey_active_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B064});
    inline uint16_t& _draw_fields_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B078});
    inline const char* s_You_exceeded_the_max_number_of_fields = reinterpret_cast<const char*>(std::uintptr_t{0x0055B080});
    inline const char* s_Maximum_number_of_button_fields_e_0055b0b0 = reinterpret_cast<const char*>(std::uintptr_t{0x0055B0B0});
    inline char* s__00551100 = reinterpret_cast<char*>(std::uintptr_t{0x00551100});
    inline char*& DAT_0055afc0 = *reinterpret_cast<char**>(std::uintptr_t{0x0055AFC0});
    inline int32_t& _max_fields_count = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055AFC4});
    inline uint8_t& _exit_the_list_field = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AFCC});
    inline s_field*& _fields = *reinterpret_cast<s_field**>(std::uintptr_t{0x0055AFD0});
    inline s_field*& _old_fields = *reinterpret_cast<s_field**>(std::uintptr_t{0x0055AFD4});
    inline int16_t& _pointer_offset = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572BF8});
    inline uint16_t& _old_input_field_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572BE4});
    inline int16_t& _old_active_input_field_number = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572BF4});
    inline int16_t& _old_down_mouse_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005762E0});
    inline int16_t& _old_fields_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00576304});
    inline int16_t& _old_input_delay = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00576308});
    inline int16_t& _old_auto_input_variable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00576320});
    inline int16_t& _fields_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572D18});
    inline int16_t& _selected_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFE0});
    inline int16_t& mouse_saved_center_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572BE8});
    inline int16_t& mouse_saved_center_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0057630C});
    inline uint16_t& _pressed_mouse_x_value = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B068});
    inline uint16_t& _pressed_mouse_y_value = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B06C});
    inline int16_t& _pressed_button_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B070});
    inline int16_t& _last_button_number = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005762DC});
    inline int16_t& _last_button_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005762E4});
    inline int16_t& _last_button_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005762E8});
    inline s_colors (&_list_normal_color) = *reinterpret_cast<s_colors(*)>(std::uintptr_t{0x00572C08});
    inline s_colors (&_list_highlight_colors) = *reinterpret_cast<s_colors(*)>(std::uintptr_t{0x005762F0});
    inline s_colors (&_list_special_colors) = *reinterpret_cast<s_colors(*)>(std::uintptr_t{0x00572BD0});
    inline int16_t& _input_delay = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572C00});
    inline MOX2::action_func_t& _function = *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x005762F8});
    inline MOX2::action_func_t& _function2 = *reinterpret_cast<MOX2::action_func_t*>(std::uintptr_t{0x00572BFC});
    inline s_help_box*& _help_struct_pointer = *reinterpret_cast<s_help_box**>(std::uintptr_t{0x00576300});
    inline char* _continuous_string = reinterpret_cast<char*>(std::uintptr_t{0x00572c18});
    inline uint8_t& _active_input_field_first = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005762d8});
    inline uint16_t& _field_outline_color = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B020});
    inline uint16_t& _help_field_outline_color = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B024});
    inline int16_t& _selected_button_text_x_offset = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFE8});
    inline int16_t& _selected_button_text_y_offset = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFEC});
    inline uint16_t& _mouse_cancel_disabled = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B018});
    inline uint16_t& _dynamic_field_refresh = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B074});
    inline uint16_t& _field_box_mode = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B044});
    inline uint16_t& _help_box_mode = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B048});
    inline int16_t& _mouse_auto_exit = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B01C});
    inline uint32_t& _draw_override_flag = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055B07C});

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

    int16_t __cdecl Add_Clipped_Button_Field_(int16_t x, int16_t y, char* help, s_animation_header* pic, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Clipped_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Irregular_Button_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, char* help, s_animation_header* pic, int16_t help_start, int16_t background_color, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Multi_Hot_Key_Field_(const char* help_text);
    int16_t __cdecl Check_Multi_Hot_Key_Fields_(int16_t hotkey);

    int16_t __cdecl Auto_Input_();

    uint16_t __cdecl Pressed_Mouse_X_();

    uint16_t __cdecl Pressed_Mouse_Y_();

    int16_t __cdecl Add_Grid_Field_(int16_t x, int16_t y, int16_t step_x, int16_t step_y, int16_t cols, int16_t rows, short* grid_x, short* grid_y, char sound);

    int16_t __cdecl Add_Scroll_Field_(int16_t x, int16_t y, int16_t range_min, int16_t range_max, int16_t return_value, int16_t set_value, int16_t width, int16_t height, int16_t* value, const char* hotkey_str, char sound);

    int16_t __cdecl Add_Continuous_String_Input_Field_(int16_t x, int16_t y, int16_t width, int16_t height, char* value, int16_t max_length, int16_t background_color, s_animation_header* pic, int8_t sound, int16_t font_shadow_style);

    int16_t __cdecl Add_Multi_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* value, int16_t count, const char* hotkey, int8_t sound);

    int16_t __cdecl Add_Clipped_Circle_Field_(int16_t center_x, int16_t center_y, int16_t radius, char* hotkey, int8_t sound);

    int16_t __cdecl Add_Radio_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* variable, const char* hotkey, int8_t sound);

    void __cdecl Disable_Draw_Field_(int16_t field_idx);
    void __cdecl Enable_Draw_Field_(int16_t field_idx);

    void __cdecl Disable_Draw_All_Fields_();
    void __cdecl Enable_Draw_All_Fields_();

    void __cdecl Disable_Cancel_();

    void __cdecl Clear_Fields_Above_(int16_t f);

    int16_t __cdecl Get_Field_Counter_();

    void __cdecl Enable_Mouse_Pressed_Field_Update_();
    void __cdecl Disable_Mouse_Pressed_Field_Update_();

    void __cdecl Deactivate_Help_List_();

    void __cdecl Set_Help_List_(s_help_box* help_data, int16_t count);

    void __cdecl Add_Button_Info_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* hotkey, int8_t sound);

    void __cdecl Invoke_Auto_Function_();
    void __cdecl Invoke_Saved_Auto_Function_();

    void __cdecl F11_Key_();

    void __cdecl Push_Field_Down_(int16_t field_idx, int16_t x, int16_t y);

    void __cdecl Copy_Continuous_String_();

    int16_t __cdecl Interpret_Keyboard_Input_(int16_t* field_idx);

    void __cdecl Increment_Scroll_Bar_(int16_t field_idx);
    void __cdecl Decrement_Scroll_Bar_(int16_t field_idx);

    void __cdecl Display_Input_Field_(int16_t field_idx, char* input_buffer);

    void __cdecl Input_String_(int16_t field_idx);

    void __cdecl Find_Bar_Position_(int16_t field_idx);

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

    int16_t __cdecl Add_String_List_Field_(int16_t x, int16_t y, int16_t width, char* help, int16_t param_5, int8_t* value, int16_t help_start, int16_t gradient_mode, uint8_t* gradient_palette, int32_t gradient_extra, s_animation_header* gradient_pic, int32_t unused_param_12, char* hotkey_str, char* unused_param_14);

    void __cdecl Mark_Fields_();

    void __cdecl Clipped_Dot_(int16_t x, int16_t y, uint8_t color);

    void __cdecl Exit_List_Field_();

    void __cdecl Release_Fields_();

    short __cdecl Get_List_Field_(int16_t x, int16_t y, int16_t width, char* help, char* list_ptr, int32_t item_size, int8_t* selected_index, uint8_t* item_flags, int16_t gradient_mode, uint8_t* gradient_palette, int32_t gradient_extra, s_animation_header* gradient_pic, int32_t unused_param_13, char* unused_param_14);
}