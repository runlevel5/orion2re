#include "pch.h"

namespace DIP_SCRN {
    s_mouse_list _diplomacy_hand_mouse = { 19, 0, 0, 0, 639, 479 };
    s_mouse_list _diplomacy_blank_mouse = { 0, 0, 0, 0, 639, 479 };
    uint8_t _ambassador_option;
    int16_t _main_diplomacy_help_entry[8] = { 567, 568, 569, 570, 571, 572, 573, 574 };
    int16_t _response_message;
    uint16_t _synch_up_established_flag;
    int16_t _old_response_message;
    int16_t _propose_treaty_help_entry[7] = { 576, 577, 578, 579, 580, 613, 0 };
    char* _main_diplomacy_title;
    char* _propose_treaty_title;
    uint8_t _talker_ambassador;
    int16_t _diplomacy_current_music;
    int16_t _net_diplomacy_esc_pressed;
    uint8_t _target_tech_list[212];
    uint8_t _listen_ambassador;
    int16_t _exchange_tech_count;
    char* _main_diplomacy_list[9];
    char* _propose_treaty_diplomacy_list[7];
    uint8_t _exchange_tech_list[212];
    char _chat_message[100];
    int16_t _diplomacy_quit_chat_field;
    int16_t _net_diplomacy_treaty_type;
    int16_t _diplomacy_chat_field;
    char _net_diplomacy_chat_message[100];
    char* _player_number_name_buffer;
    char* _player_name_ambassador_buffer;
    char _receive_chat_message[110];
    uint8_t _current_ambassador;
    char _diplomacy_message[250];
    int16_t _main_net_diplomacy_help_entry[10] = { 567, 568, 569, 570, 571, 572, 573, -1, -1, 574 };
    int16_t _net_diplomacy_tech_offered;
    const char s_empty_string_00551100[] = "";
    const char s_empty_string[] = "";
    const char s_empty[] = "";
    const char s_jimtext_lbx[] = "jimtext.lbx";
    const char s__00551100[] = "";
    const char s_jimtext2_lbx_00552340[] = "jimtext2.lbx";
    const char s_jimtext2_lbx[] = "jimtext2.lbx";
    const char s_jimtext_lbx_00552360[] = "jimtext.lbx";
    const char s__d_BC_00552384[] = "  %d BC";
    const char s_fmt_s_0055236c[] = "  %s";
    const char s__s_0055236c[] = "  %s";
    const char s_fmt_d_bc_00552384[] = "  %d BC";
    const char s_fmt_BC[] = "%d BC";
    const char s_fmt_comma[] = "%d,%d";
    const char s_fmt_dot[] = "%d.%d";
    const char s_FWEE[] = "FWEE!";
    const char s_DIPLOMSP_LBX[] = "DIPLOMSP.LBX";
    const char s_DIPLOMSI_LBX[] = "DIPLOMSI.LBX";
    const char s_DIPLOMSS_LBX[] = "DIPLOMSS.LBX";
    const char s_DIPLOMSF_LBX[] = "DIPLOMSF.LBX";
    const char s_DIPLOMSG_LBX[] = "DIPLOMSG.LBX";
    const char s_DIPLOMSE_LBX[] = "DIPLOMSE.LBX";
    uint16_t _chat_width;
    uint16_t _chat_base_y;
    uint16_t _chat_base_x;
    uint16_t _chat_height;
    uint8_t _chat_mode_flag;
    s_colors _diplomacy_normal_colors = {{248, 253, 253, 253, 253, 253, 253, 253}};
    s_colors _diplomacy_highlight_colors = {{250, 255, 255, 255, 255, 255, 255, 255}};
    s_colors _diplomacy_special_colors = {{248, 250, 250, 250, 250, 250, 250, 250}};
    int16_t _break_treaty_help_entry[7] = { 582, 583, 584, 585, 586, 613, -1 };
    char* _break_treaty_title;
    char* _break_treaty_diplomacy_list[7];
    char _diplomacy_list_choice[15][50];
    s_help_box _jim_help_list[15];
    int16_t _demand_help_entry[11] = { 588, 589, 590, 591, 592, 593, 594, 595, 596, 613, 0 };
    int16_t _main_repulsive_diplomacy_help_entry[7] = { 578, 572, 573, 574, 0, 0, 0 };
    int16_t _main_net_repulsive_diplomacy_help_entry[7] = { 578, 572, 573, -1, 574, 0, 0 };
    char* _main_repulsive_diplomacy_list[7];
    char* _main_net_repulsive_diplomacy_list[6];
    s_dipscrn_net_message _net_diplomacy_message;
    int16_t _offer_help_entry[7] = { 598, 599, 600, 601, 602, 613, -1 };
    char* _offer_diplomacy_list[7];
    char* _offer_title;
    uint8_t _diplomacy_system_planet_list[5];
    int16_t _diplomacy_system_population;
    uint8_t _diplomacy_system_receiver;
    int16_t _diplomacy_system_planet_count;
    int16_t _diplomacy_scan_input_flag;
    uint8_t _diplomacy_system_giver;
    char* _demand_diplomacy_list[11];
    char* _net_listen_mode_option_buffer;
    char* _system_unsurrenderable_buffer;
    char* _main_net_diplomacy_list[11];
    char* _demand_title;
    char* _agree_diplomacy_list[4];
    char* _system_cancel_buffer;
    int16_t _diplomacy_system_homeworld_flag;
    s_animation_header* _diplomacy_system_star_background_seg;
    int16_t _diplomacy_system_fields[72];
    uint8_t* _diplomacy_mouse_bitmap;
    char* _system_homeworld_buffer;
    int16_t _diplomacy_good_music;
    char* _statement_research_treaty_buffer;
    int16_t _diplomacy_bad_music;
    char* _choose_a_system_buffer;
    char* _enter_your_message_buffer;
    char* _statement_trade_treaty_buffer;
    char* _system_million_colonists_buffer;
    char* _net_listen_mode_good_bye_buffer;
    char* _system_x_star_system_buffer;
    char* _percent_tribute_treaty_buffer;
    char* _you_were_framed_buffer;

    void __cdecl Diplomacy_Load_New_Ambassador_(int16_t player_idx) {
        mouse::Set_Mouse_List_(&DIP_SCRN::_diplomacy_blank_mouse, 1);
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Percent_(10);
        ERIC::Fast_Fade_Out_();
        DIP_SCRN::Diplomacy_Mouse_Init_(player_idx);

        uint8_t race = (uint8_t)MOX::_player[player_idx].race;

        file_ani::Open_File_Animation_(s_DIPLOMAT_LBX, (uint16_t)race * 2 + 13);
        video::Set_Page_Off_();
        file_ani::Draw_File_Animation_();
        video::Copy_Off_To_Back_();

        DIP_SCRN::_current_ambassador = (uint8_t)player_idx;

        file_ani::Open_File_Animation_(s_DIPLOMAT_LBX, (uint16_t)race * 2 + 14);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        file_ani::Draw_File_Animation_Stencil_();
    }

    /**
     * @brief Prepares a diplomatic gift offer message based on the gift type.
     * 
     * @param player_idx The index of the player offering the gift.
     * @param offer_type The type of gift (1: BC, 2: Peace/Treaty, 3: Tech, 4: System).
     * @param value The amount of BC, tech index, or star system index.
     */
    void __cdecl Get_Human_Offer_Gift_Message_(int16_t player_idx, int16_t offer_type, int16_t value) {
        switch (offer_type) {
            case 1:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 18 /* 0x12 */, _diplomacy_message, 250);
                JIM::Decode_Text_Name_And_Stat_(_diplomacy_message, player_idx, value, 250);
                break;

            case 2:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 15 /* 0x0F */, _diplomacy_message, 250);
                if (value == 1) {
                    JIM::Decode_Text_Name_And_Stat_(_diplomacy_message, player_idx, 5, 250);
                } else {
                    JIM::Decode_Text_Name_And_Stat_(_diplomacy_message, player_idx, 10, 250);
                }
                break;

