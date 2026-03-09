#pragma once

namespace COMBFIND {
    inline uint8_t& _messed_up_fleet_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558BD0});
    inline s_combfind_target& _target = *reinterpret_cast<s_combfind_target*>(std::uintptr_t{0x0056CE38});
    inline int16_t (*&_player_needs_to_attack)[73] = *reinterpret_cast<int16_t (**)[73]>(std::uintptr_t{0x0056CE08});
    inline int8_t (*&_battle_stars) = *reinterpret_cast<int8_t**>(std::uintptr_t{0x0056CE14});
    inline int8_t (*&_star_has_nonplayer)[73] = *reinterpret_cast<int8_t (**)[73]>(std::uintptr_t{0x0056CE20});
    inline int16_t& _n_battle_stars = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CE2C});
    inline int8_t(*&_attack_confirmed)[8][8] = *reinterpret_cast<int8_t(**)[8][8]>(std::uintptr_t{ 0x0056CE64 });
    inline s_combfind_star_has_player (*&_star_has_player)[73][8] = *reinterpret_cast<s_combfind_star_has_player (**)[73][8]>(std::uintptr_t{0x0056CE68});
    inline uint32_t& _current_star_has_nonplayer = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A4AAC});
    inline uint32_t& _current_star_has_player = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A4AB0});
    inline char*& _research_treaty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE00});
    inline char*& _s_attacks_you_at_s_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE04});
    inline char*& _outpost_destroyed_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE0C});
    inline char*& _combat_is_reported_between_s_and_s_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE10});
    inline char*& _you_have_following_treaties_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE18});
    inline char*& _receiving_tribute_msg = *reinterpret_cast<char**>(std::uintptr_t{0x0056CE1C});
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

    void __cdecl Update_Player_Needs_To_Attack_(int star_idx, int player_idx1, int player_idx2);

    void __cdecl Get_Local_Human_Target_(int16_t star_idx, int16_t player_idx, int16_t* target_player_idx_out, int16_t* target_colony_idx_out);

    int8_t __cdecl Human_Confirms_Attack_(int player_idx, int target_idx);

    void __cdecl Get_AI_Target_(int16_t star_idx, int16_t player_idx, int16_t* target_player, int16_t* target_colony, int8_t* is_sneak_attack);

    inline void __cdecl Get_Ship_Strength_Info_(uint32_t* bonus_info, uint32_t* our_strength, uint32_t* enemy_strength, int16_t star_idx, int16_t player_idx) {
        using Fn = void(__cdecl*)(uint32_t*, uint32_t*, uint32_t*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bdc10});
        fn(bonus_info, our_strength, enemy_strength, star_idx, player_idx);
    }

    inline void __cdecl Get_Best_Colony_Target_(uint32_t* enemy_strength, uint32_t* our_strength, uint32_t* bonus_info, int16_t star_idx, int16_t player_idx, uint32_t* best_score, int16_t* best_colony_idx, int16_t sneak_attack_target) {
        using Fn = void(__cdecl*)(uint32_t*, uint32_t*, uint32_t*, int16_t, int16_t, uint32_t*, int16_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bde70});
        fn(enemy_strength, our_strength, bonus_info, star_idx, player_idx, best_score, best_colony_idx, sneak_attack_target);
    }

    void __cdecl Get_NPC_Target_(int16_t star_idx, int16_t event_type, int16_t *player_target_ptr, int16_t *colony_target_ptr);

    inline void __cdecl Do_1_Combat_(int star_idx, int attacker_idx, int defender_idx, int planet_idx, uint32_t* unk_ptr, int8_t unk_flag) {
        using Fn = void(__cdecl*)(int, int, int, int, uint32_t*, int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004be3b0});
        fn(star_idx, attacker_idx, defender_idx, planet_idx, unk_ptr, unk_flag);
    }

    void __cdecl Do_Dragon_Bribe_(int player_idx, int star_idx, int16_t dest_idx);

    inline void __cdecl Client_Combat_Loop_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c03e0});
        fn();
    }

    char* __cdecl Load_Combfind_Strings_(char* strings_block);

    s_colony* __cdecl Free_Colony_Slot_();

    bool __cdecl Planet_Has_Splinter_Colony_(s_planet_data* planet);

    int16_t __cdecl Target_Planet_To_Colony_(int32_t planet_idx);

    int16_t __cdecl Target_Colony_To_Planet_(int32_t colony_idx);
}