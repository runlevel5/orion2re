#pragma once

#include <cstdint>

namespace aipower {
    int16_t __cdecl Get_Best_Ship_Shield_Value_(int16_t player_idx);

    inline int16_t __cdecl Ship_Strength_Vs_Shield_(int16_t attacker_idx, int16_t target_idx, int16_t shield_val) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00508ad0});
        return fn(attacker_idx, target_idx, shield_val);
    }

    int16_t __cdecl Ship_Strength_Vs_Ships_(int16_t attacker_idx, int16_t target_idx);
}