#pragma once
#include <cstdint>

namespace graphics {
    void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    inline void __stdcall Set_Window_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        using Fn = void(__stdcall*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00528BA0});
        fn(x1, y1, x2, y2);
    }
}