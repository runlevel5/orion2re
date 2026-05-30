#pragma once

struct s_player;

namespace maintain {
    bool __cdecl Ship_Is_At_Colony_(s_ship_data* ship);
    void __cdecl Make_Scrap_Ships_Dead_();
    void __cdecl Player_Maintenance_(s_player* param_1);

    extern int32_t (*_ship_worths)[500];

    int16_t __cdecl Ship_Scrap_Value_(s_ship_data* ship);

    bool __cdecl Phase1_Building_(int16_t b);

    bool __cdecl Phase2_Building_(int16_t b);

    bool __cdecl All_But_Capitol_(int16_t b);

    extern uint8_t _research_building_table[4];

    void __cdecl Kill_Research_Buildings_(s_player* player);

    int32_t __cdecl Building_Worth_(s_colony* colony, int16_t building_idx);

    void __cdecl Kill_Buildings_(s_player* player, bool (__cdecl* can_scrap)(int16_t));

    void __cdecl Kill_Ships_(s_player* player, bool at_colony_only, bool restricted_mission, int16_t ship_type);

    void __cdecl Kill_Spy_(s_player* player);

    void __cdecl Kill_Leaders_(s_player* p);
}