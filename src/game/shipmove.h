#pragma once
#include <cstdint>
#include "types.h"

struct s_wormhole_data;

namespace SHIPMOVE {
    extern uint8_t _jump_gate_speed_bonus;
    extern bool _jump_gate_pass_nebula;

    void __cdecl Allocate_New_Ship_Slots_();
    void __cdecl Move_All_Ships_Toward_Stars_();
    void __cdecl Reset_Needless_Ignore_Combat_Ships_Flags_();

    void __cdecl Update_All_Ship_Ranges_();

    bool __cdecl Star_In_Normal_Range_Of_Player_(s_star_data* star, int16_t player_idx);

    bool __cdecl Star_In_Extended_Range_Of_Player_(s_star_data* star, int16_t player_idx);

    bool __cdecl Star_In_Range_Of_Player_Aux1_(s_star_data* star, int16_t player_idx, int16_t dist);
    bool __cdecl Star_In_Range_Of_Player_Aux2_(s_star_data* star, int16_t player_idx, int16_t dist);
    bool __cdecl Star_In_Range_Of_Player_(s_star_data* star, int16_t player_idx, int16_t dist);

    void __cdecl Update_Player_Ship_Range_(s_player* player);

    void __cdecl Ships_Try_To_Move_To_(int16_t* ship_list, int16_t target_star_idx, s_ship_move_info* move_info);

    void __cdecl Make_Ships_Move_To_(int16_t* ship_list, int16_t destination_star_idx, s_ship_move_info* move_info);
    bool __cdecl Apply_Player_Movement_Order_(int16_t* ship_list, int16_t destination_star_idx, s_ship_move_info* move_info);

    bool __cdecl Confirm_Officer_Change_(int16_t leader_idx, int16_t target_ship_idx, int8_t target_eta);
    void __cdecl Recalculate_Moving_Fleet_(int16_t ship_idx);
    void __cdecl Recalculate_All_Moving_Fleets_();

    int16_t __cdecl Create_Nonplayer_Ship_(int8_t owner, s_ship_design* design, int16_t x, int16_t y);

    bool __cdecl Player_Has_Ships_Pestering_Other_Player_(int player_idx, uint8_t colony_bit_idx);

    void __cdecl Unmark_Just_Built_AI_Ships_();

    int16_t __cdecl Player_N_Turns_From_Star_To_Star_(int16_t player_idx, int16_t start_star_idx, int16_t end_star_idx);

    int16_t __cdecl Ship_Destination_Star_(s_ship_data* ship);

    int16_t __cdecl Ship_Range_(s_ship_data* ship);

    bool __cdecl Can_Order_Ship_(s_ship_data* ship);

    bool __cdecl Ship_Is_Moving_(const s_ship_data* ship);

    void __cdecl Make_Ship_Arrive_At_Star_(s_ship_data* ship, int16_t star_idx);
    void __cdecl Update_Ship_ETAs_(int16_t player_id);

    void __cdecl Do_Hyperspace_Beast_(int32_t player_idx);

    void __cdecl Do_Wormhole_Event_(int32_t player_idx, s_wormhole_data* wormhole);
}