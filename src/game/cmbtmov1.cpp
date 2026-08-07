#include "pch.h"

namespace CMBTMOV1 {
    int16_t __cdecl Turn_Cost_(int16_t ship_idx) {
        int16_t cost = 2;

        // Check for specific ship special systems that affect turning cost
        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_INERTIAL_STABILIZER)) {
            return 1;
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_INERTIAL_NULLIFIER)) {
            cost = 0;
        }

        return cost;
    }

    int8_t __cdecl Ship_Is_In_Stasis_(int16_t ship_idx) {
        const int16_t stasis_source_idx = MOX::_combat_data[ship_idx].stasis_source_idx == UINT8_MAX
            ? int16_t{-1}
            : MOX::_combat_data[ship_idx].stasis_source_idx;
        return stasis_source_idx != -1;
    }

void __cdecl Rotate_Ship_(int16_t ship_idx, int16_t target_facing, int16_t screen_x, int16_t screen_y)
{
    int16_t center_x;
    int16_t center_y;
    int16_t moved_missiles[300];
    int16_t moved_count;
    int16_t step;
    int16_t saved_dont_draw;
    int16_t current_facing;
    s_combat_data* ship = &MOX::_combat_data[ship_idx];

    current_facing = ship->facing_dir;
    COMBINIT::Ship_Center_Offsets_(ship_idx, &center_x, &center_y);

    center_x += ship->x * 20;
    center_y += ship->y * 20;

    int16_t diff = current_facing - target_facing;
    if (diff < 0) {
        step = (abs(diff) > 8) ? -1 : 1;
    } else {
        step = (diff > 8) ? 1 : -1;
    }

    saved_dont_draw = MOX::_dont_draw_ship;
    MOX::_dont_draw_ship = ship_idx;

    while (ship->facing_dir != target_facing && ship->unit_status == 0) {
        timer::Mark_Time_();
        
        current_facing += step;
        if (current_facing < 0) {
            current_facing += 16;
        }
        current_facing %= 16;
        
        ship->facing_dir = (int8_t)current_facing;
        moved_count = 0;

        for (int16_t i = 0; i < 300; ++i) {
            s_missile* missile = &MOX::_missile[i];
            if (missile->type > 0 && missile->target_unit_idx == ship_idx) {
                int16_t move_speed = 5;
                if (ship->current_speed != 0) {
                    move_speed = (int16_t)((missile->speed * 5) / ship->current_speed);
                }
                if (move_speed < 2) {
                    move_speed = 2;
                }

                if (CMBTMIS::Move_Missile_(i, center_x, center_y, move_speed) != 0) {
                    moved_missiles[moved_count++] = i;
                }
            }
        }

        if (MOX::_super_fast_flag == 0 && CMBTDRW1::Ship_On_Screen_(ship_idx) != 0) {
            video::Set_Page_Off_();
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                const int16_t tractor_target_idx = MOX::_combat_data[i].special_cooldown == UINT8_MAX
                    ? int16_t{-1}
                    : MOX::_combat_data[i].special_cooldown;
                if (MOX::_combat_data[i].unit_status == 0 && tractor_target_idx != -1) {
                    CMBTSPEC::Draw_Continuing_Tractor_(i);
                }
            }
            CMBTDRW1::Draw_Main_Combat_Screen_();
            graphics::Set_Window_(0, 0, 639, 350);
            video::Clip_On_();
            CMBTDRW1::Draw_Ship_(ship_idx, screen_x, screen_y);
            video::Clip_Off_();
            graphics::Reset_Window_();

            for (int16_t i = 0; i < 300; ++i) {
                s_missile* missile = &MOX::_missile[i];
                if (missile->type > 0 && missile->target_unit_idx == ship_idx) {
                    CMBTMIS::Draw_Missile_(i, missile->x - MOX::_cur_combat_x * 20, missile->y - MOX::_cur_combat_y * 20, 0);
                }
            }
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }

        for (int16_t i = 0; i < moved_count; ++i) {
            if (ship->unit_status == 0 && MOX::_missile[moved_missiles[i]].target_unit_idx == ship_idx) {
                CMBTMIS::Resolve_Missile_(moved_missiles[i], center_x, center_y);
            }
        }

        if (CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx) == 0) {
            int16_t fire_x = screen_x + MOX::_cur_combat_x * 20;
            int16_t fire_y = screen_y + MOX::_cur_combat_y * 20;
            CMBTFIR2::Defensive_Fire_Check_(ship_idx, fire_x, fire_y, fire_x, fire_y);
        }
    }

    MOX::_dont_draw_ship = saved_dont_draw;
}

