#pragma once

#include <cstdint>

namespace AIPOWER {
    int16_t __cdecl Get_Best_Ship_Shield_Value_(int16_t player_idx);

    int16_t __cdecl Ship_Strength_Vs_Shield_(int16_t ship_idx, int16_t target_idx, int16_t shield_strength);

    int16_t __cdecl Ship_Strength_Vs_Ships_(int16_t attacker_idx, int16_t target_idx);

    int16_t __cdecl Ship_Strength_Vs_Ships_In_Nebula_(int16_t attacker_idx, int16_t target_idx);

    void __cdecl Ship_Strength_Vs_Planetary_Shield_(int16_t ship_idx, int16_t* results);

    int16_t __cdecl Max_Ship_Hits_(int16_t ship_idx);

    int16_t __cdecl Current_Ship_Hits_(int16_t ship_idx);

    int16_t __cdecl Base_Generic_Ocv_(int16_t player_idx);

    inline uint32_t __cdecl Colony_Space_Strength_Vs_Player_(int16_t colony_idx, int16_t player_idx) {
        using Fn = uint32_t(__cdecl*)(int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00509390})(colony_idx, player_idx);
    }

    inline uint32_t __cdecl Colony_Ground_Strength_Vs_Player_(int16_t colony_idx, int16_t player_idx) {
        using Fn = uint32_t(__cdecl*)(int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x005092d0})(colony_idx, player_idx);
    }
}