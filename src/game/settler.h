#pragma once

namespace settler {
    int32_t __cdecl N_Freighters_Player_Can_Scrap_(int16_t player_idx);

    bool __cdecl Room_For_Another_(s_colony* colony, int16_t race_idx, int8_t include_freighters);

    int8_t __cdecl Add_Settler_To_Colony_(uint32_t settler_data, s_colony* colony);

    void __cdecl Move_Settlers_();
}