void __cdecl Wait_Until_Net_Opponent_Finished_(int16_t show_wait_message) {
    int16_t opponent_idx;
    char message_buffer[80];

    opponent_idx = MOX::_combat_attacker_player;
    if (MOX::_PLAYER_NUM == MOX::_combat_attacker_player) {
        opponent_idx = MOX::_combat_defender_player;
    }

    NETMOX::Net_Debug_Log_(
        "tactical barrier send local_player=%d opponent=%d turn=%d cur_ship=%d cur_ptr=%d counter_before=%u choice=%d show=%d",
        (int)MOX::_PLAYER_NUM,
        (int)opponent_idx,
        (int)MOX::_combat_turn,
        MOX::_cur_ship,
        (int)MOX::_cur_ptr,
        (unsigned int)MOX::_combat_done_waiting,
        (int)MOX::_tactical_net_choice,
        (int)show_wait_message);
    NETMOX::Mox_Send_Message_(MOX::_player[opponent_idx].network_player_id, TACTICAL_NET_PACKET_WAIT_FOR_OPPONENT, &opponent_idx, sizeof(opponent_idx));

    while (MOX::_combat_done_waiting == 0) {
        if (show_wait_message == 0) {
            RUSS::Mox_Update_();
        } else {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            fonts::Set_Font_Style_(3, &MOX::_combat_green);
            KEN::Ken_Get_Text_Message_(55, message_buffer, sizeof(message_buffer));
            fonts::Print_(20, 8, message_buffer);
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }
        fields::Get_Input_();
    }

    MOX::_combat_done_waiting -= 1;
    NETMOX::Net_Debug_Log_(
        "tactical barrier complete local_player=%d opponent=%d turn=%d cur_ship=%d cur_ptr=%d counter_after=%u choice=%d",
        (int)MOX::_PLAYER_NUM,
        (int)opponent_idx,
        (int)MOX::_combat_turn,
        MOX::_cur_ship,
        (int)MOX::_cur_ptr,
        (unsigned int)MOX::_combat_done_waiting,
        (int)MOX::_tactical_net_choice);
}

