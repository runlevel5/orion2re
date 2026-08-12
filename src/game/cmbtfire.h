#pragma once
#include <cstdint>

#define COMBAT_PLANET_DEFENSE_MISSILE_BASE 0x1
#define COMBAT_PLANET_DEFENSE_GROUND_BATTERIES 0x2
#define COMBAT_PLANET_DEFENSE_STELLAR_CONVERTER 0x4
#define COMBAT_PLANET_DEFENSE_FIGHTER_GARRISON 0x8

namespace CMBTFIRE {
    enum e_high_energy_focus_presence : uint8_t {
        HIGH_ENERGY_FOCUS_ABSENT = 0,
        HIGH_ENERGY_FOCUS_PRESENT = 1,
        HIGH_ENERGY_FOCUS_PRESENCE_COUNT = 2,
    };

    struct s_mount_damage {
        uint16_t pd;
        uint16_t nomod;
        uint16_t heavy;
    };

    extern s_mount_damage _mount_dmg[HIGH_ENERGY_FOCUS_PRESENCE_COUNT];

    inline uint16_t _civilian_armor[ARMOR_COUNT] = {
        100, 100, 100, 100, 100, 100, 100,
    };
    inline uint8_t _fighters_use_point_defense = 0;
    inline uint8_t _heavy_fighter_can_scramble = 0;
    inline uint8_t _planet_non_enveloping = 0;

    /** Returns whether Enveloping damage should use all four directions for this target. */
    bool Enveloping_Applies_To_Target_(int16_t target_ship_idx);

    /** Applies the supplied weapon damage to the defending planet or colony. */
    int16_t __cdecl Apply_Damage_To_Planet_(int16_t* damage, uint16_t specials, uint16_t weapon_flags);

    /** Destroys up to the requested number of colony defense structures at the current combat planet. */
    void __cdecl Destroy_Colony_Defense_(int16_t n_to_destroy);

    /** Returns the current combat death state for the specified ship. */
    int16_t __cdecl Is_Combat_Ship_Dead_(int16_t ship_idx);

    /** Finalizes destruction or disabling for a combat ship and triggers any matching effects. */
    void __cdecl Destroy_Ship_(int16_t ship_idx, int16_t draw_x, int16_t draw_y, int16_t death_state);

    /** Fires all legal ready weapons from the source ship at the selected target. */
    int16_t __cdecl Fire_Ship_(int16_t source_ship_idx, int16_t sx, int16_t sy, int16_t target_ship_idx, int16_t tx, int16_t ty, int16_t flag);

    /** Triggers pre-fire weapon specials that activate before the main attack resolution. */
    void __cdecl Pre_Weapon_Specials_(int16_t unit_idx, int16_t target_idx, int16_t target_type);

    /** Converts reflected damage into an Energy Absorber counterattack against the target ship. */
    void __cdecl Energy_Absorber_Attack_(int16_t unit_idx, int16_t target_idx);

    /** Resolves a beam weapon shot against a ship or missile target. */
    int16_t __cdecl Fire_Beam_Weapon_(int16_t source, int16_t target, int16_t weapon_id, uint16_t specials, uint16_t weapon_flags, int16_t* hit_x, int16_t* hit_y, int16_t* damage);

    /** Computes beam hit and damage modifiers for the given range and special flags. */
    void __cdecl Get_Beam_Weapon_Modifiers_(int16_t ship_idx, int16_t range, uint16_t wpn_modifier_flags, int16_t wpn_idx, int16_t *hit_bonus_out, int16_t *dmg_mod_out);

    /** Resolves one ship weapon slot as a bombing attack against the defending planet. */
    int16_t __cdecl Fire_Bomb_(int16_t ship_idx, int16_t unused_target_idx, int16_t weapon_slot_idx);

    /** Resolves a special-weapon activation against the selected ship or missile target. */
    void __cdecl Fire_Special_Weapon_(int16_t ship_idx, int16_t x, int16_t y, int16_t target_idx, int16_t weapon_slot_idx, int16_t target_type);

    /** Applies the Black Hole Generator effect to the target ship. */
    void __cdecl BHG_(int16_t source_idx, int16_t target_idx);

    /** Clears the stasis state and weapon availability effects from a ship. */
    void __cdecl Remove_Stasis_Beam_(int16_t ship_idx);

    /** Retargets or destroys missiles that were aiming at the specified unit. */
    void __cdecl Retarget_Missiles_Aimed_At_(int16_t unit_idx);

    /** Places the target ship into stasis and records the source ship. */
    void __cdecl Place_Ship_In_Stasis_(int16_t ship_idx, int16_t source_idx);

    /** Returns a bitfield describing which planetary defenses are currently present. */
    int16_t __cdecl Does_Combat_Planet_Have_Defenses_();

    /** Returns whether the specified weapon may legally attack the selected target. */
    int16_t __cdecl Target_Is_Legal_For_Weapon_(int16_t weapon_id, uint16_t specials, int16_t target_idx, int16_t target_type);

    /** Maps a relative bearing angle to the shield-facing index used for damage application. */
    int16_t __cdecl Facing_Shield_(int16_t angle);

    /** Returns the beam range penalty or bonus for the specified ship and range. */
    int16_t __cdecl Get_Beam_Range_To_Hit_Bonus_(int16_t ship_idx, int16_t range);

    /** Applies weapon damage to a combat ship, including shields, armor bypass, and internals. */
    uint16_t __cdecl Apply_Ship_Damage_(
        int16_t target_ship_idx,
        int16_t* dmg_out,
        int16_t* sys_dmg,
        int16_t shield_arc,
        uint16_t specials,
        uint16_t weapon_flags,
        int16_t double_internal_damage,
        int16_t force_armor_bypass,
        int16_t internal_only
    );

    /** Resolves fighter-beam attacks launched by the specified missile stack. */
    void __cdecl Fire_Fighter_Beam_(int16_t missile_idx, int16_t shield_arc, int16_t target_is_missile, int16_t* total_damage, int16_t* total_struct_damage);

    /** Resolves fighter-bomb attacks launched by the specified missile stack. */
    void __cdecl Fire_Fighter_Bomb_(int16_t missile_idx, int16_t damage_modifier, int16_t* total_damage, int16_t* total_internal_damage);

    /** Returns whether the indexed combat special is currently active on the ship. */
    bool __cdecl Special_Is_On_(int16_t ship_idx, int16_t weapon_idx);

    /** Returns whether the requested special-device bit corresponds to an enabled active special. */
    int8_t __cdecl Special_Is_Turned_On_(int16_t ship_idx, int16_t special_idx);

    /** Returns whether the specified weapon class may target missiles. */
    int16_t __cdecl Weapon_Can_Fire_At_Missiles_(int16_t weapon_idx, uint16_t flags);

    /** Returns whether the selected weapon slot can currently reach the target. */
    int16_t __cdecl Weapon_In_Range_(int16_t weapon_slot_idx, int16_t ship_idx, int16_t target_idx, int16_t target_type, int16_t range);

    /** Applies damage to the leading missile in a missile stack and reports destroyed missiles. */
    void __cdecl Apply_Damage_To_Missile_(int16_t missile_idx, int16_t* damage_io, uint16_t specials, uint16_t weapon_flags);

    /** Draws the Energy Absorber combat effect for the specified ship. */
    void __cdecl Draw_Energy_Absorber_(int16_t ship_idx, int16_t x, int16_t y);

    /** Draws the Black Hole Generator overlay for the specified ship. */
    void __cdecl Draw_BHG_(int16_t ship_idx, int16_t x_adj, int16_t y_adj);
}