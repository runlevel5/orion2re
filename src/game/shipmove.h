#pragma once
#include <cstdint>
#include "types.h"

namespace SHIPMOVE {
    void __cdecl Allocate_New_Ship_Slots_();
    void __cdecl Move_All_Ships_Toward_Stars_();
    void __cdecl Reset_Needless_Ignore_Combat_Ships_Flags_();

    void __cdecl Update_All_Ship_Ranges_();

    bool __cdecl Star_In_Normal_Range_Of_Player_(s_star_data* star, int16_t player_idx);

    bool __cdecl Star_In_Range_Of_Player_Aux1_(s_star_data* star, int16_t player_idx, int16_t dist);
    bool __cdecl Star_In_Range_Of_Player_Aux2_(s_star_data* star, int16_t player_idx, int16_t dist);
    bool __cdecl Star_In_Range_Of_Player_(s_star_data* star, int16_t player_idx, int16_t dist);

    void __cdecl Update_Player_Ship_Range_(s_player* player);

    inline void __cdecl Ships_Try_To_Move_To_(int16_t* ids, int16_t target, s_ship_move_info* out_info) {
        using Fn = void(__cdecl*)(int16_t*, int16_t, s_ship_move_info*);
        reinterpret_cast<Fn>(0x004363E0)(ids, target, out_info);
    }

    void __cdecl Make_Ships_Move_To_(int16_t* ship_list, int16_t destination_id, s_ship_move_info* move_info);

    inline void __cdecl Reset_Needless_Ignore_Combat_Ships_Flags_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004377d0 });
        fn();
    }

    int32_t __cdecl Create_Nonplayer_Ship_(int16_t owner, s_ship_design* design, int16_t x, int16_t y);

    inline s_ship_move_info& _g_ship_move_info = *reinterpret_cast<s_ship_move_info*>(std::uintptr_t{0x0059C440});

    void __cdecl Unmark_Just_Built_AI_Ships_();

    inline uint32_t __cdecl Player_N_Turns_From_Star_To_Star_(int16_t player_idx, int16_t start_star, int16_t dest_star) {
        using Fn = uint32_t(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004368e0});
        return fn(player_idx, start_star, dest_star);
    }

    int16_t __cdecl Ship_Destination_Star_(s_ship_data* ship);

    int16_t __cdecl Ship_Range_(s_ship_data* ship);

    bool __cdecl Can_Order_Ship_(s_ship_data* ship);

    bool __cdecl Ship_Is_Moving_(s_ship_data* ship);

    void __cdecl Make_Ship_Arrive_At_Star_(s_ship_data* ship, int16_t star_idx);
    void __cdecl Update_Ship_ETAs_(int32_t player_id);
}