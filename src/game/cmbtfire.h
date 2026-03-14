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

    inline void __cdecl Apply_Ship_Damage_(int16_t ship_idx, int16_t* damage_in_out, int16_t* struct_damage_out, int16_t shield_arc_idx, int8_t armor_piercing, int16_t* special_damage_out, int8_t enveloping, int8_t shield_piercing, int8_t continuous) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*, int16_t, int8_t, int16_t*, int8_t, int8_t, int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AD980});
        fn(ship_idx, damage_in_out, struct_damage_out, shield_arc_idx, armor_piercing, special_damage_out, enveloping, shield_piercing, continuous);
    }
}