#pragma once
#include <cstdint>

namespace graphics {
    inline int16_t& _old_screen_window_y2 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0057A998 });
    inline int16_t& _old_screen_window_y1 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0057A99C });
    inline int16_t& _old_screen_window_x2 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0057A9A0 });
    inline int16_t& _old_screen_window_x1 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0057A9A4 });

    void Reset_Window_();
    void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
    void __cdecl Darken_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Set_Window_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Save_Window_();
    void __cdecl Restore_Window_();

    void __cdecl Tint_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t tint_color);

    void __cdecl Interlaced_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color1, int16_t color2);

    void __cdecl Tri_Interlaced_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color1, int16_t color2, int16_t color3);

    void __cdecl Box_(int16_t x, int16_t y, int16_t width, int16_t height, int16_t thickness, uint8_t color);

    void __cdecl Dot_(int16_t x, int16_t y, uint8_t color);
}