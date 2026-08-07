#include "pch.h"

namespace {
    constexpr int kPlayerRemapPaletteStart = 144;
    constexpr int kTractorBeamRemapPaletteStart = 80;
    constexpr int kPlasmaWebRemapPaletteStart = 176;
    constexpr int kMaxCombatShips = 210;
    constexpr int kMaxCombatMissiles = 300;
    constexpr size_t kCombatDataBytes = sizeof(s_combat_data) * kMaxCombatShips;
    constexpr size_t kCombatMissileBytes = sizeof(s_missile) * kMaxCombatMissiles;
    COMBINIT::s_satellite_capture_state satellite_capture_state;
}

namespace COMBINIT {

const char s_cmbtmisl_lbx_00551234[13] = "cmbtmisl.lbx";
const char s_sphersfx_lbx_00551218[13] = "sphersfx.lbx";

uint8_t _real_attack_flag;

void __cdecl Reset_Satellite_Capture_() {
    Reset_Satellite_Capture_State_(&satellite_capture_state);
}

void __cdecl Record_Captured_Satellite_(int16_t colony_idx, int16_t building_idx) {
    Record_Captured_Satellite_State_(&satellite_capture_state, colony_idx, building_idx);
}

int16_t __cdecl Take_Captured_Satellite_(int16_t colony_idx) {
    return Take_Captured_Satellite_State_(&satellite_capture_state, colony_idx);
}

void __cdecl Load_Display_Combat_Ship_(int16_t ship_idx) {
    if (MOX::_settings.strategic_combat_flag == 1) {
        INITSHIP::Update_Strategic_Ship_(ship_idx);
    }

    strlcpy(MOX::_global_combat_data.name, MOX::_ship[ship_idx].d.name, sizeof(MOX::_global_combat_data.name));

    MOX::_global_combat_data.owner = MOX::_ship[ship_idx].owner;
    MOX::_global_combat_data.previous_owner = MOX::_ship[ship_idx].d.previous_owner;
    MOX::_global_combat_data.ship_idx = ship_idx;
    MOX::_global_combat_data.shield_type = MOX::_ship[ship_idx].d.shield_type;
    MOX::_global_combat_data.computer_type = MOX::_ship[ship_idx].d.computer_type;
    MOX::_global_combat_data.ftl_type = MOX::_ship[ship_idx].d.ftl_type;
    MOX::_global_combat_data.size_class = MOX::_ship[ship_idx].d.size;
    MOX::_global_combat_data.unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        MOX::_global_combat_data.special_device_flags[i] = MOX::_ship[ship_idx].d.special_device_flags[i];
        MOX::_global_combat_data.special_device_damage_flags[i] = MOX::_ship[ship_idx].special_device_damage_flags[i];
    }

    for (int i = 0; i < 8; ++i) {
        if (MOX::_ship[ship_idx].d.ship_type == SHIP_TYPE_COMBAT) {
            MOX::_global_combat_data.weapons[i].weapon_id = MOX::_ship[ship_idx].d.ship_weapon[i].type;
            MOX::_global_combat_data.weapons[i].count = MOX::_ship[ship_idx].d.ship_weapon[i].current_count;
            MOX::_global_combat_data.weapons[i].arc = MOX::_ship[ship_idx].d.ship_weapon[i].firing_arc;
            MOX::_global_combat_data.weapons[i].specials = MOX::_ship[ship_idx].d.ship_weapon[i].specials;
            MOX::_global_combat_data.weapons[i].ammo = MOX::_ship[ship_idx].d.ship_weapon[i].ammo;

            if (MOX::_global_combat_data.weapons[i].arc == WEAPON_FIRING_ARC_360) {
                MOX::_global_combat_data.weapons[i].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
            }

            MOX::_global_combat_data.weapons[i].active = 1;
            MOX::_global_combat_data.weapons[i].shots_left = MOX::_global_combat_data.weapons[i].count;

            int16_t weapon_id = MOX::_global_combat_data.weapons[i].weapon_id;

            if (TECHDATA::_weapons[weapon_id].type == WEAPON_TYPE_TORPEDO) {
                MOX::_global_combat_data.weapons[i].ammo = 0;
            } else if (TECHDATA::_weapons[weapon_id].type != WEAPON_TYPE_MISSILE) {
                MOX::_global_combat_data.weapons[i].ammo = TECHDATA::_weapons[weapon_id].ammo;
            }

            if (weapon_id == WEAPON_ANTI_MISSILE_ROCKET) {
                MOX::_global_combat_data.weapons[i].ammo *= MOX::_global_combat_data.weapons[i].count;
            }

            MOX::_global_combat_data.weapons[i].status = 0;
        } else {
            MOX::_global_combat_data.weapons[i].weapon_id = WEAPON_NO_WEAPONS;
        }
    }

    for (int i = 0; i < 8; ++i) {
        MOX::_global_combat_data.weapon_ready_flags[i] = 1;
    }

    MOX::_global_combat_data.crew_quality = MOX::_ship[ship_idx].crew_quality;
    MOX::_global_combat_data.officer_idx = MOX::_ship[ship_idx].officer_index;

    if (MOX::_ship[ship_idx].d.ship_type == SHIP_TYPE_COMBAT && MOX::_ship[ship_idx].owner <= MAX_PLAYERS) {
        MOX::_global_combat_data.marine_count = (int8_t)TECHDATA::_hull_data[MOX::_global_combat_data.size_class].marines;

        if (struct_::Test_Bit_Field_(MOX::_global_combat_data.special_device_flags, SPECIAL_TROOP_PODS)) {
            if (!struct_::Test_Bit_Field_(MOX::_global_combat_data.special_device_damage_flags, SPECIAL_TROOP_PODS)) {
                MOX::_global_combat_data.marine_count *= 2;
            }
        }
    } else {
        MOX::_global_combat_data.marine_count = -1;
    }

    MOX::_global_combat_data.structure_max = INITSHIP::Get_Ship_Structure_(ship_idx);
    MOX::_global_combat_data.armor_remaining = INITSHIP::Get_Ship_Armor_Hits_(ship_idx) - MOX::_ship[ship_idx].armor_damage;
    MOX::_global_combat_data.armor_type = MOX::_ship[ship_idx].d.armor_type;

    if (MOX::_global_combat_data.size_class == SHIP_SIZE_DOOM_STAR) {
        MOX::_global_combat_data.structure_damage = MOX::_ship[ship_idx].structural_damage;
        MOX::_global_combat_data.picture_num = 43; /* 0x2B */
    } else {
        MOX::_global_combat_data.structure_damage = MOX::_ship[ship_idx].structural_damage;
        MOX::_global_combat_data.picture_num = MOX::_ship[ship_idx].d.picture_num;
    }

    MOX::_global_combat_data.is_retreating = 0;

    CMBTFIR2::Init_Internal_Space_((s_combat_data*)&MOX::_global_combat_data);

    MOX::_global_combat_data.shield_current_hits =
        ((100 - MOX::_ship[ship_idx].shield_damage_percent) * MOX::_global_combat_data.shield_max_hits) / 100;

    MOX::_global_combat_data.drive_current_hits =
        ((100 - MOX::_ship[ship_idx].drive_damage_percent) * MOX::_global_combat_data.drive_max_hits) / 100;

    MOX::_global_combat_data.computer_current_hits =
        MOX::_global_combat_data.computer_max_hits - MOX::_ship[ship_idx].computer_damage;

    if (MOX::_global_combat_data.shield_type <= 0) {
        MOX::_global_combat_data.shield_arc_max = 0;
    } else {
        int16_t shield_hull_mult = MOX::_global_combat_data.size_class < 5
            ? MOX::_global_combat_data.size_class + 1
            : MOX::_global_combat_data.size_class + 2;

        MOX::_global_combat_data.shield_arc_max =
            TECHDATA::_shields[MOX::_global_combat_data.shield_type].shield_class * shield_hull_mult * 5;

        if (struct_::Test_Bit_Field_(MOX::_global_combat_data.special_device_flags, SPECIAL_MULTI_PHASED_SHIELDS)) {
            if (!struct_::Test_Bit_Field_(MOX::_global_combat_data.special_device_damage_flags, SPECIAL_MULTI_PHASED_SHIELDS)) {
                MOX::_global_combat_data.shield_arc_max += MOX::_global_combat_data.shield_arc_max / 2;
            }
        }

        MOX::_global_combat_data.shield_arc_max =
            (MOX::_global_combat_data.shield_arc_max * MOX::_global_combat_data.shield_current_hits + MOX::_global_combat_data.shield_max_hits - 1) / MOX::_global_combat_data.shield_max_hits;
    }

    for (int i = 0; i < 4; ++i) {
        MOX::_global_combat_data.shield_arc_current[i] = MOX::_global_combat_data.shield_arc_max;
    }

    MOX::_global_combat_data.current_speed = MOX::_ship[ship_idx].d.combat_speed;

    if (struct_::Test_Bit_Field_(MOX::_ship[ship_idx].d.special_device_flags, SPECIAL_AUGMENTED_ENGINES)) {
        if (!struct_::Test_Bit_Field_(MOX::_ship[ship_idx].special_device_damage_flags, SPECIAL_AUGMENTED_ENGINES)) {
            MOX::_global_combat_data.current_speed += 5;
        }
    }

    int32_t drive_speed_adjustment = (2 * MOX::_global_combat_data.drive_max_hits) / -3;
    MOX::_global_combat_data.current_speed =
        ((MOX::_global_combat_data.drive_current_hits + drive_speed_adjustment) * MOX::_global_combat_data.current_speed) /
        (drive_speed_adjustment + MOX::_global_combat_data.drive_max_hits);

    if (MOX::_global_combat_data.current_speed < 0) {
        MOX::_global_combat_data.current_speed = 0;
    }

    if (MOX::_global_combat_data.owner < MAX_PLAYERS) {
        if (MOX::_player[MOX::_global_combat_data.owner].traits[TRAIT_TRANS_DIMENSIONAL] != 0) {
            MOX::_global_combat_data.current_speed += 4;
        }
    }
}

int16_t __cdecl Ship_Size_Offset_(int16_t size) {
    switch (size) {
    case HULL_FRIGATE:
        return 1;
    case HULL_DESTROYER:
    case HULL_CRUISER:
        return 2;
    case HULL_BATTLESHIP:
    case HULL_TITAN:
    case HULL_DOOM_STAR:
        return 3;
    default:
        return 0;
    }
}

int8_t __cdecl Colony_Has_Defenses_(int16_t colony_idx) {
    s_colony* col = &MOX::_colony[colony_idx];

    if (col->buildings[BUILDING_STAR_BASE] != 0 ||
        col->buildings[BUILDING_FIGHTER_GARRISON] != 0 ||
        col->buildings[BUILDING_BATTLESTATION] != 0 ||
        col->buildings[BUILDING_STAR_FORTRESS] != 0 ||
        col->buildings[BUILDING_MISSILE_BASE] != 0 ||
        col->buildings[BUILDING_GROUND_BATTERIES] != 0 ||
        col->buildings[BUILDING_STELLAR_CONVERTER] != 0) {
        return 1;
    }

    return 0;
}

int __cdecl qsort_ship_array_by_size_(const void* p1, const void* p2) {
    const int16_t* ship_idx_a = (const int16_t*)p1;
    const int16_t* ship_idx_b = (const int16_t*)p2;

    int8_t size_b = MOX::_ship[*ship_idx_b].d.size;
    int8_t size_a = MOX::_ship[*ship_idx_a].d.size;

    if (size_b == size_a) {
        return 0;
    }

    if (size_b > size_a) {
        return 1;
    }

    return -1;
}

int __cdecl qsort_combat_data_by_size_(const void* a, const void* b) {
    const s_combat_data* data_a = (const s_combat_data*)a;
    const s_combat_data* data_b = (const s_combat_data*)b;

    if (data_b->size_class == data_a->size_class) {
        return 0;
    }

    if (data_b->size_class > data_a->size_class) {
        return 1;
    }

    return -1;
}

void __cdecl Load_Combat_Ship_(int16_t ship_idx, int16_t combat_idx) {
    s_ship_data* ship = &MOX::_ship[ship_idx];
    s_combat_data* combat = &MOX::_combat_data[combat_idx];

    strlcpy(combat->name, ship->d.name, sizeof(combat->name));
    combat->owner = ship->owner;
    combat->previous_owner = ship->d.previous_owner;
    combat->ship_idx = ship_idx;
    combat->shield_type = ship->d.shield_type;
    combat->computer_type = ship->d.computer_type;

    if (combat->owner < MAX_PLAYERS) {
        combat->ftl_type = ship->d.ftl_type;
    } else {
        combat->ftl_type = (int8_t)INITSHIP::Best_Warp_Drive_(combat->owner);
    }

    combat->combat_speed_base = ship->d.combat_speed;
    combat->size_class = ship->d.size;
    combat->unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        combat->special_device_flags[i] = ship->d.special_device_flags[i];
        combat->special_device_damage_flags[i] = ship->special_device_damage_flags[i];
    }

    for (int i = 0; i < 8; ++i) {
        combat->weapons[i].weapon_id = ship->d.ship_weapon[i].type;
        combat->weapons[i].count = ship->d.ship_weapon[i].current_count;
        combat->weapons[i].arc = ship->d.ship_weapon[i].firing_arc;
        combat->weapons[i].specials = ship->d.ship_weapon[i].specials;
        combat->weapons[i].ammo = ship->d.ship_weapon[i].ammo;

        if (combat->weapons[i].arc == WEAPON_FIRING_ARC_360) {
            combat->weapons[i].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
        }

        combat->weapons[i].active = 1;
        combat->weapons[i].shots_left = combat->weapons[i].count;

        uint16_t weapon_id = combat->weapons[i].weapon_id;
        if (TECHDATA::_weapons[weapon_id].type == WEAPON_TYPE_TORPEDO) {
            combat->weapons[i].ammo = 0;
        } else if (TECHDATA::_weapons[weapon_id].type != WEAPON_TYPE_MISSILE) {
            combat->weapons[i].ammo = TECHDATA::_weapons[weapon_id].ammo;
        }

        if (combat->weapons[i].weapon_id == WEAPON_ANTI_MISSILE_ROCKET) {
            combat->weapons[i].ammo *= combat->weapons[i].count;
        }

        combat->weapons[i].status = 0;
    }

    for (int i = 0; i < 8; ++i) {
        combat->weapon_ready_flags[i] = 1;
    }

    combat->crew_quality = ship->crew_quality;

    int16_t officer_idx = ship->officer_index;
    if (officer_idx < 0 || MOX::_leaders[officer_idx].eta != 0) {
        combat->officer_idx = -1;
    } else {
        combat->officer_idx = officer_idx;
    }

    if (ship->d.ship_type == SHIP_TYPE_COMBAT && ship->owner <= MAX_PLAYERS) {
        combat->marine_count = (uint8_t)TECHDATA::_hull_data[combat->size_class].marines;
        if (struct_::Test_Bit_Field_(combat->special_device_flags, SPECIAL_TROOP_PODS) == 1 &&
            struct_::Test_Bit_Field_(combat->special_device_damage_flags, SPECIAL_TROOP_PODS) == 0) {
            combat->marine_count *= 2;
        }
    } else {
        combat->marine_count = -1;
    }

    combat->structure_max = INITSHIP::Get_Ship_Structure_(ship_idx);
    combat->armor_remaining = INITSHIP::Get_Ship_Armor_Hits_(ship_idx) - ship->armor_damage;
    combat->armor_type = ship->d.armor_type;
    combat->structure_damage = ship->structural_damage;

    if (combat->structure_damage > combat->structure_max) {
        combat->structure_damage = combat->structure_max - 1;
    }

    if (combat->size_class == SHIP_SIZE_DOOM_STAR && combat->owner < MAX_PLAYERS) {
        combat->picture_num = 43 /* 0x2b */;
    } else {
        combat->picture_num = ship->d.picture_num;
    }

    combat->is_retreating = 0;
    CMBTFIR2::Init_Internal_Space_(combat);

    combat->shield_current_hits = (int16_t)((100 - ship->shield_damage_percent) * combat->shield_max_hits / 100);
    combat->drive_current_hits = (int16_t)((100 - ship->drive_damage_percent) * combat->drive_max_hits / 100);
    combat->computer_current_hits = combat->computer_max_hits - ship->computer_damage;

    combat->shield_arc_max = Calc_Combat_Shield_Max_(combat_idx);
    for (int i = 0; i < 4; ++i) {
        combat->shield_arc_current[i] = combat->shield_arc_max;
    }

    if (combat->owner > MAX_PLAYERS && combat->ftl_type == DRIVE_NO_DRIVE) {
        switch (combat->owner) {
            case 9:
            case 11:
                combat->ftl_type = DRIVE_ANTI_MATTER_DRIVE;
                break;
            case 10:
            case 14:
                combat->ftl_type = DRIVE_FUSION_DRIVE;
                break;
            case 12:
            case 13:
                combat->ftl_type = DRIVE_INTERPHASED_DRIVE;
                break;
            default:
                break;
        }
    }

    Init_Special_Devices_(combat_idx);
}

