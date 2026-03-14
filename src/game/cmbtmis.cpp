#include "pch.h"

namespace CMBTMIS {
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

        // Retargeting requires rack type 4 (Fighters) or Emissions Guidance tech (tech app 46)
        if (TECHDATA::_weapons[missile.type].rack == 4 || 
            (owner_player < 8 && MOX::_player[owner_player].tech_applications[46] == 3)) {
            
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
                            missile.target_unit_idx != ship_idx) {

                            int16_t range = CMBTAI::Range_To_XY_(ship_idx, missile_idx);
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
            // If no new target found, rack type 4 (Fighters) returns to source ship if it's still active
            if (TECHDATA::_weapons[missile.type].rack == 4 && 
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

        if (TECHDATA::_weapons[type].rack == 4 /* MIRV */ && missile->is_anti_missile_rocket == 0) {
            target_x += MOX::_missile_hit_radius[MOX::_combat_data[missile->target_unit_idx].size_class] / 2;
        }

        if (missile->x == target_x && missile->y == target_y) {
            return 1;
        }

        if (type > 0) {
            int16_t dist = move_dist;
            if (missile->travel_dist < dist) {
                dist = missile->travel_dist;
            }

            Coord_Movement_(missile->x, missile->y, target_x, target_y, dist, &dx, &dy);

            missile->x += dx;
            missile->y += dy;

            for (int16_t i = 0; i < 300; ++i) {
                s_missile *other = &MOX::_missile[i];
                if (other->type > 0 && other->is_anti_missile_rocket == 1 && other->target_unit_idx == missile_idx) {
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
        if ((type == 20 || type == 40) && hit_flag == 0 && (missile->specials & 0x20) == 0) {
            int16_t owner = Missile_Owner_(missile_idx);
            int16_t speed = Missile_Speed_(type, missile->specials, owner);
            int16_t adj_val = speed * ((uint8_t)missile->fighter_count + 1) * 20 - missile->travel_dist;

            owner = Missile_Owner_(missile_idx);
            int16_t bonus = MOX::_ordnance_bonus[owner];
            if (missile->specials & 0x4000) {
                bonus += 50;
            }

            int16_t base_damage = (int16_t)((TECHDATA::_weapons[type].damage * bonus) / 100);
            int16_t damage_adjustment;
            if (type == 20) {
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

    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t step, int16_t* out_x, int16_t* out_y) {
        int16_t dx = x2 - x1;
        int16_t dy = y2 - y1;

        // Calculate absolute values for dominance check
        int abs_dx = (dx < 0) ? -dx : dx;
        int abs_dy = (dy < 0) ? -dy : dy;

        if (abs_dx < abs_dy) {
            // Movement is dominated by Y-axis (vertical)
            int16_t sign_y = (dy < 0) ? -1 : 1;
            int16_t scaled_step_y = (int16_t)(sign_y * step);
            
            *out_y = scaled_step_y;
            
            // Calculate proportional X delta: (scaled_step_y * dx) / dy
            // Using int to preserve precision during intermediate multiplication
            *out_x = (int16_t)((int)scaled_step_y * (int)dx / (int)dy);
        } else {
            // Movement is dominated by X-axis (horizontal) or is exactly 45 degrees
            int16_t sign_x = (dx < 0) ? -1 : 1;
            int16_t scaled_step_x = (int16_t)(sign_x * (int16_t)step);
            
            *out_x = scaled_step_x;
            
            // Calculate proportional Y delta: (scaled_step_x * dy) / dx
            // Division by zero risk if dx is 0, but logic implies abs_dx >= abs_dy
            if (dx != 0) {
                *out_y = (int16_t)((int)scaled_step_x * (int)dy / (int)dx);
            } else {
                *out_y = 0;
            }
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
            if (reload_count != 0) {
                for (int16_t i = 0; i < 8; ++i) {
                    if (MOX::_combat_data[source_unit_idx].weapons[i].weapon_id == type &&
                        MOX::_combat_data[source_unit_idx].weapons[i].ammo == 0) {
                        
                        MOX::_combat_data[source_unit_idx].weapons[i].ammo = 1;
                        MOX::_combat_data[source_unit_idx].weapons[i].shots_left = 0;

                        if (reload_count >= MOX::_combat_data[source_unit_idx].weapons[i].count) {
                            reload_count -= MOX::_combat_data[source_unit_idx].weapons[i].count;
                        } else {
                            MOX::_combat_data[source_unit_idx].weapons[i].count = reload_count;
                            reload_count = 0;
                        }
                    }
                    if (reload_count == 0) break;
                }
            }
            Kill_Missile_(missile_idx);
            return 0;
        }

        if (!Is_Missile_Target_Alive_(missile_idx)) {
            return 0;
        }

        int16_t shield_idx = 0;
        int16_t evade = 0;
        int16_t applied_hits = 0;
        int16_t shield_hit_dir = 0;

        if (is_anti_missile_rocket == 0) {
            if (COMBINIT::Does_Combat_Ship_Have_Special_(target_unit_idx, 19 /* Lightning Field */)) {
                int16_t loop_count = 0;
                if (missile->quantity > 0) {
                    do {
                        if (random::Random_(100) < 50) {
                            missile->quantity--;
                        }
                        loop_count++;
                    } while (loop_count < missile->quantity);
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

            if (TECHDATA::_weapons[type].rack == 4) {
                shield_idx = Find_Weakest_Shield_(target_unit_idx);
                evade = 0;
            } else {
                int16_t rel_bearing = CMBTDRW1::Relative_Bearing_XY_(target_unit_idx, missile->x, missile->y);
                shield_idx = CMBTFIRE::Facing_Shield_(rel_bearing);
                evade = MOX::_combat_data[target_unit_idx].tac_val_evade;
            }

            if (owner < 8) {
                if (MOX::_player[owner].tech_applications[160] == 3) {
                    evade -= 70;
                } else if (MOX::_player[owner].tech_applications[116] == 3) {
                    evade -= 40;
                } else if (MOX::_player[owner].tech_applications[181] == 3) {
                    evade -= 20;
                }
            } else {
                evade -= 40;
            }

            if (evade < 0) {
                evade = 0;
            } else if (specials & 0x0400) {
                evade /= 2;
            }

            if (type == 40) { // 0x28
                evade = 0;
            }
        }

        if (missile->quantity <= 0) {
            Kill_Missile_(missile_idx);
            return 0;
        }

        if (specials & 0x0200) {
            missile->quantity *= 4;
        }

        int16_t dmg_accum = 0;
        int16_t sys_dmg_accum = 0;

        if (TECHDATA::_weapons[type].rack == 4) {
            missile->is_active = 0;
            if (type == 28) { // 0x1C (Assault Shuttle)
                // TYPE MISMATCH: Resolve_Capture_ signature differs in header.
                using ResolveCaptureFn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
                auto resolve_capture = reinterpret_cast<ResolveCaptureFn>(std::uintptr_t{0x004e7ea0});
                resolve_capture(missile->source_unit_idx, target_unit_idx, missile->quantity, x, y);
                missile->fighter_count--;
            } else {
                if (type != 30) { // 0x1E (Heavy Bomber)
                    int16_t dmg_out = 0;
                    int16_t sys_dmg = 0;
                    CMBTFIRE::Fire_Fighter_Beam_(missile_idx, shield_idx, is_anti_missile_rocket, &dmg_out, &sys_dmg);
                    dmg_accum = dmg_out;
                    sys_dmg_accum = sys_dmg;
                    if (type != 29) { // 0x1D (Interceptor)
                        missile->fighter_count--;
                    }
                }

                if (missile->fighter_count != 0 && type != 31) { // 0x1F (Heavy Fighter)
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
            if (specials & 0x4000) {
                bonus += 50;
            }
            int16_t damage = (TECHDATA::_weapons[type].damage * bonus) / 100;

            if ((type == 20 || type == 40) && !(specials & 0x0020)) {
                int16_t degrad = 0;
                if (type == 20) { // 0x14
                    int16_t speed = Missile_Speed_(type, specials, Missile_Owner_(missile_idx));
                    degrad = (speed - 1) * 20 - missile->travel_dist;
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

            if (target_unit_idx == 0 && TECHDATA::_weapons[type].rack == 2) {
                damage /= 2;
            }

            int16_t loop_count = 0;
            if (missile->quantity > 0) {
                do {
                    int16_t roll = random::Random_(100);

                    if (COMBINIT::Does_Combat_Ship_Have_Special_(target_unit_idx, 8)) {
                        if (random::Random_(100) < 30) {
                            roll = -1000;
                        }
                    }

                    if (MOX::_combat_data[target_unit_idx].special_status_flag == 1) {
                        if (random::Random_(100) < 50) {
                            roll = -1000;
                        }
                    }

                    if (evade < roll) {
                        if (!(specials & 0x0100)) {
                            int16_t dmg_out = damage;
                            int16_t sys_dmg = 0;
                            // TYPE MISMATCH: Apply_Ship_Damage_ signature differs in header.
                            using ApplyShipDamageFn = int16_t(__cdecl*)(int16_t, int16_t*, int16_t*, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
                            auto apply_ship_damage = reinterpret_cast<ApplyShipDamageFn>(std::uintptr_t{0x004ad980});
                            int16_t applied = apply_ship_damage(target_unit_idx, &dmg_out, &sys_dmg, shield_idx, specials, applied_hits, 0, 0, specials & 0x2000);
                            dmg_accum += dmg_out;
                            sys_dmg_accum += sys_dmg;
                            applied_hits |= applied;
                        } else {
                            for (int16_t i = 0; i < 4; ++i) {
                                int16_t dmg_out = damage;
                                int16_t sys_dmg = 0;
                                // TYPE MISMATCH: Apply_Ship_Damage_ signature differs in header.
                                using ApplyShipDamageFn = int16_t(__cdecl*)(int16_t, int16_t*, int16_t*, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
                                auto apply_ship_damage = reinterpret_cast<ApplyShipDamageFn>(std::uintptr_t{0x004ad980});
                                apply_ship_damage(target_unit_idx, &dmg_out, &sys_dmg, i, specials, applied_hits, 0, 0, specials & 0x2000);
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

        if (TECHDATA::_weapons[type].rack == 4) {
            if (missile->fighter_count == 0) {
                int16_t source = missile->source_unit_idx;
                if (MOX::_combat_data[source].owner == Missile_Owner_(missile_idx) &&
                    COMBAT1::Combat_Ship_Is_Functional_(source) &&
                    type != 28) {
                    
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
}