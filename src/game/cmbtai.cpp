#include "pch.h"

namespace CMBTAI {

int16_t _fleet_target;

/**
 * Checks if all active enemy ships are currently in stasis.
 * If so, releases the smallest enemy ship from stasis to prevent a combat deadlock.
 * Returns the number of enemy ships that are NOT in stasis.
 */
int16_t __cdecl Ai_Stasis_Check_(int16_t player_idx) {
    int16_t enemy_player;
    int16_t total_enemy_ships = 0;
    int16_t ships_in_stasis = 0;

    if (MOX::_combat_attacker_player == player_idx) {
        enemy_player = MOX::_combat_defender_player;
    } else {
        enemy_player = MOX::_combat_attacker_player;
    }

    // Determine starting index: if planet exists, it is at index 0. 
    // If not, we skip index 0.
    int16_t start_idx = (CMBTFIRE::Does_Combat_Planet_Have_Defenses_() <= 0);

    for (int16_t i = start_idx; i < MOX::_combat_total_ship_count; ++i) {
        s_combat_data* ship = &MOX::_combat_data[i];

        // Only consider ships that are active, not captured, and belong to the enemy
        if (ship->unit_status == 0 && ship->is_captured == 0 && ship->owner != player_idx) {
            total_enemy_ships++;

            if (CMBTMOV1::Ship_Is_In_Stasis_(i)) {
                ships_in_stasis++;
            }
        }
    }

    // If all active enemy ships are in stasis, force-release one
    if ((total_enemy_ships - ships_in_stasis) < 1) {
        CMBTAI::Release_Smallest_Ship_From_Stasis_(enemy_player);
    }

    return total_enemy_ships - ships_in_stasis;
}

int32_t __cdecl Expected_Beam_Bomb_Damage_(int16_t ship_idx, int16_t weapon_slot_idx, int16_t target_idx, int16_t count, int16_t distance, int16_t target_type_flag) {
    int16_t range_penalty = 0;
    int16_t damage_bonus_pct = 0;
    int32_t attack_defense_delta;
    int32_t points_blocked;
    int16_t use_planetary_shield_adjustment = 0;

    uint16_t specials = MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].specials;
    int16_t weapon_id = MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].weapon_id;

    if (target_type_flag == 0) {
        int16_t has_hard_shields = COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, SPECIAL_HARD_SHIELDS);
        if (has_hard_shields != 0 || (specials & 0x40) == 0) {
            if (target_idx == 0) {
                use_planetary_shield_adjustment = 1;
                points_blocked = MOX::_combat_data[0].shield_arc_max;
            } else {
                int16_t blocked = 0;
                if (MOX::_combat_data[target_idx].shield_current_hits > 0) {
                    int8_t shield_type = MOX::_combat_data[target_idx].shield_type;
                    blocked = TECHDATA::_shields[shield_type].shield_class;
                }
                if (COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, SPECIAL_HARD_SHIELDS) != 0) {
                    blocked += 3;
                }
                if ((TECHDATA::_weapons[weapon_id].natural_mods & 0x100) != 0 ||
                    ((specials & 0x40) != 0 && COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, SPECIAL_HARD_SHIELDS) == 0)) {
                    blocked = 0;
                }
               points_blocked = (int32_t)blocked;
            }
        } else {
            points_blocked = 0;
        }
    } else {
        points_blocked = 0;
    }

    int32_t dist_modifier;
    if (TECHDATA::_weapons[weapon_id].type == 0) {
        CMBTFIRE::Get_Beam_Weapon_Modifiers_(ship_idx, distance, specials, weapon_id, &range_penalty, &damage_bonus_pct);
        dist_modifier = 40 - range_penalty;
        if (dist_modifier > 95) {
            dist_modifier = 95;
        }
        attack_defense_delta = MOX::_combat_data[ship_idx].tac_val_attack;
        if (target_type_flag == 0) {
            attack_defense_delta -= MOX::_combat_data[target_idx].tac_val_defense;
        } else {
            int16_t source_unit_idx = MOX::_missile[target_idx].source_unit_idx;
            int16_t owner = MOX::_combat_data[source_unit_idx].owner;
            attack_defense_delta -= CMBTMIS::Missile_Dcv_(MOX::_missile[target_idx].type, owner);
        }
    } else {
        damage_bonus_pct = 100;
        dist_modifier = 0;
        range_penalty = 0;
        attack_defense_delta = 0;
    }

    int32_t ordnance_bonus = MOX::_ordnance_bonus[MOX::_combat_data[ship_idx].owner] + damage_bonus_pct - 100;
    
    int32_t min_damage = (TECHDATA::_weapons[weapon_id].min_damage * ordnance_bonus + 50) / 100;
    if (min_damage < 1) {
        min_damage = 1;
    }
    
    int32_t max_damage = (TECHDATA::_weapons[weapon_id].max_damage * ordnance_bonus + 50) / 100;
    if (max_damage <= min_damage) {
        max_damage = min_damage;
    }

    if (use_planetary_shield_adjustment != 0 && TECHDATA::_weapons[weapon_id].type == 0) {
        int16_t shield_id = INITSHIP::Best_Ship_Shield_(MOX::_combat_defender_player);
        min_damage /= 2;
        max_damage /= 2;
        if (shield_id == SPECIAL_DAMPER_FIELD) {
            min_damage /= 4;
            max_damage /= 4;
        } else {
            min_damage -= TECHDATA::_shields[shield_id].shield_class;
            max_damage -= TECHDATA::_shields[shield_id].shield_class;
        }
        if (min_damage < 0) {
            min_damage = 0;
        }
        if (max_damage < 0) {
            max_damage = 0;
        }
    }

    int32_t actual_dmg = GEO::Actual_Expected_Damage_(min_damage, max_damage, dist_modifier, attack_defense_delta, points_blocked);
    return count * actual_dmg;
}

int16_t __cdecl Expected_Weapon_Damage_(int16_t source_idx, int16_t weapon_slot, int16_t target_idx, int16_t target_type, int16_t range) {
    int32_t expected_damage = 0;

    int16_t weapon_id = MOX::_combat_data[source_idx].weapons[weapon_slot].weapon_id;
    uint16_t specials = MOX::_combat_data[source_idx].weapons[weapon_slot].specials;
    int16_t count = MOX::_combat_data[source_idx].weapons[weapon_slot].count;

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
            int16_t arc = (int16_t)CMBTDRW1::Get_Weapon_Arc_(bearing);
            count = 0;

            if (COMBAT1::Weapon_Is_On_(source_idx, weapon_slot)) {
                if ((arc & (int16_t)MOX::_combat_data[source_idx].weapons[weapon_slot].arc) != 0) {
                    uint8_t rack = TECHDATA::_weapons[weapon_id].type;
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
            uint8_t rack = TECHDATA::_weapons[weapon_id].type;

            if (rack == 1 || rack == 4 || rack == 2) {
                expected_damage = Expected_Missile_Damage_(target_idx, weapon_id, count, specials, MOX::_combat_data[source_idx].owner);
            } else if (rack == 0 || rack == 3) {
                expected_damage = Expected_Beam_Bomb_Damage_(source_idx, weapon_slot, target_idx, count, range, target_type);
            } else if (rack == 5) {
                int16_t min_dam = TECHDATA::_weapons[weapon_id].min_damage;
                int16_t max_dam = TECHDATA::_weapons[weapon_id].max_damage;
                int32_t avg_damage_sum = ((int32_t)min_dam) + ((int32_t)max_dam);
                int32_t avg_damage = (avg_damage_sum - (avg_damage_sum >> 31)) >> 1;

                switch (weapon_id) {
                    case WEAPON_SPATIAL_COMPRESSOR:
                    case WEAPON_GYRO_DESTABILIZER: {
                        int size_mult = MOX::_combat_data[target_idx].size_class + 1;
                        expected_damage = avg_damage * size_mult * count;
                        break;
                    }
                    case WEAPON_STASIS_FIELD:
                    case WEAPON_BLACK_HOLE_GENERATOR:
                        expected_damage = 1000;
                        break;
                    case WEAPON_ANTI_MISSILE_ROCKET:
                        if (target_type != 0) {
                            s_missile* m = &MOX::_missile[target_idx];
                            int hits = CMBTMIS::Get_Missile_Hits_(m->type, MOX::_combat_data[m->source_unit_idx].owner, m->specials);
                            expected_damage = hits * (((8 - range) * count * 5 + 25) * 2 / 100);
                        }
                        break;
                    case WEAPON_PLASMA_WEB:
                    case WEAPON_STELLAR_CONVERTER:
                    case WEAPON_CAUSTIC_SLIME: {
                        int blocked = TECHDATA::_shields[MOX::_combat_data[target_idx].shield_type].shield_class;
                        expected_damage = (avg_damage - blocked) * count * 4;
                        break;
                    }
                    case WEAPON_PULSAR: {
                        int blocked = TECHDATA::_shields[MOX::_combat_data[target_idx].shield_type].shield_class;
                        int size_mult = MOX::_combat_data[target_idx].size_class + 1;
                        expected_damage = (avg_damage - blocked) * size_mult * count;
                        break;
                    }
                    case WEAPON_TRACTOR_BEAM:
                        if (MOX::_combat_data[target_idx].current_speed > 0) {
                            expected_damage = 50;
                        }
                        break;
                    case WEAPON_PLASMA_FLUX: {
                        int blocked = TECHDATA::_shields[MOX::_combat_data[target_idx].shield_type].shield_class;
                        int size_mult = MOX::_combat_data[target_idx].size_class + 1;
                        expected_damage = (avg_damage - blocked) * size_mult * count * 4;
                        break;
                    }
                }
            }
        }
    }

    if (COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, SPECIAL_DAMPER_FIELD)) {
        expected_damage >>= 2;
    }

    uint8_t rack = TECHDATA::_weapons[weapon_id].type;
    if (rack == 0 || rack == 3) {
        expected_damage = (int32_t)(((uint32_t)(expected_damage + 99)) / 100);
    }

    return (int16_t)expected_damage;
}

void __cdecl Toggle_Auto_Combat_Flags_() {
    int16_t off_val = 0;
    int16_t on_val = 1;
    int16_t ship_owner = MOX::_combat_data[MOX::_cur_ship].owner;

    if (MOX::_combat_attacker_player == ship_owner) {
        if (ship_owner == MOX::_PLAYER_NUM || MOX::_game_type == 1) {
            MOX::_auto_combat_flag[0] = 0;
            MOX::_auto_combat = 0;
            MOX::_super_fast_flag = 0;

            if (MOX::_net_flag != 0) {
                NETMOX::Mox_Send_Message_(MOX::_player[MOX::_combat_defender_player].network_player_id, TACTICAL_NET_PACKET_AUTO_COMBAT_DISABLED, &off_val, sizeof(off_val));
            }
        } else {
            MOX::_auto_combat_flag[1] = 0;
            MOX::_super_fast_flag = 0;

            if (MOX::_net_flag != 0) {
                NETMOX::Mox_Send_Message_(MOX::_player[ship_owner].network_player_id, TACTICAL_NET_PACKET_AUTO_COMBAT_DISABLED, &on_val, sizeof(on_val));
            }
        }
    } else {
        if (ship_owner == MOX::_PLAYER_NUM || MOX::_game_type == 1) {
            MOX::_auto_combat_flag[1] = 0;
            MOX::_auto_combat = 0;
            MOX::_super_fast_flag = 0;

            if (MOX::_net_flag != 0) {
                NETMOX::Mox_Send_Message_(MOX::_player[MOX::_combat_attacker_player].network_player_id, TACTICAL_NET_PACKET_AUTO_COMBAT_DISABLED, &on_val, sizeof(on_val));
            }
        } else {
            MOX::_auto_combat_flag[0] = 0;
            MOX::_super_fast_flag = 0;

            if (MOX::_net_flag != 0) {
                NETMOX::Mox_Send_Message_(MOX::_player[ship_owner].network_player_id, TACTICAL_NET_PACKET_AUTO_COMBAT_DISABLED, &off_val, sizeof(off_val));
            }
        }
    }
}

