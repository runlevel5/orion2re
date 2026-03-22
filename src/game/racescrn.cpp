#include "pch.h"

namespace RACESCRN {

    void __cdecl Init_Race_Display_Data_(s_racescrn_race_display* race_display, s_racescrn_mouse_group* agent_group) {
        memset(race_display, 0, sizeof(s_racescrn_race_display) * 7);

        s_animation_header* spy_agent_icon_sprite = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, MOX::_player[MOX::_PLAYER_NUM].race + 0x2E, MOX::_screen_seg);
        s_animation_header* slider_sprite = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, 2, MOX::_screen_seg);
        s_animation_header* eliminated_overlay_sprite = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, 0x1F, MOX::_screen_seg);

        for (int16_t i = 0; i < 7; ++i) {
            race_display[i].relationship_slider_img = slider_sprite;

            if (i < MOX::_active_count) {
                int16_t active_player_idx = MOX::_active_player[i];
                s_player* player = &MOX::_player[active_player_idx];
                int16_t relations_val = MOX::_cur_plyr_ptr->relations[active_player_idx] + 100;

                int16_t relation_idx = 0;
                while (RACESCRN::_relation_name_table[relation_idx] < (uint8_t)relations_val) {
                    relation_idx++;
                }

                race_display[i].relationship_text = RACESCRN::_relation_text[relation_idx];
                uint8_t slider_remap_idx = RACESCRN::_slider_remap_list[relations_val >> 3];

                race_display[i].list_idx = i;
                race_display[i].player_idx = active_player_idx;
                race_display[i].player = player;
                race_display[i].player_name = player->name;
                race_display[i].race = (int16_t)(uint8_t)player->race;
                race_display[i].relationship_slider_y = RACESCRN::_race_bar_y[i * 2] + (int16_t)slider_remap_idx - 6;
                race_display[i].eliminated_overlay_img = eliminated_overlay_sprite;

                if (!player->eliminated) {
                    int16_t spy_count = BILL::Get_My_Spy_Number_(active_player_idx);
                    race_display[i].agent_mission_idx = (int16_t)BILL::Get_My_Spy_Mission_(active_player_idx) + 1;

                    RACESCRN::Setup_Icon_Group_Data_(&race_display[i].spy_icons_group, &RACESCRN::_spy_group_data[i], spy_count, spy_agent_icon_sprite, nullptr);

                    race_display[i].spy_icon_img = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, i + 10, MOX::_screen_seg);
                    race_display[i].sabotage_icon_img = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, i + 17, MOX::_screen_seg);
                    race_display[i].assassination_icon_img = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, i + 24, MOX::_screen_seg);
                } else {
                    race_display[i].is_eliminated = 1;
                }

                race_display[i].race_portrait_img = (s_animation_header*)farload::Far_Reload_Next_(races_lbx, race_display[i].race + 32, MOX::_screen_seg);
            } else {
                race_display[i].is_empty_slot = 1;
            }
        }

        int16_t agent_count = BILL::Get_My_Agent_Number_();
        RACESCRN::Setup_Icon_Group_Data_(agent_group, &_agent_group_data, agent_count, spy_agent_icon_sprite, nullptr);
    }

    void __cdecl Race_Screen_() {
        int16_t selected_race_idx = -1;
        int16_t old_mission_idx = -1;
        void* auto_function = nullptr;

        MOX2::Get_Auto_Function_(&auto_function);
        palette::Save_Palette_();
        fields::Disable_Draw_All_Fields_();
        RUSS::Mox_Update_();

        for (int i = 0; i < 17; ++i) {
            JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x21 + i, _relation_text[i], 15);
            RUSS::Mox_Update_();
        }

        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x32, _ignoring_str, 20);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x33, _no_contact_str, 20);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x34, _spy_bonus_str, 20);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x35, _agent_bonus_str, 20);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x39, _research_treaty_buffer, 30);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x3A, _trade_treaty_buffer, 30);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x3B, _giving_treaty_buffer, 80);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x3C, _receiving_treaty_buffer, 80);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x4C, _diplomacy_net_requeting_buffer, 100);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 0x4D, _diplomacy_net_wait_buffer, 100);
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_billtex2_lbx_00556038, 0x1A, _declare_war_buffer, 120);
        RUSS::Mox_Update_();
        
        ERIC::Fast_Fade_Out_();

        while (true) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            RUSS::Mox_Update_();

            s_racescrn_race_display race_display_data[7];
            s_racescrn_mouse_group agent_group;
            char message_buffer[80];
            s_racescrn_mouse_group mouse_group;

            MOX::_cur_plyr_ptr = &MOX::_player[MOX::_PLAYER_NUM];
            _g_display_p = race_display_data;
            _g_mouse_group_p = &mouse_group;
            _g_agent_group_p = &agent_group;

            RUSS::Mox_Update_();
            BILLHELP::Races_Help_();
            fields::Clear_Fields_();
            video::Set_Page_Off_();
            video::Set_Refresh_Stencil_();
            RUSS::Mox_Update_();

            uint32_t bmp_size = bitmap::Get_Bitmap_Size_(187, 25);
            s_animation_header* sprite = static_cast<s_animation_header*>(allocate::Allocate_First_Block_(MOX::_screen_seg, bmp_size));
            bitmap::Create_Blank_Bitmap_(187, 25, sprite);
            RUSS::Mox_Update_();

            s_animation_header* anim = reinterpret_cast<s_animation_header*>(
                farload::Far_Reload_Next_(s_races_lbx_005574e8, 0, MOX::_screen_seg));
            RUSS::Mox_Update_();

            _arrow_seg = reinterpret_cast<s_animation_header*>(
                farload::Far_Reload_Next_(s_races_lbx_005574e8, 1, MOX::_screen_seg));
            RUSS::Mox_Update_();

            _g_mouse_seg = reinterpret_cast<s_animation_header*>(
                farload::Far_Reload_Next_(s_races_lbx_005574e8, 4, MOX::_screen_seg));
            RUSS::Mox_Update_();

            int16_t sprite_idx = 5;
            switch (MOX::_settings.language) {
                case 0: sprite_idx = 5; break;
                case 1: sprite_idx = 61; break;
                case 2: sprite_idx = 62; break;
                case 3: sprite_idx = 5; break;
                case 4: sprite_idx = 63; break;
            }
            s_animation_header* mouse_pic = reinterpret_cast<s_animation_header*>(
                farload::Far_Reload_Next_(s_races_lbx_005574e8, sprite_idx, MOX::_screen_seg));
            RUSS::Mox_Update_();

            _netbox_seg = reinterpret_cast<uintptr_t>(farload::Far_Reload_Next_(s_races_lbx_005574e8, 59, MOX::_screen_seg));
            RUSS::Mox_Update_();

            JIM::Bill_Picture_To_Mouse_(_g_mouse_seg);
            BILL::Get_Players_Dead_Or_Alive_Or_Omniscient_(0, 0);
            RUSS::Mox_Update_();

            Init_Race_Display_Data_(race_display_data, &agent_group);
            RUSS::Mox_Update_();

            Setup_Icon_Group_Data_(&mouse_group, &_mouse_group_data, 0, nullptr, sprite);
            RUSS::Mox_Update_();

            graphics::Fill_(0, 0, 639, 479, 0);
            animate::Draw_(0, 0, anim);
            RUSS::Mox_Update_();

            if (ERIC::Get_Fade_Percent_() > 0) {
                BILL::Turn_Off_Palettes_(0, 255);
            }
            palette::Reset_Cycle_Palette_Color_();
            RUSS::Mox_Update_();

            fonts::Set_Outline_Color_(177, 177);
            Draw_Race_Photos_(race_display_data);
            RUSS::Mox_Update_();

            Draw_Race_Text_(race_display_data);
            RUSS::Mox_Update_();

            BILL::Get_Active_Players_(0, 0);
            Draw_Relations_Sliders_(race_display_data);
            RUSS::Mox_Update_();

            video::Copy_Off_To_Back_();

            int16_t exit_field;
            int16_t btn_diplomacy;
            int16_t btn_report;
            int16_t btn_ignoring;
            int16_t btn_war;
            int16_t cancel_field = 0;

            int16_t state_diplomacy = 0;
            int16_t state_report = 0;
            int16_t state_ignoring = 0;
            int16_t state_war = 0;

            int16_t display_base = Setup_Main_Fields_(&exit_field, &btn_diplomacy, &state_diplomacy, &btn_report, &state_report, &btn_ignoring, &state_ignoring, &btn_war, &state_war);
            RUSS::Mox_Update_();

            Setup_Race_Display_Fields_(race_display_data, display_base);

            int16_t hidden_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);

            fields::Assign_Auto_Function_(Draw_Race_Screen_, 2);
            RUSS::Mox_Update_();
            Draw_Race_Screen_();
            RUSS::Mox_Update_();

            s_racescrn_mouse_group* active_spy_group = nullptr;
            palette::Clear_Palette_Changes_(0, 255);
            RUSS::Mox_Sync_Update_();
            ERIC::Fast_Fade_In_();
            
            _g_in_spy_mode = 0;
            _g_in_who_mode = 0;

            int16_t active_action_field = 0;
            int8_t net_flags = 0;

            while (true) {
                int16_t input = fields::Get_Input_();
                
                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    if (_g_in_spy_mode) {
                        Move_Spy_Group_From_Mouse_(active_spy_group);
                    }
                    Update_Spy_Stuff_(race_display_data);
                    break;
                }

                if (input < 0) {
                    input = -input;
                }

                if (input == 0) {
                    timer::Mark_Time_();
                    Draw_Race_Screen_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(2);
                    continue;
                }

                if (input == exit_field) {
                    if (_g_in_spy_mode) {
                        Move_Spy_Group_From_Mouse_(active_spy_group);
                        if (active_spy_group != &agent_group) {
                            race_display_data[selected_race_idx].agent_mission_idx = old_mission_idx;
                        }
                    }
                    Update_Spy_Stuff_(race_display_data);
                    ERIC::Fast_Fade_Out_();
                    MOX2::Put_Auto_Function_(&auto_function);
                    fields::Deactivate_Help_List_();
                    COLONY::Reset_Mouse_Picture_();
                    palette::Set_Palette_Changes_(0, 255);
                    palette::Restore_Palette_();
                    BILL::Turn_Off_Palettes_(0, 255);
                    fields::Enable_Draw_All_Fields_();
                    MOX::_current_screen = 0;
                    return;
                }

                if (input > display_base) {
                    if (_g_in_who_mode) {
                        if (input == cancel_field) {
                            break;
                        }

                        int16_t race_display_idx = input - display_base - 1;
                        selected_race_idx = race_display_idx;
                        int16_t target_player_idx = race_display_data[race_display_idx].player_idx;

                        if (active_action_field == btn_ignoring) {
                            MOX::_cur_plyr_ptr->ignoring ^= (1 << target_player_idx);
                            break;
                        } else if (active_action_field == btn_report) {
                            JIM::Bill_Picture_To_Mouse_(_g_mouse_seg);
                            ERIC::Fast_Fade_Out_();
                            Update_Spy_Stuff_(race_display_data);
                            RACERPRT::Race_Report_Screen_(target_player_idx);
                            RUSS::Mox_Update_();
                            break;
                        } else if (active_action_field == btn_war) {
                            Update_Spy_Stuff_(race_display_data);
                            if (MOX::_cur_plyr_ptr->treaty[target_player_idx] >= 4) {
                                break;
                            }
                            strcpy(message_buffer, _declare_war_buffer);
                            JIM::Decode_Text_Race_(message_buffer, target_player_idx, 120);
                            if (GENDRAW::Warning_Box2_(message_buffer) == 1) {
                                MOX::_player[MOX::_PLAYER_NUM].delayed_diplomacy_orders[target_player_idx] = 0;
                            }
                            break;
                        } else if (active_action_field == btn_diplomacy) {
                            Update_Spy_Stuff_(race_display_data);
                            MOX::_cur_plyr_ptr->ignoring &= ~(1 << target_player_idx);
                            
                            if (MOX::_game_type != 2 && MOX::_game_type != 3) {
                                DIP_SCRN::Diplomacy_Screen_(target_player_idx);
                                break;
                            }

                            if (MOX::_player[target_player_idx].objectives == 100) {
                                _net_state = 1;
                                net_flags = 0;
                                NETMOX::Net_Request_AI_Diplomacy_(target_player_idx);
                                if (NETMOX::Net_May_Begin_AI_Diplomacy_() == 0) {
                                    if (net_flags == 0) {
                                        if (NETMOX::No_AI_Diplomacy_Allowed_() == 0) {
                                            while (true) {
                                                timer::Mark_Time_();
                                                fields::Clear_Fields_();
                                                fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
                                                if (fields::Get_Input_() != 0) {
                                                    net_flags = 1;
                                                }
                                                Draw_Race_Screen_();
                                                RUSS::Mox_Sync_Update_();
                                                RUSS::Mox_Update_();
                                                timer::Release_Time_(2);
                                                if (NETMOX::Net_May_Begin_AI_Diplomacy_() != 0) break;
                                                if (net_flags != 0) break;
                                                if (NETMOX::No_AI_Diplomacy_Allowed_() != 0) break;
                                            }
                                        }
                                    }
                                }
                                _net_state = 0;
                                if (net_flags == 0) {
                                    if (NETMOX::No_AI_Diplomacy_Allowed_() != 0) {
                                        JIM::Get_Text_Message_(s_billtex2_lbx_00556038, 27, message_buffer, 80);
                                        GENDRAW::Message_Box_(message_buffer);
                                    } else {
                                        DIP_SCRN::Diplomacy_Screen_(target_player_idx);
                                        NETMOX::Net_End_AI_Diplomacy_();
                                    }
                                } else {
                                    NETMOX::Net_Cancel_AI_Diplomacy_Request_();
                                    while (NETMOX::Net_Cancel_AI_Diplomacy_Confirmed_() == 0) {
                                        RUSS::Mox_Update_();
                                    }
                                }
                                break;
                            } else {
                                cancel_field = 0;
                                net_flags = 0;
                                _net_state = 1;
                                NETMOX::Net_Request_Human_Diplomacy_(target_player_idx, 2);
                                if (NETMOX::Net_May_Begin_Human_Diplomacy_() == 0) {
                                    if (net_flags == 0) {
                                        if (NETMOX::Human_Hit_Next_Turn_(target_player_idx) == 0) {
                                            while (true) {
                                                timer::Mark_Time_();
                                                fields::Clear_Fields_();
                                                fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
                                                if (fields::Get_Input_() != 0) {
                                                    if (NETMOX::Allowed_To_Cancel_Human_Diplomacy_() != 0) {
                                                        net_flags = 1;
                                                    }
                                                }
                                                if (NETMOX::Human_Diplomat_Says_Wait_() != 0) {
                                                    _net_state = 2;
                                                }
                                                if (NETMOX::Net_Human_Rejected_Diplomacy_() != 0) {
                                                    cancel_field = 1;
                                                }
                                                Draw_Race_Screen_();
                                                RUSS::Mox_Sync_Update_();
                                                RUSS::Mox_Update_();
                                                timer::Release_Time_(2);
                                                if (cancel_field != 0) break;
                                                if (NETMOX::Net_May_Begin_Human_Diplomacy_() != 0) break;
                                                if (net_flags != 0) break;
                                                if (NETMOX::Human_Hit_Next_Turn_(target_player_idx) != 0) break;
                                            }
                                        }
                                    }
                                }
                                _net_state = 0;
                                if (NETMOX::Human_Hit_Next_Turn_(target_player_idx) != 0) {
                                    JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 55, message_buffer, 80);
                                    GENDRAW::Message_Box_(message_buffer);
                                    NETMOX::Net_End_Human_Diplomacy_();
                                } else {
                                    if (cancel_field != 0) {
                                        JIM::Get_Text_Message_(s_billtext_lbx_0055135c, 56, message_buffer, 80);
                                        GENDRAW::Message_Box_(message_buffer);
                                        NETMOX::Net_End_Human_Diplomacy_();
                                    } else {
                                        if (NETMOX::Net_May_Begin_Human_Diplomacy_() != 0) {
                                            DIP_SCRN::Diplomacy_Screen_(target_player_idx);
                                            NETMOX::Net_End_Human_Diplomacy_();
                                        } else {
                                            NETMOX::Net_Cancel_Human_Diplomacy_Request_();
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    } else {
                        state_diplomacy = 0;
                        state_report = 0;
                        state_ignoring = 0;
                        state_war = 0;

                        s_racescrn_mouse_group* spy_group = Get_Spy_Group_For_Field_(input, &selected_race_idx);
                        
                        if (_g_in_spy_mode) {
                            if (spy_group == nullptr) {
                                spy_group = active_spy_group;
                            }
                            if (spy_group != active_spy_group) {
                                if (active_spy_group == &agent_group) {
                                    race_display_data[selected_race_idx].agent_mission_idx = old_mission_idx;
                                }
                            }
                            Move_Spy_Group_From_Mouse_(spy_group);
                            if (_g_mouse_group_p->count == 0) {
                                _g_in_spy_mode = 0;
                                active_spy_group = nullptr;
                            }
                        } else {
                            if (spy_group != nullptr && spy_group->count > 0) {
                                Move_Spies_Group_(spy_group, _arrow_seg);
                                _g_in_spy_mode = 1;
                                old_mission_idx = race_display_data[selected_race_idx].agent_mission_idx;
                                active_spy_group = spy_group;
                            }
                        }
                        Adjust_Spy_Mission_Data_();
                    }
                } else {
                    if (_g_in_spy_mode) {
                        Move_Spy_Group_From_Mouse_(active_spy_group);
                        if (active_spy_group != &agent_group) {
                            race_display_data[selected_race_idx].agent_mission_idx = old_mission_idx;
                        }
                        if (_g_mouse_group_p->count == 0) {
                            _g_in_spy_mode = 0;
                        }
                    }

                    if (_g_in_who_mode) {
                        JIM::Bill_Picture_To_Mouse_(_g_mouse_seg);
                        state_diplomacy = 0;
                        state_report = 0;
                        state_ignoring = 0;
                        state_war = 0;
                        Setup_Race_Display_Fields_(race_display_data, display_base);
                        hidden_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
                        _g_in_who_mode = 0;
                    } else {
                        if (input == btn_ignoring) {
                            state_diplomacy = 0; state_report = 0; state_war = 0;
                        } else if (input == btn_report) {
                            state_diplomacy = 0; state_ignoring = 0; state_war = 0;
                        } else if (input == btn_diplomacy) {
                            state_report = 0; state_ignoring = 0; state_war = 0;
                        } else if (input == btn_war) {
                            state_report = 0; state_ignoring = 0; state_diplomacy = 0;
                        }

                        active_action_field = input;
                        fields::Enable_Draw_All_Fields_();
                        fields::Draw_Visible_Fields_();
                        fields::Disable_Draw_All_Fields_();
                        fields::Draw_All_Fields_();
                        video::Copy_Off_To_Back_();

                        Setup_Race_Who_Fields_(race_display_data, display_base);
                        hidden_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
                        COLONY::Picture_To_Mouse_(mouse_pic);
                        _g_in_who_mode = 1;
                    }
                }
            }
        }
    }
}