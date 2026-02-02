#pragma once

namespace fields {
    inline void*& _update_function = *reinterpret_cast<void**>(std::uintptr_t{ 0x0055AFD8 });
    inline uint16_t& _auto_active_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFDC });
    inline uint16_t& _down_mouse_button = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055AFE4});
    inline uint16_t& _auto_input_variable = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B004});
    inline uint16_t& _input_field_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B040});
    inline uint16_t& _active_input_field_number = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B04C});
    inline uint16_t& _multi_hotkey_active_field = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B064});
    inline uint16_t& _input_delay = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572C00});
    inline uint16_t& _fields_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572D18});

    void __cdecl Assign_Update_Function_(void* update_func);
    void __cdecl Deactivate_Auto_Function_();
    void __cdecl Init_Fields_State_();

    inline int16_t __cdecl Add_Hot_Key_(const char* hotkey_str) {
        using Fn = int16_t(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051d6c0});
        return fn(hotkey_str);
    }

    inline void __cdecl Assign_Auto_Function_(void (*func)(), int16_t priority) {
        using Fn = void(__cdecl*)(void (*)(), int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051fe90});
        fn(func, priority);
    }

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

    inline void __cdecl Disable_Draw_All_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051ee80});
        fn();
    }

    inline void __cdecl Enable_Draw_All_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051ee90});
        fn();
    }
}