#include "pch.h"

namespace MULTPLAY {

    void __cdecl Draw_Modem_Setup_Screen_(void) {
        char buffer[128];
        s_colors local_colors;
        int16_t x = MOX::_mp_screen->screen_x;
        int16_t y = MOX::_mp_screen->screen_y;

        video::Set_Page_Off_();

        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
        animate::Draw_(0, 0, MOX::_mp_screen->pic);

        MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x20, MOX::_global_cache_seg);
        animate::Draw_(x, y, MOX::_mp_screen->bg_seg);

        _memcpy(MULTPLAY::_cur_pal_colors, MULTPLAY::_green_pal_colors, 64);
        MOX::_mox_font_colors_offset = 0x96;

        s_colors* font_ptr = MISC::Font_Colors2_(4, 0x92, 0x98);
        local_colors = *font_ptr;
        fonts::Set_Font_Style_(4, &local_colors);

        _sprintf(buffer, s_COM_PORT_FMT_v2, NETMOX::_net.modem_settings.com_port_index + 1);
        fonts::Print_(x + 130, y + 63, buffer);

        fonts::Set_Font_Style_(4, &local_colors);
        _sprintf(buffer, s_BAUD_RATE_FMT, NETMOX::_net.modem_settings.baud_rate);
        fonts::Print_(x + 300, y + 63, buffer);

        int16_t msg_id = (NETMOX::_net.modem_initialized != 0) ? 0xf7 /* HESTR_0F7_TONE */ : 0xf8 /* HESTR_0F8_PULSE */;
        const char* msg_str = HAROLD::H_Message_(msg_id);
        _sprintf(buffer, MOX::s_fmt_s, msg_str);

        MISC::Set_Fitted_Font_Style_(buffer, &local_colors, 3, 2, 0, 0x4d, 0);
        int16_t font_height = MISC::Get_Font_Style_();
        fonts::Print_Centered_(x + 339, y - font_height + 163, buffer);

        fonts::Set_Font_Style_(4, &local_colors);

        if (MOX::_mp_screen->flag_modem_disabled == 1) {
            MULTPLAY::Print_Modem_Strings_();

            int16_t bx_offset = x + 143;
            int16_t current_y = y + 81;

            MOX::_mp_screen->btn_accept_off = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 38, MOX::_global_cache_seg);
            animate::Draw_(bx_offset, current_y, MOX::_mp_screen->btn_accept_off);

            current_y += 12;

            for (int i = 0; i < 8; ++i) {
                if (MOX::_mp_screen->game_list_field_ids[i] == MOX::_mp_screen->last_scan_id) {
                    s_colors* sel_font = MISC::Font_Colors2_(3, 0x92, 0x9a);
                    local_colors = *sel_font;
                    fonts::Set_Font_Style_(3, &local_colors);

                    MOX::_mp_screen->btn_accept_on = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 39, MOX::_global_cache_seg);
                    animate::Draw_(x + 148, current_y, MOX::_mp_screen->btn_accept_on);
                }
                else {
                    s_colors* norm_font = MISC::Font_Colors2_(3, 0x92, 0x97);
                    local_colors = *norm_font;
                    fonts::Set_Font_Style_(3, &local_colors);
                }

