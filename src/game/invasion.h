#pragma once
namespace invasion {
    int __cdecl Unload_Transports_(s_colony* colony, int16_t* transport_ship_ids, int num_transports);

    int32_t __cdecl Colony_Infantry_Limit_(s_colony* colony);

    void __cdecl Compute_Player_Ground_Combat_Bonuses_(int player_idx, s_invasion_player_bonuses* bonuses, char is_defender);

    void __cdecl Check_Rebellion_(s_colony* colony);

    void __cdecl Get_Rebellion_Info_(int16_t attacker_player_id, int16_t marines_count, s_colony* colony, s_invasion_side_info* side_a, s_invasion_side_info* side_b);

    int __cdecl Colony_N_Militia_(s_colony* colony);

    void __cdecl Compute_Attacker_Officer_Bonus_(int16_t* ship_indexes, int16_t count, s_invasion_player_bonuses* bonuses);

    void __cdecl Compute_Colony_Officer_Bonus_(s_colony* colony, s_invasion_player_bonuses* bonuses);

    void __cdecl Change_Colony_Ownership_(s_colony* colony, uint32_t new_owner_idx, s_battle_results_info* results);

    void __cdecl Change_Pop_Ownership_(int16_t colony_idx, uint32_t player_idx);

    void __cdecl Resolve_Rebellion_Troops_(s_invasion_side_info info);

    void __cdecl Enforce_Population_Limits_At_Colony_(s_colony* colony);

    void __cdecl Check_All_Rebellions_();

    void __cdecl Find_Troops_(s_invasion_side_info* side_info);

    void __cdecl Assign_New_Home_Planet_(int8_t player_idx, int16_t excluded_colony_idx);

    void __cdecl Compute_Ground_Combat_Info_(s_invasion_side_info* side, s_invasion_player_bonuses* own_mods, s_invasion_player_bonuses* opponent_mods);

    void __cdecl Get_Boarding_Info_(int16_t attacker_player_id, int16_t attacker_unit_count, int16_t attacker_additional_modifier, int16_t defender_player_id, int16_t defender_unit_count, int16_t defender_additional_modifier, s_invasion_side_info* side_a, s_invasion_side_info* side_b);

    void __cdecl Ground_Combat_Round_(s_invasion_side_info* side1, s_invasion_side_info* side2);

    void __cdecl Resolve_Ground_Combat_(s_invasion_side_info* side_a, s_invasion_side_info* side_b);

    int __cdecl Colony_Tank_Barracks_Limit_(s_colony* colony);

    int32_t __cdecl Colony_Tank_Limit_(s_colony* colony);

    uint32_t __cdecl Colony_Infantry_Barracks_Limit_(s_colony* colony);

    void __cdecl Get_Invasion_Info_(int16_t* ship_list, int16_t n_ships, s_colony* colony, s_invasion_side_info* attacker_side, s_invasion_side_info* defender_side);

    void __cdecl Invade_(int16_t* ship_list, int16_t n_ships, int16_t colony_idx, s_battle_results_info* results);

    void __cdecl Resolve_Invasion_Troops_(s_invasion_side_info info);

    bool __cdecl Has_Defending_Troops_(int8_t attacker_id, int16_t* ship_list, int n_ships, int16_t colony_idx);

    void __cdecl Compute_Colony_Ground_Combat_Info_(s_colony* colony, s_invasion_side_info* side_info);
}