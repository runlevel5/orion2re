#pragma once

namespace fonts {
    inline void Load_Palette_(int32_t id, int32_t arg2, int32_t arg3) {
        using Fn = void(*)(int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00525400 });
        fn(id, arg2, arg3);
    }
    inline void Set_PSTR_Style_(int32_t style) {
        using Fn = void(*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00526D00 });
        fn(style);
    }
    inline void Set_Font_Style_(int32_t font, intptr_t color_ptr) {
        using Fn = void(*)(int32_t, intptr_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005255A0 });
        fn(font, color_ptr);
    }
    inline void Print_(int32_t x, int32_t y, const char* str) {
        using Fn = void(*)(int32_t, int32_t, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00525910 });
        fn(x, y, str);
    }
}
