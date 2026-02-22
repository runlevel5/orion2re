#pragma once

namespace aimove {
    inline s_player*& _p_ai = *reinterpret_cast<s_player**>(std::uintptr_t{0x00565E80});
    inline s_ai_colonizable_planet*& _colonizable = *reinterpret_cast<s_ai_colonizable_planet**>(std::uintptr_t{0x00565E84});
    inline int8_t (*&_nonplayer_here)[72] = *reinterpret_cast<int8_t (**)[72]>(std::uintptr_t{0x00565E88});
    inline int8_t (*&_flee_stars)[720] = *reinterpret_cast<int8_t (**)[720]>(std::uintptr_t{0x00565E8C});
    inline int16_t& _i_ai = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E90});
    inline int16_t& _n_colonizable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565E94});

    void __cdecl Move_AI_(int32_t player_idx);

    int __cdecl Sort_colonizable_(const void* a, const void* b);

    void __cdecl Delete_Already_Assigned_Planets_(s_ai_colonizable_planet* colonizable_planets, int16_t* num_planets);

    void __cdecl Launch_Sneak_Attack_();

    inline void __cdecl Launch_Attacks_(int param_1, int param_2) {
        using Fn = void(__cdecl*)(int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00462CB0});
        fn(param_1, param_2);
    }

    inline void __cdecl Intercept_Invaders_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00463760});
        fn();
    }

    inline void __cdecl Assign_New_Colony_Ship_Destinations_(s_ai_colonizable_planet* planets, int16_t* count) {
        using Fn = void(__cdecl*)(s_ai_colonizable_planet*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00461760});
        fn(planets, count);
    }

    inline void __cdecl Move_Transports_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00464C40});
        fn();
    }

    inline void __cdecl Move_To_Stage_Points_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00464620});
        fn();
    }

    inline void __cdecl Move_To_Unexplored_Stars_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00465390});
        fn();
    }

    inline void __cdecl Move_NPCs_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00465910 });
        fn();
    }

    void __cdecl Move_All_AI_();

    void __cdecl Mark_Flee_Stars_(int16_t player_idx);

    void __cdecl Get_Colonizable_Planets_(s_ai_colonizable_planet* colonizable_planets, int16_t* count);

    int __cdecl Gather_Player_Ships_At_Star_(int16_t player_id, int16_t star_id, int16_t* ships_list);

    void __cdecl Player_Threatens_Player_(int plr1, int plr2, int* star_idx, int* aux_info, int* planet_idx);

    int16_t __cdecl Best_Uncolonized_Undestined_Planet_(int16_t player_idx);

    void __cdecl Get_Enemy_Strength_On_Turn_(int16_t star_id, int* turn_strengths, int* cumulative_strengths);

    inline void __cdecl Find_Opportunity_Attack_(int16_t player_idx, int16_t target_idx, int16_t threshold, int16_t* out_planet, int16_t* out_value, int32_t* out_ratio) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t*, int16_t*, int32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00465E00 });
        fn(player_idx, target_idx, threshold, out_planet, out_value, out_ratio);
    }
}