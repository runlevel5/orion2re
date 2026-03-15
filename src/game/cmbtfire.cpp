#include "pch.h"

namespace CMBTFIRE {
    // Suggested name: Does_Combat_Planet_Have_Defenses
    int16_t __cdecl Does_Combat_Planet_Have_Defenses_() {
        if (MOX::_cmbt_colony_id != -1) {
            // Pack colony defense buildings into a bitfield:
            // bit 3: Fighter Garrison
            // bit 2: Stellar Converter
            // bit 1: Ground Batteries
            // bit 0: Missile Base
            return (((int16_t)MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_FIGHTER_GARRISON] * 2 |
                     (int16_t)MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_STELLAR_CONVERTER]) * 2 |
                    (int16_t)MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_GROUND_BATTERIES]) * 2 |
                    (int16_t)MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_MISSILE_BASE];
        }
        return 0;
    }

    int16_t __cdecl Is_Combat_Ship_Dead_(int16_t ship_idx) {
        int16_t result = 0;

        if (ship_idx != 0) {
            s_combat_data* ship = &MOX::_combat_data[ship_idx];

            if (ship->structure_damage >= ship->structure_max) {
                result = 1;
            }
            else if (ship->drive_current_hits == 0 && ship->owner <= 9 && ship->ftl_type > 0) {
                result = 3;
            }

            if (result != 0) {
                if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x18 /* SPECIAL_STASIS_FIELD */)) {
                    result = 4;
                }
            }
        }

        return result;
    }

    int16_t __cdecl Facing_Shield_(int16_t angle) {
        if (angle > 315 || angle <= 45) {
            return 0;
        }
        if (angle <= 135) {
            return 3;
        }
        if (angle <= 225) {
            return 2;
        }
        return 1;
    }

    bool __cdecl Special_Is_On_(int16_t ship_idx, int16_t weapon_idx) {
        return MOX::_combat_data[ship_idx].weapon_ready_flags[weapon_idx] == 1;
    }

    int8_t Special_Is_Turned_On_(int16_t ship_idx, int16_t special_idx) {
        int16_t found_count = 0;
        int8_t is_on = 0;
        bool found_target = false;

        for (int16_t bit_idx = 0; bit_idx < 39; ++bit_idx) {
            if (found_target) {
                break;
            }

            if (special_idx == bit_idx) {
                if (Special_Is_On_(ship_idx, found_count)) {
                    is_on = 1;
                    found_target = true;
                }
            }

            if (struct_::Test_Bit_Field_(MOX::_combat_data[ship_idx].special_device_flags, bit_idx) == 1) {
                found_count++;
            }
        }
        return is_on;
    }

    void __cdecl Remove_Stasis_Beam_(int16_t ship_idx) {
        for (int i = 0; i < 8; ++i) {
            MOX::_combat_data[ship_idx].weapons[i].shots_left = 0;
        }
        MOX::_combat_data[ship_idx].stasis_source_idx = -1;
    }

    void __cdecl Destroy_Ship_(int16_t ship_idx, int16_t param_2, int16_t param_3, int16_t param_4) {
        s_combat_data& ship = MOX::_combat_data[ship_idx];
        int8_t new_owner;

        if (ship.owner == MOX::_combat_attacker_player) {
            new_owner = static_cast<int8_t>(MOX::_combat_defender_player);
        } else {
            new_owner = static_cast<int8_t>(MOX::_combat_attacker_player);
        }

        ship.is_retreating = 0;
        Retarget_Missiles_Aimed_At_(ship_idx);
        COMBAT1::Remove_Continuing_Effect_Specials_(ship_idx);

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            if (ship.unit_status == 0) {
                CMBTSPEC::Destroy_Ship_FX_(ship_idx, param_2, param_3, param_4);
            }
        } else {
            ship.unit_status = 5;
        }

        if (ship.previous_owner == 11) {
            for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].owner == 11 && i != ship_idx) {
                    MOX::_combat_data[i].owner = new_owner;
                }
            }
        }
    }

    int16_t __cdecl Target_Is_Legal_For_Weapon_(int16_t weapon_id, uint16_t specials, int16_t target_idx, int16_t target_type) {
        int16_t is_legal = 1;

        if (target_type == 1 /* Missile */) {
            is_legal = CMBTFIRE::Weapon_Can_Fire_At_Missiles_(weapon_id, specials);

            int16_t missile_type = MOX::_missile[target_idx].type;
            int8_t rack = TECHDATA::_weapons[missile_type].rack;

            if (rack == 4 /* Torpedo */) {
                if (weapon_id == 33 /* Transporter */) {
                    is_legal = 0;
                }
            }

            if (rack == 2 /* Missile */) {
                is_legal = 0;
            }

            return is_legal;
        }

        if (CMBTMOV1::Ship_Is_In_Stasis_(target_idx)) {
            is_legal = 0;
        } else {
            if ((TECHDATA::_weapons[weapon_id].flags & 0x8) != 0) {
                int8_t prev_owner = MOX::_combat_data[target_idx].previous_owner;
                if (prev_owner >= 8 && prev_owner != 9) {
                    is_legal = 0;
                }
            }

            if (is_legal != 0) {
                int8_t rack = TECHDATA::_weapons[weapon_id].rack;
                if (rack == 3 /* Fighter */ && target_idx != 0) {
                    is_legal = 0;
                } else if (weapon_id == 33 /* Transporter */) {
                    is_legal = 0;
                } else if (target_idx == 0) {
                    if (weapon_id == 28 /* Stellar Converter */) {
                        is_legal = 0;
                    }
                    if (rack == 5 /* Special/Bomb */) {
                        switch (weapon_id) {
                            case 13:
                            case 32:
                            case 34:
                            case 35:
                            case 36:
                            case 37:
                            case 39:
                            case 44:
                            case 45:
                                is_legal = 0;
                                break;
                            default:
                                break;
                        }
                    }
                } else if (weapon_id == 28 /* Stellar Converter */) {
                    if (MOX::_combat_data[target_idx].owner > 8) {
                        is_legal = 0;
                    }
                }
            }
        }

        if (MOX::_combat_data[target_idx].black_hole_flag != 0 && weapon_id == 37 /* Black Hole Generator */) {
            is_legal = 0;
        }

        return is_legal;
    }

    void __cdecl Retarget_Missiles_Aimed_At_(int16_t unit_idx) {
        for (int16_t i = 0; i < 300; ++i) {
            s_missile* missile = &MOX::_missile[i];

            if (missile->type > 0 && missile->is_anti_missile_rocket == 0) {
                if (missile->source_unit_idx == unit_idx) {
                    if (missile->target_unit_idx == unit_idx) {
                        CMBTMIS::Kill_Missile_(i);
                    }
                }
                else if (missile->target_unit_idx == unit_idx) {
                    CMBTMIS::Retarget_Missile_(i);
                }
            }
        }
    }

    int16_t __cdecl Weapon_In_Range_(int16_t weapon_slot_idx, int16_t ship_idx, int16_t target_idx, int16_t target_type, int16_t range) {
        uint16_t specials;
        int16_t weapon_id;
        int16_t in_range;

        specials = MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].specials;
        weapon_id = MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].weapon_id;

        if (range == 0) {
            if (target_type == 1) {
                range = CMBTAI::Range_To_XY_(ship_idx, target_idx);
            }
            else {
                range = CMBTAI::Range_To_Ship_(ship_idx, target_idx);
            }
        }

        // Double range flag
        if (specials & 0x4) {
            range *= 2;
        }

        // Half range flag
        if (specials & 0x2) {
            range /= 2;
        }

        in_range = 0;
        switch (TECHDATA::_weapons[weapon_id].rack) {
        case 0: // Standard Beam / Direct fire
            if (range <= 8) {
                return 1;
            }
            break;

        case 1:
        case 2:
        case 4: // Missiles and Torpedoes
            in_range = 1;
            if (weapon_id == 0x13 /* WPN_MISSILE_PULSAR? */ && range > 8) {
                return 0;
            }
            if (weapon_id == 0x28 /* WPN_TORPEDO_PLASMA? */ && range > 5) {
                return 0;
            }
            break;

        case 3: // Fighters / Shuttles
            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x23 /* SPEC_CARRIER_GARRISON */)) {
                if (range <= 4) {
                    in_range = 1;
                }
            }
            else if (range <= 1) {
                in_range = 1;
            }

            if (target_idx != 0) {
                return 0;
            }
            break;

        case 5: // Specials and Bombs
            if (weapon_id == 0x27 && range <= 4) return 1;
            if (weapon_id == 0x20 && range <= 1) return 1;
            if (weapon_id == 0x21 && range <= 5) return 1;
            if (weapon_id == 0x22 && range <= 5) return 1;
            if (weapon_id == 0x23 && range <= 5) return 1;
            if (weapon_id == 0x24 && range <= 1) return 1;
            if (weapon_id == 0x25 && range <= 5) return 1;
            if (weapon_id == 0x26) return 1;
            if (weapon_id == 0x0D && range <= 1) return 1;
            if (weapon_id == 0x2C && range <= 1) return 1;
            if (weapon_id == 0x2D && range <= 5) return 1;
            break;

        default:
            break;
        }

        return in_range;
    }

    int16_t __cdecl Weapon_Can_Fire_At_Missiles_(int16_t weapon_idx, uint16_t flags) {
        int16_t result = 0;

        if (weapon_idx == 0) {
            return 0;
        }

        int8_t rack = TECHDATA::_weapons[weapon_idx].rack;

        // The assembly performs an unsigned bounds check on the sign-extended rack value
        // before using the jump table.
        if (static_cast<uint32_t>(static_cast<int32_t>(rack)) > 5) {
            return 0;
        }

        switch (rack) {
            case 0:
                // Check if specific flag bit is NOT set (0x2)
                if ((flags & 0x0002) == 0) {
                    return 1;
                }
                break;

            case 4:
                if (weapon_idx == 0x1f /* 31 */) {
                    result = 1;
                }
                // fallthrough

            case 5:
                if (weapon_idx == 0x24 /* 36 */ || weapon_idx == 0x0d /* 13 */ || weapon_idx == 0x21 /* 33 */) {
                    result = 1;
                }
                break;

            case 1:
            case 2:
            case 3:
            default:
                break;
        }

        return result;
    }

    void __cdecl Fire_Fighter_Beam_(int16_t missile_idx, int16_t param_2, int16_t param_3, int16_t* total_damage, int16_t* total_struct_damage) {
        int16_t struct_damage = 0;

        int16_t target_idx = MOX::_missile[missile_idx].target_unit_idx;
        int16_t source_owner = MOX::_combat_data[MOX::_missile[missile_idx].source_unit_idx].owner;

        *total_damage = 0;
        *total_struct_damage = 0;

        int16_t weapon_id = MOX::_fighter_beam_type[source_owner];
        uint16_t specials = 0;
        uint16_t weapon_flags = TECHDATA::_weapons[weapon_id].flags;

        if (weapon_flags & 0x100) {
            specials = 0x40;
        }

        int16_t weapon_damage = TECHDATA::_weapons[weapon_id].damage;
        int16_t weapon_min_damage = TECHDATA::_weapons[weapon_id].min_damage;

        int16_t chance_to_hit;
        if (param_3 == 0) {
            int16_t ocv = CMBTMIS::Fighter_Ocv_(source_owner);
            chance_to_hit = ocv - MOX::_combat_data[target_idx].tac_val_defense;
        } else {
            int16_t ocv = CMBTMIS::Fighter_Ocv_(source_owner);
            int16_t dcv = CMBTMIS::Missile_Dcv_(MOX::_missile[target_idx].type, MOX::_combat_data[MOX::_missile[target_idx].source_unit_idx].owner);
            chance_to_hit = ocv - dcv;
        }

        // Note: The original logic tests bit 2 of bStack_c here. Since bStack_c is only assigned 0 or 0x40, 
        // this condition will never be true and appears to be a bug in the original code.
        // It's intentionally preserved to stay strictly faithful to original semantics.
        if (specials & 0x4) {
            chance_to_hit += 25;
            weapon_damage = (weapon_damage + 1) / 2;
            weapon_min_damage = (weapon_min_damage + 1) / 2;
        }

        int16_t damage_diff = weapon_damage - weapon_min_damage;

        for (int16_t i = 0; i < MOX::_missile[missile_idx].quantity; i++) {
            if (CMBTMIS::Is_Missile_Target_Alive_(missile_idx) == 0) {
                break;
            }

            int16_t rand_val = (int16_t)random::Random_(100);
            if (rand_val > 95) {
                rand_val = 100;
            } else {
                rand_val += chance_to_hit;
                if (rand_val > 100) {
                    rand_val = 100;
                }
            }

            if (rand_val >= 40) {
                int16_t damage_done = weapon_min_damage;
                if (damage_diff > 0) {
                    damage_done += ((rand_val - 40) * (damage_diff + 1)) / 60;
                }

                if (param_3 == 0) {
                    struct_damage = 0;
                    if (target_idx == 0) {
                        damage_done /= 2;
                    }

                    CMBTFIRE::Apply_Ship_Damage_(target_idx, &damage_done, &struct_damage, param_2, specials, weapon_flags, 0, 0, 0);

                    *total_damage += damage_done;
                    *total_struct_damage += struct_damage;
                } else {
                    Apply_Damage_To_Missile_(target_idx, &damage_done, specials, weapon_flags);
                    MOX::_missile[target_idx].quantity -= damage_done;
                    *total_damage += damage_done;
                }
            }
        }

        if (CMBTMIS::Is_Missile_Target_Alive_(missile_idx) == 0) {
            if (param_3 == 1) {
                CMBTMIS::Retarget_Missile_(missile_idx);
                CMBTMIS::Kill_Missile_(target_idx);
            }
        }
    }

    void __cdecl Apply_Damage_To_Missile_(int16_t missile_idx, int16_t* damage_io, uint16_t specials, uint16_t weapon_flags) {
        s_missile* missile = &MOX::_missile[missile_idx];
        int16_t destroyed_count = 0;

        // Calculate health for a single missile in the stack
        int16_t max_hits = CMBTMIS::Get_Missile_Hits_(
            missile->type,
            static_cast<int16_t>(MOX::_combat_data[missile->source_unit_idx].owner),
            missile->specials
        );

        // Cap the incoming damage to the max hits of a single missile
        if (max_hits < *damage_io) {
            *damage_io = max_hits;
        }

        // Add incoming damage to already accumulated hits on the current missile
        int16_t total_hits = missile->hits_taken + *damage_io;
        *damage_io = total_hits;

        // Determine how many missiles are destroyed by the accumulated damage
        while (max_hits <= *damage_io) {
            *damage_io -= max_hits;
            destroyed_count++;
        }

        // Remaining damage stays as hits_taken on the next missile in stack
        missile->hits_taken = *damage_io;

        // Return the number of destroyed missiles through the IO pointer
        *damage_io = destroyed_count;
    }

    void __cdecl Fire_Fighter_Bomb_(int16_t missile_idx, int16_t damage_modifier, int16_t* total_damage, int16_t* total_internal_damage) {
        int16_t target_unit_idx;
        int16_t source_unit_idx;
        int16_t weapon_id;
        uint16_t weapon_flags;
        uint16_t damage_special_flags;
        int16_t min_damage;
        int16_t damage_range;
        int16_t roll;
        int16_t damage;
        int16_t internal_damage;
        int8_t owner;

        target_unit_idx = MOX::_missile[missile_idx].target_unit_idx;
        source_unit_idx = MOX::_missile[missile_idx].source_unit_idx;
        owner = MOX::_combat_data[source_unit_idx].owner;

        *total_damage = 0;
        *total_internal_damage = 0;

        weapon_id = MOX::_fighter_bomb_type[owner];
        weapon_flags = TECHDATA::_weapons[weapon_id].flags;
        damage_special_flags = 0;

        if ((weapon_flags & 0x100) != 0) {
            damage_special_flags = 0x40;
        }

        min_damage = TECHDATA::_weapons[weapon_id].min_damage;
        damage_range = TECHDATA::_weapons[weapon_id].damage - min_damage;

        for (int16_t i = 0; i < MOX::_missile[missile_idx].quantity; ++i) {
            roll = (int16_t)random::Random_(100);
            if (roll > 0) {
                internal_damage = 0;
                if (damage_range > 0) {
                    // Specific weighted damage calculation logic found in original assembly
                    damage = (int16_t)((((100 / (damage_range * 2)) + roll) * damage_range) / 100) + min_damage;
                } else {
                    damage = min_damage;
                }

                Apply_Ship_Damage_(target_unit_idx, &damage, &internal_damage, damage_modifier, damage_special_flags, weapon_flags, 0, 0, 0);

                *total_damage += damage;
                *total_internal_damage += internal_damage;
            }
        }
    }

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
    ) {
        int16_t local_8 = 0;
        int16_t local_e = 0;
        int16_t local_10 = 0;
        uint16_t retval = 0;

        *sys_dmg = 0;

        if (target_ship_idx == 0) {
            retval = Apply_Damage_To_Planet_(dmg_out, specials, weapon_flags);
            *dmg_out += local_10;
            return retval;
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 10 /* Reflection Field */)) {
            int16_t reflected = *dmg_out / 4;
            if (reflected < 2) {
                reflected = 1;
            }
            *dmg_out -= reflected;
            MOX::_combat_data[target_ship_idx].reflected_damage_pool += reflected;
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 7 /* Hard Shields */)) {
            *dmg_out /= 4;
        }

        int16_t local_a = MOX::_combat_data[target_ship_idx].shield_arc_current[param_4];
        if ((specials & 0x40) != 0) {
            if (!COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 13 /* Multi-Phased Shields */)) {
                local_a = 0;
            }
        }

        if (MOX::_combat_data[target_ship_idx].shield_type > 0 &&
            MOX::_combat_data[target_ship_idx].shield_current_hits > 0) {

            if (COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 13) && (specials & 0x40)) {
                specials ^= 0x40;
            }

            if (!(specials & 0x40)) {
                if (!MOX::_in_nebula_flag || COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 13)) {
                    *dmg_out -= TECHDATA::_shields[MOX::_combat_data[target_ship_idx].shield_type].points_blocked;
                    if (COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 13)) {
                        *dmg_out -= 3;
                    }
                }
            }

            if (*dmg_out < 0) {
                *dmg_out = 0;
            }

            if (*dmg_out > 0) {
                if (!MOX::_in_nebula_flag || COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 13)) {
                    retval |= 1;
                }
            }

            if (*dmg_out < local_a) {
                MOX::_combat_data[target_ship_idx].shield_arc_current[param_4] -= *dmg_out;
                *sys_dmg = *dmg_out;
                *dmg_out = 0;
            } else {
                *dmg_out -= local_a;
                *sys_dmg = local_a;
                MOX::_combat_data[target_ship_idx].shield_arc_current[param_4] -= local_a;
            }
        }

        if (weapon_flags & 1) {
            *dmg_out = 0;
        }

        if ((weapon_flags & 0x10) != 0) {
            if (!COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 7)) {
                if (MOX::_combat_data[target_ship_idx].marine_count != -1) {
                    CMBTFIR2::Kill_Crew_(target_ship_idx, *dmg_out / 5);
                }
            }
        }

        if (param_7 != 0) {
            *dmg_out *= 2;
        }

        int16_t ignore_armor;
        if (param_8 == 0) {
            ignore_armor = (specials & 8) != 0;
            if (COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, 14 /* Heavy Armor */) ||
                MOX::_combat_data[target_ship_idx].armor_type == 6) {
                ignore_armor = 0;
            }
        } else {
            ignore_armor = 1;
        }

        if ((weapon_flags & 8) && MOX::_combat_data[target_ship_idx].owner > 9) {
            *dmg_out /= 4;
        }

        if (*dmg_out > 0) {
            retval |= 2;

            if (weapon_flags & 0x20) {
                MOX::_combat_data[target_ship_idx].structure_damage += *dmg_out / 2;
                int16_t cur_dmg = MOX::_combat_data[target_ship_idx].structure_damage;
                int16_t max_dmg = MOX::_combat_data[target_ship_idx].structure_max;
                if (cur_dmg >= max_dmg) {
                    MOX::_combat_data[target_ship_idx].structure_damage = max_dmg;
                } else {
                    // Replicate original bug: damage fraction added again if it didn't hit maximum
                    MOX::_combat_data[target_ship_idx].structure_damage = cur_dmg + *dmg_out / 2;
                }
                local_10 = *dmg_out / 2;
            }

            int16_t bypassed_damage;
            if (ignore_armor) {
                bypassed_damage = *dmg_out;
                int16_t remaining_damage = *dmg_out;

                while (remaining_damage > 0 && !CMBTFIRE::Is_Combat_Ship_Dead_(target_ship_idx)) {
                    CMBTFIR2::Select_Internal_(target_ship_idx, &local_8, &local_e, param_8, weapon_flags & 8, 1, param_9);
                    if (local_8 == -1) {
                        remaining_damage = 0;
                    }
                    CMBTFIR2::Apply_Internal_Damage_(target_ship_idx, &remaining_damage, local_8, local_e, 1);
                }
            } else {
                bypassed_damage = 0;
            }

            int16_t remaining_damage = *dmg_out - bypassed_damage;
            while (remaining_damage > 0 && !CMBTFIRE::Is_Combat_Ship_Dead_(target_ship_idx)) {
                CMBTFIR2::Select_Internal_(target_ship_idx, &local_8, &local_e, param_8, weapon_flags & 8, 0, param_9);
                if (local_8 == -1) {
                    remaining_damage = 0;
                }
                CMBTFIR2::Apply_Internal_Damage_(target_ship_idx, &remaining_damage, local_8, local_e, 1);
            }
        }

        *dmg_out += local_10;
        return retval;
    }
}