void __cdecl Init_Special_Devices_(int16_t combat_ship_idx) {
    s_combat_data* ship = &MOX::_combat_data[combat_ship_idx];

    ship->stasis_source_idx = 0xFF;
    ship->black_hole_source_idx = UINT8_MAX;
    ship->reflected_damage_pool = 0;
    ship->black_hole_flag = 0;
    ship->combat_status_flags = 0;
    ship->field42_0x49 = 0;
    ship->plasma_web_damage = 0;
    ship->special_cooldown = 0xFF;
    ship->special_status_timer = 0;

    if (COMBINIT::Does_Combat_Ship_Have_Special_(combat_ship_idx, SPECIAL_CLOAKING_DEVICE)) {
        ship->special_status_flag = 1;
    } else if (COMBINIT::Does_Combat_Ship_Have_Special_(combat_ship_idx, SPECIAL_PHASING_CLOAK)) {
        ship->special_status_flag = 4;
        ship->special_status_timer = 10;
    } else {
        ship->special_status_flag = 0;
    }

    if (COMBINIT::Does_Combat_Ship_Have_Special_(combat_ship_idx, SPECIAL_HYPER_X_CAPACITORS)) {
        for (int i = 0; i < 8; ++i) {
            if (TECHDATA::_weapons[ship->weapons[i].weapon_id].type == WEAPON_TYPE_BEAM) {
                ship->weapons[i].status = 1;
            }
        }
    }

    if (COMBINIT::Does_Combat_Ship_Have_Special_(combat_ship_idx, SPECIAL_FAST_MISSILE_RACKS)) {
        for (int i = 0; i < 8; ++i) {
            if (TECHDATA::_weapons[ship->weapons[i].weapon_id].type == WEAPON_TYPE_MISSILE) {
                ship->weapons[i].status = 1;
            }
        }
    }
}

int16_t __cdecl Type_Of_Space_Station_(int16_t colony_idx) {
    if (MOX::_colony[colony_idx].buildings[BUILDING_STAR_BASE] != 0) {
        return HULL_STAR_BASE;
    }
    if (MOX::_colony[colony_idx].buildings[BUILDING_BATTLESTATION] != 0) {
        return HULL_BATTLESTATION;
    }
    if (MOX::_colony[colony_idx].buildings[BUILDING_STAR_FORTRESS] != 0) {
        return HULL_STAR_FORTRESS;
    }
    return -1;
}

int16_t __cdecl Calc_Combat_Shield_Max_(int16_t ship_idx) {
    int16_t max_shield = 0;
    s_combat_data* ship = &MOX::_combat_data[ship_idx];
    int8_t shield_type = ship->shield_type;

    if (shield_type > 0) {
        int16_t multiplier;
        int8_t size_class = ship->size_class;

        if (size_class < 5) {
            multiplier = (int16_t)size_class + 1;
        } else {
            multiplier = (int16_t)size_class + 2;
        }

        // Calculation uses base shield blocking points multiplied by ship size factor and global multiplier 5
        max_shield = TECHDATA::_shields[shield_type].shield_class * multiplier * 5;

        // Shields are suppressed in a nebula unless the ship has specific warp/shield stabilization (Special 13)
        if (MOX::_in_nebula_flag != 0) {
            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_HARD_SHIELDS) == 0) {
                max_shield = 0;
            }
        }

        // Shield multipliers or hardening (Special 20) increase capacity by 50%
        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_MULTI_PHASED_SHIELDS) != 0) {
            max_shield = (max_shield * 3) / 2;
        }
    }

    return max_shield;
}

int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t ship_idx, int16_t special_id) {
    int16_t result = 0;

    if (struct_::Test_Bit_Field_(MOX::_combat_data[ship_idx].special_device_flags, special_id) == 1) {
        if (struct_::Test_Bit_Field_(MOX::_combat_data[ship_idx].special_device_damage_flags, special_id) == 0) {
            result = 1;
        }
    }

    return result;
}