int16_t __cdecl Move_Cost_(int16_t ship_idx, int16_t target_x, int16_t target_y) {
    s_combat_data* ship = &MOX::_combat_data[ship_idx];

    int16_t ship_x = (int16_t)ship->x;
    int16_t ship_y = (int16_t)ship->y;
    int16_t ship_facing = (int16_t)ship->facing_dir;

    if (target_x == ship_x && target_y == ship_y) {
        return 0;
    }

    int16_t dx = target_x - ship_x;
    int16_t dy = target_y - ship_y;
    int16_t dist_sq = dx * dx + dy * dy;

    int16_t dist = 0;
    if (dist_sq != 0) {
        dist = 1;
        while (dist < 81) {
            if ((int32_t)dist_sq < (int32_t)dist * (int32_t)dist + 1) {
                break;
            }
            dist++;
        }
    }

    int16_t target_facing = Get_Facing_(ship_x, ship_y, target_x, target_y);
    int32_t facing_diff = (int32_t)target_facing - (int32_t)ship_facing;

    int32_t abs_diff = abs(facing_diff);

    if (abs_diff > 8) {
        abs_diff = abs(abs_diff - 16);
    }

    int16_t turn_unit_cost = Turn_Cost_(ship_idx);

    return dist + (int16_t)((int32_t)turn_unit_cost * abs_diff);
}

    int16_t __cdecl Move_Ship_(int16_t ship_idx, int16_t dest_x, int16_t dest_y, int16_t move_cost_unused) {
        (void)move_cost_unused;
        int16_t tractor_count = 0;
        int16_t test_move_result = 0;
        int16_t used_teleporter = CMBTFIR2::Used_Teleporter_(ship_idx) != 0;
        int16_t move_cost = 0;
        int16_t defensive_fire_counter = 0;
        int16_t old_missile_warning_gt_0 = MOX::_missile_warning > 0;
        int16_t old_shields_flag = MOX::_shields_flag;

        int16_t dest_x_px = dest_x * 20;
        int16_t dest_y_px = dest_y * 20;

        int16_t tractor_ships[MAX_SHIPS + 1];
        int16_t tractor_target_idx;
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        if (CMBTSPEC::Try_Get_Active_Tractor_Target_(ship_idx, &tractor_target_idx)) {
            tractor_ships[tractor_count++] = ship_idx;
        }

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            const int16_t active_tractor_target_idx = MOX::_combat_data[i].special_cooldown == UINT8_MAX
                ? int16_t{-1}
                : MOX::_combat_data[i].special_cooldown;
            if (active_tractor_target_idx == ship_idx &&
                CMBTSPEC::Try_Get_Active_Tractor_Target_(i, &tractor_target_idx)) {
                tractor_ships[tractor_count++] = i;
            }
        }

        bool has_teleporter = COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_SUB_SPACE_TELEPORTER) != 0 ||
                              COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_PHASE_SHIFTER) != 0;

        bool skip_normal_move = false;

        if (has_teleporter && !used_teleporter) {
            if (CMBTAI::Range_To_XY_(ship_idx, dest_x_px, dest_y_px) < 21) {
                CMBTFIR2::Set_Teleporter_Used_(ship_idx);
                Draw_Teleporting_Ship_(ship_idx, dest_x, dest_y);
                skip_normal_move = true;
            }
        }

        if (!skip_normal_move) {
            int16_t center_x, center_y;
            COMBINIT::Ship_Center_Offsets_(ship_idx, &center_x, &center_y);

            MOX::_shields_flag = 0;

            if (MOX::_combat_grid[dest_x][dest_y] != 2000) {
                return 0;
            }

            int16_t start_x_px = ship->x * 20;
            int16_t start_y_px = ship->y * 20;
            int16_t speed_px;

            if (MOX::_net_flag != 0) {
                speed_px = 6;
            } else {
                if (MOX::_speedx2_flag == 1) {
                    speed_px = 12;
                } else {
                    speed_px = 2;
                }
            }

            int16_t cur_x_px = start_x_px;
            int16_t cur_y_px = start_y_px;
            MOX::_dont_draw_ship = ship_idx;

            if (MOX::_super_fast_flag == 0) {
                CMBTSPEC::Start_Ship_Engine_SFX_();
                video::Set_Page_Off_();
                CMBTDRW1::Full_Draw_Combat_Screen_();
                
                if (MOX::_ship_status_display == 0) {
                    CMBTDRW1::Draw_Weapon_Status_Display_(-1, -1);
                } else {
                    CMBTDRW1::Draw_Special_Status_Display_();
                }

                graphics::Interlaced_Fill_(401, 362, 489, 374, 0, 199);
                graphics::Interlaced_Fill_(401, 379, 488, 467, 0, 199);
                CMBTDRW1::Draw_Internals_Display_(401, 379);

                MOX::_last_ship_status = 666;
                MOX::_draw_active_missiles = 0;
                CMBTDRW1::Draw_Main_Combat_Screen_();
                MOX::_draw_active_missiles = 1;
                fields::Draw_Visible_Fields_();
                video::Copy_Off_To_Back_();
            }

            fields::Disable_Draw_All_Fields_();

            int16_t facing_dir = ship->facing_dir;

            if (MOX::_msl_impact_warning_flag == 1 && MOX::_auto_msl_warning_off == 0 && MOX::_missile_warning != 2) {
                test_move_result = Test_Move_Until_Impact_(ship_idx, &dest_x_px, &dest_y_px, &facing_dir);
                if (test_move_result == 1 || dest_x * 20 != dest_x_px || dest_y * 20 != dest_y_px) {
                    MOX::_missile_warning = 1;
                }
            }

            if (test_move_result != 0) {
                int32_t turns = ((int32_t)facing_dir - (int32_t)ship->facing_dir + 16) & 0x0F;
                move_cost = (int16_t)(turns * Turn_Cost_(ship_idx));
            } else {
                move_cost = Move_Cost_(ship_idx, dest_x_px / 20, dest_y_px / 20);
            }

            if (move_cost > 0) {
                facing_dir = Get_Facing_(ship->x, ship->y, dest_x_px / 20, dest_y_px / 20);
                if (old_missile_warning_gt_0) {
                    MOX::_missile_warning = 0;
                }
            }

            if (ship->facing_dir != facing_dir) {
                Rotate_Ship_(ship_idx, facing_dir, cur_x_px - MOX::_cur_combat_x * 20, cur_y_px - MOX::_cur_combat_y * 20);
            }

            if (ship->unit_status == 0 && test_move_result == 0 && (start_x_px != dest_x_px || start_y_px != dest_y_px)) {
                cur_x_px = start_x_px;
                cur_y_px = start_y_px;
                int16_t hit_missiles[300];

                while (cur_x_px != dest_x_px || cur_y_px != dest_y_px) {
                    if (CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx) != 0) {
                        break;
                    }

                    special::Absolute_Interpolate_Line_(
                        &cur_x_px, &cur_y_px,
                        start_x_px, start_y_px,
                        dest_x_px, dest_y_px,
                        speed_px
                    );

                    int16_t hit_missile_count = 0;
                    facing_dir = Get_Facing_(cur_x_px, cur_y_px, dest_x_px, dest_y_px);

                    for (int16_t m = 0; m < 300; ++m) {
                        s_missile* msl = &MOX::_missile[m];
                        if (msl->type > 0 && msl->target_unit_idx == ship_idx) {
                            int16_t msl_speed = 5;
                            if (ship->current_speed != 0) {
                                msl_speed = ((uint8_t)msl->speed * speed_px) / ship->current_speed;
                            }
                            if (msl_speed < 2) {
                                msl_speed = 2;
                            }
                            // ORACLE: The original passes the full 16-bit distance;
                            // values above 127 must not be truncated to signed int8_t.
                            if (CMBTMIS::Move_Missile_(m, center_x + cur_x_px, center_y + cur_y_px, msl_speed) != 0) {
                                hit_missiles[hit_missile_count++] = m;
                            }
                        }
                    }

                    if (MOX::_super_fast_flag == 0) {
                        graphics::Set_Window_(0, 0, 639, 350);
                        video::Clip_On_();
                        video::Set_Page_Off_();

                        if (tractor_count > 0) {
                            for (int16_t t = 0; t < tractor_count; ++t) {
                                CMBTSPEC::Draw_Moving_Tractor_(tractor_ships[t], cur_x_px, cur_y_px);
                            }
                            CMBTDRW1::Draw_Main_Combat_Screen_();
                        }

                        CMBTDRW1::Draw_Ship_(ship_idx, cur_x_px - MOX::_cur_combat_x * 20, cur_y_px - MOX::_cur_combat_y * 20);

                        for (int16_t m = 0; m < 300; ++m) {
                            s_missile* msl = &MOX::_missile[m];
                            if (msl->type > 0 && msl->target_unit_idx == ship_idx) {
                                CMBTMIS::Draw_Missile_(m, msl->x - MOX::_cur_combat_x * 20, msl->y - MOX::_cur_combat_y * 20, 0);
                            }
                        }

                        RUSS::Mox_Sync_Update_();
                        MOX::_ship_frame++;
                        if (MOX::_ship_frame == 32000) {
                            MOX::_ship_frame = 0;
                        }

                        graphics::Reset_Window_();
                        video::Clip_Off_();
                    }

                    for (int16_t i = 0; i < hit_missile_count; ++i) {
                        if (ship->unit_status == 0) {
                            s_missile* msl = &MOX::_missile[hit_missiles[i]];
                            if (msl->target_unit_idx == ship_idx) {
                                CMBTMIS::Resolve_Missile_(hit_missiles[i], center_x + cur_x_px, center_y + cur_y_px);
                            }
                        }
                    }

                    if (CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx) == 0) {
                        if (ship->special_status_flag != 4) {
                            if (defensive_fire_counter % 3 == 0) {
                                CMBTFIR2::Defensive_Fire_Check_(ship_idx, cur_x_px, cur_y_px, dest_x_px, dest_y_px);
                            }
                        }
                    }
                    defensive_fire_counter++;
                }

            }

            if (MOX::_super_fast_flag == 0) {
                CMBTSPEC::End_Ship_Engine_SFX_();
            }
        }

        ship->x = dest_x_px / 20;
        ship->y = dest_y_px / 20;
        MOX::_dont_draw_ship = -1;
        MOX::_cur_ship = ship_idx;
        MOX::_shields_flag = old_shields_flag;

        if (tractor_count > 0) {
            for (int16_t t = 0; t < tractor_count; ++t) {
                int16_t t_ship_idx = tractor_ships[t];
                int16_t target_ship_idx;
                if (CMBTSPEC::Try_Get_Active_Tractor_Target_(t_ship_idx, &target_ship_idx)) {
                    if (CMBTAI::Range_To_Ship_(t_ship_idx, target_ship_idx) > 4) {
                        MOX::_combat_data[t_ship_idx].special_cooldown = 0xFF;
                    }
                }
            }
        }

        fields::Enable_Draw_All_Fields_();
        return move_cost;
    }

