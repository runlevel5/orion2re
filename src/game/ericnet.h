#pragma once
#include <cstdint>

namespace ERICNET {
    extern int8_t _start_civ_level[3];
    extern int16_t _buildlist[43];

    void __cdecl Add_Building_(int16_t colony_idx, int16_t building_idx);

    void __cdecl Init_Homeworld_Colony2_(int16_t planet_idx, int16_t player_idx, uint8_t flag);

    void __cdecl Init_Homeworld_Colony_(int16_t planet_idx, int16_t player_idx);

    int __cdecl Mox_Play_Sound_(int16_t sound_id);

    void __cdecl Init_Non_Homeworld_Colony_(int16_t planet_idx, int16_t player_idx);
    
    void __cdecl Do_All_Ships_XP_Check_();

    void __cdecl Do_Academy_At_Location_Check_(int16_t ship_index);

    void __cdecl Calc_Ship_Level_(int16_t ship_idx);

    void __cdecl Do_XP_For_Ship_(int16_t ship_index, int16_t xp_gain);

    void __cdecl Remove_Building_(int16_t colony_idx, int16_t building_idx);

    void __cdecl Best_Instructor_Bonus_(int16_t* instructor_bonus_by_player);
}