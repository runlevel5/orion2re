#pragma once
#include <cstdint>

namespace CMBTFIRE {
    inline int16_t __cdecl Is_Combat_Ship_Dead_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AC800});
        return fn(ship_idx);
    }

    inline void __cdecl Destroy_Ship_(int16_t ship_idx, int16_t x, int16_t y, int16_t is_dead) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ADE00});
        fn(ship_idx, x, y, is_dead);
    }

    inline void __cdecl Fire_Ship_(int16_t source_ship, int16_t source_x, int16_t source_y, int16_t target_ship, int16_t target_x, int16_t target_y, int16_t weapon_flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ACA30});
        fn(source_ship, source_x, source_y, target_ship, target_x, target_y, weapon_flag);
    }

    inline void __cdecl Remove_Stasis_Beam_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AF2A0});
        fn(ship_idx);
    }

    int16_t __cdecl Does_Combat_Planet_Have_Defenses_();
}