#pragma once
#include <cstdint>

namespace special {
    // TYPE MISMATCH: original stub had int32_t return type
    int16_t __cdecl Range_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Interpolate_Line_(int16_t* x_ptr, int16_t* y_ptr, int16_t target_x, int16_t target_y, int16_t step);

    void __cdecl Absolute_Interpolate_Line_(int16_t* x_ptr, int16_t* y_ptr, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t step);
}