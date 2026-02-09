#pragma once
#include <cstdint>

namespace special {
    inline int32_t __cdecl Range_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        using Fn = int32_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00531a50});
        return fn(x1, y1, x2, y2);
    }

    inline void __cdecl Interpolate_Line_(int16_t* px, int16_t* py, int16_t tx, int16_t ty, int16_t step) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00531800});
        fn(px, py, tx, ty, step);
    }
}