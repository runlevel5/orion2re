#pragma once
#include <cstdint>

namespace graphics {
    inline int16_t& _old_screen_window_y2 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0057A998 });
    inline int16_t& _old_screen_window_y1 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0057A99C });
    inline int16_t& _old_screen_window_x2 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0057A9A0 });
    inline int16_t& _old_screen_window_x1 = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0057A9A4 });

    void Reset_Window_();
    void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    void __cdecl Set_Window_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Save_Window_();
    void __cdecl Restore_Window_();
}