#pragma once
#include <cstdint>

struct s_combfind_star_has_player;
struct s_planet_data;

namespace MOX {
    struct s_star_data;
}

namespace COMBFIND {
    inline uint8_t& _messed_up_fleet_palette = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558BD0});
    inline int16_t (*&_player_needs_to_attack)[73] = *reinterpret_cast<int16_t (**)[73]>(std::uintptr_t{0x0056CE08});
    inline int8_t (*&_battle_stars) = *reinterpret_cast<int8_t**>(std::uintptr_t{0x0056CE14});
    inline int8_t (*&_star_has_nonplayer)[73] = *reinterpret_cast<int8_t (**)[73]>(std::uintptr_t{0x0056CE20});
    inline int16_t& _n_battle_stars = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056CE2C});
    inline uint32_t*& _attack_confirmed = *reinterpret_cast<uint32_t**>(std::uintptr_t{0x0056CE64});
    inline s_combfind_star_has_player (*&_star_has_player)[73][8] = *reinterpret_cast<s_combfind_star_has_player (**)[73][8]>(std::uintptr_t{0x0056CE68});
    inline uint32_t& _current_star_has_nonplayer = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A4AAC});
    inline uint32_t& _current_star_has_player = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A4AB0});

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

    inline int16_t __cdecl Find_Planet_At_Star_Satisfying(s_star_data* star, int32_t (__cdecl* predicate)(s_planet_data*)) {
        using Fn = int16_t(__cdecl*)(s_star_data*, int32_t (__cdecl*)(s_planet_data*));
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bce80});
        return fn(star, predicate);
    }

    inline void __cdecl Update_Player_Needs_To_Attack_(int star_idx, int attacker_idx, int defender_idx) {
        using Fn = void(__cdecl*)(int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bd580});
        fn(star_idx, attacker_idx, defender_idx);
    }

    inline void __cdecl Get_Local_Human_Target_(int star_idx, int player_idx, int16_t* defender_idx, int16_t* colony_idx) {
        using Fn = void(__cdecl*)(int, int, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bd640});
        fn(star_idx, player_idx, defender_idx, colony_idx);
    }

    inline void __cdecl Get_AI_Target_(int star_idx, int player_idx, int16_t* defender_idx, int16_t* colony_idx, int8_t* unk_flag) {
        using Fn = void(__cdecl*)(int, int, int16_t*, int16_t*, int8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bd8d0});
        fn(star_idx, player_idx, defender_idx, colony_idx, unk_flag);
    }

    inline void __cdecl Get_NPC_Target_(int star_idx, int player_idx, int16_t* defender_idx, int16_t* colony_idx) {
        using Fn = void(__cdecl*)(int, int, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004be1e0});
        fn(star_idx, player_idx, defender_idx, colony_idx);
    }

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

    inline char* __cdecl Load_Combfind_Strings_(char* data) {
        using Fn = char*(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BC0D0});
        return fn(data);
    }

    s_colony* __cdecl Free_Colony_Slot_();

    bool __cdecl Planet_Has_Splinter_Colony_(s_planet_data* planet);
}