#pragma once

namespace AIMOVE {
    inline s_player*& _p_ai = *reinterpret_cast<s_player**>(std::uintptr_t{0x00565E80});
    inline s_ai_colonizable_planet*& _colonizable = *reinterpret_cast<s_ai_colonizable_planet**>(std::uintptr_t{0x00565E84});
    inline int8_t (*&_nonplayer_here)[72] = *reinterpret_cast<int8_t (**)[72]>(std::uintptr_t{0x00565E88});
    inline int8_t*& _flee_stars = *reinterpret_cast<int8_t**>(std::uintptr_t{0x00565E8C});
    inline int16_t& _i_ai = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E90});
    inline int16_t& _n_colonizable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E94});

    void __cdecl Move_AI_(int16_t player_idx);

    int __cdecl Sort_colonizable_(const void* a, const void* b);

    void __cdecl Delete_Already_Assigned_Planets_(s_ai_colonizable_planet* colonizable_planets, int16_t* num_planets);

    void __cdecl Launch_Sneak_Attack_();

    void __cdecl Launch_Attacks_(int param_1, int param_2);

    void __cdecl Find_Best_Attack_From_Star_(int16_t star_idx, int16_t* target_info, int mission_type, int param_4);

    inline int8_t __cdecl AI_May_Attack_Player_(int16_t ai_player, int16_t target_player) {
        using Fn = int8_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00460eb0});
        return fn(ai_player, target_player);
    }

    inline int16_t __cdecl Evaluate_Colony_Attack_Score_(int16_t colony_idx, int16_t* ship_list, int16_t turns_left, int8_t* mission, int8_t* visited_star, int16_t* target_info, int param_4) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t*, int16_t, int8_t*, int8_t*, int16_t*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00463090});
        return fn(colony_idx, ship_list, turns_left, mission, visited_star, target_info, param_4);
    }

    void __cdecl Intercept_Invaders_();

    inline void __cdecl Interceptions_(s_aimove_intercept_entry* intercepts, int* count, int8_t* star_to_intercept_idx, int is_desperate) {
        using Fn = void(__cdecl*)(s_aimove_intercept_entry*, int*, int8_t*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00463ce0 });
        fn(intercepts, count, star_to_intercept_idx, is_desperate);
    }

    void __cdecl Assign_New_Colony_Ship_Destinations_(s_ai_colonizable_planet* planets, int16_t* count);

    bool __cdecl Star_Has_Monster_(int star_idx);

    void __cdecl Get_Best_Unescorted_Colony_Ship_(int16_t* ship_indices, int16_t* ship_count, int16_t destination);

    int16_t __cdecl I_Soonest_Enemy_Colony_Ship_(int16_t* ship_indices, int16_t count, int16_t target_location);

    inline bool __cdecl Get_Best_Escorted_Colony_Ship_(int16_t* ship_list, int16_t* count, int32_t star_idx, int32_t max_eta) {
        using Fn = bool(__cdecl*)(int16_t*, int16_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00461eb0 });
        return fn(ship_list, count, star_idx, max_eta);
    }

    void __cdecl Move_Transports_();

    void __cdecl Move_To_Stage_Points_();

    void __cdecl Move_To_Unexplored_Stars_();

    void __cdecl Move_NPCs_();

    inline int32_t __cdecl Npc_Dest_(int16_t ship_idx, uint8_t* star_flags) {
        using Fn = int32_t(__cdecl*)(int16_t, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00465BA0});
        return fn(ship_idx, star_flags);
    }

    void __cdecl Move_All_AI_();

    void __cdecl Mark_Flee_Stars_(int16_t player_idx);

    void __cdecl Get_Colonizable_Planets_(s_ai_colonizable_planet* colonizable_planets, int16_t* count);

    int __cdecl Gather_Player_Ships_At_Star_(int16_t player_id, int16_t star_id, int16_t* ships_list);

    void __cdecl Player_Threatens_Player_(int plr1, int plr2, int* star_idx, int* aux_info, int* planet_idx);

    int16_t __cdecl Best_Uncolonized_Undestined_Planet_(int16_t player_idx);

    uint32_t __cdecl Ship_Rough_Strength_At_Star_(int16_t ship_idx, int16_t star_idx);

    void __cdecl Get_Enemy_Strength_On_Turn_(int16_t star_id, int* turn_strengths, int* cumulative_strengths);

    int __cdecl Number_Of_Attacks_Underway_(int16_t player_idx, int16_t target_idx);

    void __cdecl Find_Opportunity_Attack_(int16_t player_idx, int16_t target_idx, int16_t threshold, int16_t* out_planet, int16_t* out_value, int32_t* out_ratio);

    void __cdecl Move_Ships_With_Possible_Intermediate_(int16_t* ship_array, int16_t dest_star, int16_t* out_star, int16_t* out_turns);

    int32_t __cdecl Enemy_Colony_Worth_To_Player_(int16_t colony_idx, int16_t player_idx);

    inline void __cdecl Find_Opportunity_Attack_Aux_(int16_t plr1, int16_t plr2, int16_t val, int16_t star_idx, int16_t* planet_idx, int16_t* unknown, int32_t* aux_info) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t*, int16_t*, int32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00465e80 });
        fn(plr1, plr2, val, star_idx, planet_idx, unknown, aux_info);
    }
}