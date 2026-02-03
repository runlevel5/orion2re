#pragma once
#include <cstdint>

namespace INITSHIP {
    void __cdecl Init_Ship_Designs_(int16_t player_idx);
    int16_t __cdecl Best_Ship_Fuel_(int16_t player_idx);

    void __cdecl Update_Player_Ship_Designs_(int16_t player_idx);

    void __cdecl Update_Player_Strategic_Ship_Designs_(int16_t player_idx);

    void __cdecl Update_Automatic_Design_Equipment_(int16_t player_idx);

    void __cdecl Update_Equipment_On_Ships_Being_Built_(int16_t player_idx);

    int16_t __cdecl Best_Warp_Drive_(int16_t player_idx);

    int16_t __cdecl Calc_Player_FTL_Speed_(int16_t player_idx);

    int16_t __cdecl Best_Armor_(int16_t player_idx);

    int16_t __cdecl Get_FTL_Speed_(int16_t player_idx, int16_t ftl_type);
}