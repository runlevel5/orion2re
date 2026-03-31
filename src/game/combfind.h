#pragma once

namespace COMBFIND {
    inline uint8_t& _messed_up_fleet_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558BD0});
    inline s_combfind_target& _target = *reinterpret_cast<s_combfind_target*>(std::uintptr_t{0x0056CE38});
    inline int16_t (*&_player_needs_to_attack)[73] = *reinterpret_cast<int16_t (**)[73]>(std::uintptr_t{0x0056CE08});
    inline int8_t (*&_battle_stars) = *reinterpret_cast<int8_t**>(std::uintptr_t{0x0056CE14});
    inline uint8_t (*&_star_has_nonplayer)[73] = *reinterpret_cast<uint8_t (**)[73]>(std::uintptr_t{0x0056CE20});
    inline int16_t& _n_battle_stars = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CE2C});
    inline int8_t(*&_attack_confirmed)[8][8] = *reinterpret_cast<int8_t(**)[8][8]>(std::uintptr_t{ 0x0056CE64 });
    inline s_combfind_star_has_player (*&_star_has_player)[73][8] = *reinterpret_cast<s_combfind_star_has_player (**)[73][8]>(std::uintptr_t{0x0056CE68});
    inline uint8_t*& _current_star_has_nonplayer = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x005A4AAC});
    inline s_combfind_star_has_player*& _current_star_has_player = *reinterpret_cast<s_combfind_star_has_player**>(std::uintptr_t{0x005A4AB0});
    inline char*& _research_treaty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE00});
    inline char*& _s_attacks_you_at_s_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE04});
    inline char*& _outpost_destroyed_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE0C});
    inline char*& _combat_is_reported_between_s_and_s_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE10});
    inline char*& _you_have_following_treaties_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE18});
    inline char*& _receiving_tribute_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE1C});
    inline char*& _noncombat_ships_destroyed_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE24});
    inline char*& _combfind_strings_base = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE24});
    inline char*& _colony_seized_by_mind_control_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE28});
    inline char*& _really_risk_war_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE30});
    inline char*& _trade_treaty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE6C});

    void __cdecl Search_For_Battles_();

    struct s_retreat_report {
        int8_t reason;
        int16_t count;
        int8_t killed_officers;
    };

    void __cdecl Process_Retreating_Ships_();

    int16_t __cdecl Closest_Other_Colony_Star_(int16_t player_idx, int16_t star_idx);

    void __cdecl Pick_Star_And_Attacker_(int16_t* star_idx, int16_t* attacker_idx);

    void __cdecl Reset_One_Shot_System_Special_(s_star_data* star);

    void __cdecl Do_System_Discoveries_At_Star_(s_star_data* star);

    int16_t __cdecl Find_Planet_At_Star_Satisfying_(s_star_data* star, bool (__cdecl* predicate)(s_planet_data*));

    void __cdecl Update_Player_Needs_To_Attack_(int star_idx, int player_a_idx, int player_b_idx);

    void __cdecl Get_Local_Human_Target_(int16_t star_idx, int16_t player_idx, int16_t* target_player_idx_out, int16_t* target_colony_idx_out);

    int8_t __cdecl Human_Confirms_Attack_(int player_idx, int target_idx);

    void __cdecl Get_AI_Target_(int16_t star_idx, int16_t player_idx, int16_t* target_player, int16_t* target_colony, int8_t* is_sneak_attack);

    void __cdecl Get_Ship_Strength_Info_(uint32_t* enemy_strength, uint32_t* player_strength, uint32_t* shield_strength, int16_t star_idx, int16_t player_idx);

    void __cdecl Get_Best_Colony_Target_(
        uint32_t* enemy_fleet_space_str_arr,
        uint32_t* our_space_str_arr,
        uint32_t* our_ground_str_arr,
        int16_t star_idx,
        int16_t player_idx,
        uint32_t* best_score,
        int16_t* best_colony_idx,
        int32_t preferred_owner);

    void __cdecl Get_NPC_Target_(int16_t star_idx, int16_t event_type, int16_t *player_target_ptr, int16_t *colony_target_ptr);

    void __cdecl Do_Dragon_Bribe_(int player_idx, int star_idx, int16_t dest_idx);

    inline void __cdecl Client_Combat_Loop_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c03e0});
        fn();
    }

    void __cdecl Get_Combat_Ships_(int16_t* ships, int16_t star_idx, int16_t plr1_idx, int16_t* plr1_offset, int16_t* plr1_count, int16_t plr2_idx, int16_t* plr2_offset, int16_t* plr2_count);

    int16_t __cdecl Russ_Combat_(int16_t attacker_idx, int16_t* attacker_ships, int16_t attacker_count, int16_t defender_idx, int16_t* defender_ships, int16_t defender_count, int16_t star_idx, int16_t colony_idx, int8_t is_sneak_attack, s_battle_results* results);

    void __cdecl Kill_Noncombat_Ships_(int16_t* ship_indices, int16_t count);

    void __cdecl Determine_Retreat_Ships_(int16_t star_idx, int16_t player_idx, int16_t retreat_type);

    void __cdecl Reset_Combat_Ships_At_Current_Star_Has_Player_(int16_t player_idx);

    inline void __cdecl Do_Attacker_Beat_Colony_Stuff_(int16_t* ships, int16_t ship_count, int16_t colony_idx, s_battle_results* result) {
        using Fn = void(__cdecl*)(int16_t*, int16_t, int16_t, s_battle_results*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004bf0b0})(ships, ship_count, colony_idx, result);
    }

    char* __cdecl Load_Combfind_Strings_(char* strings_block);

    s_colony* __cdecl Free_Colony_Slot_();

    bool __cdecl Planet_Has_Splinter_Colony_(s_planet_data* planet);

    int16_t __cdecl Target_Planet_To_Colony_(int32_t planet_idx);

    int16_t __cdecl Target_Colony_To_Planet_(int32_t colony_idx);

    short __cdecl Do_1_Combat_(short star_idx, short plr2_idx, short plr1_idx, short planet_idx, s_battle_results *result, int8_t is_sneak_attack);

    bool __cdecl AI_Can_Retreat_(int player_idx, int excluded_colony_idx);


    inline bool __cdecl Some_Star_Player_Is_Busy_(int16_t star_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004BCD50 });
        return fn(star_idx);
    }

    int16_t __cdecl Pick_Attacker_At_Star_(int16_t star_idx);

    void __cdecl Collect_Targets_For_Combat_(int16_t star_idx, int16_t player_idx);

    inline bool __cdecl Player_May_Attack_Player_(int player_a, int player_b) {
        using Fn = bool(__cdecl*)(int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BCC90});
        return fn(player_a, player_b);
    }

}