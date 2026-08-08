#include "pch.h"

namespace CMBTMIS {
    int16_t _missile_group_offset[16][16] = {
        { 0, -12, 8, -6, 15, 0, 8, 6, 0, 12, -8, 6, -15, 0, -8, -6 },
        { -9, -10, 1, -8, 13, -8, 11, 1, 9, 10, -1, 8, -13, 8, -11, -1 },
        { 13, -8, 11, 1, 9, 10, -1, 8, -13, 8, -11, -1, -9, -10, 1, -8 },
        { -11, -7, -3, -8, 6, -11, 10, -4, 12, 6, 3, 8, -6, 11, -10, 4 },
        { 0, -12, 8, -6, 15, 0, 8, 6, 0, 12, -8, 6, -15, 0, -8, -6 },
        { -12, 7, -9, -3, -6, -11, 3, -8, 12, -7, 9, 3, 6, 11, -3, 8 },
        { 13, -8, 11, 1, 9, 10, -1, 8, -13, 8, -11, -1, -9, -10, 1, -8 },
        { -9, -10, 1, -8, 13, -8, 11, 1, 9, 10, -1, 8, -13, 8, -11, -1 },
        { 0, -12, 8, -6, 15, 0, 8, 6, 0, 12, -8, 6, -15, 0, -8, -6 },
        { -9, -10, 1, -8, 13, -8, 11, 1, 9, 10, -1, 8, -13, 8, -11, -1 },
        { 13, -8, 11, 1, 9, 10, -1, 8, -13, 8, -11, -1, -9, -10, 1, -8 },
        { 11, 7, 3, 8, -6, 11, -10, 4, -12, -6, -3, -8, 6, -11, 10, -4 },
        { 0, -12, 8, -6, 15, 0, 8, 6, 0, 12, -8, 6, -15, 0, -8, -6 },
        { -12, 7, -9, -3, -6, -11, 3, -8, 12, -7, 9, 3, 6, 11, -3, 8 },
        { 13, -8, 11, 1, 9, 10, -1, 8, -13, 8, -11, -1, -9, -10, 1, -8 },
        { -9, -10, 1, -8, 13, -8, 11, 1, 9, 10, -1, 8, -13, 8, -11, -1 },
    };
    int16_t _torpedo_group_offset[16][8] = {
        { 12, -12, 12, 12, -12, 12, -12, -12 },
        { 4, -16, 15, 8, -4, 16, -15, -8 },
        { -1, 18, 18, 1, 1, 18, -18, -1 },
        { -9, -14, 17, -5, 9, 14, -17, 5 },
        { -12, -12, 12, -12, 12, 12, -12, 12 },
        { -17, -5, 9, -14, 17, 5, -9, 14 },
        { -18, 1, 1, -18, 18, 1, -1, 18 },
        { -15, 8, -4, -16, 15, -8, 4, 16 },
        { -12, 12, -12, -12, 12, -12, 12, 12 },
        { -4, 16, -15, -8, 4, -16, 15, 8 },
        { 1, -18, -18, -1, -1, -18, 18, 1 },
        { 9, 14, -17, 5, -9, -14, 17, -5 },
        { 12, 12, -12, 12, -12, -12, 12, -12 },
        { 17, 5, -9, 14, -17, -5, 9, -14 },
        { 18, -1, -1, 18, -18, -1, 1, -18 },
        { 15, -8, 4, 16, -15, 8, -4, -16 },
    };
    int8_t __cdecl Did_Missile_Hit_(int16_t missile_idx, int16_t x, int16_t y) {
        int32_t dx = (int32_t)x - (int32_t)MOX::_missile[missile_idx].x;
        int32_t dy = (int32_t)y - (int32_t)MOX::_missile[missile_idx].y;
        int32_t dist_sq = dx * dx + dy * dy;

        if (MOX::_missile[missile_idx].is_anti_missile_rocket == 1) {
            return dist_sq < 64 /* 0x40 */;
        }

        int16_t target_idx = MOX::_missile[missile_idx].target_unit_idx;
        int32_t radius = (int32_t)MOX::_missile_hit_radius[MOX::_combat_data[target_idx].size_class];
        int32_t radius_sq = radius * radius;

        return dist_sq < radius_sq;
    }

    int16_t __cdecl Missile_Owner_(int16_t missile_idx) {
        return MOX::_missile[missile_idx].owner;
    }

    int8_t __cdecl On_Combat_Screen_(int16_t x, int16_t y) {
        if (MOX::_cur_combat_x <= x && x < MOX::_cur_combat_x + 32 /* 0x20 */) {
            if (MOX::_cur_combat_y <= y && y < MOX::_cur_combat_y + 18 /* 0x12 */) {
                return 1;
            }
        }
        return 0;
    }

    int8_t __cdecl Missile_On_Combat_Screen_(int16_t missile_idx) {
        s_missile* missile = &MOX::_missile[missile_idx];

        return CMBTMIS::On_Combat_Screen_(missile->x / 20, missile->y / 20);
    }

    void __cdecl Retarget_Missile_(int16_t missile_idx) {
        int16_t best_target = -1;
        s_missile& missile = MOX::_missile[missile_idx];
        int16_t owner_player = MOX::_combat_data[missile.source_unit_idx].owner;

        if (TECHDATA::_weapons[missile.type].type == WEAPON_TYPE_FIGHTER || 
            (owner_player < MAX_PLAYERS && MOX::_player[owner_player].tech_applications[TECH_APP_DAUNTLESS_GUIDANCE_SYSTEM] == TECH_RESEARCH_STATUS_RESEARCHED)) {
            
            int16_t min_range = 30000;
            int16_t target_player = MOX::_combat_attacker_player;
            if (owner_player == MOX::_combat_attacker_player) {
                target_player = MOX::_combat_defender_player;
            }

            for (int16_t ship_idx = 0; ship_idx < MOX::_combat_total_ship_count; ++ship_idx) {
                if (CMBTFIRE::Target_Is_Legal_For_Weapon_(missile.type, 0, ship_idx, 0)) {
                    // Index 0 is the planet/base defenses
                    if (ship_idx != 0 || CMBTFIRE::Does_Combat_Planet_Have_Defenses_()) {
                        s_combat_data& ship = MOX::_combat_data[ship_idx];

                        if (ship.owner == target_player &&
                            ship.is_captured == 0 &&
                            ship.special_status_flag != 4 /* Stasis */ &&
                            !CMBTMOV1::Ship_Is_In_Stasis_(ship_idx) &&
                            ship.unit_status == 0 &&
                            !Missile_Targets_Ship_(&missile, ship_idx)) {

                            int16_t range = CMBTAI::Range_To_Missile_(ship_idx, missile_idx);
                            if (range < min_range) {
                                min_range = range;
                                best_target = ship_idx;
                            }
                        }
                    }
                }
            }
        }

        if (best_target == -1) {
            if (TECHDATA::_weapons[missile.type].type == WEAPON_TYPE_FIGHTER && 
                MOX::_combat_data[missile.source_unit_idx].unit_status == 0) {
                missile.target_unit_idx = missile.source_unit_idx;
                missile.is_anti_missile_rocket = 0;
            } else {
                Kill_Missile_(missile_idx);
            }
        } else {
            missile.target_unit_idx = best_target;
            missile.is_anti_missile_rocket = 0;
        }
    }