                _sprintf(buffer, s_COM_PORT_FMT_v2, i + 1);
                fonts::Print_(x + 170, current_y + 3, buffer);
                current_y += 15;
            }
        }
        else if (MOX::_mp_screen->flag_modem_disabled == 2) {
            MULTPLAY::Print_Modem_Strings_();

            int16_t bx_offset = x + 298;
            int16_t current_y = y + 81;

            MOX::_mp_screen->btn_accept_off = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 38, MOX::_global_cache_seg);
            animate::Draw_(bx_offset, current_y, MOX::_mp_screen->btn_accept_off);

            current_y += 12;

            for (int i = 0; i < 8; ++i) {
                if (MOX::_mp_screen->game_list_field_ids[i] == MOX::_mp_screen->last_scan_id) {
                    s_colors* sel_font = MISC::Font_Colors2_(3, 0x92, 0x9a);
                    local_colors = *sel_font;
                    fonts::Set_Font_Style_(3, &local_colors);

                    MOX::_mp_screen->btn_accept_on = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 39, MOX::_global_cache_seg);
                    animate::Draw_(x + 303, current_y, MOX::_mp_screen->btn_accept_on);
                }
                else {
                    s_colors* norm_font = MISC::Font_Colors2_(3, 0x92, 0x97);
                    local_colors = *norm_font;
                    fonts::Set_Font_Style_(3, &local_colors);
                }

                fonts::Print_Right_(x + 366, current_y + 3, MULTPLAY::_baud_rate_strings[i]);
                current_y += 15;
            }
        }
    }

    void __cdecl Load_Net_Next_Turn_Screen_(char fade_in) {
        int16_t top_h;
        int16_t mid_h;
        int16_t chat_bg_h;
        int16_t top_w;
        uint8_t fade_palette[49];

        RUSS::Mox_Update_();
        MULTPLAY::_global_chat_string[0] = '\0';
        MOX::_mp_screen = &MULTPLAY::_g_mp_screen;

        memset(MOX::_mp_screen, 0, sizeof(s_mp_screen));

        // Signature copy: Evan Axelrad (12 bytes + null terminator)
        _memcpy(MOX::_mp_screen->signature, s_Evan_Axelrad_0055aa48, 13);

        MOX::_mp_screen->anim_turn_screen_top = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x2A /* 42 */, MOX::_global_cache_seg);
        top_w = animate::Get_Width_(MOX::_mp_screen->anim_turn_screen_top);
        top_h = animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_top);
        MOX::_mp_screen->screen_x = (int16_t)((640 - top_w) / 2);

        MOX::_mp_screen->anim_turn_screen_mid = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x2B /* 43 */, MOX::_global_cache_seg);
        mid_h = animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_mid);

        MOX::_mp_screen->anim_chat_msg_bg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x28 /* 40 */, MOX::_global_cache_seg);
        chat_bg_h = animate::Get_Height_(MOX::_mp_screen->anim_chat_msg_bg);

        MOX::_mp_screen->anim_unk_3 = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x2D /* 45 */, MOX::_global_cache_seg);

        MOX::_mp_screen->num_remaining_humans = HAROLD::Get_N_Remaining_Humans_();
        MOX::_mp_screen->needs_full_redraw = 1;

        MULTPLAY::Get_Net_Next_Turn_Player_Colors_();

        MOX::_mp_screen->player_field_ids[2] = (int16_t)(top_h + mid_h);

        int32_t screen_y_calc = (480 - (int16_t)(top_h + mid_h + chat_bg_h)) / 2;
        if (screen_y_calc < 1) {
            screen_y_calc = 0;
        }
        MOX::_mp_screen->screen_y = (int16_t)screen_y_calc;

        MOX::_mp_screen->subscreen_mode = 2;
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Net_Next_Turn_Screen_, 1);
        MOX::_FULL_DRAW_SCREEN_FLAG = 1;

        fields::Clear_Fields_();

        if (fade_in != '\0') {
            MISC::Fade_Into_Screen_(MULTPLAY::Draw_Net_Next_Turn_Screen_, fade_palette, 0);
        }

        MULTPLAY::Add_Net_Next_Turn_Fields_();
    }

    void __cdecl Chat_Box_Input_Loop_() {
        int16_t input_id;

        input_id = fields::Get_Input_();

        if (MOX::_mp_screen->bg_height == 0) {
            fields::_active_input_field_number = MOX::_mp_screen->player_field_ids[0];
            fields::_continuous_string = 0;
            fields::_cursor_position = 0;
            fields::_input_field_active = 1;
            fields::_active_input_field_first = 1;
            fields::_cursor_on = 0;
            MULTPLAY::_global_chat_string[0] = '\0';
            MOX::_mp_screen->bg_height = 1;
        }

        if (MOX::_mp_screen->player_field_ids[0] == input_id || MOX::_mp_screen->player_field_ids[1] == input_id) {
            if (MULTPLAY::_global_chat_string[0] != '\0') {
                CHAT::Send_Chat_Msg_(MULTPLAY::_global_chat_string);
                fields::_cursor_position = 0;
                fields::_cursor_on = 0;
                fields::_continuous_string = 0;
                MULTPLAY::_global_chat_string[0] = '\0';
            }
            fields::_input_field_active = 1;
            fields::_active_input_field_first = 1;
            fields::_active_input_field_number = MOX::_mp_screen->player_field_ids[0];
        }

        video::Set_Page_Off_();
        MULTPLAY::Draw_Net_Next_Turn_Screen_();
        RUSS::Mox_Sync_Update_();
    }

    void __cdecl Reload_Initializing_Net_Info_() {
        MULTPLAY::_allow_chat_mode = 0;
        MULTPLAY::Reload_Generic_Net_Info_(25);
    }

    void __cdecl Reload_Join_Net_Screen_() {
        MULTPLAY::_allow_chat_mode = 1;
        MULTPLAY::Reload_Generic_Net_Info_(0x17);
    }

    void __cdecl Draw_MP_Setup_Screen_() {
        s_mp_screen* screen = MOX::_mp_screen;

        screen->res_setup_bg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 1, MOX::_global_cache_seg);

        animate::Draw_(screen->screen_x, screen->screen_y, screen->res_setup_bg);

        animate::Set_Animation_Frame_(screen->anim_ten, screen->flag_ten_disabled);
        animate::Draw_(screen->screen_x + 0x3B, screen->screen_y + 0x5B, screen->anim_ten);

        animate::Set_Animation_Frame_(screen->anim_modem, screen->flag_modem_disabled);
        animate::Draw_(screen->screen_x + 0x3B, screen->screen_y + 0x7A, screen->anim_modem);

        animate::Set_Animation_Frame_(screen->anim_serial, screen->flag_serial_type);
        animate::Draw_(screen->screen_x + 0x3B, screen->screen_y + 0x9B, screen->anim_serial);

        animate::Set_Animation_Frame_(screen->anim_network, screen->flag_network_disabled);
        animate::Draw_(screen->screen_x + 0x3B, screen->screen_y + 0xBB, screen->anim_network);

        int16_t sx = screen->screen_x;
        int16_t sy = screen->screen_y;

        if (screen->field_id_next == -1000) {
            animate::Set_Animation_Frame_(screen->anim_btn_next, 2);
            animate::Draw_(sx + 0x10D, sy + 0x5B, screen->anim_btn_next);
        }

        if (screen->field_id_accept == -1000) {
            animate::Set_Animation_Frame_(screen->anim_btn_accept, 2);
            animate::Draw_(sx + 0x10D, sy + 0x7A, screen->anim_btn_accept);
        }

        if (screen->field_id_back == -1000) {
            animate::Set_Animation_Frame_(screen->anim_btn_back, 2);
            animate::Draw_(sx + 0x10D, sy + 0x9B, screen->anim_btn_back);
        }

        if (screen->field_id_setup == -1000) {
            animate::Set_Animation_Frame_(screen->anim_btn_setup, 2);
            animate::Draw_(sx + 0x10D, sy + 0xBB, screen->anim_btn_setup);
        }
    }

    void __cdecl Load_MP_Setup_Pictures_() {
        RUSS::Mox_Update_();

        MOX::_mp_screen = &MULTPLAY::_g_mp_screen;
        memset(&MULTPLAY::_g_mp_screen, 0, sizeof(s_mp_screen));

        MOX::_mp_screen->needs_full_redraw = 1;
        strcpy(MOX::_mp_screen->signature, s_Evan_Axelrad_0055aa48);

        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
        MOX::_mp_screen->res_setup_bg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 1, MOX::_global_cache_seg);

        MOX::_mp_screen->screen_x = (640 - animate::Get_Width_(MOX::_mp_screen->res_setup_bg)) / 2;
        MOX::_mp_screen->screen_y = (480 - animate::Get_Height_(MOX::_mp_screen->res_setup_bg)) / 2;

        MOX::_mp_screen->anim_ten = (s_animation_header*)farload::Far_Reload_(s_MULTIGM_LBX_00558d04, 2, MOX::_screen_seg);
        MOX::_mp_screen->anim_modem = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 3, MOX::_screen_seg);
        MOX::_mp_screen->anim_serial = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 4, MOX::_screen_seg);
        MOX::_mp_screen->anim_network = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 5, MOX::_screen_seg);
        MOX::_mp_screen->anim_btn_cancel = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 6, MOX::_screen_seg);
        MOX::_mp_screen->anim_btn_next = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 7, MOX::_screen_seg);
        MOX::_mp_screen->anim_btn_accept = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 8, MOX::_screen_seg);
        MOX::_mp_screen->anim_btn_back = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 9, MOX::_screen_seg);
        MOX::_mp_screen->anim_btn_setup = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 10, MOX::_screen_seg);
        MOX::_mp_screen->anim_btn_help = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 256, MOX::_screen_seg);

        MULTPLAY::Copy_Modem_Defaults_To_Net_Struct_();
    }

    void __cdecl Set_Default_Modem_Settings_(void) {
        // Note: com_port_index is set to 1 initially but overwritten to 0 at the end of the function.
        // Both assignments are kept per restoration rules.
        MOX::_settings.modem_settings.com_port_index = 1;
        MOX::_settings.modem_settings.serial_cfg_ext = 0;
        MOX::_settings.modem_settings.baud_rate = 19200 /* 0x4B00 */;
        MOX::_settings.modem_settings.parity = 'E' /* 0x45 */;
        MOX::_settings.modem_settings.data_bits = 7;

        strcpy(MOX::_settings.modem_settings.cmd_reset, MOX::s_ATZ_0055AA7C);
        strcpy(MOX::_settings.modem_settings.cmd_init, MOX::s_AT_K0_M1_D2_0055AA6C);
        strcpy(MOX::_settings.modem_settings.cmd_hangup, MOX::s_AT_H0_S0_0_0055AA60);
        strcpy(MOX::_settings.modem_settings.dial_number, MOX::s_5551212_0055AA58);

        MOX::_settings.modem_initialized = 1;
        MOX::_settings.modem_settings.com_port_index = 0;
    }

    int16_t __cdecl Warning_Box_Has_Opponent_Pressed_() {
        const char* message = HAROLD::H_Message_(0x1c /* HESTR_01C_HAS_YOUR_OPPONENT_PRES */);
        return GENDRAW::Warning_Box_(message);
    }

    void __cdecl Multi_Player_Screen_() {
        void* saved_auto_func;
        int8_t valid_saves[12];
        char msg_header[300];
        char msg_body[752];
        int16_t input_field;
        bool done = false;

        MOX::_temp_string[0] = '\0';

        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        MOX2::Get_Auto_Function_(&saved_auto_func);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);

        fields::Clear_Fields_();
        Load_MP_Setup_Pictures_();
        ERIC::Fast_Fade_Out_();

        video::Load_Palette_(0xB /* LBX_PALETTE_ENTRY */, 0, 0xFF);
        palette::Set_Palette_Changes_(0, 0xFF);
        palette::Refresh_Palette_();
        palstore::Darken_Palette_(100);

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        EVANHELP::Set_Multi_Player_Setup_Screen_Help_List_();

        video::Set_Page_Off_();
        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);

        animate::Draw_(0, 0, MOX::_mp_screen->pic);
        palette::Clear_Palette_Changes_(0, 0xFF);
        MOX::Mox_Sync_Update_();
        ERIC::Fast_Fade_In_();

        video::Set_Page_Back_();
        animate::Draw_(0, 0, MOX::_mp_screen->pic);
        video::Copy_Back_To_Off_();

        fields::Assign_Auto_Function_(Draw_MP_Setup_Screen_, 1);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        Add_MP_Setup_Screen_Fields_();

        while (true) {
            timer::Mark_Time_();
            input_field = fields::Get_Input_();

            if (input_field == MOX::_mp_screen->field_id_ten && MOX::_mp_screen->flag_ten_disabled == 0) {
                MOX::_settings.multi_player_game_type = 1;
            }
            else if (input_field == MOX::_mp_screen->field_id_modem && MOX::_mp_screen->flag_modem_disabled == 0) {
                MOX::_settings.multi_player_game_type = 2;
            }
            else if (input_field == MOX::_mp_screen->field_id_serial && MOX::_mp_screen->flag_serial_type == 0) {
                MOX::_settings.multi_player_game_type = 3;
            }
            else if (input_field == MOX::_mp_screen->field_id_network && MOX::_mp_screen->flag_network_disabled == 0) {
                MOX::_settings.multi_player_game_type = 4;
            }
            else if (input_field == MOX::_mp_screen->field_id_cancel) {
                MOX::_current_screen = 10;
                done = true;
            }
            else if (input_field == MOX::_mp_screen->field_id_help) {
                strcpy(msg_header, HAROLD::H_Message_(0x1D /* HESTR_01D_PLAY_MASTER_OF_ORION_I */));
                strcpy(msg_body, HAROLD::H_Message_(0x1E /* HESTR_01E_COMBATTING_COMPUTERIZE */));
                strcat(msg_body, HAROLD::H_Message_(0x1F /* HESTR_01F_THIS_WILL_GUIDE_YOU_TH */));
                TEXTBOX::Text_Box_(msg_header, msg_body);
            }

            if (input_field == MOX::_mp_screen->field_id_setup) {
                if (MOX::_mp_screen->flag_serial_type != 0) {
                    video::Set_Page_Off_();
                    Draw_MP_Setup_Screen_();
                    MOX::Mox_Sync_Update_();
                    MOX::_settings.multi_player_game_type = 3;
                    NullModem_Setup_Screen_();
                    Load_MP_Setup_Pictures_();
                    fields::Assign_Auto_Function_(Draw_MP_Setup_Screen_, 1);
                    fields::Clear_Fields_();
                    Add_MP_Setup_Screen_Fields_();
                    EVANHELP::Set_Multi_Player_Setup_Screen_Help_List_();
                }
                else if (MOX::_mp_screen->flag_modem_disabled != 0) {
                    video::Set_Page_Off_();
                    Draw_MP_Setup_Screen_();
                    MOX::Mox_Sync_Update_();
                    MOX::_settings.multi_player_game_type = 2;
                    Modem_Setup_Screen_();
                    Load_MP_Setup_Pictures_();
                    fields::Assign_Auto_Function_(Draw_MP_Setup_Screen_, 1);
                    fields::Clear_Fields_();
                    Add_MP_Setup_Screen_Fields_();
                    EVANHELP::Set_Multi_Player_Setup_Screen_Help_List_();
                }
            }
            else if (input_field == MOX::_mp_screen->field_id_accept) {
                bool proceed = true;
                MOX::_current_screen = 0xF;
                if (Get_Valid_MP_Save_Games_(valid_saves) != 0) {
                    if (MOX::_settings.multi_player_game_type == 3) {
                        if (Warning_Box_Has_Opponent_Pressed_() == 0) {
                            proceed = false;
                        }
                    }

                    if (proceed) {
                        MOX::_current_screen = 0x29 /* SCREEN_LOAD_NET */;
                        FILEDEF::Save_Game_Settings_();
                        fields::Clear_Fields_();
                        int16_t result = Load_Multi_Player_Game_Screen_();
                        if (result <= -1 || result >= 10) {
                            Load_MP_Setup_Pictures_();
                            MOX::_current_screen = 0xF /* SCREEN_MULTI_PLAYER */;
                        }
                        else if (MOX::_game_type == 1) {
                            MOX::_current_screen = 0x11 /* SCREEN_HOTSEAT_SELECT_PLAYER */;
                        }
                        else {
                            MOX::_current_screen = 0x29 /* SCREEN_LOAD_NET */;
                        }
                        done = true;
                    }
                }
            }
            else if (input_field == MOX::_mp_screen->field_id_next) {
                if (MOX::_settings.multi_player_game_type == 3) {
                    if (Warning_Box_Has_Opponent_Pressed_() != 0) {
                        MOX::_current_screen = 0xD;
                        done = true;
                    }
                }
                else {
                    if (MOX::_settings.net_game_name[0] == '\0') {
                        _memcpy(MOX::_settings.net_game_name, s_Moo_II_0055aa88, 8);
                        MOX::_settings.net_game_name[6] = (char)((abs(_clock()) & 0x1F) + 'a');
                    }
                    MOX::_current_screen = 0xD;
                    done = true;
                }
            }
            else if (input_field == MOX::_mp_screen->field_id_back) {
                MOX::_current_screen = 0x16;
                done = true;
            }

            if (!done) {
                Add_MP_Setup_Screen_Fields_();
                video::Set_Page_Off_();
                Draw_MP_Setup_Screen_();
                MOX::Mox_Sync_Update_();
            }

            timer::Release_Time_(2);

            if (done) {
                break;
            }
        }

        fields::Clear_Fields_();
        MOX2::Put_Auto_Function_(&saved_auto_func);
        fields::Clear_Fields_();
        MOX::_mp_screen = nullptr;
        fields::Deactivate_Help_List_();
        FILEDEF::Save_Game_Settings_();
    }

    void __cdecl Hotseat_Select_Player_(void) {
        uint8_t fade_flag = 0;
        uint8_t exit_code = 0;
        int16_t selected_player_idx = -1;
        int16_t prev_scan_id = -1;
        bool is_done = false;

        MISC::Fade_Into_Screen_(Fade_In_Function_, &fade_flag, 0);
        fields::Assign_Auto_Function_(Hotseat_Draw_Next_Player_, 2);
        fields::Clear_Fields_();
        fields::Set_Input_Delay_(3);
        Hotseat_Load_Next_Player_();
        fields::Set_Field_Outline_Color_(0xF0);

        MOX::_mp_screen->sel_anim_frame = 0;
        MOX::_mp_screen->sel_anim_active = 1;

        if (MOX::_mp_screen->force_player_selection == 1) {
            MOX::_game_type = 0;
            selected_player_idx = MOX::_mp_screen->forced_player_idx;
            is_done = true;
        } else if (MOX::_mp_screen->exit_screen_flag != 0) {
            exit_code = 1;
            is_done = true;
        } else {
            Hotseat_Add_Next_Player_Fields_();
            MISC::Fade_Into_Screen_(Hotseat_Draw_Next_Player_, &fade_flag, 11 /* 0xB */);
        }

        while (!is_done) {
            int16_t input = fields::Get_Input_();
            int16_t scan_id = fields::Scan_Input_();
            MOX::_mp_screen->last_scan_id = scan_id;

            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_mp_screen->player_button_status[i] == 2) {
                    if (MOX::_mp_screen->player_field_ids[i] == input || MOX::_mp_screen->game_list_field_ids[i] == input) {
                        is_done = true;
                        selected_player_idx = i;
                        MAINSCR2::_force_main_screen_fade = 1;
                    }

                    int16_t current_scan_id = MOX::_mp_screen->last_scan_id;
                    if (MOX::_mp_screen->player_field_ids[i] == current_scan_id || MOX::_mp_screen->game_list_field_ids[i] == current_scan_id) {
                        if (prev_scan_id != current_scan_id) {
                            MOX::_mp_screen->sel_anim_frame = 0;
                            MOX::_mp_screen->sel_anim_active = 1;
                        }
                        prev_scan_id = current_scan_id;
                    }
                }
            }

            if (!is_done) {
                video::Set_Page_Off_();
                Hotseat_Draw_Next_Player_();
                RUSS::Mox_Sync_Update_();
            }
        }

        fields::Clear_Fields_();
        fields::Set_Input_Delay_(3);
        fields::Deactivate_Auto_Function_();
        allocate::Pop_Block_(MOX::_screen_seg);
        MOX::_mp_screen = nullptr;

        if (exit_code == 0) {
            Restore_Hotseat_Map_Info_();
            if (selected_player_idx != -1) {
                MOX::_PLAYER_NUM = selected_player_idx;
            }
            MOX::_current_screen = 0x27 /* SCREEN_REPORTS */;
            MOX::_return_screen = 0x11 /* SCREEN_HOTSEAT_SELECT_PLAYER */;
            Restore_Hotseat_Map_Info_();

            int home_planet_id = MOX::_player[MOX::_PLAYER_NUM].home_planet_id;
            int star_idx = MOX::_planet[home_planet_id].star_index;
            s_star_data* star = &MOX::_star[star_idx];
            MAINSCR::Center_Map_(star->x, star->y);
        } else {
            MOX::_current_screen = 0xC;
            MOX::_return_screen = 0x11;
        }

        if (REPORT::Must_Display_Reports_(MOX::_PLAYER_NUM)) {
            MAINSCR::Update_Ship_Icons_In_Between_Turns_();
            MOX::_stardate_at_beg_of_turn = MOX::_stardate;
            MAINSCR::Fade_Into_Main_Screen_();
        }
    }

    void __cdecl NullModem_Setup_Screen_() {
        int32_t orig_com_port = (int32_t)NETMOX::_net.modem_settings.com_port_index;
        int32_t orig_baud_rate = (int32_t)NETMOX::_net.modem_settings.baud_rate;
        int16_t exit_screen = 0;
        int16_t input_field;

        MULTPLAY::Load_NullModem_Setup_Screen_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_NullModem_Setup_Screen_, 1);
        EVANHELP::Set_Null_Modem_Setup_Screen_Help_List_(MOX::_mp_screen->screen_x, MOX::_mp_screen->screen_y);

        do {
            fields::Clear_Fields_();
            MULTPLAY::Add_NullModem_Setup_Fields_();

            input_field = fields::Get_Input_();
            MOX::_mp_screen->last_scan_id = fields::Scan_Input_();

            if (MOX::_mp_screen->flag_serial_type == 1) {
                for (int16_t i = 0; i < 8; ++i) {
                    if (MOX::_mp_screen->game_list_field_ids[i] == input_field) {
                        input_field = -1;
                        NETMOX::_net.modem_settings.com_port_index = i;
                        MOX::_mp_screen->flag_serial_type = 0;
                        break;
                    }
                }
            }

            if (MOX::_mp_screen->flag_serial_type == 2) {
                for (int16_t i = 0; i < 8; ++i) {
                    if (MOX::_mp_screen->game_list_field_ids[i] == input_field) {
                        input_field = -1;
                        NETMOX::_net.modem_settings.baud_rate = atol(MULTPLAY::_baud_rate_strings[i]);
                        MOX::_mp_screen->flag_serial_type = 0;
                        break;
                    }
                }
            }

            if (MOX::_mp_screen->game_list_field_ids[0] == input_field) {
                MOX::_mp_screen->flag_serial_type = 1;
                MOX::_mp_screen->last_scan_id = -1;
                fields::Clear_Fields_();
            } else if (MOX::_mp_screen->game_list_field_ids[1] == input_field) {
                MOX::_mp_screen->flag_serial_type = 2;
                MOX::_mp_screen->last_scan_id = -1;
                fields::Clear_Fields_();
            }

            if (MOX::_mp_screen->field_id_close_dropdown == input_field) {
                MOX::_mp_screen->flag_serial_type = 0;
            } else if (MOX::_mp_screen->field_id_cancel == input_field) {
                NETMOX::_net.modem_settings.com_port_index = orig_com_port;
                NETMOX::_net.modem_settings.baud_rate = orig_baud_rate;
                exit_screen = 1;
            } else if (MOX::_mp_screen->field_id_ok == input_field) {
                exit_screen = 1;
            }

            if (exit_screen == 0) {
                video::Set_Page_Off_();
                MULTPLAY::Draw_NullModem_Setup_Screen_();
                RUSS::Mox_Sync_Update_();
            }

        } while (exit_screen == 0);

        _memcpy(MOX::_settings.modem_settings.cmd_reset, NETMOX::_net.modem_settings.cmd_reset, 131);

        MOX::_settings.modem_settings.com_port_index = NETMOX::_net.modem_settings.com_port_index;
        MOX::_settings.modem_settings.baud_rate = NETMOX::_net.modem_settings.baud_rate;
        MOX::_settings.modem_settings.parity = NETMOX::_net.modem_settings.parity;

        MOX::_settings.modem_settings.data_bits = NETMOX::_net.modem_settings.data_bits;

        reinterpret_cast<uint8_t*>(&MOX::_settings.modem_settings.serial_cfg_ext)[3] = 
            reinterpret_cast<uint8_t*>(&NETMOX::_net.modem_settings.serial_cfg_ext)[3];

        FILEDEF::Save_Game_Settings_();
    }

    int8_t __cdecl Get_Valid_MP_Save_Games_(int8_t* valid_types_out) {
        int8_t result;
        const char* warning_msg;

        result = Do_Get_Valid_MP_Save_Games_(valid_types_out);
        if (result == 0) {
            warning_msg = HAROLD::H_Message_(0xf4 /* HESTR_0F4_NO_MULTIPLAYER_SAVED_G */);
            GENDRAW::Draw_Warning_Box_(warning_msg);
        }

        return result;
    }

    int8_t __cdecl Do_Get_Valid_MP_Save_Games_(int8_t* valid_types_out) {
        int8_t found_valid = 0;

        for (int16_t i = 0; i < 10; ++i) {
            uint8_t game_type = FILEDEF::Get_Game_Type_From_Saved_Game_(i);

            // Valid multiplayer types are between 1 and 3
            if (game_type >= 1 && game_type <= 3) {
                valid_types_out[i] = (int8_t)game_type;
                found_valid = 1;
            } else {
                valid_types_out[i] = 0;
            }
        }

        return found_valid;
    }

    void __cdecl Copy_Modem_Defaults_To_Net_Struct_() {
        _memcpy(NETMOX::_net.modem_settings.cmd_reset, MOX::_settings.modem_settings.cmd_reset, 131);

        NETMOX::_net.modem_settings.com_port_index = MOX::_settings.modem_settings.com_port_index;
        NETMOX::_net.modem_settings.baud_rate = MOX::_settings.modem_settings.baud_rate;

        *reinterpret_cast<uint32_t*>(&NETMOX::_net.modem_settings.parity) =
            *reinterpret_cast<uint32_t*>(&MOX::_settings.modem_settings.parity);

        NETMOX::_net.modem_settings.data_bits = MOX::_settings.modem_settings.data_bits;

        *reinterpret_cast<uint8_t*>(&NETMOX::_net.modem_settings.serial_cfg_ext) =
            *reinterpret_cast<uint8_t*>(&MOX::_settings.modem_settings.serial_cfg_ext);

        if ((int32_t)NETMOX::_net.modem_settings.com_port_index < 0 || (int32_t)NETMOX::_net.modem_settings.com_port_index > 7) {
            NETMOX::_net.modem_settings.com_port_index = 1;
        }

        uint32_t baud = NETMOX::_net.modem_settings.baud_rate;
        if (baud != 1200 && baud != 4800 && baud != 9600 &&
            baud != 14400 && baud != 19200 && baud != 28800 &&
            baud != 38400) {
            NETMOX::_net.modem_settings.baud_rate = 14400;
        }

        if (NETMOX::_net.modem_settings.parity != 'E' && NETMOX::_net.modem_settings.parity != 'O') {
            NETMOX::_net.modem_settings.parity = 'E';
        }

        if ((int32_t)NETMOX::_net.modem_settings.com_port_index < 0 || (int32_t)NETMOX::_net.modem_settings.com_port_index >= 8) {
            NETMOX::_net.modem_settings.com_port_index = 0;
        }

        if ((int32_t)NETMOX::_net.modem_settings.baud_rate < 1200) {
            NETMOX::_net.modem_settings.baud_rate = 1200;
        }
        else if ((int32_t)NETMOX::_net.modem_settings.baud_rate > 38400) {
            NETMOX::_net.modem_settings.baud_rate = 38400;
        }
    }

    void __cdecl Add_MP_Setup_Screen_Fields_(void) {
        s_animation_header* anim;
        int16_t x, y;

        fields::Clear_Fields_();

        x = MOX::_mp_screen->screen_x;
        y = MOX::_mp_screen->screen_y;

        // TEN hidden field
        anim = MOX::_mp_screen->anim_ten;
        MOX::_mp_screen->field_id_ten = fields::Add_Hidden_Field_(
            x + 0x3B,
            y + 0x5B,
            x + 0x3B + animate::Get_Width_(anim),
            y + 0x5B + animate::Get_Height_(anim),
            s_N_0055765c,
            0x28 /* 40 */
        );

        // Modem hidden field
        anim = MOX::_mp_screen->anim_modem;
        MOX::_mp_screen->field_id_modem = fields::Add_Hidden_Field_(
            x + 0x3B,
            y + 0x7A,
            x + 0x3B + animate::Get_Width_(anim),
            y + 0x7A + animate::Get_Height_(anim),
            s_M_00557ed8,
            0x28 /* 40 */
        );

        // Serial hidden field
        anim = MOX::_mp_screen->anim_serial;
        MOX::_mp_screen->field_id_serial = fields::Add_Hidden_Field_(
            x + 0x3B,
            y + 0x9B,
            x + 0x3B + animate::Get_Width_(anim),
            y + 0x9B + animate::Get_Height_(anim),
            DAT_00558540,
            0x28 /* 40 */
        );

        // Network hidden field
        anim = MOX::_mp_screen->anim_network;
        MOX::_mp_screen->field_id_network = fields::Add_Hidden_Field_(
            x + 0x3B,
            y + 0xBB,
            x + 0x3B + animate::Get_Width_(anim),
            y + 0xBB + animate::Get_Height_(anim),
            s_H_005569d4,
            0x28 /* 40 */
        );

        // Help button
        MOX::_mp_screen->field_id_help = fields::Add_Button_Field_(
            x + 0x6E,
            y + 0xDC,
            s__00551100,
            MOX::_mp_screen->anim_btn_help,
            s_T_005598f8,
            0x28 /* 40 */
        );

        MOX::_mp_screen->setup_screen_state = 0;

        // Cancel button
        MOX::_mp_screen->field_id_cancel = fields::Add_Button_Field_(
            x + 0xB0,
            y + 0x11E,
            s__00551100,
            MOX::_mp_screen->anim_btn_cancel,
            s__00551120,
            0x28 /* 40 */
        );

        // Check if any multi-player method is enabled
        if (MOX::_mp_screen->flag_ten_disabled != 1 &&
            MOX::_mp_screen->flag_modem_disabled != 1 &&
            MOX::_mp_screen->flag_serial_type != 1 &&
            MOX::_mp_screen->flag_network_disabled != 1)
        {
            MOX::_mp_screen->field_id_next = -1000;
            MOX::_mp_screen->field_id_accept = -1000;
            MOX::_mp_screen->field_id_back = -1000;
            MOX::_mp_screen->field_id_setup = -1000;
            Set_Multi_Player_Game_Type_();
            return;
        }

        // Next button
        MOX::_mp_screen->field_id_next = fields::Add_Button_Field_(
            x + 0x10D,
            y + 0x5B,
            s__00551100,
            MOX::_mp_screen->anim_btn_next,
            s_S_005569cc,
            0x28 /* 40 */
        );

        // Accept button
        MOX::_mp_screen->field_id_accept = fields::Add_Button_Field_(
            x + 0x10D,
            y + 0x7A,
            s__00551100,
            MOX::_mp_screen->anim_btn_accept,
            s_L_005560e8,
            0x28 /* 40 */
        );

        // Back button
        if (MOX::_mp_screen->flag_network_disabled == 1) {
            MOX::_mp_screen->field_id_back = -1000;
        } else {
            MOX::_mp_screen->field_id_back = fields::Add_Button_Field_(
                x + 0x10D,
                y + 0x9B,
                s__00551100,
                MOX::_mp_screen->anim_btn_back,
                DAT_0055aa90,
                0x28 /* 40 */
            );
        }

        // Setup button
        if (MOX::_mp_screen->flag_modem_disabled != 1 &&
            MOX::_mp_screen->flag_serial_type != 1)
        {
            MOX::_mp_screen->field_id_setup = -1000;
        } else {
            MOX::_mp_screen->field_id_setup = fields::Add_Button_Field_(
                x + 0x10D,
                y + 0xBB,
                s__00551100,
                MOX::_mp_screen->anim_btn_setup,
                s_C_00552420,
                0x28 /* 40 */
            );
        }

        Set_Multi_Player_Game_Type_();
    }

    void __cdecl Add_Modem_Setup_Fields2_(void) {
        short screen_x = MOX::_mp_screen->screen_x;
        short screen_y = MOX::_mp_screen->screen_y;

        MOX::_mp_screen->field_id_cancel = -1000;

        if (MOX::_mp_screen->flag_modem_disabled == 1) {
            short start_x = screen_x + 0x94;
            short current_y = screen_y + 0x56;
            short end_x = start_x + 0x55;

            for (int i = 0; i < 8; ++i) {
                MOX::_mp_screen->game_list_field_ids[i] = fields::Add_Hidden_Field_(start_x, current_y, end_x, current_y + 0xF, s__00551100, 0x28);
                current_y += 0xF;
            }
        }
        else if (MOX::_mp_screen->flag_modem_disabled == 2) {
            short start_x = screen_x + 0x12F;
            short current_y = screen_y + 0x60;
            short end_x = start_x + 0x50;

            for (int i = 0; i < 8; ++i) {
                MOX::_mp_screen->game_list_field_ids[i] = fields::Add_Hidden_Field_(start_x, current_y, end_x, current_y + 0xF, s__00551100, 0x28);
                current_y += 0xF;
            }
        }
        else {
            short btn_y = screen_y + 0xC7;

            MOX::_mp_screen->field_id_cancel = fields::Add_Button_Field_(
                screen_x + 0x3D, btn_y, s__00551100, MOX::_mp_screen->anim_btn_cancel, s__00551120, 0x28
            );

            MOX::_mp_screen->field_id_ok = fields::Add_Button_Field_(
                screen_x + 0xFF, btn_y, s__00551100, MOX::_mp_screen->anim_btn_unk_3, s__00551120, 0x28
            );

            short hidden_y1 = screen_y + 0x3E;
            short hidden_y2 = screen_y + 0x4D;

            MOX::_mp_screen->game_list_field_ids[0] = fields::Add_Hidden_Field_(
                screen_x + 0x7A, hidden_y1, screen_x + 0xCA, hidden_y2, s__00551100, 0x28
            );

            MOX::_mp_screen->game_list_field_ids[1] = fields::Add_Hidden_Field_(
                screen_x + 0x124, hidden_y1, screen_x + 0x174, hidden_y2, s__00551100, 0x28
            );

            MOX::_mp_screen->game_list_field_ids[5] = fields::Add_Hidden_Field_(
                screen_x + 0x126, screen_y + 0x9D, screen_x + 0x176, screen_y + 0xAC, s__00551100, 0x28
            );
        }

        MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x20, MOX::_global_cache_seg);

        MOX::_mp_screen->field_id_close_dropdown = fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s__00551100, 0x29);
    }

