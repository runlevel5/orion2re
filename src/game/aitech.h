#pragma once

namespace aitech {
    void __cdecl Apply_Evolutionary_Upgrades_();
    void __cdecl Compute_AI_Tech_Info_(s_ai_tech_info* info);
    void __cdecl All_AI_Tech_Select_();
    void __cdecl AI_Tech_Select_(s_player* player);

    int __cdecl Player_Best_Ship_Shield_(s_player* player);

    int __cdecl Player_Best_Armor_(s_player* player);

    int __cdecl Player_Best_Rifle_(s_player* player);

    int __cdecl Player_Best_Personal_Shield_(s_player* player);

    int __cdecl Player_Best_Computer_(s_player* player);

    int __cdecl Player_Best_Fuel_(s_player* player);

    int __cdecl Player_Best_Planetary_Shield_(s_player* player);

    int __cdecl Player_Best_Drive_(s_player* player);

    int16_t __cdecl System_Proximity_Count_(int16_t player_id, uint8_t other_player_id);
}