int8_t __cdecl Get_Player_Mode_(int16_t player_idx) {
    int16_t opponent;
    int16_t player_missile_limit;
    int16_t opponent_missile_limit;
    int16_t player_eff_missile;
    int16_t player_lr_beam;
    int16_t opponent_eff_missile;
    int16_t player_eff_beam;
    int16_t opponent_eff_beam;
    int16_t opponent_lr_beam;

    if (player_idx == MOX::_combat_attacker_player) {
        opponent = MOX::_combat_defender_player;
    } else {
        opponent = MOX::_combat_attacker_player;
    }

    if (Retreat_Check_(player_idx) != 0) {
        return (int8_t)0xFF;
    }

    player_eff_missile = Get_Effective_Missile_Strength_(player_idx, &player_missile_limit);
    player_lr_beam = Get_Long_Range_Beam_Strength_(player_idx);
    opponent_eff_missile = Get_Effective_Missile_Strength_(opponent, &opponent_missile_limit);
    player_eff_beam = Get_Effective_Beam_Strength_(player_idx);
    opponent_eff_beam = Get_Effective_Beam_Strength_(opponent);
    opponent_lr_beam = Get_Long_Range_Beam_Strength_(opponent);

    // Check if the player is in a strong tactical position
    if ((opponent_eff_missile + opponent_lr_beam < player_eff_missile + player_lr_beam) &&
        MOX::_player_mode[player_idx] != 1 && MOX::_player_mode[player_idx] != 0) {

        if ((opponent_eff_beam * 3 / 4) >= opponent_missile_limit) {
            return 3;
        }

        if (player_idx == MOX::_combat_attacker_player && opponent_missile_limit > 50) {
            return 0;
        }

        return 2;
    } else {
        // Defensive or standard engagement check
        return (opponent_eff_beam * 3 / 2 >= player_eff_beam);
    }
}

int16_t __cdecl Range_To_Ship_(int16_t ship1_idx, int16_t ship2_idx) {
    int16_t w1, h1, w2, h2;
    int32_t is_large1 = 0;
    int32_t is_huge1 = 0;
    int32_t is_large2 = 0;
    int32_t is_huge2 = 0;

    COMBINIT::Ship_Center_Offsets_(ship1_idx, &w1, &h1);
    int16_t x1 = MOX::_combat_data[ship1_idx].x;
    int16_t y1 = MOX::_combat_data[ship1_idx].y;

    if (w1 > 10) {
        is_large1 = 1;
    }
    if (w1 > 20) {
        is_huge1 = 1;
        x1++;
        y1++;
    }

    COMBINIT::Ship_Center_Offsets_(ship2_idx, &w2, &h2);
    int16_t x2 = MOX::_combat_data[ship2_idx].x;
    int16_t y2 = MOX::_combat_data[ship2_idx].y;

    if (w2 > 10) {
        is_large2 = 1;
    }
    if (w2 > 20) {
        is_huge2 = 1;
        x2++;
        y2++;
    }

    int16_t x1_adj, y1_adj;
    if (x2 < x1) {
        x1_adj = x1 - is_huge1;
        if (x1_adj <= x2) {
            x1_adj = x2;
        }
    } else {
        x1_adj = x1 + is_large1;
        if (x1_adj >= x2) {
            x1_adj = x2;
        }
    }

    if (y2 < y1) {
        y1_adj = y1 - is_huge1;
        if (y1_adj <= y2) {
            y1_adj = y2;
        }
    } else {
        y1_adj = y1 + is_large1;
        if (y1_adj >= y2) {
            y1_adj = y2;
        }
    }

    int16_t x2_adj, y2_adj;
    if (x2 > x1_adj) {
        x2_adj = x2 - is_huge2;
        if (x2_adj <= x1_adj) {
            x2_adj = x1_adj;
        }
    } else {
        x2_adj = x2 + is_large2;
        if (x2_adj >= x1_adj) {
            x2_adj = x1_adj;
        }
    }

    if (y2 > y1_adj) {
        y2_adj = y2 - is_huge2;
        if (y2_adj <= y1_adj) {
            y2_adj = y1_adj;
        }
    } else {
        y2_adj = y2 + is_large2;
        if (y2_adj >= y1_adj) {
            y2_adj = y1_adj;
        }
    }

    int16_t range = special::Range_(x1_adj, y1_adj, x2_adj, y2_adj);
    return (range + 2) / 3;
}

int16_t __cdecl Range_To_Missile_(int16_t unit_idx, int16_t missile_idx) {
    s_missile* missile = &MOX::_missile[missile_idx];
    return CMBTAI::Range_To_XY_(unit_idx, missile->x, missile->y);
}

int16_t __cdecl Will_Missile_Do_Damage_(int16_t target_idx, int16_t missile_idx) {
    int16_t damage = 0;
    s_missile* missile = &MOX::_missile[missile_idx];
    int16_t type = missile->type;

    if (TECHDATA::_weapons[type].type != 4) {
        damage = TECHDATA::_weapons[type].max_damage;
    } else {
        switch (type) {
        case 28:
            damage = 100;
            break;
        case 29:
        case 30:
        {
            int16_t owner = MOX::_combat_data[missile->source_unit_idx].owner;
            int16_t fighter_type = MOX::_fighter_bomb_type[owner];
            damage = TECHDATA::_weapons[fighter_type].max_damage;
            break;
        }
        case 31:
        {
            int16_t owner = MOX::_combat_data[missile->source_unit_idx].owner;
            int16_t fighter_type = MOX::_fighter_beam_type[owner];
            damage = TECHDATA::_weapons[fighter_type].max_damage;
            break;
        }
        default:
            break;
        }
    }

    int16_t points_blocked = 0;
    s_combat_data* target = &MOX::_combat_data[target_idx];
    if (target->shield_current_hits > 0) {
       points_blocked = TECHDATA::_shields[target->shield_type].shield_class;
    }

    return damage > points_blocked;
}

bool __cdecl Safe_To_Fire_Sphere_Weapon_(int16_t source_ship_idx) {
    int16_t friendly_ship_score = 0;
    int16_t enemy_ship_score = 0;
    int16_t friendly_fighter_qty = 0;
    int16_t friendly_missile_qty = 0;
    int16_t enemy_fighter_qty = 0;
    int16_t enemy_missile_qty = 0;
    bool can_fire = true;

    if (MOX::_combat_total_ship_count > 1) {
        for (int16_t target_ship_idx = 1; target_ship_idx < MOX::_combat_total_ship_count; ++target_ship_idx) {
            if (MOX::_combat_data[target_ship_idx].unit_status == 0 && target_ship_idx != source_ship_idx) {
                if (!CMBTMOV1::Ship_Is_In_Stasis_(target_ship_idx)) {
                    int16_t distance = Range_To_Ship_(source_ship_idx, target_ship_idx);
                    if (MOX::_combat_data[source_ship_idx].owner == MOX::_combat_data[target_ship_idx].owner) {
                        if (distance < 2) {
                            friendly_ship_score += 2;
                        } else if (distance == 2) {
                            friendly_ship_score += 1;
                        }
                    } else {
                        if (distance < 2) {
                            enemy_ship_score += 2;
                        } else if (distance == 2) {
                            enemy_ship_score += 1;
                        }
                    }
                }
            }
        }
    }

    for (int16_t missile_idx = 0; missile_idx < 300; ++missile_idx) {
        int16_t weapon_type = MOX::_missile[missile_idx].type;
        if (weapon_type > 0) {
            if (TECHDATA::_weapons[weapon_type].type != 2 /* RACK_HEAVY/MIRV */) {
                int16_t distance = Range_To_Missile_(source_ship_idx, missile_idx);
                if (distance < 3) {
                    int16_t missile_owner = CMBTMIS::Missile_Owner_(missile_idx);
                    int16_t quantity = MOX::_missile[missile_idx].quantity;
                    if (MOX::_combat_data[source_ship_idx].owner == missile_owner) {
                        if (TECHDATA::_weapons[weapon_type].type == 4 /* RACK_FIGHTER */) {
                            friendly_fighter_qty += quantity;
                        } else {
                            friendly_missile_qty += quantity;
                        }
                    } else {
                        if (TECHDATA::_weapons[weapon_type].type == 4 /* RACK_FIGHTER */) {
                            enemy_fighter_qty += quantity;
                        } else {
                            enemy_missile_qty += quantity;
                        }
                    }
                }
            }
        }
    }

    int16_t total_score = (enemy_ship_score * 3 - friendly_ship_score * 2 + enemy_fighter_qty) * 5;
    total_score -= (friendly_fighter_qty * 8);
    total_score -= friendly_missile_qty;
    total_score += enemy_missile_qty;

    if (total_score < 0) {
        can_fire = false;
    }

    return can_fire;
}