int16_t __cdecl Tactical_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, int16_t retreat_flag) {
    int8_t abort_tactical = 0;
    bool no_defender_ships = star_idx != MOX::_NUM_STARS;
    int16_t has_tractor_beam = 0;
    int16_t has_plasma_web = 0;
    int16_t has_special_26 = 0;
    int16_t has_bhg = 0;
    int16_t has_stellar_converter = 0;
    int16_t colony_pop = 0;
    s_mouse_list local_mouse_list = {0, 0, 0, 0, 0x27F, 0x1DF};
    MOX2::action_func_t auto_func;
    void* glass_remap_block;
    char msg_buf[80];

    int16_t combat_winner = 0;

    if (star_idx == MOX::_NUM_STARS) {
        ERIC::Play_Dimensional_Portal_Flic_();
    }

    MOX::_ai_retreat_flag = (int16_t)retreat_flag;
    MOX::_combat_turn = 0;
    MOX::_combat_clock_frame = 15;
    MOX::_bio_weapon_flag = 0;
    MOX::_last_ship_status = -2;
    MOX::_cur_combat_x = 0;
    MOX::_cur_combat_y = 0;
    MOX::_ship_status_display = 0;
    MOX::_dont_draw_ship = -1;
    MOX::_draw_active_missiles = 1;
    MOX::_order_info_flag = 1;
    MOX::_super_fast_flag = 0;
    COMBINIT::_real_attack_flag = 0;

    MOX::_shields_flag = (uint8_t)MOX::_settings.combat_shields_arc_flag;
    MOX::_speedx2_flag = (uint8_t)MOX::_settings.combat_speedx2_flag;
    MOX::_saved_msl_warning = (uint8_t)MOX::_settings.combat_missile_impact_flag;
    MOX::_msl_impact_warning_flag = (uint8_t)MOX::_settings.combat_missile_impact_flag;
    MOX::_draw_legal_boxes_flag = (uint8_t)MOX::_settings.combat_legal_moves_flag;
    MOX::_grid_flag = (uint8_t)MOX::_settings.combat_grid_flag;
    MOX::_combat_init = MOX::_settings.ship_initiative;

    MOX::_take_control_flag = 0;
    MOX::_apply_damage_to_planet = 1;
    MOX::_missile_warning = 0;
    MOX::_ship_frame = 0;
    MOX::_combat_far_x = 0;
    MOX::_combat_mid_x = 0;
    MOX::_combat_near_x = 0;
    MOX::_combat_far_y = 0;
    MOX::_combat_mid_y = 0;
    MOX::_combat_near_y = 0;
    MOX::_net_flag = 0;
    MOX::_combat_total_ship_count = 1;
    MOX::_cmbt_colony_id = colony_idx;

    MOX::_combat_data = (s_combat_data*)allocate::Allocate_First_Block_(MOX::_screen_seg, kCombatDataBytes);
    // Resetting the screen pool invalidates resources from the previous combat.
    MOX::_missile = nullptr;
    MOX::_darken_plasma_map = nullptr;
    struct_::Clear_Structure_(MOX::_combat_data, kCombatDataBytes);

    MOX::_combat_attacker_player = MOX::_ship[plr1_ships[0]].owner;

    if (star_idx == MOX::_NUM_STARS) {
        MOX::_combat_defender_player = MAX_PLAYERS;
        colony_pop = 0;
    } else if (MOX::_cmbt_colony_id == -1) {
        MOX::_combat_defender_player = MOX::_ship[plr2_ships[0]].owner;
        colony_pop = 0;
    } else {
        MOX::_combat_defender_player = MOX::_colony[MOX::_cmbt_colony_id].owner;
        colony_pop = MOX::_colony[MOX::_cmbt_colony_id].n_pops;
    }

    if (star_idx == MOX::_NUM_STARS) {
        MOX::_in_nebula_flag = 0;
    } else {
        MOX::_in_nebula_flag = GEO::Point_Is_In_Some_Nebula_(MOX::_star[star_idx].x, MOX::_star[star_idx].y) ? 1 : 0;
    }

    qsort(plr1_ships, plr1_ship_count, sizeof(*plr1_ships), qsort_ship_array_by_size_);

    int16_t loaded_ships_1 = 0;
    for (int16_t i = 0; i < plr1_ship_count; ++i) {
        if (MOX::_ship[plr1_ships[i]].d.ship_type == SHIP_TYPE_COMBAT && loaded_ships_1 < 100) {
            Load_Combat_Ship_(plr1_ships[i], MOX::_combat_total_ship_count);

            for (int j = 0; j < 8; ++j) {
                switch (MOX::_combat_data[MOX::_combat_total_ship_count].weapons[j].weapon_id) {
                    case WEAPON_PLASMA_WEB:
                    case WEAPON_CAUSTIC_SLIME:
                        has_plasma_web = 1;
                        break;
                    case WEAPON_BLACK_HOLE_GENERATOR:
                        has_bhg = 1;
                        break;
                    case WEAPON_STELLAR_CONVERTER:
                        has_stellar_converter = 1;
                        break;
                    case WEAPON_TRACTOR_BEAM:
                        has_tractor_beam = 1;
                        break;
                }
            }

            if (Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, SPECIAL_PHASING_CLOAK) == 0) {
                COMBINIT::_real_attack_flag = 1;
            }

            // Original binary calls this and discards the result.
            Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 10);
            if (Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, SPECIAL_REFLECTION_FIELD) != 0) {
                has_special_26 = 1;
            }

            MOX::_combat_total_ship_count++;
            loaded_ships_1++;
        }
        RUSS::Mox_Update_();
    }

    if (MOX::_combat_defender_player < MAX_PLAYERS && star_idx < MOX::_NUM_STARS) {
        if (HAROLD::Player_Has_Artemis_Net_(&MOX::_player[MOX::_combat_defender_player], &MOX::_star[star_idx])) {
            abort_tactical = CMBTFIR2::Apply_Combat_Artemis_Net_(0);
            if (abort_tactical == 0) {
                KEN::Ken_Get_Text_Message_(0x53, msg_buf, 80);
            } else {
                KEN::Ken_Get_Text_Message_(0x52, msg_buf, 80);
            }
            palette::Refresh_Palette_();
            GENDRAW::Message_Box_(msg_buf);
            combat_winner = (int16_t)0xFFFF;
        }
    }

    if (no_defender_ships) {
        for (int16_t i = 0; i < plr2_ship_count; ++i) {
            if (MOX::_ship[plr2_ships[i]].d.ship_type == SHIP_TYPE_COMBAT) {
                no_defender_ships = false;
                break;
            }
        }
    }

    if (no_defender_ships) {
        if (abort_tactical == 0) {
            if (star_idx != MOX::_NUM_STARS) {
                if (MOX::_cmbt_colony_id == -1 || Colony_Has_Defenses_(colony_idx) == 0) {
                    combat_winner = 1;
                    abort_tactical = 1;
                }
            }
        }
    }

    if (abort_tactical == 0) {
        MOX2::Get_Auto_Function_(&auto_func);
        graphics::Reset_Window_();
        JIM::Play_Combat_Music_();
        RUSS::Mox_Update_();
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        mouse::Set_Mouse_List_(&local_mouse_list, 1);
        MAINSCR::Set_Reload_Fleet_Box_Palette_Flag_();
        ERIC::Fast_Fade_Out_();
        fonts::Load_Palette_(3, 0, 0xFF);
        ERIC::Set_Fade_Percent_(0);

        MOX::_missile = (s_missile*)allocate::Allocate_Next_Block_(MOX::_screen_seg, kCombatMissileBytes);
        struct_::Clear_Structure_(MOX::_missile, kCombatMissileBytes);
        RUSS::Mox_Update_();

        MOX::_level_symbol_seg = farload::Far_Reload_Next_("combat.lbx", 0x2A, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_scan_button_seg = farload::Far_Reload_Next_("combat.lbx", 1, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_board_button_seg = farload::Far_Reload_Next_("combat.lbx", 0x33, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_auto_button_seg = farload::Far_Reload_Next_("combat.lbx", 0x32, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_flee_button_seg = farload::Far_Reload_Next_("combat.lbx", 0x35, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_cmbt_done_button_seg = farload::Far_Reload_Next_("combat.lbx", 0x36, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_wait_button_seg = farload::Far_Reload_Next_("combat.lbx", 0x34, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_cmbt_info_button_seg = farload::Far_Reload_Next_("combat.lbx", 10, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_cmbt_option_seg = farload::Far_Reload_Next_("combat.lbx", 2, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_self_destruct_seg = farload::Far_Reload_Next_("combat.lbx", 3, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_weapons_button_seg = farload::Far_Reload_Next_("combat.lbx", 7, MOX::_screen_seg);
        RUSS::Mox_Update_();

        for (int16_t i = 0; i < 0x70; ++i) {
            MOX::_msl_seg[i] = (s_animation_header*)farload::Far_Reload_Next_("cmbtmisl.lbx", i, MOX::_screen_seg);
            RUSS::Mox_Update_();
        }

        MOX::_combat_mouse_grid = (s_mouse_list*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x2928);
        MOX::_specials_button_seg = farload::Far_Reload_Next_("combat.lbx", 8, MOX::_screen_seg);
        MOX::_msl_warning_on_seg = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", 4, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_warning_missile_seg = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", 0x25, MOX::_screen_seg);
        MOX::_warning_fighter_seg = farload::Far_Reload_Next_("combat.lbx", 0x26, MOX::_screen_seg);
        RUSS::Mox_Update_();

        for (int16_t i = 0; i < 3; ++i) {
            MOX::_ship_cursor[i] = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", i + 0x20, MOX::_screen_seg);
            RUSS::Mox_Update_();
        }

        MOX::_ship_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0x62, 0x62));
        MOX::_draw_ship_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0x62, 0x62));
        MOX::_combat_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(0x62, 0x62));
        RUSS::Mox_Update_();

        MOX::_shield_div_bitmap[0] = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", 0xE, MOX::_screen_seg);
        MOX::_shield_div_bitmap[1] = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", 0xF, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MOX::_shield_div_bitmap[2] = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", 0xC, MOX::_screen_seg);
        MOX::_shield_div_bitmap[3] = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", 0xD, MOX::_screen_seg);
        RUSS::Mox_Update_();

        for (int16_t i = 0; i < 4; ++i) {
            for (int16_t j = 0; j < 4; ++j) {
                MOX::_shield_mask_bitmap[j + i * 4] = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", j + i * 4 + 0x10, MOX::_screen_seg);
            }
            RUSS::Mox_Update_();
        }

        MOX::_combat_mouse_grid->x1 = 0;
        MOX::_combat_mouse_grid->x2 = 0x27F;
        MOX::_combat_mouse_grid->y1 = 0;
        MOX::_combat_mouse_grid->y2 = 0x15E;
        MOX::_combat_mouse_grid->frame = 12;
        MOX::_combat_mouse_grid->icon = 2;

        int16_t grid_x = 0;
        int16_t grid_idx = 1;
        for (int16_t col = 0; col < 32; ++col) {
            int16_t grid_y = 0;
            for (int16_t row = 0; row < 18; ++row) {
                MOX::_combat_mouse_grid[grid_idx].x1 = grid_x;
                MOX::_combat_mouse_grid[grid_idx].x2 = grid_x + 19;
                MOX::_combat_mouse_grid[grid_idx].y1 = grid_y;
                MOX::_combat_mouse_grid[grid_idx].y2 = grid_y + 19;
                MOX::_combat_mouse_grid[grid_idx].frame = 0;
                MOX::_combat_mouse_grid[grid_idx].icon = 0;
                grid_y += 20;
                grid_idx++;
            }
            grid_x += 20;
        }

        MOX::_combat_mouse_grid[0x36d].x1 = 0;
        MOX::_combat_mouse_grid[0x36d].x2 = 0x27F;
        MOX::_combat_mouse_grid[0x36d].y1 = 0x15F;
        MOX::_combat_mouse_grid[0x36d].y2 = 0x1DF;
        MOX::_combat_mouse_grid[0x36d].frame = 0;
        MOX::_combat_mouse_grid[0x36d].icon = 1;

        if (MOX::_game_type != 0 && MOX::_game_type != 1) {
            if (MOX::_combat_attacker_player < MOX::_NUM_PLAYERS && MOX::_player[MOX::_combat_attacker_player].objectives == PLAYER_OBJECTIVE_HUMAN) {
                if (MOX::_combat_defender_player < MOX::_NUM_PLAYERS && MOX::_player[MOX::_combat_defender_player].objectives == PLAYER_OBJECTIVE_HUMAN) {
                    MOX::_net_flag = 1;
                }
            }
        }

        if (MOX::_combat_attacker_player <= MAX_PLAYERS) {
            MOX::_fighter_beam_type[MOX::_combat_attacker_player] = INITSHIP::Best_Point_Defense_Weapon_(MOX::_combat_attacker_player);
            MOX::_fighter_bomb_type[MOX::_combat_attacker_player] = INITSHIP::Best_Bomb_Weapon_(MOX::_combat_attacker_player);
        }

        if (MOX::_combat_defender_player <= MAX_PLAYERS) {
            MOX::_fighter_beam_type[MOX::_combat_defender_player] = INITSHIP::Best_Point_Defense_Weapon_(MOX::_combat_defender_player);
            MOX::_fighter_bomb_type[MOX::_combat_defender_player] = INITSHIP::Best_Bomb_Weapon_(MOX::_combat_defender_player);
        }

        RUSS::Mox_Update_();
        glass_remap_block = palette::Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
        palette::Save_Glass_Remap_Block_(glass_remap_block);
        RUSS::Mox_Update_();

        if (star_idx == MOX::_NUM_STARS) {
            has_bhg = 1;
            has_special_26 = 1;
            Load_Antaran_Defense_Fleet_();
            Load_Antaran_Planet_();
        } else {
            qsort(plr2_ships, plr2_ship_count, sizeof(*plr2_ships), qsort_ship_array_by_size_);
            int16_t loaded_ships_2 = 0;
            for (int16_t i = 0; i < plr2_ship_count; ++i) {
                if (MOX::_ship[plr2_ships[i]].d.ship_type == SHIP_TYPE_COMBAT && loaded_ships_2 < 100) {
                    Load_Combat_Ship_(plr2_ships[i], MOX::_combat_total_ship_count);

                    for (int j = 0; j < 8; ++j) {
                        switch (MOX::_combat_data[MOX::_combat_total_ship_count].weapons[j].weapon_id) {
                            case WEAPON_PLASMA_WEB:
                            case WEAPON_CAUSTIC_SLIME:
                                has_plasma_web = 1;
                                break;
                            case WEAPON_BLACK_HOLE_GENERATOR:
                                has_bhg = 1;
                                break;
                            case WEAPON_STELLAR_CONVERTER:
                                has_stellar_converter = 1;
                                break;
                            case WEAPON_TRACTOR_BEAM:
                                has_tractor_beam = 1;
                                break;
                        }
                    }

                    // Original binary calls this and discards the result.
                    Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, 10);
                    if (Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, SPECIAL_REFLECTION_FIELD) != 0) {
                        has_special_26 = 1;
                    }

                    MOX::_combat_total_ship_count++;
                    loaded_ships_2++;
                }
                RUSS::Mox_Update_();
            }
            Load_Colony_Defense_(colony_idx);
        }

        if ((CMBTFIRE::Does_Combat_Planet_Have_Defenses_() & COMBAT_PLANET_DEFENSE_STELLAR_CONVERTER) != 0) {
            has_stellar_converter = 1;
        }

        RUSS::Mox_Update_();
        Load_Combat_Ship_Palette_(MOX::_combat_attacker_player);
        RUSS::Mox_Update_();
        Load_Combat_Ship_Palette_(MOX::_combat_defender_player);
        RUSS::Mox_Update_();

        bool needs_plasma_darkening = false;
        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            s_combat_data* ship = &MOX::_combat_data[i];
            if (struct_::Test_Bit_Field_(ship->special_device_flags, SPECIAL_CLOAKING_DEVICE) ||
                struct_::Test_Bit_Field_(ship->special_device_flags, SPECIAL_PHASING_CLOAK)) {
                needs_plasma_darkening = true;
                break;
            }
        }

        if (needs_plasma_darkening) {
            MOX::_darken_plasma_map = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", 0x31, MOX::_screen_seg);
            RUSS::Mox_Update_();
            CMBTDRW1::Create_Ship_Darken_Map_();
        }

        BEAMS::Load_Combat_Beam_Stuff_((int8_t)has_special_26);

        MOX::_shield_remap_colors = palette::Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
        RUSS::Mox_Update_();
        int16_t shield_c = 23;
        for (int16_t i = 1; i < 16; ++i) {
            palette::Set_Glass_Remap_Color_(i, (uint8_t)(i * 2 + 13), (uint8_t)(i * 2 + 30), 63, (uint8_t)shield_c);
            shield_c += 3;
        }
        RUSS::Mox_Update_();
        palette::Set_Palette_Gradient_(144, 32, 0, 0, 0, 0, 0, 0);
        palette::Set_Palette_Changes_(0, 255);
        RUSS::Mox_Update_();
        remap::Update_Glass_Remap_Colors_();
        palette::Save_Glass_Remap_Block_(MOX::_shield_remap_colors);
        RUSS::Mox_Update_();

        MOX::_pulsar_remap_colors = palette::Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
        s_animation_header* pulsar_pal = (s_animation_header*)buffer::Buffer_Reload_("cmbtsfx.lbx", 0x34, MOX::_global_cache_seg);
        animate::Draw_Palette_(pulsar_pal);
        RUSS::Mox_Update_();
        int16_t pulsar_c = 53;
        for (int16_t i = 1; i < 16; ++i) {
            palette::Set_Glass_Remap_Color_(
                i,
                video::_current_palette[kPlayerRemapPaletteStart + i].r,
                video::_current_palette[kPlayerRemapPaletteStart + i].g,
                video::_current_palette[kPlayerRemapPaletteStart + i].b,
                (uint8_t)pulsar_c
            );
            pulsar_c += 3;
        }
        palette::Set_Palette_Changes_(0, 255);
        remap::Update_Glass_Remap_Colors_();
        palette::Save_Glass_Remap_Block_(MOX::_pulsar_remap_colors);

        MOX::_warp_core_remap_colors = palette::Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
        s_animation_header* warp_pal = (s_animation_header*)buffer::Buffer_Reload_("sphersfx.lbx", 0, MOX::_global_cache_seg);
        RUSS::Mox_Update_();
        animate::Draw_Palette_(warp_pal);
        int16_t warp_c = 53;
        for (int16_t i = 1; i < 16; ++i) {
            palette::Set_Glass_Remap_Color_(
                i,
                video::_current_palette[kPlayerRemapPaletteStart + i].r,
                video::_current_palette[kPlayerRemapPaletteStart + i].g,
                video::_current_palette[kPlayerRemapPaletteStart + i].b,
                (uint8_t)warp_c
            );
            warp_c += 3;
        }
        palette::Set_Palette_Changes_(0, 255);
        remap::Update_Glass_Remap_Colors_();
        palette::Save_Glass_Remap_Block_(MOX::_warp_core_remap_colors);
        RUSS::Mox_Update_();

        if (has_tractor_beam != 0) {
            MOX::_small_tractor_seg = (s_animation_header*)farload::Far_Reload_Next_("cmbtsfx.lbx", 2, MOX::_screen_seg);
            RUSS::Mox_Update_();
            MOX::_tractor_remap_colors = palette::Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
            int16_t tractor_c = 23;
            for (int16_t i = 1; i < 16; ++i) {
                palette::Set_Glass_Remap_Color_(
                    i,
                    video::_current_palette[kTractorBeamRemapPaletteStart + i / 2].r,
                    video::_current_palette[kTractorBeamRemapPaletteStart + i / 2].g,
                    video::_current_palette[kTractorBeamRemapPaletteStart + i / 2].b,
                    (uint8_t)tractor_c
                );
                tractor_c += 3;
            }
            palette::Set_Palette_Changes_(0, 255);
            RUSS::Mox_Update_();
            remap::Update_Glass_Remap_Colors_();
            palette::Save_Glass_Remap_Block_(MOX::_tractor_remap_colors);
        }

        MOX::_stasis_bubble_remap_colors = MOX::_shield_remap_colors;

        if (has_bhg != 0) {
            MOX::_bhg_remap_colors = palette::Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
            s_animation_header* bhg_pal = (s_animation_header*)buffer::Buffer_Reload_("cmbtsfx.lbx", 0x2E, MOX::_global_cache_seg);
            animate::Draw_Palette_(bhg_pal);
            for (int16_t i = 1; i < 16; ++i) {
                palette::Set_Glass_Remap_Color_(
                    i,
                    video::_current_palette[kPlayerRemapPaletteStart + i].r,
                    video::_current_palette[kPlayerRemapPaletteStart + i].g,
                    video::_current_palette[kPlayerRemapPaletteStart + i].b,
                    i * 2 + 60
                );
            }
            palette::Set_Palette_Gradient_(144, 32, 0, 0, 0, 0, 0, 0);
            palette::Set_Palette_Changes_(0, 255);
            RUSS::Mox_Update_();
            remap::Update_Glass_Remap_Colors_();
            palette::Save_Glass_Remap_Block_(MOX::_bhg_remap_colors);
        }

        if (has_plasma_web != 0) {
            MOX::_plasma_web_remap_colors = palette::Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
            for (int16_t i = 1; i < 16; ++i) {
                int16_t c = (i * 3 + 15) * 2;
                if (c > 100) c = 100;
                palette::Set_Glass_Remap_Color_(
                    i,
                    video::_current_palette[kPlasmaWebRemapPaletteStart + i].r,
                    video::_current_palette[kPlasmaWebRemapPaletteStart + i].g,
                    video::_current_palette[kPlasmaWebRemapPaletteStart + i].b,
                    (uint8_t)c
                );
            }
            RUSS::Mox_Update_();
            palette::Set_Palette_Changes_(0, 255);
            remap::Update_Glass_Remap_Colors_();
            palette::Save_Glass_Remap_Block_(MOX::_plasma_web_remap_colors);
        }

        if (has_stellar_converter != 0) {
            MOX::_stellar_converter_remap_colors = palette::Allocate_Glass_Remap_Next_Block_(MOX::_screen_seg);
            RUSS::Mox_Update_();
            s_animation_header* stellar_pal = (s_animation_header*)buffer::Buffer_Reload_("cmbtsfx.lbx", 0x28, MOX::_global_cache_seg);
            animate::Draw_Palette_(stellar_pal);
            int16_t stellar_c = 53;
            for (int16_t i = 1; i < 16; ++i) {
                palette::Set_Glass_Remap_Color_(
                    i,
                    video::_current_palette[kPlayerRemapPaletteStart + i * 2].r,
                    video::_current_palette[kPlayerRemapPaletteStart + i * 2].g,
                    video::_current_palette[kPlayerRemapPaletteStart + i * 2].b,
                    (uint8_t)stellar_c
                );
                stellar_c += 3;
            }
            palette::Set_Palette_Changes_(0, 255);
            RUSS::Mox_Update_();
            remap::Update_Glass_Remap_Colors_();
            palette::Save_Glass_Remap_Block_(MOX::_stellar_converter_remap_colors);
            palette::Set_Palette_Gradient_(144, 32, 0, 0, 0, 0, 0, 0);
        }

        CMBTFIR2::Set_Combat_Ordnance_Values_();
        RUSS::Mox_Update_();
        Load_Individual_Ship_Pictures_(MOX::_combat_attacker_player);
        RUSS::Mox_Update_();
        Load_Individual_Ship_Pictures_(MOX::_combat_defender_player);
        RUSS::Mox_Update_();
        palette::Restore_Glass_Remap_Block_(MOX::_shield_remap_colors);

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            Init_Special_Devices_(i);
        }

        Deploy_Ships_();
        RUSS::Mox_Update_();

        MOX::_auto_combat_flag[0] = 0;
        MOX::_auto_combat_flag[1] = 0;
        MOX::_skan_flag = 0;

        COMBAT1::Add_Tactical_Combat_Fields_();
        RUSS::Mox_Update_();

        combat_winner = COMBAT1::Check_For_Winner_();
        fields::Assign_Auto_Function_(CMBTDRW1::Draw_Main_Combat_Screen_, 1);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        int16_t turn_plr1 = MOX::_combat_attacker_player;
        int16_t turn_plr2 = MOX::_combat_defender_player;
        if (MOX::_combat_defender_player == MAX_PLAYERS) {
            turn_plr1 = MAX_PLAYERS;
            turn_plr2 = MOX::_combat_attacker_player;
        }

        if (MOX::_net_flag != 0) {
            struct_::Clear_Structure_(&MOX::_new_message, sizeof(MOX::_new_message));
            int16_t net_plr = MOX::_combat_attacker_player;
            if (MOX::_PLAYER_NUM == MOX::_combat_attacker_player) {
                net_plr = MOX::_combat_defender_player;
            }
            NETMOX::Mox_Send_Message_(MOX::_player[net_plr].network_player_id, TACTICAL_NET_PACKET_INIT_SYNC, &MOX::_combat_init, sizeof(MOX::_combat_init));
            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(0);
        }
        
        RUSS::Mox_Update_();

        if (MOX::_net_flag != 0) {
            NETMOX::Net_Debug_Log_(
                "tactical combat loop start local_player=%d attacker=%d defender=%d initiative=%d ships=%d winner=%d",
                (int)MOX::_PLAYER_NUM,
                (int)MOX::_combat_attacker_player,
                (int)MOX::_combat_defender_player,
                (int)MOX::_combat_init,
                (int)MOX::_combat_total_ship_count,
                (int)combat_winner);
        }

        while (combat_winner == 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (i != 0 || MOX::_cmbt_colony_id != -1) {
                    MOX::_combat_data[i].current_speed = COMBAT1::Calc_Current_Speed_(i);
                    MOX::_combat_data[i].tac_val_attack = CMBTFIR2::Offensive_Combat_Bonus_(i);
                    MOX::_combat_data[i].tac_val_defense = CMBTFIR2::Defensive_Combat_Bonus_(i, 0);
                    MOX::_combat_data[i].tac_val_evade = CMBTFIR2::Defensive_Combat_Bonus_(i, 1);
                }
            }
            RUSS::Mox_Update_();

            for (int16_t i = 0; i < 300; ++i) {
                if (MOX::_missile[i].type > 0) {
                    if (MOX::_missile[i].travel_dist == 0) {
                        MOX::_missile[i].travel_dist = MOX::_missile[i].speed * 20;
                    }
                    MOX::_missile[i].is_active = 1;
                }
            }
            RUSS::Mox_Update_();

            CMBTAI::_fleet_target = -1;

            if (MOX::_combat_init == 0) {
                COMBAT1::Do_Combat_Turn_(turn_plr1, 0);
                COMBAT1::Check_For_Winner_();
                Plasma_Web_Damage_(turn_plr2);
                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();

                combat_winner = COMBAT1::Check_For_Winner_();
                if (combat_winner == 0) {
                    CMBTAI::_fleet_target = -1;
                    COMBAT1::Do_Combat_Turn_(turn_plr2, 0);
                    COMBAT1::Check_For_Winner_();
                }

                Plasma_Web_Damage_(turn_plr1);
                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();

                combat_winner = COMBAT1::Check_For_Winner_();
                if (combat_winner == 0) {
                    CMBTAI::_fleet_target = -1;
                    COMBAT1::Do_Combat_Turn_(turn_plr1, 1);
                    COMBAT1::Check_For_Winner_();
                    Increment_Bhg_(turn_plr1);
                }

                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();

                combat_winner = COMBAT1::Check_For_Winner_();
                if (combat_winner == 0) {
                    CMBTAI::_fleet_target = -1;
                    COMBAT1::Do_Combat_Turn_(turn_plr2, 1);
                    COMBAT1::Check_For_Winner_();
                    Increment_Bhg_(turn_plr2);
                }

                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();
                combat_winner = COMBAT1::Check_For_Winner_();
            } else {
                COMBAT1::Do_Combat_Turn_(0, 0);
                Plasma_Web_Damage_(turn_plr1);
                Plasma_Web_Damage_(turn_plr2);
                COMBAT1::Do_Combat_Turn_(0, 1);
                combat_winner = COMBAT1::Check_For_Winner_();
                if (MOX::_super_fast_flag != 0) CMBTDRW1::Draw_Super_Fast_Screen_();
            }

            MOX::_combat_turn++;
            End_Of_Turn_Bookeeping_();
        }

        if (MOX::_net_flag != 0) {
            NETMOX::Net_Debug_Log_(
                "tactical combat loop end local_player=%d winner=%d turn=%d cur_ship=%d cur_ptr=%d choice=%d barrier_counter=%u",
                (int)MOX::_PLAYER_NUM,
                (int)combat_winner,
                (int)MOX::_combat_turn,
                (int)MOX::_cur_ship,
                (int)MOX::_cur_ptr,
                (int)MOX::_tactical_net_choice,
                (unsigned int)MOX::_combat_done_waiting);
        }

        if (MOX::_cmbt_colony_id != -1) {
            colony_pop -= MOX::_colony[MOX::_cmbt_colony_id].n_pops;
        }
    }

    End_Of_Combat_(star_idx, colony_pop, combat_winner);

    if (combat_winner == 1 || combat_winner == 0x29A) {
        if (MOX::_combat_attacker_player < MAX_PLAYERS && MOX::_combat_defender_player >= 10 && MOX::_ship[plr2_ships[0]].d.ftl_type > 0) {
            NETMOX::Russ_Event_Monster_Update_(MOX::_combat_defender_player, -1, MOX::_combat_attacker_player);
        }
    }

    if (combat_winner == -1 || combat_winner == 0x29A) {
        if (MOX::_combat_defender_player < MAX_PLAYERS && MOX::_combat_attacker_player >= 10 && MOX::_ship[plr1_ships[0]].d.ftl_type > 0) {
            NETMOX::Russ_Event_Monster_Update_(MOX::_combat_attacker_player, -1, MOX::_combat_defender_player);
        }
    }

    if (abort_tactical == 0) {
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        fields::Deactivate_Help_List_();
        mouse::Set_Mouse_List_(&local_mouse_list, 1);
        ERIC::Fast_Fade_Out_();
        palette::Set_Palette_Gradient_(160, 95, 0, 0, 0, 0, 0, 0);
        MOX2::Put_Auto_Function_(&auto_func);
        sound::Fadeout_Sound_(-1, 30);
        JIM::Play_Background_Music_();
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
        palette::Restore_Glass_Remap_Block_(glass_remap_block);
        MISC::Fade_Into_Screen_(MAINSCR::Draw_Mini_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
    }

    MAINSCR::Allocate_First_Screen_Seg_(5);

    MOX::_settings.combat_shields_arc_flag = (char)MOX::_shields_flag;
    MOX::_settings.combat_speedx2_flag = (char)MOX::_speedx2_flag;
    MOX::_settings.combat_missile_impact_flag = (char)MOX::_msl_impact_warning_flag;
    MOX::_settings.combat_legal_moves_flag = (char)MOX::_draw_legal_boxes_flag;
    MOX::_settings.combat_grid_flag = (char)MOX::_grid_flag;

    if (combat_winner == 0x29A) {
        combat_winner = 0;
    }
    
    MOX::_FULL_DRAW_SCREEN_FLAG = MOX::_net_flag;

    return combat_winner;
}

