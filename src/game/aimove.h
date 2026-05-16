#pragma once

namespace AIMOVE {
#pragma pack(push, 1)
    struct s_ai_stage_point {
        uint8_t star_idx;
        union {
            uint32_t strength;
            struct {
                uint8_t pad[3];
                uint8_t turns_left;
            };
        };
    };
#pragma pack(pop)

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

    void __cdecl Find_Best_Attack_From_Star_(int16_t star_idx, uint8_t* target_info, int mission_type, int param_4);

    int8_t __cdecl AI_May_Attack_Player_(int16_t player_idx, int16_t target_idx);

    uint16_t __cdecl Compute_Attack_Worthiness_(
        int16_t colony_idx,
        int16_t* ship_list,
        int16_t eta,
        int8_t* out_attack_type,
        int8_t* out_flee,
        uint8_t* target_info,
        int32_t min_pop
    );

    int8_t __cdecl Player_Is_Hostile_To_Player_(int player_idx_a, int player_idx_b);

    int32_t __cdecl N_Enemy_Stars_Reachable_(int16_t player_idx, int16_t player_bit, int16_t source_star_idx);

    void __cdecl Intercept_Invaders_();

    void __cdecl Interceptions_(s_aimove_intercept_entry* intercepts, int* count, int8_t* star_to_intercept_idx, int is_desperate);

    void __cdecl Assign_New_Colony_Ship_Destinations_(s_ai_colonizable_planet* planets, int16_t* count);

    bool __cdecl Star_Has_Monster_(int star_idx);

    void __cdecl Get_Best_Unescorted_Colony_Ship_(int16_t* ship_indices, int16_t* ship_count, int16_t destination);

    int16_t __cdecl I_Soonest_Enemy_Colony_Ship_(int16_t* ship_indices, int16_t count, int16_t target_location);

    int8_t __cdecl Get_Best_Escorted_Colony_Ship_(int16_t* colony_ships, int16_t* num_colony_ships, int16_t star_idx, int16_t max_turns);

    void __cdecl Move_Transports_();

    void __cdecl Move_To_Stage_Points_();

    void __cdecl Move_To_Unexplored_Stars_();

    void __cdecl Move_NPCs_();

    int __cdecl Sort_npc_dests_(const void* a, const void* b);

    int16_t __cdecl Npc_Dest_(int16_t ship_idx, uint8_t* star_flags);

    void __cdecl Move_All_AI_();

    void __cdecl Mark_Flee_Stars_(int16_t player_idx);

    void __cdecl Get_Colonizable_Planets_(s_ai_colonizable_planet* colonizable_planets, int16_t* count);

    int __cdecl Gather_Player_Ships_At_Star_(int16_t player_id, int16_t star_id, int16_t* ships_list);

    void __cdecl Gather_Intercepting_Ships_(int16_t* ships_list, int32_t* ship_count, int8_t* intercept_map);

    void __cdecl Knock_Out_Impossible_Interceptions_(s_aimove_intercept_entry* intercepts, int* count, int8_t* temp_arr, int* temp_count, int keep_one_entry);

    uint16_t __cdecl Ship_Min_Strength_Versus_Players_At_Star_(int16_t ship_idx, uint16_t player_mask, int star_idx);

    void __cdecl Player_Threatens_Player_(int plr1, int plr2, int* star_idx, int* aux_info, int* planet_idx);

    int16_t __cdecl Best_Uncolonized_Undestined_Planet_(int16_t player_idx);

    uint32_t __cdecl Ship_Rough_Strength_At_Star_(int16_t ship_idx, int16_t star_idx);

    void __cdecl Get_Enemy_Strength_On_Turn_(int16_t star_id, int* turn_strengths, int* cumulative_strengths);

    int __cdecl Number_Of_Attacks_Underway_(int16_t player_idx, int16_t target_idx);

    void __cdecl Find_Opportunity_Attack_(int16_t player_idx, int16_t target_idx, int16_t threshold, int16_t* out_planet, int16_t* out_value, int32_t* out_ratio);

    void __cdecl Move_Ships_With_Possible_Intermediate_(int16_t* ship_array, int16_t dest_star, int16_t* out_star, int16_t* out_turns);

    int32_t __cdecl Enemy_Colony_Worth_To_Player_(int16_t colony_idx, int16_t player_idx);

    void __cdecl Find_Opportunity_Attack_Aux_(
        int16_t player_id,
        int16_t val1,
        int16_t val2,
        int16_t star_idx,
        int16_t* out_target_star,
        int16_t* out_param_6,
        int32_t* out_param_7
    );

    int __cdecl Sort_candidate_stars_(const void* candidate_star_a, const void* candidate_star_b);
}