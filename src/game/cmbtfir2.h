#pragma once

#include <cstdint>

namespace CMBTFIR2 {
    inline int16_t __cdecl Helmsman_Bonus_(int16_t officer_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e64b0});
        return fn(officer_idx);
    }

    inline int16_t __cdecl Weaponry_Bonus_(int16_t officer_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e6510});
        return fn(officer_idx);
    }
}