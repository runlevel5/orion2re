#pragma once
#include <cstdint>
#include "types.h"

namespace SHIPMOVE {
    void __cdecl Allocate_New_Ship_Slots_();

    void __cdecl Update_All_Ship_Ranges_();

    bool __cdecl Star_In_Normal_Range_Of_Player_(s_star_data* star, int16_t player_idx);

    bool __cdecl Star_In_Range_Of_Player_Aux1_(s_star_data* star, int16_t player_idx, int16_t dist);
    bool __cdecl Star_In_Range_Of_Player_Aux2_(s_star_data* star, int16_t player_idx, int16_t dist);
    bool __cdecl Star_In_Range_Of_Player_(s_star_data* star, int16_t player_idx, int16_t dist);

    void __cdecl Update_Player_Ship_Range_(s_player* player);
}