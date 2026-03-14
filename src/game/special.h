#pragma once
#include <cstdint>

namespace special {
    // TYPE MISMATCH: original stub had int32_t return type
    int16_t __cdecl Range_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Interpolate_Line_(int16_t* x_ptr, int16_t* y_ptr, int16_t target_x, int16_t target_y, int16_t step);

    inline void __cdecl Absolute_Interpolate_Line_(int16_t* px, int16_t* py, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t step) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t, int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(0x00531940)(px, py, x1, y1, x2, y2, step);
    }
}