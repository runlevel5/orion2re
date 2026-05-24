#include "pch.h"

namespace DIP_SCRN {
    int16_t __cdecl Diplomacy_Determine_Treaty_Proposal_(int16_t proposer_idx, int16_t target_idx, int16_t treaty_type) {
        int16_t sVar1;
        int16_t success;
        int16_t local_c;
        int16_t local_a;
        int16_t local_8;
        int16_t local_6;
        int16_t local_4;

        uint16_t initial_response = _response_message;

        if (MOX::_player[target_idx].objectives == 100 /* 'd' */) {
            sVar1 = Check_Human_Treaty_Proposal_(proposer_idx, target_idx, treaty_type);
        }
        else {
            sVar1 = diplomac::Check_Treaty_Proposal_(target_idx, proposer_idx, treaty_type, &_response_message, &local_4, &local_6, &local_8, &local_a, &local_c);
        }

        if ((sVar1 == 0) || (sVar1 == 3)) {
            success = (sVar1 != 0) ? 1 : 0;
        }
        else {
            if (MOX::_player[target_idx].objectives == 100 /* 'd' */) {
                sVar1 = Diplomacy_Human_Need_Better_Offer_(proposer_idx, target_idx);
            }
            else {
                sVar1 = Diplomacy_Need_Better_Offer_(local_4, local_6, local_8, local_a, local_c);
            }

            if (sVar1 == 1) {
                success = 1;
                switch (treaty_type) {
                case 1:
                    _response_message = 0x83;
                    break;
                case 2:
                    _response_message = 0x84;
                    break;
                case 3:
                    _response_message = 0x87;
                    break;
                case 7:
                    _response_message = 0x85;
                    break;
                case 8:
                    _response_message = 0x86;
                    break;
                default:
                    break;
                }
            }
            else {
                _response_message = 0x8A;
                success = 0;
            }
        }

        if (MOX::_player[target_idx].objectives == 100 /* 'd' */) {
            if (success == 1) {
                Get_Human_Form_Treaty_Message_(proposer_idx, target_idx, treaty_type, 3);
            }
            else {
                Get_Human_Form_Treaty_Message_(proposer_idx, target_idx, treaty_type, 0);
            }
        }
        else {
            npcdiplo::NPC_Message_Rejection_Check_(target_idx, proposer_idx, &_response_message);
        }

        if (initial_response == _response_message) {
            _response_message = 0x8A;
        }

        return success;
    }

    void __cdecl Diplomacy_Break_Treaty_(int16_t param_1, int16_t param_2) {
        uint8_t option_flags[7] = { 0 };

        if (MOX::_player[param_1].trade_treaty[param_2] == 1) {
            option_flags[0] = 1;
        }
        if (MOX::_player[param_1].research_treaty[param_2] == 1) {
            option_flags[1] = 1;
        }
        if (MOX::_player[param_1].tribute_treaty[param_2] > 0) {
            option_flags[2] = 1;
        }
        if (MOX::_player[param_1].treaty[param_2] == 1) {
            option_flags[3] = 1;
        }
        if (MOX::_player[param_1].treaty[param_2] == 2) {
            option_flags[4] = 1;
        }
        option_flags[5] = 1;

        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Set_Input_Delay_(1);

        for (int i = 0; i < 7; ++i) {
            strcpy(_diplomacy_list_choice[i], _break_treaty_diplomacy_list[i]);
        }

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);
        Setup_Diplomacy_Help_(7, _break_treaty_help_entry);

        int8_t list_pos = 0;
        int16_t choice = fields::Get_List_Field_(
            10, 118, 245,
            _break_treaty_title,
            (char*)_diplomacy_list_choice,
            50,
            &list_pos,
            option_flags,
            17,
            nullptr,
            0,
            nullptr,
            0,
            0
        );

        if (choice > 4) {
            return;
        }