    char __cdecl Move_Missile_(int16_t missile_idx, int16_t target_x, int16_t target_y, int16_t move_dist) {
        s_missile *missile = &MOX::_missile[missile_idx];
        int16_t type = missile->type;
        char hit_flag = 0;
        int16_t dx, dy;

        if (missile->x == target_x && missile->y == target_y) {
            return 1;
        }

        if (type > WEAPON_NO_WEAPONS) {
            int16_t dist = move_dist;
            if (missile->travel_dist < dist) {
                dist = missile->travel_dist;
            }

            Coord_Movement_(missile->x, missile->y, target_x, target_y, (uint8_t)dist, &dx, &dy);

            missile->x += dx;
            missile->y += dy;

            for (int16_t i = 0; i < 300; ++i) {
                s_missile *other = &MOX::_missile[i];
                if (other->type > WEAPON_NO_WEAPONS && other->is_anti_missile_rocket == 1 && other->target_unit_idx == missile_idx) {
                    if (Move_Missile_(i, missile->x, missile->y, dist)) {
                        Resolve_Missile_(i, missile->x, missile->y);
                    }
                }
            }

            if (dx > 0) {
                if (target_x - 3 < missile->x) {
                    missile->x = target_x - 3;
                }
            } else if (dx < 0) {
                if (missile->x < target_x + 3) {
                    missile->x = target_x + 3;
                }
            }

            if (dy > 0) {
                if (target_y - 3 < missile->y) {
                    missile->y = target_y - 3;
                }
            } else if (dy < 0) {
                if (missile->y < target_y + 3) {
                    missile->y = target_y + 3;
                }
            }

            hit_flag = Did_Missile_Hit_(missile_idx, target_x, target_y);
            missile->travel_dist -= dist;
            missile->facing_dir = (int8_t)Get_Facing_(missile_idx, target_x, target_y);
        }

        missile = &MOX::_missile[missile_idx];
        type = missile->type;
        if ((type == WEAPON_PLASMA_TORPEDO || type == WEAPON_DRAGON_BREATH) &&
            hit_flag == 0 &&
            (missile->specials & WEAPON_MOD_FLAG_NO_RANGE_DISSIPATION) == 0) {
            int16_t owner = Missile_Owner_(missile_idx);
            int16_t speed = Missile_Speed_(type, missile->specials, owner);
            int16_t adj_val = speed * (missile->fighter_count + 1) * 20 - missile->travel_dist;

            owner = Missile_Owner_(missile_idx);
            int16_t bonus = MOX::_ordnance_bonus[owner];
            if (missile->specials & WEAPON_MOD_FLAG_OVERLOADED) {
                bonus += 50;
            }

            int16_t base_damage = (int16_t)((TECHDATA::_weapons[type].max_damage * bonus) / 100);
            int16_t damage_adjustment;
            if (type == WEAPON_PLASMA_TORPEDO) {
                damage_adjustment = (adj_val / -20) * 5;
            } else {
                damage_adjustment = (adj_val / -20) * 15;
            }

            if (base_damage + damage_adjustment < 1) {
                Kill_Missile_(missile_idx);
            }
        }

        return hit_flag;
    }

    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t step, int16_t* out_x, int16_t* out_y) {
        int16_t dx = x2 - x1;
        int16_t dy = y2 - y1;
        int32_t abs_dx = (dx < 0) ? -dx : dx;
        int32_t abs_dy = (dy < 0) ? -dy : dy;

        if (abs_dx < abs_dy) {
            int16_t sy = (dy < 0) ? -1 : 1;
            int16_t v = (int16_t)(sy * step);

            *out_y = v;
            *out_x = (int16_t)((int32_t)v * dx / dy);
        } else {
            int16_t sx = (dx < 0) ? -1 : 1;
            int16_t v = (int16_t)(sx * step);

            *out_x = v;
            *out_y = (int16_t)((int32_t)v * dy / dx);
        }
    }

    int16_t __cdecl Get_Facing_(int16_t missile_idx, int16_t target_x, int16_t target_y) {
        return CMBTMOV1::Get_Facing_(
            MOX::_missile[missile_idx].x,
            MOX::_missile[missile_idx].y,
            target_x,
            target_y
        );
    }

    void __cdecl Land_Returning_Fighters_(
        s_combat_data* source, int16_t type, int16_t returned_count) {
        for (int16_t i = 0; i < 8 && returned_count > 0; ++i) {
            s_combat_weapon_slot* slot = &source->weapons[i];
            if (slot->weapon_id != type) {
                continue;
            }

            int16_t capacity = COMBINIT::Surviving_Weapon_Count_From_Hits_(
                source->weapon_hits[i],
                source->weapon_hits_per_unit[i]);

            if (slot->ammo == 0) {
                slot->count = 0;
            }

            int16_t missing_count = capacity - slot->count;
            if (missing_count <= 0) {
                continue;
            }

            int16_t restored_count =
                returned_count < missing_count ? returned_count : missing_count;
            slot->count += (int8_t)restored_count;
            returned_count -= restored_count;
            slot->ammo = 1;
            slot->shots_left = 0;
        }
    }

    int16_t __cdecl Resolve_Missile_(int16_t missile_idx, int16_t x, int16_t y) {
        int16_t owner = Missile_Owner_(missile_idx);
        MOX::_missile_warning = 0;

        s_missile* missile = &MOX::_missile[missile_idx];
        if (missile->is_active != 1) {
            MOX::_missile_warning = 0;
            return 0;
        }

        int16_t source_unit_idx = missile->source_unit_idx;
        int16_t target_unit_idx = missile->target_unit_idx;
        int16_t type = missile->type;
        uint16_t specials = missile->specials;
        int16_t is_anti_missile_rocket = missile->is_anti_missile_rocket;

        if (target_unit_idx == source_unit_idx && is_anti_missile_rocket == 0) {
            int16_t reload_count = missile->quantity / 4;
            Land_Returning_Fighters_(
                &MOX::_combat_data[source_unit_idx], type, reload_count);
            Kill_Missile_(missile_idx);
            return 0;
        }

        if (!Is_Missile_Target_Alive_(missile_idx)) {
            return 0;
        }

        int16_t shield_idx = 0;
        int16_t evade = 0;
        uint16_t weapon_flags = 0;
        uint16_t applied_hits = 0;
        int16_t shield_hit_dir = 0;

        if (is_anti_missile_rocket == 0) {
            if (COMBINIT::Does_Combat_Ship_Have_Special_(target_unit_idx, SPECIAL_LIGHTNING_FIELD)) {
                if (_lightning_field_mode == LIGHTNING_FIELD_MODE_UNIFORM) {
                    int16_t destroyed = 0;
                    int16_t roll_count = missile->quantity;
                    for (int16_t i = 0; i < roll_count; ++i) {
                        if (game_random::Random_(100) <= _lightning_field_ratio) {
                            destroyed++;
                        }
                    }
                    missile->quantity -= destroyed;
                } else {
                    for (int16_t i = 0; i < missile->quantity; ++i) {
                        if (game_random::Random_(100) <= _lightning_field_ratio) {
                            missile->quantity--;
                        }
                    }
                }

                if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                    CMBTSPEC::Lightning_Field_(target_unit_idx, missile_idx, x, y);
                }
            }

            CMBTFIR2::Final_Fire_(target_unit_idx, x, y, missile_idx);

            int16_t bearing = CMBTDRW1::Relative_Bearing_XY_(target_unit_idx, missile->x, missile->y);
            int32_t idx = (bearing * 2 + 22) / 45 + MOX::_combat_data[target_unit_idx].facing_dir;
            int32_t sign = idx >> 31;
            shield_hit_dir = ((((idx ^ sign) - sign) & 0xF) ^ sign) - sign;

            if (TECHDATA::_weapons[type].type == WEAPON_TYPE_FIGHTER) {
                shield_idx = Find_Weakest_Shield_(target_unit_idx);
                evade = 0;
            } else {
                int16_t rel_bearing = CMBTDRW1::Relative_Bearing_XY_(target_unit_idx, missile->x, missile->y);
                shield_idx = CMBTFIRE::Facing_Shield_(rel_bearing);
                evade = MOX::_combat_data[target_unit_idx].tac_val_evade;
            }

            if (owner < MAX_PLAYERS) {
                if (MOX::_player[owner].tech_applications[TECH_APP_SENSORS] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    evade -= 70;
                } else if (MOX::_player[owner].tech_applications[TECH_APP_NEUTRON_SCANNER] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    evade -= 40;
                } else if (MOX::_player[owner].tech_applications[TECH_APP_TACHYON_SCANNER] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    evade -= 20;
                }
            } else {
                evade -= 40;
            }

            if (evade < 0) {
                evade = 0;
            } else if (specials & WEAPON_MOD_FLAG_ECCM) {
                evade /= 2;
            }

            if (type == WEAPON_DRAGON_BREATH) {
                evade = 0;
            }
        }

        if (missile->quantity <= 0) {
            Kill_Missile_(missile_idx);
            return 0;
        }

        if (specials & WEAPON_MOD_FLAG_MIRV) {
            missile->quantity *= 4;
        }

        int16_t dmg_accum = 0;
        int16_t sys_dmg_accum = 0;

        if (TECHDATA::_weapons[type].type == WEAPON_TYPE_FIGHTER) {
            missile->is_active = 0;
            if (type == WEAPON_ASSAULT_SHUTTLE) {
                CMBTFIR2::Resolve_Capture_(missile->source_unit_idx, target_unit_idx, missile->quantity, x, y);
                missile->fighter_count--;
            } else {
                if (type != WEAPON_BOMBER) {
                    int16_t dmg_out = 0;
                    int16_t sys_dmg = 0;
                    CMBTFIRE::Fire_Fighter_Beam_(missile_idx, shield_idx, is_anti_missile_rocket, &dmg_out, &sys_dmg);
                    dmg_accum = dmg_out;
                    sys_dmg_accum = sys_dmg;
                    if (type != WEAPON_HEAVY_FIGHTER) {
                        missile->fighter_count--;
                    }
                }

                if (missile->fighter_count != 0 && type != WEAPON_INTERCEPTOR) {
                    int16_t dmg_out = 0;
                    int16_t sys_dmg = 0;
                    CMBTFIRE::Fire_Fighter_Bomb_(missile_idx, shield_idx, &dmg_out, &sys_dmg);
                    missile->fighter_count--;
                    dmg_accum += dmg_out;
                    sys_dmg_accum += sys_dmg;
                }

                if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                    Fighter_Combat_SFX_(missile_idx, x, y, dmg_accum, sys_dmg_accum);
                }

                dmg_accum = 0;
                sys_dmg_accum = 0;
            }
        } else {
            int16_t bonus = MOX::_ordnance_bonus[Missile_Owner_(missile_idx)];
            if (specials & WEAPON_MOD_FLAG_OVERLOADED) {
                bonus += 50;
            }
            int16_t damage = (TECHDATA::_weapons[type].max_damage * bonus) / 100;

            if ((type == WEAPON_PLASMA_TORPEDO || type == WEAPON_DRAGON_BREATH) &&
                !(specials & WEAPON_MOD_FLAG_NO_RANGE_DISSIPATION)) {
                int16_t degrad = 0;
                if (type == WEAPON_PLASMA_TORPEDO) {
                    int16_t speed = Missile_Speed_(type, specials, Missile_Owner_(missile_idx));
                    degrad = speed * (missile->fighter_count + 1) * 20 - missile->travel_dist - 20;
                    if (degrad < 0) degrad = 0;
                    degrad = (degrad / -20) * 5;
                } else {
                    degrad = 9980 - missile->travel_dist; // 0x26fc
                    if (degrad < 0) degrad = 0;
                    degrad = (degrad / -20) * 15;
                }
                damage += degrad;
                if (damage <= 0) {
                    missile->quantity = 0;
                }
            }

            if (target_unit_idx == 0 && TECHDATA::_weapons[type].type == WEAPON_TYPE_TORPEDO) {
                damage /= 2;
            }

            int16_t loop_count = 0;
            if (missile->quantity > 0) {
                do {
                    int16_t roll = game_random::Random_(100);

                    if (COMBINIT::Does_Combat_Ship_Have_Special_(target_unit_idx, SPECIAL_DISPLACEMENT_DEVICE)) {
                        if (Missile_Defense_Roll_Succeeds_(
                                game_random::Random_(100),
                                missile_defense.displacement_device_missiles_miss_percent)) {
                            roll = -1000;
                        }
                    }

                    if (MOX::_combat_data[target_unit_idx].special_status_flag == 1 &&
                        missile_defense.cloaking_device_missile_defense == 0) {
                        if (Missile_Defense_Roll_Succeeds_(
                                game_random::Random_(100),
                                missile_defense.cloaking_device_missile_miss_percent)) {
                            roll = -1000;
                        }
                    }

                    if (evade < roll) {
                        if (!(specials & WEAPON_MOD_FLAG_ENVELOPING)) {
                            int16_t dmg_out = damage;
                            int16_t sys_dmg = 0;
                            int16_t applied = CMBTFIRE::Apply_Ship_Damage_(target_unit_idx, &dmg_out, &sys_dmg, shield_idx, specials, weapon_flags, 0, 0, specials & WEAPON_MOD_FLAG_EMISSION_GUIDANCE);
                            dmg_accum += dmg_out;
                            sys_dmg_accum += sys_dmg;
                            applied_hits |= applied;
                        } else {
                            for (int16_t i = 0; i < 4; ++i) {
                                int16_t dmg_out = damage;
                                int16_t sys_dmg = 0;
                                CMBTFIRE::Apply_Ship_Damage_(target_unit_idx, &dmg_out, &sys_dmg, i, specials, weapon_flags, 0, 0, specials & WEAPON_MOD_FLAG_EMISSION_GUIDANCE);
                                dmg_accum += dmg_out;
                                sys_dmg_accum += sys_dmg;
                            }
                        }
                    }
                    loop_count++;
                } while (loop_count < missile->quantity);
            }
            Kill_Missile_(missile_idx);
        }

        if (sys_dmg_accum + dmg_accum > 0) {
            if (is_anti_missile_rocket == 0) {
                if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                    if (!CMBTDRW1::Ship_Completely_On_Screen_(target_unit_idx)) {
                        COMBINIT::Snap_Center_Combat_Screen_(x / 20, y / 20);
                    }
                    int16_t offset_x, offset_y;
                    COMBINIT::Ship_Center_Offsets_(target_unit_idx, &offset_x, &offset_y);
                    
                    BEAMS::Add_Damage_Indicator_For_Ship_(
                        (MOX::_cur_combat_x * -20) - offset_x + x,
                        (MOX::_cur_combat_y * -20) - offset_y + y,
                        target_unit_idx, 1, dmg_accum, sys_dmg_accum
                    );

                    if (sys_dmg_accum <= 0) {
                        while (!BEAMS::Done_Drawing_Damage_Message_Queue_()) {
                            timer::Mark_Time_();
                            video::Set_Page_Off_();
                            CMBTDRW1::Draw_Main_Combat_Screen_();
                            graphics::Set_Window_(0, 0, 639, 350);
                            video::Clip_On_();

                            if (target_unit_idx == MOX::_dont_draw_ship) {
                                int16_t d_x, d_y;
                                COMBINIT::Ship_Center_Offsets_(target_unit_idx, &d_x, &d_y);
                                CMBTDRW1::Draw_Ship_(target_unit_idx,
                                    (MOX::_cur_combat_x * -20) - d_x + x,
                                    (MOX::_cur_combat_y * -20) - d_y + y
                                );
                            }

                            BEAMS::Draw_Damage_Message_Queue_();
                            RUSS::Mox_Sync_Update_();
                            timer::Release_Time_(1);
                            video::Clip_Off_();
                            graphics::Reset_Window_();
                        }
                    } else {
                        BEAMS::Draw_Shield_Hit_Once_Through_(target_unit_idx, shield_hit_dir, 
                            MOX::_cur_combat_x * -20 + x, MOX::_cur_combat_y * -20 + y);
                    }
                }
            }
        }

        if (is_anti_missile_rocket == 0) {
            int16_t death_type = CMBTFIRE::Is_Combat_Ship_Dead_(target_unit_idx);
            if (death_type != 0) {
                int16_t off_x, off_y;
                COMBINIT::Ship_Center_Offsets_(target_unit_idx, &off_x, &off_y);
                CMBTFIRE::Destroy_Ship_(target_unit_idx, x - off_x, y - off_y, death_type);
            }
        }

        if (TECHDATA::_weapons[type].type == WEAPON_TYPE_FIGHTER) {
            if (missile->fighter_count == 0) {
                int16_t source = missile->source_unit_idx;
                if (MOX::_combat_data[source].owner == Missile_Owner_(missile_idx) &&
                    COMBAT1::Combat_Ship_Is_Functional_(source) &&
                    type != WEAPON_ASSAULT_SHUTTLE) {
                    
                    missile->target_unit_idx = source;
                    missile->is_anti_missile_rocket = 0;
                    missile->is_active = 1;
                    return 0;
                }
                Kill_Missile_(missile_idx);
                return 0;
            }

            if (missile->target_unit_idx == 0 && missile->is_anti_missile_rocket == 0 &&
                !CMBTFIRE::Does_Combat_Planet_Have_Defenses_()) {
                CMBTMIS::Retarget_Missile_(missile_idx);
                return 0;
            }
        }

        return 0;
    }

    int16_t __cdecl Missile_Speed_(int16_t type, uint16_t flags, int16_t player_idx) {
        bool is_warp_affected = true;
        int16_t missile_speed = 0;

        switch (type) {
            case WEAPON_NUCLEAR_MISSILE:
            case WEAPON_MERCULITE_MISSILE:
            case WEAPON_PULSON_MISSILE:
            case WEAPON_ZEON_MISSILE:
                missile_speed = 12;
                break;

            case WEAPON_ANTI_MATTER_TORP:
            case WEAPON_PROTON_TORPEDO:
                is_warp_affected = false;
                missile_speed = 20;
                break;

            case WEAPON_PLASMA_TORPEDO:
            case WEAPON_DRAGON_BREATH:
                is_warp_affected = false;
                missile_speed = 24;
                break;

            case WEAPON_ASSAULT_SHUTTLE:
                missile_speed = 6;
                if (player_idx < MAX_PLAYERS && MOX::_player[player_idx].traits[TRAIT_TRANS_DIMENSIONAL]) {
                    missile_speed = 10;
                }
                break;

            case WEAPON_HEAVY_FIGHTER:
                missile_speed = 8;
                if (player_idx < MAX_PLAYERS && MOX::_player[player_idx].traits[TRAIT_TRANS_DIMENSIONAL]) {
                    missile_speed = 12;
                }
                break;

            case WEAPON_BOMBER:
                missile_speed = 8;
                if (player_idx < MAX_PLAYERS && MOX::_player[player_idx].traits[TRAIT_TRANS_DIMENSIONAL]) {
                    missile_speed = 12;
                }
                break;

            case WEAPON_INTERCEPTOR:
                missile_speed = 10;
                if (player_idx < MAX_PLAYERS && MOX::_player[player_idx].traits[TRAIT_TRANS_DIMENSIONAL]) {
                    missile_speed = 14;
                }
                break;

            default:
                break;
        }

        if (is_warp_affected) {
            int16_t drive_level = INITSHIP::Best_Warp_Drive_(player_idx);
            missile_speed += (drive_level * 2) - 2;
        }

        if (flags & WEAPON_MOD_FLAG_FAST) {
            missile_speed += 4;
        }

        return missile_speed;
    }

    int16_t __cdecl Missile_Facing_(int16_t missile_idx, int16_t target_idx, int16_t target_is_missile) {
        int16_t target_x;
        int16_t target_y;
        int16_t off_x;
        int16_t off_y;

        if (target_is_missile == 0) {
            COMBINIT::Ship_Center_Offsets_(target_idx, &off_x, &off_y);

            target_x = (int16_t)MOX::_combat_data[target_idx].x * 20 + off_x;
            target_y = (int16_t)MOX::_combat_data[target_idx].y * 20 + off_y;
        } else {
            target_x = MOX::_missile[target_idx].x;
            target_y = MOX::_missile[target_idx].y;
        }

        return CMBTMOV1::Get_Facing_(
            MOX::_missile[missile_idx].x,
            MOX::_missile[missile_idx].y,
            target_x,
            target_y
        );
    }

    int16_t __cdecl Is_Missile_Target_Alive_(int16_t missile_idx) {
        int16_t result = 0;
        s_missile* missile = &MOX::_missile[missile_idx];

        if (missile->is_anti_missile_rocket != 0) {
            // Anti-missile rocket logic: target is another missile
            if (MOX::_missile[missile->target_unit_idx].quantity > 0) {
                return 1;
            }
        } else {
            // Standard missile logic: target is a ship
            if (missile->target_unit_idx == 0) {
                return 1;
            }

            if (CMBTFIRE::Is_Combat_Ship_Dead_(missile->target_unit_idx) == 0) {
                result = 1;
            }
        }

        return result;
    }

    int16_t __cdecl Find_Weakest_Shield_(int16_t ship_idx) {
        int16_t weakest_idx = 0;
        int16_t min_shield = 200;

        for (int16_t i = 0; i < 4; ++i) {
            if (MOX::_combat_data[ship_idx].shield_arc_current[i] < min_shield) {
                min_shield = MOX::_combat_data[ship_idx].shield_arc_current[i];
                weakest_idx = i;
            }
        }

        return weakest_idx;
    }

    void __cdecl Get_Multiple_Torpedo_Coords_(int16_t missile_idx, int16_t group_size, int16_t* out_x, int16_t* out_y) {
        uint8_t facing_dir = (uint8_t)MOX::_missile[missile_idx].facing_dir;

        memset(out_x, 0, sizeof(*out_x) * 4);
        memset(out_y, 0, sizeof(*out_y) * 4);

        int32_t stardate_mod = (int32_t)MOX::_stardate % 2;

        if (group_size == 2) {
            if (stardate_mod == 1) {
                out_x[0] = _torpedo_group_offset[facing_dir][0];
                out_y[0] = _torpedo_group_offset[facing_dir][1];
                out_x[1] = _torpedo_group_offset[facing_dir][4];
                out_y[1] = _torpedo_group_offset[facing_dir][5];
            } else {
                out_x[0] = _torpedo_group_offset[facing_dir][2];
                out_y[0] = _torpedo_group_offset[facing_dir][3];
                out_x[1] = _torpedo_group_offset[facing_dir][6];
                out_y[1] = _torpedo_group_offset[facing_dir][7];
            }
        } else if (group_size == 3) {
            if (stardate_mod == 1) {
                out_x[1] = _torpedo_group_offset[facing_dir][0];
                out_y[1] = _torpedo_group_offset[facing_dir][1];
                out_x[2] = _torpedo_group_offset[facing_dir][4];
                out_y[2] = _torpedo_group_offset[facing_dir][5];
            } else {
                out_x[1] = _torpedo_group_offset[facing_dir][2];
                out_y[1] = _torpedo_group_offset[facing_dir][3];
                out_x[2] = _torpedo_group_offset[facing_dir][6];
                out_y[2] = _torpedo_group_offset[facing_dir][7];
            }
        } else if (group_size == 4) {
            for (int16_t i = 0; i < 4; ++i) {
                out_x[i] = _torpedo_group_offset[facing_dir][i * 2];
                out_y[i] = _torpedo_group_offset[facing_dir][i * 2 + 1];
            }
        }
    }

    void __cdecl Get_Multiple_Missile_Coords_(int16_t missile_idx, int16_t quantity, int16_t* out_x, int16_t* out_y) {
        uint8_t dir = (uint8_t)MOX::_missile[missile_idx].facing_dir;

        memset(out_x, 0, sizeof(*out_x) * 9);
        memset(out_y, 0, sizeof(*out_y) * 9);

        if ((uint16_t)(quantity - 2) > 7) {
            return;
        }

        switch (quantity) {
            case 2:
                out_x[0] = (int16_t)((int32_t)CMBTMIS::_missile_group_offset[dir][2] / 2);
                out_y[0] = (int16_t)((int32_t)CMBTMIS::_missile_group_offset[dir][3] / 2);
                out_x[1] = (int16_t)((int32_t)CMBTMIS::_missile_group_offset[dir][10] / 2);
                out_y[1] = (int16_t)((int32_t)CMBTMIS::_missile_group_offset[dir][11] / 2);
                break;

            case 3:
                if ((MOX::_stardate % 2) == 1) {
                    out_x[1] = CMBTMIS::_missile_group_offset[dir][6];
                    out_y[1] = CMBTMIS::_missile_group_offset[dir][7];
                    out_x[2] = CMBTMIS::_missile_group_offset[dir][14];
                    out_y[2] = CMBTMIS::_missile_group_offset[dir][15];
                } else {
                    out_x[1] = CMBTMIS::_missile_group_offset[dir][2];
                    out_y[1] = CMBTMIS::_missile_group_offset[dir][3];
                    out_x[2] = CMBTMIS::_missile_group_offset[dir][10];
                    out_y[2] = CMBTMIS::_missile_group_offset[dir][11];
                }
                break;

            case 4:
                out_x[0] = CMBTMIS::_missile_group_offset[dir][2];
                out_y[0] = CMBTMIS::_missile_group_offset[dir][3];
                out_x[1] = CMBTMIS::_missile_group_offset[dir][6];
                out_y[1] = CMBTMIS::_missile_group_offset[dir][7];
                out_x[2] = CMBTMIS::_missile_group_offset[dir][10];
                out_y[2] = CMBTMIS::_missile_group_offset[dir][11];
                out_x[3] = CMBTMIS::_missile_group_offset[dir][14];
                out_y[3] = CMBTMIS::_missile_group_offset[dir][15];
                break;

            case 5:
                out_x[1] = CMBTMIS::_missile_group_offset[dir][2];
                out_y[1] = CMBTMIS::_missile_group_offset[dir][3];
                out_x[2] = CMBTMIS::_missile_group_offset[dir][6];
                out_y[2] = CMBTMIS::_missile_group_offset[dir][7];
                out_x[3] = CMBTMIS::_missile_group_offset[dir][10];
                out_y[3] = CMBTMIS::_missile_group_offset[dir][11];
                out_x[4] = CMBTMIS::_missile_group_offset[dir][14];
                out_y[4] = CMBTMIS::_missile_group_offset[dir][15];
                break;

            case 6:
                out_x[0] = CMBTMIS::_missile_group_offset[dir][2];
                out_y[0] = CMBTMIS::_missile_group_offset[dir][3];
                out_x[1] = CMBTMIS::_missile_group_offset[dir][4];
                out_y[1] = CMBTMIS::_missile_group_offset[dir][5];
                out_x[2] = CMBTMIS::_missile_group_offset[dir][6];
                out_y[2] = CMBTMIS::_missile_group_offset[dir][7];
                out_x[3] = CMBTMIS::_missile_group_offset[dir][10];
                out_y[3] = CMBTMIS::_missile_group_offset[dir][11];
                out_x[4] = CMBTMIS::_missile_group_offset[dir][12];
                out_y[4] = CMBTMIS::_missile_group_offset[dir][13];
                out_x[5] = CMBTMIS::_missile_group_offset[dir][14];
                out_y[5] = CMBTMIS::_missile_group_offset[dir][15];
                break;

            case 7:
                out_x[1] = CMBTMIS::_missile_group_offset[dir][2];
                out_y[1] = CMBTMIS::_missile_group_offset[dir][3];
                out_x[2] = CMBTMIS::_missile_group_offset[dir][4];
                out_y[2] = CMBTMIS::_missile_group_offset[dir][5];
                out_x[3] = CMBTMIS::_missile_group_offset[dir][6];
                out_y[3] = CMBTMIS::_missile_group_offset[dir][7];
                out_x[4] = CMBTMIS::_missile_group_offset[dir][10];
                out_y[4] = CMBTMIS::_missile_group_offset[dir][11];
                out_x[5] = CMBTMIS::_missile_group_offset[dir][12];
                out_y[5] = CMBTMIS::_missile_group_offset[dir][13];
                out_x[6] = CMBTMIS::_missile_group_offset[dir][14];
                out_y[6] = CMBTMIS::_missile_group_offset[dir][15];
                break;

            case 8:
                for (int32_t i = 0; i < 8; ++i) {
                    out_x[i] = CMBTMIS::_missile_group_offset[dir][i * 2];
                    out_y[i] = CMBTMIS::_missile_group_offset[dir][i * 2 + 1];
                }
                break;

            case 9:
                for (int32_t i = 0; i < 8; ++i) {
                    out_x[i + 1] = CMBTMIS::_missile_group_offset[dir][i * 2];
                    out_y[i + 1] = CMBTMIS::_missile_group_offset[dir][i * 2 + 1];
                }
                break;
        }
    }

    void __cdecl Fighter_Combat_SFX_(int16_t missile_idx, int16_t x, int16_t y, int16_t dmg, int16_t sys_dmg) {
        int16_t missile_type;
        int16_t target_unit_idx;
        int16_t target_x_scr, target_y_scr;
        int16_t source_x_scr, source_y_scr;
        int16_t beam_count;
        int16_t missile_x_offsets[10];
        int16_t missile_y_offsets[10];
        int16_t beams[18];
        int16_t shot_idx;
        int16_t off_x, off_y;
        uint8_t drawing;

        s_missile *missile = &MOX::_missile[missile_idx];
        missile_type = missile->type;
        target_unit_idx = missile->target_unit_idx;

        if (missile->is_anti_missile_rocket) {
            if (!Missile_On_Combat_Screen_(target_unit_idx)) {
                COMBINIT::Snap_Center_Combat_Screen_(MOX::_missile[target_unit_idx].x / 20, MOX::_missile[target_unit_idx].y / 20);
            }
            target_x_scr = MOX::_missile[target_unit_idx].x - MOX::_cur_combat_x * 20;
            target_y_scr = MOX::_missile[target_unit_idx].y - MOX::_cur_combat_y * 20;
        } else {
            if (!CMBTDRW1::Ship_Completely_On_Screen_(target_unit_idx)) {
                COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[target_unit_idx].x, MOX::_combat_data[target_unit_idx].y);
            }
            target_x_scr = x - MOX::_cur_combat_x * 20;
            target_y_scr = y - MOX::_cur_combat_y * 20;
        }

        source_x_scr = missile->x - MOX::_cur_combat_x * 20;
        source_y_scr = missile->y - MOX::_cur_combat_y * 20;

        beam_count = (missile->quantity + (missile->quantity < 0 ? 3 : 0)) / 4;
        if (beam_count > 9) beam_count = 9;
        if (beam_count < 1) beam_count = 1;

        CMBTMIS::Get_Multiple_Missile_Coords_(missile_idx, beam_count, missile_x_offsets, missile_y_offsets);
        shot_idx = 0;

        do {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            missile->type = WEAPON_NO_WEAPONS;
            CMBTDRW1::Draw_Main_Combat_Screen_();
            missile->type = missile_type;

            if (!missile->is_anti_missile_rocket && missile->target_unit_idx == MOX::_dont_draw_ship) {
                COMBINIT::Ship_Center_Offsets_(missile->target_unit_idx, &off_x, &off_y);
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();
                CMBTDRW1::Draw_Ship_(missile->target_unit_idx, target_x_scr - off_x, target_y_scr - off_y);
                video::Clip_Off_();
                graphics::Reset_Window_();
            }

            for (int16_t i = 0; i < beam_count; ++i) {
                beams[i * 2] = missile_x_offsets[i] + source_x_scr - 2;
                beams[i * 2 + 1] = missile_y_offsets[i] + source_y_scr - 2;
            }

            CMBTMIS::Draw_Fighter_(missile_idx, source_x_scr, source_y_scr, 1);
            int16_t owner = CMBTMIS::Missile_Owner_(missile_idx);
            drawing = BEAMS::Draw_Fighter_Beam_(beams, beam_count, target_x_scr, target_y_scr, MOX::_fighter_beam_type[owner], shot_idx);
            
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
            shot_idx++;
        } while (drawing);

        if (missile->is_anti_missile_rocket) {
            BEAMS::Add_Damage_Indicator_For_Missile_(target_x_scr, target_y_scr, 1, dmg);
            BEAMS::Draw_Damage_Message_Queue_Until_Done_();
        } else {
            COMBINIT::Ship_Center_Offsets_(target_unit_idx, &off_x, &off_y);
            BEAMS::Add_Damage_Indicator_For_Ship_(target_x_scr - off_x, target_y_scr - off_y, target_unit_idx, 1, dmg, sys_dmg);
            
            while (!BEAMS::Done_Drawing_Damage_Message_Queue_()) {
                timer::Mark_Time_();
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                graphics::Set_Window_(0, 0, 639, 350);
                video::Clip_On_();
                if (target_unit_idx == MOX::_dont_draw_ship) {
                    COMBINIT::Ship_Center_Offsets_(target_unit_idx, &off_x, &off_y);
                    CMBTDRW1::Draw_Ship_(target_unit_idx, target_x_scr - off_x, target_y_scr - off_y);
                }
                BEAMS::Draw_Damage_Message_Queue_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(1);
                video::Clip_Off_();
                graphics::Reset_Window_();
            }
        }
    }

    int16_t __cdecl Missile_Type_(int16_t missile_idx) {
        switch (MOX::_missile[missile_idx].type) {
            case WEAPON_NUCLEAR_MISSILE:
                return 0;
            case WEAPON_MERCULITE_MISSILE:
                return 1;
            case WEAPON_PULSON_MISSILE:
                return 2;
            case WEAPON_ZEON_MISSILE:
                return 3;
            case WEAPON_ANTI_MATTER_TORP:
                return 4;
            case WEAPON_PROTON_TORPEDO:
                return 5;
            case WEAPON_PLASMA_TORPEDO:
            case WEAPON_DRAGON_BREATH:
                return 6;
            case WEAPON_ASSAULT_SHUTTLE:
                return 3;
            case WEAPON_HEAVY_FIGHTER:
                return 1;
            case WEAPON_BOMBER:
                return 2;
            case WEAPON_INTERCEPTOR:
                return 0;
            default:
                return 0;
        }
    }

    int16_t __cdecl Picture_Count_(int16_t missile_idx) {
        s_missile* missile = &MOX::_missile[missile_idx];
        int8_t weapon_type_class = TECHDATA::_weapons[missile->type].type;
        int16_t count;

        if (weapon_type_class == WEAPON_TYPE_MISSILE) {
            count = (missile->quantity / 5) + 1;
            if (count <= 0) {
                return 1;
            }
            if (count > 9) {
                return 9;
            }
            return count;
        }

        if (weapon_type_class == WEAPON_TYPE_TORPEDO) {
            count = (missile->quantity / 5) + 1;
            if (count <= 0) {
                return 1;
            }
            if (count > 4) {
                return 4;
            }
            return count;
        }

        if (weapon_type_class == WEAPON_TYPE_FIGHTER) {
            count = missile->quantity;
            if (count <= 9) {
                count = 9;
            }
            return count;
        }

        return 1;
    }

    void __cdecl Kill_Missile_(int16_t missile_idx) {
        // Clear the missile data structure
        memset(&MOX::_missile[missile_idx], 0, sizeof(s_missile));

        // Scan for all active anti-missile rockets that were targeting this missile
        for (int16_t i = 0; i < 300; ++i) {
            if (MOX::_missile[i].target_unit_idx == missile_idx &&
                MOX::_missile[i].is_anti_missile_rocket == 1) {
                Retarget_Missile_(i);
            }
        }
    }

    void __cdecl Draw_Fighter_(int16_t missile_idx, int16_t base_x, int16_t base_y, int16_t combat_view_type) {
        int16_t rand_x = 0;
        int16_t rand_y = 0;
        int16_t num_missiles;
        int16_t x_coords[10];
        int16_t y_coords[10];
        int16_t type;
        int16_t owner;
        int16_t window_height;
        int16_t facing_dir;
        s_animation_header* fighter_anim;
        int16_t final_target_idx = Final_Flyer_Target_(MOX::_missile, missile_idx);

        num_missiles = MOX::_missile[missile_idx].quantity / 4;
        if (num_missiles > 9) {
            num_missiles = 9;
        }
        if (num_missiles < 1) {
            num_missiles = 1;
        }

        Get_Multiple_Missile_Coords_(missile_idx, num_missiles, x_coords, y_coords);

        if (combat_view_type == 1) {
            window_height = 479;
        } else {
            window_height = 350;
        }

        graphics::Set_Window_(0, 0, 639, window_height);
        video::Clip_On_();

        for (int16_t i = 0; i < num_missiles; ++i) {
            type = Missile_Type_(missile_idx);

            if (combat_view_type == 0 && (MOX::_ship_frame % 3 == 0)) {
                uint32_t old_seed = game_random::Get_Random_Seed_();
                game_random::Set_Random_Seed_((uint32_t)clock() + i + missile_idx);
                rand_x = (int16_t)game_random::Random_(3) - 2;
                rand_y = (int16_t)game_random::Random_(3) - 2;
                game_random::Set_Random_Seed_(old_seed);
            }

            owner = Missile_Owner_(missile_idx);
            facing_dir = (uint8_t)MOX::_missile[missile_idx].facing_dir;
            fighter_anim = MOX::_fighter_seg[owner][type * 16 + facing_dir];

            if (final_target_idx == MOX::_cur_ship) {
                animate::Set_Animation_Frame_(fighter_anim, (MOX::_ship_frame + missile_idx) % 3);
            } else {
                animate::Reset_Animation_Frame_(fighter_anim);
            }

            animate::Draw_(x_coords[i] + base_x + rand_x - 12,
                          y_coords[i] + base_y + rand_y - 12,
                          fighter_anim);
        }

        video::Clip_Off_();
        graphics::Reset_Window_();
    }

    void __cdecl Draw_Missile_(int16_t missile_idx, int16_t screen_x, int16_t screen_y, int16_t draw_fullscreen_view) {
        if (MOX::_missile[missile_idx].type > WEAPON_NO_WEAPONS) {
            if (Missile_On_Combat_Screen_(missile_idx) != 0 || draw_fullscreen_view != 0) {
                Missile_Owner_(missile_idx);
                int16_t type_idx = Missile_Type_(missile_idx);
                int16_t facing_idx = (int16_t)(uint8_t)MOX::_missile[missile_idx].facing_dir;
                int16_t final_target_idx = Final_Flyer_Target_(MOX::_missile, missile_idx);

                if (TECHDATA::_weapons[MOX::_missile[missile_idx].type].type != WEAPON_TYPE_FIGHTER) {
                    if (MOX::_missile[missile_idx].target_unit_idx == MOX::_cur_ship || MOX::_missile[missile_idx].type == WEAPON_PROTON_TORPEDO) {
                        animate::Set_Animation_Frame_(MOX::_msl_seg[type_idx * 16 + facing_idx], (MOX::_ship_frame + missile_idx) % 3);
                    } else {
                        animate::Reset_Animation_Frame_(MOX::_msl_seg[type_idx * 16 + facing_idx]);
                    }
                }

                if (final_target_idx != MOX::_cur_ship || MOX::_draw_active_missiles == 1) {
                    int16_t weapon_type = MOX::_missile[missile_idx].type;
                    if (TECHDATA::_weapons[weapon_type].type == WEAPON_TYPE_FIGHTER) {
                        Draw_Fighter_(missile_idx, screen_x, screen_y, draw_fullscreen_view);
                        return;
                    }

                    int16_t count = Picture_Count_(missile_idx);
                    int16_t coords_x[10];
                    int16_t coords_y[10];

                    if (TECHDATA::_weapons[weapon_type].type != WEAPON_TYPE_MISSILE && weapon_type != WEAPON_ANTI_MATTER_TORP) {
                        Get_Multiple_Torpedo_Coords_(missile_idx, count, coords_y, coords_x);
                        graphics::Set_Window_(0, 0, 0x27f, (draw_fullscreen_view == 1) ? 0x1df : 0x15e);
                        video::Clip_On_();
                        for (int32_t i = 0; i < count; ++i) {
                            animate::Draw_(coords_x[i] + screen_x - 0x17, coords_y[i] + screen_y - 0x17, MOX::_msl_seg[type_idx * 16 + facing_idx]);
                        }
                        video::Clip_Off_();
                        graphics::Reset_Window_();
                    } else {
                        Get_Multiple_Missile_Coords_(missile_idx, count, coords_y, coords_x);
                        graphics::Set_Window_(0, 0, 0x27f, (draw_fullscreen_view == 1) ? 0x1df : 0x15e);
                        video::Clip_On_();
                        for (int32_t i = 0; i < count; ++i) {
                            animate::Draw_(coords_x[i] + screen_x - 0xc, coords_y[i] + screen_y - 0xc, MOX::_msl_seg[type_idx * 16 + facing_idx]);
                        }
                        video::Clip_Off_();
                        graphics::Reset_Window_();
                    }
                }
            }
        }
    }

    int16_t __cdecl Fighter_Ocv_(int16_t player_idx) {
        int16_t ocv;
        int16_t best_computer_idx;
        int16_t pilot_bonus;

        ocv = CMBTFIR2::Fleet_Offensive_Bonus_(player_idx);
        best_computer_idx = INITSHIP::Best_Computer_(player_idx);

        // Add computer offensive bonus
        ocv += TECHDATA::_computers[best_computer_idx].bonus;

        // Players 0-7 get their racial/trained ship attack trait bonus
        if (player_idx < MAX_PLAYERS) {
            ocv += MOX::_player[player_idx].traits[TRAIT_SHIP_ATTACK];
        }

        pilot_bonus = CMBTFIR2::Get_Fleet_Pilot_Bonus_(player_idx);

        // Base fighter OCV is 50
        return ocv + pilot_bonus + 50;
    }

    void __cdecl Next_Missile_Id_(int16_t* missile_id) {
        int16_t i;
        bool found = false;

        *missile_id = -1;

        for (i = 0; i < 300; ++i) {
            if (found) {
                break;
            }

            if (MOX::_missile[i].type == WEAPON_NO_WEAPONS) {
                found = true;
                *missile_id = i;
            }
        }
    }

    void __cdecl Fire_Missile_(int16_t source_idx, int16_t target_idx, int16_t weapon_slot, int16_t is_anti_rocket) {
        int16_t missile_idx;
        int16_t source_offset_x, source_offset_y;
        int16_t target_coords_x, target_coords_y;

        Next_Missile_Id_(&missile_idx);
        if (missile_idx != -1) {
            memset(&MOX::_missile[missile_idx], 0, sizeof(s_missile));

            MOX::_missile[missile_idx].source_unit_idx = source_idx;
            MOX::_missile[missile_idx].hits_taken = 0;
            MOX::_missile[missile_idx].owner = MOX::_combat_data[source_idx].owner;
            MOX::_missile[missile_idx].type = MOX::_combat_data[source_idx].weapons[weapon_slot].weapon_id;
            MOX::_missile[missile_idx].quantity = MOX::_combat_data[source_idx].weapons[weapon_slot].count;
            MOX::_missile[missile_idx].specials = MOX::_combat_data[source_idx].weapons[weapon_slot].specials;
            MOX::_missile[missile_idx].target_unit_idx = target_idx;

            COMBINIT::Ship_Center_Offsets_(source_idx, &source_offset_x, &source_offset_y);
            MOX::_missile[missile_idx].x = (MOX::_combat_data[source_idx].x * 20) + source_offset_x;
            MOX::_missile[missile_idx].y = (MOX::_combat_data[source_idx].y * 20) + source_offset_y;

            MOX::_missile[missile_idx].facing_dir = (int8_t)Missile_Facing_(missile_idx, target_idx, is_anti_rocket);

            if (MOX::_combat_init != 0) {
                MOX::_missile[missile_idx].fighter_count = -1;
            } else {
                MOX::_missile[missile_idx].fighter_count = 0;
            }

            MOX::_missile[missile_idx].speed = (uint8_t)Missile_Speed_(MOX::_missile[missile_idx].type,
                                                                    MOX::_missile[missile_idx].specials,
                                                                    MOX::_missile[missile_idx].owner);
            MOX::_missile[missile_idx].travel_dist = (int16_t)((uint16_t)(uint8_t)MOX::_missile[missile_idx].speed * 20);
            MOX::_missile[missile_idx].is_active = 1;
            MOX::_missile[missile_idx].is_anti_missile_rocket = (int8_t)is_anti_rocket;

            if (TECHDATA::_weapons[MOX::_missile[missile_idx].type].type == WEAPON_TYPE_FIGHTER) {
                MOX::_missile[missile_idx].quantity = (int16_t)MOX::_combat_data[source_idx].weapons[weapon_slot].count << 2;

                switch (MOX::_missile[missile_idx].type) {
                    case WEAPON_ASSAULT_SHUTTLE:
                    case WEAPON_BOMBER:
                        MOX::_missile[missile_idx].fighter_count = 1;
                        break;
                    case WEAPON_HEAVY_FIGHTER:
                        MOX::_missile[missile_idx].fighter_count = 2;
                        break;
                    case WEAPON_INTERCEPTOR:
                        MOX::_missile[missile_idx].fighter_count = 4;
                        break;
                }
            }

            if (is_anti_rocket == 0) {
                int16_t target_offset_x, target_offset_y;
                COMBINIT::Ship_Center_Offsets_(target_idx, &target_offset_x, &target_offset_y);
                target_coords_x = (MOX::_combat_data[target_idx].x * 20) + target_offset_x;
                target_coords_y = (MOX::_combat_data[target_idx].y * 20) + target_offset_y;
            } else {
                target_coords_x = MOX::_missile[target_idx].x;
                target_coords_y = MOX::_missile[target_idx].y;
            }

            if (MOX::_apply_damage_to_planet == 1 && (MOX::_super_fast_flag == 0 ||
               (MOX::_missile[missile_idx].type != WEAPON_PROTON_TORPEDO &&
                MOX::_missile[missile_idx].type != WEAPON_DRAGON_BREATH))) {
                CMBTSPEC::Missile_Launch_FX_(missile_idx, target_coords_x, target_coords_y, is_anti_rocket);
            } else {
                Resolve_Missile_(missile_idx, target_coords_x, target_coords_y);
            }

            if (MOX::_missile[missile_idx].type > WEAPON_NO_WEAPONS) {
                for (int16_t i = 0; i < 300; ++i) {
                    if (MOX::_missile[i].type == MOX::_missile[missile_idx].type &&
                        MOX::_missile[i].specials == MOX::_missile[missile_idx].specials &&
                        MOX::_missile[i].target_unit_idx == MOX::_missile[missile_idx].target_unit_idx &&
                        MOX::_missile[i].x == MOX::_missile[missile_idx].x &&
                        MOX::_missile[i].y == MOX::_missile[missile_idx].y &&
                        MOX::_missile[i].is_anti_missile_rocket == MOX::_missile[missile_idx].is_anti_missile_rocket &&
                        i != missile_idx) {

                        MOX::_missile[i].quantity += MOX::_missile[missile_idx].quantity;
                        Kill_Missile_(missile_idx);
                    }
                }
            }
        }
    }

    int16_t __cdecl Get_Missile_Hits_(int16_t missile_id, int16_t ship_idx, uint16_t flags) {
        // Unmapped or out-of-range missile IDs fall back to returning the original missile_id.
        int16_t hits = missile_id;
        int16_t armor_idx;
        int16_t absorption;

        switch (missile_id) {
            case WEAPON_NUCLEAR_MISSILE:
                hits = 4;
                break;

            case WEAPON_MERCULITE_MISSILE:
                hits = 8;
                break;

            case WEAPON_PULSON_MISSILE:
                hits = 12;
                break;

            case WEAPON_ZEON_MISSILE:
                hits = 16;
                break;

            case WEAPON_ANTI_MATTER_TORP:
            case WEAPON_PROTON_TORPEDO:
            case WEAPON_PLASMA_TORPEDO:
            case WEAPON_DRAGON_BREATH:
                hits = 5;
                break;

            case WEAPON_ASSAULT_SHUTTLE:
                armor_idx = INITSHIP::Best_Armor_(ship_idx);
                absorption = TECHDATA::_armor[armor_idx].ships_bonus;
                hits = (int16_t)((((int32_t)absorption * 3) + 300) * 2 / 200);
                break;

            case WEAPON_HEAVY_FIGHTER:
                armor_idx = INITSHIP::Best_Armor_(ship_idx);
                absorption = TECHDATA::_armor[armor_idx].ships_bonus;
                hits = (int16_t)((((int32_t)absorption * 5) + 500) * 2 / 200);
                break;

            case WEAPON_BOMBER:
                armor_idx = INITSHIP::Best_Armor_(ship_idx);
                absorption = TECHDATA::_armor[armor_idx].ships_bonus;
                hits = (int16_t)((((int32_t)absorption * 8) + 800) / 200);
                break;

            case WEAPON_INTERCEPTOR:
                armor_idx = INITSHIP::Best_Armor_(ship_idx);
                absorption = TECHDATA::_armor[armor_idx].ships_bonus;
                hits = (int16_t)((((int32_t)absorption * 4) + 400) / 200);
                break;

            default:
                break;
        }

        if (flags & WEAPON_MOD_FLAG_HEAVILY_ARMORED) {
            hits *= 2;
        }

        return hits;
    }

    void __cdecl Draw_Missiles_(void) {
        for (int16_t i = 299; i >= 0; --i) {
            int16_t screen_x = MOX::_missile[i].x - (MOX::_cur_combat_x * 20);
            int16_t screen_y = MOX::_missile[i].y - (MOX::_cur_combat_y * 20);

            Draw_Missile_(i, screen_x, screen_y, 0);
        }
    }

    void __cdecl Draw_Missile_Status_(int16_t x_off, int16_t y_off, int16_t missile_idx) {
        char status_str[40];
        char msg_buf[80];
        s_animation_header* anim;
        const char* target_name;

        graphics::Fill_(401, 379, 488, 467, 0);

        anim = (s_animation_header*)buffer::Buffer_Reload_("combat.lbx", 45, MOX::_global_cache_seg);
        animate::Draw_(401, 379, anim);

        CMBTMIS::Draw_Missile_(missile_idx, x_off + 44, y_off + 44, 1);
        fonts::Set_Font_Style_(0, &MOX::_combat_green);

        KEN::Ken_Get_Text_Message_(52, msg_buf, 80);
        strlcpy(status_str, msg_buf, sizeof(status_str));

        if (MOX::_missile[missile_idx].is_anti_missile_rocket == 0) {
            target_name = MOX::_combat_data[MOX::_missile[missile_idx].target_unit_idx].name;
        } else {
            int16_t target_idx = MOX::_missile[missile_idx].target_unit_idx;
            target_name = TECHDATA::_weapons[MOX::_missile[target_idx].type].name;
        }

        strlcat(status_str, target_name, sizeof(status_str));
        fonts::Print_Centered_(x_off + 43, y_off + 2, status_str);

        snprintf(status_str, sizeof(status_str), "%i", MOX::_missile[missile_idx].quantity);

        int16_t weapon_type = MOX::_missile[missile_idx].type;
        if (TECHDATA::_weapons[weapon_type].type == WEAPON_TYPE_MISSILE) {
            int16_t msg_id = (MOX::_missile[missile_idx].quantity == 1) ? 53 : 107;
            KEN::Ken_Get_Text_Message_(msg_id, msg_buf, 80);
            strlcat(status_str, msg_buf, sizeof(status_str));
        } else if (TECHDATA::_weapons[weapon_type].type == WEAPON_TYPE_FIGHTER) {
            strlcat(status_str, " ", sizeof(status_str));
            const char* weapon_name;
            if (weapon_type == WEAPON_BOMBER) {
                weapon_name = (MOX::_missile[missile_idx].quantity == 1) ? TECHDATA::_weapons[WEAPON_BOMBER].name : TECHDATA::_weapons[WEAPON_BOMBER].name_plural;
            } else if (weapon_type == WEAPON_INTERCEPTOR) {
                weapon_name = (MOX::_missile[missile_idx].quantity == 1) ? TECHDATA::_weapons[WEAPON_INTERCEPTOR].name : TECHDATA::_weapons[WEAPON_INTERCEPTOR].name_plural;
            } else if (weapon_type == WEAPON_HEAVY_FIGHTER) {
                weapon_name = (MOX::_missile[missile_idx].quantity == 1) ? TECHDATA::_weapons[WEAPON_HEAVY_FIGHTER].name : TECHDATA::_weapons[WEAPON_HEAVY_FIGHTER].name_plural;
            } else {
                weapon_name = (MOX::_missile[missile_idx].quantity == 1) ? TECHDATA::_weapons[WEAPON_ASSAULT_SHUTTLE].name : TECHDATA::_weapons[WEAPON_ASSAULT_SHUTTLE].name_plural;
            }
            strlcat(status_str, weapon_name, sizeof(status_str));
        } else {
            int16_t msg_id = (MOX::_missile[missile_idx].quantity == 1) ? 54 : 108;
            KEN::Ken_Get_Text_Message_(msg_id, msg_buf, 80);
            strlcat(status_str, msg_buf, sizeof(status_str));
        }

        fonts::Print_Centered_(x_off + 43, y_off + 81, status_str);
    }

    int16_t __cdecl Missile_Dcv_(int16_t weapon_idx, int16_t player_idx) {
        int16_t bonus = 0;
        int16_t speed = Missile_Speed_(weapon_idx, 0, player_idx);
        int16_t base_dcv = speed * 5;

        if (TECHDATA::_weapons[weapon_idx].type == WEAPON_TYPE_FIGHTER) {
            bonus = CMBTFIR2::Get_Fleet_Pilot_Bonus_(player_idx);
            int16_t fleet_base = 0;
            bonus += CMBTFIR2::Fleet_Defensive_Bonus_(player_idx, 0, &fleet_base);

            // Add defense trait bonus if player is valid
            if (player_idx < MAX_PLAYERS && player_idx != -1) {
                bonus += MOX::_player[player_idx].traits[TRAIT_SHIP_DEFENSE];
            }
        }

        switch (weapon_idx) {
            case WEAPON_NUCLEAR_MISSILE:
                return base_dcv - 10;
            case WEAPON_MERCULITE_MISSILE:
                return base_dcv + 15;
            case WEAPON_PULSON_MISSILE:
                return base_dcv + 40;
            case WEAPON_ZEON_MISSILE:
                return base_dcv + 70;
            case WEAPON_ASSAULT_SHUTTLE:
            case WEAPON_HEAVY_FIGHTER:
            case WEAPON_BOMBER:
            case WEAPON_INTERCEPTOR:
                return base_dcv + bonus;
            default:
                return base_dcv;
        }
    }
}