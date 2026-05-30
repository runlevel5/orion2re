#pragma once
#include <cstdint>

struct s_player;

namespace newtech {
    extern int16_t _best_category[12];

    extern int32_t _tech_research_level_values[30];

    int16_t __cdecl Get_Competition_Tech_Values_(int16_t category, int16_t player_idx);

    void __cdecl Set_Competition_Tech_Values_(int16_t player_idx);

    int32_t __cdecl Get_Competition_Player_Tech_Value_(int16_t player_idx, uint16_t target_ai_group);

    int32_t __cdecl Calc_Tech_Value_(int16_t player_idx, int16_t tech_app_idx);

    int8_t __cdecl All_Gray_Boxes_Researched_(s_player* player);

    int32_t __cdecl Choose_Hyper_Advanced_Tech_();

    int16_t __cdecl Choose_Tech_Application_(int16_t player_idx);
}