void __cdecl Do_Auto_Ship_Turn_(void) {
    s_combat_data* ship = &MOX::_combat_data[MOX::_cur_ship];
    int8_t did_move = 0;

    if (CMBTMOV1::Ship_Is_In_Stasis_(MOX::_cur_ship)) {
        return;
    }

    if (!CMBTDRW1::Ship_Completely_On_Screen_(MOX::_cur_ship)) {
        COMBINIT::Snap_Center_Combat_Screen_(ship->x, ship->y);
    }

    if ((Is_Ship_Out_Of_Ammo_(MOX::_cur_ship) || MOX::_player_mode[ship->owner] == -1) && ship->ftl_type != 0) {
        if (COMBAT1::Enemy_Dissipator_Exists_(ship->owner) == 0 && ship->owner < MAX_PLAYERS && MOX::_ai_retreat_flag == 1) {
            ship->is_retreating = 1;
            ship->field40_0x47 = -1;
        }
    }

    Ai_Stasis_Check_(ship->owner);
    COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);

    int16_t action_result = 0;
    do {
        int16_t target_idx = Choose_Target_(MOX::_cur_ship);
        if (fields::Get_Input_() == MOX::_auto_button) {
            Toggle_Auto_Combat_Flags_();
        }

        if (target_idx == -1) {
            did_move = 0;
            action_result = 0;
        } else {
            ship->ai_range_tactic = Combat_Ship_Class_(MOX::_cur_ship, target_idx);
            action_result = Auto_Move_Ship_(MOX::_cur_ship, target_idx);
            if (action_result == 0) {
                MOX::_missile_warning = 2;
                did_move = 1 - did_move;
            } else {
                did_move = 0;
            }
        }

        if (fields::Get_Input_() == MOX::_auto_button) {
            Toggle_Auto_Combat_Flags_();
        }

        if (ship->unit_status == 0 && ship->is_captured == 0) {
            for (int i = 0; i < 8; ++i) {
                ship->weapons[i].active = -1;
            }

            int16_t last_target = -1;
            int16_t weapon_idx = 0;
            do {
                int16_t weapon_id = ship->weapons[weapon_idx].weapon_id;
                bool skip_fire = false;
                bool advance_weapon = false;

                if (weapon_id == WEAPON_SPATIAL_COMPRESSOR) {
                    if (!Does_Ship_Have_Missiles_Incoming_(MOX::_cur_ship)) {
                        // Proceed to other checks
                    } else {
                        skip_fire = true;
                    }
                }

                if (!skip_fire) {
                    if (weapon_id == WEAPON_PULSAR || weapon_id == WEAPON_SPATIAL_COMPRESSOR) {
                        if (!Safe_To_Fire_Sphere_Weapon_(MOX::_cur_ship)) {
                            skip_fire = true;
                        }
                    }
                }

                if (!skip_fire && weapon_id == WEAPON_STASIS_FIELD) {
                    if (Ai_Stasis_Check_(ship->owner) < 2) {
                        skip_fire = true;
                    }
                }

                if (!skip_fire) {
                    ship->weapons[weapon_idx].active = 1;
                    int16_t target_idx_inner = Get_Current_Target_(MOX::_cur_ship, last_target);

                    if (ship->target_type_flag == 0 /* Ship */) {
                        if (target_idx_inner != -1) {
                            last_target = target_idx_inner;
                        }
                        int16_t boarding_type = Boarding_Action_Type_(MOX::_cur_ship, target_idx_inner);
                        if (boarding_type != 0) {
                            Resolve_Ai_Boarding_(MOX::_cur_ship, target_idx_inner, boarding_type);
                            target_idx_inner = Get_Current_Target_(MOX::_cur_ship, target_idx_inner);
                        }
                    }

                    if (target_idx_inner != -1) {
                        if (TECHDATA::_weapons[weapon_id].type == 0) { // Beam weapon
                            uint16_t specials = ship->weapons[weapon_idx].specials;
                            if (!(specials & 2)) {
                                if (!(specials & 4) || Is_Ship_Targeted_By_Missile_(MOX::_cur_ship) == 0) {
                                    int16_t range;
                                    if (ship->target_type_flag == 0) {
                                        range = Range_To_Ship_(MOX::_cur_ship, target_idx_inner);
                                    } else {
                                        range = Range_To_Missile_(MOX::_cur_ship, target_idx_inner);
                                    }
                                    short mod1 = 0;
                                    short mod2 = 0;
                                    CMBTFIRE::Get_Beam_Weapon_Modifiers_(MOX::_cur_ship, range, specials, weapon_id, &mod1, &mod2);
                                }
                            }
                        }

                        if (!skip_fire) {
                            int16_t t_x, t_y;
                            if (ship->target_type_flag == 1 /* Missile */) {
                                t_x = MOX::_missile[target_idx_inner].x;
                                t_y = MOX::_missile[target_idx_inner].y;
                                action_result = CMBTFIRE::Fire_Ship_(MOX::_cur_ship, ship->x * 20, ship->y * 20, target_idx_inner, t_x, t_y, 1);
                            } else {
                                t_x = MOX::_combat_data[target_idx_inner].x * 20;
                                t_y = MOX::_combat_data[target_idx_inner].y * 20;
                                action_result = CMBTFIRE::Fire_Ship_(MOX::_cur_ship, ship->x * 20, ship->y * 20, target_idx_inner, t_x, t_y, 0);
                                if (ship->owner == 11 /* Crystal */) {
                                    Crystal_Control_(target_idx_inner);
                                }
                            }
                        }
                    } else {
                        skip_fire = true;
                        advance_weapon = true;
                    }
                }

                if (skip_fire) {
                    advance_weapon = true;
                    ship->weapons[weapon_idx].active = -1;
                }

                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                RUSS::Mox_Sync_Update_();
                Ai_Stasis_Check_(ship->owner);
                if (advance_weapon) {
                    weapon_idx++;
                }
                if (weapon_idx >= 8) {
                    break;
                }
            } while (CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship) == 0);

            for (int i = 0; i < 8; ++i) {
                ship->weapons[i].active = 1;
            }
        }
    } while (ship->movement_left > 0 && (action_result != 0 || did_move != 0));

    if (COMBAT1::Combat_Ship_Is_Functional_(MOX::_cur_ship)) {
        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            int16_t boarding_type = Boarding_Action_Type_(MOX::_cur_ship, i);
            if (boarding_type != 0) {
                Resolve_Ai_Boarding_(MOX::_cur_ship, i, boarding_type);
            }
        }
    }

    if (fields::Get_Input_() == MOX::_auto_button) {
        Toggle_Auto_Combat_Flags_();
    }

    if (Ai_Self_Destruct_Check_(MOX::_cur_ship)) {
        ship->drive_current_hits = 0;
        int8_t old_ftl = ship->ftl_type;
        ship->ftl_type = 1;
        int16_t dead_flag = CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship);
        CMBTFIRE::Destroy_Ship_(MOX::_cur_ship, ship->x * 20, ship->y * 20, dead_flag);
        ship->ftl_type = old_ftl;
        MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
    }
}

int16_t __cdecl Retreat_Check_(int16_t player_idx) {
    static const uint32_t kThreatSentinel = 3000000000u;
    static const int32_t kThreatPercentScale = 100;
    int16_t retreat_result = 0;
    int32_t enemy_expected_damage = 0;
    int32_t enemy_total_hits = 0;
    int32_t player_expected_damage = 0;
    int32_t player_total_hits = 0;
    int16_t ship_remaining_hits[210] = {};

    if (player_idx < MAX_PLAYERS && MOX::_player[player_idx].objectives != 100 /* 'd' */ && MOX::_ai_retreat_flag == 1) {
        if (COMBAT1::Enemy_Dissipator_Exists_(player_idx) == 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (i == 0) {
                    if (CMBTFIRE::Does_Combat_Planet_Have_Defenses_() == 0) {
                        continue;
                    }
                }

                if (MOX::_combat_data[i].unit_status == 0 && MOX::_combat_data[i].is_captured == 0) {
                    int16_t hits = CMBTAI::Get_Total_Hits_(i);
                    ship_remaining_hits[i] = hits;

                    int32_t hit_value = hits;
                    if (i == 0) {
                        hit_value *= 3;
                    }

                    if (MOX::_combat_data[i].owner == player_idx) {
                        player_total_hits += hit_value;
                    } else {
                        enemy_total_hits += hit_value;
                    }
                }
            }

            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].unit_status == 0 && MOX::_combat_data[i].is_captured == 0) {
                    int16_t max_dmg = 0;
                    int16_t best_target_idx = -1;

                    for (int16_t j = 0; j < MOX::_combat_total_ship_count; ++j) {
                        if (j == 0) {
                            if (CMBTFIRE::Does_Combat_Planet_Have_Defenses_() == 0) {
                                continue;
                            }
                        }

                        if (MOX::_combat_data[j].unit_status == 0 && MOX::_combat_data[j].is_captured == 0) {
                            if (MOX::_combat_data[i].owner != MOX::_combat_data[j].owner) {
                                int16_t total_expected_dmg = 0;
                                for (int16_t slot = 0; slot < 8; ++slot) {
                                    total_expected_dmg += CMBTAI::Expected_Weapon_Damage_(i, slot, j, 0, 0);
                                }

                                if (total_expected_dmg > ship_remaining_hits[j]) {
                                    total_expected_dmg = ship_remaining_hits[j];
                                }

                                if (total_expected_dmg > max_dmg) {
                                    max_dmg = total_expected_dmg;
                                    best_target_idx = j;
                                }
                            }
                        }
                    }

                    if (max_dmg > 0) {
                        ship_remaining_hits[best_target_idx] -= max_dmg;
                        if (MOX::_combat_data[i].owner == player_idx) {
                            player_expected_damage += max_dmg;
                        } else {
                            enemy_expected_damage += max_dmg;
                        }
                    }
                }
            }

            uint32_t player_threat = kThreatSentinel;
            if (enemy_total_hits != 0) {
                player_threat = (uint32_t)((player_expected_damage * kThreatPercentScale) / enemy_total_hits);
            }

            uint32_t enemy_threat = kThreatSentinel;
            if (player_total_hits != 0) {
                enemy_threat = (uint32_t)((enemy_expected_damage * kThreatPercentScale) / player_total_hits);
            }

            if (player_threat * 5 < enemy_threat) {
                retreat_result = 1;
            }
        }
    }

    return retreat_result;
}

int16_t __cdecl Get_Long_Range_Beam_Strength_(int16_t player_idx) {
    int16_t total_strength = 0;

    for (int16_t ship_idx = 0; ship_idx < MOX::_combat_total_ship_count; ++ship_idx) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        if (ship->unit_status == 0 && ship->is_captured == 0 && ship->owner == player_idx) {
            for (int16_t slot_idx = 0; slot_idx < 8; ++slot_idx) {
                int16_t weapon_id = ship->weapons[slot_idx].weapon_id;

                // Weapons with rack type 0 (beams) or weapon ID 19 (Mauler Device)
                if (TECHDATA::_weapons[weapon_id].type == 0 || weapon_id == 19 /* WEAPON_MAULER_DEVICE */) {
                    bool is_long_range = false;

                    if ((TECHDATA::_weapons[weapon_id].natural_mods & 4 /* W_CONT_BEAM */) != 0) {
                        is_long_range = true;
                    } else if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 15 /* SPEC_RANGE_FINDER */) != 0) {
                        is_long_range = true;
                    } else if ((ship->weapons[slot_idx].specials & 0x20 /* WSPEC_MOD_CONTINUOUS */) != 0) {
                        is_long_range = true;
                    }

                    bool add_damage = false;
                    if (is_long_range) {
                        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 25 /* SPEC_STRUCT_SCANNER */) != 0) {
                            add_damage = true;
                        }
                    }

                    if (!add_damage) {
                        // Heavy mount check or Mauler Device fallback
                        if ((ship->weapons[slot_idx].specials & 2 /* WSPEC_MOD_HEAVY */) != 0 || weapon_id == 19) {
                            add_damage = true;
                        }
                    }

                    if (add_damage) {
                        total_strength += CMBTAI::Get_Effective_Weapon_Damage_(ship_idx, slot_idx);
                    }
                }
            }
        }
        RUSS::Mox_Update_();
    }

    return total_strength;
}

int16_t __cdecl Get_Effective_Beam_Strength_(int16_t player_idx) {
    int16_t total_strength = 0;
    int16_t ship_idx = 0;

    if (MOX::_combat_total_ship_count > 0) {
        for (ship_idx = 0; ship_idx < MOX::_combat_total_ship_count; ++ship_idx) {
            s_combat_data* ship = &MOX::_combat_data[ship_idx];

            if (ship->unit_status == 0 && ship->is_captured == 0 && ship->owner == player_idx) {
                for (int16_t weapon_idx = 0; weapon_idx < 8; ++weapon_idx) {
                    int16_t weapon_id = ship->weapons[weapon_idx].weapon_id;

                    // Check if weapon is a beam (rack == 0) or specific weapon 0x13
                    if (TECHDATA::_weapons[weapon_id].type == 0 || weapon_id == 0x13) {
                        total_strength += CMBTAI::Get_Effective_Weapon_Damage_(ship_idx, weapon_idx);
                    }
                }
            }
            RUSS::Mox_Update_();
        }
    }

    return total_strength;
}

/**
 * @brief Selects the best ship to target for a given ship.
 * @param ship_idx The index of the ship selecting a target.
 * @return The index of the best target ship, or -1 if no suitable target is found.
 */
