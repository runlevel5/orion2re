#pragma once

struct s_colony;

namespace invasion {
    inline int __cdecl Unload_Transports_(s_colony* col, int16_t* ship_array, int count) {
        using Fn = int(__cdecl*)(s_colony*, int16_t*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0042cdc0 });
        return fn(col, ship_array, count);
    }

    void __cdecl Compute_Player_Ground_Combat_Bonuses_(int player_idx, s_invasion_player_bonuses* bonuses, char is_defender);

    inline void __cdecl Check_Rebellion_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042c650});
        fn(colony);
    }

    inline void __cdecl Enforce_Population_Limits_At_Colony_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042bcb0});
        fn(colony);
    }

    void __cdecl Check_All_Rebellions_();

    void __cdecl Find_Troops_(s_invasion_side_info* side_info);

    void __cdecl Assign_New_Home_Planet_(int8_t player_idx, int16_t excluded_colony_idx);

    inline void __cdecl Compute_Ground_Combat_Info_(s_invasion_side_info* side, s_invasion_player_bonuses* own_mods, s_invasion_player_bonuses* opponent_mods) {
        using Fn = void(__cdecl*)(s_invasion_side_info*, s_invasion_player_bonuses*, s_invasion_player_bonuses*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042b820});
        fn(side, own_mods, opponent_mods);
    }

    void __cdecl Get_Boarding_Info_(int16_t attacker_player_id, int16_t attacker_unit_count, int16_t attacker_additional_modifier, int16_t defender_player_id, int16_t defender_unit_count, int16_t defender_additional_modifier, s_invasion_side_info* side_a, s_invasion_side_info* side_b);

    void __cdecl Ground_Combat_Round_(s_invasion_side_info* side1, s_invasion_side_info* side2);
}