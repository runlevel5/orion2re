#pragma once
#include <cstdint>

namespace graphics {
    void Reset_Window_();
    void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    inline void __cdecl Set_Window_(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00528BA0});
        fn(x1, y1, x2, y2);
    }
}