void __cdecl Turn_To_Face_(int16_t ship_idx, int16_t target_x, int16_t target_y) {
    if (MOX::_combat_data[ship_idx].movement_left == 0) {
        return;
    }

    int16_t turn_cost = CMBTMOV1::Turn_Cost_(ship_idx);
    int16_t target_facing = CMBTMOV1::Get_Facing_(
        MOX::_combat_data[ship_idx].x,
        MOX::_combat_data[ship_idx].y,
        target_x,
        target_y
    );

    int16_t facing_dir = MOX::_combat_data[ship_idx].facing_dir;
    int16_t diff = facing_dir - target_facing;

    int16_t sign;
    if (diff < 0) {
        if (abs(diff) <= 8) {
            sign = 1;
        } else {
            sign = -1;
        }
    } else {
        if (diff <= 8) {
            sign = -1;
        } else {
            sign = 1;
        }
    }

    if (diff < 0) {
        diff += 16;
    }
    if (diff > 8) {
        diff = 16 - diff;
    }

    int32_t movement_left = MOX::_combat_data[ship_idx].movement_left;
    int32_t cost = diff * turn_cost;

    if (cost <= movement_left) {
        MOX::_combat_data[ship_idx].movement_left -= (int16_t)cost;
    } else {
        int16_t turns = (turn_cost + movement_left - 1) / turn_cost;
        int16_t new_facing = facing_dir + (turns * sign);
        
        if (new_facing < 0) {
            new_facing += 16;
        }
        
        MOX::_combat_data[ship_idx].movement_left = 0;
        target_facing = new_facing % 16;
    }

    int32_t dx = (MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x) * 20;
    int32_t dy = (MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y) * 20;

    CMBTMOV1::Rotate_Ship_(ship_idx, target_facing, (int16_t)dx, (int16_t)dy);
    COMBAT1::Assign_Combat_Grids_(ship_idx);
    CMBTDRW1::Refresh_Combat_Screen_Full_();
}