void __cdecl Load_Combat_Antaran_Ship_(int16_t ship_idx, int16_t ship_size) {
    switch (ship_size) {
    case HULL_FRIGATE:
        INITSHIP::Load_Small_Antaran_Combat_Ship_(ship_idx);
        break;
    case HULL_DESTROYER:
        INITSHIP::Load_Medium_Antaran_Combat_Ship_(ship_idx);
        break;
    case HULL_CRUISER:
        INITSHIP::Load_Large_Antaran_Combat_Ship_(ship_idx);
        break;
    case HULL_BATTLESHIP:
        INITSHIP::Load_Huge_Antaran_Combat_Ship_(ship_idx);
        break;
    case HULL_TITAN:
        INITSHIP::Load_Titan_Antaran_Combat_Ship_(ship_idx);
        break;
    default:
        break;
    }
}

void __cdecl Load_Antaran_Defense_Fleet_() {
    // Ensure at least one Antaran Frigate (defensive_ships[4]) is present
    if (MOX::_antarans.defensive_ships[4] < 1) {
        MOX::_antarans.defensive_ships[4] = 1;
    }

    for (int16_t ship_type_idx = 0; ship_type_idx < 5; ++ship_type_idx) {
        for (int16_t i = 0; i < MOX::_antarans.defensive_ships[ship_type_idx]; ++i) {
            Load_Combat_Antaran_Ship_(MOX::_combat_total_ship_count, ship_type_idx);
            MOX::_combat_total_ship_count++;
        }
    }

    Load_Antaran_Star_Fortress_();
}

