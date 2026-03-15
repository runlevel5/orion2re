#pragma once
#include <cstdint>

struct s_colony;

namespace BOMB {
    int16_t __cdecl Handle_Leader_At_Lost_Colony_(s_colony* colony);

    void __cdecl Destroy_Colony_(s_colony* colony, int32_t capturing_player);

    inline int __cdecl Resolve_Bomb_Hit_(s_colony* colony, s_combat_bomb_result* result, int16_t bomb_hits) {
        using Fn = int(__cdecl*)(s_colony*, s_combat_bomb_result*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047e8e0});
        return fn(colony, result, bomb_hits);
    }

    inline void __cdecl Resolve_Biological_Hit_(s_colony* colony, s_combat_bomb_result* result) {
        using Fn = void(__cdecl*)(s_colony*, s_combat_bomb_result*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047ebd0});
        fn(colony, result);
    }

    void __cdecl Apply_Bomb_Hits_(s_combat_bomb_result* result);
}