void __cdecl Load_Modem_Setup_Screen_(void) {
    MOX::_mp_screen = &MULTPLAY::_g_mp_screen;

    memset(&MULTPLAY::_g_mp_screen, 0, sizeof(s_mp_screen));

    _memcpy(MOX::_mp_screen->signature, s_Evan_Axelrad_0055aa48, 13);

    MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
    MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 32, MOX::_global_cache_seg);
    MOX::_mp_screen->btn_accept_off = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 38, MOX::_global_cache_seg);
    MOX::_mp_screen->btn_accept_on = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 39, MOX::_global_cache_seg);

    MOX::_mp_screen->anim_btn_cancel = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 33, MOX::_screen_seg);
    MOX::_mp_screen->anim_btn_unk_3 = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 34, MOX::_screen_seg);

    MOX::_mp_screen->screen_x = (int16_t)((640 - animate::Get_Width_(MOX::_mp_screen->bg_seg)) / 2);
    MOX::_mp_screen->screen_y = (int16_t)((480 - animate::Get_Height_(MOX::_mp_screen->bg_seg)) / 2);

    MOX::_mp_screen->subscreen_mode = 1;
}

    void __cdecl Add_NullModem_Setup_Fields_() {
        int16_t screen_x = MOX::_mp_screen->screen_x;
        int16_t screen_y = MOX::_mp_screen->screen_y;

        if (MOX::_mp_screen->flag_serial_type == 1) {
            int16_t x1 = screen_x + 0x86;
            int16_t y1 = screen_y + 0x74;
            int16_t x2 = x1 + 0x50;
            for (int i = 0; i < 8; ++i) {
                MOX::_mp_screen->game_list_field_ids[i] = fields::Add_Hidden_Field_(x1, y1, x2, y1 + 0xE, s__00551100, 41);
                y1 += 15;
            }
        } else if (MOX::_mp_screen->flag_serial_type == 2) {
            int16_t x1 = screen_x + 0x12C;
            int16_t y1 = screen_y + 0x73;
            int16_t x2 = x1 + 0x50;
            for (int i = 0; i < 8; ++i) {
                MOX::_mp_screen->game_list_field_ids[i] = fields::Add_Hidden_Field_(x1, y1, x2, y1 + 14, s__00551100, 41);
                y1 += 15;
            }
        } else {
            MOX::_mp_screen->game_list_field_ids[0] = fields::Add_Hidden_Field_(screen_x + 0x76, screen_y + 0x52, screen_x + 0xC6, screen_y + 0x61, s__00551100, 41);
            MOX::_mp_screen->game_list_field_ids[1] = fields::Add_Hidden_Field_(screen_x + 0x121, screen_y + 0x51, screen_x + 0x16F, screen_y + 0x60, s__00551100, 41);

            int16_t btn_y = screen_y + 0x7C;
            MOX::_mp_screen->field_id_cancel = fields::Add_Button_Field_(screen_x + 0x3B, btn_y, s__00551100, MOX::_mp_screen->anim_btn_cancel, s__00551120, 40);
            MOX::_mp_screen->field_id_ok = fields::Add_Button_Field_(screen_x + 0xFD, btn_y, s__00551100, MOX::_mp_screen->anim_btn_unk_3, s__00551120, 40);
        }

        MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 35, MOX::_global_cache_seg);
        MOX::_mp_screen->field_id_close_dropdown = fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551120, 41);
    }

    void __cdecl Load_NullModem_Setup_Screen_(void) {
        int16_t width;
        int16_t height;

        MOX::_mp_screen = &MULTPLAY::_g_mp_screen;

        // Reset the screen data structure
        memset(&MULTPLAY::_g_mp_screen, 0, sizeof(s_mp_screen));

        // Set the hardcoded signature for the screen
        _memcpy(MOX::_mp_screen->signature, s_Evan_Axelrad_0055aa48, 13);

        // Load visual assets from MULTIGM.LBX
        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
        MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x23, MOX::_global_cache_seg);
        MOX::_mp_screen->btn_accept_off = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x26, MOX::_global_cache_seg);
        MOX::_mp_screen->btn_accept_on = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0x27, MOX::_global_cache_seg);

        // Load button animations (note: indices might be specific to serial/modem setup)
        MOX::_mp_screen->anim_btn_cancel = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 0x24, MOX::_screen_seg);
        MOX::_mp_screen->anim_btn_unk_3 = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558d04, 0x25, MOX::_screen_seg);

        // Center the background on the 640x480 screen
        width = animate::Get_Width_(MOX::_mp_screen->bg_seg);
        MOX::_mp_screen->screen_x = (640 - width) / 2;

        height = animate::Get_Height_(MOX::_mp_screen->bg_seg);
        MOX::_mp_screen->screen_y = (480 - height) / 2;

        // Sync modem settings from global configuration to the active network state.
        // This copies 131 bytes starting from cmd_reset, which likely includes the init string.
        _memcpy(NETMOX::_net.modem_settings.cmd_reset, MOX::_settings.modem_settings.cmd_reset, 131);

        // Sync the leading fields of the modem configuration manually
        auto& src = MOX::_settings.modem_settings;
        auto& dst = NETMOX::_net.modem_settings;

        dst.com_port_index = src.com_port_index;
        dst.baud_rate = src.baud_rate;
        dst.parity = src.parity;

        // Copy data bits and serial configuration extension manually (offsets 0xC and 0x10)
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(&dst) + 0x0C) = 
            *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(&src) + 0x0C);
        
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(&dst) + 0x10) = 
            *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(&src) + 0x10);
    }

    void __cdecl Modem_Setup_Screen_() {
        int8_t original_lang = MOX::_settings.language;
        int16_t help_anchor_field;
        bool exit_loop = false;
        bool save_on_exit = false;
        bool redraw_flag = false;
        bool first_pass = true;
        char cmd_reset_buf[40];
        char cmd_init_buf[40];
        char cmd_hangup_buf[40];
        char dial_number_buf[40];
        s_network net_backup;

        MOX::_settings.language = 0;
        HAROLD::Load_Language_Fonts_();
        fields::Disable_Cancel_();

        // Backup current network configuration
        _memcpy(&net_backup, &NETMOX::_net, sizeof(s_network));

        // Ghidra decompiler shows manual string copies, likely diagnostic/unused in this function scope
        strcpy(cmd_reset_buf, NETMOX::_net.modem_settings.cmd_reset);
        strcpy(cmd_init_buf, NETMOX::_net.modem_settings.cmd_init);
        strcpy(cmd_hangup_buf, NETMOX::_net.modem_settings.cmd_hangup);
        strcpy(dial_number_buf, NETMOX::_net.modem_settings.dial_number);

        fields::Clear_Fields_();
        Load_Modem_Setup_Screen_();
        fields::Assign_Auto_Function_(Draw_Modem_Setup_Screen_, 1);
        Add_Modem_Setup_Fields_();

        help_anchor_field = fields::_fields_count;
        fields::Deactivate_Help_List_();
        help_anchor_field--;

        while (!exit_loop) {
            if (first_pass) {
                fields::Clear_Fields_Above_(help_anchor_field);
            } else {
                fields::Clear_Fields_();
            }

            Add_Modem_Setup_Fields2_();
            int16_t input_id = fields::Get_Input_();
            int16_t scan_id = fields::Scan_Input_();
            MOX::_mp_screen->last_scan_id = scan_id;

            // Mode 1: COM Port selection dropdown
            if (MOX::_mp_screen->flag_modem_disabled == 1) {
                for (int16_t i = 0; i < 8; ++i) {
                    if (MOX::_mp_screen->game_list_field_ids[i] == input_id) {
                        input_id = -1;
                        redraw_flag = true;
                        NETMOX::_net.modem_settings.com_port_index = (int32_t)i;
                        MOX::_mp_screen->flag_modem_disabled = 0;
                        break;
                    }
                }
            }

            // Mode 2: Baud rate selection dropdown
            if (MOX::_mp_screen->flag_modem_disabled == 2) {
                for (int16_t i = 0; i < 8; ++i) {
                    if (MOX::_mp_screen->game_list_field_ids[i] == input_id) {
                        input_id = -1;
                        redraw_flag = true;
                        NETMOX::_net.modem_settings.baud_rate = (int32_t)atol(MULTPLAY::_baud_rate_strings[i]);
                        MOX::_mp_screen->flag_modem_disabled = 0;
                        break;
                    }
                }
            }

            if (Check_Modem_Init_Help_Fields_(input_id, original_lang)) {
                redraw_flag = true;
            }

            if (MOX::_mp_screen->game_list_field_ids[0] == input_id) {
                MOX::_mp_screen->flag_modem_disabled = 1; /* COM selection mode */
                MOX::_mp_screen->last_scan_id = -1;
                fields::Clear_Fields_();
                first_pass = false;
            } else if (MOX::_mp_screen->game_list_field_ids[1] == input_id) {
                MOX::_mp_screen->flag_modem_disabled = 2; /* Baud selection mode */
                MOX::_mp_screen->last_scan_id = -1;
                fields::Clear_Fields_();
                first_pass = false;
            } else if (MOX::_mp_screen->game_list_field_ids[5] == input_id) {
                NETMOX::_net.modem_initialized = !NETMOX::_net.modem_initialized;
            } else if (MOX::_mp_screen->field_id_close_dropdown == input_id) {
                MOX::_mp_screen->flag_modem_disabled = 0;
                redraw_flag = true;
                MOX::_mp_screen->last_scan_id = -1;
            } else if (MOX::_mp_screen->field_id_cancel == input_id) {
                save_on_exit = false;
                exit_loop = true;
                // Restore backed up network settings
                _memcpy(&NETMOX::_net, &net_backup, sizeof(s_network));
            } else if (MOX::_mp_screen->field_id_ok == input_id) {
                save_on_exit = true;
                exit_loop = true;
            }

            if (!exit_loop) {
                if (!redraw_flag) {
                    video::Set_Page_Off_();
                    Draw_Modem_Setup_Screen_();
                    RUSS::Mox_Sync_Update_();
                } else {
                    redraw_flag = false;
                    fields::Clear_Fields_();
                    Add_Modem_Setup_Fields_();
                    first_pass = true;
                    help_anchor_field = fields::_fields_count - 1;
                }
            }
        }

        if (save_on_exit) {
            // Sync working network settings to global game settings
            _memcpy(MOX::_settings.modem_settings.cmd_reset, NETMOX::_net.modem_settings.cmd_reset, 131);

            MOX::_settings.modem_settings.com_port_index = NETMOX::_net.modem_settings.com_port_index;
            MOX::_settings.modem_settings.baud_rate = NETMOX::_net.modem_settings.baud_rate;
            MOX::_settings.modem_settings.parity = NETMOX::_net.modem_settings.parity;
            MOX::_settings.modem_settings.data_bits = NETMOX::_net.modem_settings.data_bits;
            MOX::_settings.modem_settings.serial_cfg_ext = NETMOX::_net.modem_settings.serial_cfg_ext;

            FILEDEF::Save_Game_Settings_();
        }

        MOX::_settings.language = original_lang;
        HAROLD::Load_Language_Fonts_();
    }

    int16_t __cdecl Load_Multi_Player_Game_Screen_(void) {
        void* auto_func_state;
        s_game_popup_fields descriptions;
        s_game_popup_fields* star_date_fields; // Part of local stack
        char warning_msg[100];
        bool exit_loop = false;
        int16_t result = -1;

        if (MOX::_game_type == 2) {
            MULTPLAY::Change_MP_Game_Name_();
            strcpy(NETMOX::_net.cur_game_name, MOX::_settings.net_game_name);

            if (NETMOX::_net.cur_game_name[0] == '\0') {
                // Initialize default game name "MooII " with a random letter at the end
                strcpy(NETMOX::_net.cur_game_name, "MooII "); /* s_MooII_0055aa80 */
                NETMOX::_net.cur_game_name[5] = (char)(clock() % 25) + 'a';
            }
        }

        MOX2::Get_Auto_Function_(&auto_func_state);
        fields::Clear_Fields_();
        fields::Set_Field_Outline_Color_(0xF0);
        MULTPLAY::Load_Load_MP_Game_Screen_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Load_MP_Game_Screen_, 1);

        MOX::_mp_screen->subscreen_mode = 2;
        FILEDEF::Get_Saved_Game_Descriptions_(&descriptions);

        // Offset ESP+0x18 used as first argument for Get_Star_Date_Strings_
        star_date_fields = &descriptions;
        LOADSAVE::Get_Star_Date_Strings_(star_date_fields, MOX::_save_game_stardates);
        LOADSAVE::Get_Save_Game_Date_Strings_(MOX::_save_game_dates);

        if (!MULTPLAY::Add_Load_MP_Game_Screen_Fields_()) {
            MOX2::Put_Auto_Function_(&auto_func_state);
            fields::Clear_Fields_();
            fields::Set_Input_Delay_(3);
            return -1;
        }

        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Load_MP_Game_Screen_, reinterpret_cast<uint8_t*>(&MOX::_mp_screen->needs_full_redraw), 11);

        while (!exit_loop) {
            timer::Mark_Time_();
            int16_t input_id = fields::Get_Input_();
            MOX::_mp_screen->last_scan_id = fields::Scan_Input_();

            for (int16_t i = 0; i < 10; ++i) {
                if (input_id > 0 && MOX::_mp_screen->game_list_field_ids[i] == input_id) {
                    if (MOX::_mp_screen->save_slot_valid[i]) {
                        uint8_t original_game_type = MOX::_game_type;
                        if (FILEDEF::Load_Game_(i)) {
                            MOX::_game_type = original_game_type;
                            // Re-initialize network player IDs for all players after loading
                            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                                MOX::_player[j].network_player_id = -1;
                            }
                            result = i;
                            exit_loop = true;
                        } else {
                            result = -1;
                            exit_loop = true;
                        }
                    } else {
                        char* fmt = const_cast<char*>(HAROLD::H_Message_(0xFD /* HESTR_0FD_SAVE_D_IS_NOT_A_VALID_ */));
                        _sprintf(warning_msg, fmt, i);
                        GENDRAW::Draw_Warning_Box_(warning_msg);
                        result = -1;
                    }
                }
            }

            if (input_id == MOX::_mp_screen->field_id_cancel) {
                MOX::_current_screen = 15;
                result = -1;
                exit_loop = true;
            }

            if (!exit_loop) {
                video::Set_Page_Off_();
                MULTPLAY::Draw_Load_MP_Game_Screen_();
                RUSS::Mox_Sync_Update_();
            }
        }

        MOX2::Put_Auto_Function_(&auto_func_state);
        fields::Clear_Fields_();
        fields::Set_Input_Delay_(3);

        return result;
    }

    void __cdecl Draw_NullModem_Setup_Screen_(void) {
        short x = MOX::_mp_screen->screen_x;
        short y = MOX::_mp_screen->screen_y;
        char buffer[32];
        s_colors style_colors;

        video::Set_Page_Off_();

        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
        animate::Draw_(0, 0, MOX::_mp_screen->pic);

        MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 35, MOX::_global_cache_seg);
        animate::Draw_(x, y, MOX::_mp_screen->bg_seg);

        _memcpy(MULTPLAY::_cur_pal_colors, MULTPLAY::_green_pal_colors, 64);

        MOX::_mox_font_colors_offset = 150;
        style_colors = *MISC::Font_Colors2_(4, 146, 152);
        fonts::Set_Font_Style_(4, &style_colors);

        s_modem_settings* settings = reinterpret_cast<s_modem_settings*>(&NETMOX::_net);

        _sprintf(buffer, s_COM_PORT_FMT, settings->com_port_index + 1);
        fonts::Print_(x + 119, y + 83, buffer);

        _sprintf(buffer, s_BAUD_RATE_FMT, settings->baud_rate);
        fonts::Print_(x + 293, y + 83, buffer);

        if (MOX::_mp_screen->flag_serial_type == 1) {
            MOX::_mp_screen->btn_accept_off = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 38, MOX::_global_cache_seg);
            animate::Draw_(x + 129, y + 101, MOX::_mp_screen->btn_accept_off);

            short cur_y = y + 101;
            for (int i = 0; i < 8; ++i) {
                cur_y += 15;
                if (MOX::_mp_screen->game_list_field_ids[i] == MOX::_mp_screen->last_scan_id) {
                    style_colors = *MISC::Font_Colors2_(3, 146, 154);
                    fonts::Set_Font_Style_(3, &style_colors);
                    MOX::_mp_screen->btn_accept_on = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 39, MOX::_global_cache_seg);
                    animate::Draw_(x + 134, cur_y, MOX::_mp_screen->btn_accept_on);
                } else {
                    style_colors = *MISC::Font_Colors2_(3, 146, 151);
                    fonts::Set_Font_Style_(3, &style_colors);
                }
                _sprintf(buffer, s_COM_PORT_FMT, i + 1);
                fonts::Print_(x + 156, cur_y + 3, buffer);
            }
            return;
        }

        if (MOX::_mp_screen->flag_serial_type == 2) {
            MOX::_mp_screen->btn_accept_off = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 38, MOX::_global_cache_seg);
            animate::Draw_(x + 295, y + 100, MOX::_mp_screen->btn_accept_off);

            short cur_y = y + 100;
            for (int i = 0; i < 8; ++i) {
                cur_y += 15;
                if (MOX::_mp_screen->game_list_field_ids[i] == MOX::_mp_screen->last_scan_id) {
                    style_colors = *MISC::Font_Colors2_(3, 146, 154);
                    fonts::Set_Font_Style_(3, &style_colors);
                    MOX::_mp_screen->btn_accept_on = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 39, MOX::_global_cache_seg);
                    animate::Draw_(x + 300, cur_y, MOX::_mp_screen->btn_accept_on);
                } else {
                    style_colors = *MISC::Font_Colors2_(3, 146, 151);
                    fonts::Set_Font_Style_(3, &style_colors);
                }
                fonts::Print_Right_(x + 363, cur_y + 3, MULTPLAY::_baud_rate_strings[i]);
            }
        }
    }

    void __cdecl Add_Modem_Setup_Fields_() {
        int16_t sx = MOX::_mp_screen->screen_x;
        int16_t sy = MOX::_mp_screen->screen_y;
        
        s_colors* p_colors = MISC::Font_Colors2_(1, 0x92, 0x98);
        s_colors local_colors = *p_colors;
        
        int16_t x_offset = sx + 0x7C;
        fonts::Set_Font_Style_(static_cast<uint8_t>(MOX::_mp_screen->subscreen_mode), &local_colors);
        
        MOX::_mp_screen->game_list_field_ids[3] = fields::Add_Continuous_String_Input_Field_(
            x_offset, sy + 0x81, 0x9A, 0x10, net::modem_settings_cmd_reset, 0x1E, 0, 0, 0x29, 0
        );
        
        MOX::_mp_screen->game_list_field_ids[4] = fields::Add_Continuous_String_Input_Field_(
            x_offset, sy + 0x99, 0x9A, 0x10, net::modem_settings_cmd_init, 0x1E, 0, 0, 0x29, 0
        );
        
        MOX::_mp_screen->game_list_field_ids[6] = fields::Add_Continuous_String_Input_Field_(
            x_offset, sy + 0xB1, 0x9A, 0x10, net::modem_settings_cmd_hangup, 0x1E, 0, 0, 0x29, 0
        );
        
        MOX::_mox_font_colors_offset = 0x96;
        
        p_colors = MISC::Font_Colors2_(4, 0x92, 0x98);
        local_colors = *p_colors;
        
        fonts::Set_Font_Style_(4, &local_colors);
        
        MOX::_mp_screen->game_list_field_ids[2] = fields::Add_Continuous_String_Input_Field_(
            sx + 0x91, sy + 0x5B, 0x96, 0x10, net::modem_settings_dial_number, 0x20, 0, 0, 0x29, 0
        );
    }

    bool __cdecl Check_Modem_Init_Help_Fields_(int16_t field_id, int8_t language) {
        int16_t help_id = -1;

        if (MOX::_mp_screen->game_list_field_ids[0] + field_id == 0) {
            help_id = 0x29A;
        } else if (MOX::_mp_screen->game_list_field_ids[1] + field_id == 0) {
            help_id = 0x29B;
        } else if (MOX::_mp_screen->game_list_field_ids[5] + field_id == 0) {
            help_id = 0x2A0;
        } else if (MOX::_mp_screen->field_id_cancel + field_id == 0) {
            help_id = 0x2A1;
        } else if (MOX::_mp_screen->field_id_ok + field_id == 0) {
            help_id = 0x2A2;
        } else if (MOX::_mp_screen->game_list_field_ids[3] + field_id == 0) {
            help_id = 0x29D;
        } else if (MOX::_mp_screen->game_list_field_ids[4] + field_id == 0) {
            help_id = 0x29E;
        } else if (MOX::_mp_screen->game_list_field_ids[6] + field_id == 0) {
            help_id = 0x29F;
        } else if (MOX::_mp_screen->game_list_field_ids[2] + field_id == 0) {
            help_id = 0x29C;
        }

        if (help_id != -1) {
            MOX::_settings.language = language;
            HAROLD::Load_Language_Fonts_();

            TEXTBOX::Draw_Help_Entry_(help_id);

            MOX::_settings.language = 0;
            HAROLD::Load_Language_Fonts_();

            return true;
        }

        return false;
    }

