#pragma once
#include <cstdint>

namespace CMBTFIRE {
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

    inline uint16_t __cdecl Apply_Ship_Damage_(short target_ship_idx, short* dmg_out, short* sys_dmg, short param_4, ushort specials, ushort weapon_flags, short param_7, short param_8, short param_9) {
        using Fn = uint16_t(__cdecl*)(short, short*, short*, short, ushort, ushort, short, short, short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AD980});
        return fn(target_ship_idx, dmg_out, sys_dmg, param_4, specials, weapon_flags, param_7, param_8, param_9);
    }

    void __cdecl Fire_Fighter_Beam_(int16_t missile_idx, int16_t param_2, int16_t param_3, int16_t* total_damage, int16_t* total_struct_damage);

    inline void __cdecl Fire_Fighter_Bomb_(int16_t missile_idx, int16_t shield_idx, int16_t* p_dmg, int16_t* p_sys_dmg) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004aee50})(missile_idx, shield_idx, p_dmg, p_sys_dmg);
    }

    bool __cdecl Special_Is_On_(int16_t ship_idx, int16_t weapon_idx);

    int8_t __cdecl Special_Is_Turned_On_(int16_t ship_idx, int16_t special_idx);

    int16_t __cdecl Weapon_Can_Fire_At_Missiles_(int16_t weapon_idx, uint16_t flags);

    int16_t __cdecl Weapon_In_Range_(int16_t weapon_slot_idx, int16_t ship_idx, int16_t target_idx, int16_t target_type, int16_t range);

    inline void __cdecl Apply_Damage_To_Missile_(int16_t target_idx, int16_t* damage_done, uint16_t bStack_c, uint16_t weapon_flags) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, uint16_t, uint16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004ae140})(target_idx, damage_done, bStack_c, weapon_flags);
    }
}