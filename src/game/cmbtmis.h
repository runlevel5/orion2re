#pragma once
#include <cstdint>

namespace CMBTMIS {
    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t step, int16_t* out_x, int16_t* out_y);
}