void __cdecl Load_Antaran_Star_Fortress_() {
    int16_t n = MOX::_combat_total_ship_count;
    s_combat_data* ship = &MOX::_combat_data[n];

    ship->computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER;
    ship->armor_type = ARMOR_XENTRONIUM_ARMOR;

    strlcpy(ship->name, TECHDATA::_hull_data[HULL_STAR_FORTRESS].name, sizeof(ship->name));

    ship->structure_max = (int16_t)(
        TECHDATA::_hull_data[HULL_STAR_FORTRESS].structure_hp +
        ((int32_t)TECHDATA::_armor[ship->armor_type].ships_bonus * TECHDATA::_hull_data[HULL_STAR_FORTRESS].structure_hp) / 100);
    ship->structure_max = (int16_t)(ship->structure_max * 3);

    ship->armor_remaining = (int16_t)(
        TECHDATA::_hull_data[HULL_STAR_FORTRESS].armor_hp +
        ((int32_t)TECHDATA::_armor[ship->armor_type].ships_bonus * TECHDATA::_hull_data[HULL_STAR_FORTRESS].armor_hp) / 100);
    ship->armor_remaining = (int16_t)(ship->armor_remaining * 3);

    int32_t fortress_size_scaled = (int32_t)TECHDATA::_hull_data[HULL_STAR_FORTRESS].size * 5;
    fortress_size_scaled = (fortress_size_scaled + ((fortress_size_scaled >> 31) & 3)) >> 2;
    int16_t fortress_weapon_space = (int16_t)((fortress_size_scaled - (fortress_size_scaled >> 31)) >> 1);

    ship->size_class = SHIP_SIZE_TITAN;
    ship->picture_num = 5;
    ship->owner = (int8_t)MOX::_combat_defender_player;
    ship->previous_owner = (uint8_t)MOX::_combat_defender_player;
    ship->ship_idx = -1;
    ship->ftl_type = DRIVE_NO_DRIVE;
    ship->shield_type = SHIELD_NO_SHIELD;
    ship->unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        ship->special_device_flags[i] = 0;
        ship->special_device_damage_flags[i] = 0;
    }

    struct_::Set_Bit_Field_(ship->special_device_flags, SPECIAL_BATTLE_SCANNER);
    struct_::Set_Bit_Field_(ship->special_device_flags, SPECIAL_DAMPER_FIELD);
    struct_::Set_Bit_Field_(ship->special_device_flags, SPECIAL_QUANTUM_DETONATOR);
    struct_::Set_Bit_Field_(ship->special_device_flags, SPECIAL_REFLECTION_FIELD);
    struct_::Set_Bit_Field_(ship->special_device_flags, SPECIAL_HEAVY_ARMOR);
    struct_::Set_Bit_Field_(ship->special_device_flags, SPECIAL_REINFORCED_HULL);
    struct_::Set_Bit_Field_(ship->special_device_flags, SPECIAL_SECURITY_STATIONS);
    struct_::Set_Bit_Field_(ship->special_device_flags, SPECIAL_STRUCTURAL_ANALYZER);

    for (int i = 0; i < 8; ++i) {
        struct_::Clear_Structure_(&ship->weapons[i], sizeof(ship->weapons[i]));
    }

    int w_idx = 0;
    ship->weapons[w_idx].weapon_id = WEAPON_DEATH_RAY;
    ship->weapons[w_idx].active = 1;
    ship->weapons[w_idx].ammo = -1;
    ship->weapons[w_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    ship->weapons[w_idx].specials = 2;

    int space0 = DESIGN::Weapon_Space_(MOX::_combat_defender_player, ship->weapons[w_idx].weapon_id, 1, WEAPON_FIRING_ARC_FORWARD, 0, ship->weapons[w_idx].specials);
    int count0 = (fortress_weapon_space / 2) / space0;
    if (count0 > 99) {
        count0 = 99;
    }
    ship->weapons[w_idx].count = (uint8_t)count0;
    ship->weapons[w_idx].shots_left = (uint8_t)count0;

    w_idx++;
    ship->weapons[w_idx].weapon_id = WEAPON_PARTICLE_BEAM;
    ship->weapons[w_idx].active = 1;
    ship->weapons[w_idx].ammo = -1;
    ship->weapons[w_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    ship->weapons[w_idx].specials = 0;

    int space1 = DESIGN::Weapon_Space_(MOX::_combat_defender_player, ship->weapons[w_idx].weapon_id, 1, WEAPON_FIRING_ARC_FORWARD, 0, ship->weapons[w_idx].specials);
    int count1 = (fortress_weapon_space / 4) / space1;
    if (count1 > 198) {
        count1 = 198;
    }

    if (count1 > 99) {
        int extra = (count1 - 1) / 99;
        count1 = count1 - extra * 99;
        for (int i = 0; i < extra; ++i) {
            ship->weapons[w_idx].count = 99;
            ship->weapons[w_idx].shots_left = 99;
            w_idx++;
            ship->weapons[w_idx].weapon_id = WEAPON_PARTICLE_BEAM;
            ship->weapons[w_idx].active = 1;
            ship->weapons[w_idx].ammo = -1;
            ship->weapons[w_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
            ship->weapons[w_idx].specials = 0;
        }
    }
    ship->weapons[w_idx].count = (uint8_t)count1;
    ship->weapons[w_idx].shots_left = (uint8_t)count1;

    w_idx++;
    ship->weapons[w_idx].weapon_id = WEAPON_PARTICLE_BEAM;
    ship->weapons[w_idx].active = 1;
    ship->weapons[w_idx].ammo = -1;
    ship->weapons[w_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    ship->weapons[w_idx].specials = 4;

    int space2 = DESIGN::Weapon_Space_(MOX::_combat_defender_player, ship->weapons[w_idx].weapon_id, 1, WEAPON_FIRING_ARC_FORWARD, 0, ship->weapons[w_idx].specials);
    int count2 = (fortress_weapon_space / 4) / space2;
    if (count2 > 198) {
        count2 = 198;
    }

    if (count2 > 99) {
        int extra = (count2 - 1) / 99;
        count2 = count2 - extra * 99;
        for (int i = 0; i < extra; ++i) {
            ship->weapons[w_idx].count = 99;
            ship->weapons[w_idx].shots_left = 99;
            w_idx++;
            ship->weapons[w_idx].weapon_id = WEAPON_PARTICLE_BEAM;
            ship->weapons[w_idx].active = 1;
            ship->weapons[w_idx].ammo = -1;
            ship->weapons[w_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
            ship->weapons[w_idx].specials = 4;
        }
    }
    ship->weapons[w_idx].count = (uint8_t)count2;
    ship->weapons[w_idx].shots_left = (uint8_t)count2;

    w_idx++;
    ship->weapons[w_idx].weapon_id = WEAPON_PARTICLE_BEAM;
    ship->weapons[w_idx].active = 1;
    ship->weapons[w_idx].ammo = -1;
    ship->weapons[w_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    ship->weapons[w_idx].specials = 2;

    int space3 = DESIGN::Weapon_Space_(MOX::_combat_defender_player, ship->weapons[w_idx].weapon_id, 1, WEAPON_FIRING_ARC_FORWARD, 0, ship->weapons[w_idx].specials);
    int count3 = (fortress_weapon_space / 2) / space3;
    if (count3 > 99) {
        count3 = 99;
    }
    ship->weapons[w_idx].count = (uint8_t)count3;
    ship->weapons[w_idx].shots_left = (uint8_t)count3;

    for (int i = 0; i < 8; ++i) {
        ship->weapon_ready_flags[i] = 1;
    }

    ship->crew_quality = CREW_QUALITY_ELITE;
    ship->officer_idx = 0x42;
    ship->marine_count = (uint8_t)TECHDATA::_hull_data[HULL_STAR_FORTRESS].marines;
    ship->current_speed = 0;
    ship->structure_damage = 0;
    ship->is_retreating = 0;

    CMBTFIR2::Init_Internal_Space_(ship);

    ship->shield_arc_max = 0;
    for (int i = 0; i < 4; ++i) {
        ship->shield_arc_current[i] = ship->shield_arc_max;
    }

    MOX::_combat_total_ship_count++;
}

void __cdecl Load_Antaran_Planet_() {
    struct_::Clear_Structure_(MOX::_combat_data, sizeof(*MOX::_combat_data));

    strlcpy(MOX::_combat_data->name, "Antares", sizeof(MOX::_combat_data->name));
    MOX::_combat_data->ship_idx = -1;
    MOX::_combat_data->officer_idx = -1;
    MOX::_combat_data->size_class = SHIP_SIZE_DOOM_STAR;
    MOX::_combat_data->unit_status = 0;
    MOX::_combat_data->owner = MAX_PLAYERS;
    MOX::_combat_data->previous_owner = MAX_PLAYERS;
    MOX::_combat_data->ftl_type = DRIVE_NO_DRIVE;
    MOX::_combat_data->is_retreating = 0;
    MOX::_combat_data->shield_type = SHIELD_NO_SHIELD;
    MOX::_combat_data->shield_arc_max = 0;

    for (int i = 0; i < 4; ++i) {
        MOX::_combat_data->shield_arc_current[i] = 0;
    }

    MOX::_combat_data->computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER;
    MOX::_combat_data->armor_type = ARMOR_XENTRONIUM_ARMOR;

    for (int i = 0; i < 5; ++i) {
        MOX::_combat_data->special_device_flags[i] = 0;
        MOX::_combat_data->special_device_damage_flags[i] = 0;
    }

    CMBTFIR2::Init_Internal_Space_(MOX::_combat_data);

    MOX::_combat_data->shield_max_hits = 0;
    MOX::_combat_data->shield_current_hits = 0;

    s_animation_header* palette_anim = (s_animation_header*)buffer::Buffer_Reload_("cmbtplnt.lbx", 62, MOX::_global_cache_seg);
    animate::Draw_Palette_(palette_anim);
    palette::Refresh_Palette_();

    MOX::_combat_planet_seg = (s_animation_header*)farload::Far_Reload_Next_("cmbtplnt.lbx", 60 /* 0x3C */, MOX::_screen_seg);
    RUSS::Mox_Update_();

    MOX::_combat_planet_seg2 = (s_animation_header*)farload::Far_Reload_Next_("cmbtplnt.lbx", 61 /* 0x3D */, MOX::_screen_seg);
    RUSS::Mox_Update_();
}

void __cdecl Load_Combat_Ship_Palette_(int16_t player_idx) {
    s_animation_header* ship_anim;
    int lbx_index;
    int16_t planet_size;
    int16_t player_color;
    char lbx_filename[32];
    uint8_t color_table[32];

    for (int i = 0; i < 32; ++i) {
        lbx_filename[i] = (char)i + 0x20;
        color_table[i] = (uint8_t)i + 0x60;
    }

    if (player_idx < MAX_PLAYERS) {
        strlcpy(lbx_filename, "cmbtshp.lbx", sizeof(lbx_filename));
        lbx_index = (int)MOX::_player[player_idx].color * 45 + 44;
    } else {
        strlcpy(lbx_filename, "monster.lbx", sizeof(lbx_filename));
        lbx_index = player_idx + 5;
    }

    ship_anim = (s_animation_header*)buffer::Buffer_Reload_(lbx_filename, lbx_index, MOX::_global_cache_seg);

    if (player_idx == MOX::_combat_defender_player && player_idx <= MAX_PLAYERS && (MOX::_cmbt_colony_id > -1 || player_idx == MAX_PLAYERS)) {
        if (player_idx == MAX_PLAYERS) {
            planet_size = 2;
            player_color = 2;
        } else {
            player_color = (int16_t)MOX::_player[player_idx].color;
            planet_size = (int16_t)MOX::_planet[MOX::_colony[MOX::_cmbt_colony_id].planet].size;
        }

        if (planet_size > 0) {
            MOX::_combat_map_planet_seg = (s_animation_header*)farload::Far_Reload_Next_("combat.lbx", (int)planet_size + (int)player_color * 4 + 0x36, MOX::_screen_seg);
        }

        RUSS::Mox_Update_();

        if (planet_size > 0) {
            replace::Shift_Palette_Range_(ship_anim, 0x60);
            animate::Draw_Palette_(ship_anim);
            replace::Replace_Color_Range_(MOX::_combat_map_planet_seg, color_table, 0x50, 0x10);
        }
    }

    if (player_idx == MOX::_combat_attacker_player) {
        replace::Shift_Palette_Range_(ship_anim, 0x20);
        animate::Draw_Palette_(ship_anim);
        RUSS::Mox_Update_();
    } else {
        replace::Shift_Palette_Range_(ship_anim, 0x60);
        animate::Draw_Palette_(ship_anim);
        RUSS::Mox_Update_();
    }
}

int8_t __cdecl Ship_Can_Deploy_At_(int16_t x, int16_t y, int16_t ship_idx) {
    int8_t result = 1;
    int16_t size_threshold = 0;

    if (MOX::_cmbt_colony_id != -1) {
        int16_t planet_idx = MOX::_colony[MOX::_cmbt_colony_id].planet;
        int8_t planet_size = MOX::_planet[planet_idx].size;

        switch (planet_size) {
        case PLANET_SIZE_TINY:
            size_threshold = 3;
            break;
        case PLANET_SIZE_SMALL:
            size_threshold = 4;
            break;
        case PLANET_SIZE_MEDIUM:
            size_threshold = 6;
            break;
        case PLANET_SIZE_LARGE:
            size_threshold = 7;
            break;
        case PLANET_SIZE_HUGE:
            size_threshold = 8;
            break;
        default:
            break;
        }

        // Check proximity to the primary combat object (usually a colony/starbase)
        if (MOX::_combat_data->x <= x + 2 && x <= MOX::_combat_data->x + size_threshold) {
            if (MOX::_combat_data->y + size_threshold >= y && y + 2 >= MOX::_combat_data->y) {
                result = 0;
            }
        }
    }

    // Check proximity to another specific unit if provided
    if (ship_idx > -1) {
        if (MOX::_combat_data[ship_idx].x <= x + 2 && x <= MOX::_combat_data[ship_idx].x + 3) {
            if (MOX::_combat_data[ship_idx].y + 3 >= y && y + 2 >= MOX::_combat_data[ship_idx].y) {
                result = 0;
            }
        }
    }

    return result;
}

void __cdecl Load_Combat_Colony_Picture_(int16_t colony_idx) {
    int16_t planet_idx = MOX::_colony[colony_idx].planet;
    s_planet_data* planet = &MOX::_planet[planet_idx];

    int8_t climate = planet->climate;
    int8_t size = planet->size;

    s_animation_header* palette_anim = (s_animation_header*)buffer::Buffer_Reload_("cmbtplnt.lbx", climate * 6 + 5, MOX::_global_cache_seg);
    animate::Draw_Palette_(palette_anim);
    RUSS::Mox_Update_();

    MOX::_combat_planet_seg = (s_animation_header*)farload::Far_Reload_Next_("cmbtplnt.lbx", size + (climate * 6), MOX::_screen_seg);
    RUSS::Mox_Update_();

    if (planet->size != 0) {
        MOX::_combat_planet_seg2 = (s_animation_header*)farload::Far_Reload_Next_("cmbtplnt.lbx", climate * 6, MOX::_screen_seg);
        RUSS::Mox_Update_();
    } else {
        MOX::_combat_planet_seg2 = MOX::_combat_planet_seg;
    }
}

void __cdecl Load_Colony_Defense_(int16_t colony_idx) {
    if (colony_idx < 0) {
        MOX::_combat_data->owner = MAX_PLAYERS;
        MOX::_combat_data->previous_owner = MAX_PLAYERS;
        MOX::_combat_data->ship_idx = -1;
        MOX::_combat_data->unit_status = 5;
    } else {
        COMBINIT::Load_Tactical_Colony_(colony_idx, 1);
        int16_t station_type = COMBINIT::Type_Of_Space_Station_(MOX::_cmbt_colony_id);
        if (station_type != -1) {
            COMBINIT::Load_Combat_Satelite_(station_type);
            MOX::_combat_total_ship_count++;
        }
    }
}

void __cdecl Load_Tactical_Colony_(int16_t colony_idx, int16_t load_picture) {
    char* planet_name = HACCESS::Get_Planet_Name_No_Prime_(MOX::_colony[colony_idx].planet);
    strlcpy(MOX::_combat_data->name, planet_name, sizeof(MOX::_combat_data->name));

    MOX::_combat_data->owner = MOX::_colony[colony_idx].owner;
    MOX::_combat_data->previous_owner = MOX::_colony[colony_idx].owner;
    MOX::_combat_data->ship_idx = -1;
    MOX::_combat_data->ftl_type = DRIVE_NO_DRIVE;
    MOX::_combat_data->officer_idx = -1;
    MOX::_combat_data->is_retreating = 0;

    int16_t max_hits = 0;
    if (MOX::_colony[colony_idx].buildings[BUILDING_RADIATION_SHIELD] != 0) {
        MOX::_combat_data->shield_type = 0x1c;
        max_hits = TECHDATA::_planetary_shields[PLANETARY_RADIATION_SHIELD].absorption;
    } else if (MOX::_colony[colony_idx].buildings[BUILDING_BARRIER_SHIELD] != 0) {
        MOX::_combat_data->shield_type = 0x17;
        max_hits = TECHDATA::_planetary_shields[PLANETARY_BARRIER_SHIELD].absorption;
    } else if (MOX::_colony[colony_idx].buildings[BUILDING_FLUX_SHIELD] != 0) {
        MOX::_combat_data->shield_type = 0x18;
        max_hits = TECHDATA::_planetary_shields[PLANETARY_FLUX_SHIELD].absorption;
    } else {
        MOX::_combat_data->shield_type = SHIELD_NO_SHIELD;
        max_hits = 0;
    }

    MOX::_combat_data->shield_arc_max = max_hits;
    for (int i = 0; i < 4; i++) {
        MOX::_combat_data->shield_arc_current[i] = max_hits;
    }

    int16_t shield_id = INITSHIP::Best_Ship_Shield_(MOX::_combat_defender_player);
    MOX::_combat_data->shield_arc_current[0] += TECHDATA::_shields[shield_id].shield_class;

    MOX::_combat_data->computer_type = (int8_t)INITSHIP::Best_Computer_(MOX::_colony[colony_idx].owner);
    MOX::_combat_data->armor_type = (int8_t)INITSHIP::Best_Armor_(MOX::_colony[colony_idx].owner);

    MOX::_combat_data->size_class = SHIP_SIZE_DOOM_STAR;
    MOX::_combat_data->unit_status = 0;

    for (int i = 0; i < 5; i++) {
        MOX::_combat_data->special_device_flags[i] = 0;
        MOX::_combat_data->special_device_damage_flags[i] = 0;
    }

    for (int i = 0; i < 8; i++) {
        MOX::_combat_data->weapons[i].weapon_id = WEAPON_NO_WEAPONS;
    }

    int16_t weapon_index = 0;

    if (MOX::_colony[colony_idx].buildings[BUILDING_GROUND_BATTERIES] != 0) {
        int16_t weapon_id_1 = INITSHIP::Best_Beam_Weapon_(MOX::_colony[colony_idx].owner);
        int16_t mini_level_1 = DESIGN::Miniaturization_Level_(MOX::_combat_defender_player, TECHDATA::_weapons[weapon_id_1].tech_app_id);
        int16_t specials_1 = 2;

        if (TECHDATA::_weapon_modifications[WEAPON_MOD_ARMOR_PIERCING].level <= mini_level_1) {
            specials_1 = 0xa;
        }
        if (TECHDATA::_weapon_modifications[WEAPON_MOD_CONTINUOUS].level <= mini_level_1) {
            specials_1 |= 0x10;
        }
        if (TECHDATA::_weapon_modifications[WEAPON_MOD_SHIELD_PIERCING].level <= mini_level_1) {
            specials_1 |= 0x40;
        }
        if (TECHDATA::_weapon_modifications[WEAPON_MOD_NO_RANGE_DISSIPATION].level <= mini_level_1) {
            specials_1 |= 0x20;
        }
        specials_1 &= TECHDATA::_weapons[weapon_id_1].available_mods;

        int32_t space_1 = DESIGN::Weapon_Space_(MOX::_combat_defender_player, weapon_id_1, 1, WEAPON_FIRING_ARC_FORWARD, 1, specials_1);
        int32_t count_1 = 225 / space_1;

        MOX::_combat_data->weapons[0].weapon_id = weapon_id_1;
        MOX::_combat_data->weapons[0].count = (int8_t)count_1;
        MOX::_combat_data->weapons[0].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
        MOX::_combat_data->weapons[0].specials = specials_1;
        MOX::_combat_data->weapons[0].ammo = -1;
        MOX::_combat_data->weapons[0].active = 1;
        MOX::_combat_data->weapons[0].shots_left = (int8_t)count_1;
        MOX::_combat_data->weapons[0].status = 0;

        int16_t weapon_id_2 = INITSHIP::Best_Point_Defense_Weapon_By_Size_(MOX::_colony[colony_idx].owner);
        int16_t mini_level_2 = DESIGN::Miniaturization_Level_(MOX::_combat_defender_player, TECHDATA::_weapons[weapon_id_2].tech_app_id);
        int16_t specials_2 = 4;

        if (TECHDATA::_weapon_modifications[WEAPON_MOD_CONTINUOUS].level <= mini_level_2) {
            specials_2 = 0x14;
        }
        if (TECHDATA::_weapon_modifications[WEAPON_MOD_SHIELD_PIERCING].level <= mini_level_2) {
            specials_2 |= 0x40;
        }
        specials_2 &= TECHDATA::_weapons[weapon_id_2].available_mods;

        int32_t space_2 = DESIGN::Weapon_Space_(MOX::_combat_defender_player, weapon_id_2, 1, WEAPON_FIRING_ARC_FORWARD, 1, specials_2);
        int32_t count_2 = 75 / space_2;

        MOX::_combat_data->weapons[1].weapon_id = weapon_id_2;
        MOX::_combat_data->weapons[1].count = (int8_t)count_2;
        MOX::_combat_data->weapons[1].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
        MOX::_combat_data->weapons[1].specials = specials_2;
        MOX::_combat_data->weapons[1].ammo = -1;
        MOX::_combat_data->weapons[1].active = 1;
        MOX::_combat_data->weapons[1].shots_left = (int8_t)count_2;
        MOX::_combat_data->weapons[1].status = 0;

        weapon_index = 2;
    }

    if (MOX::_colony[colony_idx].buildings[BUILDING_FIGHTER_GARRISON] != 0) {
        int16_t weapon_id_fg = WEAPON_INTERCEPTOR;
        int16_t count_fg = 5;

        if (MOX::_player[MOX::_colony[colony_idx].owner].tech_applications[TECH_APP_HEAVY_FIGHTER_BAYS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            weapon_id_fg = WEAPON_HEAVY_FIGHTER;
            count_fg = 2;
        } else if (MOX::_player[MOX::_colony[colony_idx].owner].tech_applications[TECH_APP_BOMBER_BAYS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            weapon_id_fg = WEAPON_BOMBER;
            count_fg = 3;
        }

        for (int i = 0; i < 2; ++i) {
            MOX::_combat_data->weapons[weapon_index].weapon_id = weapon_id_fg;
            MOX::_combat_data->weapons[weapon_index].count = (int8_t)count_fg;
            MOX::_combat_data->weapons[weapon_index].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
            MOX::_combat_data->weapons[weapon_index].specials = 0;
            MOX::_combat_data->weapons[weapon_index].ammo = 1;
            MOX::_combat_data->weapons[weapon_index].active = 1;
            MOX::_combat_data->weapons[weapon_index].status = 0;
            MOX::_combat_data->weapons[weapon_index].shots_left = (int8_t)count_fg;
            weapon_index++;
        }
    }

    if (MOX::_colony[colony_idx].buildings[BUILDING_STELLAR_CONVERTER] != 0) {
        MOX::_combat_data->weapons[weapon_index].weapon_id = WEAPON_STELLAR_CONVERTER;
        MOX::_combat_data->weapons[weapon_index].count = 1;
        MOX::_combat_data->weapons[weapon_index].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
        MOX::_combat_data->weapons[weapon_index].specials = 0;
        MOX::_combat_data->weapons[weapon_index].status = 0;
        MOX::_combat_data->weapons[weapon_index].ammo = -1;
        MOX::_combat_data->weapons[weapon_index].active = 1;
        MOX::_combat_data->weapons[weapon_index].shots_left = 1;
        weapon_index++;
    }

    if (MOX::_colony[colony_idx].buildings[BUILDING_MISSILE_BASE] != 0) {
        int16_t weapon_id_m = INITSHIP::Best_Missile_Weapon_NT_(MOX::_colony[colony_idx].owner);
        int16_t mini_level_m = DESIGN::Miniaturization_Level_(MOX::_combat_defender_player, TECHDATA::_weapons[weapon_id_m].tech_app_id);
        int16_t specials_m = 0;

        if (TECHDATA::_weapon_modifications[WEAPON_MOD_FAST].level <= mini_level_m) {
            specials_m = 0x1000;
        }
        if (TECHDATA::_weapon_modifications[WEAPON_MOD_HEAVILY_ARMORED].level <= mini_level_m) {
            specials_m |= 0x800;
        }

        int32_t space_m = DESIGN::Weapon_Space_(MOX::_combat_defender_player, weapon_id_m, 1, WEAPON_FIRING_ARC_360, 5, specials_m);
        int32_t total_count = 300 / space_m;
        int32_t slots = 1;
        int32_t count_per_slot = total_count;

        if (total_count > 10) {
            slots = 2;
            count_per_slot = total_count / 2;
        }

        specials_m |= 0x400;

        for (int i = 0; i < slots; i++) {
            MOX::_combat_data->weapons[weapon_index].weapon_id = weapon_id_m;
            MOX::_combat_data->weapons[weapon_index].count = (uint8_t)count_per_slot;
            MOX::_combat_data->weapons[weapon_index].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
            MOX::_combat_data->weapons[weapon_index].specials = specials_m;
            MOX::_combat_data->weapons[weapon_index].status = 0;

            if (TECHDATA::_weapons[weapon_id_m].type == WEAPON_TYPE_MISSILE) {
                MOX::_combat_data->weapons[weapon_index].ammo = 125;
            } else {
                MOX::_combat_data->weapons[weapon_index].ammo = 0;
            }

            MOX::_combat_data->weapons[weapon_index].active = 1;
            MOX::_combat_data->weapons[weapon_index].shots_left = (uint8_t)count_per_slot;
            weapon_index++;
        }
    }

    if (weapon_index > 0) {
        if (_planet_damper_enabled != 0 &&
            MOX::_player[MOX::_combat_defender_player]
                .tech_applications[TECH_APP_DAMPER_FIELD] ==
                TECH_RESEARCH_STATUS_RESEARCHED) {
            struct_::Set_Bit_Field_(
                MOX::_combat_data->special_device_flags,
                SPECIAL_DAMPER_FIELD);
        }

        if (MOX::_player[MOX::_combat_defender_player].tech_applications[TECH_APP_BATTLE_SCANNER] == TECH_RESEARCH_STATUS_RESEARCHED) {
            struct_::Set_Bit_Field_(MOX::_combat_data->special_device_flags, SPECIAL_BATTLE_SCANNER);
        }
    }

    MOX::_combat_data->structure_max = 0;
    MOX::_combat_data->crew_quality = CREW_QUALITY_REGULAR;
    MOX::_combat_data->officer_idx = -1;
    MOX::_combat_data->marine_count = 0;
    MOX::_combat_data->structure_damage = 0;
    MOX::_combat_data->current_speed = 0;
    CMBTFIR2::Init_Internal_Space_(MOX::_combat_data);
    MOX::_combat_data->shield_max_hits = 0;
    MOX::_combat_data->shield_current_hits = 0;

    if (load_picture != 0) {
        COMBINIT::Load_Combat_Colony_Picture_(colony_idx);
    }
}

void __cdecl Load_Combat_Satelite_(int16_t station_type) {
    int16_t defender = MOX::_combat_defender_player;
    MOX::_combat_data[MOX::_combat_total_ship_count].computer_type = (int8_t)INITSHIP::Best_Computer_(defender);
    MOX::_combat_data[MOX::_combat_total_ship_count].armor_type = (int8_t)INITSHIP::Best_Armor_(defender);

    int16_t armor_absorption = TECHDATA::_armor[MOX::_combat_data[MOX::_combat_total_ship_count].armor_type].ships_bonus;
    int16_t space = 0;

    if (station_type == HULL_STAR_BASE) {
        strlcpy(MOX::_combat_data[MOX::_combat_total_ship_count].name, TECHDATA::_hull_data[HULL_STAR_BASE].name, sizeof(MOX::_combat_data[MOX::_combat_total_ship_count].name));
        MOX::_combat_data[MOX::_combat_total_ship_count].structure_max = TECHDATA::_hull_data[HULL_STAR_BASE].structure_hp;
        MOX::_combat_data[MOX::_combat_total_ship_count].armor_remaining = TECHDATA::_hull_data[HULL_STAR_BASE].armor_hp +
            (int16_t)(((int32_t)armor_absorption * TECHDATA::_hull_data[HULL_STAR_BASE].armor_hp) / 100);

        int16_t size = TECHDATA::_hull_data[HULL_STAR_BASE].size;
        if (MOX::_player[defender].tech_applications[TECH_APP_MEGAFLUXERS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            size = (size * 5) / 4;
        }
        int16_t half_size = size / 2;
        space = size - half_size;
        MOX::_combat_data[MOX::_combat_total_ship_count].size_class = SHIP_SIZE_LARGE;
        MOX::_combat_data[MOX::_combat_total_ship_count].picture_num = 40;
    } else if (station_type == HULL_BATTLESTATION) {
        strlcpy(MOX::_combat_data[MOX::_combat_total_ship_count].name, TECHDATA::_hull_data[HULL_BATTLESTATION].name, sizeof(MOX::_combat_data[MOX::_combat_total_ship_count].name));
        MOX::_combat_data[MOX::_combat_total_ship_count].structure_max = TECHDATA::_hull_data[HULL_BATTLESTATION].structure_hp;
        MOX::_combat_data[MOX::_combat_total_ship_count].armor_remaining = TECHDATA::_hull_data[HULL_BATTLESTATION].armor_hp +
            (int16_t)(((int32_t)armor_absorption * TECHDATA::_hull_data[HULL_BATTLESTATION].armor_hp) / 100);

        int16_t size = TECHDATA::_hull_data[HULL_BATTLESTATION].size;
        if (MOX::_player[defender].tech_applications[TECH_APP_MEGAFLUXERS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            size = (size * 5) / 4;
        }
        int16_t half_size = size / 2;
        space = size - half_size;
        MOX::_combat_data[MOX::_combat_total_ship_count].size_class = SHIP_SIZE_HUGE;
        MOX::_combat_data[MOX::_combat_total_ship_count].picture_num = 41;
    } else if (station_type == HULL_STAR_FORTRESS) {
        strlcpy(MOX::_combat_data[MOX::_combat_total_ship_count].name, TECHDATA::_hull_data[HULL_STAR_FORTRESS].name, sizeof(MOX::_combat_data[MOX::_combat_total_ship_count].name));
        MOX::_combat_data[MOX::_combat_total_ship_count].structure_max = TECHDATA::_hull_data[HULL_STAR_FORTRESS].structure_hp;
        MOX::_combat_data[MOX::_combat_total_ship_count].armor_remaining = TECHDATA::_hull_data[HULL_STAR_FORTRESS].armor_hp +
            (int16_t)(((int32_t)armor_absorption * TECHDATA::_hull_data[HULL_STAR_FORTRESS].armor_hp) / 100);

        int16_t size = TECHDATA::_hull_data[HULL_STAR_FORTRESS].size;
        if (MOX::_player[defender].tech_applications[TECH_APP_MEGAFLUXERS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            size = (size * 5) / 4;
        }
        int16_t half_size = size / 2;
        space = size - half_size;
        MOX::_combat_data[MOX::_combat_total_ship_count].size_class = SHIP_SIZE_TITAN;
        MOX::_combat_data[MOX::_combat_total_ship_count].picture_num = 42;
    }

    MOX::_combat_data[MOX::_combat_total_ship_count].owner = (int8_t)defender;
    MOX::_combat_data[MOX::_combat_total_ship_count].previous_owner = (uint8_t)defender;
    MOX::_combat_data[MOX::_combat_total_ship_count].ship_idx = -1;
    MOX::_combat_data[MOX::_combat_total_ship_count].ftl_type = DRIVE_NO_DRIVE;
    MOX::_combat_data[MOX::_combat_total_ship_count].shield_type = (int8_t)INITSHIP::Best_Ship_Shield_(defender);
    MOX::_combat_data[MOX::_combat_total_ship_count].unit_status = 0;

    for (int i = 0; i < 5; ++i) {
        MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags[i] = 0;
        MOX::_combat_data[MOX::_combat_total_ship_count].special_device_damage_flags[i] = 0;
    }

    int bit_count = 0;
    if (MOX::_player[defender].tech_applications[TECH_APP_HEAVY_ARMOR] == TECH_RESEARCH_STATUS_RESEARCHED) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_HEAVY_ARMOR);
        MOX::_combat_data[MOX::_combat_total_ship_count].armor_remaining *= 3;
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_REINFORCED_HULL] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_REINFORCED_HULL);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_SHIELD_CAPACITORS] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_SHIELD_CAPACITOR);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_HARD_SHIELDS] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_HARD_SHIELDS);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_MULTI_PHASED_SHIELDS] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_MULTI_PHASED_SHIELDS);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_WIDE_AREA_JAMMER] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_WIDE_AREA_JAMMER);
        bit_count++;
    } else if (MOX::_player[defender].tech_applications[TECH_APP_MULTI_WAVE_ECM_JAMMER] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_MULTI_WAVE_ECM_JAMMER);
        bit_count++;
    } else if (MOX::_player[defender].tech_applications[TECH_APP_ECM_JAMMER] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_ECM_JAMMER);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_WARP_DISSIPATER] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_WARP_DISSIPATOR);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_SECURITY_STATIONS] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_SECURITY_STATIONS);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_BATTLE_SCANNER] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_BATTLE_SCANNER);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_HIGH_ENERGY_FOCUS] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_HIGH_ENERGY_FOCUS);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_STRUCTURAL_ANALYZER] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_STRUCTURAL_ANALYZER);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_ACHILLES_TARGETING_UNIT] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_ACHILLES_TARGETING_UNIT);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_RANGEMASTER_UNIT] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_RANGEMASTER_UNIT);
        bit_count++;
    }
    if (MOX::_player[defender].tech_applications[TECH_APP_TRANSPORTERS] == TECH_RESEARCH_STATUS_RESEARCHED && bit_count < 8) {
        struct_::Set_Bit_Field_(MOX::_combat_data[MOX::_combat_total_ship_count].special_device_flags, SPECIAL_TRANSPORTERS);
        bit_count++;
    }

    MOX::_combat_data[MOX::_combat_total_ship_count].structure_max +=
        (int16_t)(((int32_t)MOX::_combat_data[MOX::_combat_total_ship_count].structure_max * armor_absorption) / 100);

    if (COMBINIT::Does_Combat_Ship_Have_Special_(MOX::_combat_total_ship_count, SPECIAL_REINFORCED_HULL)) {
        MOX::_combat_data[MOX::_combat_total_ship_count].structure_max *= 3;
    }

    for (int i = 0; i < 8; ++i) {
        struct_::Clear_Structure_(&MOX::_combat_data[MOX::_combat_total_ship_count].weapons[i], sizeof(MOX::_combat_data[MOX::_combat_total_ship_count].weapons[i]));
    }

    int beam_idx = 0;
    int16_t beam_id_1 = INITSHIP::Best_Beam_Weapon_(defender);
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].weapon_id = beam_id_1;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].active = 1;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = -1;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].specials = 2;
    int beam_space_1 = DESIGN::Weapon_Space_(defender, beam_id_1, 1, WEAPON_FIRING_ARC_ALL_SECTORS, 0, 2);
    int count1 = (space / 2) / beam_space_1;
    if (count1 < 1) count1 = 1;
    if (count1 > 99) count1 = 99;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].count = (uint8_t)count1;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].shots_left = (uint8_t)count1;
    beam_idx++;

    int16_t beam_id_2 = INITSHIP::Best_Beam_Weapon_(defender);
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].weapon_id = beam_id_2;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].active = 1;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = -1;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].specials = 0;
    int beam_space_2 = DESIGN::Weapon_Space_(defender, beam_id_2, 1, WEAPON_FIRING_ARC_ALL_SECTORS, 0, 0);
    int count2 = (space / 4) / beam_space_2;
    if (count2 < 1) count2 = 1;
    if (count2 > 198) count2 = 198;

    if (count2 > 99) {
        int stacks = (count2 - 1) / 99;
        count2 -= stacks * 99;
        for (int i = 0; i < stacks; ++i) {
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].count = 99;
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].shots_left = 99;
            beam_idx++;
            // Bug in original binary: potential buffer overflow into structure_max since beam_idx is not checked against 8 here
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].weapon_id = INITSHIP::Best_Beam_Weapon_(defender);
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].active = 1;
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = -1;
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].specials = 0;
        }
    }
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].count = (uint8_t)count2;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].shots_left = (uint8_t)count2;
    beam_idx++;

    int16_t pd_id = INITSHIP::Best_Point_Defense_Weapon_By_Size_(defender);
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].weapon_id = pd_id;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].active = 1;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = -1;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].specials = 4;
    int pd_space = DESIGN::Weapon_Space_(defender, pd_id, 1, WEAPON_FIRING_ARC_ALL_SECTORS, 0, 4);
    int pd_count = (space / 4) / pd_space;
    if (pd_count < 1) pd_count = 1;
    if (pd_count > 198) pd_count = 198;

    if (pd_count > 99) {
        int stacks = (pd_count - 1) / 99;
        pd_count -= stacks * 99;
        for (int i = 0; i < stacks; ++i) {
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].count = 99;
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].shots_left = 99;
            beam_idx++;
            // Bug in original binary: potential buffer overflow into structure_max since beam_idx is not checked against 8 here
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].weapon_id = INITSHIP::Best_Point_Defense_Weapon_By_Size_(defender);
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].active = 1;
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = -1;
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].specials = 4;
        }
    }
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].count = (uint8_t)pd_count;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].shots_left = (uint8_t)pd_count;
    beam_idx++;

    int16_t missile_id = INITSHIP::Best_Missile_Weapon_NT_(defender);
    int16_t miniaturization = DESIGN::Miniaturization_Level_(defender, TECHDATA::_weapons[missile_id].tech_app_id);
    uint16_t missile_specials = 0;
    if (TECHDATA::_weapon_modifications[WEAPON_MOD_FAST].level <= miniaturization) {
        missile_specials = 0x1000;
    }
    if (TECHDATA::_weapon_modifications[WEAPON_MOD_HEAVILY_ARMORED].level <= miniaturization) {
        missile_specials |= 0x800;
    }
    missile_specials |= 0x400;

    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].weapon_id = missile_id;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].active = 1;
    if (TECHDATA::_weapons[missile_id].type == WEAPON_TYPE_MISSILE) {
        MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = 20;
    } else {
        MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = 0;
    }
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].specials = missile_specials;

    int missile_space_calc = DESIGN::Weapon_Space_(defender, missile_id, 1, WEAPON_FIRING_ARC_ALL_SECTORS, 20, 0);
    int missile_count = space / missile_space_calc;
    if (missile_count < 1) missile_count = 1;
    if (missile_count > 90) missile_count = 90;

    while (missile_count > 30 && beam_idx < 8) {
        missile_count -= 30;
        MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].count = 30;
        MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].shots_left = 30;
        beam_idx++;
        // Bug in original binary: potential buffer overflow into structure_max
        MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].weapon_id = missile_id;
        MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].active = 1;
        if (TECHDATA::_weapons[missile_id].type == WEAPON_TYPE_MISSILE) {
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = 20;
        } else {
            MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].ammo = 0;
        }
        MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].arc = WEAPON_FIRING_ARC_ALL_SECTORS;
        MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].specials = missile_specials;
    }
    if (missile_count > 30) missile_count = 30;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].count = (uint8_t)missile_count;
    MOX::_combat_data[MOX::_combat_total_ship_count].weapons[beam_idx].shots_left = (uint8_t)missile_count;

    for (int i = 0; i < 8; ++i) {
        MOX::_combat_data[MOX::_combat_total_ship_count].weapon_ready_flags[i] = 1;
    }

    MOX::_combat_data[MOX::_combat_total_ship_count].crew_quality = CREW_QUALITY_REGULAR;
    MOX::_combat_data[MOX::_combat_total_ship_count].officer_idx = -1;
    MOX::_combat_data[MOX::_combat_total_ship_count].marine_count = (uint8_t)TECHDATA::_hull_data[station_type].marines;
    MOX::_combat_data[MOX::_combat_total_ship_count].current_speed = 0;
    MOX::_combat_data[MOX::_combat_total_ship_count].structure_damage = 0;
    MOX::_combat_data[MOX::_combat_total_ship_count].is_retreating = 0;

    CMBTFIR2::Init_Internal_Space_(&MOX::_combat_data[MOX::_combat_total_ship_count]);

    if (MOX::_combat_data[MOX::_combat_total_ship_count].shield_type > 0) {
        MOX::_combat_data[MOX::_combat_total_ship_count].shield_arc_max = Calc_Combat_Shield_Max_(MOX::_combat_total_ship_count);
    } else {
        MOX::_combat_data[MOX::_combat_total_ship_count].shield_arc_max = 0;
    }

    for (int i = 0; i < 4; ++i) {
        MOX::_combat_data[MOX::_combat_total_ship_count].shield_arc_current[i] = MOX::_combat_data[MOX::_combat_total_ship_count].shield_arc_max;
    }
}

