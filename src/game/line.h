#pragma once
#include <cstdint>

namespace line {
    inline void __stdcall Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t color) {
        using Fn = void(__stdcall*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00529950});
        fn(x1, y1, x2, y2, color);
    }
}