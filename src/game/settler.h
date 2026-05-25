#pragma once

namespace settler {
    int32_t __cdecl N_Freighters_Player_Can_Scrap_(int16_t player_idx);

    bool __cdecl Room_For_Another_(s_colony* colony, int16_t race_idx, int8_t include_freighters);

    int8_t __cdecl Add_Settler_To_Colony_(uint32_t settler_data, s_colony* colony);

    void __cdecl Move_Settlers_();

    void __cdecl Pop_Tries_To_Settle_(
            s_colony *source_colony,
            int16_t selected_pop_idx,
            s_colony *dest_colony,
            s_settler_settle_result *result);

    int32_t __cdecl Settler_ETA_(s_player* player, int16_t source_star, int16_t dest_star);

    int32_t __cdecl Settle_Pop_(s_colony* source_colony, int16_t pop_index, s_colony* dest_colony);
}