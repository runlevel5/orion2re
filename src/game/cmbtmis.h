#pragma once
#include <cstdint>

namespace CMBTMIS {
    enum e_lightning_field_mode : uint8_t {
        LIGHTNING_FIELD_MODE_CLASSIC,
        LIGHTNING_FIELD_MODE_UNIFORM,
    };

    inline uint8_t _lightning_field_ratio = 50;
    inline e_lightning_field_mode _lightning_field_mode = LIGHTNING_FIELD_MODE_CLASSIC;

    struct s_missile_defense {
        uint8_t cloaking_device_missile_defense;
        uint8_t cloaking_device_missile_miss_percent;
        uint8_t displacement_device_missiles_miss_percent;
    };

    inline s_missile_defense missile_defense = {
        0,
        50,
        30,
    };

    inline bool Missile_Defense_Roll_Succeeds_(uint32_t roll, uint8_t chance) {
        return roll <= chance;
    }

    inline int32_t Expected_Damage_After_Cloak_(int32_t damage, bool actively_cloaked) {
        if (!actively_cloaked || missile_defense.cloaking_device_missile_defense != 0) {
            return damage;
        }

        uint8_t miss_percent = missile_defense.cloaking_device_missile_miss_percent;
        if (miss_percent >= 100) {
            return 0;
        }

        return damage * (100 - miss_percent) / 100;
    }

    inline int16_t Final_Flyer_Target_(const s_missile* missiles, int16_t missile_idx) {
        const s_missile* missile = &missiles[missile_idx];
        while (missile->is_anti_missile_rocket != 0) {
            missile = &missiles[missile->target_unit_idx];
        }
        return missile->target_unit_idx;
    }

    inline bool Missile_Targets_Ship_(const s_missile* missile, int16_t ship_idx) {
        return missile->type > WEAPON_NO_WEAPONS &&
               missile->is_anti_missile_rocket == 0 &&
               missile->target_unit_idx == ship_idx;
    }

    extern int16_t _missile_group_offset[16][16];
    extern int16_t _torpedo_group_offset[16][8];

    /** Returns the display group used for a missile entry's animation. */
    int16_t __cdecl Missile_Type_(int16_t missile_idx);

    /** Returns whether a combat-grid coordinate lies inside the current viewport. */
    int8_t __cdecl On_Combat_Screen_(int16_t x, int16_t y);

    /** Computes a step-sized movement delta from one missile-space position toward another. */
    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t dist, int16_t* dx, int16_t* dy);

    /** Returns the owning player id for a missile entry. */
    int16_t __cdecl Missile_Owner_(int16_t missile_idx);

    /** Retargets a missile or fighter group when its current target is no longer valid. */
    void __cdecl Retarget_Missile_(int16_t missile_idx);

    /** Advances a missile toward a target point and reports whether it reached hit range. */
    char __cdecl Move_Missile_(int16_t missile_idx, int16_t target_x, int16_t target_y, int16_t move_dist);

    /** Returns whether a missile is within hit range of a target point. */
    int8_t __cdecl Did_Missile_Hit_(int16_t missile_idx, int16_t x, int16_t y);

    /** Computes the facing from a missile toward a target point. */
    int16_t __cdecl Get_Facing_(int16_t missile_idx, int16_t target_x, int16_t target_y);

    /** Returns the movement speed for a missile or fighter weapon type. */
    int16_t __cdecl Missile_Speed_(int16_t type, uint16_t specials, int16_t owner);

    /** Returns the hit strength associated with a missile or fighter weapon id. */
    int16_t __cdecl Get_Missile_Hits_(int16_t missile_id, int16_t ship_idx, uint16_t flags);

    /** Returns whether a missile currently lies inside the visible combat viewport. */
    int8_t __cdecl Missile_On_Combat_Screen_(int16_t missile_idx);

    /** Distributes returning fighter-bay units among surviving weapon slots. */
    void __cdecl Land_Returning_Fighters_(
        s_combat_data* source, int16_t type, int16_t returned_count);

    /** Resolves a missile impact or fighter attack sequence at the target point. */
    int16_t __cdecl Resolve_Missile_(int16_t missile_idx, int16_t x, int16_t y);

    /** Returns whether the current target for a missile is still valid and alive. */
    int16_t __cdecl Is_Missile_Target_Alive_(int16_t missile_idx);

    /** Returns the shield arc index with the lowest current strength. */
    int16_t __cdecl Find_Weakest_Shield_(int16_t ship_idx);

    /** Plays fighter attack visuals and damage messaging for a resolved strike. */
    void __cdecl Fighter_Combat_SFX_(int16_t missile_idx, int16_t x, int16_t y, int16_t dmg, int16_t sys_dmg);

    /** Expands a missile group into per-sprite screen offsets. */
    void __cdecl Get_Multiple_Missile_Coords_(int16_t missile_idx, int16_t quantity, int16_t* out_x, int16_t* out_y);

    /** Expands a torpedo group into per-sprite screen offsets. */
    void __cdecl Get_Multiple_Torpedo_Coords_(int16_t missile_idx, int16_t group_size, int16_t* out_x, int16_t* out_y);

    /** Draws the fighter sprites for a fighter-group missile entry. */
    void __cdecl Draw_Fighter_(int16_t missile_idx, int16_t base_x, int16_t base_y, int16_t combat_view_type);

    /** Clears a missile entry and retargets anti-missile rockets that were tracking it. */
    void __cdecl Kill_Missile_(int16_t missile_idx);

    /** Returns how many sprites should be drawn for a missile entry. */
    int16_t __cdecl Picture_Count_(int16_t missile_idx);

    /** Draws a missile or fighter group at the supplied screen position. */
    void __cdecl Draw_Missile_(int16_t missile_idx, int16_t screen_x, int16_t screen_y, int16_t draw_fullscreen_view);

    /** Spawns and launches a missile or fighter group from a ship weapon slot. */
    void __cdecl Fire_Missile_(int16_t source_idx, int16_t target_idx, int16_t weapon_slot, int16_t is_anti_rocket);

    /** Draws the status panel for a selected missile or fighter group. */
    void __cdecl Draw_Missile_Status_(int16_t x_off, int16_t y_off, int16_t missile_idx);

    /** Draws all active missiles in back-to-front order for the current combat view. */
    void __cdecl Draw_Missiles_();

    /** Returns the offensive combat value used for fighter attacks. */
    int16_t __cdecl Fighter_Ocv_(int16_t player_idx);

    /** Returns the defensive combat value for a missile or fighter weapon type. */
    int16_t __cdecl Missile_Dcv_(int16_t weapon_idx, int16_t player_idx);

    /** Finds the next free slot in the combat missile table. */
    void __cdecl Next_Missile_Id_(int16_t* missile_id);

    /** Computes the initial facing for a missile toward its current target. */
    int16_t __cdecl Missile_Facing_(int16_t missile_idx, int16_t target_idx, int16_t target_is_missile);
}