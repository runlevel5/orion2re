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

    inline void __cdecl Draw_Directional_Multi_Colored_Line_(int16_t y1, int16_t x1, int16_t y2, int16_t x2, uint8_t* colors, int16_t start_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, uint8_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041F560});
        fn(y1, x1, y2, x2, colors, start_idx);
    }

    inline void __cdecl Plasma_Box_(int16_t x, int16_t y, int16_t w, int16_t h, int16_t thickness, int16_t color_anim, int16_t unk_color, uint8_t* colors) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052a900});
        fn(x, y, w, h, thickness, color_anim, unk_color, colors);
    }
}