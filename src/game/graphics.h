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

    inline void __cdecl Tint_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00529660});
        fn(x1, y1, x2, y2, color);
    }

    inline void __cdecl Interlaced_Fill_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4, int16_t param_5, int16_t param_6) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(0x00528f30)(param_1, param_2, param_3, param_4, param_5, param_6);
    }

    void __cdecl Box_(int16_t x, int16_t y, int16_t width, int16_t height, int16_t thickness, uint8_t color);

    void __cdecl Dot_(int16_t x, int16_t y, uint8_t color);
}