#pragma once
#include <cstdint>

namespace angle {
    int16_t __cdecl Get_Angle_(int32_t dx, int32_t dy);

    extern uint8_t _atan_lookup_table[128];

    extern uint16_t _sin_table[90];

    int32_t __cdecl Cos_(int16_t angle, int32_t magnitude);
    int32_t __cdecl Sin_(int16_t angle, int32_t magnitude);

    void __cdecl Rotate_Coordinates_(int16_t angle, int32_t* x, int32_t* y);
}