#pragma once

namespace BOMB {
    int16_t __cdecl Handle_Leader_At_Lost_Colony_(s_colony* colony);

    void __cdecl Destroy_Colony_(s_colony* colony, int32_t capturing_player);

    int __cdecl Resolve_Bomb_Hit_(s_colony* colony, s_combat_bomb_result* result, int bomb_power);

    void __cdecl Resolve_Biological_Hit_(s_colony* colony, s_combat_bomb_result* result);

    void __cdecl Apply_Bomb_Hits_(s_combat_bomb_result* result);
}