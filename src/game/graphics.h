#pragma once
#include <cstdint>

namespace graphics {
    void Reset_Window_();
    void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    void __cdecl Set_Window_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    inline void __cdecl Save_Window_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528C10 });
        fn();
    }

    inline void __cdecl Restore_Window_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00528C50 });
        fn();
    }
}