void __cdecl Load_Load_MP_Game_Screen_(void) {
    int16_t lbx_index;
    int16_t width;
    int16_t height;

    RUSS::Mox_Update_();

    MOX::_mp_screen = &MULTPLAY::_g_mp_screen;
    memset(&MULTPLAY::_g_mp_screen, 0, sizeof(s_mp_screen));

    _memcpy(MOX::_mp_screen->signature, s_Evan_Axelrad_0055aa48, 13);

    MOX::_save_game_dates = (char*)Allocate_First_Block_(MOX::_global_data_seg, 1000);
    MOX::_save_game_stardates = (char*)Allocate_Next_Block_(MOX::_global_data_seg, 1000);

    MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);

    bool load_anim = true;

    if (MOX::_game_type == 1) {
        lbx_index = 0xFF; /* 255 */
    } else if (MOX::_game_type == 2) {
        lbx_index = 0x12; /* 18 */
    } else if (MOX::_game_type == 3) {
        lbx_index = 0xFE; /* 254 */
    } else {
        load_anim = false;
    }

    if (load_anim) {
        MOX::_mp_screen->anim_btn_unk_4 = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, lbx_index, MOX::_global_cache_seg);
    }

    MOX::_mp_screen->anim_btn_cancel = (s_animation_header*)farload::Far_Reload_(s_MULTIGM_LBX_00558d04, 0x101, MOX::_screen_seg);

    width = animate::Get_Width_(MOX::_mp_screen->anim_btn_unk_4);
    MOX::_mp_screen->screen_x = (640 - width) / 2;

    height = animate::Get_Height_(MOX::_mp_screen->anim_btn_unk_4);
    MOX::_mp_screen->screen_y = (399 - height) / 2 + 37 /* 0x25 */;

    MOX::_mp_screen->sel_anim_active = 1;
    MOX::_mp_screen->sel_anim_frame = 0;
}

