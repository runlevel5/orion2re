    void __cdecl Do_Combat_Turn_(int16_t param_1, int16_t param_2) {
        MOX::_player_mode[MOX::_combat_attacker_player] = CMBTAI::Get_Player_Mode_(MOX::_combat_attacker_player);
        MOX::_player_mode[MOX::_combat_defender_player] = CMBTAI::Get_Player_Mode_(MOX::_combat_defender_player);

        for (int16_t i = 0; i < 15; ++i) {
            MOX::_side_end_of_turn[i] = 0;
        }

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (Combat_Ship_Is_Functional_(i) != 0) {
                    if (param_2 == 0 || COMBINIT::Does_Combat_Ship_Have_Special_(i, 0x22) != 0) {
                        if (MOX::_combat_init != 0 || MOX::_combat_data[i].owner == param_1) {
                            Init_Ship_For_Start_Of_Turn_(i);
                        }
                    }
                }
            }
        }

        RUSS::Mox_Update_();
        
        int16_t order_idx = 0;
        int16_t num_captured = 0;

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[i].unit_status == 0) {
                    if (CMBTMOV1::Ship_Is_In_Stasis_(i) == 0) {
                        if (param_2 == 0 || COMBINIT::Does_Combat_Ship_Have_Special_(i, 0x22) != 0) {
                            if (MOX::_combat_init != 0 || MOX::_combat_data[i].owner == param_1) {
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

        if (order_idx != num_captured) {
            if (MOX::_combat_init != 0) {
                _qsort(MOX::_ship_turn_order, order_idx, 2, qsort_ship_initiative_);
            }
            MOX::_ship_turn_order[order_idx++] = -10;
            while (order_idx < 210) {
                MOX::_ship_turn_order[order_idx++] = -1;
            }

            RUSS::Mox_Update_();
            MOX::_start_of_turn = 1;
            MOX::_cur_ptr = -1;
            int16_t next_ship_flag = Next_Ship_(param_1);

            if (next_ship_flag != 1) {
                fields::Assign_Auto_Function_(CMBTDRW1::Draw_Main_Combat_Screen_, 2);
                MOX::_skan_flag = 0;
                MOX::_board_flag = 0;
                RUSS::Mox_Update_();

                if (MOX::_combat_total_ship_count > 0) {
                    for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                        if (MOX::_combat_data[i].drive_destroyed == 0 && MOX::_combat_data[i].unit_status == 0) {
                            if (CMBTMOV1::Ship_Is_In_Stasis_(i) == 0) {
                                CMBTMOV1::Rotate_Ship_(i, (MOX::_combat_data[i].facing_dir + 2) & 0xf, (MOX::_combat_data[i].x - MOX::_cur_combat_x) * 20, (MOX::_combat_data[i].y - MOX::_cur_combat_y) * 20);
                            }
                        }
                    }
                }

                MOX::_start_of_turn = 0;
                
                while (MOX::_ship_turn_order[MOX::_cur_ptr] >= 0) {
                    if (next_ship_flag != 0) break;

                    timer::Mark_Time_();
                    MOX::_FULL_DRAW_SCREEN_FLAG = 0;
                    int16_t m_left = MOX::_combat_data[MOX::_cur_ship].movement_left;

                    if (Ship_Controlled_By_Computer_(MOX::_cur_ship) != 0 && 
                        MOX::_net_flag == 0 || MOX::_combat_data[MOX::_cur_ship].owner == MOX::_PLAYER_NUM) {
                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                            Send_Tactical_Combat_Message_(MOX::_net_destination, 10001, 0, 0, 0);
                        }
                        CMBTAI::Do_Auto_Ship_Turn_();
                        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                            Add_Tactical_Combat_Fields_();
                            Assign_Combat_Grids_(MOX::_cur_ship);
                            CMBTDRW1::Refresh_Combat_Screen_Full_();
                        }
                        MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                        next_ship_flag = Next_Ship_(param_1);
                        if (MOX::_net_flag != 0) {
                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                        }
                    } else {
                        MOX::_cur_ship = MOX::_ship_turn_order[MOX::_cur_ptr];
                        if (CMBTMOV1::Ship_Is_In_Stasis_(MOX::_cur_ship) == 0) {
                            if (MOX::_combat_init != 0 || MOX::_combat_data[MOX::_cur_ship].owner == param_1) {
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
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, 10006, m_idx, 1, 0);
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
                                            NETMOX::Mox_Send_Message_(MOX::_player[MOX::_net_destination].network_player_id, 1003, reinterpret_cast<uint8_t*>(&MOX::_msl_impact_warning_flag), 2);
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
                                            KEN::Ken_Get_Text_Message_(56, buf, 80);
                                            palette::Refresh_Palette_();
                                            GENDRAW::Message_Box_(buf);
                                        } else {
                                            KEN::Ken_Get_Text_Message_(57, buf, 80);
                                            palette::Refresh_Palette_();
                                            if (GENDRAW::Warning_Box_(buf) != 0) {
                                                MOX::_order_info_flag = 1 - MOX::_order_info_flag;
                                                CMBTDRW1::Move_Options_Background_();
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, 10000, 0, 0, 0);
                                                }
                                                MOX::_combat_data[MOX::_cur_ship].drive_current_hits = 0;
                                                int8_t saved_destr = MOX::_combat_data[MOX::_cur_ship].drive_destroyed;
                                                MOX::_combat_data[MOX::_cur_ship].drive_destroyed = 1;
                                                CMBTFIRE::Destroy_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship));
                                                MOX::_combat_data[MOX::_cur_ship].drive_destroyed = saved_destr;
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_flag = Next_Ship_(param_1);
                                                if (MOX::_net_flag != 0) {
                                                    CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                }
                                            }
                                        }
                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_msl_warning_button) {
                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, 10007, 0, 0, 0);
                                        }
                                        MOX::_missile_warning = (MOX::_missile_warning + 1) % 3;
                                        if (MOX::_net_flag != 0) {
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        Add_Tactical_Combat_Fields_();
                                        Assign_Combat_Grids_(MOX::_cur_ship);
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_auto_button) {
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
                                        MOX::_auto_button = fields::Add_Hidden_Field_(0, 0, 639, 479, MOX::s__00551100, 0);
                                        if (MOX::_net_flag != 0) {
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
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, 10009, 0, 0, 0);
                                        }
                                        next_ship_flag = Next_Ship_(param_1);
                                        if (MOX::_net_flag != 0) {
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        input_val = 0;
                                    }

                                    for (int16_t w_idx = 0; w_idx < 8; ++w_idx) {
                                        if (MOX::_weapon_button[w_idx] == input_val) {
                                            if (MOX::_ship_status_display == 0) {
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, 10010, w_idx, 0, 0);
                                                }
                                                MOX::_combat_data[MOX::_cur_ship].weapons[w_idx].active -= 1;
                                                if (MOX::_combat_data[MOX::_cur_ship].weapons[w_idx].active < -1) {
                                                    MOX::_combat_data[MOX::_cur_ship].weapons[w_idx].active = 1;
                                                }
                                                if (MOX::_combat_data[MOX::_cur_ship].weapons[w_idx].active != 1 && MOX::_combat_data[MOX::_cur_ship].weapons[w_idx].weapon_id == 0x27) {
                                                    MOX::_combat_data[MOX::_cur_ship].special_cooldown = -1;
                                                }
                                                if (MOX::_net_flag != 0) {
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
                                                if (MOX::_combat_data[MOX::_cur_ship].drive_destroyed != 0) {
                                                    msg_id = 59;
                                                } else if (MOX::_combat_data[MOX::_cur_ship].current_speed < 1) {
                                                    msg_id = 79;
                                                }
                                            } else {
                                                msg_id = 58;
                                            }
                                            if (msg_id != 0) {
                                                char buf[80];
                                                KEN::Ken_Get_Text_Message_(msg_id, buf, 80);
                                                palette::Refresh_Palette_();
                                                GENDRAW::Message_Box_(buf);
                                                CMBTDRW1::Refresh_Combat_Screen_Full_();
                                            } else {
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, 10011, 0, 0, 0);
                                                }
                                                MOX::_combat_data[MOX::_cur_ship].is_retreating = 1;
                                                MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                next_ship_flag = Next_Ship_(param_1);
                                                if (MOX::_net_flag != 0) {
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
                                            KEN::Ken_Get_Text_Message_(60, buf, 80);
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
                                        (mouse::Pointer_X_() > 635 && MOX::_cur_combat_x < 49) || 
                                        (mouse::Pointer_Y_() < 5 && MOX::_cur_combat_y > 0) || 
                                        (mouse::Pointer_Y_() > 475 && MOX::_cur_combat_y < 50)) {
                                        COMBINIT::Scroll_Combat_Screen_();
                                    }

                                    if (input_val == MOX::_center_button) {
                                        COMBINIT::Snap_Center_Combat_Screen_(MOX::_combat_data[MOX::_cur_ship].x, MOX::_combat_data[MOX::_cur_ship].y);
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
                                        
                                        int16_t tgt_idx = MOX::_combat_grid[tx * 68 + ty];
                                        if (MOX::_board_flag + MOX::_skan_flag <= 0) {
                                            if (tgt_idx == 2000) {
                                                int8_t orig_owner = MOX::_combat_data[MOX::_cur_ship].owner;
                                                if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                    Send_Tactical_Combat_Message_(MOX::_net_destination, 10005, tx, ty, 0);
                                                }
                                                int16_t cost = CMBTMOV1::Move_Cost_(MOX::_cur_ship, tx, ty);
                                                if (MOX::_combat_data[MOX::_cur_ship].movement_left < 0) {
                                                    MOX::_combat_data[MOX::_cur_ship].movement_left = 0;
                                                }
                                                int16_t used_tp1 = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                int16_t act_cost = CMBTMOV1::Move_Ship_(MOX::_cur_ship, tx, ty, cost);
                                                int16_t used_tp2 = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                if (used_tp1 == used_tp2) {
                                                    MOX::_combat_data[MOX::_cur_ship].movement_left -= act_cost;
                                                }
                                                Assign_Combat_Grids_(MOX::_cur_ship);
                                                CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                if (Ship_Done_Check_() != 0 || MOX::_combat_data[MOX::_cur_ship].owner != orig_owner) {
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(param_1);
                                                }
                                                if (MOX::_net_flag != 0) {
                                                    CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                }
                                            } else if (tgt_idx >= 0 && tgt_idx < MOX::_combat_total_ship_count && MOX::_combat_data[tgt_idx].unit_status == 0) {
                                                if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_data[tgt_idx].owner) {
                                                    if (MOX::_combat_init == 0) {
                                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                            Send_Tactical_Combat_Message_(MOX::_net_destination, 10014, tgt_idx, 0, 0);
                                                        }
                                                        Set_Cur_Ship_To_(tgt_idx);
                                                        if (MOX::_net_flag != 0) {
                                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                        }
                                                    }
                                                } else {
                                                    if (CMBTMOV1::Ship_Is_In_Stasis_(tgt_idx) != 0) {
                                                        char buf[80];
                                                        KEN::Ken_Get_Text_Message_(61, buf, 80);
                                                        palette::Set_Palette_Gradient_(144, 32, 0, 0, 0, 0, 0, 0);
                                                        palette::Refresh_Palette_();
                                                        if (GENDRAW::Warning_Box_(buf) != 0) {
                                                            if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                                Send_Tactical_Combat_Message_(MOX::_net_destination, 10013, tgt_idx, 0, 0);
                                                            }
                                                            CMBTFIRE::Remove_Stasis_Beam_(tgt_idx);
                                                            if (MOX::_net_flag != 0) {
                                                                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                                            }
                                                        }
                                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                    } else if (MOX::_combat_data[tgt_idx].special_status_flag == 4) {
                                                        char buf[80];
                                                        KEN::Ken_Get_Text_Message_(80, buf, 80);
                                                        palette::Refresh_Palette_();
                                                        GENDRAW::Message_Box_(buf);
                                                        CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                    } else {
                                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                                            Send_Tactical_Combat_Message_(MOX::_net_destination, 10006, tgt_idx, 0, 0);
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
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, 10016, MOX::_cur_combat_x, MOX::_cur_combat_y, 0);
                                        }
                                        if (MOX::_combat_init == 0) {
                                            next_ship_flag = 1;
                                        } else {
                                            MOX::_side_end_of_turn[MOX::_combat_data[MOX::_cur_ship].owner] = 1;
                                            next_ship_flag = Next_Ship_(param_1);
                                        }
                                        if (MOX::_net_flag != 0) {
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                        input_val = 0;
                                    }

                                    if (input_val == MOX::_done_button) {
                                        if (MOX::_net_flag != 0 && MOX::_PLAYER_NUM != MOX::_net_destination) {
                                            Send_Tactical_Combat_Message_(MOX::_net_destination, 10004, 0, 0, 0);
                                        }
                                        CMBTFIR2::Set_Combat_Ship_Done_(MOX::_cur_ship);
                                        MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                        next_ship_flag = Next_Ship_(param_1);
                                        if (MOX::_net_flag != 0) {
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
                                            COMBINIT::Snap_Center_Combat_Screen_(MOX::_new_message.f0 + 16, MOX::_new_message.f2 + 9);
                                            int16_t f10 = MOX::_new_message.f10;
                                            int16_t f12 = MOX::_new_message.f12;
                                            int16_t f14 = MOX::_new_message.f14;
                                            int16_t f8 = MOX::_new_message.f8;
                                            random::Set_Random_Seed_(MOX::_new_message.f4);
                                            struct_::Clear_Structure_(&MOX::_new_message, 16);

                                            switch (f8) {
                                                case 10000:
                                                    MOX::_combat_data[MOX::_cur_ship].drive_current_hits = 0;
                                                    {
                                                        int8_t saved_destr = MOX::_combat_data[MOX::_cur_ship].drive_destroyed;
                                                        MOX::_combat_data[MOX::_cur_ship].drive_destroyed = 1;
                                                        CMBTFIRE::Destroy_Ship_(MOX::_cur_ship, MOX::_combat_data[MOX::_cur_ship].x * 20, MOX::_combat_data[MOX::_cur_ship].y * 20, CMBTFIRE::Is_Combat_Ship_Dead_(MOX::_cur_ship));
                                                        MOX::_combat_data[MOX::_cur_ship].drive_destroyed = saved_destr;
                                                    }
                                                    MOX::_tactical_net_choice = 10000;
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(param_1);
                                                    break;
                                                case 10001:
                                                    CMBTAI::Do_Auto_Ship_Turn_();
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(param_1);
                                                    break;
                                                case 10004:
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(param_1);
                                                    break;
                                                case 10005: {
                                                    int8_t orig_owner = MOX::_combat_data[MOX::_cur_ship].owner;
                                                    int16_t cost = CMBTMOV1::Move_Cost_(MOX::_cur_ship, f10, f12);
                                                    if (MOX::_combat_data[MOX::_cur_ship].movement_left < 0) {
                                                        MOX::_combat_data[MOX::_cur_ship].movement_left = 0;
                                                    }
                                                    int16_t used_tp1 = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                    int16_t act_cost = CMBTMOV1::Move_Ship_(MOX::_cur_ship, f10, f12, cost);
                                                    int16_t used_tp2 = CMBTFIR2::Used_Teleporter_(MOX::_cur_ship);
                                                    if (used_tp1 == used_tp2) {
                                                        MOX::_combat_data[MOX::_cur_ship].movement_left -= act_cost;
                                                    }
                                                    CMBTDRW1::Refresh_Combat_Screen_Full_();
                                                    if (Ship_Done_Check_() != 0 || MOX::_combat_data[MOX::_cur_ship].owner != orig_owner) {
                                                        MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                        next_ship_flag = Next_Ship_(param_1);
                                                    }
                                                    break;
                                                }
                                                case 10006:
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
                                                case 10007:
                                                    MOX::_missile_warning = (MOX::_missile_warning + 1) % 3;
                                                    Add_Tactical_Combat_Fields_();
                                                    Assign_Combat_Grids_(MOX::_cur_ship);
                                                    break;
                                                case 10008:
                                                    fields::Clear_Fields_();
                                                    MOX::_auto_button = fields::Add_Hidden_Field_(0, 0, 639, 479, MOX::s__00551100, 0);
                                                    if (MOX::_combat_data[MOX::_cur_ship].owner == MOX::_combat_attacker_player) {
                                                        MOX::_auto_combat_flag[0] = 1 - MOX::_auto_combat_flag[0];
                                                        MOX::_auto_combat = MOX::_auto_combat_flag[0];
                                                    } else {
                                                        MOX::_auto_combat_flag[1] = 1 - MOX::_auto_combat_flag[1];
                                                        MOX::_auto_combat = MOX::_auto_combat_flag[1];
                                                    }
                                                    break;
                                                case 10009:
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(param_1);
                                                    break;
                                                case 10010:
                                                    MOX::_combat_data[MOX::_cur_ship].weapons[f10].active -= 1;
                                                    if (MOX::_combat_data[MOX::_cur_ship].weapons[f10].active < -1) {
                                                        MOX::_combat_data[MOX::_cur_ship].weapons[f10].active = 1;
                                                    }
                                                    if (MOX::_combat_data[MOX::_cur_ship].weapons[f10].active != 1 && MOX::_combat_data[MOX::_cur_ship].weapons[f10].weapon_id == 0x27) {
                                                        MOX::_combat_data[MOX::_cur_ship].special_cooldown = -1;
                                                    }
                                                    break;
                                                case 10011:
                                                    MOX::_combat_data[MOX::_cur_ship].is_retreating = 1;
                                                    MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                                                    next_ship_flag = Next_Ship_(param_1);
                                                    break;
                                                case 10012:
                                                    CMBTMOV1::Turn_To_Face_(MOX::_cur_ship, f10, f12);
                                                    break;
                                                case 10013:
                                                    CMBTFIRE::Remove_Stasis_Beam_(f10);
                                                    break;
                                                case 10014:
                                                    if (MOX::_combat_init == 0) {
                                                        Set_Cur_Ship_To_(f10);
                                                    }
                                                    break;
                                                case 10015:
                                                    if (f14 == 1) {
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
                                                case 10016:
                                                    if (MOX::_combat_init == 0) {
                                                        next_ship_flag = 1;
                                                    } else {
                                                        MOX::_side_end_of_turn[MOX::_combat_data[MOX::_cur_ship].owner] = 1;
                                                        next_ship_flag = Next_Ship_(param_1);
                                                    }
                                                    break;
                                            }
                                            MOX::_tactical_net_choice = 0;
                                            CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
                                        }
                                    }
                                }
                            }
                        } else {
                            MOX::_ship_turn_order[MOX::_cur_ptr] = -1;
                            next_ship_flag = Next_Ship_(param_1);
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
            if (MOX::_PLAYER_NUM != param_1) {
                MOX::_msl_impact_warning_flag = MOX::_saved_msl_warn;
            }
            if (MOX::_net_flag != 0) {
                CMBTMOV1::Wait_Until_Net_Opponent_Finished_(1);
            }
        }
    }

}