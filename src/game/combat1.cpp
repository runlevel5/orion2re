#include "pch.h"

#include "game/boarding.h"

namespace COMBAT1 {
    static constexpr int kMaxCombatShips = 210;
    static int16_t movement_reserve[kMaxCombatShips] = {};

    static int16_t Maximum_Combat_Speed_(int16_t ship_idx) {
        s_combat_data& ship = MOX::_combat_data[ship_idx];
        int16_t speed = (uint8_t)ship.combat_speed_base;

        if (struct_::Test_Bit_Field_(
                ship.special_device_flags, SPECIAL_AUGMENTED_ENGINES) != 0) {
            speed += 5;
        }

        if (ship.owner >= 0 && ship.owner < MAX_PLAYERS &&
            MOX::_player[ship.owner].traits[TRAIT_TRANS_DIMENSIONAL] != 0) {
            speed += 4;
        }

        return speed;
    }

    int32_t _end_of_turn_button;

    uint16_t _start_of_turn;

    void Recalculate_Combat_Ship_(int16_t ship_idx, bool tractor_applied) {
        if (ship_idx < 0 || ship_idx >= MOX::_combat_total_ship_count) {
            return;
        }

        s_combat_data& ship = MOX::_combat_data[ship_idx];
        const int16_t old_speed = ship.current_speed;
        const int16_t new_speed = Calc_Current_Speed_(ship_idx);
        ship.current_speed = new_speed;

        if (_interactive_combat_stats != 0) {
            ship.movement_left = Adjust_Movement_After_Speed_Change_(
                old_speed,
                new_speed,
                Maximum_Combat_Speed_(ship_idx),
                ship.movement_left,
                &movement_reserve[ship_idx]
            );
            ship.tac_val_attack = CMBTFIR2::Offensive_Combat_Bonus_(ship_idx);
            ship.tac_val_defense = CMBTFIR2::Defensive_Combat_Bonus_(ship_idx, 0);
            ship.tac_val_evade = CMBTFIR2::Defensive_Combat_Bonus_(ship_idx, 1);
        } else if (_non_ics_tractor_and_bhg_mode != 0 &&
                   ship.movement_left > new_speed &&
                   (tractor_applied || ship.black_hole_source_idx != UINT8_MAX)) {
            ship.movement_left = new_speed;
        }
    }

    void __cdecl Add_Tactical_Combat_Fields_(void) {
        int16_t auto_combat_idx = 0;
        const char* info_hotkey;
        int16_t info_y_coord;

        if (MOX::_cur_ship > -1) {
            if (MOX::_combat_data[MOX::_cur_ship].owner != MOX::_combat_attacker_player) {
                auto_combat_idx = 1;
            }
        }

        Set_Combat_Help_Fields_();

        if (MOX::_auto_combat_flag[auto_combat_idx] == 1 || 
            (MOX::_game_type != 1 && MOX::_cur_ship > -1 && 
             MOX::_combat_data[MOX::_cur_ship].owner != MOX::_PLAYER_NUM && 
             MOX::_take_control_flag == 0)) {
            
            for (int i = 0; i < 300; ++i) {
                MOX::_missile_field[i] = -1000;
            }
            MOX::_take_control_button = -1000;
            MOX::_space_field = -1000;
            MOX::_map_field = -1000;
            MOX::_msl_warning_button = -1000;
            MOX::_speedx2_button = -100;
            MOX::_draw_legal_boxes_button = -100;
            MOX::_msl_impact_warning_button = -100;
            MOX::_self_destruct_button = -100;
            MOX::_shields_button = -100;
            MOX::_grid_button = -100;
            MOX::_scan_button = -100;
            COMBAT1::_end_of_turn_button = -100;
            MOX::_board_button = -100;
            MOX::_flee_button = -100;
            MOX::_wait_button = -100;
            MOX::_done_button = -100;
            for (int i = 0; i < 8; ++i) {
                MOX::_weapon_button[i] = -1000;
            }
            MOX::_center_button = -1000;
            MOX::_weapons_button = -1000;
            MOX::_specials_button = -1000;
            MOX::_cmbt_info_button = -1000;
            MOX::_super_fast_button = -1000;
            return;
        }

        fields::Clear_Fields_();

        if (MOX::_order_info_flag == 1) {
            Add_Missile_Fields_();
            MOX::_take_control_button = fields::Add_Hot_Key_("[");
            MOX::_space_field = fields::Add_Grid_Field_(0, 0, 10, 10, 64, 36, &MOX::_xpos, &MOX::_ypos, 0);
            
            if (MOX::_missile_warning == 1) {
                MOX::_msl_warning_button = fields::Add_Hidden_Field_(505, 370, 622, 464, "", 0);
                MOX::_map_field = -1000;
            } else {
                MOX::_map_field = fields::Add_Grid_Field_(500, 364, 1, 1, 121, 102, &MOX::_map_x, &MOX::_map_y, 0);
                MOX::_msl_warning_button = -1000;
            }

            MOX::_weapons_button = fields::Add_Multi_Button_Field_(114, 361, "", (s_animation_header*)MOX::_weapons_button_seg, &MOX::_ship_status_display, 0, "", 41);
            MOX::_specials_button = fields::Add_Multi_Button_Field_(187, 361, "", (s_animation_header*)MOX::_specials_button_seg, &MOX::_ship_status_display, 1, "", 41);
            MOX::_auto_button = fields::Add_Radio_Button_Field_(273, 364, "", (s_animation_header*)MOX::_auto_button_seg, &MOX::_auto_cmbt_button_value, "a", 40);
            MOX::_scan_button = fields::Add_Radio_Button_Field_(337, 364, "", (s_animation_header*)MOX::_scan_button_seg, &MOX::_skan_flag, "s", 40);
            MOX::_board_button = fields::Add_Radio_Button_Field_(273, 392, "", (s_animation_header*)MOX::_board_button_seg, &MOX::_board_flag, "b", 40);
            MOX::_flee_button = fields::Add_Button_Field_(337, 392, "", (s_animation_header*)MOX::_flee_button_seg, "r", 40);
            MOX::_wait_button = fields::Add_Button_Field_(273, 420, "", (s_animation_header*)MOX::_wait_button_seg, "w", 40);
            MOX::_done_button = fields::Add_Button_Field_(337, 420, "", (s_animation_header*)MOX::_cmbt_done_button_seg, "d", 40);
            COMBAT1::_end_of_turn_button = (int)fields::Add_Hot_Key_("t");

            MOX::_draw_legal_boxes_button = -100;
            MOX::_msl_impact_warning_button = -100;
            MOX::_speedx2_button = -100;
            MOX::_shields_button = -100;
            MOX::_grid_button = -100;
            MOX::_self_destruct_button = -100;

            if (MOX::_combat_attacker_player < MAX_PLAYERS && MOX::_player[MOX::_combat_attacker_player].objectives == PLAYER_OBJECTIVE_HUMAN &&
                MOX::_combat_defender_player < MAX_PLAYERS && MOX::_player[MOX::_combat_defender_player].objectives == PLAYER_OBJECTIVE_HUMAN) {
                MOX::_super_fast_button = -1000;
            } else {
                MOX::_super_fast_button = fields::Add_Hot_Key_("z");
            }

            info_hotkey = "O";
            info_y_coord = 448;
        } else {
            for (int i = 0; i < 300; ++i) {
                MOX::_missile_field[i] = -1000;
            }
            MOX::_take_control_button = -100;
            MOX::_space_field = -100;
            MOX::_map_field = fields::Add_Grid_Field_(500, 364, 1, 1, 121, 102, &MOX::_map_x, &MOX::_map_y, 0);
            MOX::_msl_warning_button = -100;
            MOX::_weapons_button = -100;
            MOX::_specials_button = -100;
            MOX::_msl_impact_warning_button = fields::Add_Hidden_Field_(264, 383, 407, 396, "", 41);
            MOX::_speedx2_button = fields::Add_Hidden_Field_(264, 399, 407, 412, "", 41);
            MOX::_draw_legal_boxes_button = fields::Add_Hidden_Field_(264, 415, 407, 428, "", 41);
            MOX::_shields_button = fields::Add_Hidden_Field_(264, 431, 407, 444, "", 41);
            MOX::_grid_button = fields::Add_Hidden_Field_(264, 447, 407, 460, "", 41);
            MOX::_self_destruct_button = fields::Add_Button_Field_(283, 348, "", (s_animation_header*)MOX::_self_destruct_seg, "", 40);
            MOX::_auto_button = -100;
            MOX::_scan_button = -100;
            COMBAT1::_end_of_turn_button = -100;
            MOX::_super_fast_button = -1000;
            MOX::_board_button = -100;
            MOX::_flee_button = -100;
            MOX::_wait_button = -100;
            MOX::_done_button = -100;
            info_hotkey = "\x1B";
            info_y_coord = 301;
        }

        MOX::_cmbt_info_button = fields::Add_Button_Field_(298, info_y_coord, "", (s_animation_header*)MOX::_cmbt_info_button_seg, info_hotkey, 40);

        int16_t weapon_y = 389;
        for (int i = 0; i < 8; ++i) {
            MOX::_weapon_button[i] = fields::Add_Hidden_Field_(120, weapon_y - 10, 255, weapon_y, "", 41);
            weapon_y += 11;
        }

        MOX::_center_button = fields::Add_Hidden_Field_(15, 379, 100, 460, "c", 41);
    }

    int32_t __cdecl Send_Tactical_Combat_Message_(int16_t player_idx, int16_t message_code, int16_t arg1, int16_t arg2, int16_t arg3) {
        s_net_tactical_combat_message combat_msg;

        combat_msg.viewport_x = MOX::_cur_combat_x;
        combat_msg.viewport_y = MOX::_cur_combat_y;
        combat_msg.random_seed = game_random::Get_Random_Seed_();
        combat_msg.message_code = message_code;
        combat_msg.arg1 = arg1;
        combat_msg.arg2 = arg2;
        combat_msg.arg3 = arg3;

        int16_t net_id = MOX::_player[player_idx].network_player_id;

        NETMOX::Net_Debug_Log_(
            "tactical action send local_player=%d target_player=%d target_net_id=%d action=%d args=%d,%d,%d turn=%d cur_ship=%d cur_ptr=%d",
            (int)MOX::_PLAYER_NUM,
            (int)player_idx,
            (int)net_id,
            (int)message_code,
            arg1,
            arg2,
            arg3,
            (int)MOX::_combat_turn,
            MOX::_cur_ship,
            (int)MOX::_cur_ptr);
        return NETMOX::Mox_Send_Message_(net_id, TACTICAL_NET_PACKET_ACTION, &combat_msg, sizeof(combat_msg));
    }

    int16_t __cdecl Ship_Controlled_By_Computer_(int16_t ship_idx) {
        int16_t is_computer = 0;
        int8_t owner = MOX::_combat_data[ship_idx].owner;

        if (((owner >= MAX_PLAYERS) || (MOX::_player[owner].objectives != PLAYER_OBJECTIVE_HUMAN) || (MOX::_auto_combat == 1)) &&
            (MOX::_take_control_flag == 0)) {
            is_computer = 1;
        }

        return is_computer;
    }

    int16_t __cdecl Combat_Ship_Is_Functional_(int16_t ship_idx) {
        int16_t is_functional = 0;

        if (MOX::_combat_data[ship_idx].unit_status == 0 &&
            MOX::_combat_data[ship_idx].is_captured == 0) {

            if (!CMBTMOV1::Ship_Is_In_Stasis_(ship_idx)) {
                is_functional = 1;
            }
        }

        return is_functional;
    }

