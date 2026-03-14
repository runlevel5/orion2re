#include "pch.h"
#include <cstdlib>

namespace CMBTMOV1 {
    bool __cdecl Ship_Is_In_Stasis_(int16_t ship_idx) {
        return MOX::_combat_data[ship_idx].stasis_source_idx != 0xFF;
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

                if (CMBTMIS::Move_Missile_(i, center_x, center_y, (int8_t)move_speed) != 0) {
                    moved_missiles[moved_count++] = i;
                }
            }
        }

        if (MOX::_super_fast_flag == 0 && CMBTDRW1::Ship_On_Screen_(ship_idx) != 0) {
            video::Set_Page_Off_();
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].unit_status == 0 && MOX::_combat_data[i].special_cooldown != -1) {
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

void __cdecl Wait_Until_Net_Opponent_Finished_(int16_t show_message) {
    int16_t opponent_idx;
    char message_buffer[80];

    opponent_idx = MOX::_combat_attacker_player;
    if (MOX::_PLAYER_NUM == MOX::_combat_attacker_player) {
        opponent_idx = MOX::_combat_defender_player;
    }

    NETMOX::Mox_Send_Message_(MOX::_player[opponent_idx].network_player_id, 1004 /* 0x3ec */, &opponent_idx, 2);

    while (MOX::_combat_done_waiting == 0) {
        if (show_message == 0) {
            RUSS::Mox_Update_();
        } else {
            timer::Mark_Time_();
            video::Set_Page_Off_();
            CMBTDRW1::Draw_Main_Combat_Screen_();
            fonts::Set_Font_Style_(3, &MOX::_combat_green);
            KEN::Ken_Get_Text_Message_(55 /* 0x37 */, message_buffer, 80 /* 0x50 */);
            fonts::Print_(20 /* 0x14 */, 8, message_buffer);
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);
        }
        fields::Get_Input_();
    }

    MOX::_combat_done_waiting -= 1;
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

    int16_t __cdecl Move_Ship_(int16_t ship_idx, int16_t dest_x, int16_t dest_y) {
        int16_t tractor_count = 0;
        int16_t test_move_result = 0;
        int16_t used_teleporter = CMBTFIR2::Used_Teleporter_(ship_idx) != 0;
        int16_t move_cost = 0;
        int16_t defensive_fire_counter = 0;
        int16_t old_missile_warning_gt_0 = MOX::_missile_warning > 0;
        int16_t old_shields_flag = MOX::_shields_flag;

        int16_t dest_x_px = dest_x * 20;
        int16_t dest_y_px = dest_y * 20;

        bool has_teleporter = COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x21 /* SPEC_SUB_SPACE_TELEPORTER */) != 0 ||
                              COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x16 /* SPEC_TRANSPORTER */) != 0;

        bool skip_normal_move = false;

        if (has_teleporter && !used_teleporter) {
            if (CMBTAI::Range_To_XY_(ship_idx, dest_x_px, dest_y_px) < 21) {
                CMBTFIR2::Set_Teleporter_Used_(ship_idx);
                Draw_Teleporting_Ship_(ship_idx, dest_x, dest_y);
                skip_normal_move = true;
            }
        }

        int16_t tractor_ships[10];
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        if (!skip_normal_move) {
            int16_t center_x, center_y;
            COMBINIT::Ship_Center_Offsets_(ship_idx, &center_x, &center_y);

            if (ship->special_cooldown != -1) {
                tractor_ships[tractor_count++] = ship_idx;
            }

            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].special_cooldown == ship_idx && MOX::_combat_data[i].unit_status == 0) {
                    tractor_ships[tractor_count++] = i;
                }
            }

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

                MOX::_last_ship_status = 666; /* 0x29A */
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
                int32_t diff = facing_dir - ship->facing_dir + 16;
                int32_t turns = diff % 16;
                move_cost = turns * Turn_Cost_(ship_idx);
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
                    facing_dir &= 0xFFFF0000;
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
                            if (CMBTMIS::Move_Missile_(m, center_x + cur_x_px, center_y + cur_y_px, (int8_t)msl_speed) != 0) {
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

                if (MOX::_super_fast_flag == 0) {
                    CMBTSPEC::End_Ship_Engine_SFX_();
                }
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
                s_combat_data* t_ship = &MOX::_combat_data[t_ship_idx];
                if (t_ship->unit_status == 0) {
                    if (CMBTAI::Range_To_Ship_(t_ship_idx, (uint8_t)t_ship->special_cooldown) > 4) {
                        t_ship->special_cooldown = -1;
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
        MOX::_combat_data[ship_idx].movement_left -= static_cast<int16_t>(cost);
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

    CMBTMOV1::Rotate_Ship_(ship_idx, target_facing, static_cast<int16_t>(dx), static_cast<int16_t>(dy));
    COMBAT1::Assign_Combat_Grids_(ship_idx);
    CMBTDRW1::Refresh_Combat_Screen_Full_();
}

}