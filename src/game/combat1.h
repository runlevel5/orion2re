#pragma once
#include <cstdint>

namespace COMBAT1 {
    inline int32_t& _end_of_turn_button = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00565E98});

    int16_t __cdecl Combat_Ship_Is_Functional_(int16_t ship_idx);

    void __cdecl Init_Ship_For_Start_Of_Turn_(int16_t ship_idx);

    int __cdecl qsort_ship_initiative_(const void* p1, const void* p2);

    int16_t __cdecl Ship_Initiative_Value_(int16_t ship_idx);

    inline uint16_t& _start_of_turn = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A6E10});

    int16_t __cdecl Next_Ship_(int16_t param_1);

    int16_t __cdecl Ship_Controlled_By_Computer_(int16_t ship_idx);

    int32_t __cdecl Send_Tactical_Combat_Message_(int16_t player_idx, int16_t p2, int16_t p3, int16_t p4, int16_t p5);

    void __cdecl Add_Tactical_Combat_Fields_(void);

    void __cdecl Add_Missile_Fields_(void);

    void __cdecl Assign_Combat_Grids_(int16_t ship_idx);

    void __cdecl Set_Legal_Moves_(int16_t ship_idx, int16_t range, int16_t check_cost);

    void __cdecl Assign_Mouse_Images_();

    int16_t __cdecl Ship_Done_Check_(void);

    void __cdecl Set_Cur_Ship_To_(int16_t ship_idx);

    void __cdecl Set_Combat_Help_Fields_();

    int16_t __cdecl Enemy_Dissipator_Exists_(int16_t player_idx);

    void __cdecl Initiate_Boarding_Action_(int16_t attacker_idx, uint16_t defender_idx);

    void __cdecl Seeking_Missiles_(int16_t player_idx);

    void __cdecl Do_Combat_Turn_(int16_t param_1, int16_t param_2);
    int16_t __cdecl Check_For_Winner_();
    int16_t __cdecl Calc_Current_Speed_(int16_t ship_idx);

    void __cdecl Retreat_Ship_(uint16_t ship_idx);

    void __cdecl Remove_Continuing_Effect_Specials_(int16_t source_idx);

    int16_t __cdecl Get_Number_Of_Ships_In_Stasis_(int16_t source_ship_idx);

    bool __cdecl Weapon_Is_On_(int16_t ship_idx, int16_t weapon_idx);
}