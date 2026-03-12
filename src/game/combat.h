#pragma once
#include <cstdint>

namespace COMBAT {
    int16_t __cdecl Strategic_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, char* summary, char* details);

    inline void __cdecl Get_Leader_Combat_Bonuses_(int16_t* ships, int16_t ship_count, int16_t* b1, int16_t* b2, int16_t* leader_bonuses, int16_t* b3, int16_t* b4) {
        using Fn = void(__cdecl*)(int16_t*, int16_t, int16_t*, int16_t*, int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00489ea0});
        fn(ships, ship_count, b1, b2, leader_bonuses, b3, b4);
    }

    inline void __cdecl Qload_Ships_(int16_t* ships, int16_t count, int16_t b1, int16_t b2, int16_t b3, int16_t b4) {
        using Fn = void(__cdecl*)(int16_t*, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048be60});
        fn(ships, count, b1, b2, b3, b4);
    }

    inline void __cdecl QLoad_Antaran_Defense_Fleet_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00489c40});
        fn();
    }

    inline void __cdecl _no_op3() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048d1e0});
        fn();
    }

    inline int16_t __cdecl QGet_Target_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048cc30});
        return fn(ship_idx);
    }

    inline int16_t __cdecl QGet_Target_SC_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048cd00});
        return fn(ship_idx);
    }

    inline void __cdecl Stellar_Converter_Attack_(int16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048cf80});
        fn(target_idx);
    }

    inline void __cdecl Beam_Attack_(int16_t ship_idx, int16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048d0b0});
        fn(ship_idx, target_idx);
    }

    inline void __cdecl Missile_Attack_(int16_t ship_idx, int16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048cd80});
        fn(ship_idx, target_idx);
    }

    inline void __cdecl Strat_Special_Attack_(int16_t ship_idx, int16_t target_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048cfb0});
        fn(ship_idx, target_idx);
    }

    inline void __cdecl Strat_Bomb_Attack_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048cee0});
        fn(ship_idx);
    }

    inline void __cdecl Resolve_Strat_Colony_Damage_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048d2a0});
        fn();
    }

    inline void __cdecl Build_End_Of_Combat_Message_(char* summary, char* details, int16_t star_idx, int16_t defense_flags, int16_t result) {
        using Fn = void(__cdecl*)(char*, char*, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048ad30});
        fn(summary, details, star_idx, defense_flags, result);
    }

    inline void __cdecl Message_Box_Titled_(char* summary, char* details) {
        using Fn = void(__cdecl*)(char*, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00489A70});
        fn(summary, details);
    }
}