bool __cdecl Ship_In_Black_Hole_(int16_t ship_idx) {
    return MOX::_combat_data[ship_idx].black_hole_source_idx != UINT8_MAX;
}

int16_t __cdecl Get_Facing_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    int16_t dx = (int16_t)(x2 - x1);
    int16_t dy = (int16_t)(y2 - y1);
    int16_t angle = angle::Get_Angle_(dx, dy);

    angle = -angle;
    if (angle < 0) {
        angle += 360;
    }

    int16_t scaled = (int16_t)((uint16_t)angle << 2);
    int16_t quotient16 = (int16_t)(((int32_t)scaled + 45) / 90);
    int32_t result = quotient16;
    int32_t sign = result >> 31;

    result = ((result ^ sign) - sign) & 0x0F;
    result = (result ^ sign) - sign;

    return (int16_t)result;
}

    int16_t __cdecl Rotate_Ship_Until_Impact_(int16_t ship_idx, int16_t* target_facing) {
        int16_t impact_detected = 0;
        uint8_t missile_tracked[300];
        int16_t saved_missile_x[300];
        int16_t saved_missile_y[300];
        int16_t saved_missile_travel[300];
        int16_t sx, sy;
        int16_t rotation_step;
        int8_t current_facing;

        memset(missile_tracked, 0, sizeof(missile_tracked));

        s_combat_data* ship = &MOX::_combat_data[ship_idx];
        current_facing = ship->facing_dir;

        COMBINIT::Ship_Center_Offsets_(ship_idx, &sx, &sy);
        sx += (int16_t)ship->x * 20;
        sy += (int16_t)ship->y * 20;

        int16_t diff = (int16_t)current_facing - *target_facing;
        if (diff < 0) {
            rotation_step = ((-diff) > 8) ? -1 : 1;
        } else {
            rotation_step = (diff > 8) ? 1 : -1;
        }

        if ((int16_t)current_facing != *target_facing) {
            while (impact_detected == 0) {
                current_facing = (int8_t)((current_facing + rotation_step + 16) % 16);

                for (int32_t i = 0; i < 300; ++i) {
                    s_missile* missile = &MOX::_missile[i];
                    if (missile->type > 0 && missile->target_unit_idx == ship_idx && missile->source_unit_idx != ship_idx) {
                        int16_t move_speed = 5;
                        if (ship->current_speed != 0) {
                            move_speed = (int16_t)(((int32_t)missile->speed * 5) / (int32_t)ship->current_speed);
                        }
                        if (move_speed < 2) {
                            move_speed = 2;
                        }

                        if (missile_tracked[i] == 0) {
                            saved_missile_x[i] = missile->x;
                            saved_missile_y[i] = missile->y;
                            saved_missile_travel[i] = missile->travel_dist;
                            missile_tracked[i] = 1;
                        }

                        if (CMBTMIS::Move_Missile_(i, sx, sy, move_speed)) {
                            if (CMBTAI::Will_Missile_Do_Damage_(ship_idx, i) != 0 && missile->is_active == 1) {
                                impact_detected = 1;
                            }
                        }
                    }
                }

                if ((int16_t)current_facing == *target_facing) {
                    break;
                }
            }
        }

        if (impact_detected != 0) {
            current_facing = (int8_t)((current_facing - rotation_step + 16) % 16);
            *target_facing = (int16_t)current_facing;
        }

        for (int32_t i = 0; i < 300; ++i) {
            if (missile_tracked[i] == 1) {
                MOX::_missile[i].x = saved_missile_x[i];
                MOX::_missile[i].y = saved_missile_y[i];
                MOX::_missile[i].travel_dist = saved_missile_travel[i];
            }
        }

        return impact_detected;
    }