int8_t __cdecl Add_Load_MP_Game_Screen_Fields_(void) {
    int8_t valid_games;
    int16_t y_offset;
    int16_t field_id;

    valid_games = MULTPLAY::Get_Valid_MP_Save_Games_(MOX::_mp_screen->save_slot_valid);

    if (valid_games != 0) {
        y_offset = 0;
        for (int i = 0; i < 10; ++i) {
            field_id = fields::Add_Hidden_Field_(
                MOX::_mp_screen->screen_x + 38,
                MOX::_mp_screen->screen_y + 64 + y_offset,
                MOX::_mp_screen->screen_x + 400,
                MOX::_mp_screen->screen_y + 86 + y_offset,
                s__00551100,
                0x29 /* ) */
            );

            MOX::_mp_screen->game_list_field_ids[i] = field_id;
            y_offset += 27;
        }

        MOX::_mp_screen->anim_btn_unk_4 = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 18 /* 0x12 */, MOX::_global_cache_seg);

        field_id = fields::Add_Button_Field_(
            MOX::_mp_screen->screen_x + 191,
            MOX::_mp_screen->screen_y + 344,
            s__00551100,
            MOX::_mp_screen->anim_btn_cancel,
            s__00551120,
            0x28 /* ( */
        );
        MOX::_mp_screen->field_id_cancel = field_id;
    }

    return valid_games;
}

void __cdecl Change_MP_Game_Name_() {
    int16_t num_games;
    int16_t i;
    int16_t x, y;
    bool already_exists;
    bool is_valid;
    char prompt[200];
    char warning_msg[120];

    num_games = 10;
    if (netcode::Net_Get_Game_Count_() < 10) {
        num_games = (int16_t)netcode::Net_Get_Game_Count_();
    }

    while (true) {
        video::Set_Page_Back_();
        graphics::Fill_(0, 0, 0x27F /* 639 */, 0x1DF /* 479 */, 0);
        animate::Draw_(0, 0, MOX::_mp_screen->pic);
        video::Copy_Back_To_Off_();
        RUSS::Mox_Sync_Update_();
        
        fields::Assign_Auto_Function_(No_Op1_, 1);
        
        strcpy(prompt, HAROLD::H_Message_(0xFE /* HESTR_0FE_ENTER_NET_GAME_NAME */));
        
        y = namestar::Star_Name_Popup_Screen_Center_Y_();
        x = namestar::Star_Name_Popup_Screen_Center_X_();
        
        namestar::Input_Box_Popup2_(MOX::_settings.net_game_name, 8, 0, prompt, x, y);
        
        is_valid = true;
        already_exists = false;
        
        if (num_games > 0) {
            for (i = 0; i < num_games; i++) {
                if (already_exists) {
                    break;
                }
                
                if (strcmp(MOX::_settings.net_game_name, netcode::Net_Get_Game_Name_(i)) == 0) {
                    _sprintf(warning_msg, HAROLD::H_Message_(0xFF /* HESTR_0FF_S_IS_ALREADY_A_GAME_NA */), MOX::_settings.net_game_name);
                    GENDRAW::Draw_Warning_Box_(warning_msg);
                    already_exists = true;
                    is_valid = false;
                }
            }
        }
        
        if (is_valid) {
            break;
        }
    }

    LOADSAVE::Save_Session_Related_Settings_();
}

void __cdecl Print_Modem_Strings_() {
    int16_t screen_x = MOX::_mp_screen->screen_x;
    int16_t screen_y = MOX::_mp_screen->screen_y;
    s_colors* colors_ptr;
    s_colors local_colors;

    colors_ptr = MISC::Font_Colors2_(1, 0x92, 0x98);
    local_colors = *colors_ptr;

    int16_t x_offset_batch = screen_x + 0x86;
    fonts::Set_Font_Style_(1, &local_colors);

    fonts::Print_(x_offset_batch, screen_y + 0x85, NETMOX::_net.modem_settings.cmd_reset);
    fonts::Print_(x_offset_batch, screen_y + 0x9D, NETMOX::_net.modem_settings.cmd_init);
    fonts::Print_(x_offset_batch, screen_y + 0xB5, NETMOX::_net.modem_settings.cmd_hangup);

    MOX::_mox_font_colors_offset = 0x96;

    colors_ptr = MISC::Font_Colors2_(4, 0x92, 0x98);
    local_colors = *colors_ptr;

    fonts::Set_Font_Style_(4, &local_colors);
    fonts::Print_(screen_x + 0x9B, screen_y + 0x5C, NETMOX::_net.modem_settings.dial_number);
}

int16_t __cdecl Hotseat_Player_Status_(int16_t player_idx) {
    int16_t status = 0;

    if (MOX::_player[player_idx].objectives == 100 && MOX::_player[player_idx].eliminated == 0) {
        status = (MOX::_player[player_idx].player_done_flags == 0) + 1;
    }

    return status;
}

void __cdecl Draw_Load_MP_Game_Screen_() {
    s_colors text_colors;
    char stardate_buf[25];
    char date_buf[25];
    bool is_selected;
    int16_t y_offset = 0;
    int32_t string_buffer_offset = 0;
    const char* lbx_filename = MULTPLAY::s_MULTIGM_LBX_00558d04;

    if (MOX::_mp_screen->needs_full_redraw != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
        MOX::_mp_screen->needs_full_redraw = 0;
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(lbx_filename, 0, MOX::_global_cache_seg);
        animate::Draw_(0, 0, MOX::_mp_screen->pic);
        video::Copy_Off_To_Back_();
    }

    int32_t entry = -1;
    if (MOX::_game_type == 1) {
        entry = 0xFF;
    } else if (MOX::_game_type == 2) {
        entry = 0x12;
    } else if (MOX::_game_type == 3) {
        entry = 0xFE;
    }

    if (entry != -1) {
        MOX::_mp_screen->anim_btn_unk_4 = (s_animation_header*)Buffer_Reload_(lbx_filename, entry, MOX::_global_cache_seg);
    }

    animate::Draw_(MOX::_mp_screen->screen_x, MOX::_mp_screen->screen_y, MOX::_mp_screen->anim_btn_unk_4);

    _memcpy(MULTPLAY::_cur_pal_colors, MULTPLAY::_green_pal_colors, 0x40);

    for (int i = 0; i < 10; ++i) {
        int16_t y_base = MOX::_mp_screen->screen_y + 0x42 + y_offset;
        is_selected = false;

        LOADSAVE::Remove_Embedded_Special_Codes_(MOX::_save_game_description[i].description);

        _memcpy(stardate_buf, reinterpret_cast<void*>(MOX::_save_game_stardates + string_buffer_offset), 25);
        _memcpy(date_buf, reinterpret_cast<void*>(MOX::_save_game_dates + string_buffer_offset), 25);

        s_colors* palette_colors = MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, 0x90, 0x93);
        _memcpy(&text_colors, palette_colors, sizeof(s_colors));

        if (MOX::_mp_screen->save_slot_valid[i] != 0) {
            if (MOX::_mp_screen->game_list_field_ids[i] == MOX::_mp_screen->last_scan_id) {
                palette_colors = MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, 0x90, 0x99);
                _memcpy(&text_colors, palette_colors, sizeof(s_colors));
                is_selected = true;
                MOX::_mp_screen->sel_anim_frame += MOX::_mp_screen->sel_anim_active;
            } else {
                palette_colors = MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, 0x90, 0x97);
                _memcpy(&text_colors, palette_colors, sizeof(s_colors));
            }
        }

        fonts::Set_Font_Style_(MOX::_mp_screen->subscreen_mode, &text_colors);

        if (is_selected) {
            MISC::Increase_Font_Color_Range_(MOX::_mp_screen->sel_anim_frame);

            if (MOX::_mp_screen->sel_anim_frame >= 4) {
                MOX::_mp_screen->sel_anim_frame = 4;
                MOX::_mp_screen->sel_anim_active = -1;
            } else if (MOX::_mp_screen->sel_anim_frame <= -2) {
                MOX::_mp_screen->sel_anim_frame = -2;
                MOX::_mp_screen->sel_anim_active = 1;
            }
            MOX::_mp_screen->sel_anim_frame += MOX::_mp_screen->sel_anim_active;
        }

        int16_t font_h = fonts::Get_Font_Height_();
        const char* desc_text = MOX::_save_game_description[i].description;
        if (desc_text[0] == '\0') {
            desc_text = HAROLD::H_Message_(0x184);
        }

        fonts::Print_(MOX::_mp_screen->screen_x + 0x2F, ((0x16 - font_h) / 2) + y_base, desc_text);

        fonts::Set_Font_Style_(1, &text_colors);
        fonts::Print_(MOX::_mp_screen->screen_x + 0x119, y_base, stardate_buf);
        
        int16_t y_date = y_base + fonts::Get_Font_Height_() + 2;
        fonts::Print_(MOX::_mp_screen->screen_x + 0x119, y_date, date_buf);

        if (MOX::_save_game_description[i].description[0] != '\0') {
            int16_t icon_entry = 0;
            switch (MOX::_mp_screen->save_slot_valid[i]) {
                case 0: icon_entry = 0x102; break;
                case 1: icon_entry = 0x103; break;
                case 2:
                case 3: icon_entry = 0x104; break;
            }

            if (icon_entry > 0) {
                const char* lbx_ptr = *MULTPLAY::PTR_00558D3C;
                s_animation_header* icon_anim = (s_animation_header*)Buffer_Reload_(lbx_ptr, icon_entry, MOX::_global_cache_seg);
                animate::Draw_(MOX::_mp_screen->screen_x + 0x17C, y_date, icon_anim);
            }
        }

        y_offset += 0x1B;
        string_buffer_offset += 0x19;
    }
}

void __cdecl Get_Net_Next_Turn_Player_Colors_() {
    for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
        uint8_t player_color = (uint8_t)MOX::_player[i].color;
        uint8_t base_color = MOX::_main_palette_player_colors[player_color];

        if (player_color == 3) {
            MOX::_mox_font_colors_offset = 12;
            MOX::_mp_screen->player_font_colors_1[i] =
                *MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, 10, 16);

            MOX::_mox_font_colors_offset = 8;
            MOX::_mp_screen->player_font_colors_3[i] =
                *MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, 6, 12);

            MOX::_mox_font_colors_offset = 12;
            MOX::_mp_screen->player_font_colors_2[i] =
                *MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, 10, 16);
        } else {
            MOX::_mox_font_colors_offset = (uint8_t)(base_color + 2);
            MOX::_mp_screen->player_font_colors_1[i] =
                *MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, base_color, (uint8_t)(base_color + 5));

            MOX::_mox_font_colors_offset = (uint8_t)(base_color + 1);
            MOX::_mp_screen->player_font_colors_3[i] =
                *MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, base_color, (uint8_t)(base_color + 3));

            MOX::_mox_font_colors_offset = (uint8_t)(base_color + 2);
            MOX::_mp_screen->player_font_colors_2[i] =
                *MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, base_color, (uint8_t)(base_color + 6));
        }
    }
}