        switch (choice) {
        case 0:
            _old_response_message = -1;
            Diplomacy_Break_Treaty_Resolution_(param_1, param_2, 7);
            break;
        case 1:
            _old_response_message = -1;
            Diplomacy_Break_Treaty_Resolution_(param_1, param_2, 8);
            break;
        case 2:
            _old_response_message = -1;
            if (MOX::_player[param_1].tribute_treaty[param_2] == 1) {
                Diplomacy_Break_Treaty_Resolution_(param_1, param_2, 9);
            }
            else {
                Diplomacy_Break_Treaty_Resolution_(param_1, param_2, 10);
            }
            break;
        case 3:
            _old_response_message = -1;
            Diplomacy_Break_Treaty_Resolution_(param_1, param_2, 1);
            break;
        case 4:
            _old_response_message = -1;
            Diplomacy_Break_Treaty_Resolution_(param_1, param_2, 2);
            break;
        }
    }

    void __cdecl Get_Human_Form_Treaty_Message_(int16_t proposer_idx, int16_t target_idx, int16_t treaty_type, int16_t msg_type) {
        switch (treaty_type) {
        case 1:
            if (msg_type != 3) {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x27, _diplomacy_message, 0xFA);
            }
            else {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x26, _diplomacy_message, 0xFA);
            }
            break;
        case 2:
            if (msg_type != 3) {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x29, _diplomacy_message, 0xFA);
            }
            else {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x28, _diplomacy_message, 0xFA);
            }
            break;
        case 7:
            if (msg_type != 3) {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x21, _diplomacy_message, 0xFA);
            }
            else {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x20, _diplomacy_message, 0xFA);
            }
            break;
        case 8:
            if (msg_type != 3) {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x23, _diplomacy_message, 0xFA);
            }
            else {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x22, _diplomacy_message, 0xFA);
            }
            break;
        case 9:
        case 10:
            if (msg_type != 3) {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x25, _diplomacy_message, 0xFA);
            }
            else {
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x24, _diplomacy_message, 0xFA);
            }
            break;
        default:
            break;
        }
    }

    void __cdecl Get_Diplomacy_Demand_Treaty_List_(int16_t player1, int16_t player2, int16_t treaty_type, int16_t* out_count, uint8_t* out_list) {
        *out_count = 0;

        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (player2 != i && player1 != i) {
                if (MOX::_player[i].contact[player2] == 1 && MOX::_player[i].contact[player1] == 1) {
                    bool add_to_list = false;

                    if (treaty_type == 2) {
                        if (MOX::_player[i].treaty[player2] == 2) {
                            add_to_list = true;
                        }
                    }
                    else if (treaty_type == 3) {
                        if (MOX::_player[i].treaty[player2] == 4 || MOX::_player[i].treaty[player2] == 5) {
                            add_to_list = true;
                        }
                    }
                    else if (treaty_type == 5) {
                        if (MOX::_player[i].treaty[player2] < 4) {
                            add_to_list = true;
                        }
                    }

                    if (add_to_list) {
                        out_list[*out_count] = static_cast<uint8_t>(i);
                        *out_count += 1;
                    }
                }
            }
        }
    }

    int16_t __cdecl Diplomacy_Generic_List_(int16_t item_count, char* list_strings, int8_t add_cancel, char* background) {
        if (item_count > 8) {
            item_count = 8;
        }

        uint8_t item_states[10];
        for (int i = 0; i < 10; ++i) {
            item_states[i] = 1;
        }

        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        Setup_Full_Screen_Diplomacy_Help_(0x2b6);
        fields::Set_Input_Delay_(1);

        for (int16_t i = 0; i < item_count; ++i) {
            const char* src = reinterpret_cast<const char*>(list_strings + i * 40);
            char* dst = _diplomacy_list_choice[i];
            strcpy(dst, src);
        }

        int16_t cancel_idx;
        if (add_cancel == 1) {
            JIM::Get_Text_Message_(s_jimtext_lbx, 46, _diplomacy_list_choice[item_count], 50);
            _diplomacy_list_choice[item_count + 1][0] = MOX::s__00551100[0];
            cancel_idx = item_count;
        }
        else {
            cancel_idx = item_count - 1;
            _diplomacy_list_choice[item_count][0] = MOX::s__00551100[0];
        }

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);

        int8_t selected_idx = 0;

        int16_t result = fields::Get_List_Field_(
            10, 0x76, 0xf5, (char*)background, (char*)_diplomacy_list_choice, 50,
            &selected_idx, item_states, 0x11, nullptr, 0, nullptr, 0, 0
        );

        if (result == -1 || result == cancel_idx) {
            result = -1;
        }

        return result;
    }

    void __cdecl Diplomacy_Offer_Gift_(int16_t player_idx, int16_t target_idx) {
        uint8_t options[16] = { 0 };
        int16_t local_446 = 0;
        uint8_t local_424[212];
        int32_t local_350[212];

        options[0] = 1;
        options[1] = 1;

        if (MOX::_player[target_idx].tribute_treaty[player_idx] > 0) {
            options[0] = 0;
            options[1] = 0;
        }
        else {
            int16_t player_tribute = MOX::_player[player_idx].tribute_treaty[target_idx];
            if (player_tribute > 0) {
                options[0] = 0;
                options[1] = 0;
            }
            else if (player_tribute == 1) {
                options[0] = 0;
            }
            else if (player_tribute == 2) {
                options[0] = 0;
                options[1] = 0;
            }
        }

        local_446 = 0;
        npcdiplo::Get_Differential_Tech_List_(target_idx, player_idx, 0, 0xc343cb00, local_424, local_350, &local_446, 1);

        if (local_446 > 0) {
            options[2] = 1;
        }

        if (Check_Surrender_System_(player_idx, target_idx) == 1) {
            options[3] = 1;
        }

        if (MOX::_player[player_idx].bc > 100) {
            options[4] = 1;
        }

        options[5] = 1;

        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Set_Input_Delay_(1);

        for (int i = 0; i < 7; ++i) {
            strcpy(_diplomacy_list_choice[i], _offer_diplomacy_list[i]);
        }

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);

        Setup_Diplomacy_Help_(7, &_offer_help_entry);

        int8_t title = 0;

        int16_t choice = fields::Get_List_Field_(
            10, 0x76, 0xf5,
            _offer_title,
            (char*)_diplomacy_list_choice,
            0x32,
            &title,
            options,
            0x11,
            nullptr, 0, nullptr, 0, 0
        );

        switch (choice) {
        case 0:
            _old_response_message = -1;
            diplomac::Start_Tribute_Treaty_(player_idx, target_idx, 1);
            if (MOX::_player[target_idx].objectives != 100) {
                diplomac::Get_Gift_Response_(player_idx, target_idx, 2, 1, &_response_message);
                npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
            }
            else {
                Get_Human_Gift_Response_(player_idx, target_idx, 2, 1);
            }
            break;

        case 1:
            _old_response_message = -1;
            diplomac::Start_Tribute_Treaty_(player_idx, target_idx, 2);
            if (MOX::_player[target_idx].objectives != 100) {
                diplomac::Get_Gift_Response_(player_idx, target_idx, 2, 2, &_response_message);
                npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
            }
            else {
                Get_Human_Gift_Response_(player_idx, target_idx, 2, 2);
            }
            break;

        case 2:
        {
            int16_t tech_idx = Diplomacy_Generic_Exchange_Tech_List_(player_idx, target_idx, _offer_title, local_424, local_446, 0);
            if (tech_idx == -1) {
                *_diplomacy_message = MOX::s__00551100[0];
            }
            else {
                uint8_t tech_app = local_424[tech_idx];
                _old_response_message = -1;
                COLCALC::Player_Gets_Tech_App_(&MOX::_player[target_idx], tech_app);

                if (MOX::_player[target_idx].objectives != 100) {
                    diplomac::Get_Gift_Response_(player_idx, target_idx, 3, tech_app, &_response_message);
                    npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
                    npcdiplo::Get_Exchange_Tech_List_(player_idx, target_idx, &_exchange_tech_count, _target_tech_list, _exchange_tech_list);
                }
                else {
                    Get_Human_Gift_Response_(player_idx, target_idx, 3, tech_app);
                }
            }
            break;
        }

        case 3:
        {
            int16_t system_idx = Diplomacy_System_Display_(player_idx, _current_ambassador, 0);
            if (system_idx == -1) {
                *_diplomacy_message = MOX::s__00551100[0];
            }
            else {
                _old_response_message = -1;
                if (MOX::_player[target_idx].objectives != 100) {
                    diplomac::Get_Gift_Response_(player_idx, target_idx, 4, system_idx, &_response_message);
                    npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
                }
                else {
                    Get_Human_Gift_Response_(player_idx, target_idx, 4, system_idx);
                }

                if (COLCALC::May_Surrender_Star_(system_idx, player_idx, target_idx)) {
                    COLCALC::Surrender_Star_(system_idx, player_idx, target_idx);
                }
            }
            break;
        }

        case 4:
            Diplomacy_Offer_Money_(player_idx, target_idx, 1);
            break;

        default:
            break;
        }
    }

    void __cdecl Diplomacy_Mouse_Init_(int16_t param_1) {
        _diplomacy_mouse_bitmap = (uint8_t*)allocate::Allocate_First_Block_(MOX::_screen_seg, 0x395);
        s_animation_header* diplomat_anim = (s_animation_header*)
            farload::Far_Reload_Next_(s_DIPLOMAT_LBX_00552350, (int)MOX::_player[param_1].race, MOX::_screen_seg);

        bitmap::Clear_Mouse_Image_(0);
        bitmap::Create_Blank_Bitmap_(30, 30, _diplomacy_mouse_bitmap);
        bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)_diplomacy_mouse_bitmap, diplomat_anim);

        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);

        animate::Draw_Palette_(diplomat_anim);
        remap::Update_Glass_Remap_Colors_();
        palette::Set_Palette_Changes_(0, 0xFF);

        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
        bitmap::Draw_Bitmap_To_Mouse_(0, 0, (s_animation_header*)_diplomacy_mouse_bitmap, 0);

        _diplomacy_system_star_background_seg = (s_animation_header*)farload::Far_Reload_Next_(s_dipstars_lbx_00552374, 0, MOX::_screen_seg);
        for (int i = 0; i < 11; ++i) {
            MOX::_fleet_galaxy_star_seg[i] = (s_animation_header*)
                farload::Far_Reload_Next_(s_dipstars_lbx_00552374, i + 1, MOX::_screen_seg);
        }

        _main_diplomacy_title = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        for (int i = 0; i < 9; ++i) {
            _main_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        }

        for (int i = 0; i < 11; ++i) {
            _main_net_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        }

        for (int i = 0; i < 5; ++i) {
            _main_repulsive_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        }

        for (int i = 0; i < 6; ++i) {
            _main_net_repulsive_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        }

        _propose_treaty_title = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        for (int i = 0; i < 7; ++i) {
            _propose_treaty_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        }

        _break_treaty_title = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        for (int i = 0; i < 7; ++i) {
            _break_treaty_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        }

        _demand_title = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        for (int i = 0; i < 11; ++i) {
            _demand_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        }

        _offer_title = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        for (int i = 0; i < 7; ++i) {
            _offer_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        }

        for (int i = 0; i < 3; ++i) {
            _agree_diplomacy_list[i] = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 20);
        }

        _choose_a_system_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _system_cancel_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 20);
        _system_million_colonists_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        _system_homeworld_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30);
        _system_x_star_system_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 35);
        _player_number_name_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _player_name_ambassador_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _you_were_framed_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _system_unsurrenderable_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _enter_your_message_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _percent_tribute_treaty_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _statement_trade_treaty_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _statement_research_treaty_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 40);
        _net_listen_mode_option_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 20);
        _net_listen_mode_good_bye_buffer = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 20);

        file_ani::Set_File_Animation_Buffer_(allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x7C830));
        Load_Diplomacy_Text_Messages_();

        _diplomacy_current_music = -1;
        _diplomacy_good_music = (int16_t)MOX::_player[param_1].race + 1;
        _diplomacy_bad_music = (int16_t)random::Random_(3) + 13;
    }

    void __cdecl Start_Diplomacy_Music_(int16_t music_type) {
        int16_t new_music;

        if (music_type == 0) {
            new_music = _diplomacy_bad_music;
        }
        else {
            new_music = _diplomacy_good_music;
        }

        if (new_music != _diplomacy_current_music) {
            _diplomacy_current_music = new_music;
        }
    }

    void __cdecl Get_Diplomacy_Statement_(int16_t param_1, int16_t param_2, int16_t param_3, char* param_4) {
        param_4[0] = '\0';

        char filename[16];
        switch (MOX::_settings.language) {
        case 1: strcpy(filename, s_DIPLOMSG_LBX); break;
        case 2: strcpy(filename, s_DIPLOMSF_LBX); break;
        case 3: strcpy(filename, s_DIPLOMSS_LBX); break;
        case 4: strcpy(filename, s_DIPLOMSI_LBX); break;
        case 5: strcpy(filename, s_DIPLOMSP_LBX); break;
        case 0:
        default: strcpy(filename, s_DIPLOMSE_LBX); break;
        }

        bool add_framed = false;
        if (param_1 == 0x14 || param_1 == 0x16 || param_1 == 0x1D || param_1 == 0x1F ||
            param_1 == 0x28 || param_1 == 0x2A || param_1 == 0x31 || param_1 == 0x33 ||
            param_1 == 0x3A || param_1 == 0x3C || param_1 == 0x48 || param_1 == 0x4A ||
            param_1 == 0x8F || param_1 == 0x91) {
            add_framed = true;
        }

        int file_msg_id = param_1;
        if (add_framed || param_1 == 0x6A) {
            file_msg_id = param_1 - 1;
        }

        uint8_t buf[5202];
        farload::Farload_Data_Static_(filename, file_msg_id, buf, 0, 1, 5202);

        int count = buf[1];
        int rnd_idx = random::Random_(count) - 1;
        char* record = reinterpret_cast<char*>(buf) + 2 + rnd_idx * 200;

        int out_idx = 0;
        for (int i = 0; record[i] != '\0'; ++i) {
            uint8_t c = record[i];
            if (c < 0x80 || c > 0x95) {
                param_4[out_idx++] = c;
                continue;
            }

            param_4[out_idx] = '\0';
            char* replacement = nullptr;
            char temp_buf[64];

            switch (c) {
            case 0x80:
                replacement = MOX::_player[param_2].name;
                break;
            case 0x81:
                replacement = MOX::_player[param_2].race_name;
                break;
            case 0x82:
                replacement = MOX::_player[param_3].race_name;
                break;
            case 0x83:
                replacement = MOX::_player[param_3].name;
                break;
            case 0x84:
                replacement = An_(MOX::_player[param_2].race_name);
                break;
            case 0x85:
                replacement = An_(MOX::_player[param_3].race_name);
                break;
            case 0x86:
                replacement = MOX::_star[MOX::_player[param_3].diplomacy_system[param_2]].name;
                break;
            case 0x87:
            case 0x88:
                replacement = MOX::_player[MOX::_player[param_3].diplomacy_proposal_war_player[param_2]].race_name;
                break;
            case 0x89:
                replacement = TECH::Technology_Applications_Name_(MOX::_player[param_3].diplomacy_proposal_exchange_tech[param_2], param_3);
                break;
            case 0x8A: {
                int treaty = MOX::_player[param_3].treaty[param_2];
                if (treaty == 1 || treaty == 2) {
                    replacement = MOX::_treaty_labels[treaty];
                }
                else if (MOX::_player[param_3].trade_treaty[param_2] != 0) {
                    replacement = _statement_trade_treaty_buffer;
                }
                else if (MOX::_player[param_3].research_treaty[param_2] != 0) {
                    replacement = _statement_research_treaty_buffer;
                }
                break;
            }
            case 0x8B:
                replacement = MOX::_player[MOX::_player[param_3].diplomacy_proposal_war_player[param_2]].name;
                break;
            case 0x8C: {
                int gift = MOX::_player[param_3].last_gift[param_2];
                if (gift >= 1 && gift <= 4) {
                    JIM::Get_Text_Message_(s_jimtext2_lbx, 0x4A + gift, temp_buf, 40);
                    replacement = temp_buf;
                }
                else {
                    replacement = const_cast<char*>(s_FWEE);
                }
                break;
            }
            case 0x8D:
                replacement = TECHDATA::_buildings[MOX::_player[param_3].diplomacy_value[param_2]].name;
                break;
            case 0x8E:
                if (MOX::_settings.language == 0) {
                    sprintf(temp_buf, s_fmt_dot, MOX::_stardate / 10, MOX::_stardate % 10);
                }
                else {
                    sprintf(temp_buf, s_fmt_comma, MOX::_stardate / 10, MOX::_stardate % 10);
                }
                replacement = temp_buf;
                break;
            case 0x8F:
                break;
            case 0x90:
            case 0x95:
                if (MOX::_player[param_3].diplomacy_proposal_tribute_bribe[param_2] == 0) {
                    if (MOX::_player[param_3].diplomacy_proposal_tech_bribe1[param_2] != 0) {
                        replacement = TECH::Technology_Applications_Name_(MOX::_player[param_3].diplomacy_proposal_tech_bribe1[param_2], param_3);
                    }
                    else if (MOX::_player[param_3].diplomacy_proposal_gold_bribe[param_2] == 0) {
                        if (MOX::_player[param_3].diplomacy_proposal_system_bribe[param_2] != -1) {
                            JIM::Get_Text_Message_(s_jimtext2_lbx, 0x3A, temp_buf, 50);
                            JIM::Decode_Text_System_(temp_buf, MOX::_player[param_3].diplomacy_proposal_system_bribe[param_2], 50);
                            replacement = temp_buf;
                        }
                    }
                    else {
                        sprintf(temp_buf, s_fmt_BC, MOX::_player[param_3].diplomacy_proposal_gold_bribe[param_2]);
                        replacement = temp_buf;
                    }
                }
                else {
                    strcpy(temp_buf, _percent_tribute_treaty_buffer);
                    int stat_val = (MOX::_player[param_3].diplomacy_proposal_tribute_bribe[param_2] == 1) ? 5 : 10;
                    JIM::Decode_Text_Stat_(temp_buf, stat_val, 40);
                    replacement = temp_buf;
                }
                break;
            case 0x91:
                replacement = MOX::_player[MOX::_player[param_3].diplomacy_proposal_war_player[param_2]].race_name;
                break;
            case 0x92: {
                int broken = MOX::_player[param_3].last_broken_treaty[param_2];
                if (broken == 7) {
                    replacement = _statement_trade_treaty_buffer;
                }
                else if (broken == 8) {
                    replacement = _statement_research_treaty_buffer;
                }
                else {
                    replacement = MOX::_treaty_labels[broken];
                }
                break;
            }
            case 0x93:
                replacement = MOX::_star[MOX::_player[param_3].diplomacy_system[param_2]].name;
                break;
            case 0x94:
                replacement = MOX::_player[MOX::_player[param_3].trust_breaker_player[param_2]].race_name;
                break;
            }

            if (replacement) {
                strcpy(param_4 + out_idx, replacement);
                out_idx += static_cast<int>(strlen(replacement));
            }
        }

        param_4[out_idx] = '\0';

        if (add_framed) {
            strcat(param_4, _you_were_framed_buffer);
        }

        Start_Diplomacy_Music_(buf[0]);
    }

    /**
     * Checks if player_a has at least one system (up to 3) that could potentially
     * surrender to player_b.
     */
    bool __cdecl Check_Surrender_System_(int16_t player_a, int16_t player_b) {
        int16_t star_idx = 0;
        int16_t surrender_count = 0;

        if (MOX::_NUM_STARS > 0) {
            do {
                // Limit search once we find at least 3 systems
                if (surrender_count >= 3) {
                    break;
                }

                // has_colony is treated as a bitmask where bit N indicates player N has a colony
                if (((MOX::_star[star_idx].has_colony >> (player_a & 0x1F)) & 1) != 0) {
                    if (COLCALC::May_Surrender_Star_(star_idx, player_a, player_b)) {
                        surrender_count++;
                    }
                }

                star_idx++;
            } while (star_idx < MOX::_NUM_STARS);
        }

        return surrender_count >= 1;
    }

    int16_t __cdecl Diplomacy_Need_Better_Offer_(int16_t type, int16_t tech_app, int16_t bc, int16_t star_idx, int16_t tribute) {
        uint8_t selectable[16] = { 0 };
        char text_buffer[40];

        selectable[0] = 1;
        selectable[1] = 1;
        selectable[2] = 1;

        int16_t msg_idx = (random::Random_(2) == 1) ? 0x4A : 0x4B;
        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, msg_idx, text_buffer, 40);

        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Deactivate_Help_List_();
        fields::Set_Input_Delay_(1);

        for (int16_t i = 0; i < 3; ++i) {
            char* src = reinterpret_cast<char**>(_agree_diplomacy_list)[i];
            strcpy(_diplomacy_list_choice[i], src);
        }

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);

        int8_t current_idx = 0;
        int16_t result = fields::Get_List_Field_(
            10, 118, 245, text_buffer, _diplomacy_list_choice[0], 50,
            &current_idx, selectable, 17, nullptr, 0, nullptr, 0, 0
        );

        if (result == -1 || result == 1) {
            return 0;
        }

        switch (type) {
        case 1:
            MOX::_player[MOX::_PLAYER_NUM].bc -= bc;
            MOX::_player[_current_ambassador].bc += bc;
            return 1;
        case 2:
            if (COLCALC::May_Surrender_Star_(star_idx, MOX::_PLAYER_NUM, _current_ambassador)) {
                COLCALC::Surrender_Star_(star_idx, MOX::_PLAYER_NUM, _current_ambassador);
            }
            return 1;
        case 3:
            MOX::_player[MOX::_PLAYER_NUM].tribute_treaty[_current_ambassador] = tribute;
            return 1;
        case 4:
            COLCALC::Player_Gets_Tech_App_(&MOX::_player[_current_ambassador], tech_app);
            return 1;
        default:
            return 1;
        }
    }

    void __cdecl Get_Human_Gift_Response_(int16_t sender_id, int16_t target_id, int16_t gift_type, int16_t gift_arg) {
        if (MOX::_game_type == 1) {
            DIP_SCRN::_old_response_message = DIP_SCRN::_response_message;
            DIP_SCRN::Get_Human_Offer_Gift_Message_(sender_id, gift_type, gift_arg);
            DIP_SCRN::Diplomacy_Load_New_Ambassador_(sender_id);
            DIP_SCRN::Diplomacy_Display_Response_();
            DIP_SCRN::Diplomacy_Load_New_Ambassador_(target_id);
        }
        else {
            DIP_SCRN::_net_diplomacy_message.sender_player_id = static_cast<uint8_t>(sender_id);
            DIP_SCRN::_net_diplomacy_message.target_player_id = static_cast<uint8_t>(target_id);
            DIP_SCRN::_net_diplomacy_message.arg1 = gift_type;
            DIP_SCRN::_net_diplomacy_message.msg_type = 1004 /* DIP_NET_MSG_GIFT_RESPONSE */;
            DIP_SCRN::_net_diplomacy_message.arg2 = gift_arg;

            NETMOX::Mox_Send_Message_(MOX::_player[target_id].network_player_id, 2000, &DIP_SCRN::_net_diplomacy_message, 8);
            RUSS::Mox_Update_();
        }

        switch (gift_type) {
        case 1:
            DIP_SCRN::_response_message = 0xA3;
            break;
        case 2:
            DIP_SCRN::_response_message = 0xA2;
            break;
        case 3:
            DIP_SCRN::_response_message = 0xA1;
            break;
        case 4:
            DIP_SCRN::_response_message = 0xA4;
            break;
        default:
            break;
        }
    }

    /**
     * @brief Scans a star system for colonies belonging to a specific player.
     *
     * @param star_idx Index of the star system to scan.
     * @param player_idx Index of the player whose colonies to search for.
     * @param artemis_planet_idx Output: Index of the planet with an Artemis Net (building index 9), or -1 if none found.
     * @param owned_count Output: Total number of colonies owned by the player in this system.
     * @param planet_indices Output: Array of indices for the planets owned by the player.
     * @return The player index if colonies exist, otherwise the original star owner.
     */
    int16_t __cdecl Diplomacy_System_Scan_(int16_t star_idx, int16_t player_idx, int16_t* artemis_planet_idx, int16_t* owned_count, int8_t* planet_indices) {
        *artemis_planet_idx = -1;
        *owned_count = 0;

        s_star_data* star = &MOX::_star[star_idx];

        // Check if the player has any colonies in this system using a bitmask
        if ((star->has_colony >> (uint8_t)player_idx) & 1) {
            for (int16_t i = 0; i < 5; ++i) {
                int16_t planet_idx = star->planet_index[i];
                if (planet_idx != -1) {
                    int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
                    if (colony_idx != -1) {
                        s_colony* colony = &MOX::_colony[colony_idx];
                        if (colony->owner == (int8_t)player_idx) {
                            planet_indices[*owned_count] = (int8_t)planet_idx;

                            // Check for Artemis Net (building 9)
                            if (colony->buildings[9] != 0) {
                                *artemis_planet_idx = planet_idx;
                            }

                            *owned_count = *owned_count + 1;
                        }
                    }
                }
            }
            return player_idx;
        }

        return (int16_t)star->owner;
    }

    int16_t __cdecl Diplomacy_Offer_Money_(int16_t player_idx, int16_t target_idx, int8_t do_transaction) {
        int16_t result_bc = -1;
        int16_t choice_count;
        int16_t bc_options[5];
        int8_t selection_idx = 0;
        uint8_t selection_flags[15];
        char title_text[40];

        Setup_Full_Screen_Diplomacy_Help_(0x267);

        memset(selection_flags, 1, 15);

        for (int i = 0; i < 15; ++i) {
            _diplomacy_list_choice[i][0] = s__00551100[0];
        }

        int current_bc = MOX::_player[player_idx].bc;
        if (current_bc > 32000) {
            current_bc = 32000;
        }

        int16_t rounded_bc = (int16_t)(current_bc / 20) * 20;

        if (rounded_bc < 100) {
            choice_count = rounded_bc / 20;
            bc_options[0] = 20;
            bc_options[1] = 40;
            bc_options[2] = 60;
            bc_options[3] = 80;
            bc_options[4] = 100;
        }
        else {
            choice_count = 5;
            bc_options[0] = (int16_t)((rounded_bc / 5) / 20) * 20;
            bc_options[1] = (int16_t)((rounded_bc * 2 / 5) / 20) * 20;
            bc_options[2] = (int16_t)((rounded_bc * 3 / 5) / 20) * 20;
            bc_options[3] = (int16_t)((rounded_bc * 4 / 5) / 20) * 20;
            bc_options[4] = (int16_t)(rounded_bc / 20) * 20;
        }

        for (int i = 0; i < choice_count; ++i) {
            sprintf(_diplomacy_list_choice[i], s__d_BC_00552384, (int)bc_options[i]);
            if (MOX::_player[player_idx].bc < (int)bc_options[i]) {
                selection_flags[i] = 0;
            }
        }

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2E, _diplomacy_list_choice[choice_count], 0x28);

        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 8, title_text, 0x28);

        int16_t selection = fields::Get_List_Field_(
            10, 0x76, 0xF5, title_text, _diplomacy_list_choice[0], 0x32,
            &selection_idx, selection_flags, 0x11, 0, 0, 0, 0, 0
        );

        if (selection >= 0 && selection < choice_count) {
            result_bc = bc_options[selection];
            if (do_transaction == 1) {
                _old_response_message = -1;
                MOX::_player[player_idx].bc -= (int)result_bc;
                MOX::_player[target_idx].bc += (int)result_bc;

                if (MOX::_player[target_idx].objectives == 100 /* 'd' */) {
                    Get_Human_Gift_Response_(player_idx, target_idx, 1, result_bc);
                }
                else {
                    diplomac::Get_Gift_Response_(player_idx, target_idx, 1, result_bc, &_response_message);
                    if ((int)(MOX::_stardate * 5 - 175000) <= (int)result_bc) {
                        npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
                    }
                }
            }
        }

        return result_bc;
    }

    void Resolve_Delayed_Diplomacy_Orders_() {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            for (int16_t j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                if (i == j) {
                    continue;
                }

                // Only process diplomatic orders if both parties are still in the game
                if (MOX::_player[j].eliminated == 0 && MOX::_player[i].eliminated == 0) {
                    int8_t order = MOX::_player[i].delayed_diplomacy_orders[j];

                    if (order == 0) {
                        diplomac::Declare_War_(i, j);
                    }
                    else if (order == 1) {
                        diplomac::Declare_Peace_(i, j);
                    }
                    else if (order == 2) {
                        diplomac::Break_Treaties_(i, j);
                    }

                    // Reset the order to "no order" (-1) once processed or if invalid
                    MOX::_player[i].delayed_diplomacy_orders[j] = -1;
                }
            }
        }
    }

    void __cdecl Set_Opportunity_Attacks_() {
        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int16_t p_idx = 0; p_idx < (int16_t)MOX::_NUM_PLAYERS; ++p_idx) {
            for (int16_t target_idx = 0; target_idx < (int16_t)MOX::_NUM_PLAYERS; ++target_idx) {
                s_player& player = MOX::_player[p_idx];
                s_player& target = MOX::_player[target_idx];

                bool valid_opportunity = false;

                // Condition for player looking for an opportunity:
                // Must not be eliminated, must not already have reached 100 objectives,
                // and hyperspace flux must not be present.
                if (player.eliminated == 0 && player.objectives != 100) {
                    if (EVENTS::Event_Check_Hyperspace_Flux_() == 0) {
                        // Condition for the target:
                        // Must have reached 100 objectives, must be a different player,
                        // must not be eliminated, and must be in contact and within range.
                        if (target.objectives == 100 && p_idx != target_idx && target.eliminated == 0 &&
                            player.contact[target_idx] == 1 && player.within_range[target_idx] == 1)
                        {
                            valid_opportunity = true;
                        }
                    }
                }

                if (valid_opportunity) {
                    int16_t planet_num = -1;
                    int16_t value = 0;
                    int32_t ratio = 0;

                    AIMOVE::Find_Opportunity_Attack_(p_idx, target_idx, 100, &planet_num, &value, &ratio);

                    player.opportunity_planet_num[target_idx] = planet_num;
                    player.opportunity_planet_value[target_idx] = value;
                    player.opportunity_actual_attacker_ratio[target_idx] = ratio;
                }
                else {
                    player.opportunity_planet_num[target_idx] = -1;
                    player.opportunity_planet_value[target_idx] = 0;
                    player.opportunity_actual_attacker_ratio[target_idx] = 0;
                }
            }
        }
    }

    void __cdecl Show_Sneak_Attack_Message_(int16_t player_idx, int16_t defender_idx) {
        Npc_Diplomacy_Screen_(player_idx, 1);
    }

    void __cdecl Diplomacy_Display_Response_() {
        bool done = false;

        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Deactivate_Help_List_();

        while (!done) {
            timer::Mark_Time_();
            fields::Clear_Fields_();

            int16_t hidden_field_idx = fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, s_empty_string_00551100, 0);
            int16_t input_idx = fields::Get_Input_();

            if (hidden_field_idx == input_idx) {
                done = true;
            }

            if (done) {
                break;
            }

            Draw_Diplomacy_Synch_Mode_();
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(2);
        }

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
    }

    void __cdecl Add_Diplomacy_Turn_Summary_Messages_()
    {
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i)
        {
            for (int16_t j = i + 1; j < MOX::_NUM_PLAYERS; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                if (MOX::_player[j].eliminated != 0)
                {
                    continue;
                }

                if (MOX::_player[i].eliminated != 0)
                {
                    continue;
                }

                if (MOX::_player[MOX::_PLAYER_NUM].contact[j] == 1 ||
                    MOX::_player[MOX::_PLAYER_NUM].contact[i] == 1)
                {
                    if (MOX::_player[i].last_turn_treaty[j] != MOX::_player[i].treaty[j])
                    {
                        switch (MOX::_player[i].treaty[j])
                        {
                        case 1:
                            MSG_::Add_Nonaggression_Msg_(i, j);
                            break;
                        case 2:
                            MSG_::Add_Alliance_Msg_(i, j);
                            break;
                        case 3:
                            MSG_::Add_Peace_Msg_(i, j);
                            break;
                        case 4:
                        case 5:
                        case 6:
                            MSG_::Add_War_Msg_(i, j);
                            break;
                        }
                    }
                }
            }
        }
    }

    void __cdecl Get_Main_Diplomacy_Choices_() {
        bool exit_flag = false;

        if (MOX::_player[_current_ambassador].objectives != 100) {
            npcdiplo::Get_Exchange_Tech_List_(
                MOX::_PLAYER_NUM,
                _current_ambassador,
                &_exchange_tech_count,
                _target_tech_list,
                _exchange_tech_list
            );
        }

        while (!exit_flag) {
            uint8_t main_options[9];
            memset(main_options, 1, sizeof(main_options));

            Diplomacy_Set_Main_Options_(MOX::_PLAYER_NUM, _current_ambassador, main_options);

            int16_t worst_modifier = diplomac::Find_Worst_Modifier_(MOX::_PLAYER_NUM, _current_ambassador);

            if (worst_modifier <= -100) {
                if (_old_response_message != _response_message) {
                    Diplomacy_Display_Response_();
                }
                _response_message = 0x82; /* 130 */
                Diplomacy_Display_Response_();
                exit_flag = true;
            }
            else {
                fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
                fields::Set_Input_Delay_(1);

                for (int i = 0; i < 9; ++i) {
                    strcpy(_diplomacy_list_choice[i], _main_diplomacy_list[i]);
                }

                fonts::Set_Outline_Color_(0, 0xF8);
                fonts::Set_Font_Style_Outline_(4, &_diplomacy_normal_colors);
                fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
                fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
                fonts::Set_Font_LF_(4);

                Setup_Diplomacy_Help_(9, &_main_diplomacy_help_entry);

                int8_t search_string = 0;

                int16_t choice = fields::Get_List_Field_(
                    10, 0x76, 0xF5,
                    _main_diplomacy_title,
                    (char*)_diplomacy_list_choice, 50,
                    &search_string, main_options, 0x11,
                    nullptr, 0, nullptr, 0, 0
                );

                switch (choice) {
                case 0:
                    *_diplomacy_message = MOX::s__00551100[0];
                    Diplomacy_Propose_Treaty_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 1:
                    *_diplomacy_message = MOX::s__00551100[0];
                    Diplomacy_Break_Treaty_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 2:
                    *_diplomacy_message = MOX::s__00551100[0];
                    Diplomacy_Demand_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 3:
                    *_diplomacy_message = MOX::s__00551100[0];
                    Diplomacy_Offer_Gift_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 4:
                    *_diplomacy_message = MOX::s__00551100[0];
                    Diplomacy_Exchange_Technology_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 5:
                    if (Diplomacy_Declare_War_Resolution_(MOX::_PLAYER_NUM, _current_ambassador) == 1) {
                        exit_flag = true;
                        Diplomacy_Display_Response_();
                    }
                    break;
                case 6:
                    if (Diplomacy_Surrender_(MOX::_PLAYER_NUM, _current_ambassador) == 1) {
                        exit_flag = true;
                    }
                    break;
                case 7:
                case -1:
                    exit_flag = true;
                    break;
                default:
                    break;
                }
            }
        }

        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Set_Input_Delay_(1);
    }

    void __cdecl Diplomacy_Screen_(int player_idx) {
        _diplomacy_current_music = -1;
        _synch_up_established_flag = 0;

        fields::Deactivate_Help_List_();
        fields::Enable_Draw_All_Fields_();
        graphics::Reset_Window_();
        video::Clip_Off_();
        mouse::Set_Mouse_List_(&_diplomacy_blank_mouse, 1);
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();

        _old_response_message = -1;
        _net_diplomacy_esc_pressed = 0;
        _current_ambassador = (uint8_t)player_idx;

        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Percent_(10);
        ERIC::Fast_Fade_Out_();

        fields::Set_Input_Delay_(1);
        fields::Clear_Fields_();
        video::Set_Refresh_Stencil_();
        fields::Deactivate_Auto_Function_();

        int16_t modifier = diplomac::Find_Worst_Modifier_(MOX::_PLAYER_NUM, _current_ambassador);
        if (modifier <= -100 || diplodef::_COUNCIL_FLAG == 2 || diplodef::_COUNCIL_FLAG == 3) {
            _response_message = 0x7E;
            if (MOX::_player[MOX::_PLAYER_NUM].treaty[_current_ambassador] < 4) {
                _response_message = 0x7F;
            }
            _ambassador_option = 0;
        }
        else {
            _response_message = 0x80;
            if (MOX::_player[MOX::_PLAYER_NUM].relations[_current_ambassador] < -12) {
                _response_message = 0x81;
            }
            _ambassador_option = 1;
        }

        bool do_net_logic = false;
        if (MOX::_player[_current_ambassador].objectives == 100) {
            if (MOX::_game_type != 0 && MOX::_game_type != 1) {
                do_net_logic = true;
            }
        }
        else {
            if (MOX::_game_type != 0 && MOX::_game_type != 1) {
                _ambassador_option = 1;
            }
        }

        if (do_net_logic) {
            if (NETMOX::_net.is_diplomacy_initiator == 1) {
                _talker_ambassador = static_cast<uint8_t>(MOX::_PLAYER_NUM);
                _listen_ambassador = static_cast<uint8_t>(NETMOX::_net.diplomacy_target_id);
            }
            else {
                _talker_ambassador = NETMOX::_net.diplomacy_target_id;
                _listen_ambassador = static_cast<uint8_t>(MOX::_PLAYER_NUM);
            }
            _ambassador_option = 1;
            _net_diplomacy_chat_message[0] = MOX::s__00551100[0];
            Diplomacy_Fade_In_();
            Get_Net_Diplomacy_Choices_();
        }
        else {
            Diplomacy_Fade_In_();
            if (_ambassador_option == 1) {
                if (MOX::_player[MOX::_PLAYER_NUM].traits[0x13] == 1 ||
                    MOX::_player[_current_ambassador].traits[0x13] == 1) {
                    Get_Main_Repulsive_Diplomacy_Choices_(MOX::_PLAYER_NUM, _current_ambassador, 0);
                }
                else {
                    Get_Main_Diplomacy_Choices_();
                }
            }
            else {
                Diplomacy_Display_Response_();
            }
        }

        JIM::_jim_current_stream = -1;
        JIM::Play_Background_Music_();
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
        file_ani::Reset_File_Animation_Buffer_();
        RUSS::Release_All_Blocks_(MOX::_screen_seg);
        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Percent_(10);
        ERIC::Fast_Fade_Out_();
    }

    void __cdecl Npc_Diplomacy_Screen_(int16_t player_idx, int8_t is_sneak_attack) {
        if (MOX::_player[player_idx].eliminated == 1) {
            return;
        }

        fields::Deactivate_Help_List_();
        fields::Enable_Draw_All_Fields_();
        graphics::Reset_Window_();
        video::Clip_Off_();
        mouse::Set_Mouse_List_(&_diplomacy_blank_mouse, 1);
        MOX2::Get_Auto_Function_(&EVENTS::_jim_saved_autofunction);
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
        _current_ambassador = static_cast<uint8_t>(player_idx);
        _old_response_message = -1;
        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Percent_(10);
        ERIC::Fast_Fade_Out_();
        fields::Set_Input_Delay_(1);
        fields::Clear_Fields_();
        video::Set_Refresh_Stencil_();
        fields::Deactivate_Auto_Function_();
        _ambassador_option = 1;

        if (is_sneak_attack == 1) {
            _response_message = MOX::_player[_current_ambassador].sneak_attack_message;
        }
        else {
            _response_message = MOX::_player[_current_ambassador].diplomacy_message[MOX::_PLAYER_NUM];
        }

        Diplomacy_Fade_In_();

        int16_t original_response = _response_message;

        if ((_response_message >= 0x53 && _response_message <= 0x55) ||
            (_response_message >= 0x61 && _response_message <= 0x68) ||
            _response_message == 0x7d || _response_message == 0x56 ||
            _response_message == 0x69 || _response_message == 0x6a ||
            (_response_message >= 0x1c && _response_message <= 0x24))
        {
            _response_message = 0x80;
            if (MOX::_player[MOX::_PLAYER_NUM].relations[_current_ambassador] < -12) {
                _response_message = 0x81;
            }
            _ambassador_option = 2;
        }

        if (_ambassador_option == 1) {
            Diplomacy_Display_Response_();
            switch (_response_message) {
            case 0x30:
            case 0x31:
            case 0x32:
            case 0x33:
                diplomac::Break_Trade_(MOX::_PLAYER_NUM, _current_ambassador);
                diplomac::Break_Research_(MOX::_PLAYER_NUM, _current_ambassador);
                diplomac::Break_Tribute_(MOX::_PLAYER_NUM, _current_ambassador);
                diplomac::Break_Treaties_(MOX::_PLAYER_NUM, _current_ambassador);
                break;
            case 0x39:
            case 0x3a:
                diplomac::Declare_War_(MOX::_PLAYER_NUM, _current_ambassador);
                break;
            case 0x60:
            case 0x6b:
                Npc_Give_Gift_(_current_ambassador, MOX::_PLAYER_NUM);
                break;
            }
        }
        else {
            if (MOX::_player[_current_ambassador].objectives != 0x64 /* 'd' */) {
                Diplomacy_Display_Response_();
            }
            _response_message = original_response;

            if (Get_Non_Player_Proposals_() == 0) {
                npcdiplo::NPC_Proposal_Rejection_Accept_(_current_ambassador, MOX::_PLAYER_NUM, 1);
                switch (_response_message) {
                case 0x1c:
                case 0x1d:
                case 0x1e:
                case 0x1f:
                case 0x20:
                case 0x21:
                case 0x22:
                case 0x23:
                case 0x24:
                    _response_message = 0x26;
                    Diplomacy_Display_Response_();
                    Accepted_Npc_Demand_Resolution_(MOX::_PLAYER_NUM, _current_ambassador);
                    Npc_Give_Gift_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 0x53:
                    diplomac::Declare_Peace_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 0x54:
                    Npc_Give_Gift_(MOX::_PLAYER_NUM, _current_ambassador);
                    diplomac::Declare_Peace_(MOX::_PLAYER_NUM, _current_ambassador);
                    Accepted_Npc_Demand_Resolution_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 0x55:
                    Npc_Give_Gift_(_current_ambassador, MOX::_PLAYER_NUM);
                    diplomac::Declare_Peace_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 0x56:
                    diplomac::Declare_War_(MOX::_PLAYER_NUM, MOX::_player[_current_ambassador].diplomacy_proposal_war_player[MOX::_PLAYER_NUM]);
                    _response_message = 0x58;
                    Diplomacy_Display_Response_();
                    break;
                case 0x61:
                    diplomac::Start_Treaty_(MOX::_PLAYER_NUM, _current_ambassador, 1);
                    break;
                case 0x62:
                    diplomac::Start_Treaty_(MOX::_PLAYER_NUM, _current_ambassador, 2);
                    break;
                case 0x63:
                    diplomac::Start_Trade_Treaty_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 0x64:
                    diplomac::Start_Research_Treaty_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 0x66:
                    diplomac::Declare_War_(MOX::_PLAYER_NUM, MOX::_player[_current_ambassador].diplomacy_proposal_war_player[MOX::_PLAYER_NUM]);
                    break;
                case 0x67:
                    Npc_Give_Gift_(_current_ambassador, MOX::_PLAYER_NUM);
                    diplomac::Declare_War_(MOX::_PLAYER_NUM, MOX::_player[_current_ambassador].diplomacy_proposal_war_player[MOX::_PLAYER_NUM]);
                    break;
                case 0x68:
                    diplomac::Break_Treaties_(MOX::_PLAYER_NUM, MOX::_player[_current_ambassador].diplomacy_proposal_war_player[MOX::_PLAYER_NUM]);
                    break;
                case 0x69:
                case 0x6a:
                    _response_message = 0x26;
                    Diplomacy_Display_Response_();
                    Accepted_Npc_Demand_Resolution_(MOX::_PLAYER_NUM, _current_ambassador);
                    Npc_Give_Gift_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                }
            }
            else {
                npcdiplo::NPC_Proposal_Rejection_Accept_(_current_ambassador, MOX::_PLAYER_NUM, 0);
                switch (_response_message) {
                case 0x1c:
                case 0x1d:
                case 0x1e:
                case 0x1f:
                case 0x20:
                case 0x21:
                case 0x22:
                case 0x23:
                case 0x24:
                    diplomac::Break_Trade_(MOX::_PLAYER_NUM, _current_ambassador);
                    diplomac::Break_Research_(MOX::_PLAYER_NUM, _current_ambassador);
                    diplomac::Break_Tribute_(MOX::_PLAYER_NUM, _current_ambassador);
                    diplomac::Break_Treaties_(MOX::_PLAYER_NUM, _current_ambassador);
                    _response_message = 0x25;
                    Diplomacy_Display_Response_();
                    break;
                case 0x56:
                    diplomac::Break_Treaties_(MOX::_PLAYER_NUM, _current_ambassador);
                    _response_message = 0x57;
                    Diplomacy_Display_Response_();
                    break;
                case 0x68:
                    diplomac::Break_Treaties_(MOX::_PLAYER_NUM, _current_ambassador);
                    break;
                case 0x69:
                    diplomac::Change_Relations_(-0x32, _current_ambassador, MOX::_PLAYER_NUM, 0, 0, 0);
                    _response_message = 0x25;
                    Diplomacy_Display_Response_();
                    break;
                case 0x6a:
                    if (MOX::_player[_current_ambassador].opportunity_planet_num[MOX::_PLAYER_NUM] != -1) {
                        MOX::_player[_current_ambassador].sneak_attack_planet = MOX::_player[_current_ambassador].opportunity_planet_num[MOX::_PLAYER_NUM];
                        MOX::_player[_current_ambassador].sneak_attack_message = MOX::_player[_current_ambassador].diplomacy_demand_rejection_message[MOX::_PLAYER_NUM];
                    }
                    else {
                        diplomac::Declare_War_(MOX::_PLAYER_NUM, _current_ambassador);
                        _response_message = 0xb3;
                        Diplomacy_Display_Response_();
                    }
                    break;
                }
            }
        }

        sound::Fade_Stream_Down_(10);
        fields::Deactivate_Auto_Function_();
        MOX2::Put_Auto_Function_(&EVENTS::_jim_saved_autofunction);
        fields::Clear_Fields_();
        file_ani::Reset_File_Animation_Buffer_();
        RUSS::Release_All_Blocks_(MOX::_screen_seg);
        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Percent_(10);
        ERIC::Fast_Fade_Out_();
        JIM::_jim_current_stream = -1;
        JIM::Play_Background_Music_();
    }

    void __cdecl Draw_Diplomacy_Synch_Mode_() {
        video::Set_Page_Off_();

        if (MOX::_FULL_DRAW_SCREEN_FLAG == 1) {
            Setup_Back_Page_();
            Setup_Ambassador_Pic_(1);
        }

        if (_ambassador_option != 0 && MOX::_settings.animations_on == 1) {
            file_ani::Draw_File_Animation_Stencil_();
            if (file_ani::Get_File_Animation_Frame_() == 0) {
                file_ani::Set_File_Animation_Frame_(1);
            }
        }

        fonts::Set_Outline_Color_(0, 0xF8);
        fonts::Set_Font_Style_Outline_(4, &_diplomacy_normal_colors);

        if (_old_response_message != (int16_t)_response_message) {
            if (_response_message < 1000) {
                _old_response_message = (int16_t)_response_message;
                Get_Diplomacy_Statement_(_response_message, MOX::_PLAYER_NUM, _current_ambassador, _diplomacy_message);
            }
            else {
                _response_message = 0;
                _old_response_message = 0;
            }
        }

        int16_t max_height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(0x1D6, _diplomacy_message);
        int16_t y_pos = 0x1B8 - (max_height / 2);

        FMTPARA::Print_Formatted_Paragraph_(0x50, y_pos, 0x1D6, _diplomacy_message, 2);

        uint8_t c = static_cast<uint8_t>(max_height / 2);
        switch (MOX::_player[_current_ambassador].color) {
        case 0: c = 0xF3; break;
        case 1: c = 0xF0; break;
        case 2: c = 0xFD; break;
        case 3: c = 0xE3; break;
        case 4: c = 0xF5; break;
        case 5: c = 0xEB; break;
        case 6: c = 0xF4; break;
        case 7: c = 0xF1; break;
        }

        s_colors shadow_color;
        memset(&shadow_color, c, sizeof(s_colors));
        shadow_color.colors[0] = 0;

        fonts::Set_Outline_Color_(0, 0);
        fonts::Set_Font_Style_Shadow_Down_(4, &shadow_color);

        char local_str[120];

        if (MOX::_player[_current_ambassador].objectives == 100 /* 'd' */) {
            strcpy(local_str, _player_number_name_buffer);
            JIM::Decode_Text_Name_And_Stat_(local_str, _current_ambassador, _current_ambassador + 1, 0x78);
        }
        else {
            strcpy(local_str, _player_name_ambassador_buffer);
            JIM::Decode_Text_Race_(local_str, _current_ambassador, 0x78);
        }

        fonts::Print_Centered_(0x140, 0xA, local_str);

        if (MOX::_game_type != 0 && MOX::_game_type != 1) {
            if (MOX::_player[_current_ambassador].objectives == 100 /* 'd' */) {
                if (_receive_chat_message[0] != '\0') {
                    fonts::Set_Outline_Color_(0xF8, 0xF8);
                    fonts::Set_Font_Style_Shadow_Down_(4, &_diplomacy_normal_colors);

                    sprintf(local_str, s_s_sez__s_00552334, MOX::_player[_current_ambassador].name, _receive_chat_message);
                    fonts::Print_Centered_(0x140, 0x1D, local_str);
                }
            }
        }

        if (_diplomacy_current_music != -1) {
            JIM::Play_Streaming_Music_(_diplomacy_current_music + 100, -1, 1);
        }

        RUSS::Mox_Update_();

        if (_net_diplomacy_esc_pressed == 1) {
            fields::Exit_List_Field_();
        }

        fonts::Set_Outline_Color_(0xF8, 0xF8);
    }

    void __cdecl Setup_Back_Page_() {
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;

        int anim_index = (static_cast<uint8_t>(MOX::_player[_current_ambassador].race) * 2) + 13;
        file_ani::Open_File_Animation_(s_DIPLOMAT_LBX_00552350, anim_index);

        video::Set_Page_Off_();

        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);

        file_ani::Draw_File_Animation_();
        video::Copy_Off_To_Back_();
    }

    void __cdecl Draw_Diplomacy_Fade_In_Screen_() {
        video::Set_Page_Off_();
        file_ani::Draw_File_Animation_Stencil_();
        JIM::Play_Streaming_Music_(_diplomacy_current_music + 100, -1, 1);
    }

    void __cdecl Setup_Ambassador_Pic_(int16_t frame_selector) {
        uint8_t race = static_cast<uint8_t>(MOX::_player[_current_ambassador].race);

        file_ani::Open_File_Animation_(
            s_DIPLOMAT_LBX,
            race * 2 + 14
        );

        file_ani::Set_File_Animation_Frame_(frame_selector == 0 ? 1 : 0);

        if (MOX::_settings.animations_on == 0) {
            file_ani::Draw_File_Animation_Stencil_();
        }
    }

    void __cdecl Diplomacy_Set_Main_Options_(int16_t player_id, int16_t target_id, uint8_t* options) {
        int32_t tech_list_buffer[212];
        int16_t diff_tech_count;

        if (MOX::_player[player_id].treaty[target_id] < 4) {
            if (MOX::_player[target_id].objectives == 100 /* 'd' */) {
                _exchange_tech_count = 0;
                npcdiplo::Get_Differential_Tech_List_(target_id, player_id, 0, 0xC343CB00, _target_tech_list, tech_list_buffer, &diff_tech_count,
                    1);
                if (diff_tech_count > 0) {
                    npcdiplo::Get_Differential_Tech_List_(player_id, target_id, 0, 0xC343CB00, _target_tech_list, tech_list_buffer,
                        &_exchange_tech_count, 1);
                }
            }
            options[4] = 0;
            if (_exchange_tech_count > 0) {
                options[4] = 1;
            }
        }
        else {
            options[1] = 0;
            options[5] = 0;
            options[2] = 0;
            options[4] = 0;
        }

        options[1] = 0;
        if (MOX::_player[player_id].trade_treaty[target_id] == 1 ||
            MOX::_player[player_id].research_treaty[target_id] == 1 ||
            MOX::_player[player_id].tribute_treaty[target_id] > 0 ||
            MOX::_player[player_id].treaty[target_id] == 1 ||
            MOX::_player[player_id].treaty[target_id] == 2) {
            options[1] = 1;
        }

        if (diplomac::Valid_Treaty_Proposal_(player_id, target_id, 7) == 0 &&
            diplomac::Valid_Treaty_Proposal_(player_id, target_id, 8) == 0 &&
            diplomac::Valid_Treaty_Proposal_(player_id, target_id, 3) == 0 &&
            diplomac::Valid_Treaty_Proposal_(player_id, target_id, 1) == 0 &&
            diplomac::Valid_Treaty_Proposal_(player_id, target_id, 2) == 0) {
            options[0] = 0;
        }

        options[6] = 0;
        if (COLCALC::May_Surrender_Empire_(player_id, target_id) == 1) {
            options[6] = 1;
        }
    }

    int16_t __cdecl Diplomacy_Generic_Exchange_Tech_List_(int16_t tech_app_player, int16_t unused_2, char* list_title, uint8_t* tech_ids, int16_t num_techs, int8_t mode) {
        int16_t page_size = (mode == 1) ? 8 : 11;

        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Set_Input_Delay_(1);
        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);

        char str_prev[40];
        char str_next[40];
        char str_cancel[40];

        JIM::Get_Text_Message_(s_jimtext_lbx, 76, str_prev, 40);
        JIM::Get_Text_Message_(s_jimtext_lbx, 77, str_next, 40);
        JIM::Get_Text_Message_(s_jimtext_lbx, (mode == 1) ? 70 : 46, str_cancel, 40);

        int16_t start_idx = 0;
        int16_t selected_idx = -1;
        bool loop_done = false;

        while (!loop_done) {
            int16_t btn_prev = -1;
            int16_t btn_next = -1;

            uint8_t list_highlight[16];
            memset(list_highlight, 1, 15);

            strcpy(_diplomacy_list_choice[0], str_prev);

            int16_t item_values[16];
            item_values[0] = -1;

            int16_t has_prev = 0;
            if (start_idx != 0) {
                btn_prev = 0;
                has_prev = 1;
            }

            int16_t items_to_process;
            if (page_size + start_idx < num_techs) {
                items_to_process = page_size + has_prev;
                btn_next = items_to_process;
            }
            else {
                items_to_process = num_techs - start_idx;
            }

            if (items_to_process > 0) {
                for (int16_t i = 0; i < items_to_process; i++) {
                    int16_t target_idx = i + has_prev;
                    char* tech_name = TECH::Technology_Applications_Name_((uint8_t)tech_ids[start_idx + i], tech_app_player);
                    sprintf(_diplomacy_list_choice[target_idx], s_percent_s_0055236c, tech_name);
                    item_values[target_idx] = (uint8_t)tech_ids[start_idx + i];
                }
            }

            int16_t list_idx = items_to_process;

            if (btn_next != -1) {
                strcpy(_diplomacy_list_choice[list_idx], str_next);
                item_values[list_idx] = -1;
                list_idx++;
            }

            if (btn_prev == 0 && btn_next == -1) {
                list_idx++;
            }

            strcpy(_diplomacy_list_choice[list_idx], str_cancel);
            item_values[list_idx] = 0x265;
            _diplomacy_list_choice[list_idx + 1][0] = s_empty_string_00551100[0];

            fonts::Set_Font_LF_(4);
            Setup_Diplomacy_Help_(list_idx + 2, item_values);

            int8_t selection_buffer = 0;

            int16_t action = fields::Get_List_Field_(
                10, 118, 245, list_title,
                _diplomacy_list_choice[0], 50, &selection_buffer,
                list_highlight, 17, nullptr, 0, nullptr, 0, 0
            );

            if (action == -1 || action == list_idx) {
                selected_idx = -1;
                loop_done = true;
            }
            else if (action == btn_prev) {
                start_idx -= page_size;
            }
            else if (action == btn_next) {
                start_idx += page_size;
            }
            else {
                loop_done = true;
                if (btn_prev == -1) {
                    selected_idx = action + start_idx;
                }
                else {
                    selected_idx = start_idx - 1 + action;
                }
            }
        }

        return selected_idx;
    }

    int16_t __cdecl Get_Non_Player_Proposals_() {
        int16_t default_retval = 1;
        int16_t cancel_sentinel = -1000;
        uint8_t hotkey_flags[10];
        memset(hotkey_flags, 1, sizeof(hotkey_flags));

        int8_t list_pos = 0;
        char text_buffer[80];
        uint8_t tech_list[212];
        int32_t aiStack_350[212];
        int16_t sStack_488;
        int16_t result = -1;

        fields::Deactivate_Help_List_();
        JIM::Get_Text_Message_(s_jimtext_lbx, 0x4A, text_buffer, 80);

        switch (_response_message) {
        case 0x1c: case 0x1d: case 0x1e: case 0x1f:
        case 0x20: case 0x21: case 0x22: case 0x23:
        case 0x24: case 0x53: case 0x54: case 0x55:
        case 0x56: case 0x61: case 0x62: case 99:
        case 100: case 0x66: case 0x67: case 0x68:
        case 0x69: case 0x6a: case 0x7d:
            JIM::Get_Text_Message_(s_jimtext_lbx, 0x4F, _diplomacy_list_choice[0], 50);
            JIM::Get_Text_Message_(s_jimtext_lbx, 0x50, _diplomacy_list_choice[1], 50);
            _diplomacy_list_choice[2][0] = '\0';
            break;
        default:
            break;
        }

        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Set_Input_Delay_(1);

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);
        Setup_Full_Screen_Diplomacy_Help_(0x2b6);

        if (_response_message == 0x65) {
            default_retval = -1;
            cancel_sentinel = -1;

            npcdiplo::Get_Differential_Tech_List_(
                MOX::_PLAYER_NUM,
                _current_ambassador,
                0,
                MOX::_player[_current_ambassador].diplomacy_proposal_exchange_max_value[MOX::_PLAYER_NUM],
                tech_list,
                aiStack_350,
                &sStack_488,
                1
            );

            JIM::Get_Text_Message_(s_jimtext_lbx, 0x51, text_buffer, 80);

            result = Diplomacy_Generic_Exchange_Tech_List_(
                _current_ambassador,
                MOX::_PLAYER_NUM,
                text_buffer,
                tech_list,
                sStack_488,
                1
            );

            if (result != -1) {
                COLCALC::Player_Gets_Tech_App_(
                    &MOX::_player[MOX::_PLAYER_NUM],
                    tech_list[result]
                );
                COLCALC::Player_Gets_Tech_App_(
                    &MOX::_player[_current_ambassador],
                    MOX::_player[_current_ambassador].diplomacy_proposal_exchange_tech[MOX::_PLAYER_NUM]
                );
            }
        }
        else if (_response_message == 0x7d) {
            fields::Clear_Fields_();
            fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s_empty_string, 0);
            result = fields::Get_Input_();
            while (result == 0) {
                timer::Mark_Time_();
                Draw_Diplomacy_Synch_Mode_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(2);
                result = fields::Get_Input_();
            }
            result = 1;
        }
        else {
            result = fields::Get_List_Field_(
                10, 0x76, 0xF5, text_buffer, (char*)_diplomacy_list_choice, 50,
                &list_pos, hotkey_flags, 0x11, nullptr, 0, nullptr, 0, 0
            );
        }

        if (result == -1) {
            result = default_retval;
        }

        if (cancel_sentinel == result) {
            result = -1;
        }

        return result;
    }

    void __cdecl Accepted_Npc_Demand_Resolution_(int16_t player_id, int16_t ambassador_id) {
        int16_t relation_change = random::Random_(13) + 11;
        diplomac::Change_Relations_(relation_change, player_id, ambassador_id, 0, 0, 0);

        MOX::_player[player_id].threats[ambassador_id] = '\0';
        MOX::_player[ambassador_id].threats[player_id] = '\0';
    }

    void __cdecl Setup_Diplomacy_Help_(int16_t count, int16_t* help_ids) {
        fields::Deactivate_Help_List_();

        if (MOX::_game_type == 0 || MOX::_game_type == 1 ||
            MOX::_player[_current_ambassador].objectives != 100 /* 'd' */) {

            count--;
            int16_t font_h = fonts::Get_Font_Height_();
            int16_t step = font_h + fonts::Get_Font_Vertical_Spacing_();
            int16_t y = step + 0x76;

            for (int16_t i = 0; i < count; i++) {
                _jim_help_list[i].x1 = 10;
                _jim_help_list[i].y1 = y;
                _jim_help_list[i].x2 = 0xFF;
                _jim_help_list[i].y2 = y + fonts::Get_Font_Height_();
                _jim_help_list[i].help_id = help_ids[i];
                y += step;
            }

            _jim_help_list[count].x1 = 0;
            _jim_help_list[count].y1 = 0;
            _jim_help_list[count].x2 = 0x27F;
            _jim_help_list[count].y2 = 0x1DF;
            _jim_help_list[count].help_id = 0x2B6;

            fields::Set_Help_List_(_jim_help_list, count + 1);
        }
    }

    void __cdecl Diplomacy_Fade_In_() {
        Diplomacy_Mouse_Init_(_current_ambassador);
        Get_Diplomacy_Statement_(_response_message, MOX::_PLAYER_NUM, _current_ambassador, _diplomacy_message);
        Setup_Back_Page_();
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        if (_ambassador_option != 0) {
            uint16_t skip_anim = 0;
            fields::Assign_Auto_Function_(Draw_Diplomacy_Fade_In_Screen_, 2);
            file_ani::Set_File_Animation_Frame_(1);
            if (MOX::_settings.animations_on == 0) {
                skip_anim = 1;
            }
            for (int16_t i = 0; i < 38; i++) {
                if (skip_anim != 0) {
                    break;
                }
                timer::Mark_Time_();
                fields::Clear_Fields_();
                int16_t hidden_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s_empty_string_00551100, 0);
                int16_t input = fields::Get_Input_();
                if (hidden_field == input || key::Keyboard_Status_()) {
                    skip_anim = 1;
                }
                if (skip_anim == 0) {
                    Draw_Diplomacy_Fade_In_Screen_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(2);
                }
            }
            Setup_Ambassador_Pic_(skip_anim);
        }
    }

    void __cdecl Setup_Full_Screen_Diplomacy_Help_(int16_t help_id) {
        fields::Deactivate_Help_List_();

        if (MOX::_game_type == 0 ||
            MOX::_game_type == 1 ||
            MOX::_player[_current_ambassador].objectives != 100 /* 'd' */) {

            _jim_help_list[0].x2 = 0x27F;
            _jim_help_list[0].x1 = 0;
            _jim_help_list[0].y1 = 0;
            _jim_help_list[0].y2 = 0x1DF;
            _jim_help_list[0].help_id = help_id;

            fields::Set_Help_List_(_jim_help_list, 1);
        }
    }

    void __cdecl Diplomacy_Propose_Treaty_(int16_t player_idx, int16_t target_idx) {
        uint8_t valid_treaties[7];
        int8_t hotkeys = 0;

        valid_treaties[0] = 1;
        valid_treaties[1] = 1;
        valid_treaties[2] = 1;
        valid_treaties[3] = 1;
        valid_treaties[4] = 1;
        valid_treaties[5] = 1;
        valid_treaties[6] = 1;

        valid_treaties[0] = static_cast<uint8_t>(diplomac::Valid_Treaty_Proposal_(player_idx, target_idx, 7));
        valid_treaties[1] = static_cast<uint8_t>(diplomac::Valid_Treaty_Proposal_(player_idx, target_idx, 8));
        valid_treaties[2] = static_cast<uint8_t>(diplomac::Valid_Treaty_Proposal_(player_idx, target_idx, 3));
        valid_treaties[3] = static_cast<uint8_t>(diplomac::Valid_Treaty_Proposal_(player_idx, target_idx, 1));
        valid_treaties[4] = static_cast<uint8_t>(diplomac::Valid_Treaty_Proposal_(player_idx, target_idx, 2));

        fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
        fields::Set_Input_Delay_(1);

        for (int16_t i = 0; i < 7; ++i) {
            strcpy(_diplomacy_list_choice[i], _propose_treaty_diplomacy_list[i]);
        }

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);

        Setup_Diplomacy_Help_(7, _propose_treaty_help_entry);

        int16_t treaty_status = 0;

        int16_t choice = fields::Get_List_Field_(
            10, 0x76, 0xf5,
            _propose_treaty_title,
            reinterpret_cast<char*>(_diplomacy_list_choice),
            50,
            &hotkeys,
            valid_treaties,
            0x11,
            nullptr, 0, nullptr, 0, 0
        );

        switch (choice) {
        case 0:
            _old_response_message = -1;
            treaty_status = Diplomacy_Determine_Treaty_Proposal_(player_idx, target_idx, 7);
            diplomac::Adjust_Diplomat_Modifiers_(player_idx, target_idx);
            MOX::_player[player_idx].trade_modifier[target_idx] -= 20;
            if (treaty_status != 1) {
                return;
            }
            diplomac::Start_Trade_Treaty_(player_idx, target_idx);
            break;
        case 1:
            _old_response_message = -1;
            treaty_status = Diplomacy_Determine_Treaty_Proposal_(player_idx, target_idx, 8);
            diplomac::Adjust_Diplomat_Modifiers_(player_idx, target_idx);
            MOX::_player[player_idx].trade_modifier[target_idx] -= 20;
            if (treaty_status != 1) {
                return;
            }
            diplomac::Start_Research_Treaty_(player_idx, target_idx);
            break;
        case 2:
            _old_response_message = -1;
            treaty_status = Diplomacy_Determine_Treaty_Proposal_(player_idx, target_idx, 3);
            diplomac::Adjust_Diplomat_Modifiers_(player_idx, target_idx);
            MOX::_player[player_idx].peace_modifier[target_idx] -= 20;
            if (treaty_status != 1) {
                return;
            }
            diplomac::Declare_Peace_(player_idx, target_idx);
            break;
        case 3:
            _old_response_message = -1;
            treaty_status = Diplomacy_Determine_Treaty_Proposal_(player_idx, target_idx, 1);
            diplomac::Adjust_Diplomat_Modifiers_(player_idx, target_idx);
            MOX::_player[player_idx].treaty_modifier[target_idx] -= 20;
            if (treaty_status != 1) {
                return;
            }
            diplomac::Start_Treaty_(player_idx, target_idx, 1);
            break;
        case 4:
            _old_response_message = -1;
            treaty_status = Diplomacy_Determine_Treaty_Proposal_(player_idx, target_idx, 2);
            diplomac::Adjust_Diplomat_Modifiers_(player_idx, target_idx);
            MOX::_player[player_idx].treaty_modifier[target_idx] -= 20;
            if (treaty_status != 1) {
                return;
            }
            diplomac::Start_Treaty_(player_idx, target_idx, 2);
            break;
        default:
            break;
        }

        if (treaty_status == 1) {
            if (MOX::_player[target_idx].objectives != 100 /* 'd' */) {
                npcdiplo::Get_Exchange_Tech_List_(
                    player_idx,
                    target_idx,
                    &_exchange_tech_count,
                    _target_tech_list,
                    _exchange_tech_list
                );
            }
        }
    }

    void __cdecl Get_Main_Repulsive_Diplomacy_Choices_(int16_t param_1, int16_t param_2, int8_t param_3) {
        int16_t done = 0;

        while (true) {
            uint8_t flags[5];
            flags[0] = 1;
            flags[1] = 1;
            flags[2] = 1;
            flags[3] = 1;
            flags[4] = 1;

            if (MOX::_player[param_1].treaty[param_2] < 4) {
                flags[0] = 0;
            }
            else {
                flags[1] = 0;
            }

            flags[2] = 0;
            if (COLCALC::May_Surrender_Empire_(param_1, param_2)) {
                flags[2] = 1;
            }

            if (param_3 == 0 && diplomac::Find_Worst_Modifier_(param_1, param_2) < -99) {
                if (_old_response_message != _response_message) {
                    Diplomacy_Display_Response_();
                }
                _response_message = 0x82;
                Diplomacy_Display_Response_();
                done = 1;
            }
            else {
                if (_net_diplomacy_esc_pressed == 1) {
                    done = 1;
                }

                fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
                fields::Set_Input_Delay_(1);
                fonts::Set_Outline_Color_(0, 0xF8);
                fonts::Set_Font_Style_Outline_(4, &_diplomacy_normal_colors);
                fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
                fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
                fonts::Set_Font_LF_(4);

                int16_t list_size = 0;
                int16_t* help_entry = nullptr;

                if (param_3 == 1) {
                    for (int i = 0; i < 6; ++i) {
                        strcpy(_diplomacy_list_choice[i], _main_net_repulsive_diplomacy_list[i]);
                    }
                    help_entry = _main_net_repulsive_diplomacy_help_entry;
                    list_size = 6;
                }
                else {
                    for (int i = 0; i < 5; ++i) {
                        strcpy(_diplomacy_list_choice[i], _main_repulsive_diplomacy_list[i]);
                    }
                    help_entry = _main_repulsive_diplomacy_help_entry;
                    list_size = 5;
                }

                Setup_Diplomacy_Help_(list_size, help_entry);

                int8_t list_pos = 0;
                int16_t choice = fields::Get_List_Field_(
                    10, 0x76, 0xF5,
                    _main_diplomacy_title,
                    _diplomacy_list_choice[0],
                    0x32,
                    &list_pos,
                    flags,
                    0x11, nullptr, 0, nullptr, 0, 0
                );

                if (param_3 == 1 && choice == 3) {
                    choice = 5;
                }

                switch (choice) {
                case 0: {
                    *_diplomacy_message = s_empty_string_00551100[0];
                    _old_response_message = -1;
                    int16_t proposal = Diplomacy_Determine_Treaty_Proposal_(param_1, param_2, 3);
                    diplomac::Adjust_Diplomat_Modifiers_(param_1, param_2);
                    if (proposal == 1) {
                        diplomac::Declare_Peace_(param_1, param_2);
                    }
                    break;
                }
                case 1:
                    if (Diplomacy_Declare_War_Resolution_(param_1, param_2) == 1) {
                        done = 1;
                        Diplomacy_Display_Response_();
                        _net_diplomacy_esc_pressed = 1;
                    }
                    break;
                case 2:
                    if (Diplomacy_Surrender_(param_1, param_2) == 1) {
                        done = 1;
                        _net_diplomacy_esc_pressed = 1;
                    }
                    break;
                case 5:
                    Diplomacy_Main_Net_Chat_(param_2, param_1);
                    if (_net_diplomacy_esc_pressed == 1) {
                        done = 1;
                    }
                    break;
                case -1:
                case 3:
                case 4:
                    done = 1;
                    if (param_3 == 1) {
                        _net_diplomacy_message.sender_player_id = static_cast<uint8_t>(param_1);
                        _net_diplomacy_message.target_player_id = static_cast<uint8_t>(param_2);
                        _net_diplomacy_esc_pressed = 1;
                        _net_diplomacy_message.arg2 = -1;
                        _net_diplomacy_message.msg_type = 0x82;

                        NETMOX::Mox_Send_Message_(
                            MOX::_player[param_1].network_player_id, 2000,
                            (uint8_t*)&_net_diplomacy_message, 8
                        );
                        RUSS::Mox_Update_();
                    }
                    break;
                }
            }

            if (done != 0) {
                fields::Assign_Auto_Function_(Draw_Diplomacy_Synch_Mode_, 2);
                fields::Set_Input_Delay_(1);
                return;
            }
        }
    }

    void __cdecl Npc_Give_Gift_(int16_t player_idx, int16_t target_idx) {
        if (MOX::_player[player_idx].diplomacy_proposal_tribute_bribe[target_idx] != 0) {
            if (MOX::_player[player_idx].diplomacy_proposal_tribute_bribe[target_idx] == 1) {
                diplomac::Start_Tribute_Treaty_(player_idx, target_idx, 1);
            }
            else {
                diplomac::Start_Tribute_Treaty_(player_idx, target_idx, 2);
            }
            return;
        }

        if (MOX::_player[player_idx].diplomacy_proposal_tech_bribe1[target_idx] != 0) {
            COLCALC::Player_Gets_Tech_App_(
                &MOX::_player[target_idx],
                (int16_t)(uint8_t)MOX::_player[player_idx].diplomacy_proposal_tech_bribe1[target_idx]
            );
            return;
        }

        if (MOX::_player[player_idx].diplomacy_proposal_gold_bribe[target_idx] != 0) {
            MOX::_player[target_idx].bc += MOX::_player[player_idx].diplomacy_proposal_gold_bribe[target_idx];
            MOX::_player[player_idx].bc -= MOX::_player[player_idx].diplomacy_proposal_gold_bribe[target_idx];

            if (MOX::_player[player_idx].bc < 1) {
                MOX::_player[player_idx].bc = 0;
            }
            return;
        }

        if (MOX::_player[player_idx].diplomacy_proposal_system_bribe[target_idx] != -1) {
            if (COLCALC::May_Surrender_Star_(
                MOX::_player[player_idx].diplomacy_proposal_system_bribe[target_idx],
                player_idx,
                target_idx)) {
                COLCALC::Surrender_Star_(
                    MOX::_player[player_idx].diplomacy_proposal_system_bribe[target_idx],
                    player_idx,
                    target_idx
                );
            }
        }
    }
}