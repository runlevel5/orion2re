#pragma once
#include <cstdint>
#include "types.h"

namespace SHIPMOVE {
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

    void __cdecl Make_Ships_Move_To_(int16_t* ship_list, int16_t destination_id, s_ship_move_info* move_info);

    int32_t __cdecl Create_Nonplayer_Ship_(int16_t owner, s_ship_design* design, int16_t x, int16_t y);

    inline s_ship_move_info& _g_ship_move_info = *reinterpret_cast<s_ship_move_info*>(std::uintptr_t{0x0059C440});

    inline uint8_t __cdecl Player_Has_Ships_Pestering_Other_Player_(int16_t param_1, int16_t param_2) {
        using Fn = uint8_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00437910});
        return fn(param_1, param_2);
    }

    void __cdecl Unmark_Just_Built_AI_Ships_();

    int16_t __cdecl Player_N_Turns_From_Star_To_Star_(int16_t player_idx, int16_t start_star_idx, int16_t end_star_idx);

    int16_t __cdecl Ship_Destination_Star_(s_ship_data* ship);

    int16_t __cdecl Ship_Range_(s_ship_data* ship);

    bool __cdecl Can_Order_Ship_(s_ship_data* ship);

    bool __cdecl Ship_Is_Moving_(s_ship_data* ship);

    void __cdecl Make_Ship_Arrive_At_Star_(s_ship_data* ship, int16_t star_idx);
    void __cdecl Update_Ship_ETAs_(int32_t player_id);

    inline void __cdecl Do_Hyperspace_Beast_(int32_t player_idx) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004374e0});
        fn(player_idx);
    }

    inline void __cdecl Do_Wormhole_Event_(int16_t player_idx, void* data) {
        using Fn = void(__cdecl*)(int16_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004373A0});
        fn(player_idx, data);
    }
}