#include "pch.h"

namespace CMBTAI {

int16_t __cdecl Expected_Weapon_Damage_(int16_t source_idx, int16_t weapon_slot, int16_t target_idx, int16_t target_type, int16_t range) {
    int expected_damage = 0;

    int16_t weapon_id = MOX::_combat_data[source_idx].weapons[weapon_slot].weapon_id;
    uint16_t specials = MOX::_combat_data[source_idx].weapons[weapon_slot].specials;
    int count = MOX::_combat_data[source_idx].weapons[weapon_slot].count;

    if (weapon_id > 0 && CMBTFIRE::Target_Is_Legal_For_Weapon_(weapon_id, specials, target_idx, target_type)) {
        if (range == 0) {
            range = 1;
        } else {
            short bearing;
            if (target_type == 0) {
                bearing = CMBTDRW1::Relative_Bearing_(source_idx, target_idx);
            } else {
                bearing = CMBTDRW1::Relative_Bearing_XY_(source_idx, MOX::_missile[target_idx].x, MOX::_missile[target_idx].y);
            }
            uint8_t arc = CMBTDRW1::Get_Weapon_Arc_(bearing);
            count = 0;

            if (COMBAT1::Weapon_Is_On_(source_idx, weapon_slot)) {
                if ((arc & MOX::_combat_data[source_idx].weapons[weapon_slot].arc) != 0) {
                    int8_t rack = TECHDATA::_weapons[weapon_id].rack;
                    bool can_fire = false;

                    if (rack == 2) {
                        if (MOX::_combat_data[source_idx].weapons[weapon_slot].ammo == 0) {
                            can_fire = true;
                        }
                    } else {
                        if (MOX::_combat_data[source_idx].weapons[weapon_slot].ammo != 0) {
                            can_fire = true;
                        }
                    }

                    if (can_fire && CMBTFIRE::Weapon_In_Range_(weapon_slot, source_idx, target_idx, target_type, range)) {
                        count = MOX::_combat_data[source_idx].weapons[weapon_slot].shots_left;
                    }
                }
            }
        }

        if (count > 0) {
            int8_t rack = TECHDATA::_weapons[weapon_id].rack;

            if (rack == 1 || rack == 4 || rack == 2) {
                expected_damage = Expected_Missile_Damage_(target_idx, weapon_id, count, specials, MOX::_combat_data[source_idx].owner);
            } else if (rack == 0 || rack == 3) {
                expected_damage = Expected_Beam_Bomb_Damage_(source_idx, weapon_slot, target_idx, count, range, target_type);
            } else if (rack == 5) {
                int min_dam = TECHDATA::_weapons[weapon_id].min_damage;
                int max_dam = TECHDATA::_weapons[weapon_id].damage;
                int avg_dam = (min_dam + max_dam) / 2;
                int blocked = TECHDATA::_shields[MOX::_combat_data[target_idx].shield_type].points_blocked;
                int size_mult = MOX::_combat_data[target_idx].size_class + 1;

                switch (weapon_id) {
                    case 0xD: // 13
                    case 0x22: // 34
                        expected_damage = avg_dam * size_mult * count;
                        break;
                    case 0x20: // 32
                    case 0x25: // 37
                        expected_damage = 1000;
                        break;
                    case 0x21: // 33
                        if (target_type != 0) {
                            s_missile* m = &MOX::_missile[target_idx];
                            int hits = CMBTMIS::Get_Missile_Hits_(m->type, MOX::_combat_data[m->source_unit_idx].owner, m->specials);
                            expected_damage = hits * (((8 - range) * count * 5 + 25) * 2 / 100);
                        }
                        break;
                    case 0x23: // 35
                    case 0x26: // 38
                    case 0x2D: // 45
                        expected_damage = (avg_dam - blocked) * count * 4;
                        break;
                    case 0x24: // 36
                        expected_damage = (avg_dam - blocked) * size_mult * count;
                        break;
                    case 0x27: // 39
                        if (MOX::_combat_data[target_idx].current_speed > 0) {
                            expected_damage = 50;
                        }
                        break;
                    case 0x2C: // 44
                        expected_damage = (avg_dam - blocked) * size_mult * count * 4;
                        break;
                }
            }
        }
    }

    if (COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, 7)) {
        expected_damage >>= 2;
    }

    int8_t rack = TECHDATA::_weapons[weapon_id].rack;
    if (rack == 0 || rack == 3) {
        expected_damage = (expected_damage + 99) / 100;
    }

    return (int16_t)expected_damage;
}

} // namespace CMBTAI