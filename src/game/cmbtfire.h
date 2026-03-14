#pragma once
#include <cstdint>

namespace CMBTFIRE {
    int16_t __cdecl Is_Combat_Ship_Dead_(int16_t ship_idx);

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

    inline void __cdecl Retarget_Missiles_Aimed_At_(uint16_t ship_idx) {
        using Fn = void(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ac7a0});
        fn(ship_idx);
    }

    int16_t __cdecl Does_Combat_Planet_Have_Defenses_();

    inline int16_t __cdecl Target_Is_Legal_For_Weapon_(int16_t weapon_type, int16_t source_unit_idx, int16_t target_unit_idx, int16_t unknown) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ac880});
        return fn(weapon_type, source_unit_idx, target_unit_idx, unknown);
    }

    inline int16_t __cdecl Facing_Shield_(int16_t bearing) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ADDD0});
        return fn(bearing);
    }

    inline void __cdecl Apply_Ship_Damage_(int16_t ship_idx, int16_t* damage_in_out, int16_t* struct_damage_out, int16_t shield_arc_idx, int8_t armor_piercing, int16_t* special_damage_out, int8_t enveloping, int8_t shield_piercing, int8_t continuous) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*, int16_t, int8_t, int16_t*, int8_t, int8_t, int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AD980});
        fn(ship_idx, damage_in_out, struct_damage_out, shield_arc_idx, armor_piercing, special_damage_out, enveloping, shield_piercing, continuous);
    }

    inline void __cdecl Fire_Fighter_Beam_(int16_t missile_idx, int16_t shield_idx, int16_t is_anti_missile_rocket, int16_t* p_dmg, int16_t* p_sys_dmg) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004aefd0})(missile_idx, shield_idx, is_anti_missile_rocket, p_dmg, p_sys_dmg);
    }

    inline void __cdecl Fire_Fighter_Bomb_(int16_t missile_idx, int16_t shield_idx, int16_t* p_dmg, int16_t* p_sys_dmg) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004aee50})(missile_idx, shield_idx, p_dmg, p_sys_dmg);
    }

    inline int8_t __cdecl Special_Is_Turned_On_(int16_t ship_idx, int16_t special_id) {
        using Fn = int8_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AF320});
        return fn(ship_idx, special_id);
    }
}