void __cdecl Deploy_Ships_() {
    int16_t attacker_ship_count = 0;
    int16_t defender_ship_count = 0;
    int16_t stationary_ship_idx = -1;
    int16_t defender_deploy_x;
    int16_t attacker_deploy_x;
    int8_t attacker_facing;
    int8_t defender_facing;
    int8_t saved_size;
    int16_t attacker_deploy_y_down = 34;
    int16_t attacker_deploy_y_up = 34;
    int16_t defender_deploy_y_down = 34;
    int16_t defender_deploy_y_up = 34;

    if ((MOX::_PLAYER_NUM == MOX::_combat_attacker_player) || (MOX::_net_flag == 1)) {
        defender_deploy_x = 0x37 /* 55 */;
        attacker_deploy_x = 0x19 /* 25 */;
        MOX::_combat_data[0].x = 65;
        attacker_facing = 0;
        defender_facing = 8;
    } else {
        defender_deploy_x = 0x19 /* 25 */;
        attacker_deploy_x = 0x37 /* 55 */;
        MOX::_combat_data[0].x = 10;
        attacker_facing = 8;
        defender_facing = 0;
    }

    MOX::_combat_data[0].y = 34;
    MOX::_combat_data[0].size_class = 100;

    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
        if (MOX::_combat_data[i].ftl_type == DRIVE_NO_DRIVE) {
            saved_size = MOX::_combat_data[i].size_class;
            MOX::_combat_data[i].size_class = 99;
        }
    }

    qsort(MOX::_combat_data, MOX::_combat_total_ship_count, sizeof(*MOX::_combat_data), qsort_combat_data_by_size_);

    MOX::_combat_data[0].size_class = SHIP_SIZE_DOOM_STAR;

    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
        if (MOX::_combat_data[i].ftl_type == DRIVE_NO_DRIVE) {
            MOX::_combat_data[i].size_class = saved_size;
        }
    }

    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
        s_combat_data* ship = &MOX::_combat_data[i];
        if (ship->ftl_type == DRIVE_NO_DRIVE) {
            int8_t x_pos;
            if (defender_deploy_x < 40) {
                x_pos = MOX::_combat_data[0].x + 11;
            } else {
                x_pos = MOX::_combat_data[0].x - 4;
            }
            ship->x = x_pos;
            ship->y = MOX::_combat_data[0].y + 1;
            stationary_ship_idx = i;
        } else {
            if (ship->owner == MOX::_combat_attacker_player) {
                if (!(attacker_ship_count & 1)) {
                    ship->x = (int8_t)attacker_deploy_x;
                    ship->y = (int8_t)attacker_deploy_y_down;
                    int16_t offset = Ship_Size_Offset_(ship->size_class);
                    attacker_deploy_y_down += (int16_t)(offset + 1);
                    if (attacker_deploy_y_down > 64) {
                        if ((MOX::_PLAYER_NUM == MOX::_combat_attacker_player) || (MOX::_net_flag != 0)) {
                            attacker_deploy_x -= 3;
                        } else {
                            attacker_deploy_x += 3;
                        }
                        attacker_deploy_y_up = 34;
                        attacker_deploy_y_down = 34;
                    }
                } else {
                    if (attacker_deploy_y_up < 4) {
                        if ((MOX::_PLAYER_NUM == MOX::_combat_attacker_player) || (MOX::_net_flag != 0)) {
                            attacker_deploy_x -= 3;
                        } else {
                            attacker_deploy_x += 3;
                        }
                        attacker_deploy_y_up = 34;
                        attacker_deploy_y_down = 34;
                    }
                    int16_t offset = Ship_Size_Offset_(ship->size_class);
                    attacker_deploy_y_up -= (int16_t)(offset + 1);
                    ship->x = (int8_t)attacker_deploy_x;
                    ship->y = (int8_t)attacker_deploy_y_up;
                }
                attacker_ship_count++;
                ship->facing_dir = attacker_facing;
            } else {
                if (!(defender_ship_count & 1)) {
                    ship->x = (int8_t)defender_deploy_x;
                    ship->y = (int8_t)defender_deploy_y_down;
                    while (true) {
                        int16_t offset = Ship_Size_Offset_(ship->size_class);
                        defender_deploy_y_down += (int16_t)(offset + 1);
                        if (defender_deploy_y_down > 64) {
                            if ((MOX::_PLAYER_NUM == MOX::_combat_attacker_player) || (MOX::_net_flag != 0)) {
                                defender_deploy_x += 3;
                            } else {
                                defender_deploy_x -= 3;
                            }
                            defender_deploy_y_up = 34;
                            defender_deploy_y_down = 34;
                        }
                        if (Ship_Can_Deploy_At_(defender_deploy_x, defender_deploy_y_down, stationary_ship_idx)) {
                            break;
                        }
                    }
                } else {
                    while (true) {
                        if (defender_deploy_y_up < 4) {
                            if ((MOX::_PLAYER_NUM == MOX::_combat_attacker_player) || (MOX::_net_flag != 0)) {
                                defender_deploy_x += 3;
                            } else {
                                defender_deploy_x -= 3;
                            }
                            defender_deploy_y_up = 34;
                            defender_deploy_y_down = 34;
                        }
                        int16_t offset = Ship_Size_Offset_(ship->size_class);
                        defender_deploy_y_up -= (int16_t)(offset + 1);
                        if (Ship_Can_Deploy_At_(defender_deploy_x, defender_deploy_y_down, stationary_ship_idx)) {
                            break;
                        }
                    }
                    ship->x = (int8_t)defender_deploy_x;
                    ship->y = (int8_t)defender_deploy_y_up;
                }
                defender_ship_count++;
                ship->facing_dir = defender_facing;
            }
        }
    }
}

