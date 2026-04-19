#pragma once

namespace aibomb {
    inline bool __cdecl AI_Invasion_Will_Succeed_(int8_t attacker_id, int16_t* ship_list, int n_ships, s_colony* colony) {
        using Fn = bool(__cdecl*)(int8_t, int16_t*, int, s_colony*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005009a0});
        return fn(attacker_id, ship_list, n_ships, colony);
    }
}