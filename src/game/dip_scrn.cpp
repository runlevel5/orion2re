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
        } else {
            sVar1 = diplomac::Check_Treaty_Proposal_(target_idx, proposer_idx, treaty_type, &_response_message, &local_4, &local_6, &local_8, &local_a, &local_c);
        }

        if ((sVar1 == 0) || (sVar1 == 3)) {
            success = (sVar1 != 0) ? 1 : 0;
        } else {
            if (MOX::_player[target_idx].objectives == 100 /* 'd' */) {
                sVar1 = Diplomacy_Human_Need_Better_Offer_(proposer_idx, target_idx);
            } else {
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
            } else {
                _response_message = 0x8A;
                success = 0;
            }
        }

        if (MOX::_player[target_idx].objectives == 100 /* 'd' */) {
            if (success == 1) {
                Get_Human_Form_Treaty_Message_(proposer_idx, target_idx, treaty_type, 3);
            } else {
                Get_Human_Form_Treaty_Message_(proposer_idx, target_idx, treaty_type, 0);
            }
        } else {
            npcdiplo::NPC_Message_Rejection_Check_(target_idx, proposer_idx, &_response_message);
        }

        if (initial_response == _response_message) {
            _response_message = 0x8A;
        }

        return success;
    }

    void __cdecl Diplomacy_Break_Treaty_(int16_t param_1, int16_t param_2) {
        uint8_t option_flags[7] = {0};

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

        int8_t list_pos[2] = {0, 0};
        int16_t choice = fields::Get_List_Field_(
            10, 118, 245,
            _break_treaty_title,
            (char*)_diplomacy_list_choice,
            50,
            list_pos,
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
                } else {
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
                } else {
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x26, _diplomacy_message, 0xFA);
                }
                break;
            case 2:
                if (msg_type != 3) {
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x29, _diplomacy_message, 0xFA);
                } else {
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x28, _diplomacy_message, 0xFA);
                }
                break;
            case 7:
                if (msg_type != 3) {
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x21, _diplomacy_message, 0xFA);
                } else {
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x20, _diplomacy_message, 0xFA);
                }
                break;
            case 8:
                if (msg_type != 3) {
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x23, _diplomacy_message, 0xFA);
                } else {
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x22, _diplomacy_message, 0xFA);
                }
                break;
            case 9:
            case 10:
                if (msg_type != 3) {
                    JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 0x25, _diplomacy_message, 0xFA);
                } else {
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
                    } else if (treaty_type == 3) {
                        if (MOX::_player[i].treaty[player2] == 4 || MOX::_player[i].treaty[player2] == 5) {
                            add_to_list = true;
                        }
                    } else if (treaty_type == 5) {
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

    int16_t __cdecl Diplomacy_Generic_List_(int16_t item_count, uint8_t* list_strings, int8_t add_cancel, uint8_t* background) {
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
        } else {
            cancel_idx = item_count - 1;
            _diplomacy_list_choice[item_count][0] = MOX::s__00551100[0];
        }

        fonts::Set_Font_Style_(4, &_diplomacy_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&_diplomacy_highlight_colors);
        fonts::Set_Font_Special_Colors_(&_diplomacy_special_colors);
        fonts::Set_Font_LF_(4);

        int8_t selected_idx[2] = { 0 };

        int16_t result = fields::Get_List_Field_(
            10, 0x76, 0xf5, (char*)background, (char*)_diplomacy_list_choice, 50,
            selected_idx, item_states, 0x11, nullptr, 0, nullptr, 0, 0
        );

        if (result == -1 || result == cancel_idx) {
            result = -1;
        }

        return result;
    }

    void __cdecl Diplomacy_Offer_Gift_(int16_t player_idx, int16_t target_idx) {
        uint8_t options[16] = {0};
        int8_t title[2];
        int16_t local_446 = 0;
        uint8_t local_424[212];
        int32_t local_350[212];

        options[0] = 1;
        options[1] = 1;

        if (MOX::_player[target_idx].tribute_treaty[player_idx] > 0) {
            options[0] = 0;
            options[1] = 0;
        } else {
            int16_t player_tribute = MOX::_player[player_idx].tribute_treaty[target_idx];
            if (player_tribute > 0) {
                options[0] = 0;
                options[1] = 0;
            } else if (player_tribute == 1) {
                options[0] = 0;
            } else if (player_tribute == 2) {
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

        title[0] = '\0';
        title[1] = '\0';

        int16_t choice = fields::Get_List_Field_(
            10, 0x76, 0xf5,
            _offer_title,
            (char*)_diplomacy_list_choice,
            0x32,
            title,
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
                } else {
                    Get_Human_Gift_Response_(player_idx, target_idx, 2, 1);
                }
                break;

            case 1:
                _old_response_message = -1;
                diplomac::Start_Tribute_Treaty_(player_idx, target_idx, 2);
                if (MOX::_player[target_idx].objectives != 100) {
                    diplomac::Get_Gift_Response_(player_idx, target_idx, 2, 2, &_response_message);
                    npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
                } else {
                    Get_Human_Gift_Response_(player_idx, target_idx, 2, 2);
                }
                break;

            case 2:
            {
                int16_t tech_idx = Diplomacy_Generic_Exchange_Tech_List_(player_idx, target_idx, _offer_title, (int8_t*)local_424, local_446, 0);
                if (tech_idx == -1) {
                    *_diplomacy_message = MOX::s__00551100[0];
                } else {
                    uint8_t tech_app = local_424[tech_idx];
                    _old_response_message = -1;
                    COLCALC::Player_Gets_Tech_App_(&MOX::_player[target_idx], tech_app);
                    
                    if (MOX::_player[target_idx].objectives != 100) {
                        diplomac::Get_Gift_Response_(player_idx, target_idx, 3, tech_app, &_response_message);
                        npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
                        npcdiplo::Get_Exchange_Tech_List_(player_idx, target_idx, &_exchange_tech_count, _target_tech_list, _exchange_tech_list);
                    } else {
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
                } else {
                    _old_response_message = -1;
                    if (MOX::_player[target_idx].objectives != 100) {
                        diplomac::Get_Gift_Response_(player_idx, target_idx, 4, system_idx, &_response_message);
                        npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
                    } else {
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

        _diplomacy_system_star_background_seg = (uint8_t*)farload::Far_Reload_Next_(s_dipstars_lbx_00552374, 0, MOX::_screen_seg);
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
        } else {
            new_music = _diplomacy_good_music;
        }

        if (new_music != _diplomacy_current_music) {
            _diplomacy_current_music = new_music;
        }
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

        int8_t current_idx[2] = { 0 };
        int16_t result = fields::Get_List_Field_(
            10, 118, 245, text_buffer, _diplomacy_list_choice[0], 50,
            current_idx, selectable, 17, nullptr, 0, nullptr, 0, 0
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
        } else {
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
        Setup_Full_Screen_Diplomacy_Help_(0x267);

        uint8_t selection_flags[15];
        memset(selection_flags, 1, 15);

        for (int i = 0; i < 15; ++i) {
            strcpy(_diplomacy_list_choice[i], s__00551100);
        }

        int current_bc = MOX::_player[player_idx].bc;
        if (current_bc > 32000) {
            current_bc = 32000;
        }

        int16_t rounded_bc = (int16_t)(current_bc / 20) * 20;
        int16_t bc_options[5];
        int8_t choice_count;

        if (rounded_bc < 100) {
            choice_count = rounded_bc / 20;
            bc_options[0] = 20;
            bc_options[1] = 40;
            bc_options[2] = 60;
            bc_options[3] = 80;
            bc_options[4] = 100;
        } else {
            choice_count = 5;
            bc_options[0] = (int16_t)((rounded_bc / 5) / 20) * 20;
            bc_options[1] = (int16_t)((rounded_bc * 2 / 5) / 20) * 20;
            bc_options[2] = (int16_t)((rounded_bc * 3 / 5) / 20) * 20;
            bc_options[3] = (int16_t)((rounded_bc * 4 / 5) / 20) * 20;
            bc_options[4] = (int16_t)(rounded_bc / 20) * 20;
        }

        for (int i = 0; i < (int)choice_count; ++i) {
            _sprintf(_diplomacy_list_choice[i], s__d_BC_00552384, (int)bc_options[i]);
            if (MOX::_player[player_idx].bc < (int)bc_options[i]) {
                selection_flags[i] = 0;
            }
        }

        JIM::Get_Text_Message_(s_jimtext_lbx_00552360, 0x2E, _diplomacy_list_choice[choice_count], 0x28);
        
        char title_text[40];
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 8, title_text, 0x28);

        int16_t selection = fields::Get_List_Field_(
            10, 0x76, 0xF5, title_text, _diplomacy_list_choice[0], 0x32,
            &choice_count, selection_flags, 0x11, 0, 0, 0, 0, 0
        );

        if (selection >= 0 && selection < choice_count) {
            result_bc = bc_options[selection];
            if (do_transaction == 1) {
                _old_response_message = -1;
                MOX::_player[player_idx].bc -= (int)result_bc;
                MOX::_player[target_idx].bc += (int)result_bc;
                
                if (MOX::_player[target_idx].objectives == 100 /* 'd' */) {
                    Get_Human_Gift_Response_(player_idx, target_idx, 1, result_bc);
                } else {
                    diplomac::Get_Gift_Response_(player_idx, target_idx, 1, result_bc, &_response_message);
                    if ((int)(MOX::_stardate * 5 - 175000) <= (int)result_bc) {
                        npcdiplo::NPC_Proposal_Rejection_Accept_(target_idx, player_idx, 1);
                    }
                }
            }
        }

        return result_bc;
    }

    void __cdecl Diplomacy_Load_New_Ambassador_(int16_t player_idx) {
        mouse::Set_Mouse_List_(&DIP_SCRN::_diplomacy_blank_mouse, 1);
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Perc(10);
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
                JIM::Decode_Text_Name_And_Stat_(_diplomacy_message, (uint8_t)player_idx, (uint8_t)value, 250);
                break;

            case 2:
                JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 15 /* 0x0F */, _diplomacy_message, 250);
                if (value == 1) {
                    JIM::Decode_Text_Name_And_Stat_(_diplomacy_message, (uint8_t)player_idx, 5, 250);
                } else {
                    JIM::Decode_Text_Name_And_Stat_(_diplomacy_message, (uint8_t)player_idx, 10, 250);
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
        char buffer1[40];
        char buffer2[80];
        int16_t result = 0;

        Setup_Full_Screen_Diplomacy_Help_(0x266);

        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 59, buffer1, 40);
        JIM::Get_Text_Message_(s_jimtext2_lbx_00552340, 60, buffer2, 80);

        if (Diplomacy_Generic_List_(1, (uint8_t*)&buffer1[12], 1, (uint8_t*)buffer2) != -1) {
            result = 1;

            if (MOX::_player[target_id].objectives == 'd' /* 0x64 */) {
                if (MOX::_game_type == 1) {
                    Diplomacy_Load_New_Ambassador_(sender_id);
                    Get_Human_Declare_War_Message_(sender_id);
                    Diplomacy_Display_Response_();
                    Diplomacy_Load_New_Ambassador_(target_id);
                } else {
                    _net_diplomacy_message.arg2 = (int16_t)_response_message;
                    _net_diplomacy_message.sender_player_id = (uint8_t)sender_id;
                    _net_diplomacy_message.target_player_id = (uint8_t)target_id;
                    _net_diplomacy_message.arg1 = 0;
                    _net_diplomacy_message.msg_type = 1002 /* 0x3ea */;

                    NETMOX::Mox_Send_Message_(MOX::_player[target_id].network_player_id, 2000, &_net_diplomacy_message, 8);
                    RUSS::Mox_Update_();
                }
                _response_message = 160 /* 0xa0 */;
            } else {
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

        if (DIP_SCRN::Diplomacy_Generic_List_(1, (uint8_t*)&msg6_buffer[12], 1, (uint8_t*)msg7_buffer) != -1) {
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
            *reinterpret_cast<char*>(0x00572C18) = *DIP_SCRN::s__00551100;

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
            DIP_SCRN::Diplomacy_Chat_(param_1, param_2);

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
            DIP_SCRN::Diplomacy_Listen_Mode_(target_player_id, player_id);
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
        char choice1_text[40];
        char choice2_text[40];
        char choice3_text[40];
        char title_text[40];
        char* choices[3];
        int16_t num_choices = 2;
        int16_t selected_idx;
        int16_t result = 0;

        choices[0] = choice1_text;
        choices[1] = choice2_text;
        choices[2] = choice3_text;

        JIM::Get_Text_Message_(JIM::jimtext_lbx, 0x4F, choice1_text, 40);
        JIM::Get_Text_Message_(JIM::jimtext_lbx, 0x50, choice2_text, 40);

        choice3_text[0] = DIP_SCRN::s_empty[0];

        if (DIP_SCRN::Check_Better_Offer_Available_(player_a, player_b)) {
            num_choices = 3;
            JIM::Get_Text_Message_(JIM::jimtext_lbx, 0x52, choice3_text, 40);
        }

        DIP_SCRN::_response_message = 0;
        DIP_SCRN::_old_response_message = 0;

        DIP_SCRN::Get_Human_Propose_Treaty_Message_(treaty_type);
        fields::Deactivate_Help_List_();

        JIM::Get_Text_Message_(JIM::jimtext_lbx, 0x4A, title_text, 40);

        selected_idx = DIP_SCRN::Diplomacy_Generic_List_(num_choices, (uint8_t*)choice1_text, 0, (uint8_t*)title_text);

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
}