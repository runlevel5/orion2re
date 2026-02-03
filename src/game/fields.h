#pragma once

namespace fields {
    inline void*& _update_function = *reinterpret_cast<void**>(std::uintptr_t{ 0x0055AFD8 });
    inline uint16_t& _auto_active_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFDC });
    inline uint16_t& _auto_function_delay = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFF4 });
    inline int16_t& _down_mouse_button = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055AFE4});
    inline uint16_t& _auto_input_variable = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B004});
    inline uint16_t& _input_field_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B040});
    inline int16_t& _active_input_field_number = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B04C});
    inline uint16_t& _help_list_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B05C});
    inline uint16_t& _help_list_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B060});
    inline uint16_t& _multi_hotkey_active_field = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B064});
    inline uint16_t& _draw_fields_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B078});
    inline uint16_t& _input_delay = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572C00});
    inline uint16_t& _fields_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572D18});
    inline void*& _function = *reinterpret_cast<void**>(std::uintptr_t{0x005762F8});
    inline void*& _help_struct_pointer = *reinterpret_cast<void**>(std::uintptr_t{0x00576300});

    void __cdecl Assign_Update_Function_(void* update_func);
    void __cdecl Deactivate_Auto_Function_();
    void __cdecl Clear_Fields_();

    inline int16_t __cdecl Add_Hot_Key_(const char* hotkey_str) {
        using Fn = int16_t(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051d6c0});
        return fn(hotkey_str);
    }

    void __cdecl Assign_Auto_Function_(void* function_ptr, int16_t delay);

    inline int32_t __cdecl Get_Input_() {
        using Fn = int32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051edf0});
        return fn();
    }

    inline int16_t __cdecl Scan_Input_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051d290});
        return fn();
    }

    inline int16_t __cdecl Add_Button_Field_(int16_t x, int16_t y, const char* help, void* seg, const char* label, int16_t key) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, const char*, void*, const char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051db80});
        return fn(x, y, help, seg, label, key);
    }

    inline int16_t __cdecl Add_Hidden_Field_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const char* help, int16_t key) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t, const char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051d370});
        return fn(x1, y1, x2, y2, help, key);
    }

    inline int16_t __cdecl Add_Irregular_Button_Field_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const char* help, void* seg, int16_t u1, int16_t u2, const char* label, int16_t key) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t, const char*, void*, int16_t, int16_t, const char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00520220});
        return fn(x1, y1, x2, y2, help, seg, u1, u2, label, key);
    }

    inline int16_t __cdecl Add_Multi_Hot_Key_Field_(const char* keys) {
        using Fn = int16_t(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00520690});
        return fn(keys);
    }

    inline int16_t __cdecl Add_Grid_Field_(int16_t x, int16_t y, int16_t rows, int16_t cols, int16_t w, int16_t h, int16_t* px, int16_t* py, int16_t key) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t*, int16_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051d7f0});
        return fn(x, y, rows, cols, w, h, px, py, key);
    }

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

    void __cdecl Invoke_Auto_Function_();
}