void __cdecl Snap_Center_Combat_Screen_(int16_t target_x, int16_t target_y) {
    int16_t new_combat_x;
    int16_t new_combat_y;
    int16_t delta_x;
    int16_t delta_y;
    int16_t near_x;
    int16_t mid_x;
    int16_t far_x;

    new_combat_x = target_x - 16;
    new_combat_y = target_y - 9;

    if (new_combat_x < 0) {
        new_combat_x = 0;
    }
    if (new_combat_x > 0x31) {
        new_combat_x = 0x31;
    }

    if (new_combat_y < 0) {
        new_combat_y = 0;
    }
    if (new_combat_y > 0x32) {
        new_combat_y = 0x32;
    }

    delta_x = MOX::_cur_combat_x - new_combat_x;
    delta_y = MOX::_cur_combat_y - new_combat_y;

    near_x = MOX::_combat_near_x + (delta_x * -160);
    MOX::_combat_near_y += (delta_y * -160);

    mid_x = MOX::_combat_mid_x + (delta_x * -80);
    MOX::_combat_mid_y += (delta_y * -80);

    far_x = MOX::_combat_far_x + (delta_x * -20);
    MOX::_combat_far_y += (delta_y * -20);

    // Normalize negative coordinates for wrapping
    if (near_x < 0) {
        near_x += ((0x27F - near_x) / 0x280) * 0x280;
    }
    if (MOX::_combat_near_y < 0) {
        MOX::_combat_near_y += ((0x1DF - MOX::_combat_near_y) / 0x1E0) * 0x1E0;
    }

    if (mid_x < 0) {
        mid_x += ((0x27F - mid_x) / 0x280) * 0x280;
    }
    if (MOX::_combat_mid_y < 0) {
        MOX::_combat_mid_y += ((0x1DF - MOX::_combat_mid_y) / 0x1E0) * 0x1E0;
    }

    if (far_x < 0) {
        far_x += ((0x27F - far_x) / 0x280) * 0x280;
    }
    if (MOX::_combat_far_y < 0) {
        MOX::_combat_far_y += ((0x1DF - MOX::_combat_far_y) / 0x1E0) * 0x1E0;
    }

    // Apply wrapping within screen resolution (640x480)
    MOX::_combat_near_x = near_x % 0x280;
    MOX::_combat_near_y = MOX::_combat_near_y % 0x1E0;
    MOX::_combat_mid_x = mid_x % 0x280;
    MOX::_combat_mid_y = MOX::_combat_mid_y % 0x1E0;
    MOX::_combat_far_x = far_x % 0x280;
    MOX::_combat_far_y = MOX::_combat_far_y % 0x1E0;

    MOX::_cur_combat_x = new_combat_x;
    MOX::_cur_combat_y = new_combat_y;

    COMBAT1::Add_Tactical_Combat_Fields_();
    COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);
    CMBTDRW1::Refresh_Combat_Screen_Full_();
}

void __cdecl Recharge_Shields_() {
    if (MOX::_combat_total_ship_count <= 1) {
        return;
    }

    for (int16_t ship_idx = 1; ship_idx < MOX::_combat_total_ship_count; ++ship_idx) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        if (ship->shield_type > 0 &&
            CMBTMOV1::Ship_Is_In_Stasis_(ship_idx) == 0 &&
            ship->shield_current_hits > 0) {

            int32_t recharge_rate = 3;
            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_SHIELD_CAPACITOR) != 0) {
                recharge_rate = 10;
            }

            int32_t recharge_pool = (ship->shield_arc_max * recharge_rate) / 10;

            if (recharge_pool > 0) {
                bool changed = true;
                while (recharge_pool > 0 && changed) {
                    changed = false;
                    for (int16_t arc = 0; arc < 4; ++arc) {
                        if (ship->shield_arc_current[arc] < ship->shield_arc_max) {
                            if (recharge_pool > 0) {
                                recharge_pool--;
                                changed = true;
                                ship->shield_arc_current[arc]++;
                            }
                        }
                    }
                }
            }
        }
    }
}

void __cdecl Increment_Bhg_(int16_t player_idx) {
    if (MOX::_combat_total_ship_count > 1) {
        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
            s_combat_data& ship = MOX::_combat_data[i];

            if (ship.unit_status == 0 && ship.owner == (int8_t)player_idx) {
                if (CMBTMOV1::Ship_Is_In_Stasis_(i) == 0 && CMBTMOV1::Ship_In_Black_Hole_(i)) {
                    ship.black_hole_flag++;

                    if (ship.black_hole_flag == 3) {
                        int16_t x_pos = (int16_t)ship.x * 20;
                        int16_t y_pos = (int16_t)ship.y * 20;
                        CMBTFIRE::Destroy_Ship_(i, x_pos, y_pos, 1);
                    }
                }
            }
        }
    }
}

void __cdecl End_Of_Turn_Bookeeping_() {
    Recharge_Shields_();
    Repair_All_Combat_Ships_();
    COMBAT1::Update_Phasing_Cloak_Timers_();

    int16_t turn = MOX::_combat_turn;
    if (MOX::_combat_data->unit_status == 0 && (int16_t)(turn % 10) == 0) {
        for (int16_t i = 0; i < 8; ++i) {
            s_combat_weapon_slot* slot = &MOX::_combat_data->weapons[i];
            uint16_t weapon_id = slot->weapon_id;

            if (TECHDATA::_weapons[weapon_id].type == WEAPON_TYPE_FIGHTER) {
                slot->ammo = 1;
            }
        }
    }
}

void __cdecl Scroll_Combat_Screen_() {
    int16_t scroll_timer = 0;
    int16_t next_combat_x;
    int16_t next_combat_y;
    int16_t scroll_speed;
    int16_t old_combat_x;
    int16_t old_combat_y;
    int16_t dir_x;
    int16_t dir_y;
    int16_t diff_x;
    int16_t diff_y;

    while (true) {
        if (mouse::Pointer_X_() >= 5 &&
            mouse::Pointer_X_() <= 0x27B /* 635 */ &&
            mouse::Pointer_Y_() >= 5 &&
            mouse::Pointer_Y_() <= 0x1DB /* 475 */) {
            COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);
            COMBAT1::Add_Tactical_Combat_Fields_();
            CMBTDRW1::Refresh_Combat_Screen_Full_();
            MOX::_last_ship_status = -1;
            return;
        }

        old_combat_x = MOX::_cur_combat_x;
        old_combat_y = MOX::_cur_combat_y;
        next_combat_x = old_combat_x;
        next_combat_y = old_combat_y;

        scroll_timer++;
        if (scroll_timer > 4) {
            scroll_speed = 3;
        } else if (scroll_timer > 2) {
            scroll_speed = 2;
        } else {
            scroll_speed = 1;
        }

        if (mouse::Pointer_X_() < 5) {
            next_combat_x = old_combat_x - scroll_speed;
        } else if (mouse::Pointer_X_() > 0x27B /* 635 */) {
            next_combat_x = old_combat_x + scroll_speed;
        }

        if (mouse::Pointer_Y_() < 5) {
            next_combat_y = old_combat_y - scroll_speed;
        } else if (mouse::Pointer_Y_() > 0x1DB /* 475 */) {
            next_combat_y = old_combat_y + scroll_speed;
        }

        if (next_combat_x < 0) {
            next_combat_x = 0;
        } else if (next_combat_x > 0x31 /* 49 */) {
            next_combat_x = 0x31;
        }

        if (next_combat_y < 0) {
            next_combat_y = 0;
        } else if (next_combat_y > 0x32 /* 50 */) {
            next_combat_y = 0x32;
        }

        diff_x = (int16_t)(old_combat_x - next_combat_x);
        if (diff_x < 0) {
            dir_x = 1;
        } else {
            dir_x = (diff_x > 0) ? -1 : 0;
        }

        diff_y = (int16_t)(old_combat_y - next_combat_y);
        if (diff_y < 0) {
            dir_y = 1;
        } else {
            dir_y = (diff_y > 0) ? -1 : 0;
        }

        if (next_combat_x != old_combat_x) {
            MOX::_combat_near_x += dir_x * -0x50; // -80
            MOX::_combat_mid_x += dir_x * -0x28;  // -40
            MOX::_combat_far_x += (int16_t)(((int32_t)dir_x * -20) / 2); // -10
        }

        if (next_combat_y != old_combat_y) {
            MOX::_combat_near_y += dir_y * -0x50; // -80
            MOX::_combat_mid_y += dir_y * -0x28;  // -40
            MOX::_combat_far_y += (int16_t)(((int32_t)dir_y * -20) / 2); // -10
        }

        if (MOX::_combat_near_x < 0) MOX::_combat_near_x += 0x280; // 640
        if (MOX::_combat_near_y < 0) MOX::_combat_near_y += 0x1E0; // 480
        if (MOX::_combat_mid_x < 0) MOX::_combat_mid_x += 0x280;
        if (MOX::_combat_mid_y < 0) MOX::_combat_mid_y += 0x1E0;
        if (MOX::_combat_far_x < 0) MOX::_combat_far_x += 0x280;
        if (MOX::_combat_far_y < 0) MOX::_combat_far_y += 0x1E0;

        MOX::_combat_near_x %= 0x280;
        MOX::_combat_near_y %= 0x1E0;
        MOX::_combat_mid_x %= 0x280;
        MOX::_combat_mid_y %= 0x1E0;
        MOX::_combat_far_x %= 0x280;
        MOX::_combat_far_y %= 0x1E0;

        MOX::_cur_combat_x = next_combat_x;
        MOX::_cur_combat_y = next_combat_y;

        video::Set_Page_Off_();
        CMBTDRW1::Scroll_Draw_Combat_Screen_();
        RUSS::Mox_Sync_Update_();
    }
}

void __cdecl Load_Individual_Ship_Pictures_(int16_t player_idx) {
    uint8_t attacker_palette[32];
    uint8_t defender_palette[32];
    s_animation_header* ship_cache[45];

    for (int i = 0; i < 32; ++i) {
        attacker_palette[i] = (uint8_t)i + 0x20;
        defender_palette[i] = (uint8_t)i + 0x60;
    }

    for (int i = 0; i < 45; ++i) {
        ship_cache[i] = nullptr;
    }

    if (player_idx < MAX_PLAYERS) {
        for (int i = 0; i < 64; ++i) {
            MOX::_fighter_seg[player_idx][i] = (s_animation_header*)farload::Far_Reload_Next_("cmbtfgtr.lbx", i, MOX::_screen_seg);
            RUSS::Mox_Update_();
        }

        if (MOX::_combat_attacker_player == player_idx) {
            for (int i = 0; i < 64; ++i) {
                replace::Replace_Color_Range_(MOX::_fighter_seg[player_idx][i], attacker_palette, 0x50, 0x10);
                RUSS::Mox_Update_();
            }
        }
        else {
            for (int i = 0; i < 64; ++i) {
                replace::Replace_Color_Range_(MOX::_fighter_seg[player_idx][i], defender_palette, 0x50, 0x10);
                RUSS::Mox_Update_();
            }
        }
    }
    else if (player_idx == MAX_PLAYERS) {
        for (int i = 0; i < 16; ++i) {
            MOX::_fighter_seg[MAX_PLAYERS][i] = (s_animation_header*)farload::Far_Reload_Next_("cmbtfgtr.lbx", i + 0x40, MOX::_screen_seg);
            RUSS::Mox_Update_();
        }

        if (MOX::_combat_attacker_player == MAX_PLAYERS) {
            for (int i = 0; i < 16; ++i) {
                replace::Replace_Color_Range_(MOX::_fighter_seg[MAX_PLAYERS][i], attacker_palette, 0x50, 0x10);
                RUSS::Mox_Update_();
            }
        }
        else {
            for (int i = 0; i < 16; ++i) {
                replace::Replace_Color_Range_(MOX::_fighter_seg[MAX_PLAYERS][i], defender_palette, 0x50, 0x10);
                RUSS::Mox_Update_();
            }
        }
    }

    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
        s_combat_data* ship = &MOX::_combat_data[i];
        if (ship->owner == player_idx) {
            char lbx_filename[16];
            int16_t lbx_index;

            if ((uint8_t)ship->previous_owner < MAX_PLAYERS && ship->picture_num != 44) {
                strlcpy(lbx_filename, "cmbtshp.lbx", sizeof(lbx_filename));
                lbx_index = (int16_t)MOX::_player[player_idx].color * 45 + ship->picture_num;
            }
            else {
                strlcpy(lbx_filename, "monster.lbx", sizeof(lbx_filename));
                lbx_index = (ship->picture_num == 44) ? 0x19 : ship->picture_num;
            }

            if (ship_cache[ship->picture_num] == nullptr) {
                if (allocate::Get_Remaining_Block_Space_(MOX::_screen_seg) <= 200000 /* 0x30D40 */) {
                    ship->ship_image_seg = nullptr;
                }
                else {
                    s_animation_header* anim = (s_animation_header*)farload::Far_Reload_Next_(lbx_filename, (int)lbx_index, MOX::_screen_seg);
                    ship_cache[ship->picture_num] = anim;
                    RUSS::Mox_Update_();

                    if (MOX::_combat_defender_player == player_idx) {
                        replace::Replace_Color_Range_(anim, defender_palette, 0x20, 0x20);
                    }
                    RUSS::Mox_Update_();
                    ship->ship_image_seg = anim;
                }
            }
            else {
                ship->ship_image_seg = ship_cache[ship->picture_num];
            }
        }
    }
}

void __cdecl Repair_All_Combat_Ships_() {
    int16_t attacker_bonus;
    int16_t defender_bonus;
    int16_t repair_amount;

    attacker_bonus = CMBTFIR2::Get_Fleet_Engineer_Bonus_(MOX::_combat_attacker_player);
    defender_bonus = CMBTFIR2::Get_Fleet_Engineer_Bonus_(MOX::_combat_defender_player);

    if (MOX::_combat_attacker_player < MAX_PLAYERS) {
        if (MOX::_player[MOX::_combat_attacker_player].traits[TRAIT_CYBERNETIC] != 0) {
            attacker_bonus += 10;
        }
    }

    if (MOX::_combat_defender_player < MAX_PLAYERS) {
        if (MOX::_player[MOX::_combat_defender_player].traits[TRAIT_CYBERNETIC] != 0) {
            defender_bonus += 10;
        }
    }

    for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
        repair_amount = 0;

        if (MOX::_combat_data[i].unit_status == 0 &&
            MOX::_combat_data[i].is_captured == 0 &&
            !CMBTMOV1::Ship_Is_In_Stasis_(i))
        {
            if (COMBINIT::Does_Combat_Ship_Have_Special_(i, SPECIAL_AUTOMATED_REPAIR_UNIT) != 0) {
                repair_amount = 20;
            }

            if (MOX::_combat_data[i].owner == MOX::_combat_attacker_player) {
                repair_amount += attacker_bonus;
            } else {
                repair_amount += defender_bonus;
            }

            if (repair_amount > 0) {
                CMBTFIR2::Repair_Combat_Ship_(i, repair_amount);
            }
        }
    }
}

