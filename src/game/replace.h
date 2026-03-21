#pragma once
#include <cstdint>

namespace replace {
    inline void __cdecl Replace_Color_All_(void* bitmap, int32_t color) {
        using Fn = void(__cdecl*)(void*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00530710});
        fn(bitmap, color);
    }

    inline void __cdecl Replace_Color_Range_(void* bitmap, uint8_t* color_table, int32_t start_color, int32_t count) {
        using Fn = void(__cdecl*)(void*, uint8_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00530890});
        fn(bitmap, color_table, start_color, count);
    }

    inline void __cdecl Shift_Palette_Range_(void* bitmap, int32_t range) {
        using Fn = void(__cdecl*)(void*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00530af0});
        fn(bitmap, range);
    }

    inline void __cdecl Replace_Color_(void* bitmap, int32_t target_color, int32_t replacement_color) {
        using Fn = void(__cdecl*)(void*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00530570});
        fn(bitmap, target_color, replacement_color);
    }

    inline void __cdecl Interlaced_Gray_Scale_Off_Page_(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t unknown) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00530b40});
        fn(x1, y1, x2, y2, unknown);
    }
}