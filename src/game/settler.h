#pragma once

namespace settler {
    int32_t __cdecl N_Freighters_Player_Can_Scrap_(int16_t player_idx);

    bool __cdecl Room_For_Another_(s_colony* colony, int16_t race_idx, int8_t include_freighters);

    int8_t __cdecl Add_Settler_To_Colony_(uint32_t settler_data, s_colony* colony);

    void __cdecl Move_Settlers_();

    inline void __cdecl Pop_Tries_To_Settle_(s_colony* from, int16_t pop_idx, s_colony* to, uint8_t* flags) {
        using Fn = void(__cdecl*)(s_colony*, int16_t, s_colony*, uint8_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0043b820})(from, pop_idx, to, flags);
    }

    inline int32_t __cdecl Settler_ETA_(s_player* player, int32_t from_star_idx, int32_t to_star_idx) {
        using Fn = int32_t(__cdecl*)(s_player*, int32_t, int32_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0043ba50})(player, from_star_idx, to_star_idx);
    }

    void __cdecl Settle_Pop_(s_colony* source_colony, int16_t pop_index, s_colony* dest_colony);
}