int16_t __cdecl Select_Ship_To_Target_(int16_t ship_idx) {
    int16_t best_target_idx = -1;
    int32_t best_value = -5000;

    if (MOX::_combat_total_ship_count > 1) {
        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
            // Only target active ships that are enemies, not in stasis, and not cloaked/disabled (flag 4)
            if (MOX::_combat_data[i].unit_status == 0 &&
                MOX::_combat_data[ship_idx].owner != MOX::_combat_data[i].owner &&
                MOX::_combat_data[i].special_status_flag != 4 &&
                !CMBTMOV1::Ship_Is_In_Stasis_(i)) {

                int32_t value = Target_Ship_Value_(ship_idx, i, 0);
                if (value > best_value) {
                    best_value = value;
                    best_target_idx = i;
                }
            }
        }
    }

    return best_target_idx;
}

/**
 * @brief Chooses a target for the given ship in combat.
 * @param ship_idx The index of the ship in the combat data array.
 * @return The index of the selected target ship, or 0 in specific edge cases where no ship is found.
 */
int16_t __cdecl Choose_Target_(int16_t ship_idx) {
    MOX::_combat_data[ship_idx].target_type_flag = 0;

    int16_t target_idx = CMBTAI::Select_Ship_To_Target_(ship_idx);

    if (target_idx == -1) {
        // Special case: if no target is found, and the first ship in the combat list
        // is in a specific unit status (0), and the current ship belongs to the attacker,
        // default the target to ship 0.
        if (MOX::_combat_data[0].unit_status == 0 &&
            MOX::_combat_data[ship_idx].owner == MOX::_combat_attacker_player) {
            target_idx = 0;
        }
    }

    return target_idx;
}

int16_t __cdecl Is_Ship_Out_Of_Ammo_(int16_t ship_idx) {
    int16_t out_of_ammo = 1;

    if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 37 /* 0x25 */) != 0) {
        out_of_ammo = 0;
    } else if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 38 /* 0x26 */) != 0) {
        out_of_ammo = 0;
    } else {
        for (int16_t slot_idx = 0; slot_idx < 8 && out_of_ammo == 1; ++slot_idx) {
            s_combat_weapon_slot& slot = MOX::_combat_data[ship_idx].weapons[slot_idx];

            if (slot.weapon_id != 0 && slot.count > 0) {
                int8_t rack_type = TECHDATA::_weapons[slot.weapon_id].type;

                switch (rack_type) {
                    case 0:
                    case 2:
                        // Standard weapons and torpedoes are not ammo-limited in AI logic or have internal replenishment
                        out_of_ammo = 0;
                        break;

                    case 1: // Missiles
                        if (slot.ammo > 0) {
                            out_of_ammo = 0;
                        }
                        break;

                    case 3: // Bombs
                        // Only count as having ammo if the ship is the attacker
                        if (slot.ammo > 0 && MOX::_combat_data[ship_idx].owner == MOX::_combat_attacker_player) {
                            out_of_ammo = 0;
                        }
                        break;

                    case 4: // Fighters
                        if (slot.ammo > 0) {
                            out_of_ammo = 0;
                        } else {
                            // Even if hangar is empty, if fighters are still in space, the ship has combat capacity
                            if (CMBTAI::Check_For_Launched_Fighters_(ship_idx) == 1) {
                                out_of_ammo = 0;
                            }
                        }
                        break;

                    case 5:
                        if (slot.weapon_id == WEAPON_ANTI_MISSILE_ROCKET) {
                            if (slot.ammo > 0) {
                                out_of_ammo = 0;
                            }
                        } else {
                            out_of_ammo = 0;
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    }

    return out_of_ammo;
}

int32_t __cdecl Target_Ship_Value_(int16_t attacker_idx, int16_t target_idx, int16_t distance_flag) {
    int16_t damage_dealt = 0;
    int16_t damage_received = 0;

    int16_t total_hits = CMBTAI::Get_Total_Hits_(target_idx);

    if (CMBTMOV1::Ship_Is_In_Stasis_(target_idx)) {
        return -20000;
    }

    int16_t range;
    if (distance_flag <= 0) {
        range = 0;
    } else {
        range = CMBTAI::Range_To_Ship_(attacker_idx, target_idx);
        if (range < 1) {
            range = 1;
        }
    }

    for (int16_t w = 0; w < 8; ++w) {
        int16_t expected_dmg = CMBTAI::Expected_Weapon_Damage_(attacker_idx, w, target_idx, 0, range);
        if (target_idx != 0 && TECHDATA::_weapons[MOX::_combat_data[attacker_idx].weapons[w].weapon_id].type == 4) {
            expected_dmg *= 2;
        }
        damage_dealt += expected_dmg;

        if (MOX::_combat_data[target_idx].is_captured == 0) {
            damage_received += CMBTAI::Expected_Weapon_Damage_(target_idx, w, attacker_idx, 0, 0);
        }
    }

    if (damage_dealt == 0) {
        return -10000;
    }

    uint32_t value_num = (uint32_t)((int64_t)damage_dealt * damage_received * 3);
    int32_t value = (int32_t)(value_num / (uint32_t)total_hits);

    if (distance_flag != 0) {
        if (target_idx != 0) {
            if (MOX::_combat_data[target_idx].shield_current_hits > 0) {
                int16_t bearing = CMBTDRW1::Relative_Bearing_(target_idx, attacker_idx);
                int16_t shield_idx = CMBTFIRE::Facing_Shield_(bearing);
                int16_t shield_hp = MOX::_combat_data[target_idx].shield_arc_current[shield_idx];

                if (damage_dealt < shield_hp / 4) {
                    value = 1;
                } else if (damage_dealt < shield_hp) {
                    value /= 2;
                    if (value < 2) {
                        value = 2;
                    }
                }
            }
        }
    }

    if (target_idx == 0) {
        if (damage_dealt > total_hits) {
            value += damage_received * 3;
        }
    } else {
        if (damage_dealt > total_hits / 3) {
            value += damage_received * 5;
        }
    }

    if (CMBTMOV1::Ship_In_Black_Hole_(target_idx)) {
        value /= 2;
    }

    if (CMBTAI::Missile_Overkill_Check_(target_idx) == 1) {
        value -= 1000;
    }

    if (damage_received == 0) {
        value -= 2000;
    }

    if (CMBTFIR2::Being_Captured_(target_idx) != 0) {
        value -= 5000;
    }

    return value;
}

int8_t __cdecl Combat_Ship_Class_(int16_t ship_idx, int16_t target_idx) {
    int16_t dmg_short_range = 0;
    int16_t dmg_med_range = 0;
    int16_t dmg_long_range = 0;

    for (int16_t slot = 0; slot < 8; ++slot) {
        int16_t weapon_id = MOX::_combat_data[ship_idx].weapons[slot].weapon_id;
        if (weapon_id == 0) {
            continue;
        }

        int8_t rack = TECHDATA::_weapons[weapon_id].type;
        switch (rack) {
        case 0: {
            int16_t d1 = Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 1);
            int16_t d5 = Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 5);
            int16_t d12 = Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 12);

            if (d5 / 2 < d12) {
                dmg_long_range += d12;
            } else if (d1 / 2 < d5) {
                dmg_med_range += d5;
            } else {
                dmg_short_range += d1;
            }
            break;
        }
        case 1:
            if (MOX::_combat_turn == 1) {
                dmg_med_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 5);
            } else {
                dmg_long_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 12);
            }
            break;
        case 2:
            if (weapon_id == WEAPON_PLASMA_TORPEDO || weapon_id == WEAPON_DRAGON_BREATH) {
                dmg_short_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 1);
            } else if (weapon_id == WEAPON_PROTON_TORPEDO) {
                dmg_med_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 5);
            } else {
                dmg_long_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 12);
            }
            break;
        case 3:
            if (target_idx == 0) {
                dmg_short_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 0);
            }
            break;
        case 4:
            dmg_short_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 1);
            break;
        case 5:
            if (weapon_id == WEAPON_PULSAR || weapon_id == WEAPON_PLASMA_FLUX || weapon_id == WEAPON_STASIS_FIELD ||
                weapon_id == WEAPON_SPATIAL_COMPRESSOR || weapon_id == WEAPON_TRACTOR_BEAM) {
                dmg_short_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 1);
            } else if (weapon_id == WEAPON_GYRO_DESTABILIZER || weapon_id == WEAPON_CAUSTIC_SLIME ||
                       weapon_id == WEAPON_BLACK_HOLE_GENERATOR || weapon_id == WEAPON_PLASMA_WEB) {
                dmg_med_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 1);
            } else if (weapon_id == WEAPON_STELLAR_CONVERTER) {
                dmg_long_range += Expected_Weapon_Damage_(ship_idx, slot, target_idx, 0, 12);
            }
            break;
        default:
            break;
        }
    }

    int8_t owner = MOX::_combat_data[MOX::_cur_ship].owner;
    if (MOX::_player_mode[owner] == 0 || MOX::_player_mode[owner] == 1) {
        dmg_short_range *= 2;
    }

    int32_t short_3_2 = (((int32_t)dmg_short_range) * 3) / 2;
    if (short_3_2 <= (int32_t)dmg_long_range) {
        return ((((int32_t)dmg_med_range) * 3) / 2 <= (int32_t)dmg_long_range) + 1;
    } else {
        return (int32_t)dmg_med_range >= short_3_2;
    }
}

bool __cdecl Does_Ship_Have_Missiles_Incoming_(int16_t ship_idx) {
    bool has_incoming = false;

    for (int32_t i = 0; i < 300; ++i) {
        s_missile* missile = &MOX::_missile[i];

        if (missile->target_unit_idx == ship_idx && missile->type > 0 && missile->is_anti_missile_rocket == 0) {
            if (CMBTAI::Will_Missile_Do_Damage_(ship_idx, (int16_t)i)) {
                // Check if the missile is hostile to the ship owner
                if (MOX::_combat_data[ship_idx].owner != CMBTMIS::Missile_Owner_((int16_t)i)) {
                    has_incoming = true;
                }
            }
        }
    }

    return has_incoming;
}

