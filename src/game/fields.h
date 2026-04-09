#pragma once

namespace fields {
    inline void*& _update_function = *reinterpret_cast<void**>(std::uintptr_t{ 0x0055AFD8 });
    inline uint16_t& _auto_active_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFDC });
    inline uint16_t& _auto_function_delay = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFF4 });
    inline int16_t& _down_mouse_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFE4});
    inline int16_t& _auto_input_variable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B004});
    inline uint16_t& _input_field_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B040});
    inline int16_t& _active_input_field_number = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B04C});
    inline int16_t& _cursor_position = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055b038});
    inline int16_t& _cursor_on = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055b03c});
    inline uint16_t& _help_list_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B05C});
    inline uint16_t& _help_list_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B060});
    inline int16_t& _multi_hotkey_active_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B064});
    inline uint16_t& _draw_fields_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B078});
    inline const char* s_You_exceeded_the_max_number_of_fields = reinterpret_cast<const char*>(std::uintptr_t{0x0055B080});
    inline const char* s_Maximum_number_of_button_fields_e_0055b0b0 = reinterpret_cast<const char*>(std::uintptr_t{0x0055B0B0});
    inline int16_t& _max_fields_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFC4});
    inline s_field*& _fields = *reinterpret_cast<s_field**>(std::uintptr_t{0x0055AFD0});
    inline s_field*& _old_fields = *reinterpret_cast<s_field**>(std::uintptr_t{0x0055AFD4});
    inline int16_t& _pointer_offset = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572BF8});
    inline uint16_t& _old_input_field_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572BE4});
    inline int16_t& _old_active_input_field_number = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572BF4});
    inline int16_t& _old_down_mouse_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005762E0});
    inline int16_t& _old_fields_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00576304});
    inline uint16_t& _old_input_delay = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00576308});
    inline int16_t& _old_auto_input_variable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00576320});
    inline int16_t& _fields_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572D18});
    inline s_colors (&_list_normal_color) = *reinterpret_cast<s_colors(*)>(std::uintptr_t{0x00572C08});
    inline s_colors (&_list_highlight_colors) = *reinterpret_cast<s_colors(*)>(std::uintptr_t{0x005762F0});
    inline s_colors (&_list_special_colors) = *reinterpret_cast<s_colors(*)>(std::uintptr_t{0x00572BD0});
    inline uint16_t& _input_delay = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572C00});
    inline void*& _function = *reinterpret_cast<void**>(std::uintptr_t{0x005762F8});
    inline void*& _help_struct_pointer = *reinterpret_cast<void**>(std::uintptr_t{0x00576300});
    inline uint8_t& _continuous_string = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00572c18});
    inline uint8_t& _active_input_field_first = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005762d8});
    inline uint16_t& _field_outline_color = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B020});
    inline uint16_t& _mouse_cancel_disabled = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B018});
    inline uint16_t& _dynamic_field_refresh = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B074});
    inline uint16_t& _field_box_mode = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B044});

    void __cdecl Assign_Update_Function_(void (*function_ptr)());
    void __cdecl Deactivate_Auto_Function_();
    inline void __cdecl Activate_Auto_Function_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051FF10});
        fn();
    }
    void __cdecl Clear_Fields_();

    int16_t __cdecl Add_Hot_Key_(const char* hotkey_str);

    void __cdecl Assign_Auto_Function_(void (*function_ptr)(), int32_t delay);

    int16_t __cdecl Get_Input_();

    inline void __cdecl Enable_Mouse_Wait_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051D1A0});
        fn();
    }

    inline int16_t __cdecl Interpret_Mouse_Input_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00521a60});
        return fn();
    }

    inline int16_t __cdecl Scan_Input_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051d290});
        return fn();
    }

    int16_t __cdecl Add_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* text, int16_t key);

    int16_t __cdecl Add_Clipped_Button_Field_(int16_t x, int16_t y, char* help, s_animation_header* pic, void* highlight_color, char hotkey);

    int16_t __cdecl Add_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int16_t sound);

    inline int16_t __cdecl Add_Clipped_Hidden_Field_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const char* label, int16_t unknown) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t, const char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051d4d0});
        return fn(x1, y1, x2, y2, label, unknown);
    }

    int16_t __cdecl Add_Irregular_Button_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* help, s_animation_header* pic, int16_t help_start, int16_t help_end, const char* hotkey_ptr, int16_t sound);

    int16_t __cdecl Add_Multi_Hot_Key_Field_(const char* help_text);

    int16_t __cdecl Auto_Input_();

    int16_t __cdecl Add_Grid_Field_(int16_t x, int16_t y, int16_t step_x, int16_t step_y, int16_t cols, int16_t rows, short* grid_x, short* grid_y, char sound);

    int16_t __cdecl Add_Scroll_Field_(int16_t x, int16_t y, int16_t min_val, int16_t max_val, int16_t pos, int16_t step, int16_t width, int16_t height, short* context_ptr, const char* hotkey_ptr, char sound);

    int16_t __cdecl Add_Continuous_String_Input_Field_(int16_t x, int16_t y, int16_t width, int16_t height, char* help, int16_t max_len, int16_t unk1, int32_t unk2, int16_t sound, int16_t unk3);

    int16_t __cdecl Add_Multi_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* value, int16_t count, const char* hotkey, int16_t sound);

    inline int16_t __cdecl Add_Clipped_Circle_Field_(int16_t x, int16_t y, int16_t radius, char* str, char flags) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, char*, char);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00523b00});
        return fn(x, y, radius, str, flags);
    }

    inline int16_t __cdecl Add_Radio_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* variable, const char* hotkey, int16_t sound) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, const char*, s_animation_header*, int16_t*, const char*, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0051DC20})(x, y, help, pic, variable, hotkey, sound);
    }

    inline void __cdecl Disable_Draw_Field_(int16_t field_id) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051eea0});
        fn(field_id);
    }

    inline void __cdecl Enable_Draw_Field_(int16_t field_id) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051eef0});
        fn(field_id);
    }

    void __cdecl Disable_Draw_All_Fields_();
    void __cdecl Enable_Draw_All_Fields_();

    void __cdecl Disable_Cancel_();

    void __cdecl Clear_Fields_Above_(int16_t f);

    inline int16_t __cdecl Get_Field_Counter_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051d170});
        return fn();
    }

    inline void __cdecl Init_Mouse_Keyboard_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00538760});
        fn();
    }

    void __cdecl Enable_Mouse_Pressed_Field_Update_();

    void __cdecl Deactivate_Help_List_();

    void __cdecl Set_Help_List_(void* help_data, int16_t count);

    void __cdecl Add_Button_Info_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* hotkey, int16_t sound);

    void __cdecl Invoke_Auto_Function_();

    inline void __cdecl Evaluate_Input_(int16_t* input, int16_t* scan) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0050B520})(input, scan);
    }

    int16_t __cdecl Scan_Field_();

    void __cdecl Clear_Mouse_Buffer_();

    inline void __cdecl Timed_Wait_For_Input_(int32_t ticks) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00523e10});
        fn(ticks);
    }

    inline void __cdecl Draw_Visible_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00522C80});
        fn();
    }

    inline void __cdecl Draw_All_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0051EF40})();
    }
    inline void __cdecl Set_Input_Delay_(int16_t delay) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051EDD0});
        fn(delay);
    }
    inline void __cdecl Set_Field_Outline_Color_(uint8_t color) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00520000});
        fn(color);
    }

    inline void __cdecl Draw_Visible_Fields_Back_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051ef60});
        fn();
    }

    int16_t __cdecl Add_Locked_Button_Field_(int16_t x, int16_t y, const char* help, s_animation_header* pic, int16_t* selection, const char* hotkey, int16_t sound);

    int16_t __cdecl Add_String_List_Field_(int16_t x, int16_t y, int16_t width, char* help, int16_t param_5, char** strings, int16_t help_start, int16_t param_8, int16_t* value, int32_t param_10, s_animation_header* pic, int32_t param_12, char* hotkey_str);

    inline void __cdecl Mark_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00520120});
        fn();
    }

    inline void __cdecl Clipped_Dot_(int16_t x, int16_t y, uint8_t color) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005210c0});
        fn(x, y, color);
    }

    inline void __cdecl Exit_List_Field_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00523df0});
        fn();
    }

    void __cdecl Release_Fields_();

    inline int16_t __cdecl Get_List_Field_(int16_t x, int16_t y, int16_t width, char* title, void* choices, int16_t max_choice_len, void* p_selection, uint8_t* p_enabled, int16_t num_items, int16_t* p10, int16_t p11, s_animation_header* anim, int16_t p13, int16_t p14) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, char*, void*, int16_t, void*, uint8_t*, int16_t, int16_t*, int16_t, s_animation_header*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00521210});
        return fn(x, y, width, title, choices, max_choice_len, p_selection, p_enabled, num_items, p10, p11, anim, p13, p14);
    }
}