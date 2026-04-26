#pragma once
#include <cstdint>

namespace angle {
    int16_t __cdecl Get_Angle_(int32_t dx, int32_t dy);

    inline int8_t (&_atan_lookup_table)[128] =
        *reinterpret_cast<int8_t(*)[128]>(std::uintptr_t{0x0054F4C8});

    inline uint16_t (&_sin_table)[90] =
        *reinterpret_cast<uint16_t(*)[90]>(std::uintptr_t{0x0054F548});

    int32_t __cdecl Cos_(int16_t angle, int32_t magnitude);
    int32_t __cdecl Sin_(int16_t angle, int32_t magnitude);

    void __cdecl Rotate_Coordinates_(int32_t angle, int32_t* x, int32_t* y);
}