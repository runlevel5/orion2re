#pragma once
#include <cstdint>

namespace CMBTMIS {
    inline void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t speed, int16_t* dx, int16_t* dy) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004FD9C0});
        fn(x1, y1, x2, y2, speed, dx, dy);
    }
}