#pragma once

namespace fields {
    inline void*& _update_function = *reinterpret_cast<void**>(std::uintptr_t{ 0x0055AFD8 });
    inline uint16_t& _auto_active_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFDC });
    inline uint16_t& _auto_function_delay = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFF4 });
    inline int16_t& _down_mouse_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFE4});
    inline int16_t& _auto_input_variable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B004});
    inline uint16_t& _input_field_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B040});
    inline int16_t& _active_input_field_number = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B04C});
    inline uint16_t& _help_list_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B05C});
    inline uint16_t& _help_list_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B060});
    inline int16_t& _multi_hotkey_active_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B064});
    inline uint16_t& _draw_fields_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B078});
    inline const char* s_You_exceeded_the_max_number_of_fields = reinterpret_cast<const char*>(std::uintptr_t{0x0055B080});
    inline const char* s_Maximum_number_of_button_fields_e_0055b0b0 = reinterpret_cast<const char*>(std::uintptr_t{0x0055B0B0});

    enum FIELD_TYPE : int16_t {
        FIELD_TYPE_BUTTON = 0,
        FIELD_TYPE_1 = 1,
        FIELD_TYPE_2 = 2,
        FIELD_TYPE_3 = 3,
        FIELD_TYPE_4 = 4,
        FIELD_TYPE_5 = 5,
        FIELD_TYPE_SCROLL = 6,
        FIELD_TYPE_HIDDEN = 7,
        FIELD_TYPE_MULTI_HOTKEY = 8,
        FIELD_TYPE_9 = 9,
        FIELD_TYPE_10 = 10,
        FIELD_TYPE_CONTINUOUS_INPUT = 11,
        FIELD_TYPE_GRID = 12,
        FIELD_TYPE_IRREGULAR_INPUT = 13
    };

#pragma pack(push, 1)
    struct s_field {
        int16_t x;
        int16_t y;
        int16_t x_end;
        int16_t y_end;
        FIELD_TYPE field_type;
        int16_t font_style;
        uint8_t* font_color;
        uint8_t field7_0x10;
        uint8_t field8_0x11;
        int16_t field9_0x12;
        uint8_t* highlight_color;
        const char* help;
        int16_t step_x;
        uint8_t field13_0x1e;
        uint8_t field14_0x1f;
        int16_t step_y;
        uint8_t field16_0x22;
        uint8_t field17_0x23;
        int16_t full_store_flag;
        uint8_t field19_0x26;
        uint8_t field20_0x27;
        int32_t field21_0x28;
        s_animation_header* pic;
        int16_t field23_0x30;
        uint8_t field24_0x32;
        uint8_t field25_0x33;
        char hotkey;
        int16_t sound;
    };
#pragma pack(pop)

    inline int16_t& _max_fields_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFC4});
    inline s_field*& _fields = *reinterpret_cast<s_field**>(std::uintptr_t{0x0055AFD0});
    inline int16_t& _pointer_offset = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572BF8});
    inline int16_t& _fields_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572D18});
    inline int16_t& _input_delay = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572C00});
    inline void*& _function = *reinterpret_cast<void**>(std::uintptr_t{0x005762F8});
    inline void*& _help_struct_pointer = *reinterpret_cast<void**>(std::uintptr_t{0x00576300});

    void __cdecl Assign_Update_Function_(void* update_func);
    void __cdecl Deactivate_Auto_Function_();
    void __cdecl Clear_Fields_();

    int16_t __cdecl Add_Hot_Key_(const char* hotkey_str);

    void __cdecl Assign_Auto_Function_(void* function_ptr, int16_t delay);

    int16_t __cdecl Get_Input_();

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

    int16_t __cdecl Add_Hidden_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* hotkey, int16_t sound);

    int16_t __cdecl Add_Irregular_Button_Field_(int16_t x, int16_t y, int16_t x_end, int16_t y_end, const char* help, s_animation_header* pic, int16_t param_7, int16_t param_8, const char* hotkey_ptr, int16_t sound);

    int16_t __cdecl Add_Multi_Hot_Key_Field_(const char* help_text);

    int16_t __cdecl Auto_Input_();

    int16_t __cdecl Add_Grid_Field_(int16_t x, int16_t y, int16_t step_x, int16_t step_y, int16_t cols, int16_t rows, int16_t* param_7, int16_t* param_8, int16_t sound);

    inline int16_t __cdecl Add_Scroll_Field_(int16_t x, int16_t y, int v1, int v2, int v3, int v4, int v5, int v6, int16_t* out, const char* label, char type) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int, int, int, int, int, int, int16_t*, const char*, char);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051de80});
        return fn(x, y, v1, v2, v3, v4, v5, v6, out, label, type);
    }

    inline int32_t __cdecl Add_Continuous_String_Input_Field_(int16_t x, int16_t y, int w, int h, MOX::s_save_game_description* desc, int max_len, uint16_t v1, uint32_t v2, char hotkey, uint16_t v3) {
        using Fn = int32_t(__cdecl*)(int16_t, int16_t, int, int, MOX::s_save_game_description*, int, uint16_t, uint32_t, char, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051e2b0});
        return fn(x, y, w, h, desc, max_len, v1, v2, hotkey, v3);
    }

    void __cdecl Disable_Draw_All_Fields_();
    void __cdecl Enable_Draw_All_Fields_();

    void __cdecl Deactivate_Help_List_();

    void __cdecl Set_Help_List_(void* help_data, int16_t count);

    void __cdecl Add_Button_Info_(int16_t x, int16_t y, const char* help, s_animation_header* pic, const char* hotkey, int16_t sound);

    void __cdecl Invoke_Auto_Function_();

    int16_t __cdecl Scan_Field_();
}