#pragma once
#include <cstdint>

struct s_colors;

namespace line {
    inline void __cdecl Multi_Colored_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors, int16_t offset) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, s_colors*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052a230});
        fn(x1, y1, x2, y2, colors, offset);
    }

    int16_t __cdecl Clip_Line_(int16_t* x1, int16_t* y1, int16_t* x2, int16_t* y2);

    void __cdecl Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    void __cdecl Draw_Vertical_Run_(uint8_t** buf, int16_t param_2, int16_t count, uint8_t color);
    void __cdecl Draw_Horizontal_Run_(uint8_t** buf, int16_t step_x, int16_t count, uint8_t color);
}