            case 3:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 16 /* 0x10 */, _diplomacy_message, 250);
                JIM::Decode_Text_Name_And_Tech_(_diplomacy_message, player_idx, value, 250);
                break;

            case 4:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 17 /* 0x11 */, _diplomacy_message, 250);
                JIM::Decode_Text_Name_And_System_(_diplomacy_message, player_idx, value, 250);
                break;

            default:
                break;
        }
    }

    void __cdecl Diplomacy_Break_Treaty_Resolution_(int16_t sender_id, int16_t target_id, int16_t treaty_type) {
        if (MOX::_player[target_id].objectives == 100 /* 'd' */) {
            if (MOX::_game_type == 1) {
                Diplomacy_Load_New_Ambassador_(sender_id);
                Get_Human_Break_Treaty_Message_(target_id, treaty_type);
                _response_message = 0;
                _old_response_message = 0;
                Diplomacy_Display_Response_();
                Diplomacy_Load_New_Ambassador_(target_id);
            } else {
                _net_diplomacy_message.sender_player_id = (uint8_t)sender_id;
                _net_diplomacy_message.target_player_id = (uint8_t)target_id;
                _net_diplomacy_message.arg1 = treaty_type;
                _net_diplomacy_message.msg_type = 1001 /* 0x3e9 */;
                _net_diplomacy_message.arg2 = _response_message;
                NETMOX::Mox_Send_Message_(MOX::_player[target_id].network_player_id, 2000, &_net_diplomacy_message, 8);
                RUSS::Mox_Update_();
            }
        }

        switch (treaty_type) {
            case 1:
            case 2:
                diplomac::Break_Treaties_(sender_id, target_id);
                if (MOX::_player[target_id].objectives == 100 /* 'd' */) {
                    _response_message = 153 /* 0x99 */;
                } else {
                    diplomac::Get_Break_Treaty_Message_(sender_id, target_id, &_response_message);
                }
                break;

            case 7:
                diplomac::Break_Trade_(sender_id, target_id);
                if (MOX::_player[target_id].objectives == 100 /* 'd' */) {
                    _response_message = 156 /* 0x9c */;
                } else {
                    diplomac::Get_Break_Trade_Message_(sender_id, target_id, &_response_message);
                }
                break;

            case 8:
                diplomac::Break_Research_(sender_id, target_id);
                if (MOX::_player[target_id].objectives == 100 /* 'd' */) {
                    _response_message = 158 /* 0x9e */;
                } else {
                    diplomac::Get_Break_Research_Message_(sender_id, target_id, &_response_message);
                }
                break;

            case 9:
            case 10:
                diplomac::Break_Tribute_(sender_id, target_id);
                if (MOX::_player[target_id].objectives == 100 /* 'd' */) {
                    _response_message = 171 /* 0xab */;
                } else {
                    diplomac::Get_Break_Tribute_Treaty_Message_(sender_id, target_id, &_response_message);
                }
                break;

            default:
                break;
        }
    }

    int16_t __cdecl Diplomacy_Declare_War_Resolution_(int16_t sender_id, int16_t target_id) {
        char buffer1[400] = { 0 };
        char buffer2[80];
        int16_t result = 0;

        Setup_Full_Screen_Diplomacy_Help_(0x266);

        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x3B, buffer1, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x3C, buffer2, 0x50);

        if (Diplomacy_Generic_List_(1, buffer1, 1, buffer2) != -1) {
            result = 1;

            if (MOX::_player[target_id].objectives == 100 /* 'd' */) {
                if (MOX::_game_type == 1) {
                    Diplomacy_Load_New_Ambassador_(sender_id);
                    Get_Human_Declare_War_Message_(sender_id);
                    Diplomacy_Display_Response_();
                    Diplomacy_Load_New_Ambassador_(target_id);
                }
                else {
                    _net_diplomacy_message.arg2 = (int16_t)_response_message;
                    _net_diplomacy_message.sender_player_id = (uint8_t)sender_id;
                    _net_diplomacy_message.target_player_id = (uint8_t)target_id;
                    _net_diplomacy_message.arg1 = 0;
                    _net_diplomacy_message.msg_type = 0x3EA;

                    NETMOX::Mox_Send_Message_(MOX::_player[target_id].network_player_id, 2000, &_net_diplomacy_message, 8);
                    RUSS::Mox_Update_();
                }
                _response_message = 0xA0;
            }
            else {
                diplomac::Get_Declare_War_Message_(sender_id, target_id, &_response_message);
            }

            diplomac::Declare_War_(sender_id, target_id);
            diplomac::Adjust_Diplomat_Modifiers_(sender_id, target_id);
        }

        return result;
    }

    int16_t __cdecl Diplomacy_Surrender_(int16_t sender_player_id, int16_t target_player_id) {
        int16_t status = 0;
        char msg6_buffer[40];
        char msg7_buffer[80];

        Setup_Full_Screen_Diplomacy_Help_(612);

        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 6, msg6_buffer, 40);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 7, msg7_buffer, 80);

        if (DIP_SCRN::Diplomacy_Generic_List_(1, msg6_buffer, 1, msg7_buffer) != -1) {
            if (COLCALC::May_Surrender_Empire_(sender_player_id, target_player_id)) {
                COLCALC::Surrender_Empire_(sender_player_id, target_player_id);
            }

            status = 1;
            s_player* target_plr = &MOX::_player[target_player_id];

            if (target_plr->objectives == 100) {
                if (MOX::_game_type == 1) {
                    DIP_SCRN::Diplomacy_Load_New_Ambassador_(sender_player_id);
                    DIP_SCRN::Get_Human_Surrender_Message_(sender_player_id);
                    DIP_SCRN::Diplomacy_Display_Response_();
                    DIP_SCRN::Diplomacy_Load_New_Ambassador_(target_player_id);
                } else {
                    DIP_SCRN::_net_diplomacy_message.arg2 = DIP_SCRN::_response_message;
                    DIP_SCRN::_net_diplomacy_message.sender_player_id = (uint8_t)sender_player_id;
                    DIP_SCRN::_net_diplomacy_message.target_player_id = (uint8_t)target_player_id;
                    DIP_SCRN::_net_diplomacy_message.arg1 = 0;
                    DIP_SCRN::_net_diplomacy_message.msg_type = 1003 /* 0x3eb */;

                    NETMOX::Mox_Send_Message_(target_plr->network_player_id, 2000, &DIP_SCRN::_net_diplomacy_message, 8);
                    RUSS::Mox_Update_();
                }
            }
        }

        return status;
    }

    void __cdecl Diplomacy_Chat_(int16_t player_idx, uint8_t target_player_id) {
        if (DIP_SCRN::_chat_mode_flag == 0) {
            DIP_SCRN::Setup_Full_Screen_Diplomacy_Help_(0x2b6 /* HELP_NET_CHAT */);
            fields::Clear_Fields_();
            DIP_SCRN::_chat_mode_flag = 1;
            fonts::Set_Font_Style_Shadow_Down_(4, &DIP_SCRN::_diplomacy_normal_colors);

            DIP_SCRN::_chat_message[0] = *DIP_SCRN::s__00551100;
            fields::_continuous_string[0] = DIP_SCRN::s__00551100[0];

            DIP_SCRN::_diplomacy_quit_chat_field = fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, DIP_SCRN::s__00551100, 0);

            DIP_SCRN::_diplomacy_chat_field = fields::Add_Continuous_String_Input_Field_(
                DIP_SCRN::_chat_base_x,
                DIP_SCRN::_chat_base_y,
                DIP_SCRN::_chat_width - 20,
                DIP_SCRN::_chat_height,
                DIP_SCRN::_chat_message,
                0x50, // Max 80 chars
                0,
                0,
                0x29, // Font ID
                0
            );

            fields::_active_input_field_first = 1;
            fields::_active_input_field_number = DIP_SCRN::_diplomacy_chat_field;
            fields::_input_field_active = 1;
            fields::_cursor_position = 0;
            fields::_cursor_on = 0;
        } else {
            if (DIP_SCRN::_net_diplomacy_esc_pressed == 1) {
                DIP_SCRN::_chat_mode_flag = 0;
            }

            int16_t input_field = fields::Get_Input_();

            if (DIP_SCRN::_net_diplomacy_esc_pressed == 1) {
                DIP_SCRN::_chat_mode_flag = 0;
            }

            if (input_field == DIP_SCRN::_diplomacy_chat_field || input_field == DIP_SCRN::_diplomacy_quit_chat_field) {
                fields::Clear_Fields_();
                DIP_SCRN::_chat_mode_flag = 0;

                strcpy(DIP_SCRN::_net_diplomacy_chat_message, DIP_SCRN::_chat_message);

                NETMOX::Mox_Send_Message_(
                    MOX::_player[player_idx].network_player_id,
                    0x834 /* MSG_NET_CHAT */,
                    DIP_SCRN::_net_diplomacy_chat_message,
                    100
                );

                RUSS::Mox_Update_();
                DIP_SCRN::_net_diplomacy_chat_message[0] = *DIP_SCRN::s__00551100;
            }
        }
    }

    void __cdecl Diplomacy_Main_Net_Chat_(int16_t param_1, int16_t param_2) {
        do {
            timer::Mark_Time_();
            DIP_SCRN::Diplomacy_Chat_(param_1, (uint8_t)param_2);

            if (DIP_SCRN::_chat_mode_flag != 1) {
                break;
            }

            fields::Invoke_Auto_Function_();
            fonts::Set_Font_Style_Shadow_Down_(4, &DIP_SCRN::_diplomacy_normal_colors);
            fonts::Print_(10, 117, DIP_SCRN::_enter_your_message_buffer);

            graphics::Darken_Fill_(DIP_SCRN::_chat_base_x,
                                   DIP_SCRN::_chat_base_y,
                                   DIP_SCRN::_chat_base_x + DIP_SCRN::_chat_width,
                                   DIP_SCRN::_chat_base_y + DIP_SCRN::_chat_height - 1);

            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(2);

        } while (DIP_SCRN::_chat_mode_flag == 1);
    }

    /**
     * @address 00418170
     */
    int16_t __cdecl Check_Human_Treaty_Proposal_(int16_t player_id, int16_t target_player_id, int16_t treaty_type) {
        int16_t choice;

        switch (treaty_type) {
        case 1:
            _response_message = 0x61;
            break;
        case 2:
            _response_message = 0x62;
            break;
        case 3:
            _response_message = 0x53;
            break;
        case 7:
            _response_message = 0x63;
            break;
        case 8:
            _response_message = 0x64;
            break;
        }

        if (MOX::_game_type == 1 /* SINGLE_PLAYER */) {
            DIP_SCRN::Diplomacy_Load_New_Ambassador_(player_id);
            choice = DIP_SCRN::Get_Human_Treaty_Choice_(player_id, target_player_id, treaty_type);
            DIP_SCRN::Diplomacy_Load_New_Ambassador_(target_player_id);
            return choice;
        }

        DIP_SCRN::_net_diplomacy_message.sender_player_id = (uint8_t)player_id;
        DIP_SCRN::_net_diplomacy_message.target_player_id = (uint8_t)target_player_id;
        DIP_SCRN::_net_diplomacy_message.arg1 = treaty_type;
        DIP_SCRN::_net_diplomacy_message.msg_type = 1000;
        DIP_SCRN::_net_diplomacy_message.arg2 = (int16_t)_response_message;

        NETMOX::Mox_Send_Message_(MOX::_player[target_player_id].network_player_id, 2000, &DIP_SCRN::_net_diplomacy_message, 8);
        RUSS::Mox_Update_();

        DIP_SCRN::Get_Listen_Mode_Message_(target_player_id);
        DIP_SCRN::_net_diplomacy_message.arg2 = -1;

        do {
            DIP_SCRN::Diplomacy_Listen_Mode_(target_player_id, (uint8_t)player_id);
            if (DIP_SCRN::_net_diplomacy_message.arg2 != -1) {
                break;
            }
        } while (DIP_SCRN::_net_diplomacy_esc_pressed == 0);

        if (DIP_SCRN::_net_diplomacy_esc_pressed == 1) {
            DIP_SCRN::_net_diplomacy_message.arg2 = 0;
        }

        return DIP_SCRN::_net_diplomacy_message.arg2;
    }

    void __cdecl Get_Diplomacy_Tech_List_(int16_t player_a_idx, int16_t player_b_idx, int16_t* tech_count_ptr, uint8_t* tech_list_buffer) {
        int16_t local_count;
        uint8_t local_tech_list[212];
        int32_t local_buffer[212]; // Buffer for NPC diplomacy calculations

        // Initialize output buffer
        memset(tech_list_buffer, 0, 10);

        aitech::Get_Max_Technology_Value_(player_a_idx);

        // Get list of technologies that player B has but player A does not
        npcdiplo::Get_Differential_Tech_List_(
            player_b_idx,
            player_a_idx,
            0,
            (int32_t)0xC343CB00,
            local_tech_list,
            local_buffer,
            &local_count,
            1
        );

        // Diplomacy UI can only display up to 10 technologies for trade
        if (local_count > 10) {
            local_count = 10;
        }

        *tech_count_ptr = local_count;

        if (local_count > 0) {
            memcpy(tech_list_buffer, local_tech_list, local_count);
        }
    }

    int16_t __cdecl Get_Human_Treaty_Choice_(int16_t player_a, int16_t player_b, int16_t treaty_type) {
        char choice_text[3][40];
        char title_text[40];
        int16_t num_choices = 2;
        int16_t selected_idx;
        int16_t result = 0;

        JIM::Get_Text_Message_(JIM::jimtext_lbx, 0x4F, choice_text[0], 40);
        JIM::Get_Text_Message_(JIM::jimtext_lbx, 0x50, choice_text[1], 40);

        choice_text[2][0] = DIP_SCRN::s_empty[0];

        if (DIP_SCRN::Check_Better_Offer_Available_(player_a, player_b) == 1) {
            num_choices = 3;
            JIM::Get_Text_Message_(JIM::jimtext_lbx, 0x52, choice_text[2], 40);
        }

        DIP_SCRN::_response_message = 0;
        DIP_SCRN::_old_response_message = 0;

        DIP_SCRN::Get_Human_Propose_Treaty_Message_(treaty_type);
        fields::Deactivate_Help_List_();

        JIM::Get_Text_Message_(JIM::jimtext_lbx, 0x4A, title_text, 40);

        selected_idx = DIP_SCRN::Diplomacy_Generic_List_(num_choices, (char*)choice_text, 0, title_text);

        if (num_choices == 2 && selected_idx == -1) {
            selected_idx = 1;
        } else if (num_choices == 3 && selected_idx == -1) {
            selected_idx = 2;
        }

        if (selected_idx == 0) {
            result = 3;
            switch (treaty_type) {
            case 1: DIP_SCRN::_response_message = 0x83; break;
            case 2: DIP_SCRN::_response_message = 0x84; break;
            case 3: DIP_SCRN::_response_message = 0x87; break;
            case 7: DIP_SCRN::_response_message = 0x85; break;
            case 8: DIP_SCRN::_response_message = 0x86; break;
            default: break;
            }
        } else if (selected_idx == 1) {
            DIP_SCRN::_response_message = 0x8A;
            result = 0;
        } else if (selected_idx == 2) {
            result = 1;
        }

        return result;
    }

    void __cdecl Get_Listen_Mode_Message_(int16_t race_idx) {
        _response_message = 0;
        _old_response_message = 0;

        JIM::Get_Text_Message_(s_jimtext2_lbx, 12, _diplomacy_message, 250);
        JIM::Decode_Text_Race_(_diplomacy_message, race_idx, 250);
    }

    void __cdecl Diplomacy_Listen_Mode_(int16_t player_idx, uint8_t target_player_id) {
        int16_t field_cancel;
        int16_t field_goodbye;
        int16_t hotkey_space;
        int16_t input_field;

        fields::Assign_Auto_Function_(Draw_Diplomacy_Listen_Mode_, 2);
        timer::Mark_Time_();

        if (_chat_mode_flag == 0) {
            _diplomacy_chat_field = (int16_t)0xfc18 /* -1000 */;
            fields::Clear_Fields_();
            Setup_Full_Screen_Diplomacy_Help_(0x2b6);

            field_cancel = fields::Add_Hidden_Field_(15, 139, 157, 162, s_C_00552420, 0);
            field_goodbye = fields::Add_Hidden_Field_(15, 165, 157, 188, s_G_0055241c, 0);
            hotkey_space = fields::Add_Hot_Key_(s__00551120);

            input_field = fields::Get_Input_();

            if (input_field == field_goodbye || input_field == hotkey_space) {
                _net_diplomacy_message.sender_player_id = (uint8_t)player_idx;
                _net_diplomacy_message.target_player_id = target_player_id;
                _net_diplomacy_message.arg2 = -1;
                _net_diplomacy_message.msg_type = 1006 /* 0x3ee */;

                NETMOX::Mox_Send_Message_(
                    MOX::_player[player_idx].network_player_id,
                    2000,
                    &_net_diplomacy_message,
                    8
                );
                RUSS::Mox_Update_();
                _net_diplomacy_esc_pressed = 1;
            }

            if (input_field == field_cancel) {
                Diplomacy_Chat_(player_idx, target_player_id);
            }
        } else {
            Diplomacy_Chat_(player_idx, target_player_id);
        }

        Draw_Diplomacy_Listen_Mode_();

        if (_chat_mode_flag == 1) {
            graphics::Darken_Fill_(
                _chat_base_x,
                _chat_base_y,
                _chat_base_x + _chat_width,
                _chat_base_y + _chat_height - 1
            );
        }

        RUSS::Mox_Sync_Update_();
        timer::Release_Time_(2);
        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
    }

    void __cdecl Get_Human_Declare_War_Message_(int16_t race_idx) {
        JIM::Get_Text_Message_(s_jimtext2_lbx, 14, _diplomacy_message, 250);
        JIM::Decode_Text_Race_(_diplomacy_message, (int16_t)race_idx, 250);
    }

    void __cdecl Get_Human_Break_Treaty_Message_(int16_t race_idx, int16_t treaty_type) {
        switch (treaty_type) {
            case 1:
                JIM::Get_Text_Message_(s_jimtext2_lbx, 30, _diplomacy_message, 250);
                JIM::Decode_Text_Race_(_diplomacy_message, race_idx, 250);
                break;
            case 2:
                JIM::Get_Text_Message_(s_jimtext2_lbx, 31, _diplomacy_message, 250);
                JIM::Decode_Text_Race_(_diplomacy_message, race_idx, 250);
                break;
            case 7:
                JIM::Get_Text_Message_(s_jimtext2_lbx, 27, _diplomacy_message, 250);
                JIM::Decode_Text_Race_(_diplomacy_message, race_idx, 250);
                break;
            case 8:
                JIM::Get_Text_Message_(s_jimtext2_lbx, 28, _diplomacy_message, 250);
                JIM::Decode_Text_Race_(_diplomacy_message, race_idx, 250);
                break;
            case 9:
            case 10:
                JIM::Get_Text_Message_(s_jimtext2_lbx, 29, _diplomacy_message, 250);
                JIM::Decode_Text_Race_(_diplomacy_message, race_idx, 250);
                break;
            default:
                break;
        }
    }

    void __cdecl Get_Human_Propose_Treaty_Message_(int16_t treaty_type) {
        switch (treaty_type) {
            case 1:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 45, _diplomacy_message, 250);
                break;
            case 2:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 46, _diplomacy_message, 250);
                break;
            case 3:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 66, _diplomacy_message, 250);
                break;
            case 7:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 42, _diplomacy_message, 250);
                break;
            case 8:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 43, _diplomacy_message, 250);
                break;
            case 9:
            case 10:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 44, _diplomacy_message, 250);
                break;
            default:
                break;
        }
    }

    void __cdecl Get_Human_Surrender_Message_(int16_t race_idx) {
        JIM::Get_Text_Message_(DIP_SCRN::s_jimtext2_lbx, 13, DIP_SCRN::_diplomacy_message, 250);
        JIM::Decode_Text_Race_(DIP_SCRN::_diplomacy_message, race_idx, 250);
    }

    void __cdecl Draw_Diplomacy_Listen_Mode_(void) {
        Draw_Diplomacy_Synch_Mode_();
        fonts::Set_Font_Style_Shadow_Down_(4, &_diplomacy_normal_colors);

        if (_chat_mode_flag == 0) {
            fonts::Print_(10, 117 /* 0x75 */, _net_listen_mode_option_buffer);
            fonts::Print_(10, 141 /* 0x8D */, _main_net_diplomacy_list[7]);
            fonts::Print_(18 /* 0x12 */, 166 /* 0xA6 */, _net_listen_mode_good_bye_buffer);
        } else {
            fonts::Print_(10, 117 /* 0x75 */, _enter_your_message_buffer);
        }
    }

    /**
     * @brief Checks if there are better offers (systems, tech, or significant BC) available from a player.
     * 
     * @param player_idx The index of the player offering.
     * @param other_player_idx The index of the player being offered to.
     * @return 1 if a better offer is available, 0 otherwise.
     */
    int16_t __cdecl Diplomacy_System_Display_(int16_t giver_idx, int16_t receiver_idx, int16_t display_mode)
    {
        int16_t y, x;
        int16_t exit_field_id;
        int16_t input;
        int16_t scanned_input;
        int16_t result_star_idx = -1;
        bool done = false;
        s_animation_header* saved_star_bg_seg;

        MOX::_current_screen = 6;
        saved_star_bg_seg = MOX::_star_bg_seg;
        MOX::_star_bg_seg = DIP_SCRN::_diplomacy_system_star_background_seg;
        DIP_SCRN::_diplomacy_system_giver = (uint8_t)giver_idx;
        DIP_SCRN::_diplomacy_system_receiver = (uint8_t)receiver_idx;
        COLONY::_using_colony_screen_palette = 1;
        MOX::_star_bg_index = 0;

        if (display_mode == 1) {
            DIP_SCRN::Setup_Full_Screen_Diplomacy_Help_(0x261);
        } else {
            DIP_SCRN::Setup_Full_Screen_Diplomacy_Help_(0x268);
        }

        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(DIP_SCRN::Draw_Diplomacy_System_Display_, 2);

        int16_t text_msg_idx = (display_mode == 1) ? 0x3F : 0x37;
        JIM::Get_Text_Message_(DIP_SCRN::s_jimtext2_lbx, text_msg_idx, DIP_SCRN::_diplomacy_message, 0xFA);
        JIM::Decode_Text_Name_And_Name_(DIP_SCRN::_diplomacy_message, receiver_idx, giver_idx, 0xFA);

        DIP_SCRN::_diplomacy_scan_input_flag = -1;
        fields::Clear_Fields_();

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            MOVEBOX::Get_Galaxy_Map_Star_XY_(i, 10, 0xAB, 0xB4, 0x6E, 0, 0, 1, &x, &y);
            int16_t field_id = fields::Add_Hidden_Field_(x, y, x + 5, y + 5, s__00551100, 0);
            DIP_SCRN::_diplomacy_system_fields[i + 1] = field_id;
        }

        exit_field_id = fields::Add_Hidden_Field_(0xF, 0x8B, 0x9D, 0xA2, s__00551100, 0);

        while (true) {
            timer::Mark_Time_();
            DIP_SCRN::_diplomacy_scan_input_flag = -1;
            input = fields::Get_Input_();
            scanned_input = fields::Scan_Input_();
            MOX::_galaxy_map_scanned_star = -1;
            DIP_SCRN::_diplomacy_scan_input_flag = scanned_input;

            if (scanned_input > 0) {
                if (scanned_input == exit_field_id) {
                    DIP_SCRN::_diplomacy_scan_input_flag = 1000;
                } else if (scanned_input >= DIP_SCRN::_diplomacy_system_fields[1] && scanned_input <= DIP_SCRN::_diplomacy_system_fields[MOX::_NUM_STARS]) {
                    if (DIP_SCRN::Diplomacy_System_Scan_(scanned_input - 1, giver_idx, &DIP_SCRN::_diplomacy_system_homeworld_flag, &DIP_SCRN::_diplomacy_system_planet_count, DIP_SCRN::_diplomacy_system_planet_list) == giver_idx) {
                        MOX::_galaxy_map_scanned_star = scanned_input - 1;
                        DIP_SCRN::_diplomacy_system_population = COLCALC::Get_System_Population_(giver_idx, MOX::_galaxy_map_scanned_star);
                        JIM::Get_Text_Message_(DIP_SCRN::s_jimtext2_lbx, 0x38, DIP_SCRN::_system_million_colonists_buffer, 0x1E);
                        JIM::Decode_Text_Stat_(DIP_SCRN::_system_million_colonists_buffer, DIP_SCRN::_diplomacy_system_population, 0x1E);
                        JIM::Get_Text_Message_(DIP_SCRN::s_jimtext2_lbx, 0x3A, DIP_SCRN::_system_x_star_system_buffer, 0x23);
                        JIM::Decode_Text_System_(DIP_SCRN::_system_x_star_system_buffer, MOX::_galaxy_map_scanned_star, 0x23);
                    } else {
                        DIP_SCRN::_diplomacy_scan_input_flag = -1;
                    }
                } else {
                    DIP_SCRN::_diplomacy_scan_input_flag = -1;
                }
            } else {
                DIP_SCRN::_diplomacy_scan_input_flag = -1;
            }

            if (input == exit_field_id || DIP_SCRN::_net_diplomacy_esc_pressed == 1) {
                done = true;
            }

            if (input != 0 && !done) {
                DIP_SCRN::_diplomacy_scan_input_flag = -1;
                for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                    if (done) break;
                    if (DIP_SCRN::_diplomacy_system_fields[i + 1] == input) {
                        if (DIP_SCRN::Diplomacy_System_Scan_(input - 1, giver_idx, &DIP_SCRN::_diplomacy_system_homeworld_flag, &DIP_SCRN::_diplomacy_system_planet_count, DIP_SCRN::_diplomacy_system_planet_list) == giver_idx) {
                            if (COLCALC::May_Surrender_Star_(input - 1, (int16_t)DIP_SCRN::_diplomacy_system_giver, (int16_t)DIP_SCRN::_diplomacy_system_receiver)) {
                                if (DIP_SCRN::_diplomacy_system_homeworld_flag == -1) {
                                    done = true;
                                    result_star_idx = i;
                                }
                            }
                        }
                    }
                }
            }

            if (done) break;

            DIP_SCRN::Draw_Diplomacy_System_Display_();
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(2);
        }

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        MOX::_star_bg_seg = saved_star_bg_seg;
        COLONY::_using_colony_screen_palette = 0;

        return result_star_idx;
    }

    int16_t __cdecl Check_Better_Offer_Available_(int16_t player_idx, int16_t other_player_idx) {
        if (Check_Surrender_System_(player_idx, other_player_idx)) {
            return 1;
        }

        int16_t tech_count;
        uint8_t tech_list[12];
        Get_Diplomacy_Tech_List_(player_idx, other_player_idx, &tech_count, tech_list);

        if (tech_count > 0) {
            return 1;
        }

        // Check if player has more than 100 BC
        if (MOX::_player[player_idx].bc > 100) {
            return 1;
        }

        return 0;
    }

    void __cdecl Draw_Diplomacy_System_() {
        graphics::Box_(9, 0xaa, 0xb6, 0x70, 1, 0xfb);
        graphics::Darken_Fill_(0xa, 0xab, 0xbe, 0x118);
        MOVEBOX::Draw_Galaxy_Map_Box_(nullptr, nullptr, 0xa, 0xab, 0xb4, 0x6e, 0, 0, 0, 0, 4, 0);
    }

    void __cdecl Draw_Diplomacy_System_Display_() {
        Draw_Diplomacy_Synch_Mode_();
        Draw_Diplomacy_System_();

        fonts::Set_Font_Style_Shadow_Down_(4, &_diplomacy_normal_colors);
        fonts::Print_(10, 0x75, _choose_a_system_buffer);

        if (_diplomacy_scan_input_flag == 1000) {
            fonts::Set_Font_Style_Shadow_Down_(4, &_diplomacy_highlight_colors);
            _diplomacy_scan_input_flag = -1;
        }

        fonts::Print_(0x12, 0x8D, _system_cancel_buffer);

        if (_diplomacy_scan_input_flag != -1) {
            Draw_Diplomacy_System_Text_(_diplomacy_scan_input_flag - 1, _diplomacy_system_homeworld_flag);
        }
    }

    void __cdecl Load_Diplomacy_Text_Messages_() {
        RUSS::Mox_Update_();
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x1E, _main_diplomacy_title, 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x1F, _main_diplomacy_list[0], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x20, _main_diplomacy_list[1], 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x21, _main_diplomacy_list[2], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x22, _main_diplomacy_list[3], 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x23, _main_diplomacy_list[4], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x24, _main_diplomacy_list[5], 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x25, _main_diplomacy_list[6], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x26, _main_diplomacy_list[7], 0x1E);

        *_main_diplomacy_list[8] = *s__00551100;
        RUSS::Mox_Update_();

        strcpy(_main_net_diplomacy_list[0], _main_diplomacy_list[0]);
        strcpy(_main_net_diplomacy_list[1], _main_diplomacy_list[1]);
        strcpy(_main_net_diplomacy_list[2], _main_diplomacy_list[2]);
        strcpy(_main_net_diplomacy_list[3], _main_diplomacy_list[3]);
        strcpy(_main_net_diplomacy_list[4], _main_diplomacy_list[4]);
        strcpy(_main_net_diplomacy_list[5], _main_diplomacy_list[5]);
        strcpy(_main_net_diplomacy_list[6], _main_diplomacy_list[6]);

        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x41, _main_net_diplomacy_list[7], 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x27, _main_net_diplomacy_list[8], 0x28);

        strcpy(_main_net_diplomacy_list[9], _main_diplomacy_list[7]);
        *_main_net_diplomacy_list[10] = *s__00551100;

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2B, _main_repulsive_diplomacy_list[0], 0x1E);

        strcpy(_main_repulsive_diplomacy_list[1], _main_diplomacy_list[5]);
        strcpy(_main_repulsive_diplomacy_list[2], _main_diplomacy_list[6]);
        strcpy(_main_repulsive_diplomacy_list[3], _main_diplomacy_list[7]);

        *_main_repulsive_diplomacy_list[4] = *s__00551100;
        RUSS::Mox_Update_();

        strcpy(_main_net_repulsive_diplomacy_list[0], _main_repulsive_diplomacy_list[0]);
        strcpy(_main_net_repulsive_diplomacy_list[1], _main_diplomacy_list[5]);
        strcpy(_main_net_repulsive_diplomacy_list[2], _main_diplomacy_list[6]);
        strcpy(_main_net_repulsive_diplomacy_list[3], _main_net_diplomacy_list[7]);
        strcpy(_main_net_repulsive_diplomacy_list[4], _main_diplomacy_list[7]);

        *_main_net_repulsive_diplomacy_list[5] = *s__00551100;
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x28, _propose_treaty_title, 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x29, _propose_treaty_diplomacy_list[0], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2A, _propose_treaty_diplomacy_list[1], 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2B, _propose_treaty_diplomacy_list[2], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2C, _propose_treaty_diplomacy_list[3], 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2D, _propose_treaty_diplomacy_list[4], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2E, _propose_treaty_diplomacy_list[5], 0x1E);

        *_propose_treaty_diplomacy_list[6] = *s__00551100;
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2F, _break_treaty_title, 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x30, _break_treaty_diplomacy_list[0], 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x31, _break_treaty_diplomacy_list[1], 0x28);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x32, _break_treaty_diplomacy_list[2], 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x33, _break_treaty_diplomacy_list[3], 0x28);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x34, _break_treaty_diplomacy_list[4], 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2E, _break_treaty_diplomacy_list[5], 0x28);

        *_break_treaty_diplomacy_list[6] = *s__00551100;
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x35, _demand_title, 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x36, _demand_diplomacy_list[0], 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x37, _demand_diplomacy_list[1], 0x28);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x38, _demand_diplomacy_list[2], 0x28);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x39, _demand_diplomacy_list[3], 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x3A, _demand_diplomacy_list[4], 0x28);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x3B, _demand_diplomacy_list[5], 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x3C, _demand_diplomacy_list[6], 0x28);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x3D, _demand_diplomacy_list[7], 0x28);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x3E, _demand_diplomacy_list[8], 0x28);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2E, _demand_diplomacy_list[9], 0x23);
        *_demand_diplomacy_list[10] = *s__00551100;
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x3F, _offer_title, 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x40, _offer_diplomacy_list[0], 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x41, _offer_diplomacy_list[1], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x42, _offer_diplomacy_list[2], 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x43, _offer_diplomacy_list[3], 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x44, _offer_diplomacy_list[4], 0x1E);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2E, _offer_diplomacy_list[5], 0x1E);
        *_offer_diplomacy_list[6] = *s__00551100;

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x45, _agree_diplomacy_list[0], 0x14);
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x46, _agree_diplomacy_list[1], 0x14);
        *_agree_diplomacy_list[2] = *s__00551100;
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x02, _choose_a_system_buffer, 0x28);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2E, _system_cancel_buffer, 0x14);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x38, _system_million_colonists_buffer, 0x1E);
        RUSS::Mox_Update_();

        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x39, _system_homeworld_buffer, 0x1E);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x3A, _system_x_star_system_buffer, 0x23);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x43, _player_number_name_buffer, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x44, _player_name_ambassador_buffer, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x45, _you_were_framed_buffer, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x46, _system_unsurrenderable_buffer, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x47, _enter_your_message_buffer, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x48, _percent_tribute_treaty_buffer, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x49, _statement_trade_treaty_buffer, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x4A, _statement_research_treaty_buffer, 0x28);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x0A, _net_listen_mode_option_buffer, 0x14);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x0B, _net_listen_mode_good_bye_buffer, 0x14);
        RUSS::Mox_Update_();
    }

    void __cdecl Diplomacy_Exchange_Technology_(int16_t player_1, int16_t player_2) {
        char diplomacy_list[3][40];

        for (int i = 0; i < 3; i++) {
            strcpy(diplomacy_list[i], _agree_diplomacy_list[i]);
        }

        uint16_t accepted = 0;

        if (_exchange_tech_count > 0) {
            char text_msg[40];
            JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 3, text_msg, 40);

            int16_t selected_tech_idx = Diplomacy_Generic_Exchange_Tech_List_(
                player_1, player_2, text_msg, _target_tech_list, _exchange_tech_count, 0
            );

            if (selected_tech_idx != -1) {
                uint16_t tech_1 = _target_tech_list[selected_tech_idx];
                uint16_t tech_2 = 0;

                if (MOX::_player[player_2].objectives == 100 /* 'd' */) {
                    if (MOX::_game_type == 1) {
                        Diplomacy_Load_New_Ambassador_(player_1);
                        Get_Human_Tech_Exchange_First_Message_(player_1, player_2, tech_1);
                        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 4, text_msg, 40);

                        int32_t tech_categories[212];
                        memset(tech_categories, 0, sizeof(tech_categories));
                        memset(tech_categories, 1, 15);

                        npcdiplo::Get_Differential_Tech_List_(
                            player_2, player_1, 0, 0xC343CB00,
                            _target_tech_list, tech_categories,
                            &_exchange_tech_count, 1
                        );

                        int16_t ex_selected_tech_idx = Diplomacy_Generic_Exchange_Tech_List_(
                            player_2, player_1, text_msg, _target_tech_list, _exchange_tech_count, 0
                        );

                        Diplomacy_Load_New_Ambassador_(player_2);

                        if (ex_selected_tech_idx != -1) {
                            tech_2 = _target_tech_list[ex_selected_tech_idx];
                            Get_Human_Tech_Exchange_Message_(player_2, player_1, tech_1, tech_2);
                            JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 5, text_msg, 40);

                            int16_t action = Diplomacy_Generic_List_(2, (char*)diplomacy_list, 0, text_msg);
                            if (action == -1) {
                                Get_Human_Tech_Exchange_Decline_Message_();
                            } else {
                                accepted = 1;
                                Get_Human_Tech_Exchange_Accept_Message_();
                            }
                        }
                    } else {
                        _net_diplomacy_message.arg2 = 0;
                        _net_diplomacy_message.sender_player_id = (uint8_t)player_1;
                        _net_diplomacy_message.msg_type = 0x3F1;
                        _net_diplomacy_message.target_player_id = (uint8_t)player_2;
                        _net_diplomacy_message.arg1 = tech_1;

                        NETMOX::Mox_Send_Message_(MOX::_player[player_2].network_player_id, 2000, &_net_diplomacy_message, 8);
                        RUSS::Mox_Update_();

                        Get_Listen_Mode_Message_(player_2);
                        _net_diplomacy_message.arg2 = -2;

                        do {
                            Diplomacy_Listen_Mode_(player_2, (uint8_t)player_1);
                            if (_net_diplomacy_message.arg2 != -2) {
                                break;
                            }
                        } while (_net_diplomacy_esc_pressed == 0);

                        if (_net_diplomacy_esc_pressed == 1) {
                            _net_diplomacy_message.arg2 = -1;
                        }

                        int16_t ex_selected_tech_idx = _net_diplomacy_message.arg2;
                        if (ex_selected_tech_idx != -1) {
                            tech_2 = (uint16_t)ex_selected_tech_idx;
                            Get_Human_Tech_Exchange_Message_(player_2, player_1, tech_1, tech_2);
                            JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 5, text_msg, 40);

                            int16_t action = Diplomacy_Generic_List_(2, (char*)diplomacy_list, 0, text_msg);

                            _net_diplomacy_message.arg1 = action;
                            _net_diplomacy_message.sender_player_id = (uint8_t)player_1;
                            _net_diplomacy_message.msg_type = 0x3F2;
                            _net_diplomacy_message.target_player_id = (uint8_t)player_2;
                            _net_diplomacy_message.arg2 = tech_2;

                            NETMOX::Mox_Send_Message_(MOX::_player[player_2].network_player_id, 2000, &_net_diplomacy_message, 8);
                            RUSS::Mox_Update_();

                            if (action == -1) {
                                Get_Human_Tech_Exchange_Decline_Message_();
                            } else {
                                accepted = 1;
                                Get_Human_Tech_Exchange_Accept_Message_();
                            }
                        }
                    }
                } else if (_exchange_tech_list[selected_tech_idx] == 0) {
                    _old_response_message = -1;
                    _response_message = 0xAE;
                    npcdiplo::NPC_Message_Rejection_Check_(player_2, player_1, &_response_message);
                } else {
                    tech_2 = _exchange_tech_list[selected_tech_idx];
                    Get_Human_Tech_Exchange_Message_(player_2, player_1, tech_1, tech_2);
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 5, text_msg, 40);

                    int16_t action = Diplomacy_Generic_List_(2, (char*)diplomacy_list, 0, text_msg);
                    if (action == -1) {
                        *_diplomacy_message = MOX::s__00551100[0];
                    } else {
                        accepted = 1;
                        Get_Human_Tech_Exchange_Accept_Message_();
                    }
                }

                if (accepted == 1) {
                    diplomac::Change_Relations_(12, player_1, player_2, 0, 0, 0);
                    COLCALC::Player_Gets_Tech_App_(&MOX::_player[player_1], tech_1);
                    COLCALC::Player_Gets_Tech_App_(&MOX::_player[player_2], tech_2);

                    if (MOX::_player[player_2].objectives != 100 /* 'd' */) {
                        npcdiplo::Get_Exchange_Tech_List_(player_1, player_2, &_exchange_tech_count, _target_tech_list, _exchange_tech_list);
                        diplomac::Adjust_Diplomat_Modifiers_(player_1, player_2);
                        MOX::_player[player_1].tech_exchange_modifier[player_2] -= 20;
                    }
                }
            }
        }
    }

    int16_t __cdecl Diplomacy_Human_Need_Better_Offer_(int16_t player_idx, int16_t target_player_idx) {
        char list_items[10][40];
        char list_title[40];
        uint8_t tech_list[12];
        int16_t result;
        int16_t tech_list_count;
        int16_t list_count;
        int16_t offer_system_id;
        int16_t offer_money_id;
        int16_t offer_tech_id;
        int16_t selection;
        int16_t system_sel;
        int16_t confirm;
        int16_t money_amount;
        int16_t tech_sel;
        int16_t tech_app_id;

        _chat_mode_flag = 0;
        result = 0;
        offer_tech_id = -10;
        offer_system_id = -10;
        offer_money_id = -10;

        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 51, _diplomacy_message, 250);
        JIM::Decode_Text_Race_(_diplomacy_message, target_player_idx, 250);

        list_count = 0;

        if (Check_Surrender_System_(player_idx, target_player_idx)) {
            JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 52, list_items[list_count], 40);
            offer_system_id = list_count;
            list_count++;
        }

        Get_Diplomacy_Tech_List_(player_idx, target_player_idx, &tech_list_count, tech_list);
        if (tech_list_count > 0) {
            JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 53, list_items[list_count], 40);
            offer_tech_id = list_count;
            list_count++;
        }

        if (MOX::_player[player_idx].bc > 100) {
            JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 64, list_items[list_count], 40);
            offer_money_id = list_count;
            list_count++;
        }

        list_items[list_count][0] = s__00551100[0];

        if (list_count > 0) {
            result = 0;
            fields::Deactivate_Help_List_();
            JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 54, list_title, 40);
            selection = Diplomacy_Generic_List_(list_count, (char*)list_items, 1, list_title);

            if (selection == offer_system_id) {
                system_sel = Diplomacy_System_Display_(player_idx, _current_ambassador, 0);
                if (system_sel != -1) {
                    sprintf(list_items[0], s_fmt_s_0055236c, MOX::_star[system_sel].name);
                    if (MOX::_game_type == 1) {
                        Diplomacy_Load_New_Ambassador_(target_player_idx);
                        confirm = Diplomacy_Generic_System_List_(system_sel, player_idx, target_player_idx, 0);
                        if (confirm != -1) {
                            if (COLCALC::May_Surrender_Star_(system_sel, player_idx, target_player_idx)) {
                                COLCALC::Surrender_Star_(system_sel, player_idx, target_player_idx);
                            }
                            result = 1;
                        }
                    } else {
                        _net_diplomacy_message.sender_player_id = (uint8_t)player_idx;
                        _net_diplomacy_message.target_player_id = (uint8_t)target_player_idx;
                        _net_diplomacy_message.arg1 = system_sel;
                        _net_diplomacy_message.msg_type = 1007; // 0x3ef
                        _net_diplomacy_message.arg2 = _response_message;

                        NETMOX::Mox_Send_Message_(MOX::_player[target_player_idx].network_player_id, 2000, &_net_diplomacy_message, 8);
                        RUSS::Mox_Update_();
                        Get_Listen_Mode_Message_(target_player_idx);
                        _net_diplomacy_message.arg2 = -1;
                        do {
                            Diplomacy_Listen_Mode_(target_player_idx, (uint8_t)player_idx);
                            if (_net_diplomacy_message.arg2 != -1) break;
                        } while (_net_diplomacy_esc_pressed == 0);

                        if (_net_diplomacy_esc_pressed == 1) {
                            _net_diplomacy_message.arg2 = 0;
                        }

                        result = _net_diplomacy_message.arg2;
                        if (result == 1) {
                            if (COLCALC::May_Surrender_Star_(system_sel, player_idx, target_player_idx)) {
                                COLCALC::Surrender_Star_(system_sel, player_idx, target_player_idx);
                            }
                        }
                    }
                }
            }
            else if (selection == offer_tech_id) {
                if (tech_list_count > 8) {
                    tech_list_count = 8;
                }
                if (tech_list_count > 0) {
                    for (int i = 0; i < tech_list_count; i++) {
                        char* tech_name = TECH::Technology_Applications_Name_(tech_list[i], MOX::_PLAYER_NUM);
                        sprintf(list_items[i], s_fmt_s_0055236c, tech_name);
                    }
                }
                tech_sel = Diplomacy_Generic_List_(tech_list_count, (char*)list_items, 1, _offer_title);
                if (tech_sel != -1) {
                    tech_app_id = tech_list[tech_sel];
                    char* tech_name = TECH::Technology_Applications_Name_(tech_app_id, player_idx);
                    sprintf(list_items[0], s_fmt_s_0055236c, tech_name);

                    if (MOX::_game_type == 1) {
                        Diplomacy_Load_New_Ambassador_(player_idx);
                        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 9, list_title, 40);
                        confirm = Diplomacy_Generic_List_(1, (char*)list_items, 1, list_title);
                        if (confirm != -1) {
                            COLCALC::Player_Gets_Tech_App_(&MOX::_player[target_player_idx], tech_app_id);
                            result = 1;
                        }
                        Diplomacy_Load_New_Ambassador_(target_player_idx);
                    } else {
                        _net_diplomacy_message.sender_player_id = (uint8_t)player_idx;
                        _net_diplomacy_message.target_player_id = (uint8_t)target_player_idx;
                        _net_diplomacy_message.arg1 = tech_app_id;
                        _net_diplomacy_message.msg_type = 1008; // 0x3f0
                        _net_diplomacy_message.arg2 = _response_message;

                        NETMOX::Mox_Send_Message_(MOX::_player[target_player_idx].network_player_id, 2000, &_net_diplomacy_message, 8);
                        RUSS::Mox_Update_();
                        _net_diplomacy_message.arg2 = -1;
                        Get_Listen_Mode_Message_(target_player_idx);
                        do {
                            Diplomacy_Listen_Mode_(target_player_idx, (uint8_t)player_idx);
                            if (_net_diplomacy_message.arg2 != -1) break;
                        } while (_net_diplomacy_esc_pressed == 0);

                        if (_net_diplomacy_esc_pressed == 1) {
                            _net_diplomacy_message.arg2 = 0;
                        }

                        result = _net_diplomacy_message.arg2;
                        if (result == 1) {
                            COLCALC::Player_Gets_Tech_App_(&MOX::_player[target_player_idx], tech_app_id);
                        }
                    }
                }
            }
            else if (selection == offer_money_id) {
                money_amount = Diplomacy_Offer_Money_(player_idx, target_player_idx, 0);
                if (money_amount > 0) {
                    sprintf(list_items[0], s_fmt_d_bc_00552384, money_amount);
                    if (MOX::_game_type == 1) {
                        Diplomacy_Load_New_Ambassador_(player_idx);
                        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 9, list_title, 40);
                        confirm = Diplomacy_Generic_List_(1, (char*)list_items, 1, list_title);
                        if (confirm != -1) {
                            MOX::_player[player_idx].bc -= money_amount;
                            MOX::_player[target_player_idx].bc += money_amount;
                            result = 1;
                        } else {
                            result = 0;
                        }
                        Diplomacy_Load_New_Ambassador_(target_player_idx);
                    } else {
                        _net_diplomacy_message.sender_player_id = (uint8_t)player_idx;
                        _net_diplomacy_message.target_player_id = (uint8_t)target_player_idx;
                        _net_diplomacy_message.arg1 = money_amount;
                        _net_diplomacy_message.msg_type = 1013; // 0x3f5
                        _net_diplomacy_message.arg2 = _response_message;

                        NETMOX::Mox_Send_Message_(MOX::_player[target_player_idx].network_player_id, 2000, &_net_diplomacy_message, 8);
                        RUSS::Mox_Update_();
                        _net_diplomacy_message.arg2 = -1;
                        Get_Listen_Mode_Message_(target_player_idx);
                        do {
                            Diplomacy_Listen_Mode_(target_player_idx, (uint8_t)player_idx);
                            if (_net_diplomacy_message.arg2 != -1) break;
                        } while (_net_diplomacy_esc_pressed == 0);

                        if (_net_diplomacy_esc_pressed == 1) {
                            _net_diplomacy_message.arg2 = 0;
                        }

                        result = _net_diplomacy_message.arg2;
                        if (result == 1) {
                            MOX::_player[player_idx].bc -= money_amount;
                            MOX::_player[target_player_idx].bc += money_amount;
                        }
                    }
                }
            }
        }

        return result;
    }

    void __cdecl Diplomacy_Demand_(int16_t player_num, int16_t ambassador_idx) {
        uint8_t demand_list_flags[17];
        for (int i = 0; i < 17; ++i) {
            demand_list_flags[i] = 0;
        }

        int16_t treaties_1_count = 0;
        uint8_t treaties_1[12];
        Get_Diplomacy_Demand_Treaty_List_(player_num, ambassador_idx, 5, &treaties_1_count, treaties_1);
        if (treaties_1_count > 0) {
            demand_list_flags[0] = 1;
        }

        int16_t treaties_2_count = 0;
        uint8_t treaties_2[12];
        if (MOX::_game_type == 0) {
            Get_Diplomacy_Demand_Treaty_List_(player_num, ambassador_idx, 3, &treaties_2_count, treaties_2);
            if (treaties_2_count > 0) {
                demand_list_flags[1] = 1;
            }
        }

        int16_t treaties_3_count = 0;
        uint8_t treaties_3[12];
        Get_Diplomacy_Demand_Treaty_List_(player_num, ambassador_idx, 2, &treaties_3_count, treaties_3);
        if (treaties_3_count > 0) {
            demand_list_flags[2] = 1;
        }

        demand_list_flags[3] = 1;
        demand_list_flags[4] = 1;

        int p2_tribute_to_p1 = MOX::_player[ambassador_idx].tribute_treaty[player_num];
        if (p2_tribute_to_p1 > 0) {
            demand_list_flags[3] = 0;
            demand_list_flags[4] = 0;
        } else if (MOX::_player[player_num].tribute_treaty[ambassador_idx] > 0) {
            demand_list_flags[3] = 0;
            demand_list_flags[4] = 0;
        } else if (p2_tribute_to_p1 == 1) {
            demand_list_flags[3] = 0;
        } else if (p2_tribute_to_p1 == 2) {
            demand_list_flags[3] = 0;
            demand_list_flags[4] = 0;
        }

        demand_list_flags[5] = 1;

        int16_t local_624 = 0;
        uint8_t local_5b4[212];
        int32_t local_350[212];
        npcdiplo::Get_Differential_Tech_List_(player_num, ambassador_idx, 0, 0xc343cb00, local_5b4, local_350, &local_624, 1);
        if (local_624 > 0) {
            demand_list_flags[6] = 1;
        }

        if (Check_Surrender_System_(ambassador_idx, player_num) == 1) {
            demand_list_flags[7] = 1;
        }

        if (SHIPMOVE::Player_Has_Ships_Pestering_Other_Player_(ambassador_idx, (uint8_t)player_num) == 1) {
            demand_list_flags[8] = 1;
        }

        demand_list_flags[9] = 1;

        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Set_Input_Delay_(1);

        for (int i = 0; i < 11; ++i) {
            strcpy(_diplomacy_list_choice[i], _demand_diplomacy_list[i]);
        }

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);
        Setup_Diplomacy_Help_(11, _demand_help_entry);

        int8_t list_index = 0;
        int16_t selection = fields::Get_List_Field_(
            10, 0x76, 0xF5, _demand_title, (char*)_diplomacy_list_choice,
            0x32, &list_index, demand_list_flags, 17,
            nullptr, 0, nullptr, 0, 0
        );

        switch (selection) {
            case 0: {
                char treaties_str_list[15][40];
                for (int i = 0; i < treaties_1_count; ++i) {
                    sprintf(treaties_str_list[i], s__s_0055236c, MOX::_player[treaties_1[i]].name);
                }
                char title[40];
                JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x47, title, 40);
                int16_t list_sel = Diplomacy_Generic_List_(treaties_1_count, (char*)treaties_str_list, 1, title);
                if (list_sel == -1) break;

                _old_response_message = -1;
                uint8_t treaty_val = treaties_1[list_sel];
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 0, treaty_val, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 0, treaty_val, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                if (response == 1) {
                    Diplomacy_Give_In_To_Demand_(player_num, ambassador_idx, 0, treaty_val);
                    return;
                }
                break;
            }
            case 1: {
                char treaties_str_list[15][40];
                for (int i = 0; i < treaties_2_count; ++i) {
                    sprintf(treaties_str_list[i], s__s_0055236c, MOX::_player[treaties_2[i]].name);
                }
                char title[40];
                JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x48, title, 40);
                int16_t list_sel = Diplomacy_Generic_List_(treaties_2_count, (char*)treaties_str_list, 1, title);
                if (list_sel == -1) break;

                _old_response_message = -1;
                uint8_t treaty_val = treaties_2[list_sel];
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 1, treaty_val, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 1, treaty_val, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                if (response == 1) {
                    Diplomacy_Give_In_To_Demand_(player_num, ambassador_idx, 1, treaty_val);
                    return;
                }
                break;
            }
            case 2: {
                char treaties_str_list[15][40];
                for (int i = 0; i < treaties_3_count; ++i) {
                    sprintf(treaties_str_list[i], s__s_0055236c, MOX::_player[treaties_3[i]].name);
                }
                char title[40];
                JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x49, title, 40);
                int16_t list_sel = Diplomacy_Generic_List_(treaties_3_count, (char*)treaties_str_list, 1, title);
                if (list_sel == -1) break;

                _old_response_message = -1;
                uint8_t treaty_val = treaties_3[list_sel];
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 2, treaty_val, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 2, treaty_val, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                if (response == 1) {
                    Diplomacy_Give_In_To_Demand_(player_num, ambassador_idx, 2, treaty_val);
                    return;
                }
                break;
            }
            case 3: {
                _old_response_message = -1;
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 3, 0, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 3, 0, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                if (response == 1) {
                    Diplomacy_Give_In_To_Demand_(player_num, ambassador_idx, 3, 0);
                    return;
                }
                break;
            }
            case 4: {
                _old_response_message = -1;
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 4, 0, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 4, 0, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                if (response == 1) {
                    Diplomacy_Give_In_To_Demand_(player_num, ambassador_idx, 4, 0);
                    return;
                }
                break;
            }
            case 5: {
                _old_response_message = -1;
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 5, 0, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 5, 0, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                return;
            }
            case 6: {
                int16_t selection_tech = Diplomacy_Generic_Exchange_Tech_List_(player_num, ambassador_idx, _demand_title, local_5b4, local_624, 0);
                if (selection_tech == -1) break;

                _old_response_message = -1;
                uint8_t tech_val = local_5b4[selection_tech];
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 6, tech_val, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 6, tech_val, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                if (response == 1) {
                    Diplomacy_Give_In_To_Demand_(player_num, ambassador_idx, 6, tech_val);
                    return;
                }
                break;
            }
            case 7: {
                int16_t selection_sys = Diplomacy_System_Display_(ambassador_idx, player_num, 1);
                if (selection_sys == -1) break;

                _old_response_message = -1;
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 7, selection_sys, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 7, selection_sys, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                if (response == 1) {
                    Diplomacy_Give_In_To_Demand_(player_num, ambassador_idx, 7, selection_sys);
                    return;
                }
                break;
            }
            case 8: {
                _old_response_message = -1;
                int16_t response;
                if (MOX::_player[ambassador_idx].objectives == 100) {
                    Get_Human_Demand_Response_(player_num, ambassador_idx, 8, 0, &response);
                } else {
                    diplomac::Get_Demand_Response_(player_num, ambassador_idx, 8, 0, &response, &_response_message);
                }
                diplomac::Adjust_Diplomat_Modifiers_(player_num, ambassador_idx);
                if (response == 1) {
                    Diplomacy_Give_In_To_Demand_(player_num, ambassador_idx, 8, 0);
                }
                return;
            }
            default:
                break;
        }
    }

    void __cdecl Start_Net_Treaty_() {
        switch (_net_diplomacy_treaty_type) {
        case 1:
            diplomac::Start_Treaty_(
                static_cast<int16_t>(_talker_ambassador),
                static_cast<int16_t>(_listen_ambassador),
                1
            );
            break;
        case 2:
            diplomac::Start_Treaty_(
                static_cast<int16_t>(_talker_ambassador),
                static_cast<int16_t>(_listen_ambassador),
                2
            );
            break;
        case 3:
            diplomac::Declare_Peace_(
                static_cast<int16_t>(_talker_ambassador),
                static_cast<int16_t>(_listen_ambassador)
            );
            break;
        case 7:
            diplomac::Start_Trade_Treaty_(
                static_cast<int16_t>(_talker_ambassador),
                static_cast<int16_t>(_listen_ambassador)
            );
            break;
        case 8:
            diplomac::Start_Research_Treaty_(
                static_cast<int16_t>(_talker_ambassador),
                static_cast<int16_t>(_listen_ambassador)
            );
            break;
        default:
            break;
        }

        Get_Human_Form_Treaty_Message_(
            static_cast<int16_t>(_talker_ambassador),
            static_cast<int16_t>(_listen_ambassador),
            _net_diplomacy_treaty_type,
            3
        );
    }

    void __cdecl Get_Net_Diplomacy_Choices_() {
        int8_t selected;
        int16_t tech_count;
        int16_t done_flag;
        uint8_t options[11];
        char title_buf[40];
        uint8_t tech_list[212];
        char msg_buf[400];
        int32_t costs[212];

        _chat_message[0] = s__00551100[0];
        _chat_base_x = 10;
        _chat_base_y = 0x8D;
        _chat_width = 300;
        _old_response_message = 0;
        _chat_mode_flag = 0;
        _response_message = 0;
        _diplomacy_message[0] = s__00551100[0];
        _chat_height = 0x32;

        if (MOX::_PLAYER_NUM != _talker_ambassador) {
            Get_Listen_Mode_Message_(_talker_ambassador);
            Send_Diplomacy_Handshake_(_talker_ambassador, _listen_ambassador);
            do {
                Diplomacy_Synch_Mode_(_listen_ambassador, _talker_ambassador);
            } while (_synch_up_established_flag == 0);
        }
        else {
            Get_Listen_Mode_Message_(_listen_ambassador);
            Send_Diplomacy_Handshake_(_listen_ambassador, _talker_ambassador);
            do {
                Diplomacy_Synch_Mode_(_talker_ambassador, _listen_ambassador);
            } while (_synch_up_established_flag == 0);
        }

        done_flag = 0;
        _old_response_message = 0;
        _response_message = 0;
        _diplomacy_message[0] = s__00551100[0];

        if (MOX::_PLAYER_NUM != _talker_ambassador) {
            Get_Listen_Mode_Message_(_talker_ambassador);
        }

        fields::Set_Input_Delay_(1);

        while (true) {
            if (_net_diplomacy_esc_pressed != 0) {
                return;
            }

            fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);

            if (MOX::_PLAYER_NUM != _talker_ambassador) {
                if (_chat_mode_flag == 0) {
                    fields::Clear_Fields_();
                }

                if (_net_diplomacy_esc_pressed == 1) {
                    done_flag = 1;
                }

                if (_old_response_message == _response_message || _response_message == 0x80) {
                    Diplomacy_Listen_Mode_(_talker_ambassador, _listen_ambassador);
                }
                else {
                    int32_t resp = _response_message;
                    _chat_mode_flag = 0;

                    if (resp <= 1000) {
                        if (resp == 1000) {
                            _response_message = _net_diplomacy_message.arg2;
                            _net_diplomacy_treaty_type = _net_diplomacy_message.arg1;
                            int16_t choice = Get_Human_Treaty_Choice_(_talker_ambassador, _listen_ambassador, _net_diplomacy_message.arg1);
                            _old_response_message = _response_message;
                            _net_diplomacy_message.msg_type = _response_message;
                            _net_diplomacy_message.arg2 = choice;
                            _net_diplomacy_message.sender_player_id = static_cast<uint8_t>(_talker_ambassador);
                            _net_diplomacy_message.target_player_id = static_cast<uint8_t>(_listen_ambassador);
                            NETMOX::Mox_Send_Message_(MOX::_player[_talker_ambassador].network_player_id, 2000, reinterpret_cast<uint8_t*>(&_net_diplomacy_message), 8);
                            RUSS::Mox_Update_();
                            _response_message = 0;
                            _old_response_message = 0;

                            if (choice == 0 || choice == 1) {
                                Get_Listen_Mode_Message_(_talker_ambassador);
                            }
                            else if (choice == 3) {
                                Start_Net_Treaty_();
                            }
                        }
                        else if (resp == 0x82) {
                            if (_net_diplomacy_esc_pressed == 0) {
                                Diplomacy_Display_Response_();
                            }
                            done_flag = 1;
                        }
                    }
                    else if (resp <= 1013) {
                        switch (resp) {
                        case 1001: {
                            _response_message = _net_diplomacy_message.arg2;
                            Get_Human_Break_Treaty_Message_(_talker_ambassador, _net_diplomacy_message.arg1);
                            switch (_net_diplomacy_message.arg1) {
                            case 1:
                            case 2:
                                diplomac::Break_Treaties_(_talker_ambassador, _listen_ambassador);
                                break;
                            case 7:
                                diplomac::Break_Trade_(_talker_ambassador, _listen_ambassador);
                                break;
                            case 8:
                                diplomac::Break_Research_(_talker_ambassador, _listen_ambassador);
                                break;
                            case 9:
                            case 10:
                                diplomac::Break_Tribute_(_talker_ambassador, _listen_ambassador);
                                break;
                            }
                            _response_message = 0;
                            _old_response_message = 0;
                            break;
                        }
                        case 1002: {
                            Get_Human_Declare_War_Message_(_talker_ambassador);
                            _response_message = 0;
                            _old_response_message = 0;
                            diplomac::Declare_War_(_talker_ambassador, _listen_ambassador);
                            break;
                        }
                        case 1003: {
                            _response_message = 0;
                            _old_response_message = 0;
                            Get_Human_Surrender_Message_(_talker_ambassador);
                            Diplomacy_Display_Response_();
                            done_flag = 1;
                            break;
                        }
                        case 1004: {
                            _response_message = 0;
                            _old_response_message = 0;
                            Get_Human_Offer_Gift_Message_(_talker_ambassador, _net_diplomacy_message.arg1, _net_diplomacy_message.arg2);
                            _response_message = 0;
                            _old_response_message = 0;
                            switch (_net_diplomacy_message.arg1) {
                            case 1:
                                MOX::_player[_talker_ambassador].bc -= _net_diplomacy_message.arg2;
                                MOX::_player[_listen_ambassador].bc += _net_diplomacy_message.arg2;
                                break;
                            case 2:
                                diplomac::Start_Tribute_Treaty_(_talker_ambassador, _listen_ambassador, _net_diplomacy_message.arg2);
                                break;
                            case 3:
                                COLCALC::Player_Gets_Tech_App_(&MOX::_player[_listen_ambassador], _net_diplomacy_message.arg2);
                                break;
                            case 4:
                                if (COLCALC::May_Surrender_Star_(_net_diplomacy_message.arg2, _talker_ambassador, _listen_ambassador) == 1) {
                                    COLCALC::Surrender_Star_(_net_diplomacy_message.arg2, _talker_ambassador, _listen_ambassador);
                                }
                                break;
                            }
                            break;
                        }
                        case 1005: {
                            Get_Human_Demand_Message_(_talker_ambassador, _listen_ambassador, _net_diplomacy_message.arg1, _net_diplomacy_message.arg2);
                            int16_t demand_choice = Get_Human_Demand_Choice_(_listen_ambassador, _talker_ambassador, _net_diplomacy_message.arg1, _net_diplomacy_message.arg2);
                            _response_message = 0xA8;
                            if (demand_choice == 1) {
                                Diplomacy_Give_In_To_Demand_(_talker_ambassador, _listen_ambassador, _net_diplomacy_message.arg1, _net_diplomacy_message.arg2);
                                _response_message = 0xA9;
                            }
                            _old_response_message = _response_message;
                            _net_diplomacy_message.msg_type = _response_message;
                            _net_diplomacy_message.arg2 = demand_choice;
                            _net_diplomacy_message.sender_player_id = static_cast<uint8_t>(_talker_ambassador);
                            _net_diplomacy_message.target_player_id = static_cast<uint8_t>(_listen_ambassador);
                            NETMOX::Mox_Send_Message_(MOX::_player[_talker_ambassador].network_player_id, 2000, reinterpret_cast<uint8_t*>(&_net_diplomacy_message), 8);
                            RUSS::Mox_Update_();
                            Get_Listen_Mode_Message_(_talker_ambassador);
                            break;
                        }
                        case 1007: {
                            _response_message = _old_response_message;
                            int16_t list_choice = Diplomacy_Generic_System_List_(_net_diplomacy_message.arg1, _talker_ambassador, _listen_ambassador, 0);
                            if (list_choice == -1) {
                                Get_Listen_Mode_Message_(_talker_ambassador);
                                list_choice = 0;
                            }
                            else {
                                if (COLCALC::May_Surrender_Star_(_net_diplomacy_message.arg1, _talker_ambassador, _listen_ambassador) == 1) {
                                    COLCALC::Surrender_Star_(_net_diplomacy_message.arg1, _talker_ambassador, _listen_ambassador);
                                }
                                Start_Net_Treaty_();
                                list_choice = 1;
                            }
                            _net_diplomacy_message.msg_type = _response_message;
                            _net_diplomacy_message.arg2 = list_choice;
                            _net_diplomacy_message.sender_player_id = static_cast<uint8_t>(_talker_ambassador);
                            _net_diplomacy_message.target_player_id = static_cast<uint8_t>(_listen_ambassador);
                            NETMOX::Mox_Send_Message_(MOX::_player[_talker_ambassador].network_player_id, 2000, reinterpret_cast<uint8_t*>(&_net_diplomacy_message), 8);
                            RUSS::Mox_Update_();
                            _response_message = 0;
                            _old_response_message = 0;
                            break;
                        }
                        case 1008: {
                            _response_message = _old_response_message;
                            char* tech_name = TECH::Technology_Applications_Name_(_net_diplomacy_message.arg1, _talker_ambassador);
                            sprintf(msg_buf, GLOBALS::s_spaces_percent_s_0055236c, tech_name);
                            JIM::Get_Text_Message_(s_jimtext2_lbx, 9, title_buf, 40);
                            int16_t list_choice = Diplomacy_Generic_List_(1, msg_buf, 1, title_buf);
                            if (list_choice == -1) {
                                Get_Listen_Mode_Message_(_talker_ambassador);
                                list_choice = 0;
                            }
                            else {
                                COLCALC::Player_Gets_Tech_App_(&MOX::_player[_listen_ambassador], _net_diplomacy_message.arg1);
                                Start_Net_Treaty_();
                                list_choice = 1;
                            }
                            _net_diplomacy_message.msg_type = _response_message;
                            _net_diplomacy_message.arg2 = list_choice;
                            _net_diplomacy_message.sender_player_id = static_cast<uint8_t>(_talker_ambassador);
                            _net_diplomacy_message.target_player_id = static_cast<uint8_t>(_listen_ambassador);
                            NETMOX::Mox_Send_Message_(MOX::_player[_talker_ambassador].network_player_id, 2000, reinterpret_cast<uint8_t*>(&_net_diplomacy_message), 8);
                            RUSS::Mox_Update_();
                            _response_message = 0;
                            _old_response_message = 0;
                            break;
                        }
                        case 1009: {
                            _response_message = 0;
                            _old_response_message = 0;
                            Get_Human_Tech_Exchange_First_Message_(_talker_ambassador, _listen_ambassador, _net_diplomacy_message.arg1);
                            _net_diplomacy_tech_offered = _net_diplomacy_message.arg1;
                            JIM::Get_Text_Message_(s_jimtext2_lbx, 4, title_buf, 40);
                            npcdiplo::Get_Differential_Tech_List_(_listen_ambassador, _talker_ambassador, 0, 0xC343CB00, (uint8_t*)tech_list, costs, &tech_count, 1);
                            int16_t exch_choice = Diplomacy_Generic_Exchange_Tech_List_(_listen_ambassador, _talker_ambassador, title_buf, tech_list, tech_count, 0);
                            if (exch_choice >= 0) {
                                exch_choice = tech_list[exch_choice];
                            }
                            _net_diplomacy_message.msg_type = _response_message;
                            _net_diplomacy_message.arg2 = exch_choice;
                            _net_diplomacy_message.sender_player_id = static_cast<uint8_t>(_talker_ambassador);
                            _net_diplomacy_message.target_player_id = static_cast<uint8_t>(_listen_ambassador);
                            NETMOX::Mox_Send_Message_(MOX::_player[_talker_ambassador].network_player_id, 2000, reinterpret_cast<uint8_t*>(&_net_diplomacy_message), 8);
                            RUSS::Mox_Update_();
                            Get_Listen_Mode_Message_(_talker_ambassador);
                            break;
                        }
                        case 1010: {
                            _response_message = 0;
                            _old_response_message = 0;
                            if (_net_diplomacy_message.arg1 == -1) {
                                Get_Human_Tech_Exchange_Decline_Message_();
                            }
                            else {
                                COLCALC::Player_Gets_Tech_App_(&MOX::_player[_talker_ambassador], _net_diplomacy_tech_offered);
                                COLCALC::Player_Gets_Tech_App_(&MOX::_player[_listen_ambassador], _net_diplomacy_message.arg2);
                                Get_Human_Tech_Exchange_Accept_Message_();
                            }
                            _response_message = 0;
                            _old_response_message = 0;
                            break;
                        }
                        case 1011: {
                            _response_message = 0;
                            _old_response_message = 0;
                            sprintf(_diplomacy_message, "%s", s__00551100);
                            break;
                        }
                        case 1013: {
                            _response_message = _old_response_message;
                            sprintf(msg_buf, s__d_BC_00552384, _net_diplomacy_message.arg1);
                            JIM::Get_Text_Message_(s_jimtext2_lbx, 9, title_buf, 40);
                            int16_t list_choice = Diplomacy_Generic_List_(1, msg_buf, 1, title_buf);
                            if (list_choice == -1) {
                                Get_Listen_Mode_Message_(_talker_ambassador);
                                list_choice = 0;
                            }
                            else {
                                MOX::_player[_talker_ambassador].bc -= _net_diplomacy_message.arg1;
                                MOX::_player[_listen_ambassador].bc += _net_diplomacy_message.arg1;
                                Start_Net_Treaty_();
                                list_choice = 1;
                            }
                            _net_diplomacy_message.msg_type = _response_message;
                            _net_diplomacy_message.arg2 = list_choice;
                            _net_diplomacy_message.sender_player_id = static_cast<uint8_t>(_talker_ambassador);
                            _net_diplomacy_message.target_player_id = static_cast<uint8_t>(_listen_ambassador);
                            NETMOX::Mox_Send_Message_(MOX::_player[_talker_ambassador].network_player_id, 2000, reinterpret_cast<uint8_t*>(&_net_diplomacy_message), 8);
                            RUSS::Mox_Update_();
                            _response_message = 0;
                            _old_response_message = 0;
                            break;
                        }
                        }
                    }
                }
            }
            else {
                _chat_mode_flag = 0;

                if (MOX::_player[_talker_ambassador].traits[19] == 1 || MOX::_player[_listen_ambassador].traits[19] == 1) {
                    Get_Main_Repulsive_Diplomacy_Choices_(_talker_ambassador, _listen_ambassador, 1);
                    fields::Clear_Fields_();
                }
                else {
                    for (int i = 0; i < 11; ++i) {
                        options[i] = 1;
                    }
                    Diplomacy_Set_Main_Options_(_talker_ambassador, _listen_ambassador, options);

                    for (int i = 0; i < 11; ++i) {
                        strcpy(_diplomacy_list_choice[i], _main_net_diplomacy_list[i]);
                    }

                    fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
                    fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
                    fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
                    fonts::Set_Font_LF_(4);
                    Setup_Diplomacy_Help_(11, _main_net_diplomacy_help_entry);

                    selected = 0;
                    int16_t main_choice = fields::Get_List_Field_(
                        10, 118, 245, _main_diplomacy_title,
                        reinterpret_cast<char*>(_diplomacy_list_choice), 50, &selected, options, 17,
                        nullptr, 0, nullptr, 0, 0
                    );

                    switch (main_choice) {
                    case 0:
                        _diplomacy_message[0] = s__00551100[0];
                        Diplomacy_Propose_Treaty_(_talker_ambassador, _listen_ambassador);
                        _response_message = 0;
                        _old_response_message = 0;
                        _chat_mode_flag = 0;
                        fields::Clear_Fields_();
                        break;
                    case 1:
                        _diplomacy_message[0] = s__00551100[0];
                        Diplomacy_Break_Treaty_(_talker_ambassador, _listen_ambassador);
                        _old_response_message = 0;
                        _response_message = 0;
                        _diplomacy_message[0] = s__00551100[0];
                        fields::Clear_Fields_();
                        break;
                    case 2:
                        _diplomacy_message[0] = s__00551100[0];
                        Diplomacy_Demand_(_talker_ambassador, _listen_ambassador);
                        fields::Clear_Fields_();
                        break;
                    case 3:
                        _diplomacy_message[0] = s__00551100[0];
                        Diplomacy_Offer_Gift_(_talker_ambassador, _listen_ambassador);
                        fields::Clear_Fields_();
                        break;
                    case 4:
                        _diplomacy_message[0] = s__00551100[0];
                        Diplomacy_Exchange_Technology_(_talker_ambassador, _listen_ambassador);
                        fields::Clear_Fields_();
                        break;
                    case 5:
                        if (Diplomacy_Declare_War_Resolution_(_talker_ambassador, _listen_ambassador) == 1) {
                            Diplomacy_Display_Response_();
                        }
                        fields::Clear_Fields_();
                        break;
                    case 6:
                        if (Diplomacy_Surrender_(_talker_ambassador, _listen_ambassador) == 1) {
                            done_flag = 1;
                        }
                        fields::Clear_Fields_();
                        break;
                    case 7:
                        Diplomacy_Main_Net_Chat_(_listen_ambassador, _talker_ambassador);
                        if (_net_diplomacy_esc_pressed == 1) {
                            done_flag = 1;
                        }
                        fields::Clear_Fields_();
                        break;
                    case 8:
                        Diplomacy_Net_Switcheroo_();
                        Get_Listen_Mode_Message_(_talker_ambassador);
                        fields::Clear_Fields_();
                        break;
                    case -1:
                    case 9:
                        _net_diplomacy_message.sender_player_id = static_cast<uint8_t>(_talker_ambassador);
                        _net_diplomacy_message.target_player_id = static_cast<uint8_t>(_listen_ambassador);
                        _net_diplomacy_message.arg2 = -1;
                        _net_diplomacy_message.msg_type = 0x82;
                        done_flag = 1;
                        NETMOX::Mox_Send_Message_(MOX::_player[_listen_ambassador].network_player_id, 2000, reinterpret_cast<uint8_t*>(&_net_diplomacy_message), 8);
                        RUSS::Mox_Update_();
                        fields::Clear_Fields_();
                        break;
                    default:
                        fields::Clear_Fields_();
                        break;
                    }
                }
            }

            if (done_flag != 0) {
                return;
            }
        }
    }

    void __cdecl Get_Human_Tech_Exchange_Message_(int16_t player_2, int16_t player_1, int16_t tech_1, int16_t tech_2) {
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x2F, _diplomacy_message, 0xFA);
        JIM::Decode_Text_Name_Tech_And_Other_Tech_(_diplomacy_message, player_2, player_1, tech_2, tech_1, 0xFA);
    }

    void __cdecl Get_Human_Tech_Exchange_First_Message_(int16_t player_1, int16_t player_2, int16_t tech_id) {
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x30, _diplomacy_message, 0xFA);
        JIM::Decode_Text_Name_And_Other_Tech_(_diplomacy_message, player_1, player_2, tech_id, 0xFA);
    }

    void __cdecl Get_Human_Tech_Exchange_Accept_Message_() {
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x31, _diplomacy_message, 0xFA);
    }

    void __cdecl Get_Human_Tech_Exchange_Decline_Message_() {
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x32, _diplomacy_message, 0xFA);
    }

    /**
     * Returns the appropriate English indefinite article ("a " or "an ") for the given string.
     * If the current game language is not English, it returns an empty string.
     */
    void __cdecl Get_Human_Demand_Response_(int16_t player_id, int16_t target_player_id, int16_t demand_type, int16_t demand_arg, int16_t* out_choice) {
        int16_t choice_1_response = 0xA9;
        int16_t choice_else_response = 0xA8;

        if (MOX::_game_type == 1 /* SINGLE_PLAYER */) {
            _old_response_message = _response_message;
            Get_Human_Demand_Message_(player_id, target_player_id, demand_type, demand_arg);
            Diplomacy_Load_New_Ambassador_(player_id);

            // The binary keeps the demander's ambassador loaded for both the prompt and
            // follow-up response handling, and uses the same player ordering here.
            int16_t choice = Get_Human_Demand_Choice_(player_id, target_player_id, demand_type, demand_arg);
            *out_choice = choice;
            
            _old_response_message = -1;
            _response_message = choice_1_response;
            
            if (*out_choice != 1) {
                _response_message = choice_else_response;
            }
            
            Diplomacy_Load_New_Ambassador_(player_id);
        } else {
            _net_diplomacy_message.sender_player_id = (uint8_t)player_id;
            _net_diplomacy_message.target_player_id = (uint8_t)target_player_id;
            _net_diplomacy_message.arg1 = demand_type;
            _net_diplomacy_message.msg_type = 1005 /* 0x3ED */;
            _net_diplomacy_message.arg2 = demand_arg;

            NETMOX::Mox_Send_Message_(
                MOX::_player[target_player_id].network_player_id, 
                2000 /* 0x7D0 */, 
                &_net_diplomacy_message, 
                8
            );

            RUSS::Mox_Update_();
            Get_Listen_Mode_Message_(target_player_id);
            _net_diplomacy_message.arg2 = -1;

            do {
                Diplomacy_Listen_Mode_(target_player_id, (uint8_t)player_id);
            } while (_net_diplomacy_message.arg2 == -1 && _net_diplomacy_esc_pressed == 0);

            if (_net_diplomacy_esc_pressed == 1) {
                _net_diplomacy_message.arg2 = 0;
            }

            *out_choice = _net_diplomacy_message.arg2;
        }
    }

    char* __cdecl An_(char* text) {
        if (MOX::_settings.language != 0) {
            return const_cast<char*>(s__00551100);
        }

        char first_char = *text;

        // Check if the first character is a vowel (case-insensitive)
        if (first_char == 'A' || first_char == 'a' ||
            first_char == 'E' || first_char == 'e' ||
            first_char == 'I' || first_char == 'i' ||
            first_char == 'O' || first_char == 'o' ||
            first_char == 'U' || first_char == 'u') {
            return s_an;
        }

        return s_a;
    }

    void __cdecl Diplomacy_Give_In_To_Demand_(int16_t asking_player_idx, int16_t giving_player_idx, int16_t demand_type, int16_t demand_data) {
        switch (demand_type) {
        case 0:
            MOX::_player[giving_player_idx].delayed_diplomacy_orders[demand_data] = 0;
            break;

        case 1:
            MOX::_player[giving_player_idx].delayed_diplomacy_orders[demand_data] = 1;
            break;

        case 2:
            MOX::_player[giving_player_idx].delayed_diplomacy_orders[demand_data] = 2;
            break;

        case 3:
            diplomac::Start_Tribute_Treaty_(giving_player_idx, asking_player_idx, 1);
            break;

        case 4:
            diplomac::Start_Tribute_Treaty_(giving_player_idx, asking_player_idx, 2);
            break;

        case 6:
            COLCALC::Player_Gets_Tech_App_(&MOX::_player[asking_player_idx], demand_data);
            break;

        case 7:
            if (COLCALC::May_Surrender_Star_(demand_data, giving_player_idx, asking_player_idx)) {
                COLCALC::Surrender_Star_(demand_data, giving_player_idx, asking_player_idx);
            }
            break;

        case 8:
            {
                int32_t r1 = random::Random_(50);
                int32_t r2 = random::Random_(50);
                MOX::_player[giving_player_idx].stop_blockading_duration[asking_player_idx] = (int8_t)r1 + (int8_t)r2 + 10;
            }
            break;

        case 5:
        default:
            break;
        }
    }

    void __cdecl Diplomacy_Synch_Mode_(int16_t p1, int16_t p2) {
        _old_response_message = 0;
        _response_message = 0;

        fields::Assign_Auto_Function_(Draw_Diplomacy_Listen_Mode_, 2);
        timer::Mark_Time_();
        fields::Clear_Fields_();

        Draw_Diplomacy_Synch_Mode_();
        RUSS::Mox_Sync_Update_();

        timer::Release_Time_(2);
        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
    }

    void __cdecl Get_Human_Demand_Message_(int16_t player_idx, int16_t unused, int16_t demand_type, int16_t item_idx) {
        switch (demand_type) {
        case 0:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x13, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Name_And_Name_(_diplomacy_message, player_idx, item_idx, 0xfa);
            break;
        case 1:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x14, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Name_And_Name_(_diplomacy_message, player_idx, item_idx, 0xfa);
            break;
        case 2:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x15, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Name_And_Name_(_diplomacy_message, player_idx, item_idx, 0xfa);
            break;
        case 3:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x16, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Name_And_Stat_(_diplomacy_message, player_idx, 5, 0xfa);
            break;
        case 4:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x16, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Name_And_Stat_(_diplomacy_message, player_idx, 10, 0xfa);
            break;
        case 5:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x17, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Race_(_diplomacy_message, player_idx, 0xfa);
            break;
        case 6:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x18, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Name_And_Tech_(_diplomacy_message, player_idx, item_idx, 0xfa);
            break;
        case 7:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x19, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Name_And_System_(_diplomacy_message, player_idx, item_idx, 0xfa);
            break;
        case 8:
            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x1a, _diplomacy_message, 0xfa);
            JIM::Decode_Text_Race_(_diplomacy_message, player_idx, 0xfa);
            break;
        default:
            break;
        }
    }

    /**
     * @brief Displays a list for the human player to select a demand (e.g., star system or tech).
     * 
     * @param player_idx The index of the player making the demand.
     * @param other_player_idx The index of the player being demanded from.
     * @param demand_type The type of demand being made (e.g., 7 for a Star System).
     * @param some_id An additional identifier used for system selection.
     * @return int16_t 1 if a choice was made (or for system demands), 0 if cancelled.
     */
    int16_t __cdecl Get_Human_Demand_Choice_(int16_t player_idx, int16_t other_player_idx, int16_t demand_type, int16_t some_id) {
        char cancel_btn_text[40];
        char demand_text[80];
        char separator;
        int16_t result = 0;

        if (demand_type == 7 /* ECON_STAR_SYSTEM */) {
            if (DIP_SCRN::Diplomacy_Generic_System_List_(some_id, player_idx, other_player_idx, 1) != -1) {
                result = 1;
            }
        } else {
            JIM::Get_Text_Message_(DIP_SCRN::s_jimtext_lbx, 79 /* MSG_DEMAND_HEADER */, demand_text, 40);
            JIM::Get_Text_Message_(DIP_SCRN::s_jimtext_lbx, 80 /* MSG_DEMAND_SUBHEADER */, &demand_text[40], 40);

            separator = DIP_SCRN::s__00551100[0];
            fields::Deactivate_Help_List_();

            JIM::Get_Text_Message_(DIP_SCRN::s_jimtext2_lbx, 0 /* MSG_CANCEL */, cancel_btn_text, 40);

            if (DIP_SCRN::Diplomacy_Generic_List_(2, demand_text, 0, cancel_btn_text) == 0) {
                result = 1;
            }
        }

        return result;
    }

    short __cdecl Diplomacy_Generic_System_List_(short star_idx, short giver_player_idx, short receiver_player_idx, short mode_flag)
    {
        s_animation_header* saved_bg_seg;
        int16_t result;
        int8_t current_selection;
        uint8_t enabled_flags[10];
        char list_title[40];

        COLONY::_using_colony_screen_palette = 1;
        MOX::_star_bg_index = 0;
        saved_bg_seg = MOX::_star_bg_seg;
        MOX::_current_screen = 6;
        MOX::_galaxy_map_scanned_star = star_idx;
        _diplomacy_scan_input_flag = star_idx;
        MOX::_star_bg_seg = _diplomacy_system_star_background_seg;
        _diplomacy_system_giver = (uint8_t)giver_player_idx;
        _diplomacy_system_receiver = (uint8_t)receiver_player_idx;

        Diplomacy_System_Scan_(star_idx, giver_player_idx, &_diplomacy_system_homeworld_flag, &_diplomacy_system_planet_count, _diplomacy_system_planet_list);
        
        _diplomacy_system_population = COLCALC::Get_System_Population_(giver_player_idx, star_idx);
        
        JIM::Get_Text_Message_(s_jimtext2_lbx, 0x38 /* 56 */, _system_million_colonists_buffer, 0x1E /* 30 */);
        JIM::Decode_Text_Stat_(_system_million_colonists_buffer, _diplomacy_system_population, 0x1E);
        
        JIM::Get_Text_Message_(s_jimtext2_lbx, 0x3A /* 58 */, _system_x_star_system_buffer, 0x23 /* 35 */);
        JIM::Decode_Text_System_(_system_x_star_system_buffer, star_idx, 0x23);

        memset(enabled_flags, 1, 10);

        JIM::Get_Text_Message_(s_jimtext_lbx, 0x4B /* 75 */, list_title, 0x28 /* 40 */);

        if (mode_flag == 1) {
            Get_Human_Demand_Message_(receiver_player_idx, giver_player_idx, 7, star_idx);
        } else {
            JIM::Get_Text_Message_(s_jimtext_lbx, 0x4E /* 78 */, _diplomacy_message, 0xFA /* 250 */);
        }

        JIM::Get_Text_Message_(s_jimtext_lbx, 0x4F /* 79 */, _diplomacy_list_choice[0], 0x32 /* 50 */);
        JIM::Get_Text_Message_(s_jimtext_lbx, 0x50 /* 80 */, _diplomacy_list_choice[1], 0x32);
        _diplomacy_list_choice[2][0] = s_empty[0];

        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(Diplomacy_Generic_System_List_Draw_, 2);
        fields::Set_Input_Delay_(1);

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);

        current_selection = 0;
        result = fields::Get_List_Field_(10, 102, 245, list_title, (char*)_diplomacy_list_choice, 50, &current_selection, enabled_flags, 17, 0, 0, nullptr, 0, 0);

        MOX::_star_bg_seg = saved_bg_seg;
        COLONY::_using_colony_screen_palette = 0;

        if (result == -1 || result == 1) {
            result = -1;
        }

        return result;
    }

    // Suggested name: DIP_SCRN::Draw_Diplomacy_System_Text_
    void __cdecl Draw_Diplomacy_System_Text_(int16_t star_idx, int16_t selected_planet_idx) {
        char text_buffer[80];
        bool use_special_colors = false;

        if (selected_planet_idx == -1) {
            fonts::Set_Font_Style_Shadow_Down_(4, &DIP_SCRN::_diplomacy_normal_colors);
        } else {
            use_special_colors = true;
            fonts::Set_Font_Style_Shadow_Down_(4, &DIP_SCRN::_diplomacy_special_colors);
        }

        strcpy(text_buffer, DIP_SCRN::_system_x_star_system_buffer);

        if (!COLCALC::May_Surrender_Star_(star_idx, DIP_SCRN::_diplomacy_system_giver, DIP_SCRN::_diplomacy_system_receiver)) {
            fonts::Set_Font_Style_Shadow_Down_(4, &DIP_SCRN::_diplomacy_special_colors);
            strcat(text_buffer, DIP_SCRN::_system_unsurrenderable_buffer);
            use_special_colors = true;
        }

        fonts::Print_(10, 287 /* 0x11f */, text_buffer);

        if (selected_planet_idx != (int16_t)0xFFFF || use_special_colors) {
            fonts::Set_Font_Style_Shadow_Down_(3, &DIP_SCRN::_diplomacy_special_colors);
        } else {
            fonts::Set_Font_Style_Shadow_Down_(3, &DIP_SCRN::_diplomacy_normal_colors);
        }

        fonts::Print_(10, 309 /* 0x135 */, DIP_SCRN::_system_million_colonists_buffer);

        if (DIP_SCRN::_diplomacy_system_planet_count > 0) {
            int y_offset = 32;
            for (int16_t i = 0; i < DIP_SCRN::_diplomacy_system_planet_count; ++i) {
                uint8_t planet_idx = DIP_SCRN::_diplomacy_system_planet_list[i];
                s_planet_data* planet = &MOX::_planet[planet_idx];

                sprintf(text_buffer, "%s, %s", MOX::_planet_size_string[planet->size], MOX::_planet_climate_string[planet->climate]);

                if ((int16_t)planet_idx == selected_planet_idx) {
                    strcat(text_buffer, DIP_SCRN::_system_homeworld_buffer);
                }

                fonts::Print_(10, y_offset + 291 /* 0x123 */, text_buffer);
                y_offset += 14;
            }
        }
    }

    void __cdecl Send_Diplomacy_Handshake_(int16_t sender_player_id, int16_t target_player_id) {
        DIP_SCRN::_net_diplomacy_message.sender_player_id = (uint8_t)sender_player_id;
        DIP_SCRN::_net_diplomacy_message.target_player_id = (uint8_t)target_player_id;

        // Note: _net_diplomacy_message.arg1 is left uninitialized by this function.

        DIP_SCRN::_net_diplomacy_message.arg2 = -1;
        DIP_SCRN::_net_diplomacy_message.msg_type = 1012 /* 0x3F4 */;

        NETMOX::Mox_Send_Message_(
            MOX::_player[sender_player_id].network_player_id,
            2000 /* 0x7D0 */,
            &DIP_SCRN::_net_diplomacy_message,
            8
        );

        RUSS::Mox_Update_();
    }

    /**
     * @address 00417150
     */
    void __cdecl Diplomacy_Generic_System_List_Draw_() {
        Draw_Diplomacy_Synch_Mode_();
        Draw_Diplomacy_System_();

        Draw_Diplomacy_System_Text_(
            _diplomacy_scan_input_flag,
            _diplomacy_system_homeworld_flag
        );
    }

    /**
     * @address 0041c480
     */
    void __cdecl Diplomacy_Net_Switcheroo_() {
        uint8_t original_talker = (uint8_t)DIP_SCRN::_talker_ambassador;
        uint8_t original_listen = DIP_SCRN::_listen_ambassador;

        DIP_SCRN::_old_response_message = -1;
        DIP_SCRN::_net_diplomacy_message.arg2 = -1;

        // Perform Ambassador swap
        DIP_SCRN::_talker_ambassador = original_listen;
        DIP_SCRN::_listen_ambassador = original_talker;

        // Populate message with original state before variables were swapped
        DIP_SCRN::_net_diplomacy_message.sender_player_id = original_listen;
        DIP_SCRN::_net_diplomacy_message.target_player_id = original_talker;
        DIP_SCRN::_net_diplomacy_message.msg_type = 1011 /* 0x3F3 */;

        // Send message to the new talker (who was the original listener)
        NETMOX::Mox_Send_Message_(
            MOX::_player[DIP_SCRN::_talker_ambassador].network_player_id,
            2000,
            &DIP_SCRN::_net_diplomacy_message,
            sizeof(s_dipscrn_net_message)
        );

        RUSS::Mox_Update_();

        DIP_SCRN::_response_message = 0;
        DIP_SCRN::_old_response_message = 0;
    }

    /**
     * @address 0041ca80
     */
    int8_t __cdecl Diplomacy_Message_Is_Interactive_(int16_t msg_id) {
        if (msg_id >= 0x53 && msg_id <= 0x55) {
            return 1;
        }

        if (msg_id >= 0x61 && msg_id <= 0x68) {
            return 1;
        }

        if (msg_id == 0x7D) {
            return 1;
        }

        if (msg_id == 0x56) {
            return 1;
        }

        if (msg_id == 0x69) {
            return 1;
        }

        if (msg_id == 0x6A) {
            return 1;
        }

        if (msg_id >= 0x1C && msg_id <= 0x24) {
            return 1;
        }

        return 0;
    }

    void __cdecl Diplomacy_Receive_Message_(uint32_t param_1, uint16_t msg_code, uint32_t param_3, s_dipscrn_net_message* net_msg) {
        if (msg_code < 2000 || msg_code >= 3000) {
            return;
        }

        if (msg_code == 2000) {
            _net_diplomacy_chat_message[0] = s__00551100[0];
            _receive_chat_message[0] = s__00551100[0];

            _net_diplomacy_message.sender_player_id = net_msg->sender_player_id;
            _net_diplomacy_message.target_player_id = net_msg->target_player_id;
            _net_diplomacy_message.msg_type = net_msg->msg_type;
            _net_diplomacy_message.arg2 = net_msg->arg2;
            _net_diplomacy_message.arg1 = net_msg->arg1;

            switch (_net_diplomacy_message.msg_type) {
                case 1006 /* 0x3ee */:
                    _net_diplomacy_esc_pressed = 1;
                    return;

                case 1011 /* 0x3f3 */:
                    _net_diplomacy_message.msg_type = 0;
                    _response_message = 0;
                    _old_response_message = 0;
                    sprintf(_diplomacy_message, s__00551100);
                    // Fallthrough to default behavior
                    break;

                case 1012 /* 0x3f4 */:
                    _synch_up_established_flag = 1;
                    return;

                case 1014 /* 0x3f6 */:
                    return;

                default:
                    break;
            }

            _response_message = _net_diplomacy_message.msg_type;
            _talker_ambassador = _net_diplomacy_message.sender_player_id;
            _listen_ambassador = _net_diplomacy_message.target_player_id;
            return;
        }

        if (msg_code == 2100 /* 0x834 */) {
            strcpy(_receive_chat_message, reinterpret_cast<char*>(net_msg));
        }
    }
}