int16_t __cdecl Auto_Move_Ship_(int16_t ship_idx, int16_t target_idx) {
    int16_t moved = 0;
    int16_t num_possible_moves = 0;
    int16_t cur_ship_idx = MOX::_cur_ship;
    s_combat_data* cur_ship = &MOX::_combat_data[cur_ship_idx];

    if (MOX::_player_mode[cur_ship->owner] == -1) {
        cur_ship->ai_range_tactic = 2;
    }

    bool skip_ai_movement = false;
    if (cur_ship->target_type_flag == 0 /* TARGET_TYPE_SHIP */) {
        int8_t mode = MOX::_player_mode[cur_ship->owner];
        if (mode == 3 || mode == 2) {
            if (MOX::_combat_turn != 1 || TECHDATA::_weapons[cur_ship->weapons[0].weapon_id].type != 1) {
                skip_ai_movement = true;
            }
        }
    }

    MOX::_auto_msl_warning_off = 0;

    int16_t move_y_coords[20];
    int16_t move_x_coords[20];

    if (!skip_ai_movement) {
        int16_t target_x;
        int16_t target_y;

        if (cur_ship->target_type_flag == 1 /* TARGET_TYPE_MISSILE */) {
            target_y = MOX::_missile[target_idx].y / 20;
            target_x = MOX::_missile[target_idx].x / 20;
        } else {
            target_y = (int16_t)MOX::_combat_data[target_idx].y;
            target_x = (int16_t)MOX::_combat_data[target_idx].x;
        }

        int16_t dist = special::Range_((int16_t)cur_ship->x, (int16_t)cur_ship->y, target_x, target_y);
        int16_t score_limit = 0;

        if (cur_ship->ai_range_tactic == 0) {
            score_limit = dist - 3;
        } else if (cur_ship->ai_range_tactic == 1) {
            score_limit = dist - 15;
        } else if (cur_ship->ai_range_tactic == 2) {
            score_limit = dist - 50;
        }

        if (score_limit > 0) {
            for (int16_t gx = 0; gx < 81; ++gx) {
                for (int16_t gy = 0; gy < 68; ++gy) {
                    if (MOX::_combat_grid[gx][gy] == 2000) {
                        int16_t dist_from_hex;
                        if (cur_ship->target_type_flag == 1 /* TARGET_TYPE_MISSILE */) {
                            dist_from_hex = special::Range_(gx, gy, MOX::_missile[target_idx].x / 20, MOX::_missile[target_idx].y / 20);
                        } else {
                            dist_from_hex = special::Range_(gx, gy, (int16_t)MOX::_combat_data[target_idx].x, (int16_t)MOX::_combat_data[target_idx].y);
                        }

                        score_limit = CMBTAI::Evaluate_Possible_Move_(
                            dist_from_hex,
                            gx,
                            gy,
                            (int16_t)MOX::_combat_data[ship_idx].ai_range_tactic,
                            &num_possible_moves,
                            move_x_coords,
                            move_y_coords,
                            score_limit
                        );
                    }
                }
            }
        }

        if (num_possible_moves > 0) {
            int rnd_idx = game_random::Random_((int)num_possible_moves) - 1;
            int16_t mx = move_x_coords[rnd_idx];
            int16_t my = move_y_coords[rnd_idx];
            int16_t move_cost = CMBTMOV1::Move_Cost_(cur_ship_idx, mx, my);
            int16_t movement_used = CMBTMOV1::Move_Ship_(cur_ship_idx, mx, my, move_cost);

            if (movement_used > 0) {
                moved = 1;
                cur_ship->movement_left -= movement_used;
            }
        }
    }

    int16_t final_target_x;
    int16_t final_target_y;

    if (cur_ship->target_type_flag == 0 /* TARGET_TYPE_SHIP */) {
        final_target_y = (int16_t)MOX::_combat_data[target_idx].y;
        final_target_x = (int16_t)MOX::_combat_data[target_idx].x;
    } else {
        final_target_y = MOX::_missile[target_idx].y / 20;
        final_target_x = MOX::_missile[target_idx].x / 20;
    }

    CMBTMOV1::Turn_To_Face_(cur_ship_idx, final_target_x, final_target_y);
    MOX::_auto_msl_warning_off = 0;

    return moved;
}

int16_t __cdecl Boarding_Action_Type_(int16_t attacker_idx, int16_t target_idx) {
    s_invasion_side_info attacker_invasion;
    s_invasion_side_info target_invasion;
    int16_t result = 0;
    bool can_board = true;
    int16_t attacker_owner = MOX::_combat_data[attacker_idx].owner;

    if (target_idx == -1) {
        return 0;
    }

    int16_t dist = Range_To_Ship_(attacker_idx, target_idx);
    int16_t reach = COMBINIT::Does_Combat_Ship_Have_Special_(attacker_idx, SPECIAL_TRANSPORTERS) ? 4 : 1;

    if (MOX::_combat_data[target_idx].owner > 8 ||
        attacker_owner > 8 ||
        !COMBAT1::Combat_Ship_Is_Functional_(target_idx) ||
        MOX::_combat_data[target_idx].special_status_flag == 4 ||
        CMBTFIR2::Already_Boarded_(attacker_idx) != 0 ||
        target_idx == 0 ||
        MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_data[target_idx].owner ||
        dist > reach ||
        (reach != 4 && COMBAT1::Calc_Current_Speed_(target_idx) > 0) ||
        MOX::_combat_data[MOX::_cur_ship].marine_count == 0) {
        
        can_board = false;
    } else if (reach == 4) {
        if (dist != 1 || MOX::_combat_data[target_idx].current_speed > 0) {
            int16_t bearing = CMBTDRW1::Relative_Bearing_(target_idx, attacker_idx);
            int16_t shield_idx = CMBTFIRE::Facing_Shield_(bearing);
            if (MOX::_combat_data[target_idx].shield_arc_current[shield_idx] > 0 ||
                COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, SPECIAL_HARD_SHIELDS) != 0) {
                can_board = false;
            }
        }
    }

    if (can_board) {
        int16_t opp_player = (attacker_owner == MOX::_combat_attacker_player) ? MOX::_combat_defender_player : MOX::_combat_attacker_player;

        if (MOX::_combat_attacker_player < 9 && MOX::_combat_defender_player < 9 && CMBTFIR2::Already_Boarded_(attacker_idx) == 0) {
            if (attacker_owner == 8 /* Antarans */) {
                result = 1;
            } else {
                invasion::Get_Boarding_Info_(attacker_owner, 1, 0, opp_player, 1, 0, &attacker_invasion, &target_invasion);
                
                int16_t attacker_total = CMBTFIR2::Crew_Boarding_Combat_Bonus_(attacker_idx) + 
                                         CMBTFIR2::Get_Fleet_Commando_Bonus_(attacker_owner) + 
                                         attacker_invasion.unit_strengths[1];
                                         
                int16_t target_total = CMBTFIR2::Crew_Boarding_Combat_Bonus_(target_idx) + 
                                       (COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, SPECIAL_SECURITY_STATIONS) ? 20 : 0) + 
                                       CMBTFIR2::Get_Fleet_Commando_Bonus_(opp_player) + 
                                       CMBTFIR2::Get_Fleet_Security_Bonus_(opp_player) + 
                                       target_invasion.unit_strengths[1];

                if (target_total - 20 < attacker_total) {
                    if (MOX::_player[attacker_owner].traits[TRAIT_TELEPATHIC] != 0) {
                        result = 2;
                    } else {
                        result = 1;
                        if (MOX::_combat_data[target_idx].marine_count <= MOX::_combat_data[attacker_idx].marine_count && 
                            target_total - 10 < attacker_total) {
                            result = 2;
                        }
                    }
                }
            }
        } else {
            result = 0;
        }
    }

    return result;
}

void __cdecl Resolve_Ai_Boarding_(int16_t attacker_idx, int16_t target_idx, int16_t action_type) {
    if (action_type != 0) {
        int16_t dist = Range_To_Ship_(attacker_idx, target_idx);
        bool has_transporters = (COMBINIT::Does_Combat_Ship_Have_Special_(attacker_idx, SPECIAL_TRANSPORTERS) == 1);

        s_combat_data* attacker = &MOX::_combat_data[attacker_idx];
        int16_t marines_to_send = (uint8_t)attacker->marine_count >> 1;
        if (marines_to_send == 0) {
            marines_to_send = 1;
        }
        attacker->marine_count -= (int8_t)marines_to_send;

        if (has_transporters) {
            bool skip_displacement = false;
            if (dist == 1) {
                if (MOX::_combat_data[target_idx].current_speed <= 0) {
                    skip_displacement = true;
                }
            }

            if (!skip_displacement) {
                if (COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, SPECIAL_DAMPER_FIELD) != 0) {
                    marines_to_send /= 3;
                    if (marines_to_send < 1) {
                        action_type = 0;
                    }
                }
            }
        }

        if (MOX::_net_flag != 0) {
            if ((action_type == 1 || action_type == 2) && (MOX::_PLAYER_NUM != MOX::_net_destination)) {
                COMBAT1::Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_BOARDING_ACTION, target_idx, marines_to_send, action_type);
            }
        }

        if (action_type == 1 /* RAID */) {
            CMBTFIR2::Set_Boarded_Flag_(attacker_idx);
            CMBTFIR2::Resolve_Raid_(attacker_idx, target_idx, &marines_to_send);
            attacker->marine_count += (int8_t)marines_to_send;
        } else if (action_type == 2 /* CAPTURE */) {
            CMBTFIR2::Set_Boarded_Flag_(attacker_idx);
            CMBTFIR2::Set_Capturing_Flag_(target_idx);
            s_combat_data* target = &MOX::_combat_data[target_idx];
            CMBTFIR2::Resolve_Capture_(attacker_idx, target_idx, marines_to_send, target->x * 20, target->y * 20);
        }

        MOX::_board_flag = 0;
        if (MOX::_net_flag != 0) {
            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
        }
    }
}

/**
 * Checks if a specific ship is the target of any active offensive missile.
 * 
 * @param ship_idx The index of the ship to check.
 * @return 1 if the ship is targeted by a missile that is not an anti-missile rocket and not fired by itself; 0 otherwise.
 */
int16_t __cdecl Is_Ship_Targeted_By_Missile_(int16_t ship_idx) {
    int16_t result = 0;

    // Iterate through the missile array (fixed size of 300)
    for (int16_t i = 0; i < 300; ++i) {
        // Early exit if a match was already found in a previous iteration
        if (result != 0) {
            break;
        }

        // A missile is targeting the ship if:
        // 1. Its target matches the ship index
        // 2. It is not an anti-missile rocket (which targets other missiles/fighters)
        // 3. The ship did not fire the missile at itself (source index check)
        if (MOX::_missile[i].target_unit_idx == ship_idx &&
            MOX::_missile[i].is_anti_missile_rocket == 0 &&
            MOX::_missile[i].source_unit_idx != ship_idx) {
            result = 1;
        }
    }

    return result;
}

int16_t __cdecl Range_To_XY_(int16_t ship_idx, int16_t target_x_px, int16_t target_y_px) {
    int16_t off_x, off_y;
    COMBINIT::Ship_Center_Offsets_(ship_idx, &off_x, &off_y);

    int16_t off_gx = off_x / 20;
    int16_t off_gy = off_y / 20;

    int16_t ship_gx = MOX::_combat_data[ship_idx].x;
    int16_t ship_gy = MOX::_combat_data[ship_idx].y;

    int16_t target_gx = target_x_px / 20;
    int16_t target_gy = target_y_px / 20;

    int16_t best_ship_x;
    if (target_gx < ship_gx) {
        best_ship_x = ship_gx - off_gx;
        if (best_ship_x <= target_gx) {
            best_ship_x = target_gx;
        }
    } else {
        best_ship_x = ship_gx + off_gx;
        if (best_ship_x >= target_gx) {
            best_ship_x = target_gx;
        }
    }

    int16_t best_ship_y;
    if (target_gy < ship_gy) {
        best_ship_y = ship_gy - off_gy;
        if (best_ship_y <= target_gy) {
            best_ship_y = target_gy;
        }
    } else {
        best_ship_y = ship_gy + off_gy;
        if (best_ship_y >= target_gy) {
            best_ship_y = target_gy;
        }
    }

    // The following comparisons are present in the disassembly as identity assignments
    if (target_gx == best_ship_x) { target_gx = best_ship_x; }
    if (target_gy == best_ship_y) { target_gy = best_ship_y; }

    int16_t dist = special::Range_(best_ship_x, best_ship_y, target_gx, target_gy);

    // Result is rounded up and scaled down
    return (dist + 2) / 3;
}

/**
 * @brief Checks if there are any active fighter-type missiles/units launched by the specified unit.
 * 
 * @param source_unit_idx The index of the ship or unit that launched the fighters.
 * @return 1 if fighters from this source are found, 0 otherwise.
 */
int16_t __cdecl Check_For_Launched_Fighters_(int16_t source_unit_idx) {
    int16_t found = 0;

    for (int16_t i = 0; i < 300; ++i) {
        // Check if we found it in a previous iteration
        if (found != 0) {
            break;
        }

        // Check if the missile/fighter belongs to the specified source unit
        if (MOX::_missile[i].source_unit_idx == source_unit_idx) {
            // Check if the weapon type associated with this missile is a fighter rack
            // rack type 4 corresponds to WEAPON_RACK_FIGHTER
            if (TECHDATA::_weapons[MOX::_missile[i].type].type == 4 /* WEAPON_RACK_FIGHTER */) {
                found = 1;
            }
        }
    }

    return found;
}