int16_t __cdecl Test_Move_Until_Impact_(int16_t ship_idx, int16_t *dest_x, int16_t *dest_y, int16_t *dest_facing) {
    int16_t impact_flag = 0;
    int16_t time_spent = 0;
    
    int8_t missile_checked[300];
    int16_t missile_x_saved[300];
    int16_t missile_y_saved[300];
    int16_t missile_travel_dist_saved[300];

    struct_::Clear_Structure_(missile_checked, sizeof(missile_checked));

    int16_t target_x_val = *dest_x;
    int16_t target_y_val = *dest_y;
    
    int16_t center_x, center_y;
    COMBINIT::Ship_Center_Offsets_(ship_idx, &center_x, &center_y);

    s_combat_data *ship = &MOX::_combat_data[ship_idx];
    
    int16_t start_x = ship->x * 20;
    int16_t start_y = ship->y * 20;
    
    int16_t speed;
    if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
        speed = 12;
    } else {
        speed = 2;
    }

    int16_t current_x = start_x;
    int16_t current_y = start_y;
    
    int16_t facing = Get_Facing_(start_x, start_y, target_x_val, target_y_val);
    
    if (ship->facing_dir != facing) {
        time_spent = Rotate_Ship_Until_Impact_(ship_idx, &facing);
        impact_flag = time_spent;
    }
    
    *dest_facing = facing;
    
    int16_t legal_x = current_x;
    int16_t legal_y = current_y;

    if (impact_flag == 0) {
        while (true) {
            if (current_x == target_x_val && current_y == target_y_val) {
                break;
            }

            if (Ship_In_Legal_Square_(MOX::_cur_ship, current_x, current_y, target_x_val, target_y_val)) {
                legal_x = current_x;
                legal_y = current_y;
            }

            special::Absolute_Interpolate_Line_(&current_x, &current_y, start_x, start_y, target_x_val, target_y_val, speed);

            for (int16_t i = 0; i < 300; ++i) {
                s_missile *missile = &MOX::_missile[i];
                if (missile->type > 0 && missile->target_unit_idx == ship_idx && missile->source_unit_idx != ship_idx) {
                    
                    int16_t missile_speed = 5;
                    if (ship->current_speed != 0) {
                        uint8_t m_speed = missile->speed;
                        missile_speed = (m_speed * speed) / ship->current_speed;
                    }
                    if (missile_speed < 2) {
                        missile_speed = 2;
                    }

                    if (missile_checked[i] == 0) {
                        missile_x_saved[i] = missile->x;
                        missile_y_saved[i] = missile->y;
                        missile_travel_dist_saved[i] = missile->travel_dist;
                        missile_checked[i] = 1;
                    }

                    if (CMBTMIS::Move_Missile_(i, current_x + center_x, current_y + center_y, missile_speed)) {
                        if (CMBTAI::Will_Missile_Do_Damage_(ship_idx, i)) {
                            if (missile->is_active == 1) {
                                impact_flag = 1;
                            }
                        }
                    }
                }
            }

            if (impact_flag != 0) {
                break;
            }
        }
    }

    if (impact_flag != 0) {
        if (legal_x == start_x && legal_y == start_y) {
            current_x = legal_x;
            current_y = legal_y;
        } else {
            if (legal_x > target_x_val) {
                current_x = legal_x - (legal_x % 20) + 20;
            } else {
                current_x = legal_x - (legal_x % 20);
            }

            if (legal_y > target_y_val) {
                current_y = legal_y - (legal_y % 20) + 20;
            } else {
                current_y = legal_y - (legal_y % 20);
            }
        }
    }

    *dest_x = current_x;
    *dest_y = current_y;

    for (int16_t i = 0; i < 300; ++i) {
        if (missile_checked[i] == 1) {
            s_missile *missile = &MOX::_missile[i];
            missile->x = missile_x_saved[i];
            missile->y = missile_y_saved[i];
            missile->travel_dist = missile_travel_dist_saved[i];
        }
    }

    return time_spent;
}

