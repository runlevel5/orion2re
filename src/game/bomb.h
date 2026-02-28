#pragma once
#include <cstdint>

struct s_colony;

namespace BOMB {
    inline void __cdecl Destroy_Colony_(s_colony* colony, int16_t attacker_player_idx) {
        using Fn = void(__cdecl*)(s_colony*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047e780});
        fn(colony, attacker_player_idx);
    }
}