void __cdecl Release_Smallest_Ship_From_Stasis_(int16_t owner_idx) {
    int16_t smallest_ship_idx = -1;
    int16_t min_size_class = 100;

    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
        s_combat_data* ship = &MOX::_combat_data[i];

        if (ship->unit_status == 0 && ship->is_captured == 0 && ship->owner == owner_idx) {
            if (CMBTMOV1::Ship_Is_In_Stasis_(i)) {
                if (ship->size_class < min_size_class) {
                    min_size_class = ship->size_class;
                    smallest_ship_idx = i;
                }
            }
        }
    }

    if (smallest_ship_idx != -1) {
        CMBTFIRE::Remove_Stasis_Beam_(smallest_ship_idx);
    }
}

/**
 * @brief Evaluates a possible move score against the current best score and updates the best moves list.
 * 
 * @param score The raw score of the move being evaluated.
 * @param move_x First value associated with the move (for example, an X coordinate).
 * @param move_y Second value associated with the move (for example, a Y coordinate).
 * @param move_type Determines the base offset to subtract from the score.
 * @param best_move_count Pointer to the current count of tied best moves.
 * @param best_move_xs Array storing the first values of the tied best moves.
 * @param best_move_ys Array storing the second values of the tied best moves.
 * @param best_score The current best score found so far.
 * @return The updated best score.
 */
int16_t __cdecl Evaluate_Possible_Move_(int16_t score, int16_t move_x, int16_t move_y, int16_t move_type, int16_t* best_move_count, int16_t* best_move_xs, int16_t* best_move_ys, int16_t best_score) {
    int16_t base_val;

    if (move_type == 0) {
        base_val = 3;
    }
    else if (move_type == 1) {
        base_val = 15; /* 0xF */
    }
    else if (move_type == 2) {
        base_val = 50; /* 0x32 */
    }
    else {
        // In the original binary, if move_type is not 0, 1, or 2, 
        // base_val is loaded from an uninitialized stack location.
    }

    int16_t eval_score = score - base_val;

    // Only consider moves with non-negative adjusted scores.
    if (eval_score >= 0) {
        if (eval_score < best_score) {
            // Found a new strictly better score: reset the ties list.
            *best_move_count = 0;
            best_move_xs[0] = move_x;
            best_move_ys[0] = move_y;
            (*best_move_count)++;
            return eval_score;
        }
        else if (eval_score == best_score) {
            // Found a move with an equivalent score: add it to the ties list if space remains.
            if (*best_move_count < 20 /* 0x14 */) {
                best_move_xs[*best_move_count] = move_x;
                best_move_ys[*best_move_count] = move_y;
                (*best_move_count)++;
            }
            return best_score;
        }
    }

    return best_score;
}

void __cdecl Crystal_Control_(int16_t ship_idx) {
    char msg_buffer[80];

    if (ship_idx != 0) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        if (ship->unit_status == 0 && ship->marine_count == 0) {
            CMBTFIR2::Capture_Ship_(ship_idx, (int16_t)ship->x * 20, (int16_t)ship->y * 20);

            ship->is_captured = 0;

            KEN::Ken_Get_Text_Message_(0x4c, msg_buffer, 0x50);
            strlcat(msg_buffer, ship->name, sizeof(msg_buffer));

            palette::Refresh_Palette_();
            GENDRAW::Message_Box_(msg_buffer);
            CMBTDRW1::Refresh_Combat_Screen_Full_();
        }
    }
}

/**
 * @brief Evaluates and selects the best target (ship or missile) for the current unit.
 *
 * @param ship_idx Index of the ship performing targeting.
 * @param reference_target_idx Used as a target ship index filter/limit in human modes.
 * @return int16_t The index of the selected target.
 */
int16_t __cdecl Get_Current_Target_(int16_t ship_idx, int16_t reference_target_idx) {
    int32_t missile_score;
    int32_t ship_score;
    int16_t missile_target;
    int16_t ship_target;
    int8_t owner_idx;

    owner_idx = MOX::_combat_data[ship_idx].owner;

    if (MOX::_player_mode[owner_idx] != 1 && MOX::_player_mode[owner_idx] != 3) {
        // AI Logic: Prioritize targets based on weighted scores
        missile_target = Get_Current_Missile_Target_(ship_idx, &missile_score);
        ship_target = Get_Current_Ship_Target_(ship_idx, -1, &ship_score);

        if (ship_score / 2 > missile_score) {
            MOX::_combat_data[ship_idx].target_type_flag = 0 /* Ship */;
            return ship_target;
        } else {
            MOX::_combat_data[ship_idx].target_type_flag = 1 /* Missile */;
            return missile_target;
        }
    } else {
        // Human/Manual Logic
        ship_target = Get_Current_Ship_Target_(ship_idx, reference_target_idx, &ship_score);
        missile_target = Get_Current_Missile_Target_(ship_idx, &missile_score);

        if (missile_score / 2 > ship_score) {
            MOX::_combat_data[ship_idx].target_type_flag = 1 /* Missile */;
            return missile_target;
        } else {
            MOX::_combat_data[ship_idx].target_type_flag = 0 /* Ship */;
            return ship_target;
        }
    }
}

int16_t __cdecl Get_Current_Missile_Target_(int16_t unit_idx, int32_t* value_out) {
    int32_t best_value = -5000;
    int16_t best_missile_idx = -1;

    MOX::_combat_data[unit_idx].target_type_flag = 0; /* ship */

    for (int16_t i = 0; i < 300; ++i) {
        s_missile* missile = &MOX::_missile[i];

        if (missile->type > 0 && missile->quantity > 0) {
            if (MOX::_combat_data[missile->source_unit_idx].owner != MOX::_combat_data[unit_idx].owner) {
                if (CMBTAI::Will_Missile_Do_Damage_(unit_idx, i)) {
                    int8_t rack = TECHDATA::_weapons[missile->type].type;
                    // Filters for specific weapon racks (possibly missile/torpedo variants)
                    if (rack == 1 || rack == 4) {
                        int32_t current_value = CMBTAI::Target_Missile_Value_(unit_idx, i, 1);
                        if (best_value < current_value) {
                            best_missile_idx = i;
                            best_value = current_value;
                        }
                    }
                }
            }
        }
    }

    if (best_missile_idx == -1) {
        *value_out = -20000;
    } else {
        *value_out = best_value;
    }

    return best_missile_idx;
}

int16_t __cdecl Get_Total_Hits_(int16_t ship_idx) {
    int16_t total_hits = 0;

    if (ship_idx != 0) {
        int16_t internal_space = CMBTFIR2::Total_Internal_Space_(ship_idx);
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        total_hits = ship->armor_remaining;
        total_hits += internal_space;
        total_hits += ship->structure_max;
        total_hits -= ship->structure_damage;
        total_hits += COMBINIT::Calc_Combat_Shield_Max_(ship_idx);
    }
    else {
        int16_t owner = MOX::_combat_data[0].owner;
        int16_t best_armor_idx = INITSHIP::Best_Armor_(owner);
        int16_t absorption = TECHDATA::_armor[best_armor_idx].ships_bonus;

        // defense_value = ((absorption * 4 + 400) * 25) / 100
        int16_t defense_value = (int16_t)(((absorption * 4 + 400) * 25) / 100);
        int16_t planet_defenses = CMBTFIRE::Does_Combat_Planet_Have_Defenses_();

        for (int16_t i = 0; i < 4; ++i) {
            if (planet_defenses & (1 << i)) {
                total_hits += defense_value;
            }
        }
    }

    return total_hits;
}

int16_t __cdecl Get_Effective_Weapon_Damage_(int16_t unit_idx, int16_t slot_idx) {
    s_combat_data* unit = &MOX::_combat_data[unit_idx];
    s_combat_weapon_slot& slot = unit->weapons[slot_idx];

    int16_t weapon_id = slot.weapon_id;
    uint16_t specials = slot.specials;

    // Note: The following damage calculation is performed in the original binary,
    // but the result in EAX is immediately overwritten by the return value.
    // We preserve it here to maintain semantic equivalence with the assembly.
    int16_t damage = TECHDATA::_weapons[weapon_id].max_damage;

    if (specials & 0x4) { /* Point Defense */
        damage = (damage + 1) / 2;
    }

    if (specials & 0x2) { /* Heavy Mount */
        damage = (damage * 3) / 2;
    }

    // MOVSX AX, byte ptr [ECX + 0x54]
    // The function returns the 'count' byte (offset 0x2 in slot), sign-extended to short.
    return (int16_t)(int8_t)slot.count;
}

/**
 * Finds the best target for the current ship based on distance, ship values, and weapon availability.
 * Returns the target ship index and updates out_value with the target's relative priority score.
 */
