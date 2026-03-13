#include "pch.h"

namespace COMBAT1 {

    void __cdecl Do_Combat_Turn_(int16_t param_1, int16_t param_2) {
        MOX::_player_mode[MOX::_combat_attacker_player] = CMBTAI::Get_Player_Mode_(MOX::_combat_attacker_player);
        MOX::_player_mode[MOX::_combat_defender_player] = CMBTAI::Get_Player_Mode_(MOX::_combat_defender_player);

        for (int i = 0; i < 15; ++i) {
            MOX::_side_end_of_turn[i] = 0;
        }

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (Combat_Ship_Is_Functional_(i)) {
                    if (param_2 == 0 || COMBINIT::Does_Combat_Ship_Have_Special_(i, 0x22)) {
                        if (MOX::_combat_init == 0 || MOX::_combat_data[i].owner == param_1) {
                            Init_Ship_For_Start_Of_Turn_(i);
                        }
                    }
                }
            }
        }

        RUSS::Mox_Update_();

        int16_t captured_count = 0;
        int16_t order_idx = 0;
        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].unit_status == 0 &&
                    CMBTMOV1::Ship_Is_In_Stasis_(i) == 0 &&
                    (param_2 == 0 || COMBINIT::Does_Combat_Ship_Have_Special_(i, 0x22) != 0) &&
                    (MOX::_combat_init == 0 || MOX::_combat_data[i].owner == param_1)) {
                    
                    if (MOX::_combat_data[i].is_captured == 0) {
                        MOX::_ship_turn_order[order_idx] = i;
                    } else {
                        MOX::_ship_turn_order[order_idx] = -1;
                        captured_count++;
                    }
                    order_idx++;
                }
            }
        }

        if (order_idx != captured_count) {
            if (MOX::_combat_init != 0) {
                _qsort(MOX::_ship_turn_order, order_idx, 2, qsort_ship_initiative_);
            }
            
            MOX::_ship_turn_order[order_idx] = -10;
            for (int i = order_idx + 1; i < 210; ++i) {
                MOX::_ship_turn_order[i] = -1;
            }

            RUSS::Mox_Update_();
            MOX::_start_of_turn = 1;
            MOX::_cur_ptr = -1;
            int16_t next_ship_status = Next_Ship_(param_1);

            if (next_ship_status != 1) {
                fields::Assign_Auto_Function_(CMBTDRW1::Draw_Main_Combat_Screen_, 2);
                MOX::_skan_flag = 0;
                MOX::_board_flag = 0;
                RUSS::Mox_Update_();

                if (MOX::_combat_total_ship_count > 0) {
                    for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                        if (MOX::_combat_data[i].drive_destroyed == 0 &&
                            MOX::_combat_data[i].unit_status == 0 &&
                            CMBTMOV1::Ship_Is_In_Stasis_(i) == 0) {
                            
                            int16_t rot = (MOX::_combat_data[i].facing_dir + 2) % 16;
                            CMBTMOV1::Rotate_Ship_(i, rot, 
                                                   (MOX::_combat_data[i].x - MOX::_cur_combat_x) * 20, 
                                                   (MOX::_combat_data[i].y - MOX::_cur_combat_y) * 20);
                        }
                    }
                }

                MOX::_start_of_turn = 0;

                while (MOX::_ship_turn_order[MOX::_cur_ptr] >= 0) {
                    if (next_ship_status != 0) break;

                    timer::Mark_Time_();
                    MOX::_FULL_DRAW_SCREEN_FLAG = 0;
                    int16_t local_60 = MOX::_combat_data[MOX::_cur_ship].movement_left;

                    if (Ship_Controlled_By_Computer_(MOX::_cur_ship)) {
                        if (MOX::_net_flag != 0) {
                            if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_PLAYER_NUM && 
                                MOX::_PLAYER_NUM != MOX::_net_destination) {
                                Send_Tactical_Combat_Message_(MOX::_net_destination, 10001, 0, 0, 0);
                            }
                        }
                        CMBTAI::Do_Auto_Ship_Turn_();
                        
                        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                            Add_Tactical_Combat_Fields_();
                            Assign_Combat_Grids_(MOX::_cur_ship);
                            CMBTDRW1::Refresh_Combat_Screen_Full_();
                        }
                        
                        MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                        next_ship_status = Next_Ship_(param_1);
                        
                        if (MOX::_net_flag != 0) {
                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                        }
                        continue;
                    }

                    MOX::_cur_ship = MOX::_ship_turn_order[MOX::_cur_ptr];
                    if (CMBTMOV1::Ship_Is_In_Stasis_(MOX::_cur_ship) == 0) {
                        if (MOX::_combat_init == 0 || MOX::_combat_data[MOX::_cur_ship].owner == param_1) {
                            if (MOX::_net_flag == 0 || MOX::_combat_data[MOX::_cur_ship].owner == MOX::_PLAYER_NUM) {
                                int32_t action = 0;
                                do {
                                    if (MOX::_tactical_net_choice == 0) {
                                        timer::Mark_Time_();
                                        if (MOX::_net_flag != 0) {
                                            action = fields::Get_Input_();
                                            if (action == MOX::_auto_button) {
                                                CMBTAI::Toggle_Auto_Combat_Flags_();
                                            }
                                        }
                                        video::Set_Page_Off_();
                                        CMBTDRW1::Draw_Main_Combat_Screen_();
                                        RUSS::Mox_Sync_Update_();
                                        action = fields::Get_Input_();
                                        timer::Release_Time_(1);
                                    } else {
                                        action = 1;
                                        COMBINIT::Snap_Center_Combat_Screen_(MOX::_new_message.f0 + 16, MOX::_new_message.f2 + 9);
                                        int16_t msg_f12 = MOX::_new_message.f12;
                                        int16_t msg_f10 = MOX::_new_message.f10;
                                        int16_t msg_f14 = MOX::_new_message.f14;
                                        int16_t msg_f8 = MOX::_new_message.f8;
                                        random::Set_Random_Seed_(MOX::_new_message.f4);
                                        struct_::Clear_Structure_(&MOX::_new_message, 16);

                                        switch (msg_f8) {
                                            case 0x2710: {
                                                MOX::_combat_data[MOX::_cur_ship].drive_current_hits = 0;
                                                uint8_t was_destroyed = MOX::_combat_data[MOX::_cur_ship].drive_destroyed;
                                                MOX::_combat_data[MOX::_cur_ship].drive_destroyed = 1;
                                                CMBTFIRE::Destroy_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship));
                                                MOX::_combat_data[MOX::_cur_ship].drive_destroyed = was_destroyed;
                                                MOX::_tactical_net_choice = 10000;
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_status = Next_Ship_(param_1);
                                                break;
                                            }
                                            case 0x2711:
                                                CMBTAI::Do_Auto_Ship_Turn_();
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_status = Next_Ship_(param_1);
                                                break;
                                            case 0x2712:
                                            case 0x2713:
                                            case 0x2714:
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_status = Next_Ship_(param_1);
                                                break;
                                            case 0x2715: {
                                                int8_t cur_owner = MOX::_combat_data[MOX::_cur_ship].owner;
                                                int16_t cost = CMBTMOV1::Move_Cost_(MOX::_cur_ship, msg_f10, msg_f12);
                                                if (MOX::_combat_data[MOX::_cur_ship].movement_left < 0) {
                                                    MOX::_combat_data[MOX::_cur_ship].movement_left = 0;
                                                }
                                                int16_t was_teleporter = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                int16_t dist = CMBTMOV1::Move_Ship_(MOX::_cur_ship, msg_f10, msg_f12, cost);
                                                if (was_teleporter == CMBTFIR2::Used_Teleporter_(MOX::_cur_ship)) {
                                                    MOX::_combat_data[MOX::_cur_ship].movement_left -= dist;
                                                }
                                                CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                if (Ship_Done_Check_() != 0 || MOX::_combat_data[MOX::_cur_ship].owner != cur_owner) {
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_status = Next_Ship_(param_1);
                                                }
                                                break;
                                            }
                                            case 0x2716: {
                                                int16_t target_x, target_y;
                                                if (msg_f12 == 0) {
                                                    target_x = MOX::_combat_data[msg_f10].x * 20;
                                                    target_y = MOX::_combat_data[msg_f10].y * 20;
                                                } else {
                                                    target_x = MOX::_missile[msg_f10].x;
                                                    target_y = MOX::_missile[msg_f10].y;
                                                }
                                                CMBTFIRE::Fire_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, msg_f10, target_x, target_y, msg_f12);
                                                Add_Tactical_Combat_Fields_();
                                                Assign_Combat_Grids_(MOX::_cur_ship);
                                                for (int16_t j = 0; j < 8; ++j) {
                                                    if (MOX::_combat_data[MOX::_cur_ship].weapons[j].active == 0) {
                                                        MOX::_combat_data[MOX::_cur_ship].weapons[j].active = 1;
                                                    }
                                                }
                                                break;
                                            }
                                            case 0x2717:
                                                MOX::_missile_warning = (MOX::_missile_warning + 1) % 3;
                                                Add_Tactical_Combat_Fields_();
                                                Assign_Combat_Grids_(MOX::_cur_ship);
                                                break;
                                            case 0x2718:
                                                fields::Clear_Fields_();
                                                MOX::_auto_button = fields::Add_Hidden_Field_(0, 0, 639, 479, reinterpret_cast<const char*>(std::uintptr_t{0x00551100}), 0);
                                                if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_attacker_player) {
                                                    MOX::_auto_combat_flag[0] = 1 - MOX::_auto_combat_flag[0];
                                                    MOX::_auto_combat = MOX::_auto_combat_flag[0];
                                                } else {
                                                    MOX::_auto_combat_flag[1] = 1 - MOX::_auto_combat_flag[1];
                                                    MOX::_auto_combat = MOX::_auto_combat_flag[1];
                                                }
                                                break;
                                            case 0x2719:
                                                next_ship_status = Next_Ship_(param_1);
                                                break;
                                            case 0x271a: {
                                                MOX::_combat_data[MOX::_cur_ship].weapons[msg_f10].active--;
                                                if (MOX::_combat_data[MOX::_cur_ship].weapons[msg_f10].active < -1) {
                                                    MOX::_combat_data[MOX::_cur_ship].weapons[msg_f10].active = 1;
                                                }
                                                if (MOX::_combat_data[MOX::_cur_ship].weapons[msg_f10].active != 1 && MOX::_combat_data[MOX::_cur_ship].weapons[msg_f10].weapon_id == 0x27) {
                                                    MOX::_combat_data[MOX::_cur_ship].special_cooldown = -1;
                                                }
                                                break;
                                            }
                                            case 0x271b:
                                                MOX::_combat_data[MOX::_cur_ship].is_retreating = 1;
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_status = Next_Ship_(param_1);
                                                break;
                                            case 0x271c:
                                                CMBTMOV1::Turn_To_Face_(MOX::_cur_ship, msg_f10, msg_f12);
                                                break;
                                            case 0x271d:
                                                CMBTFIRE::Remove_Stasis_Beam_(msg_f10);
                                                break;
                                            case 0x271e:
                                                if (MOX::_combat_init == 0) {
                                                    Set_Cur_Ship_To_(msg_f10);
                                                }
                                                break;
                                            case 0x271f:
                                                if (msg_f14 == 1) {
                                                    if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 1) {
                                                        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                                                            CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 2);
                                                        }
                                                        MOX::_combat_data[MOX::_cur_ship].special_status_flag = 2;
                                                    } else if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 4) {
                                                        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                                                            CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 5);
                                                        }
                                                        MOX::_combat_data[MOX::_cur_ship].special_status_flag = 6;
                                                    }
                                                    Set_Boarded_Flag_(MOX::_cur_ship);
                                                    MOX::_combat_data[MOX::_cur_ship].marine_count -= static_cast<int8_t>(msg_f12);
                                                    CMBTFIR2::Resolve_Raid_(MOX::_cur_ship, msg_f10, &msg_f12);
                                                    MOX::_combat_data[MOX::_cur_ship].marine_count += static_cast<int8_t>(msg_f12);
                                                } else {
                                                    if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 1) {
                                                        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                                                            CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 2);
                                                        }
                                                        MOX::_combat_data[MOX::_cur_ship].special_status_flag = 2;
                                                    } else if (MOX::_combat_data[MOX::_cur_ship].special_status_flag == 4) {
                                                        if (MOX::_apply_damage_to_planet == 1 && MOX::_super_fast_flag == 0) {
                                                            CMBTSPEC::Draw_Cloak_(MOX::_cur_ship, 5);
                                                        }
                                                        MOX::_combat_data[MOX::_cur_ship].special_status_flag = 6;
                                                    }
                                                    Set_Boarded_Flag_(MOX::_cur_ship);
                                                    MOX::_combat_data[MOX::_cur_ship].marine_count -= static_cast<int8_t>(msg_f12);
                                                    CMBTFIR2::Resolve_Capture_(MOX::_cur_ship, msg_f10, msg_f12, MOX::_combat_data[msg_f10].x * 20, MOX::_combat_data[msg_f10].y * 20);
                                                }
                                                break;
                                            case 0x2720:
                                                if (MOX::_combat_init == 0) {
                                                    next_ship_status = 1;
                                                } else {
                                                    MOX::_side_end_of_turn[MOX::_combat_data[MOX::_cur_ship].owner] = 1;
                                                    next_ship_status = Next_Ship_(param_1);
                                                }
                                                break;
                                        }
                                        MOX::_tactical_net_choice = 0;
                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                    }
                                } while (action == 0);
                            } else {
                                int32_t action = 0;
                                if (CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship) != 0) {
                                    action = MOX::_done_button;
                                } else {
                                    action = fields::Get_Input_();
                                }

                                for (int16_t j = 0; j < 300; ++j) {
                                    if (action == MOX::_missile_field[j]) {
                                        if (CMBTMIS::Missile_Owner_(j) != MOX::_combat_data[MOX::_cur_ship].owner) {
                                            if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                Send_Tactical_Combat_Message_(MOX::_net_destination, 10006, j, 1, 0);
                                            }
                                            CMBTFIRE::Fire_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, j, MOX::_missile[j].x, MOX::_missile[j].y, 1);
                                            Add_Tactical_Combat_Fields_();
                                            Assign_Combat_Grids_(MOX::_cur_ship);
                                            for (int16_t k = 0; k < 8; ++k) {
                                                if (MOX::_combat_data[MOX::_cur_ship].weapons[k].active == 0) {
                                                    MOX::_combat_data[MOX::_cur_ship].weapons[k].active = 1;
                                                }
                                            }
                                            if (MOX::_net_flag != 0) {
                                                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                            }
                                            action = 0;
                                        }
                                    }
                                }

                                if (action == MOX::_msl_impact_warning_button) {
                                    MOX::_saved_msl_warning = 1 - MOX::_msl_impact_warning_flag;
                                    MOX::_msl_impact_warning_flag = MOX::_saved_msl_warning;
                                    if (MOX::_net_flag != 0) {
                                        NETMOX::Mox_Send_Message_(MOX::_player[MOX::_net_destination].network_player_id, 0x3eb, &MOX::_msl_impact_warning_flag, 2);
                                    }
                                    action = 0;
                                }

                                if (action == MOX::_speedx2_button) {
                                    MOX::_speedx2_flag = 1 - MOX::_speedx2_flag;
                                    action = 0;
                                }

                                if (action == MOX::_grid_button) {
                                    MOX::_grid_flag = 1 - MOX::_grid_flag;
                                    CMBTDRW1::Refresh_Combat_Screen_Full_();
                                    action = 0;
                                }

                                if (action == MOX::_draw_legal_boxes_button) {
                                    MOX::_draw_legal_boxes_flag = 1 - MOX::_draw_legal_boxes_flag;
                                    CMBTDRW1::Refresh_Combat_Screen_Full_();
                                    action = 0;
                                }

                                if (action == MOX::_shields_button) {
                                    MOX::_shields_flag = 1 - MOX::_shields_flag;
                                    action = 0;
                                }

                                if (action == MOX::_super_fast_button) {
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

                                if (action == MOX::_self_destruct_button) {
                                    char text[80];
                                    if (MOX::_cur_ship == 0) {
                                        KEN::Ken_Get_Text_Message_(0x38, text, 80);
                                        palette::Refresh_Palette_();
                                        GENDRAW::Message_Box_(text);
                                    } else {
                                        KEN::Ken_Get_Text_Message_(0x39, text, 80);
                                        palette::Refresh_Palette_();
                                        if (GENDRAW::Warning_Box2_(text) != 0) {
                                            MOX::_order_info_flag = 1 - MOX::_order_info_flag;
                                            CMBTDRW1::Move_Options_Background_();
                                            if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                Send_Tactical_Combat_Message_(MOX::_net_destination, 10000, 0, 0, 0);
                                            }
                                            MOX::_combat_data[MOX::_cur_ship].drive_current_hits = 0;
                                            uint8_t was_destroyed = MOX::_combat_data[MOX::_cur_ship].drive_destroyed;
                                            MOX::_combat_data[MOX::_cur_ship].drive_destroyed = 1;
                                            CMBTFIRE::Destroy_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship));
                                            MOX::_combat_data[MOX::_cur_ship].drive_destroyed = was_destroyed;
                                            MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                            next_ship_status = Next_Ship_(param_1);
                                            if (MOX::_net_flag != 0) {
                                                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                            }
                                        }
                                    }
                                    CMBTDRW1::Refresh_Combat_Screen_Full_();
                                    action = 0;
                                }

                                if (action == MOX::_msl_warning_button) {
                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10007, 0, 0, 0);
                                    }
                                    MOX::_missile_warning = (MOX::_missile_warning + 1) % 3;
                                    if (MOX::_net_flag != 0) {
                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                    }
                                    action = 0;
                                    Add_Tactical_Combat_Fields_();
                                    Assign_Combat_Grids_(MOX::_cur_ship);
                                }

                                if (action == MOX::_auto_button) {
                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10008, 0, 0, 0);
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
                                    MOX::_auto_button = fields::Add_Hidden_Field_(0, 0, 639, 479, reinterpret_cast<const char*>(std::uintptr_t{0x00551100}), 0);
                                    if (MOX::_net_flag != 0) {
                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                    }
                                    action = 0;
                                }

                                if (action == MOX::_weapons_button) {
                                    MOX::_last_ship_status = -1;
                                    Set_Combat_Help_Fields_();
                                }

                                if (action == MOX::_specials_button) {
                                    MOX::_last_ship_status = -1;
                                    Set_Combat_Help_Fields_();
                                }

                                if (action == MOX::_wait_button) {
                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10009, 0, 0, 0);
                                    }
                                    action = 0;
                                    next_ship_status = Next_Ship_(param_1);
                                    if (MOX::_net_flag != 0) {
                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                    }
                                }

                                for (int16_t j = 0; j < 8; ++j) {
                                    if (action == MOX::_weapon_button[j]) {
                                        if (MOX::_ship_status_display == 0) {
                                            if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                Send_Tactical_Combat_Message_(MOX::_net_destination, 10010, j, 0, 0);
                                            }
                                            MOX::_combat_data[MOX::_cur_ship].weapons[j].active--;
                                            if (MOX::_combat_data[MOX::_cur_ship].weapons[j].active < -1) {
                                                MOX::_combat_data[MOX::_cur_ship].weapons[j].active = 1;
                                            }
                                            if (MOX::_combat_data[MOX::_cur_ship].weapons[j].active != 1 && MOX::_combat_data[MOX::_cur_ship].weapons[j].weapon_id == 0x27) {
                                                MOX::_combat_data[MOX::_cur_ship].special_cooldown = -1;
                                            }
                                            if (MOX::_net_flag != 0) {
                                                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                            }
                                            MOX::_last_ship_status = -1;
                                        }
                                        action = 0;
                                    }
                                }

                                if (action == MOX::_cmbt_info_button) {
                                    MOX::_order_info_flag = 1 - MOX::_order_info_flag;
                                    CMBTDRW1::Move_Options_Background_();
                                    action = 0;
                                }

                                if (action == MOX::_scan_button) {
                                    MOX::_board_flag = 0;
                                    Assign_Combat_Grids_(MOX::_cur_ship);
                                    action = 0;
                                }

                                if (action == MOX::_flee_button) {
                                    if (MOX::_combat_data[MOX::_cur_ship].is_retreating == 0) {
                                        bool show_msg = false;
                                        int16_t msg_id = 0;
                                        if (Enemy_Dissipator_Exists_(MOX::_combat_data[MOX::_cur_ship].owner) == 0) {
                                            if (MOX::_combat_data[MOX::_cur_ship].drive_destroyed == 0) {
                                                if (MOX::_combat_data[MOX::_cur_ship].current_speed >= 1) {
                                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10011, 0, 0, 0);
                                                    }
                                                    MOX::_combat_data[MOX::_cur_ship].is_retreating = 1;
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_status = Next_Ship_(param_1);
                                                    if (MOX::_net_flag != 0) {
                                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                    }
                                                } else {
                                                    msg_id = 0x4f;
                                                    show_msg = true;
                                                }
                                            } else {
                                                msg_id = 0x3b;
                                                show_msg = true;
                                            }
                                        } else {
                                            msg_id = 0x3a;
                                            show_msg = true;
                                        }
                                        
                                        if (show_msg) {
                                            char text[80];
                                            KEN::Ken_Get_Text_Message_(msg_id, text, 80);
                                            palette::Refresh_Palette_();
                                            GENDRAW::Message_Box_(text);
                                            CMBTDRW1::Refresh_Combat_Screen_Full_();
                                        }
                                    }
                                    action = 0;
                                }

                                if (action == MOX::_board_button) {
                                    MOX::_skan_flag = 0;
                                    if (CMBTFIR2::Already_Boarded_(MOX::_cur_ship) == 0) {
                                        Add_Tactical_Combat_Fields_();
                                        Assign_Combat_Grids_(MOX::_cur_ship);
                                    } else {
                                        char text[80];
                                        KEN::Ken_Get_Text_Message_(0x3c, text, 80);
                                        palette::Refresh_Palette_();
                                        GENDRAW::Message_Box_(text);
                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                        MOX::_board_flag = 0;
                                    }
                                    action = 0;
                                }

                                if (action == MOX::_map_field) {
                                    COMBINIT::Snap_Center_Combat_Screen_((MOX::_map_x * 2) / 3, (MOX::_map_y * 2) / 3);
                                }

                                if ((mouse::Pointer_X_() < 5 && MOX::_cur_combat_x > 0) ||
                                    (mouse::Pointer_X_() > 635 && MOX::_cur_combat_x < 49) ||
                                    (mouse::Pointer_Y_() < 5 && MOX::_cur_combat_y > 0) ||
                                    (mouse::Pointer_Y_() > 475 && MOX::_cur_combat_y < 50)) {
                                    COMBINIT::Scroll_Combat_Screen_();
                                }

                                if (action == MOX::_center_button) {
                                    COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[MOX::_cur_ship].x, MOX::_combat_data[MOX::_cur_ship].y);
                                }

                                if (action + MOX::_space_field == 0) {
                                    int16_t cx = (MOX::_xpos / 2) + MOX::_cur_combat_x;
                                    int16_t cy = (MOX::_ypos / 2) + MOX::_cur_combat_y;
                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10012, cx, cy, 0);
                                    }
                                    CMBTMOV1::Turn_To_Face_(MOX::_cur_ship, cx, cy);
                                    if (MOX::_net_flag != 0) {
                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                    }
                                    action = 0;
                                }

                                if (action == _end_of_turn_button) {
                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10016, MOX::_cur_combat_x, MOX::_cur_combat_y, 0);
                                    }
                                    if (MOX::_combat_init == 0) {
                                        next_ship_status = 1;
                                    } else {
                                        MOX::_side_end_of_turn[MOX::_combat_data[MOX::_cur_ship].owner] = 1;
                                        next_ship_status = Next_Ship_(param_1);
                                    }
                                    if (MOX::_net_flag != 0) {
                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                    }
                                    action = 0;
                                }

                                if (action == MOX::_space_field) {
                                    int32_t img = mouse::Get_Pointer_Image_Number_();
                                    int16_t grid_x, grid_y;
                                    if (img < 4 || img > 11 || MOX::_combat_data[MOX::_cur_ship].size_class == 0) {
                                        grid_x = MOX::_cur_combat_x + MOX::_xpos / 2;
                                        grid_y = MOX::_cur_combat_y + MOX::_ypos / 2;
                                    } else if (MOX::_combat_data[MOX::_cur_ship].size_class <= 2) {
                                        grid_x = MOX::_cur_combat_x - 1 + (MOX::_xpos + 1) / 2;
                                        grid_y = MOX::_cur_combat_y - 1 + (MOX::_ypos + 1) / 2;
                                    } else {
                                        grid_x = MOX::_cur_combat_x - 1 + MOX::_xpos / 2;
                                        grid_y = MOX::_cur_combat_y - 1 + MOX::_ypos / 2;
                                    }

                                    int16_t target_grid = MOX::_combat_grid[grid_y * 68 + grid_x];

                                    if (MOX::_board_flag + MOX::_skan_flag > 0) {
                                        if (MOX::_skan_flag != 0 && target_grid >= 0 && target_grid < MOX::_combat_total_ship_count && MOX::_combat_data[target_grid].unit_status == 0) {
                                            CMBTDRW1::Display_Combat_View_Ship_(target_grid);
                                            action = 0;
                                        } else if (MOX::_board_flag != 0 && target_grid >= 0 && target_grid < MOX::_combat_total_ship_count && MOX::_combat_data[target_grid].unit_status == 0) {
                                            Initiate_Boarding_Action_(MOX::_cur_ship, target_grid);
                                            action = 0;
                                        }
                                    } else {
                                        if (target_grid == 2000) {
                                            int8_t c_owner = MOX::_combat_data[MOX::_cur_ship].owner;
                                            int16_t cost = CMBTMOV1::Move_Cost_(MOX::_cur_ship, grid_x, grid_y);
                                            if (MOX::_combat_data[MOX::_cur_ship].movement_left < 0) {
                                                MOX::_combat_data[MOX::_cur_ship].movement_left = 0;
                                            }
                                            int16_t was_teleporter = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                            int16_t dist = CMBTMOV1::Move_Ship_(MOX::_cur_ship, grid_x, grid_y, cost);
                                            if (was_teleporter == CMBTFIR2::Used_Teleporter_(MOX::_cur_ship)) {
                                                MOX::_combat_data[MOX::_cur_ship].movement_left -= dist;
                                            }
                                            Assign_Combat_Grids_(MOX::_cur_ship);
                                            CMBTDRW1::Refresh_Combat_Screen_Full_();
                                            if (Ship_Done_Check_() != 0 || MOX::_combat_data[MOX::_cur_ship].owner != c_owner) {
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_status = Next_Ship_(param_1);
                                            }
                                            if (MOX::_net_flag != 0) {
                                                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                            }
                                            action = 0;
                                        } else if (target_grid >= 0 && target_grid < MOX::_combat_total_ship_count && MOX::_combat_data[target_grid].unit_status == 0) {
                                            if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_data[target_grid].owner) {
                                                if (MOX::_combat_init == 0) {
                                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10014, target_grid, 0, 0);
                                                    }
                                                    Set_Cur_Ship_To_(target_grid);
                                                    if (MOX::_net_flag != 0) {
                                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                    }
                                                }
                                                action = 0;
                                            } else {
                                                if (CMBTMOV1::Ship_Is_In_Stasis_(target_grid) != 0) {
                                                    char text[80];
                                                    KEN::Ken_Get_Text_Message_(0x3d, text, 80);
                                                    palette::Set_Palette_Gradient_(0x90, 0x20, 0, 0, 0, 0, 0, 0);
                                                    palette::Refresh_Palette_();
                                                    if (GENDRAW::Warning_Box2_(text) != 0) {
                                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                            Send_Tactical_Combat_Message_(MOX::_net_destination, 10013, target_grid, 0, 0);
                                                        }
                                                        CMBTFIRE::Remove_Stasis_Beam_(target_grid);
                                                        if (MOX::_net_flag != 0) {
                                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                        }
                                                    }
                                                    CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                    action = 0;
                                                } else if (MOX::_combat_data[target_grid].special_status_flag == 4) {
                                                    char text[80];
                                                    KEN::Ken_Get_Text_Message_(0x50, text, 80);
                                                    palette::Refresh_Palette_();
                                                    GENDRAW::Message_Box_(text);
                                                    CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                    action = 0;
                                                } else {
                                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10006, target_grid, 0, 0);
                                                    }
                                                    CMBTFIRE::Fire_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, target_grid, MOX::_combat_data[target_grid].x * 20, MOX::_combat_data[target_grid].y * 20, 0);
                                                    Add_Tactical_Combat_Fields_();
                                                    Assign_Combat_Grids_(MOX::_cur_ship);
                                                    for (int16_t j = 0; j < 8; ++j) {
                                                        if (MOX::_combat_data[MOX::_cur_ship].weapons[j].active == 0) {
                                                            MOX::_combat_data[MOX::_cur_ship].weapons[j].active = 1;
                                                        }
                                                    }
                                                    if (MOX::_net_flag != 0) {
                                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                    }
                                                    action = 0;
                                                }
                                            }
                                        } else {
                                            action = 0;
                                        }
                                    }
                                }

                                if (action == MOX::_done_button) {
                                    if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                        Send_Tactical_Combat_Message_(MOX::_net_destination, 10004, 0, 0, 0);
                                    }
                                    CMBTFIR2::Set_Combat_Ship_Done_(MOX::_cur_ship);
                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                    next_ship_status = Next_Ship_(param_1);
                                    if (MOX::_net_flag != 0) {
                                        CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                    }
                                    action = 0;
                                    fields::Set_Input_Delay_(3);
                                }
                            }
                        } else {
                            MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                            next_ship_status = Next_Ship_(param_1);
                        }
                    }

                    if (next_ship_status == 0) {
                        if (MOX::_FULL_DRAW_SCREEN_FLAG == 0 && MOX::_combat_data[MOX::_cur_ship].movement_left == local_60) {
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
                    if (MOX::_combat_init == 0 || MOX::_combat_data[i].owner == param_1) {
                        MOX::_combat_data[i].field39_0x45 = 0;
                    }
                }
            }
        }

        if (param_2 == 0) {
            Seeking_Missiles_(param_1);
        }

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (Combat_Ship_Is_Functional_(i) != 0) {
                    if (MOX::_combat_data[i].owner == param_1) {
                        for (int16_t j = 0; j < 8; ++j) {
                            if (MOX::_combat_data[i].weapons[j].status == 2) {
                                MOX::_combat_data[i].weapons[j].status = 1;
                            }
                        }
                        if (MOX::_combat_data[i].special_status_flag == 3) {
                            CMBTSPEC::Draw_Cloak_(i, 1);
                            MOX::_combat_data[i].special_status_flag = 1;
                        }
                        if (MOX::_combat_data[i].special_status_flag == 4) {
                            MOX::_combat_data[i].special_status_timer--;
                            if (MOX::_combat_data[i].special_status_timer == 0) {
                                CMBTSPEC::Draw_Cloak_(i, 5);
                                CMBTSPEC::Draw_Cloak_(i, 1);
                                MOX::_combat_data[i].special_status_flag = 1;
                            }
                        }
                        if (MOX::_combat_data[i].special_status_flag == 7) {
                            CMBTSPEC::Draw_Cloak_(i, 4);
                            MOX::_combat_data[i].special_status_flag = 4;
                            for (int16_t m = 0; m < 300; ++m) {
                                if (MOX::_missile[m].type > 0 && MOX::_missile[m].target_unit_idx == i && MOX::_missile[m].source_unit_idx != i) {
                                    CMBTMIS::Retarget_Missile_(m);
                                }
                            }
                        }
                        MOX::_combat_data[i].ai_range_tactic = static_cast<int8_t>(CMBTFIR2::Max_Defensive_Fire_Range_(i));
                    }
                }
            }
        }

        if (MOX::_net_flag != 0) {
            if (MOX::_PLAYER_NUM == param_1) {
                MOX::_msl_impact_warning_flag = MOX::_saved_msl_warn;
            }
            if (MOX::_net_flag != 0) {
                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
            }
        }
    }

}