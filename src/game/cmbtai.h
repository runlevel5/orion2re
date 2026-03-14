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
}