void __cdecl End_Of_Combat_(int16_t star_idx, int16_t colony_pop_lost, int16_t combat_winner) {
    int16_t leader_count = 0;
    int16_t defender_losses = 0;
    int16_t attacker_losses = 0;
    int32_t defender_power_lost = 0;
    int32_t attacker_power_lost = 0;
    int16_t leaders[8];

    int16_t victor_player;
    if (combat_winner < 0) {
        victor_player = MOX::_combat_defender_player;
    } else if (combat_winner == 666) {
        victor_player = -1;
    } else {
        victor_player = MOX::_combat_attacker_player;
    }

    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
        COMBINIT::Init_Special_Devices_(i);
        s_combat_data* ship = &MOX::_combat_data[i];

        if (ship->ftl_type == DRIVE_NO_DRIVE && (ship->unit_status == 5 || ship->owner != MOX::_combat_defender_player)) {
            if (star_idx != MOX::_NUM_STARS) {
                s_colony* col = &MOX::_colony[MOX::_cmbt_colony_id];
                int16_t satellite_building = -1;
                if (col->buildings[BUILDING_STAR_BASE] != 0) {
                    satellite_building = BUILDING_STAR_BASE;
                } else if (col->buildings[BUILDING_BATTLESTATION] != 0) {
                    satellite_building = BUILDING_BATTLESTATION;
                } else if (col->buildings[BUILDING_STAR_FORTRESS] != 0) {
                    satellite_building = BUILDING_STAR_FORTRESS;
                }

                if (satellite_building != -1) {
                    if (Satellite_Was_Captured_By_Attacker_(
                            ship->owner, victor_player, MOX::_combat_attacker_player)) {
                        Record_Captured_Satellite_(MOX::_cmbt_colony_id, satellite_building);
                    }
                    ERICNET::Remove_Building_(MOX::_cmbt_colony_id, satellite_building);
                }
            }
            continue;
        }

        if ((ship->unit_status == 5 && ship->is_retreating != 2) ||
            (ship->is_captured != 0 && ship->owner != victor_player) ||
            (ship->owner >= MAX_PLAYERS && ship->previous_owner != ship->owner)) {
            
            ship->unit_status = 5;

            if (MOX::_combat_attacker_player == ship->owner) {
                attacker_losses += (ship->size_class + 1);
                attacker_power_lost += (1 << (ship->size_class + 1));
            } else {
                defender_losses += (ship->size_class + 1);
                defender_power_lost += (1 << (ship->size_class + 1));
            }

            if (ship->ship_idx != -1 && ship->officer_idx > -1) {
                int16_t officer_idx = ship->officer_idx;
                if (MOX::_leaders[officer_idx].eta > 0) {
                    OFFICER::Move_Officer_To_Pool_(officer_idx);
                } else {
                    if (MOX::_player[ship->owner].tech_applications[TECH_APP_SURVIVAL_PODS] == TECH_RESEARCH_STATUS_RESEARCHED) {
                        leaders[leader_count] = officer_idx;
                        leader_count++;
                        MOX::_ship[ship->ship_idx].officer_index = -1;
                    } else {
                        OFFICER::Kill_Officer_(officer_idx);
                    }
                }
            }
        } else {
            if (COMBINIT::Does_Combat_Ship_Have_Special_(i, SPECIAL_AUTOMATED_REPAIR_UNIT) == 1 ||
                CMBTFIR2::Get_Fleet_Engineer_Bonus_(ship->owner) > 0 ||
                (ship->owner < MAX_PLAYERS && (MOX::_player[ship->owner].tech_applications[TECH_APP_ADVANCED_DAMAGE_CONTROL] == TECH_RESEARCH_STATUS_RESEARCHED ||
                                     MOX::_player[ship->owner].traits[TRAIT_CYBERNETIC] != 0))) {
                
                CMBTFIR2::Clear_Internal_Space_(ship);
                for (int16_t j = 0; j < 5; ++j) {
                    ship->special_device_damage_flags[j] = 0;
                }
                ship->structure_damage = 0;
                if (ship->ship_idx != -1) {
                    ship->armor_remaining = INITSHIP::Get_Ship_Armor_Hits_(ship->ship_idx);
                }
            }
            COMBINIT::Unload_Combat_Ship_(i);
        }
    }

    if (MOX::_combat_attacker_player < MAX_PLAYERS) {
        if (MOX::_bio_weapon_flag != 0) {
            COMBINIT::Apply_Bio_Weapon_Penalty_(MOX::_combat_attacker_player, star_idx, colony_pop_lost);
        }
        if (MOX::_combat_defender_player < MAX_PLAYERS) {
            MOX::_player[MOX::_combat_attacker_player].ship_war_losses[MOX::_combat_defender_player] += (int16_t)attacker_power_lost;
            MOX::_player[MOX::_combat_defender_player].ship_war_losses[MOX::_combat_attacker_player] += (int16_t)defender_power_lost;
            
            if (defender_losses <= 0) {
                defender_losses = 1;
            }
            
            if (COMBINIT::_real_attack_flag != 0) {
                if (colony_pop_lost > 0) {
                    NETMOX::Russ_Change_Relations_(-10000, MOX::_combat_attacker_player, MOX::_combat_defender_player, 10, star_idx, colony_pop_lost);
                }
                NETMOX::Russ_Change_Relations_(-15 * defender_losses, MOX::_combat_attacker_player, MOX::_combat_defender_player, 11, star_idx, 0);
            }
        }
    }

    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
        s_combat_data* ship = &MOX::_combat_data[i];
        if ((ship->unit_status == 0 || ship->is_retreating == 2) &&
            ship->officer_idx != -1 &&
            ship->owner == victor_player &&
            ship->ship_idx != -1) {
            
            int16_t xp = (victor_player == MOX::_combat_attacker_player) ? defender_losses : attacker_losses;
            xp /= 2;
            if (xp < 1) {
                xp = 1;
            }
            OFFICER::Give_Officer_XP_(victor_player, ship->officer_idx, xp);
        }
    }

    if (leader_count > 0) {
        for (int16_t j = 0; j < leader_count; ++j) {
            int16_t ship_idx_found = 0;
            int16_t ship_assigned = 0;
            int16_t leader_id = leaders[j];

            for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
                if (ship_idx_found != 0) {
                    break;
                }
                s_combat_data* ship = &MOX::_combat_data[i];
                if ((ship->unit_status != 5 || ship->is_retreating == 2) && ship->ship_idx != -1) {
                    if (MOX::_leaders[leader_id].player_index == ship->owner) {
                        if (MOX::_ship[ship->ship_idx].officer_index == -1) {
                            ship_idx_found = 1;
                            MOX::_ship[ship->ship_idx].officer_index = leader_id;
                            ship->officer_idx = leader_id;
                            MOX::_ship[MOX::_leaders[leader_id].location].officer_index = -1;
                            MOX::_leaders[leader_id].location = ship->ship_idx;
                        }
                        ship_assigned = 1;
                    }
                }
            }

            if (ship_idx_found != 0 || ship_assigned != 0) {
                if (MOX::_leaders[leader_id].player_index == victor_player) {
                    int16_t xp = (victor_player == MOX::_combat_attacker_player) ? defender_losses : attacker_losses;
                    xp /= 2;
                    if (xp < 1) {
                        xp = 1;
                    }
                    OFFICER::Give_Officer_XP_(victor_player, leader_id, xp);
                }
            }

            if (ship_idx_found == 0) {
                if (ship_assigned == 1) {
                    OFFICER::Move_Officer_To_Pool_(leader_id);
                } else {
                    OFFICER::Kill_Officer_(leader_id);
                }
            }
        }
    }

    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
        s_combat_data* ship = &MOX::_combat_data[i];
        
        if (star_idx == MOX::_NUM_STARS && ship->owner == MAX_PLAYERS &&
            ship->unit_status == 5 && ship->ftl_type != 0) {
            antarans::Kill_Antaran_Defensive_Ship_(ship->size_class);
        } 
        else if (ship->unit_status == 5 && ship->ship_idx != -1 && ship->is_retreating != 2) {
            if (ship->owner == MAX_PLAYERS) {
                antarans::Kill_Antaran_Offensive_Ship_(ship->size_class);
            }
            if (MOX::_ship[ship->ship_idx].officer_index > -1) {
                OFFICER::Move_Officer_To_Pool_(MOX::_ship[ship->ship_idx].officer_index);
            }
            SHIPS::Kill_Ship_(ship->ship_idx);
        } 
        else if (ship->owner == victor_player && ship->ship_idx != -1) {
            int16_t xp = (victor_player == MOX::_combat_attacker_player) ? defender_losses : attacker_losses;
            xp /= 2;
            if (xp < 1) {
                xp = 1;
            }
            MOX::_ship[ship->ship_idx].crew_experience += xp;
        }
    }

    if (MOX::_cmbt_colony_id != -1) {
        s_colony* col = &MOX::_colony[MOX::_cmbt_colony_id];
        if (col->outpost_flag == 0 && col->n_pops == 0) {
            int16_t has_buildings = 0;
            for (int16_t i = 0; i < BUILDING_COUNT; ++i) {
                if (has_buildings != 0) {
                    break;
                }
                has_buildings += col->buildings[i];
            }
            if (has_buildings == 0) {
                BOMB::Destroy_Colony_(col, 1);
            }
        }
    }
}

void __cdecl Unload_Combat_Ship_(int16_t combat_ship_idx) {
    int16_t ship_idx = MOX::_combat_data[combat_ship_idx].ship_idx;

    if (ship_idx != -1) {
        while (MOX::_combat_data[combat_ship_idx].current_speed < 1) {
            MOX::_combat_data[combat_ship_idx].drive_current_hits++;
            MOX::_combat_data[combat_ship_idx].current_speed = COMBAT1::Calc_Current_Speed_(combat_ship_idx);
        }

        if (MOX::_combat_data[combat_ship_idx].drive_max_hits < MOX::_combat_data[combat_ship_idx].drive_current_hits) {
            MOX::_combat_data[combat_ship_idx].drive_current_hits = MOX::_combat_data[combat_ship_idx].drive_max_hits;
        }

        MOX::_ship[ship_idx].owner = MOX::_combat_data[combat_ship_idx].owner;

        for (int16_t i = 0; i < 8; i++) {
            MOX::_ship[ship_idx].d.ship_weapon[i].current_count = MOX::_combat_data[combat_ship_idx].weapons[i].count;
        }

        for (int16_t i = 0; i < 5; i++) {
            MOX::_ship[ship_idx].special_device_damage_flags[i] = MOX::_combat_data[combat_ship_idx].special_device_damage_flags[i];
        }

        MOX::_ship[ship_idx].structural_damage = MOX::_combat_data[combat_ship_idx].structure_damage;
        MOX::_ship[ship_idx].armor_damage = INITSHIP::Get_Ship_Armor_Hits_(ship_idx) - MOX::_combat_data[combat_ship_idx].armor_remaining;
        MOX::_ship[ship_idx].computer_damage =
            MOX::_combat_data[combat_ship_idx].computer_max_hits -
            MOX::_combat_data[combat_ship_idx].computer_current_hits;

        if (MOX::_combat_data[combat_ship_idx].shield_max_hits > 0) {
            MOX::_ship[ship_idx].shield_damage_percent =
                Damage_Percent_From_Hits_(
                    MOX::_combat_data[combat_ship_idx].shield_max_hits,
                    MOX::_combat_data[combat_ship_idx].shield_current_hits);
        }

        if (MOX::_combat_data[combat_ship_idx].drive_max_hits > 0) {
            MOX::_ship[ship_idx].drive_damage_percent =
                Damage_Percent_From_Hits_(
                    MOX::_combat_data[combat_ship_idx].drive_max_hits,
                    MOX::_combat_data[combat_ship_idx].drive_current_hits);
        }

        for (int16_t i = 0; i < 8; ++i) {
            int16_t weapon_id = MOX::_combat_data[combat_ship_idx].weapons[i].weapon_id;
            if (TECHDATA::_weapons[weapon_id].type == WEAPON_TYPE_FIGHTER) {
                MOX::_ship[ship_idx].d.ship_weapon[i].current_count =
                    Surviving_Weapon_Count_From_Hits_(
                        MOX::_combat_data[combat_ship_idx].weapon_hits[i],
                        MOX::_combat_data[combat_ship_idx].weapon_hits_per_unit[i]);
            }
        }
    }
}

void __cdecl Plasma_Web_Damage_(int16_t owner_idx) {
    if (MOX::_combat_total_ship_count <= 0) {
        return;
    }

    for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
        s_combat_data* ship = &MOX::_combat_data[i];

        if (ship->unit_status != 0) {
            continue;
        }

        if (ship->plasma_web_damage <= 0) {
            continue;
        }

        if (ship->owner != owner_idx) {
            continue;
        }

        if (CMBTMOV1::Ship_Is_In_Stasis_(i) != 0) {
            continue;
        }

        int16_t total_damage = 0;
        int16_t struct_damage_total = 0;

        for (int16_t arc = 0; arc < 4; ++arc) {
            int16_t arc_damage = ship->plasma_web_damage;
            int16_t arc_struct_damage = 0;

            CMBTFIRE::Apply_Ship_Damage_(
                i,
                &arc_damage,
                &arc_struct_damage,
                arc,
                0,
                0,
                0,
                0,
                0
            );

            total_damage += arc_damage;
            struct_damage_total += arc_struct_damage;
        }

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            if (CMBTDRW1::Ship_Completely_On_Screen_(i) == 0) {
                COMBINIT::Snap_Center_Combat_Screen_(ship->x, ship->y);
            }

            BEAMS::Add_Damage_Indicator_For_Ship_(
                (ship->x - MOX::_cur_combat_x) * 20,
                (ship->y - MOX::_cur_combat_y) * 20,
                i,
                1,
                total_damage,
                struct_damage_total
            );

            BEAMS::Draw_Damage_Message_Queue_Until_Done_();
        }

        int16_t cause = CMBTFIRE::Is_Combat_Ship_Dead_(i);
        if (cause != 0) {
            CMBTFIRE::Destroy_Ship_(i, ship->x * 20, ship->y * 20, cause);
        } else {
            int16_t new_plasma_damage = ship->plasma_web_damage - 5;
            if (new_plasma_damage < 0) {
                new_plasma_damage = 0;
            }
            ship->plasma_web_damage = new_plasma_damage;
        }
    }
}

void __cdecl Apply_Bio_Weapon_Penalty_(int16_t player_idx, int16_t star_idx, int16_t colony_pop_lost) {
    if (player_idx < MAX_PLAYERS) {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives != PLAYER_OBJECTIVE_HUMAN && i != player_idx) {
                int16_t change = -(int16_t)game_random::Random_(10);
                NETMOX::Russ_Change_Relations_(change, player_idx, i, 6, star_idx, colony_pop_lost);
            }
        }
    }
}

void __cdecl Ship_Center_Offsets_(int16_t ship_idx, int16_t* out_x, int16_t* out_y) {
    int16_t result_offset = 0;

    if (ship_idx == 0) {
        if (MOX::_cmbt_colony_id == -1) {
            result_offset = 60;
        } else {
            int16_t planet_idx = MOX::_colony[MOX::_cmbt_colony_id].planet;
            int8_t planet_size = MOX::_planet[planet_idx].size;

            switch (planet_size) {
                case PLANET_SIZE_TINY:
                    result_offset = 30;
                    break;
                case PLANET_SIZE_SMALL:
                    result_offset = 40;
                    break;
                case PLANET_SIZE_MEDIUM:
                    result_offset = 60;
                    break;
                case PLANET_SIZE_LARGE:
                    result_offset = 70;
                    break;
                case PLANET_SIZE_HUGE:
                    result_offset = 80;
                    break;
                default:
                    result_offset = 0;
                    break;
            }
        }
    } else {
        int8_t size_class = MOX::_combat_data[ship_idx].size_class;

        switch (size_class) {
            case SHIP_SIZE_SMALL:
                result_offset = 10;
                break;
            case SHIP_SIZE_MEDIUM:
            case SHIP_SIZE_LARGE:
                result_offset = 20;
                break;
            case SHIP_SIZE_HUGE:
            case SHIP_SIZE_TITAN:
            case SHIP_SIZE_DOOM_STAR:
                result_offset = 30;
                break;
            default:
                result_offset = 0;
                break;
        }
    }

    *out_x = result_offset;
    *out_y = result_offset;
}

}