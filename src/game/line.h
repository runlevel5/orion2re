#pragma once
#include <cstdint>

struct s_colors;

namespace line {
    inline int32_t& g_screen_res_x =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055B1A8});

    void __cdecl Multi_Colored_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors, uint16_t offset);

    int16_t __cdecl Clip_Line_(int16_t* x1, int16_t* y1, int16_t* x2, int16_t* y2);

    void __cdecl Draw_Horizontal_Run_Multi_(uint8_t** dest_ptr, int16_t step, int16_t length, s_colors* colors, uint16_t* color_index);

    void __cdecl Draw_Vertical_Run_Multi_(uint8_t** screen_ptr, int16_t offset, int16_t height, s_colors* colors, uint16_t* color_idx);

    void __cdecl Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    void __cdecl Draw_Vertical_Run_(uint8_t** buf, int16_t param_2, int16_t count, uint8_t color);
    void __cdecl Draw_Horizontal_Run_(uint8_t** buf, int16_t step_x, int16_t count, uint8_t color);

    void __cdecl Plasma_Box_(int16_t x, int16_t y, uint16_t w, int16_t h, int16_t thickness, int16_t val1, uint8_t val2, int32_t val3);

    void __cdecl Create_Plasma_Line_(int16_t seed, uint16_t start_val, const uint8_t* color_map);

    void __cdecl Plasma_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t* color_map, uint16_t color_index);

    inline void __cdecl Draw_Horizontal_Run_Plasma_(uint8_t** ptr, int16_t x_dir, int16_t run_len, uint8_t* color_map, uint16_t* color_index) {
        using Fn = void(__cdecl*)(uint8_t**, int16_t, int16_t, uint8_t*, uint16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052B180});
        fn(ptr, x_dir, run_len, color_map, color_index);
    }

    inline void __cdecl Draw_Vertical_Run_Plasma_(uint8_t** ptr, int16_t x_dir, int16_t run_len, uint8_t* color_map, uint16_t* color_index) {
        using Fn = void(__cdecl*)(uint8_t**, int16_t, int16_t, uint8_t*, uint16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052B1D0});
        fn(ptr, x_dir, run_len, color_map, color_index);
    }
}