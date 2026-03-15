#pragma once
#include <cstdint>

namespace CMBTFIRE {
    inline uint16_t __cdecl Apply_Damage_To_Planet_(int16_t* dmg_out, uint16_t specials, uint16_t weapon_flags) {
        using Fn = uint16_t(__cdecl*)(int16_t*, uint16_t, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ae4d0});
        return fn(dmg_out, specials, weapon_flags);
    }

    int16_t __cdecl Is_Combat_Ship_Dead_(int16_t ship_idx);

    void __cdecl Destroy_Ship_(int16_t ship_idx, int16_t x, int16_t y, int16_t is_dead);

    inline void __cdecl Fire_Ship_(int16_t ship_idx, int16_t target_x, int16_t target_y, int16_t missile_idx, int16_t missile_x, int16_t missile_y, int16_t flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ACA30});
        fn(ship_idx, target_x, target_y, missile_idx, missile_x, missile_y, flag);
    }

    void __cdecl Remove_Stasis_Beam_(int16_t ship_idx);

    void __cdecl Retarget_Missiles_Aimed_At_(int16_t unit_idx);

    int16_t __cdecl Does_Combat_Planet_Have_Defenses_();

    int16_t __cdecl Target_Is_Legal_For_Weapon_(int16_t weapon_id, uint16_t specials, int16_t target_idx, int16_t target_type);

    int16_t __cdecl Facing_Shield_(int16_t angle);

    uint16_t __cdecl Apply_Ship_Damage_(
        int16_t target_ship_idx,
        int16_t* dmg_out,
        int16_t* sys_dmg,
        int16_t param_4,
        uint16_t specials,
        uint16_t weapon_flags,
        int16_t param_7,
        int16_t param_8,
        int16_t param_9
    );

    void __cdecl Fire_Fighter_Beam_(int16_t missile_idx, int16_t param_2, int16_t param_3, int16_t* total_damage, int16_t* total_struct_damage);

    void __cdecl Fire_Fighter_Bomb_(int16_t missile_idx, int16_t damage_modifier, int16_t* total_damage, int16_t* total_internal_damage);

    bool __cdecl Special_Is_On_(int16_t ship_idx, int16_t weapon_idx);

    int8_t __cdecl Special_Is_Turned_On_(int16_t ship_idx, int16_t special_idx);

    int16_t __cdecl Weapon_Can_Fire_At_Missiles_(int16_t weapon_idx, uint16_t flags);

    int16_t __cdecl Weapon_In_Range_(int16_t weapon_slot_idx, int16_t ship_idx, int16_t target_idx, int16_t target_type, int16_t range);

    void __cdecl Apply_Damage_To_Missile_(int16_t missile_idx, int16_t* damage_io, uint16_t specials, uint16_t weapon_flags);
}