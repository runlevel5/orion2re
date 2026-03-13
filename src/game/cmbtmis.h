#pragma once
#include <cstdint>

namespace CMBTMIS {
    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t step, int16_t* out_x, int16_t* out_y);

    inline int16_t __cdecl Missile_Owner_(int16_t m) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004fd8d0})(m);
    }

    inline void __cdecl Retarget_Missile_(int16_t m) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00500070})(m);
    }
}