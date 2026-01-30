#pragma once
#include <cstdint>

namespace line {
    inline void __cdecl Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00529950});
        fn(x1, y1, x2, y2, color);
    }

    inline void __cdecl Line_ASM_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color_data) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, uint32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00466420})(x1, y1, x2, y2, color_data);
    }
}