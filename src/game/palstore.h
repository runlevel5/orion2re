#pragma once

#include <cstdint>

struct IDirectDrawPalette;

namespace palstore {
    inline IDirectDrawPalette*& g_palette =
        *reinterpret_cast<IDirectDrawPalette**>(std::uintptr_t{0x00578E64});

    int16_t Slow_Fade_In_();

    void __cdecl Darken_Palette_(int32_t darken_percent);

    inline void __cdecl Store_Palette_Block_(int start_index, int count) {
        using Fn = void(__cdecl*)(int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b7e0});
        fn(start_index, count);
    }

    inline void __cdecl Store_Single_Palette_Color_(int32_t pal_idx, uint8_t r, uint8_t g, uint8_t b) {
        using Fn = void(__cdecl*)(int32_t, uint8_t, uint8_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b770});
        fn(pal_idx, r, g, b);
    }
}