#pragma once
#include <cstdint>

namespace angle {
    inline int16_t __cdecl Get_Angle_(int32_t dx, int32_t dy) {
        using Fn = int16_t(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00531af0});
        return fn(dx, dy);
    }

    inline int16_t __cdecl Cos_(int16_t angle, int16_t step) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00531ca0});
        return fn(angle, step);
    }

    inline int16_t __cdecl Sin_(int16_t angle, int16_t step) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00531bc0});
        return fn(angle, step);
    }
}