void __cdecl Add_Net_Next_Turn_Fields_()
{
    int16_t screen_x = MOX::_mp_screen->screen_x;
    int16_t screen_y = MOX::_mp_screen->screen_y;
    int16_t x = screen_x + 25;
    
    fields::Clear_Fields_();
    
    int16_t field_y_offset = MOX::_mp_screen->player_field_ids[2];
    uint8_t color_a, color_b, color_c;
    MULTPLAY::Get_Main_Palette_Base_Colors_(MOX::_PLAYER_NUM, &color_a, &color_b, &color_c);
    
    MOX::_mox_font_colors_offset = color_b + 1;
    s_colors* colors_ptr1 = MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, color_a + 1, color_c + 2);
    s_colors local_colors1 = *colors_ptr1;
    
    MOX::_mox_font_colors_offset = color_b + 3;
    s_colors* colors_ptr2 = MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, color_a + 1, color_c + 4);
    s_colors local_colors2 = *colors_ptr2;
    
    fonts::Set_Font_Style_(MOX::_mp_screen->subscreen_mode, &local_colors1);
    fonts::Set_Font_Special_Colors_(&local_colors2);
    
    char name_buffer[80];
    _sprintf(name_buffer, s_format_0055aa28, MOX::_player[MOX::_PLAYER_NUM].name);
    
    int16_t name_width = fonts::Get_String_Width_(name_buffer);
    MULTPLAY::_global_chat_string[0] = '\0';
    
    int16_t chat_field_id = fields::Add_Continuous_String_Input_Field_(
        x + name_width,
        screen_y + field_y_offset + 187 /* 0xBB */,
        567 /* 0x237 */ - name_width,
        17 /* 0x11 */,
        MULTPLAY::_global_chat_string,
        80 /* 0x50 */,
        0,
        0,
        41 /* 0x29 */,
        0
    );
    MOX::_mp_screen->player_field_ids[0] = chat_field_id;
    
    int16_t hidden_field_id = fields::Add_Hidden_Field_(0, 0, 639 /* 0x27F */, 479 /* 0x1DF */, s_C_00552420, 41 /* 0x29 */);
    MOX::_mp_screen->player_field_ids[1] = hidden_field_id;
}

    void __cdecl Draw_Generic_Net_Info_Screen_() {
        video::Set_Page_Off_();

        if (ERIC::Get_Fade_Percent_() > 0) {
            graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);
            MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
            animate::Draw_(0, 0, MOX::_mp_screen->pic);
        } else if (MOX::_mp_screen->needs_full_redraw != 0) {
            graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);
            MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
            animate::Draw_(0, 0, MOX::_mp_screen->pic);
            video::Copy_Off_To_Back_();
            video::Set_Refresh_Stencil_();
            MOX::_mp_screen->needs_full_redraw = 0;
        }

        MOX::_mp_screen->anim_frame_timer++;
        MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, (int)MOX::_mp_screen->lbx_entry_num, MOX::_global_cache_seg);

        if (MOX::_mp_screen->anim_frame_timer != 0) {
            for (int16_t frame_idx = 0; frame_idx < (int16_t)MOX::_mp_screen->anim_frame_timer; frame_idx++) {
                animate::Set_Animation_Frame_(MOX::_mp_screen->bg_seg, frame_idx);
                animate::Draw_(MOX::_mp_screen->screen_x, MOX::_mp_screen->screen_y, MOX::_mp_screen->bg_seg);
            }
        }

        if (animate::Get_Animation_Frame_Count_(MOX::_mp_screen->bg_seg) <= (int16_t)MOX::_mp_screen->anim_frame_timer) {
            MOX::_mp_screen->anim_frame_timer = 0;
        }
    }

int16_t __cdecl Choose_Multi_Network_Game_Screen_() {
    void* saved_auto_fn;
    int16_t result;
    int16_t game_count;
    bool exit_screen = false;
    int16_t input_field;
    int16_t scan_id;

    WINNET::_net_refresh_timer = 0;
    MOX2::Get_Auto_Function_(&saved_auto_fn);
    fields::Clear_Fields_();
    fields::Set_Input_Delay_(3);
    fields::Set_Field_Outline_Color_(0xf0);
    video::Set_Page_Back_();
    graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);

    MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(lbx_filename, 0, MOX::_global_cache_seg);
    animate::Draw_(0, 0, MOX::_mp_screen->pic);
    video::Copy_Back_To_Off_();
    RUSS::Mox_Sync_Update_();
    fields::Assign_Auto_Function_(Draw_Choose_Multi_Net_Game_Screen_, 1);
    Load_Choose_Multi_Net_Game_Screen_();
    MOX::_mp_screen->subscreen_mode = 4;

    do {
        fields::Clear_Fields_();
        Add_Choose_Multi_Net_Game_Fields_();
        timer::Mark_Time_();
        input_field = fields::Get_Input_();
        scan_id = fields::Scan_Input_();
        MOX::_mp_screen->last_scan_id = scan_id;

        if (netcode::Net_Get_Game_Count_() >= 10) {
            game_count = 10;
        } else {
            game_count = (int16_t)netcode::Net_Get_Game_Count_();
        }

        for (int16_t i = 0; i < game_count; ++i) {
            if (input_field > 0 && MOX::_mp_screen->game_list_field_ids[i] == input_field) {
                char* game_name = netcode::Net_Get_Game_Name_(i);
                if (game_name[0] != '\0') {
                    strcpy(NETMOX::_net.cur_game_name, game_name);
                    exit_screen = true;
                }
            }
        }

        if (MOX::_mp_screen->field_id_cancel == input_field) {
            result = -1;
            MOX::_current_screen = 15 /* 0xf */;
            exit_screen = true;
            netcode::Net_Uninit_();
            NETMOX::_net.cur_game_name[0] = '\0';
        }

        if (!exit_screen) {
            video::Set_Page_Off_();
            Draw_Choose_Multi_Net_Game_Screen_();
            RUSS::Mox_Sync_Update_();
        }
    } while (!exit_screen);

    MOX2::Put_Auto_Function_(&saved_auto_fn);
    fields::Clear_Fields_();
    fields::Set_Input_Delay_(3);

    return result;
}

void __cdecl Reload_Generating_Map_Info_() {
    MULTPLAY::_allow_chat_mode = 1;
    MULTPLAY::Reload_Generic_Net_Info_(0x1E /* 30 */);
}

void __cdecl Reload_Getting_Data_Info_() {
    MULTPLAY::_allow_chat_mode = 0;
    MULTPLAY::Reload_Generic_Net_Info_(0x1F);
    MOX::_mp_screen->msg_layout_alt = 1;
}

void __cdecl Draw_SendGet_Net_Info_Screen_(void) {
    s_colors font_colors;
    int16_t text_x;
    int16_t text_y;

    graphics::Fill_(0, 0, 639, 479, 0);

    MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
    animate::Draw_(0, 0, MOX::_mp_screen->pic);

    MOX::_mp_screen->anim_frame_timer++;

    MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, (int)MOX::_mp_screen->lbx_entry_num, MOX::_global_cache_seg);

    if (MOX::_mp_screen->anim_frame_timer != 0) {
        for (int16_t frame = 0; frame < (int16_t)MOX::_mp_screen->anim_frame_timer; ++frame) {
            animate::Set_Animation_Frame_(MOX::_mp_screen->bg_seg, (int)frame);
            animate::Draw_(MOX::_mp_screen->screen_x, MOX::_mp_screen->screen_y, MOX::_mp_screen->bg_seg);
        }
    }

    int16_t frame_count = animate::Get_Animation_Frame_Count_(MOX::_mp_screen->bg_seg);
    if (frame_count <= (int16_t)MOX::_mp_screen->anim_frame_timer) {
        MOX::_mp_screen->anim_frame_timer = 0;
    }

    if (MOX::_temp_string[0] != '\0') {
        MOX::_mox_font_colors_offset = 0xF2;
        s_colors* palette_colors = MISC::Font_Colors2_(4, 0x9C, 0xF8);

        _memcpy(&font_colors, palette_colors, 8);

        fonts::Set_Font_Style_(4, &font_colors);

        if (MOX::_mp_screen->msg_layout_alt == 0) {
            text_x = MOX::_mp_screen->screen_x + 0x72;
            text_y = MOX::_mp_screen->screen_y + 0x42;
        } else {
            text_x = MOX::_mp_screen->screen_x + 0x79;
            text_y = MOX::_mp_screen->screen_y + 0x41;
        }

        fonts::Print_(text_x, text_y, MOX::_temp_string);
    }
}