void __cdecl Draw_Teleporting_Ship_(int16_t ship_idx, int16_t target_x, int16_t target_y) {
    int16_t off_x, off_y;
    int16_t ship_screen_x, ship_screen_y;
    int16_t target_screen_x, target_screen_y;
    s_animation_header* anim;
    int32_t sound_handle;
    int16_t step;
    int16_t step_10;
    int16_t counter;
    int16_t vanish_out, vanish_in;
    int32_t pitch;

    if (MOX::_super_fast_flag != 0) {
        return;
    }

    if (!CMBTMIS::On_Combat_Screen_(target_x, target_y)) {
        COMBINIT::Snap_Center_Combat_Screen_(target_x, target_y);
    }

    MOX::_dont_draw_ship = ship_idx;
    CMBTDRW1::Get_Draw_Ship_Offsets_(ship_idx, &off_x, &off_y);

    ship_screen_x = (MOX::_combat_data[ship_idx].x - MOX::_cur_combat_x) * 20 + off_x;
    ship_screen_y = (MOX::_combat_data[ship_idx].y - MOX::_cur_combat_y) * 20 + off_y;

    target_screen_x = (target_x - MOX::_cur_combat_x) * 20 + off_x;
    target_screen_y = (target_y - MOX::_cur_combat_y) * 20 + off_y;

    allocate::Push_Block_(MOX::_screen_seg);
    anim = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x2584);
    video::Set_Page_Off_();
    CMBTDRW1::Full_Draw_Combat_Screen_();

    if (MOX::_speedx2_flag == 1 || MOX::_net_flag != 0) {
        step = 2;
    } else {
        step = 1;
    }

    sound_handle = ERICNET::Mox_Play_Sound_(0x37);
    pitch = (2 - (int32_t)MOX::_combat_data[ship_idx].size_class) * 2500;
    sound::Pitch_Bend_Sound_(sound_handle, pitch);

    counter = 0;
    step_10 = step * 10;
    vanish_out = 0;
    vanish_in = 100;

    do {
        CMBTDRW1::Draw_Main_Combat_Screen_();
        graphics::Set_Window_(0, 0, 639, 350);
        video::Clip_On_();

        // Ship vanishing at current location
        CMBTDRW1::Draw_Ship_To_Bitmap_(ship_idx, 0, 0, 0);
        bitmap::Vanish_Bitmap_(anim, MOX::_draw_ship_bitmap, vanish_out);
        animate::Draw_(ship_screen_x, ship_screen_y, anim);

        // Ship appearing at target location
        CMBTDRW1::Draw_Ship_To_Bitmap_(ship_idx, 0, 0, 0);
        bitmap::Vanish_Bitmap_(anim, MOX::_draw_ship_bitmap, vanish_in);
        animate::Draw_(target_screen_x, target_screen_y, anim);

        video::Clip_Off_();
        graphics::Reset_Window_();
        RUSS::Mox_Sync_Update_();

        counter += step;
        vanish_out += step_10;
        vanish_in -= step_10;
    } while (counter < 10);

    video::Clip_Off_();
    allocate::Pop_Block_(MOX::_screen_seg);
    sound::Fadeout_Sound_(sound_handle, 10);
}

