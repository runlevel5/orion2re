#pragma once
#include <cstdint>

namespace CMBTAI {
    inline int8_t* _player_mode = reinterpret_cast<int8_t*>(std::uintptr_t{0x0059D650});

    inline int8_t __cdecl Get_Player_Mode_(int16_t player_idx) {
        using Fn = int8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00450360});
        return fn(player_idx);
    }

    inline void __cdecl Toggle_Auto_Combat_Flags_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044D360});
        fn();
    }

    inline void __cdecl Do_Auto_Ship_Turn_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044D610});
        fn();
    }

    inline int __cdecl Range_To_Ship_(int16_t attacker_idx, int16_t defender_idx) {
        using Fn = int(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044C390});
        return fn(attacker_idx, defender_idx);
    }

    inline int16_t __cdecl Range_To_XY_(int16_t ship_idx, int16_t x, int16_t y) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044C260});
        return fn(ship_idx, x, y);
    }

    inline int16_t __cdecl Range_To_XY_(int16_t ship_idx, int16_t missile_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044c360});
        return fn(ship_idx, missile_idx);
    }

    inline int16_t __cdecl Will_Missile_Do_Damage_(int16_t ship_idx, int16_t missile_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044ed00});
        return fn(ship_idx, missile_idx);
    }

    inline bool __cdecl Safe_To_Fire_Sphere_Weapon_(int16_t ship_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044BA50});
        return fn(ship_idx);
    }

    inline int32_t __cdecl Expected_Beam_Bomb_Damage_(int16_t ship_idx, int16_t slot_idx, int16_t target_idx, int16_t shots, int16_t range_tactic, int16_t is_missile) {
        using Fn = int32_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0044CD80})(ship_idx, slot_idx, target_idx, shots, range_tactic, is_missile);
    }

    inline int16_t __cdecl Expected_Missile_Damage_(int16_t target_idx, uint16_t weapon_id, int16_t shots, uint16_t specials, int16_t owner) {
        using Fn = int16_t(__cdecl*)(int16_t, uint16_t, int16_t, uint16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0044CB10})(target_idx, weapon_id, shots, specials, owner);
    }

    int16_t __cdecl Expected_Weapon_Damage_(int16_t source_idx, int16_t weapon_slot, int16_t target_idx, int16_t target_type, int16_t range);
}