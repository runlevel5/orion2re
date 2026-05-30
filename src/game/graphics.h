#pragma once
#include <cstdint>

namespace graphics {
    extern int16_t _old_screen_window_y2;
    extern int16_t _old_screen_window_y1;
    extern int16_t _old_screen_window_x2;
    extern int16_t _old_screen_window_x1;

    void Reset_Window_();
    void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
    void __cdecl Darken_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Set_Window_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Save_Window_();
    void __cdecl Restore_Window_();

    void __cdecl Tint_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t tint_color);

    void __cdecl Gray_Scale_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t* color_ptr, int32_t extra, int32_t param_7);

    void __cdecl Gradient_Fill_(int x1, int y1, int x2, int y2, int mode, uint8_t* palette, int extra, s_animation_header* pic, s_colors* colors);

    void __cdecl Interlaced_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color1, int16_t color2);

    void __cdecl Tri_Interlaced_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color1, int16_t color2, int16_t color3);

    void __cdecl Box_(int16_t x, int16_t y, int16_t width, int16_t height, int16_t thickness, uint8_t color);

    void __cdecl Dot_(int16_t x, int16_t y, uint8_t color);
}