    static void Process_Retreating_Ships_(int16_t active_player_id, int16_t time_warp_only) {
        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            if (Combat_Ship_Is_Functional_(i) == 0) {
                continue;
            }
            if (time_warp_only != 0 &&
                COMBINIT::Does_Combat_Ship_Have_Special_(i, SPECIAL_TIME_WARP_FACILITATOR) == 0) {
                continue;
            }

            s_combat_data* ship = &MOX::_combat_data[i];
            if ((MOX::_combat_init == 0 && ship->owner != active_player_id) ||
                ship->is_retreating != 1) {
                continue;
            }

            if (ship->current_speed <= 0 || Enemy_Dissipator_Exists_(ship->owner) != 0) {
                ship->is_retreating = 0;
                continue;
            }

            if (MOX::_net_flag != 0 && MOX::_combat_init != 0) {
                NETMOX::Net_Debug_Log_(
                    "Wait_Until_Net_Opponent_Finished_: caller=%s:%d",
                    __FILE__,
                    __LINE__);
                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
            }

            MOX::_cur_ship = i;
            Retreat_Ship_(i);
        }
    }

    static void Advance_After_Ship_Action_(
        int8_t original_owner,
        int16_t active_player_id,
        int16_t* next_ship_flag
    ) {
        const bool owner_changed =
            MOX::_combat_data[MOX::_cur_ship].owner != original_owner;
        const bool ship_done = Ship_Done_Check_() != 0;
        if (!owner_changed && !ship_done) {
            return;
        }

        if (!owner_changed) {
            MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
        }
        *next_ship_flag = Next_Ship_(active_player_id);
    }

    void __cdecl Do_Combat_Turn_(int16_t active_player_id, int16_t time_warp_only) {
        MOX::_player_mode[MOX::_combat_attacker_player] = CMBTAI::Get_Player_Mode_(MOX::_combat_attacker_player);
        MOX::_player_mode[MOX::_combat_defender_player] = CMBTAI::Get_Player_Mode_(MOX::_combat_defender_player);

        for (int16_t i = 0; i < 15; ++i) {
            MOX::_side_end_of_turn[i] = 0;
        }

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (Combat_Ship_Is_Functional_(i) != 0) {
                    if (time_warp_only == 0 || COMBINIT::Does_Combat_Ship_Have_Special_(i, SPECIAL_TIME_WARP_FACILITATOR) != 0) {
                        if (MOX::_combat_init != 0 || MOX::_combat_data[i].owner == active_player_id) {
                            Init_Ship_For_Start_Of_Turn_(i);
                        }
                    }
                }
            }
        }

        Process_Retreating_Ships_(active_player_id, time_warp_only);
        RUSS::Mox_Update_();
        
        int16_t order_idx = 0;
        int16_t num_captured = 0;

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].unit_status == 0) {
                    if (CMBTMOV1::Ship_Is_In_Stasis_(i) == 0 || MOX::_combat_init != 0) {
                        if (time_warp_only == 0 || COMBINIT::Does_Combat_Ship_Have_Special_(i, SPECIAL_TIME_WARP_FACILITATOR) != 0) {
                            if (MOX::_combat_init != 0 || MOX::_combat_data[i].owner == active_player_id) {
                                if (MOX::_combat_data[i].is_captured == 0) {
                                    MOX::_ship_turn_order[order_idx] = i;
                                } else {
                                    MOX::_ship_turn_order[order_idx] = -1;
                                    ++num_captured;
                                }
                                ++order_idx;
                            }
                        }
                    }
                }
            }
        }

        if (order_idx == num_captured) {
            return;
        }

        if (order_idx != num_captured) {
            if (MOX::_combat_init != 0) {
                qsort(MOX::_ship_turn_order, order_idx, sizeof(MOX::_ship_turn_order[0]), qsort_ship_initiative_);
            }
            MOX::_ship_turn_order[order_idx++] = -10;
            while (order_idx < 210) {
                MOX::_ship_turn_order[order_idx++] = -1;
            }

            RUSS::Mox_Update_();
            COMBAT1::_start_of_turn = 1;
            MOX::_cur_ptr = -1;
            int16_t next_ship_flag = Next_Ship_(active_player_id);

            if (next_ship_flag == 1) {
                return;
            }

            if (next_ship_flag != 1) {
                fields::Assign_Auto_Function_(CMBTDRW1::Draw_Main_Combat_Screen_, 2);
                MOX::_skan_flag = 0;
                MOX::_board_flag = 0;
                RUSS::Mox_Update_();

                if (MOX::_combat_total_ship_count > 0) {
                    for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
                        if (MOX::_combat_data[i].ftl_type == 0 && MOX::_combat_data[i].unit_status == 0) {
                            if (CMBTMOV1::Ship_Is_In_Stasis_(i) == 0) {
                                CMBTMOV1::Rotate_Ship_(i, (MOX::_combat_data[i].facing_dir + 2) & 0xf, (MOX::_combat_data[i].x - MOX::_cur_combat_x) * 20, (MOX::_combat_data[i].y - MOX::_cur_combat_y) * 20);
                            }
                        }
                    }
                }

                COMBAT1::_start_of_turn = 0;

                while (MOX::_ship_turn_order[MOX::_cur_ptr] >= 0) {
                    if (next_ship_flag != 0) break;

                    timer::Mark_Time_();
                    MOX::_FULL_DRAW_SCREEN_FLAG = 0;
                    int16_t m_left = MOX::_combat_data[MOX::_cur_ship].movement_left;

                    if (Ship_Controlled_By_Computer_(MOX::_cur_ship) != 0 && 
                        (MOX::_net_flag == 0 || MOX::_combat_data[MOX::_cur_ship].owner == MOX::_PLAYER_NUM)) {
                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_AUTO_TURN, 0, 0, 0);
                        }
                        CMBTAI::Do_Auto_Ship_Turn_();
                        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                            Add_Tactical_Combat_Fields_();
                            Assign_Combat_Grids_(MOX::_cur_ship);
                            CMBTDRW1::Refresh_Combat_Screen_Full_();
                        }
                        MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                        next_ship_flag = Next_Ship_(active_player_id);
                        if (MOX::_net_flag != 0) {
                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                        }
                        timer::Release_Time_(1);
                        continue;
                    } else {
                        MOX::_cur_ship = MOX::_ship_turn_order[MOX::_cur_ptr];
                        if (CMBTMOV1::Ship_Is_In_Stasis_(MOX::_cur_ship) == 0) {
                            if (MOX::_combat_init != 0 || MOX::_combat_data[MOX::_cur_ship].owner == active_player_id) {
                                if (MOX::_net_flag == 0 || MOX::_combat_data[MOX::_cur_ship].owner == MOX::_PLAYER_NUM) {
                                    int16_t input_val = 0;
                                    if (CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship) != 0) {
                                        input_val = MOX::_done_button;
                                    } else {
                                        input_val = fields::Get_Input_();
                                    }

                                    for (int16_t m_idx = 0; m_idx < 300; ++m_idx) {
                                        if (MOX::_missile_field[m_idx] == input_val) {
                                            if (CMBTMIS::Missile_Owner_(m_idx) != MOX::_combat_data[MOX::_cur_ship].owner) {
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_FIRE_WEAPON, m_idx, 1, 0);
                                                }
                                                CMBTFIRE::Fire_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, m_idx, MOX::_missile[m_idx].x, MOX::_missile[m_idx].y, 1);
                                                Add_Tactical_Combat_Fields_();
                                                Assign_Combat_Grids_(MOX::_cur_ship);
                                                for (int16_t i = 0; i < 8; ++i) {
                                                    if (MOX::_combat_data[MOX::_cur_ship].weapons[i].active == 0) {
                                                        MOX::_combat_data[MOX::_cur_ship].weapons[i].active = 1;
                                                    }
                                                }
                                                if (MOX::_net_flag != 0) {
                                                    NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                                    CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                }
                                            }
                                            input_val = 0;
                                            break;
                                        }
                                    }

                                    if (input_val == MOX::_msl_impact_warning_button) {
                                        MOX::_msl_impact_warning_flag = 1 - MOX::_msl_impact_warning_flag;
                                        MOX::_saved_msl_warning = MOX::_msl_impact_warning_flag;
                                        if (MOX::_net_flag != 0) {
                                            NETMOX::Mox_Send_Message_(MOX::_player[MOX::_net_destination].network_player_id, TACTICAL_NET_PACKET_MISSILE_WARNING, &MOX::_msl_impact_warning_flag, sizeof(MOX::_msl_impact_warning_flag));
                                        }
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_speedx2_button) {
                                        MOX::_speedx2_flag = 1 - MOX::_speedx2_flag;
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_grid_button) {
                                        MOX::_grid_flag = 1 - MOX::_grid_flag;
                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_draw_legal_boxes_button) {
                                        MOX::_draw_legal_boxes_flag = 1 - MOX::_draw_legal_boxes_flag;
                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_shields_button) {
                                        MOX::_shields_flag = 1 - MOX::_shields_flag;
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_super_fast_button) {
                                        MOX::_super_fast_flag = 1;
                                        if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_attacker_player) {
                                            MOX::_auto_combat_flag[0] = 1 - MOX::_auto_combat_flag[0];
                                            MOX::_auto_combat = MOX::_auto_combat_flag[0];
                                        } else {
                                            MOX::_auto_combat_flag[1] = 1 - MOX::_auto_combat_flag[1];
                                            MOX::_auto_combat = MOX::_auto_combat_flag[1];
                                        }
                                        CMBTDRW1::Draw_Super_Fast_Screen_();
                                    }

                                    if (input_val == MOX::_self_destruct_button) {
                                        char buf[80];
                                        if (MOX::_cur_ship == 0) {
                                            KEN::Ken_Get_Text_Message_(56, buf, sizeof(buf));
                                            palette::Refresh_Palette_();
                                            GENDRAW::Message_Box_(buf);
                                        } else {
                                            KEN::Ken_Get_Text_Message_(57, buf, sizeof(buf));
                                            palette::Refresh_Palette_();
                                            if (GENDRAW::Do_Confirmation_Box_(buf) != 0) {
                                                MOX::_order_info_flag = 1 - MOX::_order_info_flag;
                                                CMBTDRW1::Move_Options_Background_();
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_SELF_DESTRUCT, 0, 0, 0);
                                                }
                                                MOX::_combat_data[MOX::_cur_ship].drive_current_hits = 0;
                                                int8_t saved_ftl_type = MOX::_combat_data[MOX::_cur_ship].ftl_type;
                                                MOX::_combat_data[MOX::_cur_ship].ftl_type = 1;
                                                CMBTFIRE::Destroy_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship));
                                                MOX::_combat_data[MOX::_cur_ship].ftl_type = saved_ftl_type;
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_flag = Next_Ship_(active_player_id);
                                                if (MOX::_net_flag != 0) {
                                                    NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                                    CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                }
                                            }
                                        }
                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_msl_warning_button) {
                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_CYCLE_MISSILE_WARNING, 0, 0, 0);
                                        }
                                        MOX::_missile_warning = (MOX::_missile_warning + 1) % 3;
                                        if (MOX::_net_flag != 0) {
                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        Add_Tactical_Combat_Fields_();
                                        Assign_Combat_Grids_(MOX::_cur_ship);
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_auto_button) {
                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_TOGGLE_AUTO_COMBAT, 0, 0, 0);
                                        }
                                        if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_attacker_player) {
                                            MOX::_auto_combat_flag[0] = 1 - MOX::_auto_combat_flag[0];
                                            MOX::_auto_combat = MOX::_auto_combat_flag[0];
                                        } else {
                                            MOX::_auto_combat_flag[1] = 1 - MOX::_auto_combat_flag[1];
                                            MOX::_auto_combat = MOX::_auto_combat_flag[1];
                                        }
                                        if (MOX::_auto_combat == 0) {
                                            MOX::_super_fast_flag = 0;
                                        }
                                        fields::Clear_Fields_();
                                        MOX::_auto_button = fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0);
                                        if (MOX::_net_flag != 0) {
                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_weapons_button) {
                                        MOX::_last_ship_status = -1;
                                        Set_Combat_Help_Fields_();
                                    }

                                    if (input_val == MOX::_specials_button) {
                                        MOX::_last_ship_status = -1;
                                        Set_Combat_Help_Fields_();
                                    }

                                    if (input_val == MOX::_wait_button) {
                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_WAIT, 0, 0, 0);
                                        }
                                        next_ship_flag = Next_Ship_(active_player_id);
                                        if (MOX::_net_flag != 0) {
                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        input_val = 0;
                                    }

                                    for (int16_t w_idx = 0; w_idx < 8; ++w_idx) {
                                        if (MOX::_weapon_button[w_idx] == input_val) {
                                            if (MOX::_ship_status_display == 0) {
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_TOGGLE_WEAPON, w_idx, 0, 0);
                                                }
                                                MOX::_combat_data[MOX::_cur_ship].weapons[w_idx].active -= 1;
                                                if (MOX::_combat_data[MOX::_cur_ship].weapons[w_idx].active < -1) {
                                                    MOX::_combat_data[MOX::_cur_ship].weapons[w_idx].active = 1;
                                                }
                                                if (MOX::_net_flag != 0) {
                                                    NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                                    CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                }
                                                MOX::_last_ship_status = -1;
                                            }
                                            input_val = 0;
                                            break;
                                        }
                                    }

                                    if (input_val == MOX::_cmbt_info_button) {
                                        MOX::_order_info_flag = 1 - MOX::_order_info_flag;
                                        CMBTDRW1::Move_Options_Background_();
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_scan_button) {
                                        MOX::_board_flag = 0;
                                        Assign_Combat_Grids_(MOX::_cur_ship);
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_flee_button) {
                                        if (MOX::_combat_data[MOX::_cur_ship].is_retreating == 0) {
                                            int16_t msg_id = 0;
                                            if (Enemy_Dissipator_Exists_(MOX::_combat_data[MOX::_cur_ship].owner) == 0) {
                                                if (MOX::_combat_data[MOX::_cur_ship].ftl_type == 0) {
                                                    msg_id = 59;
                                                } else if (MOX::_combat_data[MOX::_cur_ship].current_speed < 1) {
                                                    msg_id = 79;
                                                }
                                            } else {
                                                msg_id = 58;
                                            }
                                            if (msg_id != 0) {
                                                char buf[80];
                                                KEN::Ken_Get_Text_Message_(msg_id, buf, sizeof(buf));
                                                palette::Refresh_Palette_();
                                                GENDRAW::Message_Box_(buf);
                                                CMBTDRW1::Refresh_Combat_Screen_Full_();
                                            } else {
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_RETREAT, 0, 0, 0);
                                                }
                                                MOX::_combat_data[MOX::_cur_ship].is_retreating = 1;
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_flag = Next_Ship_(active_player_id);
                                                if (MOX::_net_flag != 0) {
                                                    NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                                    CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                }
                                            }
                                        }
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_board_button) {
                                        MOX::_skan_flag = 0;
                                        if (CMBTFIR2::Already_Boarded_(MOX::_cur_ship) != 0) {
                                            char buf[80];
                                            KEN::Ken_Get_Text_Message_(60, buf, sizeof(buf));
                                            palette::Refresh_Palette_();
                                            GENDRAW::Message_Box_(buf);
                                            CMBTDRW1::Refresh_Combat_Screen_Full_();
                                            MOX::_board_flag = 0;
                                        } else {
                                            Add_Tactical_Combat_Fields_();
                                            Assign_Combat_Grids_(MOX::_cur_ship);
                                        }
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_map_field) {
                                        COMBINIT::Snap_Center_Combat_Screen_((MOX::_map_x * 2) / 3, (MOX::_map_y * 2) / 3);
                                        input_val = 0;
                                    }

                                    int16_t ptr_x = mouse::Pointer_X_();
                                    if ((ptr_x < 5 && MOX::_cur_combat_x > 0) || 
                                        (mouse::Pointer_X_() > 635 && MOX::_cur_combat_x < COMBAT_GRID_WIDTH - COMBAT_GRID_VIEW_WIDTH) || 
                                        (mouse::Pointer_Y_() < 5 && MOX::_cur_combat_y > 0) || 
                                        (mouse::Pointer_Y_() > 475 && MOX::_cur_combat_y < COMBAT_GRID_HEIGHT - COMBAT_GRID_VIEW_HEIGHT)) {
                                        COMBINIT::Scroll_Combat_Screen_();
                                    }

                                    if (input_val == MOX::_center_button) {
                                        COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[MOX::_cur_ship].x, MOX::_combat_data[MOX::_cur_ship].y);
                                    }

                                    if ((int16_t)(input_val + MOX::_space_field) == 0) {
                                        int16_t face_x = MOX::_cur_combat_x + MOX::_xpos / 2;
                                        int16_t face_y = MOX::_cur_combat_y + MOX::_ypos / 2;
                                        int8_t original_owner = MOX::_combat_data[MOX::_cur_ship].owner;
                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_TURN_TO_FACE, face_x, face_y, 0);
                                        }
                                        CMBTMOV1::Turn_To_Face_(MOX::_cur_ship, face_x, face_y);
                                        Advance_After_Ship_Action_(original_owner, active_player_id, &next_ship_flag);
                                        if (MOX::_net_flag != 0) {
                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_space_field) {
                                        int16_t tx = MOX::_cur_combat_x + MOX::_xpos / 2;
                                        int16_t ty = MOX::_cur_combat_y + MOX::_ypos / 2;
                                        int16_t img_num = mouse::Get_Pointer_Image_Number_();
                                        
                                        if (img_num >= 4 && img_num <= 11 && MOX::_combat_data[MOX::_cur_ship].size_class != 0) {
                                            if (MOX::_combat_data[MOX::_cur_ship].size_class <= 2) {
                                                tx = MOX::_cur_combat_x - 1 + (MOX::_xpos + 1) / 2;
                                                ty = MOX::_cur_combat_y - 1 + (MOX::_ypos + 1) / 2;
                                            } else {
                                                tx = MOX::_cur_combat_x - 1 + MOX::_xpos / 2;
                                                ty = MOX::_cur_combat_y - 1 + MOX::_ypos / 2;
                                            }
                                        }

                                        int16_t tgt_idx = -1;
                                        if (tx >= 0 && tx < COMBAT_GRID_WIDTH && ty >= 0 && ty < COMBAT_GRID_HEIGHT) {
                                            tgt_idx = MOX::_combat_grid[tx][ty];
                                        }
                                        if (MOX::_board_flag + MOX::_skan_flag <= 0) {
                                            if (tgt_idx == 2000) {
                                                int8_t orig_owner = MOX::_combat_data[MOX::_cur_ship].owner;
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_MOVE_SHIP, tx, ty, 0);
                                                }
                                                int16_t move_cost = CMBTMOV1::Move_Cost_(MOX::_cur_ship, tx, ty);
                                                if (MOX::_combat_data[MOX::_cur_ship].movement_left < 0) {
                                                    MOX::_combat_data[MOX::_cur_ship].movement_left = 0;
                                                }
                                                int16_t used_tp1 = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                int16_t act_cost = CMBTMOV1::Move_Ship_(MOX::_cur_ship, tx, ty, move_cost);
                                                int16_t used_tp2 = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                if (used_tp1 == used_tp2) {
                                                    MOX::_combat_data[MOX::_cur_ship].movement_left -= act_cost;
                                                    if (MOX::_combat_data[MOX::_cur_ship].movement_left < 0) {
                                                        MOX::_combat_data[MOX::_cur_ship].movement_left = 0;
                                                    }
                                                }
                                                Assign_Combat_Grids_(MOX::_cur_ship);
                                                CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                Advance_After_Ship_Action_(orig_owner, active_player_id, &next_ship_flag);
                                                if (MOX::_net_flag != 0) {
                                                    NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                                    CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                }
                                            } else if (tgt_idx >= 0 && tgt_idx < MOX::_combat_total_ship_count && MOX::_combat_data[tgt_idx].unit_status == 0) {
                                                if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_data[tgt_idx].owner) {
                                                    if (MOX::_combat_init == 0) {
                                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_SELECT_SHIP, tgt_idx, 0, 0);
                                                        }
                                                        Set_Cur_Ship_To_(tgt_idx);
                                                        if (MOX::_net_flag != 0) {
                                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                        }
                                                    }
                                                } else {
                                                    if (CMBTMOV1::Ship_Is_In_Stasis_(tgt_idx) != 0) {
                                                        char buf[80];
                                                        KEN::Ken_Get_Text_Message_(61, buf, sizeof(buf));
                                                        palette::Set_Palette_Gradient_(144, 32, 0, 0, 0, 0, 0, 0);
                                                        palette::Refresh_Palette_();
                                                        if (GENDRAW::Do_Confirmation_Box_(buf) != 0) {
                                                            if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                                Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_REMOVE_STASIS, tgt_idx, 0, 0);
                                                            }
                                                            CMBTFIRE::Remove_Stasis_Beam_(tgt_idx);
                                                            if (MOX::_net_flag != 0) {
                                                                NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                                                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                            }
                                                        }
                                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                    } else if (MOX::_combat_data[tgt_idx].special_status_flag == 4) {
                                                        char buf[80];
                                                        KEN::Ken_Get_Text_Message_(80, buf, sizeof(buf));
                                                        palette::Refresh_Palette_();
                                                        GENDRAW::Message_Box_(buf);
                                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                    } else {
                                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_FIRE_WEAPON, tgt_idx, 0, 0);
                                                        }
                                                        CMBTFIRE::Fire_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, tgt_idx, MOX::_combat_data[tgt_idx].x * 20, MOX::_combat_data[tgt_idx].y * 20, 0);
                                                        Add_Tactical_Combat_Fields_();
                                                        Assign_Combat_Grids_(MOX::_cur_ship);
                                                        for (int16_t w = 0; w < 8; ++w) {
                                                            if (MOX::_combat_data[MOX::_cur_ship].weapons[w].active == 0) {
                                                                MOX::_combat_data[MOX::_cur_ship].weapons[w].active = 1;
                                                            }
                                                        }
                                                        if (MOX::_net_flag != 0) {
                                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                        }
                                                    }
                                                }
                                            }
                                        } else if (MOX::_skan_flag != 0 && tgt_idx >= 0 && tgt_idx < MOX::_combat_total_ship_count && MOX::_combat_data[tgt_idx].unit_status == 0) {
                                            CMBTDRW1::Display_Combat_View_Ship_(tgt_idx);
                                        } else if (MOX::_board_flag != 0 && tgt_idx >= 0 && tgt_idx < MOX::_combat_total_ship_count && MOX::_combat_data[tgt_idx].unit_status == 0) {
                                            Initiate_Boarding_Action_(MOX::_cur_ship, tgt_idx);
                                        }
                                        input_val = 0;
                                    }

                                    if (input_val == _end_of_turn_button) {
                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_END_TURN, MOX::_cur_combat_x, MOX::_cur_combat_y, 0);
                                        }
                                        if (MOX::_combat_init == 0) {
                                            next_ship_flag = 1;
                                        } else {
                                            MOX::_side_end_of_turn[MOX::_combat_data[MOX::_cur_ship].owner] = 1;
                                            next_ship_flag = Next_Ship_(active_player_id);
                                        }
                                        if (MOX::_net_flag != 0) {
                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_done_button) {
                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, TACTICAL_NET_MSG_SHIP_DONE, 0, 0, 0);
                                        }
                                        CMBTFIR2::Set_Combat_Ship_Done_(MOX::_cur_ship);
                                        MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                        next_ship_flag = Next_Ship_(active_player_id);
                                        if (MOX::_net_flag != 0) {
                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        fields::Set_Input_Delay_(3);
                                    }

                                    if (next_ship_flag == 0) {
                                        if (MOX::_FULL_DRAW_SCREEN_FLAG == 0 && MOX::_combat_data[MOX::_cur_ship].movement_left == m_left) {
                                            video::Set_Page_Off_();
                                            CMBTDRW1::Draw_Main_Combat_Screen_();
                                            RUSS::Mox_Sync_Update_();
                                        } else {
                                            Add_Tactical_Combat_Fields_();
                                            Assign_Combat_Grids_(MOX::_cur_ship);
                                            CMBTDRW1::Refresh_Combat_Screen_Full_();
                                        }
                                    }

                                } else {
                                    int16_t net_processed = 0;
                                    while (net_processed == 0) {
                                        if (MOX::_tactical_net_choice == 0) {
                                            timer::Mark_Time_();
                                            if (MOX::_net_flag != 0) {
                                                if (fields::Get_Input_() == MOX::_auto_button) {
                                                    CMBTAI::Toggle_Auto_Combat_Flags_();
                                                }
                                            }
                                            video::Set_Page_Off_();
                                            CMBTDRW1::Draw_Main_Combat_Screen_();
                                            RUSS::Mox_Sync_Update_();
                                            fields::Get_Input_();
                                            timer::Release_Time_(1);
                                        } else {
                                            net_processed = 1;
                                            COMBINIT::Snap_Center_Combat_Screen_(MOX::_new_message.viewport_x + 16, MOX::_new_message.viewport_y + 9);
                                            int16_t f10 = MOX::_new_message.arg1;
                                            int16_t f12 = MOX::_new_message.arg2;
                                            int16_t f14 = MOX::_new_message.arg3;
                                            int16_t f8 = MOX::_new_message.message_code;
                                            game_random::Set_Random_Seed_(MOX::_new_message.random_seed);
                                            struct_::Clear_Structure_(&MOX::_new_message, sizeof(MOX::_new_message));

                                            NETMOX::Net_Debug_Log_(
                                                "tactical action consume local_player=%d action=%d args=%d,%d,%d turn=%d cur_ship=%d cur_ptr=%d",
                                                (int)MOX::_PLAYER_NUM,
                                                (int)f8,
                                                f10,
                                                f12,
                                                f14,
                                                (int)MOX::_combat_turn,
                                                MOX::_cur_ship,
                                                (int)MOX::_cur_ptr);
                                            switch (f8) {
                                                case TACTICAL_NET_MSG_SELF_DESTRUCT:
                                                    MOX::_combat_data[MOX::_cur_ship].drive_current_hits = 0;
                                                    {
                                                        int8_t saved_ftl_type = MOX::_combat_data[MOX::_cur_ship].ftl_type;
                                                        MOX::_combat_data[MOX::_cur_ship].ftl_type = 1;
                                                        CMBTFIRE::Destroy_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship));
                                                        MOX::_combat_data[MOX::_cur_ship].ftl_type = saved_ftl_type;
                                                    }
                                                    MOX::_tactical_net_choice = TACTICAL_NET_MSG_SELF_DESTRUCT;
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(active_player_id);
                                                    break;
                                                case TACTICAL_NET_MSG_AUTO_TURN:
                                                    CMBTAI::Do_Auto_Ship_Turn_();
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(active_player_id);
                                                    break;
                                                case TACTICAL_NET_MSG_SHIP_DONE:
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(active_player_id);
                                                    break;
                                                case TACTICAL_NET_MSG_MOVE_SHIP: {
                                                    int8_t orig_owner = MOX::_combat_data[MOX::_cur_ship].owner;
                                                    int16_t move_cost = CMBTMOV1::Move_Cost_(MOX::_cur_ship, f10, f12);
                                                    if (MOX::_combat_data[MOX::_cur_ship].movement_left < 0) {
                                                        MOX::_combat_data[MOX::_cur_ship].movement_left = 0;
                                                    }
                                                    int16_t used_tp1 = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                    int16_t act_cost = CMBTMOV1::Move_Ship_(MOX::_cur_ship, f10, f12, move_cost);
                                                    int16_t used_tp2 = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                    if (used_tp1 == used_tp2) {
                                                        MOX::_combat_data[MOX::_cur_ship].movement_left -= act_cost;
                                                        if (MOX::_combat_data[MOX::_cur_ship].movement_left < 0) {
                                                            MOX::_combat_data[MOX::_cur_ship].movement_left = 0;
                                                        }
                                                    }
                                                    Assign_Combat_Grids_(MOX::_cur_ship);
                                                    CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                    Advance_After_Ship_Action_(orig_owner, active_player_id, &next_ship_flag);
                                                    break;
                                                }
                                                case TACTICAL_NET_MSG_FIRE_WEAPON:
                                                    if (f12 == 0) {
                                                        CMBTFIRE::Fire_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, f10, MOX::_combat_data[f10].x * 20, MOX::_combat_data[f10].y * 20, f12);
                                                    } else {
                                                        CMBTFIRE::Fire_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, f10, MOX::_missile[f10].x, MOX::_missile[f10].y, f12);
                                                    }
                                                    Add_Tactical_Combat_Fields_();
                                                    Assign_Combat_Grids_(MOX::_cur_ship);
                                                    for (int16_t i = 0; i < 8; ++i) {
                                                        if (MOX::_combat_data[MOX::_cur_ship].weapons[i].active == 0) {
                                                            MOX::_combat_data[MOX::_cur_ship].weapons[i].active = 1;
                                                        }
                                                    }
                                                    break;
                                                case TACTICAL_NET_MSG_CYCLE_MISSILE_WARNING:
                                                    MOX::_missile_warning = (MOX::_missile_warning + 1) % 3;
                                                    Add_Tactical_Combat_Fields_();
                                                    Assign_Combat_Grids_(MOX::_cur_ship);
                                                    break;
                                                case TACTICAL_NET_MSG_TOGGLE_AUTO_COMBAT:
                                                    fields::Clear_Fields_();
                                                    MOX::_auto_button = fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0);
                                                    if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_attacker_player) {
                                                        MOX::_auto_combat_flag[0] = 1 - MOX::_auto_combat_flag[0];
                                                        MOX::_auto_combat = MOX::_auto_combat_flag[0];
                                                    } else {
                                                        MOX::_auto_combat_flag[1] = 1 - MOX::_auto_combat_flag[1];
                                                        MOX::_auto_combat = MOX::_auto_combat_flag[1];
                                                    }
                                                    break;
                                                case TACTICAL_NET_MSG_WAIT:
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(active_player_id);
                                                    break;
                                                case TACTICAL_NET_MSG_TOGGLE_WEAPON:
                                                    MOX::_combat_data[MOX::_cur_ship].weapons[f10].active -= 1;
                                                    if (MOX::_combat_data[MOX::_cur_ship].weapons[f10].active < -1) {
                                                        MOX::_combat_data[MOX::_cur_ship].weapons[f10].active = 1;
                                                    }
                                                    break;
                                                case TACTICAL_NET_MSG_RETREAT:
                                                    MOX::_combat_data[MOX::_cur_ship].is_retreating = 1;
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(active_player_id);
                                                    break;
                                                case TACTICAL_NET_MSG_TURN_TO_FACE: {
                                                    int8_t original_owner = MOX::_combat_data[MOX::_cur_ship].owner;
                                                    CMBTMOV1::Turn_To_Face_(MOX::_cur_ship, f10, f12);
                                                    Advance_After_Ship_Action_(original_owner, active_player_id, &next_ship_flag);
                                                    break;
                                                }
                                                case TACTICAL_NET_MSG_REMOVE_STASIS:
                                                    CMBTFIRE::Remove_Stasis_Beam_(f10);
                                                    break;
                                                case TACTICAL_NET_MSG_SELECT_SHIP:
                                                    if (MOX::_combat_init == 0) {
                                                        Set_Cur_Ship_To_(f10);
                                                    }
                                                    break;
                                                case TACTICAL_NET_MSG_BOARDING_ACTION:
                                                    if (f14 == 1) {
                                                        if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 1) {
                                                            if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                                                                CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 2);
                                                            }
                                                            MOX::_combat_data[MOX::_cur_ship].special_status_flag = 2;
                                                            Recalculate_Combat_Ship_(MOX::_cur_ship);
                                                        } else if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 4) {
                                                            if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                                                                CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 5);
                                                            }
                                                            MOX::_combat_data[MOX::_cur_ship].special_status_flag = 6;
                                                        }
                                                        CMBTFIR2::Set_Boarded_Flag_(MOX::_cur_ship);
                                                        MOX::_combat_data[MOX::_cur_ship].marine_count -= (int8_t)f12;
                                                        CMBTFIR2::Resolve_Raid_(MOX::_cur_ship, f10, &f12);
                                                        MOX::_combat_data[MOX::_cur_ship].marine_count += (int8_t)f12;
                                                    } else {
                                                        if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 1) {
                                                            if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                                                                CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 2);
                                                            }
                                                            MOX::_combat_data[MOX::_cur_ship].special_status_flag = 2;
                                                            Recalculate_Combat_Ship_(MOX::_cur_ship);
                                                        } else if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 4) {
                                                            if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                                                                CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 5);
                                                            }
                                                            MOX::_combat_data[MOX::_cur_ship].special_status_flag = 6;
                                                        }
                                                        CMBTFIR2::Set_Boarded_Flag_(MOX::_cur_ship);
                                                        MOX::_combat_data[MOX::_cur_ship].marine_count -= (int8_t)f12;
                                                        CMBTFIR2::Resolve_Capture_(MOX::_cur_ship, f10, f12, MOX::_combat_data[f10].x * 20, MOX::_combat_data[f10].y * 20);
                                                    }
                                                    break;
                                                case TACTICAL_NET_MSG_END_TURN:
                                                    if (MOX::_combat_init == 0) {
                                                        next_ship_flag = 1;
                                                    } else {
                                                        MOX::_side_end_of_turn[MOX::_combat_data[MOX::_cur_ship].owner] = 1;
                                                        next_ship_flag = Next_Ship_(active_player_id);
                                                    }
                                                    break;
                                            }
                                            MOX::_tactical_net_choice = 0;
                                            NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                    }
                                }
                            }
                        } else {
                            MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                            next_ship_flag = Next_Ship_(active_player_id);
                        }
                    }

                    if (next_ship_flag == 0) {
                        if (MOX::_FULL_DRAW_SCREEN_FLAG == 0 && MOX::_combat_data[MOX::_cur_ship].movement_left == m_left) {
                            video::Set_Page_Off_();
                            CMBTDRW1::Draw_Main_Combat_Screen_();
                            RUSS::Mox_Sync_Update_();
                        } else {
                            Add_Tactical_Combat_Fields_();
                            Assign_Combat_Grids_(MOX::_cur_ship);
                            CMBTDRW1::Refresh_Combat_Screen_Full_();
                        }
                    }
                    timer::Release_Time_(1);
                }
            }
        }

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (Combat_Ship_Is_Functional_(i) != 0) {
                    if (MOX::_combat_init == 0 || MOX::_combat_data[i].owner == active_player_id) {
                        MOX::_combat_data[i].reflected_damage_pool = 0;
                    }
                }
            }
        }

        if (time_warp_only == 0) {
            Seeking_Missiles_(active_player_id);
        }

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (Combat_Ship_Is_Functional_(i) != 0) {
                    if (MOX::_combat_data[i].owner == active_player_id) {
                        for (int16_t j = 0; j < 8; ++j) {
                            if (MOX::_combat_data[i].weapons[j].status == 2) {
                                MOX::_combat_data[i].weapons[j].status = 1;
                            }
                        }
                        const int8_t recloak_state =
                            _recloak_next_turn != 0 ? 2 : 3;
                        if (MOX::_combat_data[i].special_status_flag == recloak_state &&
                            COMBINIT::Does_Combat_Ship_Have_Special_(
                                i, SPECIAL_CLOAKING_DEVICE) != 0) {
                            CMBTSPEC::Draw_Cloak_(i, 1);
                            MOX::_combat_data[i].special_status_flag = 1;
                        }
                        const int8_t rephase_state =
                            _rephase_cloak_next_turn != 0 ? 6 : 7;
                        if (MOX::_combat_data[i].special_status_flag == rephase_state &&
                            MOX::_combat_data[i].special_status_timer > 0 &&
                            COMBINIT::Does_Combat_Ship_Have_Special_(
                                i, SPECIAL_PHASING_CLOAK) != 0) {
                            CMBTSPEC::Draw_Cloak_(i, 4);
                            MOX::_combat_data[i].special_status_flag = 4;

                            const uint8_t tractor_target_idx = MOX::_combat_data[i].special_cooldown;
                            MOX::_combat_data[i].special_cooldown = UINT8_MAX;
                            if (tractor_target_idx < MOX::_combat_total_ship_count) {
                                Recalculate_Combat_Ship_(tractor_target_idx);
                            }

                            for (int16_t tractor_ship_idx = 0;
                                 tractor_ship_idx < MOX::_combat_total_ship_count;
                                 ++tractor_ship_idx) {
                                if (MOX::_combat_data[tractor_ship_idx].special_cooldown == i) {
                                    MOX::_combat_data[tractor_ship_idx].special_cooldown = UINT8_MAX;
                                }
                            }

                            Recalculate_Combat_Ship_(i);
                            CMBTDRW1::Refresh_Combat_Screen_Full_();

                            for (int16_t m = 0; m < 300; ++m) {
                                if (MOX::_missile[m].type > 0 && MOX::_missile[m].target_unit_idx == i && MOX::_missile[m].source_unit_idx != i) {
                                    CMBTMIS::Retarget_Missile_(m);
                                }
                            }
                        }
                        MOX::_combat_data[i].ai_range_tactic = (int8_t)CMBTFIR2::Max_Defensive_Fire_Range_(i);
                    }
                }
            }
        }

        if (MOX::_net_flag != 0) {
            if (MOX::_PLAYER_NUM != active_player_id) {
                MOX::_msl_impact_warning_flag = MOX::_saved_msl_warn;
            }
            if (MOX::_net_flag != 0) {
                NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
            }
        }
    }

    void __cdecl Update_Phasing_Cloak_Timers_() {
        if (MOX::_combat_total_ship_count <= 0) {
            return;
        }

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            if (Combat_Ship_Is_Functional_(i) == 0) {
                continue;
            }

            s_combat_data* ship = &MOX::_combat_data[i];
            if (ship->special_status_flag != 4 ||
                COMBINIT::Does_Combat_Ship_Have_Special_(
                    i, SPECIAL_PHASING_CLOAK) == 0) {
                continue;
            }

            if (ship->special_status_timer > 0) {
                ship->special_status_timer--;
            }

            if (ship->special_status_timer == 0) {
                CMBTSPEC::Draw_Cloak_(i, 5);
                CMBTSPEC::Draw_Cloak_(i, 1);
                ship->special_status_flag = 1;
            }
        }
    }

    int16_t __cdecl Calc_Current_Speed_(int16_t ship_idx)
    {
        int16_t speed = 0;

        if (MOX::_combat_data[ship_idx].ftl_type == DRIVE_NO_DRIVE || CMBTMOV1::Ship_In_Black_Hole_(ship_idx)) {
            speed = 0;
        } else {
            int combat_speed = (uint8_t)MOX::_combat_data[ship_idx].combat_speed_base;
            
            if (struct_::Test_Bit_Field_(MOX::_combat_data[ship_idx].special_device_flags, SPECIAL_AUGMENTED_ENGINES) == 1 &&
                struct_::Test_Bit_Field_(MOX::_combat_data[ship_idx].special_device_damage_flags, SPECIAL_AUGMENTED_ENGINES) == 0) {
                combat_speed += 5;
            }
            
            int owner = MOX::_combat_data[ship_idx].owner;

            if (MOX::_combat_data[ship_idx].ftl_type != DRIVE_NO_DRIVE) {
                int current_hits = MOX::_combat_data[ship_idx].drive_current_hits;
                int max_hits = MOX::_combat_data[ship_idx].drive_max_hits;

                if (owner <= 9) {
                    int half_max = max_hits / 2;
                    int range = max_hits - half_max;
                    speed = (int16_t)(((current_hits - half_max) * combat_speed) / range);
                } else {
                    speed = (int16_t)((current_hits * combat_speed) / max_hits);
                }
            }
            
            if (speed < 0) {
                speed = 0;
            }
            
            if (owner < MAX_PLAYERS && MOX::_player[owner].traits[TRAIT_TRANS_DIMENSIONAL] != 0) {
                speed += 4;
            }
            
            if (speed > 0) {
                int16_t tractor_beams = 0;
                for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                    const int16_t tractor_target_idx = MOX::_combat_data[i].special_cooldown == UINT8_MAX
                        ? -1
                        : MOX::_combat_data[i].special_cooldown;
                    if (tractor_target_idx == ship_idx &&
                        Combat_Ship_Is_Functional_(i)) {
                        for (int j = 0; j < 8; ++j) {
                            if (MOX::_combat_data[i].weapons[j].weapon_id == WEAPON_TRACTOR_BEAM) {
                                tractor_beams += MOX::_combat_data[i].weapons[j].count;
                            }
                        }
                    }
                }
                
                const int8_t size_class = MOX::_combat_data[ship_idx].size_class;
                int16_t tractors_to_stop = size_class + 1;
                if (size_class >= 0 && size_class < SHIP_SIZE_COUNT) {
                    tractors_to_stop = _tractors_to_stop[(uint8_t)size_class];
                }

                if (tractors_to_stop <= tractor_beams) {
                    speed = 0;
                } else {
                    speed -= (int16_t)((speed * tractor_beams) / tractors_to_stop);
                    if (speed < 1) {
                        speed = 1;
                    }
                }
            }
        }

        return speed;
    }

    void __cdecl Init_Ship_For_Start_Of_Turn_(int16_t ship_idx) {
        int16_t is_captured_now = CMBTFIR2::Being_Captured_(ship_idx);
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        ship->combat_status_flags = 0;
        if (is_captured_now != 0) {
            CMBTFIR2::Set_Capturing_Flag_(ship_idx);
        }
        ship->field42_0x49 = 0;

        if (ship->is_captured == 1) {
            ship->movement_left = 0;
            ship->special_status_flag = 2;
            for (int i = 0; i < 8; ++i) {
                ship->weapons[i].shots_left = 0;
                ship->weapons[i].status = 0;
            }
            return;
        }

        int16_t stasis_count = Get_Number_Of_Ships_In_Stasis_(ship_idx);
        int16_t speed = Calc_Current_Speed_(ship_idx);
        ship->current_speed = speed;
        ship->movement_left = speed;
        movement_reserve[ship_idx] = Maximum_Combat_Speed_(ship_idx) - speed;

        if (ship->special_status_flag < 4 &&
            ship->special_status_timer != 0 &&
            COMBINIT::Does_Combat_Ship_Have_Special_(
                ship_idx, SPECIAL_PHASING_CLOAK) != 0) {
            ship->special_status_flag = 6;
        }
        if (ship->special_status_flag == 2) {
            ship->special_status_flag = 3;
        }
        if (ship->special_status_flag == 6) {
            ship->special_status_flag = 7;
        }

        for (int i = 0; i < 8; ++i) {
            s_combat_weapon_slot* slot = &ship->weapons[i];
            int8_t count = slot->count;

            if (slot->weapon_id == WEAPON_STASIS_FIELD) {
                if ((int16_t)count < stasis_count) {
                    slot->shots_left = 0;
                    stasis_count -= (int16_t)count;
                } else {
                    slot->shots_left = (int8_t)((int16_t)count - stasis_count);
                    stasis_count = 0;
                }
            } else {
                slot->shots_left = count;
            }

            if (slot->active != -1) {
                slot->active = 1;
            }

            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_HYPER_X_CAPACITORS) != 0) {
                if (TECHDATA::_weapons[slot->weapon_id].type == WEAPON_TYPE_BEAM && slot->status != 1) {
                    slot->status = 2;
                }
            }

            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_FAST_MISSILE_RACKS) != 0) {
                if (TECHDATA::_weapons[slot->weapon_id].type == WEAPON_TYPE_MISSILE && slot->status != 1) {
                    slot->status = 2;
                }
            }

            if (slot->weapon_id > 0 && TECHDATA::_weapons[slot->weapon_id].type == WEAPON_TYPE_TORPEDO) {
                if (slot->ammo > 0) {
                    slot->ammo--;
                }
            }
        }

        if (_continuous_bhg != 0) {
            int16_t active_target_count = 0;
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].black_hole_source_idx == ship_idx) {
                    active_target_count++;
                }
            }

            for (int i = 0; i < 8 && active_target_count > 0; ++i) {
                s_combat_weapon_slot* slot = &ship->weapons[i];
                if (slot->weapon_id != WEAPON_BLACK_HOLE_GENERATOR) {
                    continue;
                }

                int16_t committed_shots = slot->shots_left;
                if (committed_shots > active_target_count) {
                    committed_shots = active_target_count;
                }
                slot->shots_left -= (int8_t)committed_shots;
                active_target_count -= committed_shots;
            }
        }
    }

    int __cdecl qsort_ship_initiative_(const void* p1, const void* p2) {
        const int16_t* ship1_ptr = (const int16_t*)p1;
        const int16_t* ship2_ptr = (const int16_t*)p2;

        int16_t init2 = Ship_Initiative_Value_(*ship2_ptr);
        int16_t init1 = Ship_Initiative_Value_(*ship1_ptr);

        if (init2 == init1) {
            return 0;
        }

        init2 = Ship_Initiative_Value_(*ship2_ptr);
        init1 = Ship_Initiative_Value_(*ship1_ptr);

        if (init2 < init1) {
            return -1;
        }

        return 1;
    }

    static bool Should_End_Combat_() {
        int16_t side_counts[2] = {};

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            const s_combat_data* unit = &MOX::_combat_data[i];
            if (unit->unit_status != 5 && unit->is_captured == 0) {
                side_counts[unit->owner == MOX::_combat_attacker_player]++;
            }
        }

        const bool attacker_is_human =
            MOX::_combat_attacker_player >= 0 &&
            MOX::_combat_attacker_player < MAX_PLAYERS &&
            MOX::_combat_attacker_player < MOX::_NUM_PLAYERS &&
            MOX::_player[MOX::_combat_attacker_player].objectives == PLAYER_OBJECTIVE_HUMAN;

        if (MOX::_cmbt_colony_id != -1 &&
            CMBTFIRE::Does_Combat_Planet_Have_Defenses_() == 0 &&
            (!attacker_is_human || MOX::_auto_combat_flag[0] == 1) &&
            side_counts[0] > 0) {
            side_counts[0]--;
        }

        if (side_counts[0] > 0 && side_counts[1] > 0) {
            return false;
        }

        const int16_t surviving_player =
            side_counts[0] == 0
                ? MOX::_combat_attacker_player
                : MOX::_combat_defender_player;

        if (surviving_player < 0 ||
            surviving_player >= MAX_PLAYERS ||
            surviving_player >= MOX::_NUM_PLAYERS ||
            MOX::_player[surviving_player].objectives != PLAYER_OBJECTIVE_HUMAN) {
            return true;
        }

        for (int16_t i = 0; i < 300; ++i) {
            const s_missile* missile = &MOX::_missile[i];
            if (missile->type > 0 &&
                missile->quantity != 0 &&
                missile->owner != surviving_player) {
                return false;
            }
        }

        return true;
    }

    int16_t __cdecl Check_For_Winner_() {
        int16_t attacker_count = 0;
        int16_t defender_count = 0;
        int16_t winner = 0;

        if (MOX::_combat_data[0].unit_status != 5) {
            if (CMBTFIRE::Does_Combat_Planet_Have_Defenses_()) {
                defender_count = 1;
            }
        }

        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
            if (MOX::_combat_data[i].unit_status != 5 && MOX::_combat_data[i].is_captured == 0) {
                if (MOX::_combat_data[i].owner == MOX::_combat_attacker_player) {
                    attacker_count++;
                } else {
                    defender_count++;
                }
            }
        }

        if (MOX::_combat_turn > 50) {
            winner = -1;
            if (defender_count <= 0) {
                winner = 666;
            }
        }

        if (attacker_count <= 0) {
            winner = -1;
            if (defender_count <= 0) {
                winner = 666;
            }
        } else if (defender_count <= 0) {
            winner = 1;
        }

        if (winner != 0 && MOX::_net_flag != 0) {
            NETMOX::Net_Debug_Log_(
                "tactical winner local_player=%d winner=%d attackers=%d defenders=%d turn=%d cur_ship=%d cur_ptr=%d choice=%d barrier_counter=%u",
                (int)MOX::_PLAYER_NUM,
                (int)winner,
                (int)attacker_count,
                (int)defender_count,
                (int)MOX::_combat_turn,
                MOX::_cur_ship,
                (int)MOX::_cur_ptr,
                (int)MOX::_tactical_net_choice,
                (unsigned int)MOX::_combat_done_waiting);
        }

        return winner;
    }

    int16_t __cdecl Next_Ship_(int16_t unused_player_idx) {
        (void)unused_player_idx;
        int16_t saved_cur_ship = MOX::_cur_ship;
        int16_t saved_cur_ptr = MOX::_cur_ptr;
        int16_t planet_has_defenses = 0;

        if (Should_End_Combat_()) {
            return 1;
        }

        do {
            MOX::_cur_ptr++;
            int16_t current_ptr = MOX::_cur_ptr;

            if (MOX::_ship_turn_order[current_ptr] == -10) {
                if (MOX::_cur_ptr != 0 && MOX::_cur_ptr != 1) {
                    MOX::_cur_ptr = 0;
                } else {
                    MOX::_cur_ptr = saved_cur_ptr;
                }
            }

            current_ptr = MOX::_cur_ptr;
            if (MOX::_ship_turn_order[current_ptr] == 0) {
                if (CMBTFIRE::Does_Combat_Planet_Have_Defenses_() != 0) {
                    planet_has_defenses = 1;
                }
            }

            if (saved_cur_ptr == MOX::_cur_ptr) {
                break;
            }

            current_ptr = MOX::_cur_ptr;
            int16_t ship_idx = MOX::_ship_turn_order[current_ptr];

            if (ship_idx == 0) {
                if (planet_has_defenses != 0) break;
            } else if (ship_idx != -1) {
                int8_t owner = MOX::_combat_data[ship_idx].owner;
                if (MOX::_side_end_of_turn[owner] == 0) {
                    if (COMBAT1::Combat_Ship_Is_Functional_(ship_idx) != 0) {
                        break;
                    }
                }
            }
        } while (true);

        if (MOX::_cur_ptr < 0) {
            MOX::_cur_ship = -1;
        } else {
            MOX::_cur_ship = MOX::_ship_turn_order[MOX::_cur_ptr];
        }

        int16_t result;
        if (MOX::_cur_ship < 0) {
            result = 1;
            MOX::_cur_ship = saved_cur_ship;
            MOX::_cur_ptr = saved_cur_ptr;
        } else {
            if (MOX::_combat_init != 0 || (CMBTDRW1::Ship_Completely_On_Screen_(MOX::_cur_ship) == 0 && COMBAT1::_start_of_turn != 0)) {
                COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[MOX::_cur_ship].x, MOX::_combat_data[MOX::_cur_ship].y);
            }

            COMBAT1::Add_Tactical_Combat_Fields_();
            COMBAT1::Assign_Combat_Grids_(MOX::_cur_ship);
            CMBTDRW1::Refresh_Combat_Screen_Full_();

            int8_t owner = MOX::_combat_data[MOX::_cur_ship].owner;
            if (owner == MOX::_combat_attacker_player) {
                MOX::_auto_combat = MOX::_auto_combat_flag[0];
            } else {
                MOX::_auto_combat = MOX::_auto_combat_flag[1];
            }

            if (MOX::_net_flag != 0) {
                if (owner == MOX::_combat_attacker_player) {
                    MOX::_net_destination = MOX::_combat_defender_player;
                } else {
                    MOX::_net_destination = MOX::_combat_attacker_player;
                }

                if (owner == MOX::_PLAYER_NUM) {
                    MOX::_msl_impact_warning_flag = MOX::_saved_msl_warning;
                    NETMOX::Mox_Send_Message_(MOX::_player[MOX::_net_destination].network_player_id, TACTICAL_NET_PACKET_MISSILE_WARNING, &MOX::_msl_impact_warning_flag, sizeof(MOX::_msl_impact_warning_flag));
                }
                NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
            }
            result = 0;
        }

        MOX::_missile_warning = 0;
        if (MOX::_super_fast_flag != 0) {
            CMBTDRW1::Draw_Super_Fast_Screen_();
        }

        return result;
    }

    int16_t __cdecl Ship_Done_Check_(void) {
        int16_t done = 1;
        s_combat_data* ship = &MOX::_combat_data[MOX::_cur_ship];

        if (ship->movement_left > 0) {
            done = 0;
        }

        for (int i = 0; i < 8; ++i) {
            if (ship->weapons[i].shots_left > 0) {
                done = 0;
            }
        }

        return done;
    }

    void __cdecl Retreat_Ship_(int16_t ship_idx) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];
        int16_t is_dead = CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx);
        int16_t speed = 0;
        if (is_dead == 0) {
            speed = Calc_Current_Speed_(ship_idx);
        }

        if (is_dead != 0 || speed <= 0 || ship->is_retreating != 1) {
            if (is_dead != 0 || speed <= 0) {
                ship->movement_left = 0;
            }
            ship->is_retreating = 0;
            return;
        }

        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
            if (CMBTDRW1::Ship_On_Screen_(ship_idx) == 0) {
                COMBINIT::Snap_Center_Combat_Screen_(ship->x, ship->y);
            }
            CMBTDRW1::Draw_Retreating_Ship_(ship_idx);
        }

        ship->unit_status = 5;
        ship->is_retreating = 2;
        CMBTFIRE::Retarget_Missiles_Aimed_At_(ship_idx);
        COMBAT1::Remove_Continuing_Effect_Specials_(ship_idx);
    }

    int16_t __cdecl Get_Number_Of_Ships_In_Stasis_(int16_t source_ship_idx) {
        int16_t count = 0;

        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
            const int16_t stasis_source_idx = MOX::_combat_data[i].stasis_source_idx == UINT8_MAX
                ? -1
                : MOX::_combat_data[i].stasis_source_idx;
            if (source_ship_idx >= 0 && stasis_source_idx == source_ship_idx) {
                count++;
            }
        }

        return count;
    }

    void __cdecl Add_Missile_Fields_() {
        MOX::_cur_missile_count = 0;
        
        graphics::Set_Window_(0, 0, 0x27F, 0x15E);
        video::Clip_On_();

        for (int16_t i = 0; i < 300; ++i) {
            s_missile* missile = &MOX::_missile[i];

            if (missile->type > 0 && CMBTMIS::On_Combat_Screen_((missile->x - 8) / 20, (missile->y - 8) / 20)) {
                int16_t local_x = missile->x - (MOX::_cur_combat_x * 20);
                int16_t local_y = missile->y - (MOX::_cur_combat_y * 20);

                MOX::_missile_field[i] = fields::Add_Clipped_Hidden_Field_(
                    local_x - 16,
                    local_y - 16,
                    local_x - 8,
                    local_y - 8,
                    "",
                    0
                );

                MOX::_combat_mouse_grid[i + 577].x1 = local_x - 16;
                MOX::_combat_mouse_grid[i + 577].x2 = local_x - 8;
                MOX::_combat_mouse_grid[i + 577].y1 = local_y - 16;
                MOX::_combat_mouse_grid[i + 577].y2 = local_y - 8;
                MOX::_combat_mouse_grid[i + 577].frame = 0;
                MOX::_combat_mouse_grid[i + 577].icon = 12;

                MOX::_cur_missile_count++;
            } else {
                MOX::_missile_field[i] = -1000;
                
                MOX::_combat_mouse_grid[i + 577].x1 = 0;
                MOX::_combat_mouse_grid[i + 577].x2 = 0;
                MOX::_combat_mouse_grid[i + 577].y1 = 0;
                MOX::_combat_mouse_grid[i + 577].y2 = 0;
                MOX::_combat_mouse_grid[i + 577].frame = 0;
                MOX::_combat_mouse_grid[i + 577].icon = 0;
            }
        }

        video::Clip_Off_();
        graphics::Reset_Window_();
    }

    void __cdecl Remove_Continuing_Effect_Specials_(int16_t source_idx) {
        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            // Check if this ship is being held in stasis by the source ship
            if (MOX::_combat_data[i].stasis_source_idx == source_idx) {
                CMBTFIRE::Remove_Stasis_Beam_(i);
            }

            // Check if this ship is affected by a black hole created by the source ship
            if (MOX::_combat_data[i].black_hole_source_idx == source_idx) {
                MOX::_combat_data[i].black_hole_flag = 0;
                MOX::_combat_data[i].black_hole_source_idx = UINT8_MAX;
            }

            // Check if special cooldown (or effect tracking) matches the source ship
            if (MOX::_combat_data[i].special_cooldown == source_idx) {
                MOX::_combat_data[i].special_cooldown = 0xFF;
            }
        }

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            Recalculate_Combat_Ship_(i);
        }
        if (_interactive_combat_stats != 0) {
            CMBTFIR2::Set_Combat_Ordnance_Values_();
        }
    }

    void __cdecl Assign_Mouse_Images_() {
        if (MOX::_game_type != 1 && MOX::_combat_data[MOX::_cur_ship].owner != MOX::_PLAYER_NUM) {
            MOX::_cmbt_mouse_list_clock.icon = MOX::_combat_clock_frame;
            mouse::Set_Mouse_List_(&MOX::_cmbt_mouse_list_clock, 1);
            return;
        }

        if (MOX::_order_info_flag != 1) {
            mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
            return;
        }

        int grid_idx = 1;
        int16_t x_val = 0;

        for (int i = 0; i < COMBAT_GRID_VIEW_WIDTH; ++i) {
            int16_t y_val = 0;
            for (int j = 0; j < COMBAT_GRID_VIEW_HEIGHT; ++j) {
                MOX::_combat_mouse_grid[grid_idx].x1 = x_val;
                MOX::_combat_mouse_grid[grid_idx].x2 = x_val + 0x13;
                MOX::_combat_mouse_grid[grid_idx].y1 = y_val;

                if (j == COMBAT_GRID_VIEW_MAX_Y) {
                    MOX::_combat_mouse_grid[grid_idx].y2 = y_val + 0x16;
                } else {
                    MOX::_combat_mouse_grid[grid_idx].y2 = y_val + 0x13;
                }

                grid_idx++;
                y_val += 0x14;
            }
            x_val += 0x14;
        }

        int16_t size_info = 0;
        if (MOX::_combat_data[MOX::_cur_ship].size_class > 0) {
            if (MOX::_combat_data[MOX::_cur_ship].size_class <= 2) {
                size_info = 10;
            } else {
                size_info = 20;
            }
        }

        for (int grid_x = COMBAT_GRID_VIEW_MAX_X; grid_x >= 0; --grid_x) {
            for (int grid_y = COMBAT_GRID_VIEW_MAX_Y; grid_y >= 0; --grid_y) {
                int idx = grid_x * COMBAT_GRID_VIEW_HEIGHT + grid_y + 1;
                int16_t target_ship = -1;
                int16_t world_x = grid_x + MOX::_cur_combat_x;
                int16_t world_y = grid_y + MOX::_cur_combat_y;

                if (world_x >= 0 && world_x < COMBAT_GRID_WIDTH && world_y >= 0 && world_y < COMBAT_GRID_HEIGHT) {
                    target_ship = MOX::_combat_grid[world_x][world_y];
                }

                if (MOX::_skan_flag == 1) {
                    if (target_ship < 0 || target_ship >= MOX::_combat_total_ship_count) {
                        MOX::_combat_mouse_grid[idx].icon = 14;
                        MOX::_combat_mouse_grid[idx].frame = 12;
                    } else {
                        MOX::_combat_mouse_grid[idx].icon = 13;
                        MOX::_combat_mouse_grid[idx].frame = 12;
                    }
                } else if (MOX::_board_flag == 1) {
                    if (target_ship < 0 || target_ship >= MOX::_combat_total_ship_count ||
                        MOX::_combat_data[target_ship].owner == MOX::_combat_data[MOX::_cur_ship].owner) {
                        MOX::_combat_mouse_grid[idx].icon = 2;
                        MOX::_combat_mouse_grid[idx].frame = 12;
                    } else {
                        MOX::_combat_mouse_grid[idx].icon = 3;
                        MOX::_combat_mouse_grid[idx].frame = 12;
                    }
                } else {
                    if (target_ship < 0) {
                        MOX::_combat_mouse_grid[idx].icon = 2;
                        MOX::_combat_mouse_grid[idx].frame = 12;
                    } else if (target_ship >= MOX::_combat_total_ship_count) {
                        if (target_ship == 2000) {
                            int16_t facing = CMBTMOV1::Get_Facing_(
                                MOX::_combat_data[MOX::_cur_ship].x,
                                MOX::_combat_data[MOX::_cur_ship].y,
                                MOX::_cur_combat_x + grid_x,
                                MOX::_cur_combat_y + grid_y
                            );
                            MOX::_combat_mouse_grid[idx].icon = (facing / 2) + 4;
                            MOX::_combat_mouse_grid[idx].frame = 12;
                        }
                    } else {
                        if (MOX::_combat_data[target_ship].owner == MOX::_combat_data[MOX::_cur_ship].owner) {
                            if (target_ship == MOX::_cur_ship) {
                                MOX::_combat_mouse_grid[idx].icon = 2;
                                MOX::_combat_mouse_grid[idx].frame = 12;
                            } else {
                                MOX::_combat_mouse_grid[idx].icon = 1;
                                MOX::_combat_mouse_grid[idx].frame = 0;
                            }
                        } else {
                            if (CMBTMOV1::Ship_Is_In_Stasis_(target_ship) == 0) {
                                MOX::_combat_mouse_grid[idx].icon = 12;
                                MOX::_combat_mouse_grid[idx].frame = 12;
                            } else {
                                MOX::_combat_mouse_grid[idx].icon = 1;
                                MOX::_combat_mouse_grid[idx].frame = 0;
                            }
                        }
                    }
                }
            }
        }

        if (size_info > 0) {
            int idx_base = 558;
            int16_t x_val_base = size_info + 620;

            for (int grid_x = COMBAT_GRID_VIEW_MAX_X; grid_x >= 0; --grid_x) {
                int16_t y_end = size_info + 340;

                for (int grid_y = COMBAT_GRID_VIEW_MAX_Y; grid_y >= 0; --grid_y) {
                    int idx = idx_base + grid_y + 1;
                    int16_t icon = MOX::_combat_mouse_grid[idx].icon;

                    if (icon >= 4 && icon <= 11) {
                        if (size_info == 20) {
                            if (grid_x < COMBAT_GRID_VIEW_MAX_X && grid_y < COMBAT_GRID_VIEW_MAX_Y) {
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT + 1].icon = icon;
                            }
                        } else if (size_info == 10) {
                            int16_t icon19 = MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT + 1].icon;
                            if (icon19 < 4 || icon19 > 11) {
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT + 1].x1 = 0;
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT + 1].x2 = 0;
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT + 1].y1 = 0;
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT + 1].y2 = 0;
                            }

                            int16_t icon1 = MOX::_combat_mouse_grid[idx + 1].icon;
                            if (icon1 < 4 || icon1 > 11) {
                                MOX::_combat_mouse_grid[idx + 1].x1 = 0;
                                MOX::_combat_mouse_grid[idx + 1].x2 = 0;
                                MOX::_combat_mouse_grid[idx + 1].y1 = 0;
                                MOX::_combat_mouse_grid[idx + 1].y2 = 0;
                            }

                            int16_t icon18 = MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT].icon;
                            if (icon18 < 4 || icon18 > 11) {
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT].x1 = 0;
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT].x2 = 0;
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT].y1 = 0;
                                MOX::_combat_mouse_grid[idx + COMBAT_GRID_VIEW_HEIGHT].y2 = 0;
                            }
                        }

                        MOX::_combat_mouse_grid[idx].x1 = x_val_base;
                        MOX::_combat_mouse_grid[idx].x2 = x_val_base + 0x13;
                        MOX::_combat_mouse_grid[idx].y1 = y_end;
                        MOX::_combat_mouse_grid[idx].y2 = y_end + 0x13;
                    }
                    y_end -= 20;
                }
                x_val_base -= 20;
                idx_base -= COMBAT_GRID_VIEW_HEIGHT;
            }
        }

        mouse::Set_Mouse_List_(MOX::_combat_mouse_grid, 878);
    }

    void __cdecl Set_Cur_Ship_To_(int16_t ship_idx) {
        if (ship_idx == 0) {
            if (CMBTFIRE::Does_Combat_Planet_Have_Defenses_() <= 0) {
                return;
            }
        }

        int16_t cur_ptr_idx = -1;
        int16_t total_ships = MOX::_combat_total_ship_count;

        for (int16_t i = 0; i < total_ships; ++i) {
            int16_t order_val = MOX::_ship_turn_order[i];
            if (order_val == -10) {
                break;
            }
            if (ship_idx == order_val) {
                cur_ptr_idx = i;
            }
        }

        if (cur_ptr_idx == -1) {
            for (int16_t i = 0; i < total_ships; ++i) {
                if (MOX::_ship_turn_order[i] == -1) {
                    cur_ptr_idx = i;
                    MOX::_ship_turn_order[i] = ship_idx;
                    break;
                }
            }
        }

        MOX::_cur_ptr = cur_ptr_idx;
        MOX::_cur_ship = ship_idx;

        CMBTDRW1::Refresh_Combat_Screen_Full_();
    }

    int16_t __cdecl Enemy_Dissipator_Exists_(int16_t player_idx) {
        int16_t exists = 0;
        int16_t total_ships = MOX::_combat_total_ship_count;

        for (int16_t i = 0; i < total_ships; ++i) {
            if (!CMBTMOV1::Ship_Is_In_Stasis_(i) &&
                COMBINIT::Does_Combat_Ship_Have_Special_(i, SPECIAL_WARP_DISSIPATOR) != 0) {
                s_combat_data* ship = &MOX::_combat_data[i];

                if (ship->is_captured == 0 && ship->unit_status == 0 && ship->owner != player_idx) {
                    exists = 1;
                }
            }
        }

        return exists;
    }

    void __cdecl Assign_Combat_Grids_(int16_t active_ship_idx) {
        s_combat_data* ship;
        int16_t dim;
        int16_t i;
        int16_t j;

        for (i = 0; i < COMBAT_GRID_WIDTH; ++i) {
            for (j = 0; j < COMBAT_GRID_HEIGHT; ++j) {
                MOX::_combat_grid[i][j] = -1;
            }
        }

        if (MOX::_order_info_flag == 1) {
            for (int16_t ship_idx = 0; ship_idx < MOX::_combat_total_ship_count; ++ship_idx) {
                ship = &MOX::_combat_data[ship_idx];

                if (ship->unit_status == 0) {
                    MOX::_combat_grid[ship->x][ship->y] = ship_idx;

                    if (ship_idx != active_ship_idx || MOX::_skan_flag == 1 || MOX::_board_flag == 1) {
                        if (ship_idx == 0) {
                            if (MOX::_cmbt_colony_id == -1) {
                                dim = 6;
                            } else {
                                int8_t planet_size = MOX::_planet[MOX::_colony[MOX::_cmbt_colony_id].planet].size;
                                switch (planet_size) {
                                    case PLANET_SIZE_TINY: dim = 3; break;
                                    case PLANET_SIZE_SMALL: dim = 4; break;
                                    case PLANET_SIZE_MEDIUM: dim = 6; break;
                                    case PLANET_SIZE_LARGE: dim = 7; break;
                                    case PLANET_SIZE_HUGE: dim = 8; break;
                                    default: dim = 6; break;
                                }
                            }

                            for (i = 0; i < dim; ++i) {
                                for (j = 0; j < dim; ++j) {
                                    MOX::_combat_grid[ship->x + i][ship->y + j] = 0;
                                }
                            }
                        } else {
                            dim = (ship->size_class + 1) / 2;
                            if (dim == 1) {
                                for (i = 0; i < 2; ++i) {
                                    for (j = 0; j < 2; ++j) {
                                        MOX::_combat_grid[ship->x + i][ship->y + j] = ship_idx;
                                    }
                                }
                            } else if (dim > 1 && dim < 4) {
                                for (i = 0; i < 3; ++i) {
                                    for (j = 0; j < 3; ++j) {
                                        MOX::_combat_grid[ship->x + i][ship->y + j] = ship_idx;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (MOX::_cur_ship == active_ship_idx) {
            COMBAT1::Set_Legal_Moves_(active_ship_idx, MOX::_combat_data[active_ship_idx].movement_left, 1);
            COMBAT1::Assign_Mouse_Images_();
        }
    }

    void __cdecl Initiate_Boarding_Action_(int16_t attacker_idx, int16_t defender_idx) {
        bool can_board = true;
        int16_t range = CMBTAI::Range_To_Ship_(attacker_idx, defender_idx);
        int16_t has_transporters = COMBINIT::Does_Combat_Ship_Have_Special_(attacker_idx, SPECIAL_TRANSPORTERS);
        int16_t max_range = (has_transporters == 1) ? 4 : 1;
        s_combat_data* target = &MOX::_combat_data[defender_idx];
        char msg_buffer[80];
        int msg_idx = 0;

        if (target->owner > 8) {
            can_board = false;
            msg_idx = 0x3E; // Monsters cannot be boarded
        } else if (defender_idx == 0) {
            can_board = false;
            msg_idx = 0x3F; // Planets cannot be boarded
        } else if (MOX::_combat_data[MOX::_cur_ship].owner == target->owner) {
            can_board = false;
            msg_idx = (target->is_captured == 0) ? 0x40 : 0x41; // Cannot board own ship
        } else if (range > max_range) {
            can_board = false;
            msg_idx = 0x42; // Out of range
        } else if (max_range != 4 && COMBAT1::Calc_Current_Speed_(defender_idx) > 0) {
            can_board = false;
            msg_idx = 0x43; // Ship moving too fast
        } else if (MOX::_combat_data[MOX::_cur_ship].marine_count == 0) {
            can_board = false;
            msg_idx = 0x44; // No marines
        } else {
            const BOARDING::e_boarding_filter boarding_filter =
                BOARDING::Filter_Boarding_(attacker_idx, defender_idx);
            if (boarding_filter != BOARDING::e_boarding_filter::allowed) {
                can_board = false;
                const char* warning = nullptr;
                if (boarding_filter ==
                    BOARDING::e_boarding_filter::target_in_black_hole) {
                    warning = "Ships trapped in a black hole may not be boarded.";
                } else if (boarding_filter ==
                           BOARDING::e_boarding_filter::attacker_in_black_hole) {
                    warning = "Ships trapped in a black hole may not board.";
                }
                if (warning != nullptr) {
                    palette::Refresh_Palette_();
                    GENDRAW::Warning_Box_(warning);
                    CMBTDRW1::Refresh_Combat_Screen_Full_();
                }
            } else if (max_range == 4) {
                if (range > 1 || MOX::_combat_data[defender_idx].current_speed > 0) {
                    int16_t bearing = CMBTDRW1::Relative_Bearing_(defender_idx, attacker_idx);
                    int16_t shield_idx = CMBTFIRE::Facing_Shield_(bearing);
                    if (target->shield_arc_current[shield_idx] > 0) {
                        can_board = false;
                        KEN::Ken_Get_Text_Message_(0x45, msg_buffer, sizeof(msg_buffer));
                        palette::Refresh_Palette_();
                        GENDRAW::Warning_Box_(msg_buffer);
                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                    }
                    if (COMBINIT::Does_Combat_Ship_Have_Special_(defender_idx, SPECIAL_HARD_SHIELDS) != 0) {
                        can_board = false;
                        msg_idx = 0x46;
                    }
                }
            }
        }

        if (!can_board) {
            if (msg_idx != 0) {
                KEN::Ken_Get_Text_Message_((int16_t)msg_idx, msg_buffer, sizeof(msg_buffer));
                palette::Refresh_Palette_();
                GENDRAW::Warning_Box_(msg_buffer);
                CMBTDRW1::Refresh_Combat_Screen_Full_();
            }
        } else {
            int16_t marines_sent;
            int16_t result = CMBTFIR2::Init_Boarding_Action_(MOX::_cur_ship, &marines_sent);
            if (result != 0) {
                MOX::_combat_data[MOX::_cur_ship].marine_count -= (int8_t)marines_sent;

                if (max_range == 4 && (range != 1 || MOX::_combat_data[defender_idx].current_speed > 0)) {
                    if (COMBINIT::Does_Combat_Ship_Have_Special_(defender_idx, SPECIAL_DAMPER_FIELD) != 0) {
                        marines_sent = marines_sent / 3;
                    }
                }

                if (MOX::_net_flag != 0 && (result == 1 || result == 2)) {
                    int16_t msg_player = MOX::_combat_attacker_player;
                    if (MOX::_combat_data[attacker_idx].owner == MOX::_combat_attacker_player) {
                        msg_player = MOX::_combat_defender_player;
                    }
                    if (MOX::_PLAYER_NUM != msg_player) {
                        COMBAT1::Send_Tactical_Combat_Message_(msg_player, TACTICAL_NET_MSG_BOARDING_ACTION, defender_idx, marines_sent, result);
                    }
                }

                if (result == 1) { // Raid
                    if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 1) {
                        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                            CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 2);
                        }
                        MOX::_combat_data[MOX::_cur_ship].special_status_flag = 2;
                        Recalculate_Combat_Ship_(MOX::_cur_ship);
                    } else if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 4) {
                        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                            CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 5);
                        }
                        MOX::_combat_data[MOX::_cur_ship].special_status_flag = 6;
                    }
                    CMBTFIR2::Set_Boarded_Flag_(MOX::_cur_ship);
                    CMBTFIR2::Resolve_Raid_(MOX::_cur_ship, defender_idx, &marines_sent);
                    MOX::_combat_data[MOX::_cur_ship].marine_count += (int8_t)marines_sent;
                } else if (result == 2) { // Capture
                    if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 1) {
                        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                            CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 2);
                        }
                        MOX::_combat_data[MOX::_cur_ship].special_status_flag = 2;
                        Recalculate_Combat_Ship_(MOX::_cur_ship);
                    } else if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 4) {
                        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                            CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 5);
                        }
                        MOX::_combat_data[MOX::_cur_ship].special_status_flag = 6;
                    }
                    CMBTFIR2::Set_Boarded_Flag_(MOX::_cur_ship);
                    CMBTFIR2::Resolve_Capture_(MOX::_cur_ship, defender_idx, marines_sent, target->x * 20, target->y * 20);
                }

                MOX::_board_flag = 0;
                if (MOX::_net_flag != 0) {
                    NETMOX::Net_Debug_Log_("Wait_Until_Net_Opponent_Finished_: caller=%s:%d", __FILE__, __LINE__);
                    CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                }
            }
        }
    }

    void __cdecl Seeking_Missiles_(int16_t player_idx) {
        int16_t resolved_missile_idx[300];
        int16_t resolved_target_idx[300];
        int16_t is_finished;
        int16_t resolve_count;
        int16_t target_x, target_y;

        do {
            is_finished = 1;
            resolve_count = 0;
            int16_t screen_updated = 0;

            for (int16_t i = 0; i < 300; ++i) {
                s_missile* missile = &MOX::_missile[i];
                int16_t target_idx = missile->target_unit_idx;

                if (missile->type <= 0 || missile->travel_dist <= 0 || missile->is_anti_missile_rocket != 0) {
                    continue;
                }

                if (MOX::_combat_init != 0) {
                    if (missile->fighter_count == -1) {
                        if (MOX::_combat_init != 0) continue;
                    }
                } else {
                    if (MOX::_combat_data[target_idx].owner != player_idx) {
                        if (player_idx != MOX::_combat_defender_player || missile->source_unit_idx != target_idx || 
                            MOX::_combat_data[missile->source_unit_idx].owner == player_idx) {
                            continue;
                        }
                    }
                }

                if (missile->is_anti_missile_rocket == 0) {
                    COMBINIT::Ship_Center_Offsets_(target_idx, &target_x, &target_y);
                    target_x += MOX::_combat_data[target_idx].x * 20;
                    target_y += MOX::_combat_data[target_idx].y * 20;
                } else {
                    target_x = MOX::_missile[target_idx].x;
                    target_y = MOX::_missile[target_idx].y;
                }

                uint8_t resolve_flag = CMBTMIS::Move_Missile_(i, target_x, target_y, missile->speed);

                if (CMBTMIS::Missile_On_Combat_Screen_(i)) {
                    screen_updated = 1;
                }

                if (missile->x != target_x || missile->y != target_y) {
                    is_finished = 0;
                }

                if (resolve_flag == 1) {
                    if (TECHDATA::_weapons[missile->type].type != WEAPON_TYPE_FIGHTER || missile->is_active == 1) {
                        resolved_missile_idx[resolve_count] = i;
                        resolved_target_idx[resolve_count] = target_idx;
                        resolve_count++;
                    }
                }
            }

            if (screen_updated != 0) {
                video::Set_Page_Off_();
                CMBTDRW1::Draw_Main_Combat_Screen_();
                RUSS::Mox_Sync_Update_();
            }

            for (int16_t j = 0; j < resolve_count; ++j) {
                int16_t m_idx = resolved_missile_idx[j];
                int16_t t_idx = resolved_target_idx[j];
                s_missile* missile = &MOX::_missile[m_idx];
                s_combat_data* target_ship = &MOX::_combat_data[t_idx];

                if (MOX::_missile[m_idx].target_unit_idx == t_idx) {
                    if (target_ship->unit_status == 0) {
                        int16_t owner = CMBTMIS::Missile_Owner_(m_idx);
                        if (owner != target_ship->owner || missile->source_unit_idx == t_idx) {
                            if (MOX::_missile[j].is_anti_missile_rocket == 0) {
                                COMBINIT::Ship_Center_Offsets_(t_idx, &target_x, &target_y);
                                target_x += target_ship->x * 20;
                                target_y += target_ship->y * 20;
                            } else {
                                target_x = MOX::_missile[t_idx].x;
                                target_y = MOX::_missile[t_idx].y;
                            }
                            MOX::_cur_ship = t_idx;
                            CMBTMIS::Resolve_Missile_(m_idx, target_x, target_y);
                        } else {
                            CMBTMIS::Kill_Missile_(m_idx);
                        }
                    } else {
                        CMBTMIS::Kill_Missile_(m_idx);
                    }
                }
            }
        } while (is_finished == 0);

        for (int16_t k = 0; k < 300; ++k) {
            s_missile* missile = &MOX::_missile[k];
            int16_t target_idx = missile->target_unit_idx;

            if (missile->type < 1 || TECHDATA::_weapons[missile->type].type == WEAPON_TYPE_FIGHTER || 
                missile->fighter_count == -1 || missile->is_anti_missile_rocket != 0) {
                if (missile->fighter_count == -1) {
                    missile->fighter_count = 0;
                }
            } else {
                bool should_increment = false;
                if (MOX::_combat_init == 0) {
                    if (MOX::_combat_data[target_idx].owner != player_idx) {
                        if (player_idx == MOX::_combat_defender_player && missile->source_unit_idx == target_idx && 
                            MOX::_combat_data[missile->source_unit_idx].owner != player_idx) {
                            should_increment = true;
                        }
                    } else {
                        should_increment = true;
                    }
                } else {
                    should_increment = true;
                }

                if (should_increment) {
                    missile->fighter_count++;
                }
            }
        }
    }

    int16_t __cdecl Ship_Initiative_Value_(int16_t ship_idx) {
        int16_t initiative = -5;

        if (ship_idx != -1) {
            s_combat_data* ship = &MOX::_combat_data[ship_idx];
            initiative = (ship->tac_val_attack / 10) + ship->current_speed;
        }

        return initiative;
    }

    void __cdecl Set_Legal_Moves_(int16_t ship_idx, int16_t range, int16_t check_cost) {
        int16_t footprint;
        int16_t ship_x;
        int16_t ship_y;
        int16_t x_pixel;
        int16_t y_pixel;

        if (((COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_SUB_SPACE_TELEPORTER) ||
              COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_PHASE_SHIFTER)) &&
             CMBTFIR2::Used_Teleporter_(ship_idx) == 0) &&
            (range < 20)) {
            range = 20;
        }

        s_combat_data* ship = &MOX::_combat_data[ship_idx];
        footprint = (ship->size_class + 1) / 2;
        if (footprint > 2) {
            footprint = 2;
        }

        ship_x = ship->x;
        ship_y = ship->y;
        x_pixel = 0;

        for (int16_t x = 0; x < COMBAT_GRID_WIDTH; ++x) {
            y_pixel = 0;
            for (int16_t y = 0; y < COMBAT_GRID_HEIGHT; ++y) {
                int16_t* grid_cell = &MOX::_combat_grid[x][y];

                if (*grid_cell == -1) {
                    int16_t dx = ship_x - x;
                    if (dx < 0) dx = -dx;

                    int16_t dy = ship_y - y;
                    if (dy < 0) dy = -dy;

                    if (dx <= range && dy <= range) {
                        *grid_cell = 2000;

                        if (footprint == 1) {
                            for (int16_t f_dx = 0; f_dx < 2; ++f_dx) {
                                for (int16_t f_dy = 0; f_dy < 2; ++f_dy) {
                                    if ((x + f_dx >= COMBAT_GRID_WIDTH) || (y + f_dy >= COMBAT_GRID_HEIGHT)) {
                                        *grid_cell = -1;
                                    } else {
                                        int16_t cell_val = MOX::_combat_grid[x + f_dx][y + f_dy];
                                        if (cell_val != -1 && cell_val != 2000 && cell_val != ship_idx) {
                                            *grid_cell = -1;
                                        }
                                    }
                                }
                            }
                        } else if (footprint == 2) {
                            for (int16_t f_dx = 0; f_dx <= 2; ++f_dx) {
                                for (int16_t f_dy = 0; f_dy <= 2; ++f_dy) {
                                    if ((x + f_dx >= COMBAT_GRID_WIDTH) || (y + f_dy >= COMBAT_GRID_HEIGHT)) {
                                        *grid_cell = -1;
                                    } else {
                                        int16_t cell_val = MOX::_combat_grid[x + f_dx][y + f_dy];
                                        if (cell_val != -1 && cell_val != 2000 && cell_val != ship_idx) {
                                            *grid_cell = -1;
                                        }
                                    }
                                }
                            }
                        }

                        if ((COMBAT_GRID_WIDTH - footprint < x) || (COMBAT_GRID_HEIGHT - footprint < y)) {
                            *grid_cell = -1;
                        }

                        if (*grid_cell == 2000 && check_cost == 1) {
                            int16_t cost = CMBTMOV1::Move_Cost_(ship_idx, x, y);
                            if (ship->movement_left < cost) {
                                *grid_cell = -1;
                            }

                            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_SUB_SPACE_TELEPORTER)) {
                                if (*grid_cell == -1 && CMBTFIR2::Used_Teleporter_(ship_idx) == 0) {
                                    if (CMBTFIRE::Special_Is_Turned_On_(ship_idx, SPECIAL_SUB_SPACE_TELEPORTER) == 1) {
                                        if (CMBTAI::Range_To_XY_(ship_idx, x_pixel, y_pixel) <= 20) {
                                            *grid_cell = 2000;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                y_pixel += 20;
            }
            x_pixel += 20;
        }
    }

    void __cdecl Set_Combat_Help_Fields_() {
        int16_t entry_idx = 0;

        if (MOX::_order_info_flag == 1) {
            MOX::_help_entry_list[0].help_id = 0x269;
            MOX::_help_entry_list[0].x1 = 0x124;
            MOX::_help_entry_list[0].y1 = 0x1be;
            MOX::_help_entry_list[0].x2 = 0x176;
            MOX::_help_entry_list[0].y2 = 0x1d8;
            
            MOX::_help_entry_list[1].help_id = 0x26f;
            MOX::_help_entry_list[1].x1 = 0x10f;
            MOX::_help_entry_list[1].y1 = 0x16a;
            MOX::_help_entry_list[1].x2 = 0x14a;
            MOX::_help_entry_list[1].y2 = 0x182;
            
            MOX::_help_entry_list[2].help_id = 0x270;
            MOX::_help_entry_list[2].x1 = 0x14c;
            MOX::_help_entry_list[2].y1 = 0x16a;
            MOX::_help_entry_list[2].x2 = 0x18a;
            MOX::_help_entry_list[2].y2 = 0x182;
            
            MOX::_help_entry_list[3].help_id = 0x271;
            MOX::_help_entry_list[3].x1 = 0x10f;
            MOX::_help_entry_list[3].y1 = 0x185;
            MOX::_help_entry_list[3].x2 = 0x14a;
            MOX::_help_entry_list[3].y2 = 0x19e;
            
            MOX::_help_entry_list[4].help_id = 0x272;
            MOX::_help_entry_list[4].x1 = 0x14d;
            MOX::_help_entry_list[4].y1 = 0x185;
            MOX::_help_entry_list[4].x2 = 0x18a;
            MOX::_help_entry_list[4].y2 = 0x19e;
            
            MOX::_help_entry_list[5].help_id = 0x273;
            MOX::_help_entry_list[5].x1 = 0x10f;
            MOX::_help_entry_list[5].y1 = 0x1a2;
            MOX::_help_entry_list[5].x2 = 0x14a;
            MOX::_help_entry_list[5].y2 = 0x1b9;
            
            MOX::_help_entry_list[6].help_id = 0x274;
            MOX::_help_entry_list[6].x1 = 0x14d;
            MOX::_help_entry_list[6].y1 = 0x1a2;
            MOX::_help_entry_list[6].x2 = 0x18a;
            MOX::_help_entry_list[6].y2 = 0x1b9;
            entry_idx = 7;
        } else {
            MOX::_help_entry_list[0].help_id = 0x2b7;
            MOX::_help_entry_list[0].x1 = 0x11a;
            MOX::_help_entry_list[0].y1 = 0x15e;
            MOX::_help_entry_list[0].x2 = 0x180;
            MOX::_help_entry_list[0].y2 = 0x173;
            
            MOX::_help_entry_list[1].help_id = 0x2b8;
            MOX::_help_entry_list[1].x1 = 0x108;
            MOX::_help_entry_list[1].y1 = 0x17f;
            MOX::_help_entry_list[1].x2 = 0x197;
            MOX::_help_entry_list[1].y2 = 0x18c;
            
            MOX::_help_entry_list[2].help_id = 0x2b9;
            MOX::_help_entry_list[2].x1 = 0x108;
            MOX::_help_entry_list[2].y1 = 0x18f;
            MOX::_help_entry_list[2].x2 = 0x197;
            MOX::_help_entry_list[2].y2 = 0x19c;
            
            MOX::_help_entry_list[3].help_id = 0x2ba;
            MOX::_help_entry_list[3].x1 = 0x108;
            MOX::_help_entry_list[3].y1 = 0x19f;
            MOX::_help_entry_list[3].x2 = 0x197;
            MOX::_help_entry_list[3].y2 = 0x1ac;
            
            MOX::_help_entry_list[4].help_id = 0x2bb;
            MOX::_help_entry_list[4].x1 = 0x108;
            MOX::_help_entry_list[4].y1 = 0x1af;
            MOX::_help_entry_list[4].x2 = 0x197;
            MOX::_help_entry_list[4].y2 = 0x1bc;
            
            MOX::_help_entry_list[5].help_id = 0x2bc;
            MOX::_help_entry_list[5].x1 = 0x108;
            MOX::_help_entry_list[5].y1 = 0x1bf;
            MOX::_help_entry_list[5].x2 = 0x197;
            MOX::_help_entry_list[5].y2 = 0x1cc;
            entry_idx = 6;
        }

        MOX::_help_entry_list[entry_idx].help_id = 0x26a;
        MOX::_help_entry_list[entry_idx].x1 = 0x0a;
        MOX::_help_entry_list[entry_idx].y1 = 0x168;
        MOX::_help_entry_list[entry_idx].x2 = 0x6a;
        MOX::_help_entry_list[entry_idx].y2 = 0x1d7;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].x1 = 0x1f6;
        MOX::_help_entry_list[entry_idx].y1 = 0x168;
        MOX::_help_entry_list[entry_idx].x2 = 0x272;
        MOX::_help_entry_list[entry_idx].y2 = 0x1d7;
        MOX::_help_entry_list[entry_idx].help_id = 0x26d;
        if (MOX::_missile_warning != 1) {
            MOX::_help_entry_list[entry_idx].help_id = 0x26c;
        }
        entry_idx++;

        MOX::_help_entry_list[entry_idx].help_id = 0x26e;
        MOX::_help_entry_list[entry_idx].x1 = 0x18f;
        MOX::_help_entry_list[entry_idx].y1 = 0x168;
        MOX::_help_entry_list[entry_idx].x2 = 0x1ef;
        MOX::_help_entry_list[entry_idx].y2 = 0x1d7;
        entry_idx++;

        if (MOX::_ship_status_display == 0) {
            for (int j = 0; j < 8; j++) {
                uint16_t tech_app_id = TECHDATA::_weapons[MOX::_combat_data[MOX::_cur_ship].weapons[j].weapon_id].tech_app_id;
                if (tech_app_id != 0) {
                    MOX::_help_entry_list[entry_idx + j].help_id = tech_app_id;
                } else {
                    MOX::_help_entry_list[entry_idx + j].help_id = -1;
                }
            }
        } else {
            int special_count = 0;
            for (int j = 0; j < 39; j++) {
                if (struct_::Test_Bit_Field_(MOX::_combat_data[MOX::_cur_ship].special_device_flags, j) == 1) {
                    MOX::_help_entry_list[entry_idx + special_count].help_id = TECHDATA::_specials[j].tech_app_id;
                    special_count++;
                }
            }
            for (int k = 0; k < 8; k++) {
                MOX::_help_entry_list[entry_idx + special_count + k].help_id = -1;
            }
        }

        MOX::_help_entry_list[entry_idx].x1 = 0x72;
        MOX::_help_entry_list[entry_idx].y1 = 0x17b;
        MOX::_help_entry_list[entry_idx].x2 = 0x108;
        MOX::_help_entry_list[entry_idx].y2 = 0x185;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].x1 = 0x72;
        MOX::_help_entry_list[entry_idx].y1 = 0x186;
        MOX::_help_entry_list[entry_idx].x2 = 0x108;
        MOX::_help_entry_list[entry_idx].y2 = 0x190;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].x1 = 0x72;
        MOX::_help_entry_list[entry_idx].y1 = 0x191;
        MOX::_help_entry_list[entry_idx].x2 = 0x108;
        MOX::_help_entry_list[entry_idx].y2 = 0x19b;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].x1 = 0x72;
        MOX::_help_entry_list[entry_idx].y1 = 0x19c;
        MOX::_help_entry_list[entry_idx].x2 = 0x108;
        MOX::_help_entry_list[entry_idx].y2 = 0x1a6;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].x1 = 0x72;
        MOX::_help_entry_list[entry_idx].y1 = 0x1a7;
        MOX::_help_entry_list[entry_idx].x2 = 0x108;
        MOX::_help_entry_list[entry_idx].y2 = 0x1b1;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].x1 = 0x72;
        MOX::_help_entry_list[entry_idx].y1 = 0x1b2;
        MOX::_help_entry_list[entry_idx].x2 = 0x108;
        MOX::_help_entry_list[entry_idx].y2 = 0x1bc;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].x1 = 0x72;
        MOX::_help_entry_list[entry_idx].y1 = 0x1bd;
        MOX::_help_entry_list[entry_idx].x2 = 0x108;
        MOX::_help_entry_list[entry_idx].y2 = 0x1c7;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].x1 = 0x72;
        MOX::_help_entry_list[entry_idx].y1 = 0x1c8;
        (void)(MOX::_ship_status_display == 0);
        MOX::_help_entry_list[entry_idx].x2 = 0x108;
        MOX::_help_entry_list[entry_idx].y2 = 0x1d3;
        entry_idx++;

        MOX::_help_entry_list[entry_idx].help_id = 0x275;
        MOX::_help_entry_list[entry_idx].x1 = 0x71;
        MOX::_help_entry_list[entry_idx].y1 = 0x168;
        MOX::_help_entry_list[entry_idx].x2 = 0x109;
        MOX::_help_entry_list[entry_idx].y2 = 0x1d7;
        entry_idx++;

        fields::Set_Help_List_(MOX::_help_entry_list, entry_idx);
    }

    bool __cdecl Weapon_Is_On_(int16_t ship_idx, int16_t weapon_idx) {
        return MOX::_combat_data[ship_idx].weapons[weapon_idx].active == 1;
    }
}