int16_t __cdecl Ship_In_Legal_Square_(int16_t ship_idx, int16_t x, int16_t y, int16_t target_x, int16_t target_y) {
    int16_t grid_x;
    int16_t grid_y;

    if (target_x < x) {
        grid_x = (x - (x % 20)) + 20;
    } else {
        grid_x = x - (x % 20);
    }

    if (target_y < y) {
        grid_y = (y - (y % 20)) + 20;
    } else {
        grid_y = y - (y % 20);
    }

    int16_t cell_x = grid_x / 20;
    int16_t cell_y = grid_y / 20;

    int16_t cell_val = MOX::_combat_grid[cell_x][cell_y];

    if (cell_val == 2000 || cell_val == ship_idx) {
        return 1;
    }

    return 0;
}

void __cdecl Tactical_Combat_Receive_Message_(int32_t sender_net_id, uint16_t message_id, int32_t message_size, s_net_tactical_combat_message* message) {
    (void)sender_net_id;
    (void)message_size;

    if (message_id >= TACTICAL_NET_PACKET_ACTION && message_id < DIP_NET_PACKET_MESSAGE) {
        switch (message_id) {
            case TACTICAL_NET_PACKET_ACTION:
                NETMOX::Net_Debug_Log_(
                    "tactical action recv sender_net_id=%d local_player=%d action=%d args=%d,%d,%d pending=%d choice=%d turn=%d cur_ship=%d",
                    (int)sender_net_id,
                    (int)MOX::_PLAYER_NUM,
                    (int)message->message_code,
                    (int)message->arg1,
                    (int)message->arg2,
                    (int)message->arg3,
                    (int)MOX::_new_message.message_code,
                    (int)MOX::_tactical_net_choice,
                    (int)MOX::_combat_turn,
                    MOX::_cur_ship);
                if (MOX::_new_message.message_code == 0) {
                    struct_::Copy_Structure_(&MOX::_new_message, message, sizeof(MOX::_new_message));
                    MOX::_tactical_net_choice = MOX::_new_message.message_code;
                    return;
                }

                NETMOX::Net_Debug_Log_(
                    "Tactical_Combat_Receive_Message_: overwriting pending action pending=%d incoming=%d choice=%d",
                    (int)MOX::_new_message.message_code,
                    (int)message->message_code,
                    (int)MOX::_tactical_net_choice);
                struct_::Copy_Structure_(&MOX::_new_message, message, sizeof(MOX::_new_message));
                MOX::_tactical_net_choice = MOX::_new_message.message_code;
                return;

            case TACTICAL_NET_PACKET_AUTO_COMBAT_DISABLED: {
                
                int16_t player_idx = message->viewport_x;
                MOX::_auto_combat_flag[player_idx] = 0;

                if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_attacker_player) {
                    if (player_idx == 0) {
                        MOX::_auto_combat = 0;
                        return;
                    }
                } else {
                    if (player_idx == 1) {
                        MOX::_auto_combat = 0;
                        return;
                    }
                }
                break;
            }

            case TACTICAL_NET_PACKET_MISSILE_WARNING:
                MOX::_saved_msl_warn = MOX::_msl_impact_warning_flag;
                MOX::_msl_impact_warning_flag = message->viewport_x;
                return;

            case TACTICAL_NET_PACKET_WAIT_FOR_OPPONENT:
                NETMOX::Net_Debug_Log_(
                    "tactical barrier recv sender_net_id=%d local_player=%d turn=%d cur_ship=%d cur_ptr=%d counter_before=%u choice=%d",
                    (int)sender_net_id,
                    (int)MOX::_PLAYER_NUM,
                    (int)MOX::_combat_turn,
                    MOX::_cur_ship,
                    (int)MOX::_cur_ptr,
                    (unsigned int)MOX::_combat_done_waiting,
                    (int)MOX::_tactical_net_choice);
                MOX::_combat_done_waiting++;
                NETMOX::Net_Debug_Log_(
                    "tactical barrier queued local_player=%d counter_after=%u",
                    (int)MOX::_PLAYER_NUM,
                    (unsigned int)MOX::_combat_done_waiting);
                return;

            case TACTICAL_NET_PACKET_INIT_SYNC:
                if (message->viewport_x == 1) {
                    MOX::_combat_init = message->viewport_x;
                }
                break;
        }
    }
}

}