int16_t __cdecl Get_Current_Ship_Target_(int16_t current_ship_idx, int16_t reference_target_idx, int32_t* out_value) {
    int16_t best_target_idx = -1;
    int32_t best_value = -5000;

    int16_t start_idx = (CMBTFIRE::Does_Combat_Planet_Have_Defenses_() == 0) ? 1 : 0;

    for (int16_t target_idx = start_idx; target_idx < MOX::_combat_total_ship_count; ++target_idx) {
        if (MOX::_combat_data[target_idx].unit_status == 0 &&
            MOX::_combat_data[current_ship_idx].owner != MOX::_combat_data[target_idx].owner &&
            MOX::_combat_data[target_idx].special_status_flag != 4 &&
            CMBTMOV1::Ship_Is_In_Stasis_(target_idx) == 0) {
            
            bool weapon_found = false;

            for (int16_t weapon_idx = 0; weapon_idx < 8; ++weapon_idx) {
                s_combat_weapon_slot *weapon = &MOX::_combat_data[current_ship_idx].weapons[weapon_idx];
                
                if (weapon->weapon_id != 0) {
                    int16_t bearing = CMBTDRW1::Relative_Bearing_(current_ship_idx, target_idx);
                    int8_t arc_mask = CMBTDRW1::Get_Weapon_Arc_(bearing);
                    
                    int8_t rack = TECHDATA::_weapons[weapon->weapon_id].type;
                    
                    if (COMBAT1::Weapon_Is_On_(current_ship_idx, weapon_idx) != 0) {
                        if (weapon->shots_left > 0) {
                            if ((arc_mask & weapon->arc) != 0) {
                                if (rack == 2) {
                                    if (weapon->ammo != 0) {
                                        continue;
                                    }
                                } else {
                                    if (weapon->ammo == 0) {
                                        continue;
                                    }
                                }
                                
                                if (CMBTFIRE::Weapon_In_Range_(weapon_idx, current_ship_idx, target_idx, 0, 0) != 0) {
                                    weapon_found = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            if (weapon_found) {
                int32_t value = CMBTAI::Target_Ship_Value_(current_ship_idx, target_idx, 1);
                
                if (target_idx == reference_target_idx && value > 0) {
                    if (value <= 6000) {
                        value *= 10;
                    } else {
                        value = 60000;
                    }
                }
                
                if (CMBTAI::_fleet_target == target_idx && value > 0) {
                    int32_t old_value = value;
                    value *= 5;
                    if (old_value > 12000) {
                        value = 60000;
                    }
                }
                
                if (value > best_value) {
                    best_target_idx = target_idx;
                    best_value = value;
                }
            }
        }
    }

    if (best_target_idx == -1) {
        *out_value = -20000;
    } else {
        *out_value = best_value;
    }

    return best_target_idx;
}

int16_t __cdecl Missile_Overkill_Check_(int16_t target_idx) {
    int16_t total_damage = 0;
    int16_t overkill = 0;

    for (int16_t i = 0; i < 300; ++i) {
        s_missile* missile = &MOX::_missile[i];

        if (missile->target_unit_idx == target_idx &&
            missile->source_unit_idx != target_idx &&
            missile->type > 0) {

            int16_t source_owner = MOX::_combat_data[missile->source_unit_idx].owner;
            total_damage += CMBTAI::Expected_Missile_Damage_(
                target_idx,
                missile->type,
                missile->quantity,
                missile->specials,
                source_owner
            );
        }
    }

    if (target_idx == 0) {
        int16_t total_hits = CMBTAI::Get_Total_Hits_(0);
        if (total_damage > (total_hits * 3) / 2) {
            overkill = 1;
        }
    } else {
        int16_t total_hits = CMBTAI::Get_Total_Hits_(target_idx);
        if (total_damage > total_hits) {
            overkill = 1;
        }
    }

    return overkill;
}

int16_t __cdecl Expected_Missile_Damage_(int16_t target_ship_idx, int16_t weapon_id, int16_t weapon_count, uint16_t weapon_specials, int16_t attacker_player_idx) {
    int16_t shield_reduction = 0;
    int16_t damage;
    int16_t result_damage = 0;

    if (target_ship_idx == 0) {
        if (CMBTFIRE::Does_Combat_Planet_Have_Defenses_() != 0) {
            shield_reduction = MOX::_combat_data[0].shield_arc_current[0];
        } else {
            shield_reduction = MOX::_combat_data[0].shield_arc_current[1];
        }
    } else {
        s_combat_data* target_ship = &MOX::_combat_data[target_ship_idx];
        if (target_ship->shield_current_hits > 0) {
            shield_reduction = TECHDATA::_shields[target_ship->shield_type].shield_class;
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(target_ship_idx, SPECIAL_HARD_SHIELDS) != 0) {
            shield_reduction += 3;
        }
    }

    s_tech_weapon_data* weapon = &TECHDATA::_weapons[weapon_id];
    if (weapon->type == 4) {
        int16_t base_damage = 0;
        switch (weapon_id) {
            case 28:
                base_damage = 100;
                break;
            case 29:
            case 30:
                base_damage = TECHDATA::_weapons[MOX::_fighter_bomb_type[attacker_player_idx]].max_damage;
                break;
            case 31:
                base_damage = TECHDATA::_weapons[MOX::_fighter_beam_type[attacker_player_idx]].max_damage;
                break;
            default:
                break;
        }

        damage = (int16_t)((MOX::_ordnance_bonus[attacker_player_idx] * base_damage) / 100);
        int16_t volley_damage = (int16_t)((damage - shield_reduction) * weapon_count);
        result_damage = (int16_t)(volley_damage + volley_damage);
    } else {
        int16_t base_damage = weapon->max_damage;

        if ((weapon_specials & WEAPON_MOD_FLAG_OVERLOADED) != 0) {
            base_damage = (int16_t)((base_damage * 3) / 2);
        }

        if ((weapon_specials & WEAPON_MOD_FLAG_MIRV) != 0) {
            base_damage <<= 2;
        }

        damage = (int16_t)((MOX::_ordnance_bonus[attacker_player_idx] * base_damage) / 100);

        int16_t evade = MOX::_combat_data[target_ship_idx].tac_val_evade;
        if (attacker_player_idx < MAX_PLAYERS) {
            s_player* attacker = &MOX::_player[attacker_player_idx];
            if (attacker->tech_applications[TECH_APP_SENSORS] == 3) {
                evade -= 70;
            } else if (attacker->tech_applications[TECH_APP_NEUTRON_SCANNER] == 3) {
                evade -= 40;
            } else if (attacker->tech_applications[TECH_APP_TACHYON_SCANNER] == 3) {
                evade -= 20;
            }
        } else {
            evade -= 40;
        }

        if (weapon_id == WEAPON_DRAGON_BREATH) {
            evade = 0;
        }

        if (evade < 0) {
            evade = 0;
        } else if ((weapon_specials & WEAPON_MOD_FLAG_ECCM) != 0) {
            evade /= 2;
        }

        if ((weapon_id == WEAPON_PLASMA_TORPEDO || weapon_id == WEAPON_DRAGON_BREATH) && ((weapon_specials & WEAPON_MOD_FLAG_NO_RANGE_DISSIPATION) == 0)) {
            damage /= 2;
        }

        int16_t volley_damage = (int16_t)((damage - shield_reduction) * weapon_count);
        result_damage = (int16_t)(((100 - evade) * (int32_t)volley_damage) / 100);

        if (MOX::_combat_data[target_ship_idx].special_status_flag == 1) {
            result_damage /= 2;
        }
    }

    if (result_damage < 1) {
        result_damage = 0;
    }

    return result_damage;
}

int32_t __cdecl Target_Missile_Value_(int16_t unit_idx, int32_t missile_idx, int16_t range_sensitive) {
    int16_t range;
    int16_t m_idx = (int16_t)missile_idx;

    if (range_sensitive > 0) {
        range = Range_To_Missile_(unit_idx, m_idx);
        if (range < 1) {
            range = 1;
        }
    } else {
        range = 0;
    }

    int16_t total_expected_pd_damage = 0;
    for (int16_t weapon_idx = 0; weapon_idx < 8; ++weapon_idx) {
        total_expected_pd_damage += Expected_Weapon_Damage_(unit_idx, weapon_idx, m_idx, 1, range);
    }

    s_missile* missile = &MOX::_missile[m_idx];
    int16_t hits_to_kill = CMBTMIS::Get_Missile_Hits_(
        missile->type,
        MOX::_combat_data[missile->source_unit_idx].owner,
        missile->specials
    );

    int16_t effective_missile_hits = total_expected_pd_damage / hits_to_kill;
    if (missile->quantity < effective_missile_hits) {
        effective_missile_hits = missile->quantity;
    }

    int16_t weapon_damage;
    int16_t missile_type = missile->type;
    if (TECHDATA::_weapons[missile_type].type == 4 /* RACK_FIGHTER */) {
        int16_t source_owner = MOX::_combat_data[missile->source_unit_idx].owner;
        switch (missile_type) {
            case 0x1c: // Heavy Fighter
                weapon_damage = 100;
                break;
            case 0x1d: // Fighter Bomber 1
            case 0x1e: // Fighter Bomber 2
                // Fighter loadouts are stored for the 8 regular players plus the Antaran slot.
                if (source_owner >= 0 && source_owner < MAX_PLAYERS + 1) {
                    weapon_damage = TECHDATA::_weapons[MOX::_fighter_bomb_type[source_owner]].max_damage;
                } else {
                    weapon_damage = 0;
                }
                break;
            case 0x1f: // Fighter Interceptor/Beam
                if (source_owner >= 0 && source_owner < MAX_PLAYERS + 1) {
                    weapon_damage = TECHDATA::_weapons[MOX::_fighter_beam_type[source_owner]].max_damage;
                } else {
                    weapon_damage = 0;
                }
                break;
            default:
                weapon_damage = 0;
                break;
        }
    } else {
        weapon_damage = TECHDATA::_weapons[missile_type].max_damage;
    }

    int16_t shield_blocked;
    if (unit_idx == 0) {
        shield_blocked = MOX::_combat_data[0].shield_arc_current[0];
    } else {
        shield_blocked = TECHDATA::_shields[MOX::_combat_data[unit_idx].shield_type].shield_class;
    }

    if (COMBINIT::Does_Combat_Ship_Have_Special_(unit_idx, SPECIAL_DAMPER_FIELD)) {
        effective_missile_hits /= 4;
    }

    int32_t final_value = (int16_t)(uint16_t)((weapon_damage - shield_blocked) * effective_missile_hits);

    if (TECHDATA::_weapons[missile->type].type != 4) {
        final_value /= 2;
    }

    if (final_value < 1) {
        final_value = -10000;
    }

    if (!Will_Missile_Do_Damage_(missile->target_unit_idx, m_idx)) {
        final_value = -10000;
    }

    return final_value;
}

int16_t __cdecl Convert_Ordinal_To_Msl_Type_(int16_t ordinal) {
    switch (ordinal) {
        case 0:
            return 14 /* 0x0e */;
        case 1:
            return 15 /* 0x0f */;
        case 2:
            return 16 /* 0x10 */;
        case 3:
            return 17 /* 0x11 */;
        case 4:
            return 29 /* 0x1d */;
        case 5:
            return 30 /* 0x1e */;
        case 6:
            return 31 /* 0x1f */;
        case 7:
            return 18 /* 0x12 */;
        case 8:
            return 20 /* 0x14 */;
        case 9:
            return 38 /* 0x26 */;
        default:
            return ordinal;
    }
}

int16_t __cdecl Get_Effective_Missile_Strength_(int16_t player_idx, int16_t* beam_strength) {
    int16_t target_player;
    int16_t shield_points;
    int16_t missile_counts[11];
    int16_t total_quantity = 0;

    if (player_idx == MOX::_combat_attacker_player) {
        target_player = MOX::_combat_defender_player;
    } else {
        target_player = MOX::_combat_attacker_player;
    }

    int16_t best_shield = INITSHIP::Best_Ship_Shield_(target_player);
    if (best_shield == SPECIAL_DAMPER_FIELD) {
        shield_points = 100;
    } else {
        shield_points = TECHDATA::_shields[best_shield].shield_class;
    }

    *beam_strength = 0;
    struct_::Clear_Structure_(missile_counts, 0x16);

    for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
        if (MOX::_combat_data[i].unit_status == 0 && MOX::_combat_data[i].is_captured == 0 && MOX::_combat_data[i].owner == player_idx) {
            for (int16_t j = 0; j < 8; ++j) {
                int16_t weapon_id = MOX::_combat_data[i].weapons[j].weapon_id;
                if (weapon_id <= 0) continue;

                int8_t count = MOX::_combat_data[i].weapons[j].count;
                if (count <= 0) continue;

                int8_t rack = TECHDATA::_weapons[weapon_id].type;
                if (rack == 1 || rack == 4) {
                    if (MOX::_combat_data[i].weapons[j].ammo <= 0) continue;
                } else if (rack != 2) {
                    continue;
                }

                int16_t ordinal = -1;
                if (weapon_id >= 14 && weapon_id <= 31) {
                    static const uint8_t ordinal_map[] = { 0, 1, 2, 3, 9, 9, 7, 9, 8, 9, 9, 9, 9, 9, 9, 4, 5, 6 };
                    ordinal = ordinal_map[weapon_id - 14];
                }

                if (ordinal >= 0 && ordinal <= 6) {
                    if (ordinal >= 4) {
                        missile_counts[ordinal] += (count * 4);
                    } else {
                        missile_counts[ordinal] += count;
                    }
                    total_quantity += count;
                } else if (ordinal == 7 || ordinal == 8) {
                    missile_counts[ordinal] += count;
                }
            }
        }
        RUSS::Mox_Update_();
    }

    for (int16_t i = 0; i < 300; ++i) {
        s_missile* msl = &MOX::_missile[i];
        if (msl->type > 0 && TECHDATA::_weapons[msl->type].type == 4 && MOX::_combat_data[msl->source_unit_idx].owner == player_idx) {
            if (msl->type == 29 /* WEAP_FIGHTER */) {
                missile_counts[4] += msl->quantity;
                total_quantity += msl->quantity;
            } else if (msl->type == 30 /* WEAP_BOMBER */) {
                missile_counts[5] += msl->quantity;
                total_quantity += msl->quantity;
            } else if (msl->type == 31 /* WEAP_HEAVY_BOMBER */) {
                missile_counts[6] += msl->quantity;
                total_quantity += msl->quantity;
            }
        }
        RUSS::Mox_Update_();
    }

    if (total_quantity <= 0) {
        *beam_strength = CMBTAI::Get_Effective_Beam_Strength_(target_player);
    } else {
        int16_t current_ordinal = 0;
        while (missile_counts[current_ordinal] == 0 && current_ordinal < 7) {
            current_ordinal++;
        }
        int16_t msl_type = Convert_Ordinal_To_Msl_Type_(current_ordinal);

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            if (current_ordinal > 6) {
                break;
            }

            if (MOX::_combat_data[i].unit_status == 0 && MOX::_combat_data[i].is_captured == 0 && MOX::_combat_data[i].owner != player_idx) {
                for (int16_t j = 0; j < 8; ++j) {
                    if (current_ordinal > 6) {
                        break;
                    }

                    int16_t weapon_id = MOX::_combat_data[i].weapons[j].weapon_id;
                    int16_t remaining_pd_count = 0;

                    if (CMBTFIRE::Weapon_Can_Fire_At_Missiles_(weapon_id, MOX::_combat_data[i].weapons[j].specials)) {
                        if (TECHDATA::_weapons[weapon_id].type == 0) {
                            remaining_pd_count = MOX::_combat_data[i].weapons[j].count;
                            while (current_ordinal <= 6 && remaining_pd_count > 0) {
                                int16_t kills = Average_Missile_Kills_(i, j, missile_counts[current_ordinal], msl_type, &remaining_pd_count, -1);
                                missile_counts[current_ordinal] -= (int16_t)kills;

                                while (missile_counts[current_ordinal] == 0 && current_ordinal < 7) {
                                    current_ordinal++;
                                }
                                msl_type = Convert_Ordinal_To_Msl_Type_(current_ordinal);

                                if (missile_counts[current_ordinal] == 0 || current_ordinal > 6 || remaining_pd_count <= 0) {
                                    break;
                                }
                            }
                        }
                    }

                    if (TECHDATA::_weapons[weapon_id].type == 0) {
                        *beam_strength += TECHDATA::_weapons[weapon_id].max_damage * remaining_pd_count;
                    }
                }
            }
            RUSS::Mox_Update_();
        }
    }

    int16_t total_effective_strength = 0;
    for (int16_t i = 0; i <= 9; ++i) {
        int16_t msl_id = Convert_Ordinal_To_Msl_Type_(i);
        int16_t dmg_per;
        int16_t max_damage = TECHDATA::_weapons[msl_id].max_damage;
        if (shield_points == 100) {
            dmg_per = (max_damage + (max_damage < 0 ? 3 : 0)) / 4;
        } else {
            dmg_per = 0;
            if (max_damage > shield_points) {
                dmg_per = max_damage - shield_points;
            }
        }
        total_effective_strength += dmg_per * missile_counts[i];
    }

    return total_effective_strength;
}

int16_t __cdecl Average_Missile_Kills_(int16_t ship_idx, int16_t weapon_slot_idx, int16_t msl_count, int16_t msl_type, int16_t* remaining_pd_count, int16_t missile_idx) {
    int16_t kills = 0;
    int16_t slot_weapon_id = MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].weapon_id;
    uint16_t specials = MOX::_combat_data[ship_idx].weapons[weapon_slot_idx].specials;
    int16_t range;

    if (missile_idx > -1) {
        range = Range_To_Missile_(ship_idx, missile_idx);
    } else {
        range = 2;
    }

    int16_t target_player;
    if (MOX::_combat_data[ship_idx].owner == MOX::_combat_attacker_player) {
        target_player = MOX::_combat_defender_player;
    } else {
        target_player = MOX::_combat_attacker_player;
    }

    int16_t current_health = *remaining_pd_count;

    if (TECHDATA::_weapons[msl_type].type != 2) {
        int16_t hit_chance = (CMBTMIS::Missile_Dcv_(msl_type, target_player) - 
                              MOX::_combat_data[ship_idx].tac_val_attack - 
                              MOX::_ranged_to_hit_penalty[range]) + 40;

        if (hit_chance < 95) {
            hit_chance = (CMBTMIS::Missile_Dcv_(msl_type, target_player) - 
                          MOX::_combat_data[ship_idx].tac_val_attack - 
                          MOX::_ranged_to_hit_penalty[range]) + 40;
        } else {
            hit_chance = 95;
        }

        int16_t base_dmg = (int16_t)((TECHDATA::_weapons[slot_weapon_id].min_damage + TECHDATA::_weapons[slot_weapon_id].max_damage) * 5);

        if (!((specials & 0x20) || (TECHDATA::_weapons[slot_weapon_id].natural_mods & 0x4) || COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0xF))) {
            int16_t penalty = MOX::_ranged_damage_penalty[range];
            base_dmg += (abs(penalty) * base_dmg) / -100;
        }

        int16_t avg_dmg_per_missile = ((100 - hit_chance) * base_dmg) / 100;
        int16_t total_hits = (int16_t)(CMBTMIS::Get_Missile_Hits_(msl_type, 0, 0) * 10);
        int16_t divisor;

        if (total_hits < avg_dmg_per_missile) {
            divisor = 1;
        } else if (avg_dmg_per_missile == 0) {
            divisor = 1000;
        } else {
            divisor = (total_hits + avg_dmg_per_missile - 1) / avg_dmg_per_missile;
        }

        kills = current_health / divisor;

        if (msl_count < kills) {
            *remaining_pd_count = current_health - (int16_t)(msl_count * divisor);
            return msl_count;
        } else {
            *remaining_pd_count = 0;
        }
    }

    return kills;
}

int8_t __cdecl Ai_Self_Destruct_Check_(int16_t ship_idx) {
    int8_t ret_val = 0;
    int16_t dist_friendly = 0; // BUG: Original code failed to initialize this variable due to a compiler offset bug
    int16_t dist_enemy = 0;
    int16_t friendly_rack4 = 0;
    int16_t enemy_rack4 = 0;
    int16_t friendly_other = 0;
    int16_t score = 0;
    int16_t strength_attacker = 0;
    int16_t strength_defender = 0;
    int32_t marine_count_sum = 0;

    if (ship_idx == 0) return ret_val;

    s_combat_data* ship = &MOX::_combat_data[ship_idx];

    if (ship->owner > 8 || ship->ftl_type <= 0) return ret_val;

    int16_t player_id = MOX::_combat_attacker_player;
    if (ship->owner == MOX::_combat_attacker_player) {
        player_id = MOX::_combat_defender_player;
    }

    if (player_id < MOX::_NUM_PLAYERS) {
        int16_t attacker_bonus = CMBTFIR2::Get_Fleet_Commando_Bonus_(player_id);
        int16_t defender_bonus = CMBTFIR2::Crew_Boarding_Combat_Bonus_(ship_idx);
        defender_bonus += COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_SECURITY_STATIONS) * 20;
        defender_bonus += CMBTFIR2::Get_Fleet_Commando_Bonus_(ship->owner);
        defender_bonus += CMBTFIR2::Get_Fleet_Security_Bonus_(ship->owner);

        s_invasion_side_info attacker_info;
        s_invasion_side_info defender_info;

        invasion::Get_Boarding_Info_(
            player_id, 5, attacker_bonus, 
            ship->owner, 5, defender_bonus, 
            &attacker_info, &defender_info
        );

        strength_attacker = attacker_info.unit_strengths[1];
        strength_defender = defender_info.unit_strengths[1];
    }

    if (MOX::_combat_total_ship_count > 1) {
        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
            if (MOX::_combat_data[i].unit_status == 0 && i != ship_idx) {
                if (CMBTMOV1::Ship_Is_In_Stasis_(i) == 0) {
                    int16_t range = Range_To_Ship_(ship_idx, i);
                    strength_attacker += CMBTFIR2::Crew_Boarding_Combat_Bonus_(i);

                    if (MOX::_combat_data[i].owner == ship->owner) {
                        if (range <= 1) {
                            dist_friendly += 2;
                        } else if (range == 2) {
                            dist_friendly += 1;
                        }
                    } else {
                        int8_t has_security = (int8_t)COMBINIT::Does_Combat_Ship_Have_Special_(i, SPECIAL_TRANSPORTERS);
                        bool add_marines = false;
                        
                        if (range <= 1) {
                            dist_enemy += 2;
                            if (has_security != 0 || ship->current_speed == 0) {
                                add_marines = true;
                            }
                        } else if (range == 2) {
                            dist_enemy += 1;
                            if (has_security != 0 || ship->current_speed == 0) {
                                add_marines = true;
                            }
                        } else if (range < 7) {
                            if (has_security != 0) {
                                add_marines = true;
                            }
                        }
                        
                        if (add_marines) {
                            marine_count_sum += (MOX::_combat_data[i].marine_count >> 1);
                        }
                    }
                }
            }
        }
    }

    for (int16_t i = 0; i < 300; ++i) {
        int16_t type = MOX::_missile[i].type;
        if (type > 0) {
            if (TECHDATA::_weapons[type].type != 2) {
                int16_t range = Range_To_Missile_(ship_idx, i);
                if (range < 3) {
                    int16_t owner = CMBTMIS::Missile_Owner_(i);
                    if (owner == ship->owner) {
                        if (TECHDATA::_weapons[type].type == 4) {
                            friendly_rack4 += MOX::_missile[i].quantity;
                        } else {
                            friendly_other += MOX::_missile[i].quantity;
                        }
                    } else {
                        if (TECHDATA::_weapons[type].type == 4) {
                            enemy_rack4 += MOX::_missile[i].quantity;
                        } else {
                            score += MOX::_missile[i].quantity;
                        }
                    }
                }
            }
        }
    }

    score += ((((dist_enemy * 2) - dist_friendly) * 2 - friendly_rack4 + enemy_rack4) * 5) - friendly_other;

    if (marine_count_sum > 0 && player_id < MOX::_NUM_PLAYERS) {
        int16_t strength_diff = strength_attacker - strength_defender;
        uint16_t ship_marines = ship->marine_count;

        if (strength_diff >= 51) {
            marine_count_sum *= 10;
        } else if (strength_diff >= 40) {
            marine_count_sum *= 7;
        } else if (strength_diff >= 30) {
            marine_count_sum *= 4;
        } else if (strength_diff >= 20) {
            marine_count_sum *= 3;
        } else if (strength_diff >= 10) {
            marine_count_sum *= 2;
        } else if (strength_diff <= -50) {
            ship_marines *= 10;
        } else if (strength_diff <= -40) {
            ship_marines *= 7;
        } else if (strength_diff <= -30) {
            ship_marines *= 4;
        } else if (strength_diff <= -20) {
            ship_marines *= 3;
        } else if (strength_diff <= -10) {
            ship_marines *= 2;
        }

        if (ship_marines < marine_count_sum) {
            int16_t diff = (int16_t)marine_count_sum - ship_marines;
            if (MOX::_player[player_id].traits[TRAIT_TELEPATHIC] != 0) {
                score += diff * 10;
            } else {
                score += diff * 5;
            }
        }
    }

    if (score > 0) {
        if ((int)game_random::Random_(100) < score) {
            if (ship->current_speed == 0) {
                ret_val = 1;
            }
        }
    }

    return ret_val;
}

} // namespace CMBTAI