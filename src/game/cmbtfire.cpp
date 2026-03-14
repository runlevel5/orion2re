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

    int16_t __cdecl Target_Is_Legal_For_Weapon_(int16_t weapon_id, int16_t source_unit_idx, int16_t target_idx, int16_t is_missile_target) {
        bool is_legal = true;

        if (is_missile_target) {
            is_legal = Weapon_Can_Fire_At_Missiles_(weapon_id, source_unit_idx);
            int16_t missile_type = MOX::_missile[target_idx].type;
            if (TECHDATA::_weapons[missile_type].rack == 4 && weapon_id == 0x21) {
                is_legal = false;
            }
            if (TECHDATA::_weapons[missile_type].rack == 2) {
                return false;
            }
            return is_legal;
        }

        if (CMBTMOV1::Ship_Is_In_Stasis_(target_idx)) {
            is_legal = false;
        }
        else {
            bool check_weapons_flag = true;
            if ((TECHDATA::_weapons[weapon_id].flags & 8) != 0 &&
                (MOX::_combat_data[target_idx].previous_owner >= 8 && MOX::_combat_data[target_idx].previous_owner != 9)) {
                check_weapons_flag = false;
            }

            if (check_weapons_flag && (TECHDATA::_weapons[weapon_id].rack != 3 || target_idx == 0) && weapon_id != 0x21) {
                if (target_idx == 0) {
                    is_legal = (weapon_id != 0x1c);
                    if (TECHDATA::_weapons[weapon_id].rack == 5) {
                        switch (weapon_id) {
                        case 0xd:
                        case 0x20:
                        case 0x22:
                        case 0x23:
                        case 0x24:
                        case 0x25:
                        case 0x27:
                        case 0x2c:
                        case 0x2d:
                            is_legal = false;
                            break;
                        default:
                            break;
                        }
                    }
                }
                else {
                    if (weapon_id == 0x1c && MOX::_combat_data[target_idx].owner >= 9) {
                        is_legal = true;
                    }
                    else {
                        is_legal = false;
                    }
                }
            }
        }

        if (is_legal && MOX::_combat_data[target_idx].black_hole_flag != 0 && weapon_id == 0x25) {
            is_legal = false;
        }

        return is_legal;
    }
}