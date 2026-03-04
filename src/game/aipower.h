#pragma once

#include <cstdint>

namespace aipower {
    int16_t __cdecl Get_Best_Ship_Shield_Value_(int16_t player_idx);

    int16_t __cdecl Ship_Strength_Vs_Shield_(int16_t ship_idx, int16_t target_idx, int16_t shield_strength);

    int16_t __cdecl Ship_Strength_Vs_Ships_(int16_t attacker_idx, int16_t target_idx);

    int16_t __cdecl Ship_Strength_Vs_Ships_In_Nebula_(int16_t attacker_idx, int16_t target_idx);

    void __cdecl Ship_Strength_Vs_Planetary_Shield_(int16_t ship_idx, int16_t* results);

    int16_t __cdecl Max_Ship_Hits_(int16_t ship_idx);

    int16_t __cdecl Current_Ship_Hits_(int16_t ship_idx);

    int16_t __cdecl Base_Generic_Ocv_(int16_t player_idx);
}