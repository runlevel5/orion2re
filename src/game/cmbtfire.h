#pragma once
#include <cstdint>

namespace CMBTFIRE {
    int16_t __cdecl Apply_Damage_To_Planet_(int16_t* damage, uint16_t specials, uint16_t weapon_flags);

    void __cdecl Destroy_Colony_Defense_(int16_t n_to_destroy);

    int16_t __cdecl Is_Combat_Ship_Dead_(int16_t ship_idx);

    void __cdecl Destroy_Ship_(int16_t ship_idx, int16_t x, int16_t y, int16_t is_dead);

    int16_t __cdecl Fire_Ship_(int16_t source_ship_idx, int16_t sx, int16_t sy, int16_t target_ship_idx, int16_t tx, int16_t ty, int16_t flag);

    void __cdecl Pre_Weapon_Specials_(int16_t unit_idx, int16_t target_idx, int16_t is_coord);

    void __cdecl Energy_Absorber_Attack_(int16_t unit_idx, int16_t target_idx);

    int16_t __cdecl Fire_Beam_Weapon_(int16_t source, int16_t target, int16_t weapon_id, uint16_t specials, uint16_t weapon_flags, int16_t* hit_x, int16_t* hit_y, int16_t* damage);

    void __cdecl Get_Beam_Weapon_Modifiers_(int16_t ship_idx, int16_t range, uint16_t wpn_modifier_flags, int16_t wpn_idx, int16_t *hit_bonus_out, int16_t *dmg_mod_out);

    int16_t __cdecl Fire_Bomb_(int16_t ship_idx, int16_t param_2, int16_t weapon_slot_idx);

    void __cdecl Fire_Special_Weapon_(int16_t ship_idx, int16_t x, int16_t y, uint16_t target_idx, int16_t weapon_slot_idx, int16_t target_type);

    void __cdecl BHG_(int16_t source_idx, int16_t target_idx);

    void __cdecl Remove_Stasis_Beam_(int16_t ship_idx);

    void __cdecl Retarget_Missiles_Aimed_At_(int16_t unit_idx);

    void __cdecl Place_Ship_In_Stasis_(int16_t ship_idx, uint8_t source_idx);

    int16_t __cdecl Does_Combat_Planet_Have_Defenses_();

    int16_t __cdecl Target_Is_Legal_For_Weapon_(int16_t weapon_id, uint16_t specials, int16_t target_idx, int16_t target_type);

    int16_t __cdecl Facing_Shield_(int16_t angle);

    int16_t __cdecl Get_Beam_Range_To_Hit_Bonus_(int16_t ship_idx, int16_t range);

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

    void __cdecl Draw_Energy_Absorber_(int16_t ship_idx, int16_t x, int16_t y);

    void __cdecl Draw_BHG_(int16_t ship_idx, int16_t x_adj, int16_t y_adj);
}