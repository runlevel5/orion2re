#pragma once
#include <cstdint>

namespace COMBINIT {
    inline void __cdecl Load_Display_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040f660});
        fn(ship_idx);
    }
}