void __cdecl Reload_Generic_Net_Info_(int16_t lbx_entry_num) {
    MOX::_temp_string[0] = '\0';
    MOX::_mp_screen = &MULTPLAY::_g_mp_screen;

    memset(&MULTPLAY::_g_mp_screen, 0, sizeof(s_mp_screen));

    memcpy(MOX::_mp_screen->signature, MULTPLAY::s_Evan_Axelrad_0055aa48, 13);
    MOX::_mp_screen->lbx_entry_num = lbx_entry_num;

    MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(MULTPLAY::s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
    MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(MULTPLAY::s_MULTIGM_LBX_00558d04, lbx_entry_num, MOX::_global_cache_seg);

    int16_t width = animate::Get_Width_(MOX::_mp_screen->bg_seg);
    int16_t height = animate::Get_Height_(MOX::_mp_screen->bg_seg);

    MOX::_mp_screen->bg_height = animate::Get_Height_(MOX::_mp_screen->bg_seg);

    MOX::_mp_screen->screen_x = (int16_t)((640 - width) / 2);
    MOX::_mp_screen->screen_y = (int16_t)((480 - height) / 2);

    MOX::_mp_screen->subscreen_mode = 4;
    MOX::_mp_screen->needs_full_redraw = 1;
}

    void __cdecl Draw_Choose_Multi_Net_Game_Screen_() {
        if (WINNET::_net_refresh_timer < _clock() + 5000) {
            WINNET::Refresh_Game_List_(WINNET::_game_guid);
            WINNET::_net_refresh_timer = (uint32_t)_clock();
        }

        int16_t display_count;
        if (netcode::Net_Get_Game_Count_() < 10) {
            display_count = (int16_t)netcode::Net_Get_Game_Count_();
        } else {
            display_count = 10;
        }

        MOX::_mp_screen->anim_btn_unk_4 = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 41 /* 0x29 */, MOX::_global_cache_seg);
        animate::Draw_(MOX::_mp_screen->screen_x, MOX::_mp_screen->screen_y, MOX::_mp_screen->anim_btn_unk_4);

        // Update green palette range for the game selection list
        s_palette_entry* dst = &video::_current_palette[144];
        s_palette_entry* src = reinterpret_cast<s_palette_entry*>(_green_pal_colors);
        for (int i = 0; i < 16; ++i) {
            *dst++ = *src++;
        }

        for (int16_t i = 0, row_y_offset = 0; i < display_count; ++i) {
            char* name_ptr = netcode::Net_Get_Game_Name_(i);
            if (name_ptr[0] != '\0') {
                char game_name[100];
                strcpy(game_name, name_ptr);

                bool is_selected = (MOX::_mp_screen->game_list_field_ids[i] == MOX::_mp_screen->last_scan_id);
                s_colors* font_colors;
                s_colors local_colors;

                if (is_selected) {
                    MOX::_mox_font_colors_offset = 151 /* 0x97 */;
                    font_colors = MISC::Font_Colors2_((uint16_t)(uint8_t)MOX::_mp_screen->subscreen_mode, 145 /* 0x91 */, 153 /* 0x99 */);
                    local_colors = *font_colors;
                } else {
                    MOX::_mox_font_colors_offset = 149 /* 0x95 */;
                    font_colors = MISC::Font_Colors2_((uint16_t)(uint8_t)MOX::_mp_screen->subscreen_mode, 145 /* 0x91 */, 151 /* 0x97 */);
                    local_colors = *font_colors;
                }

                fonts::Set_Font_Style_((uint16_t)(uint8_t)MOX::_mp_screen->subscreen_mode, &local_colors);

                if (is_selected) {
                    MISC::Increase_Font_Color_Range_((int16_t)MOX::_mp_screen->sel_anim_frame);

                    if (MOX::_mp_screen->sel_anim_frame >= 4) {
                        MOX::_mp_screen->sel_anim_frame = 4;
                        MOX::_mp_screen->sel_anim_active = -1;
                    } else if (MOX::_mp_screen->sel_anim_frame <= -3) {
                        MOX::_mp_screen->sel_anim_frame = -3;
                        MOX::_mp_screen->sel_anim_active = 1;
                    }
                    MOX::_mp_screen->sel_anim_frame += MOX::_mp_screen->sel_anim_active;
                }

                int16_t font_h = fonts::Get_Font_Height_();
                int16_t centered_y = (22 - font_h) / 2;
                fonts::Print_(MOX::_mp_screen->screen_x + 47 /* 0x2F */, 
                              MOX::_mp_screen->screen_y + 67 /* 0x43 */ + row_y_offset + centered_y, 
                              game_name);
            }
            row_y_offset += 27 /* 0x1b */;
        }
    }

    void __cdecl Add_Choose_Multi_Net_Game_Fields_() {
        if (WINNET::_net_refresh_timer < (uint32_t)_clock() + 5000 /* 5 seconds */) {
            WINNET::Refresh_Game_List_(WINNET::_game_guid);
            WINNET::_net_refresh_timer = (uint32_t)_clock();
        }

        int16_t count = 10;
        if (netcode::Net_Get_Game_Count_() < 10) {
            count = (int16_t)netcode::Net_Get_Game_Count_();
        }

        for (int16_t i = 0; i < 10; ++i) {
            MOX::_mp_screen->game_list_field_ids[i] = -1000;
        }

        if (count > 0) {
            int16_t y_step = 0;
            for (int16_t i = 0; i < count; ++i) {
                int16_t x1 = MOX::_mp_screen->screen_x + 38 /* 0x26 */;
                int16_t y1 = MOX::_mp_screen->screen_y + 64 /* 0x40 */ + y_step;
                int16_t x2 = MOX::_mp_screen->screen_x + 400 /* 0x190 */;
                int16_t y2 = MOX::_mp_screen->screen_y + 86 /* 0x56 */ + y_step;

                int16_t field_id = fields::Add_Hidden_Field_(x1, y1, x2, y2, s__00551100, 41 /* 0x29 */);
                MOX::_mp_screen->game_list_field_ids[i] = field_id;
                y_step += 27 /* 0x1B */;
            }
        }

        MOX::_mp_screen->anim_btn_unk_4 = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 41 /* 0x29 */, MOX::_global_cache_seg);

        int16_t cancel_btn_id = fields::Add_Button_Field_(
            MOX::_mp_screen->screen_x + 191 /* 0xBF */,
            MOX::_mp_screen->screen_y + 344 /* 0x158 */,
            s__00551100,
            MOX::_mp_screen->anim_btn_cancel,
            s__00551120,
            41 /* 0x29 */
        );
        MOX::_mp_screen->field_id_cancel = cancel_btn_id;
    }

    void __cdecl Load_Choose_Multi_Net_Game_Screen_(void) {
        int16_t width;
        int16_t height;

        RUSS::Mox_Update_();

        MOX::_mp_screen = &MULTPLAY::_g_mp_screen;

        memset(&MULTPLAY::_g_mp_screen, 0, sizeof(s_mp_screen));

        // Note: Manual 13-byte copy of the signature string "Evan Axelrad"
        _memcpy(MOX::_mp_screen->signature, MULTPLAY::s_signature_axelrad, 13);

        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, 0, MOX::_global_cache_seg);

        MOX::_mp_screen->anim_btn_unk_4 = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, 0x29, MOX::_global_cache_seg);

        MOX::_mp_screen->anim_btn_cancel = (s_animation_header*)farload::Far_Reload_(s_MULTIGM_LBX, 0x101, MOX::_screen_seg);

        width = animate::Get_Width_(MOX::_mp_screen->anim_btn_unk_4);
        MOX::_mp_screen->screen_x = (640 - width) / 2;

        height = animate::Get_Height_(MOX::_mp_screen->anim_btn_unk_4);
        MOX::_mp_screen->screen_y = ((399 - height) / 2) + 0x25;

        MOX::_mp_screen->sel_anim_frame = 0;
        MOX::_mp_screen->sel_anim_active = 1;
    }

    int16_t __cdecl Choose_Network_Plyrs_Screen_(int16_t* out_selected_player, int8_t* connected_flags, int8_t is_host, int8_t should_load) {
        uint32_t unused_data[4];
        void* saved_auto_func;
        uint8_t fade_flag = 0;
        int16_t result_status = 0;
        bool exit_loop = false;
        int16_t input_field_id;

        unused_data[0] = 0xffffffff;
        unused_data[1] = 0xffffffff;
        unused_data[2] = 0xffffffff;
        unused_data[3] = 0xffffffff;

        MOX2::Get_Auto_Function_(&saved_auto_func);

        if (should_load != 0) {
            Load_Choose_Net_Plyrs_Screen_();
        }

        MOX::_mp_screen->num_remaining_humans = HAROLD::Get_N_Remaining_Humans_();

        MOX::_mp_screen->anim_turn_screen_top = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, 0x1b, MOX::_global_cache_seg);

        int16_t top_w = animate::Get_Width_(MOX::_mp_screen->anim_turn_screen_top);
        MOX::_mp_screen->screen_x = (640 - top_w) / 2;

        int16_t mid_h = animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_mid);
        int16_t bot_h = animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_bot);
        int16_t top_h = animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_top);

        int16_t total_h = mid_h * MOX::_mp_screen->num_remaining_humans + bot_h + top_h - 1;
        MOX::_mp_screen->screen_y = (480 - total_h) / 2;

        MOX::_mp_screen->sel_anim_frame = 0;
        MOX::_mp_screen->sel_anim_active = 1;

        Add_Choose_Net_Plyrs_Fields_();
        fields::Assign_Auto_Function_(Draw_Choose_Net_Plyrs_Screen_, 1);
        MOX::_mp_screen->subscreen_mode = 4;

        MISC::Fade_Into_Screen_(Draw_Choose_Net_Plyrs_Screen_, &fade_flag, 11);

        MOX::_mp_screen->is_host_flag = is_host;
        _memcpy(MOX::_mp_screen->player_connected_flags, connected_flags, 8);

        do {
            timer::Mark_Time_();
            input_field_id = fields::Get_Input_();
            MOX::_mp_screen->last_scan_id = fields::Scan_Input_();

            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if ((MOX::_mp_screen->player_field_ids[i] == input_field_id ||
                     MOX::_mp_screen->game_list_field_ids[i] == input_field_id) &&
                     MOX::_mp_screen->player_connected_flags[i] == 0) {
                    exit_loop = true;
                    *out_selected_player = i;
                    break;
                }
            }

            if (is_host != 0 && MOX::_mp_screen->field_id_begin == input_field_id) {
                char* msg = const_cast<char*>(HAROLD::H_Message_(0xF5 /* HESTR_0F5_READY_TO_START_A_NETWO */));
                if (GENDRAW::Warning_Box2_(msg) != 0) {
                    result_status = 1;
                    exit_loop = true;
                } else {
                    video::Set_Page_Back_();
                    graphics::Fill_(0, 0, 639, 479, 0);
                    video::Copy_Back_To_Off_();
                    RUSS::Mox_Sync_Update_();
                    Add_Choose_Net_Plyrs_Fields_();
                }
            }

            if (!exit_loop) {
                video::Set_Page_Off_();
                Draw_Choose_Net_Plyrs_Screen_();
                RUSS::Mox_Sync_Update_();
            }

            timer::Release_Time_(2);

            if (exit_loop || MOX::_global_esc != 0) {
                break;
            }

        } while (true);

        MOX2::Put_Auto_Function_(&saved_auto_func);
        fields::Clear_Fields_();
        fields::Set_Input_Delay_(3);
        MOX::_mp_screen = nullptr;

        return result_status;
    }

    void __cdecl Get_Main_Palette_Base_Colors_(int16_t player_idx, uint8_t* color_a, uint8_t* color_b, uint8_t* color_c) {
        uint8_t player_color = static_cast<uint8_t>(MOX::_player[player_idx].color);

        *color_a = 0;
        *color_b = 0;
        *color_c = 0;

        if (player_color == 3) {
            *color_a = 10;
            *color_b = 14;
            *color_c = 23;
        } else if (player_color == 4) {
            *color_a = 59; /* 0x3b */
            *color_b = 60; /* 0x3c */
            *color_c = 61; /* 0x3d */
        } else {
            uint8_t base_color = MOX::_main_palette_player_colors[player_color];
            *color_a = base_color;
            *color_b = base_color + 2;
            *color_c = base_color + 3;
        }
    }

    void __cdecl Draw_Net_Next_Turn_Screen_(void) {
        int16_t screen_x = MOX::_mp_screen->screen_x;
        int16_t screen_y = MOX::_mp_screen->screen_y;
        int16_t banner_active_count = 0;
        
        MOX::_mp_screen->anim_frame_timer++;

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            video::Set_Page_Off_();
            MOX::_dont_draw_main_screen_to_back = 1;
            graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);
            MAINSCR::Draw_Mini_Main_Screen_();
            MOX::_mp_screen->needs_full_redraw = 1;
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        MOX::_mp_screen->anim_turn_screen_top = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 42 /* 0x2a */, MOX::_global_cache_seg);
        
        if (MOX::_mp_screen->anim_frame_timer != 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_mp_screen->anim_frame_timer; ++i) {
                animate::Set_Animation_Frame_(MOX::_mp_screen->anim_turn_screen_top, i);
                animate::Draw_(screen_x, screen_y, MOX::_mp_screen->anim_turn_screen_top);
            }
        }

        if (animate::Get_Animation_Frame_Count_(MOX::_mp_screen->anim_turn_screen_top) <= (int16_t)MOX::_mp_screen->anim_frame_timer) {
            MOX::_mp_screen->anim_frame_timer = 0;
        }

        int16_t current_y = screen_y + animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_top);
        MOX::_mp_screen->anim_turn_screen_mid = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 43 /* 0x2b */, MOX::_global_cache_seg);
        
        if (MOX::_mp_screen->needs_full_redraw != 0) {
            animate::Draw_(screen_x, current_y, MOX::_mp_screen->anim_turn_screen_mid);
        }

        current_y += animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_mid);
        
        if (MOX::_mp_screen->needs_full_redraw != 0) {
            MOX::_mp_screen->anim_chat_msg_bg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 40 /* 0x28 */, MOX::_global_cache_seg);
            animate::Draw_(screen_x, current_y, MOX::_mp_screen->anim_chat_msg_bg);
        }

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            uint8_t c1, c2, c3;
            MULTPLAY::Get_Main_Palette_Base_Colors_(i, &c1, &c2, &c3);

            MOX::_mox_font_colors_offset = (uint8_t)(c2 + 1);
            s_colors font_colors_1 = *MISC::Font_Colors2_(4, (uint8_t)(c1 + 1), (uint8_t)(c3 + 2));

            MOX::_mox_font_colors_offset = c2;
            s_colors font_colors_2 = *MISC::Font_Colors2_(4, c1, c3);

            MOX::_mox_font_colors_offset = (uint8_t)(c2 + 3);
            s_colors font_colors_3 = *MISC::Font_Colors2_(4, (uint8_t)(c1 + 1), (uint8_t)(c3 + 4));

            s_player* plr = &MOX::_player[i];
            if (plr->objectives == 100 /* 'd' */ && plr->eliminated == 0) {
                int16_t banner_x = (int16_t)((i % 2) * 294 + screen_x + 92);
                int16_t banner_y = (int16_t)((i / 2) * 36 + screen_y + 63);

                if (NETMOX::_net.players_hit_next_turn[i] == 0) {
                    MULTPLAY::Draw_Multi_Player_Banner_(i, banner_active_count, 1);
                    fonts::Set_Font_Style_(4, &font_colors_1);
                } else {
                    MULTPLAY::Draw_Fill_Seg_(i);
                    fonts::Set_Font_Style_(4, &font_colors_2);
                }

                fonts::Set_Font_Spacing_(1);
                int16_t text_y = (int16_t)(banner_y + (29 - fonts::Get_Font_Height_()) / 2);

                fonts::Print_(banner_x, text_y, plr->name);
                if (fonts::Get_String_Width_(plr->name) < 152) {
                    fonts::Print_Right_(banner_x + 205, text_y, HACCESS::Race_Name_String_(i));
                }
                banner_active_count++;
            }
        }

        int16_t chat_msg_count = (int16_t)CHAT::_chat_info->count;
        if (chat_msg_count > 0) {
            int16_t chat_x = screen_x + 24;
            int16_t chat_y = current_y + 14;

            for (int16_t i = 0; i < chat_msg_count; ++i) {
                uint8_t sender_id = CHAT::_chat_info->messages[i].player_id;
                char chat_buffer[100];

                if (sender_id < 8) {
                    uint8_t c1, c2, c3;
                    MULTPLAY::Get_Main_Palette_Base_Colors_(sender_id, &c1, &c2, &c3);
                    MOX::_mox_font_colors_offset = (uint8_t)(c2 + 1);
                    s_colors chat_colors = *MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, (uint8_t)(c1 + 1), (uint8_t)(c3 + 2));
                    fonts::Set_Font_Style_(MOX::_mp_screen->subscreen_mode, &chat_colors);
                    _sprintf(chat_buffer, s_chat_msg_fmt, MOX::_player[sender_id].name, CHAT::_chat_info->messages[i].text);
                } else {
                    MOX::_mox_font_colors_offset = 16;
                    s_colors gnn_colors = *MISC::Font_Colors2_(MOX::_mp_screen->subscreen_mode, 12, 23);
                    fonts::Set_Font_Style_(MOX::_mp_screen->subscreen_mode, &gnn_colors);
                    _sprintf(chat_buffer, s_gnn_fmt, CHAT::_chat_info->messages[i].text);
                }

                if (i == 0) {
                    graphics::Set_Window_(chat_x, chat_y, chat_x + 570, chat_y + 11);
                    video::Clip_On_();
                    MOX::_mp_screen->anim_chat_msg_bg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 40, MOX::_global_cache_seg);
                    animate::Draw_(screen_x, current_y, MOX::_mp_screen->anim_chat_msg_bg);
                    video::Clip_Off_();
                    graphics::Reset_Window_();
                }

                fonts::Print_(chat_x, chat_y, chat_buffer);
                chat_y += 12;
            }
        }

        char final_name_buffer[100];
        _sprintf(final_name_buffer, s_name_fmt, MOX::_player[MOX::_PLAYER_NUM].name);

        uint8_t pc1, pc2, pc3;
        MULTPLAY::Get_Main_Palette_Base_Colors_(MOX::_PLAYER_NUM, &pc1, &pc2, &pc3);
        // Using pc3 + 2 as suggested by the decompiler comment
        s_colors player_name_colors = *MISC::Font_Colors2_(4, (uint8_t)(pc1 + 1), (uint8_t)(pc3 + 2));
        MOX::_mox_font_colors_offset = pc2;
        fonts::Set_Font_Style_(MOX::_mp_screen->subscreen_mode, &player_name_colors);
        fonts::Print_(screen_x + 25, (int16_t)(MOX::_mp_screen->player_field_ids[2] + 190 + screen_y), final_name_buffer);

        if (MOX::_mp_screen->needs_full_redraw != 0) {
            video::Copy_Off_To_Back_();
            video::Set_Refresh_Stencil_();
        }

        MOX::_mp_screen->needs_full_redraw = 0;
    }

    void __cdecl Draw_Fill_Seg_(int16_t idx) {
        int16_t x;
        int16_t y;

        x = (idx % 2) * 294 + MOX::_mp_screen->screen_x + 40;
        y = (idx / 2) * 36 + MOX::_mp_screen->screen_y + 65;

        MOX::_mp_screen->anim_unk_3 = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 45, MOX::_global_cache_seg);

        animate::Draw_(x, y, MOX::_mp_screen->anim_unk_3);
    }

    void __cdecl Draw_Multi_Player_Banner_(int16_t player_idx, int16_t row_idx, int8_t is_alt_layout) {
        s_player* player = &MOX::_player[player_idx]; // s_player size 0xEA9
        int16_t screen_x = MOX::_mp_screen->screen_x;
        int16_t screen_y = MOX::_mp_screen->screen_y;
        int entry_id;
        int16_t x, y;

        if (is_alt_layout == 0) {
            entry_id = (uint8_t)player->race * 8 + (uint8_t)player->color + 46 /* 0x2E */;
            x = screen_x + 40 /* 0x28 */;
            y = screen_y + 65 /* 0x41 */ + row_idx * 36 /* 0x24 */;

            MOX::_mp_screen->player_banner_anims[player_idx] = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558D04, entry_id, MOX::_global_cache_seg);
            animate::Draw_(x, y, MOX::_mp_screen->player_banner_anims[player_idx]);
        } else {
            entry_id = (uint8_t)player->race * 8 + (uint8_t)player->color + 150 /* 0x96 */;
            // Horizontal position depends on parity of player_idx (left/right column)
            x = (int16_t)(player_idx % 2) * 294 /* 0x126 */ + screen_x + 40 /* 0x28 */;
            // Vertical position depends on player_idx pair (rows of 2)
            y = (int16_t)(player_idx / 2) * 36 /* 0x24 */ + screen_y + 65 /* 0x41 */;

            MOX::_mp_screen->player_banner_anims[player_idx] = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558D04, entry_id, MOX::_global_cache_seg);
            animate::Draw_(x, y, MOX::_mp_screen->player_banner_anims[player_idx]);
        }
    }

    void __cdecl Load_Choose_Net_Plyrs_Screen_() {
        RUSS::Mox_Update_();

        MOX::_mp_screen = &MULTPLAY::_g_mp_screen;
        memset(MOX::_mp_screen, 0, sizeof(s_mp_screen));

        _memcpy(MOX::_mp_screen->signature, s_Evan_Axelrad_0055AA48, 13);
        MOX::_mp_screen->needs_full_redraw = 1;

        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558D04, 0, MOX::_global_cache_seg);
        MOX::_mp_screen->anim_turn_screen_top = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558D04, 27 /* 0x1B */, MOX::_global_cache_seg);
        MOX::_mp_screen->anim_turn_screen_mid = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558D04, 28 /* 0x1C */, MOX::_global_cache_seg);
        MOX::_mp_screen->anim_turn_screen_bot = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558D04, 29 /* 0x1D */, MOX::_global_cache_seg);

        MOX::_mp_screen->anim_btn_begin = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558D04, 16 /* 0x10 */, MOX::_screen_seg);
        MOX::_mp_screen->anim_btn_cancel = (s_animation_header*)farload::Far_Reload_Next_(s_MULTIGM_LBX_00558D04, 257 /* 0x101 */, MOX::_screen_seg);

        MOX::_mp_screen->num_remaining_humans = HAROLD::Get_N_Remaining_Humans_();
    }

    void __cdecl Draw_Choose_Net_Plyrs_Screen_() {
        int16_t screen_x = MOX::_mp_screen->screen_x;
        int16_t screen_y = MOX::_mp_screen->screen_y;
        int16_t player_list_idx = 0;
        int16_t y_cursor = screen_y;

        if (MOX::_mp_screen->needs_full_redraw != 0) {
            video::Set_Page_Off_();
            MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, 0, MOX::_global_cache_seg);
            animate::Draw_(0, 0, MOX::_mp_screen->pic);
            video::Copy_Off_To_Back_();
            video::Set_Refresh_Stencil_();
            MOX::_mp_screen->needs_full_redraw = 0;
        }

        MOX::_mp_screen->anim_turn_screen_top = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, 27 /* 0x1B */, MOX::_global_cache_seg);
        MOX::_mp_screen->anim_frame_timer++;

        if (MOX::_mp_screen->anim_frame_timer != 0) {
            for (int16_t frame = 0; frame < (int16_t)MOX::_mp_screen->anim_frame_timer; ++frame) {
                animate::Set_Animation_Frame_(MOX::_mp_screen->anim_turn_screen_top, frame);
                animate::Draw_(screen_x, y_cursor, MOX::_mp_screen->anim_turn_screen_top);
            }
        }

        if (animate::Get_Animation_Frame_Count_(MOX::_mp_screen->anim_turn_screen_top) <= (int16_t)MOX::_mp_screen->anim_frame_timer) {
            MOX::_mp_screen->anim_frame_timer = 0;
        }

        y_cursor += animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_top);

        MOX::_mp_screen->anim_turn_screen_mid = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, 28 /* 0x1C */, MOX::_global_cache_seg);
        if (MOX::_mp_screen->num_remaining_humans - 1 > 0) {
            for (int16_t i = 0; i < MOX::_mp_screen->num_remaining_humans - 1; ++i) {
                animate::Draw_(screen_x, y_cursor, MOX::_mp_screen->anim_turn_screen_mid);
                y_cursor += animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_mid);
            }
        }

        MOX::_mp_screen->anim_turn_screen_bot = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, 29 /* 0x1D */, MOX::_global_cache_seg);
        animate::Draw_(screen_x, y_cursor, MOX::_mp_screen->anim_turn_screen_bot);

        int16_t y_offset_for_player_row = 0;
        for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
            s_player* plr = &MOX::_player[player_idx]; // size 0xEA9
            if (plr->objectives == 100 /* 'd' */ && plr->eliminated == 0) {
                s_colors local_colors_1;
                s_colors local_colors_2;
                s_colors local_colors_3;
                Get_Player_Colors_(plr->color, &local_colors_1, &local_colors_2, &local_colors_3);

                s_colors* font_colors;
                bool is_not_connected = false;

                if (MOX::_mp_screen->player_connected_flags[player_idx] == 0) {
                    is_not_connected = true;
                    if (MOX::_mp_screen->player_field_ids[player_idx] == MOX::_mp_screen->last_scan_id ||
                        MOX::_mp_screen->game_list_field_ids[player_idx] == MOX::_mp_screen->last_scan_id) {

                        uint8_t base_color = local_colors_1.colors[0];
                        uint8_t limit_low = base_color + 1;
                        uint8_t limit_high = base_color + 5;

                        for (int i = 1; i < 8; ++i) {
                            int val = (uint8_t)(local_colors_1.colors[i] + MOX::_mp_screen->sel_anim_frame);
                            if (val < (int)limit_low) val = limit_low;
                            if (val > (int)limit_high) val = limit_high;
                            local_colors_1.colors[i] = (uint8_t)val;
                        }

                        MOX::_mp_screen->sel_anim_frame += MOX::_mp_screen->sel_anim_active;
                        if (MOX::_mp_screen->sel_anim_frame >= 2) {
                            MOX::_mp_screen->sel_anim_frame = 2;
                            MOX::_mp_screen->sel_anim_active = -1;
                        } else if (MOX::_mp_screen->sel_anim_frame <= -1) {
                            MOX::_mp_screen->sel_anim_frame = -1;
                            MOX::_mp_screen->sel_anim_active = 1;
                        }
                    }
                    font_colors = &local_colors_1;
                } else {
                    font_colors = &local_colors_2;
                }

                fonts::Set_Font_Style_((uint16_t)(uint8_t)MOX::_mp_screen->subscreen_mode, font_colors);
                char* race_name = HACCESS::Race_Name_String_(player_idx);
                char buffer[100];
                _sprintf(buffer, s_fmt_player_race, plr->name, race_name);

                int16_t font_h = fonts::Get_Font_Height_();
                int16_t print_y = screen_y + 0x40 + y_offset_for_player_row + (29 - font_h) / 2;
                fonts::Print_(screen_x + 0x6A, print_y, buffer);

                if (is_not_connected) {
                    Draw_Multi_Player_Banner_(player_idx, (int16_t)player_list_idx, 0);
                }

                y_offset_for_player_row += 0x24;
                player_list_idx++;
            }
        }
    }

    void __cdecl Reload_Waiting_For_Joiners_Screen_() {
        fields::Clear_Fields_();
        MULTPLAY::_allow_chat_mode = 1;
        MULTPLAY::Reload_Generic_Net_Info_(15);
        MOX::_mp_screen->subscreen_mode = 4;
        MOX::_mp_screen->anim_btn_begin = (struct s_animation_header*)farload::Far_Reload_(MULTPLAY::s_MULTIGM_LBX, 16, MOX::_screen_seg);
    }

    void __cdecl Add_Waiting_For_Joiners_Field_() {
        MOX::_mp_screen->field_id_begin = fields::Add_Button_Field_(
            MOX::_mp_screen->screen_x + 158,
            MOX::_mp_screen->screen_y + 106,
            s__00551100,
            MOX::_mp_screen->anim_btn_begin,
            s_S_005569cc,
            40
        );
    }

    void __cdecl Add_Choose_Net_Plyrs_Fields_() {
        int16_t banner_idx = 0;
        int16_t y_offset = 0;

        fields::Clear_Fields_();

        for (int i = 0; i < 8; ++i) {
            MOX::_mp_screen->player_field_ids[i] = -1000;
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];

            if (plr->objectives == 100 && MOX::_mp_screen->player_connected_flags[i] == 0 && plr->eliminated == 0) {
                int16_t x1 = MOX::_mp_screen->screen_x + 106;
                int16_t y1 = MOX::_mp_screen->screen_y + 64 + y_offset;
                int16_t x2 = MOX::_mp_screen->screen_x + 435;
                int16_t y2 = y1 + 29;

                int16_t field_id = fields::Add_Hidden_Field_(x1, y1, x2, y2, s__00551100, 41);
                MOX::_mp_screen->player_field_ids[i] = field_id;

                MULTPLAY::Add_Banner_Field_(i, banner_idx);
                banner_idx++;
                y_offset += 36;
            } else {
                MOX::_mp_screen->player_field_ids[i] = -1000;
                MOX::_mp_screen->game_list_field_ids[i] = -1000;
            }
        }

        MOX::_mp_screen->anim_turn_screen_top = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 27, MOX::_global_cache_seg);

        int16_t h_mid = animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_mid);
        int16_t h_bot = animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_bot);
        int16_t h_top = animate::Get_Height_(MOX::_mp_screen->anim_turn_screen_top);

        int16_t combined_h = (int16_t)(h_mid * (MOX::_mp_screen->num_remaining_humans - 1) + h_bot + h_top);

        if (MOX::_mp_screen->is_host_flag != 0) {
            int16_t field_id = fields::Add_Button_Field_(
                MOX::_mp_screen->screen_x,
                MOX::_mp_screen->screen_y + combined_h + 1,
                s__00551100,
                MOX::_mp_screen->anim_btn_begin,
                s_S_005569cc,
                40
            );
            MOX::_mp_screen->field_id_begin = field_id;
        } else {
            MOX::_mp_screen->field_id_begin = -1000;
        }
    }

    void __cdecl Reload_Sending_Data_Info_() {
        uint8_t local_status;

        MULTPLAY::_allow_chat_mode = 0;
        local_status = 0;
        fields::Clear_Fields_();
        MULTPLAY::Reload_Generic_Net_Info_(26 /* 0x1a */);
        MOX::_mp_screen->msg_layout_alt = 0;
        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Generic_Net_Info_Screen_, &local_status, 11 /* 0xb */);
    }

    void __cdecl Draw_Wait_For_New_Joiners_Screen_() {
        s_colors font_colors;
        char buffer[64];

        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);

        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, 0, MOX::_global_cache_seg);
        animate::Draw_(0, 0, MOX::_mp_screen->pic);

        MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, MOX::_mp_screen->lbx_entry_num, MOX::_global_cache_seg);

        MOX::_mp_screen->anim_frame_timer++;
        if (MOX::_mp_screen->anim_frame_timer != 0) {
            for (int16_t i = 0; i < (int16_t)MOX::_mp_screen->anim_frame_timer; ++i) {
                animate::Set_Animation_Frame_(MOX::_mp_screen->bg_seg, (int)i);
                animate::Draw_(MOX::_mp_screen->screen_x, MOX::_mp_screen->screen_y, MOX::_mp_screen->bg_seg);
            }
        }

        if (animate::Get_Animation_Frame_Count_(MOX::_mp_screen->bg_seg) <= (int16_t)MOX::_mp_screen->anim_frame_timer) {
            MOX::_mp_screen->anim_frame_timer = 0;
        }

        MOX::_mox_font_colors_offset = 0xF2;
        s_colors* colors_ptr = MISC::Font_Colors2_(4, 0x9C, 0xF8);
        _memcpy(&font_colors, colors_ptr, sizeof(s_colors));

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            // Manual loop preserved from assembly
        }

        int16_t num_users = (int16_t)netcode::Net_Get_Users_(nullptr);
        int16_t msg_id = (num_users == 1) ? 0xF9 : 0xFA;
        const char* format = HAROLD::H_Message_(msg_id);
        
        // Note: In original binary, font_colors and buffer overlap by 4 bytes on stack
        _sprintf(buffer, format, (int)num_users);

        fonts::Set_Font_Style_(MOX::_mp_screen->subscreen_mode, &font_colors);
        fonts::Print_(MOX::_mp_screen->screen_x + 120 /* 0x78 */, MOX::_mp_screen->screen_y + 71 /* 0x47 */, buffer);
    }

    void __cdecl Add_Banner_Field_(int16_t player_idx, int16_t vertical_idx) {
        int16_t x1 = MOX::_mp_screen->screen_x + 40;
        int16_t y1 = MOX::_mp_screen->screen_y + 65 + vertical_idx * 36;

        uint8_t race = static_cast<uint8_t>(MOX::_player[player_idx].race);
        uint8_t color = static_cast<uint8_t>(MOX::_player[player_idx].color);

        int16_t lbx_entry = color + (race * 8) + 46;

        s_animation_header* banner_anim = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX_00558d04, lbx_entry, MOX::_global_cache_seg);

        int16_t width = animate::Get_Width_(banner_anim);
        int16_t height = animate::Get_Height_(banner_anim);

        int16_t field_id = fields::Add_Hidden_Field_(x1, y1, x1 + width, y1 + height, s__00551100, 41);

        MOX::_mp_screen->game_list_field_ids[player_idx] = field_id;
    }

    void __cdecl Reload_Wait_For_Race_Info_() {
        fields::Clear_Fields_();
        MULTPLAY::_allow_chat_mode = 1;
        MULTPLAY::Reload_Generic_Net_Info_(0x18);
    }

    void __cdecl Get_Player_Colors_(int16_t player_idx, s_colors* out_colors_primary, s_colors* out_colors_secondary, s_colors* out_colors_tertiary) {
        int16_t base_color_idx = player_idx * 6 + 112 /* 'p' */;
        int16_t mode = static_cast<int16_t>(MOX::_mp_screen->subscreen_mode);

        s_colors* colors1 = MISC::Font_Colors2_(mode, (uint8_t)base_color_idx, (uint8_t)(base_color_idx + 4 /* 't' */));
        *out_colors_primary = *colors1;

        s_colors* colors2 = MISC::Font_Colors2_(mode, (uint8_t)base_color_idx, (uint8_t)(base_color_idx + 2 /* 'r' */));
        *out_colors_secondary = *colors2;

        s_colors* colors3 = MISC::Font_Colors2_(mode, (uint8_t)base_color_idx, (uint8_t)(base_color_idx + 5 /* 'u' */));
        *out_colors_tertiary = *colors3;
    }

    void __cdecl Draw_Wait_For_Race_Info_Screen_() {
        s_colors font_colors;
        char msg_buffer[76];
        int16_t connected_count = 0;
        int16_t i;

        MOX::_mox_font_colors_offset = 0xF2;

        s_colors* colors_ptr = MISC::Font_Colors2_(4, 156 /* 0x9c */, 248 /* 0xf8 */);
        _memcpy(&font_colors, colors_ptr, sizeof(s_colors));

        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);

        MOX::_mp_screen->pic = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, 0, MOX::_global_cache_seg);
        animate::Draw_(0, 0, MOX::_mp_screen->pic);

        MOX::_mp_screen->anim_frame_timer++;

        MOX::_mp_screen->bg_seg = (s_animation_header*)Buffer_Reload_(s_MULTIGM_LBX, MOX::_mp_screen->lbx_entry_num, MOX::_global_cache_seg);

        if (MOX::_mp_screen->anim_frame_timer != 0) {
            for (i = 0; i < (int16_t)MOX::_mp_screen->anim_frame_timer; ++i) {
                animate::Set_Animation_Frame_(MOX::_mp_screen->bg_seg, (int32_t)i);
                animate::Draw_(MOX::_mp_screen->screen_x, MOX::_mp_screen->screen_y, MOX::_mp_screen->bg_seg);
            }
        }

        int16_t frame_count = animate::Get_Animation_Frame_Count_(MOX::_mp_screen->bg_seg);
        if (frame_count <= (int16_t)MOX::_mp_screen->anim_frame_timer) {
            MOX::_mp_screen->anim_frame_timer = 0;
        }

        for (i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (NETMOX::_net.player_connected_flags[i] != 0) {
                connected_count++;
            }
        }

        int32_t total_users = netcode::Net_Get_Users_(nullptr);
        const char* format = HAROLD::H_Message_(251 /* 0xfb */);
        _sprintf(msg_buffer, format, (int32_t)connected_count, total_users);

        fonts::Set_Font_Style_(MOX::_mp_screen->subscreen_mode, &font_colors);
        fonts::Print_(MOX::_mp_screen->screen_x + 118 /* 0x76 */, MOX::_mp_screen->screen_y + 73 /* 0x49 */, msg_buffer);
    }

}