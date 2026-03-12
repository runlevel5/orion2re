#pragma once
#include <cstdint>

namespace COMBINIT {
    inline void __cdecl Load_Display_Combat_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0040f660});
        fn(ship_idx);
    }

    inline int16_t __cdecl Tactical_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, int32_t retreat_flag) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t, int16_t*, int16_t, int16_t, int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00409540});
        return fn(plr1_ships, plr1_ship_count, plr2_ships, plr2_ship_count, star_idx, colony_idx, retreat_flag);
    }
}