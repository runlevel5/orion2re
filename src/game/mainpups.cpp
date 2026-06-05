#include "pch.h"

namespace MAINPUPS {
    char* _g_msg_ptr;

    s_animation_header* _anims[38];

    uint8_t _on_officer_screen_flag;

    int16_t _g_officer = -1;

    int16_t _officer_gains_level_state;

    char* _g_str_ptr;

    int16_t _last_scanned_field;
    int16_t _g_state;
    int8_t _selection_state_flag;
    uint8_t _g_using_colony_ship;
    int16_t _g_max;
    int16_t _g_min;
    char* _g_msg;
    int16_t _g_var;
    int16_t _g_temp_tax_rate;
    int16_t _field_state;
    s_colors _main_scr_font_colors = {{0xA0, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9}};

    uint8_t _g_special;
    int16_t _g_x;
    int16_t _g_y;
    int16_t _g_star_n = -1;
    int16_t _g_player_n = -1;
    int16_t _field_mode;
    int16_t _g_scanned_star = -1;
    int16_t _g_old_scanned_star = -1;
    uint8_t _no_flicker_between_mainpups_flag;
    int16_t _autofunction_ctr;
    int16_t _num_field_saves;
    s_fieldsav_data _stats_array[3];
    uint8_t _explode_flag = 1;
    int16_t _g_refresh_mode;
    MOX2::action_func_t _eric_saved_autofunction2;
    MOX2::action_func_t _eric_saved_autofunction;

    MOX2::action_func_t _temp_af;

    s_colony_combat_popup_data* _g_data_ptr;

    int16_t _mp_fields[26];

    void __cdecl Load_Colony_Fields_() {
        allocate::Push_Block_(MOX::_screen_seg);
        _anims[0] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0, MOX::_screen_seg);
    }

    void __cdecl Load_SLIDER_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);
        _anims[14] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x34, MOX::_screen_seg);
        _anims[34] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x4a, MOX::_screen_seg);
        _anims[33] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x4b, MOX::_screen_seg);
        _anims[35] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x4c, MOX::_screen_seg);
        _anims[4] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x4d, MOX::_screen_seg);
    }

    void __cdecl Load_Tax_Rate_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);
        _anims[27] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x3E, MOX::_screen_seg);
        _anims[28] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x3F, MOX::_screen_seg);
        _anims[29] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x40, MOX::_screen_seg);
        _anims[30] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x41, MOX::_screen_seg);
        _anims[31] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x42, MOX::_screen_seg);
        _anims[32] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x43, MOX::_screen_seg);
        _anims[35] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x44, MOX::_screen_seg);
        _anims[36] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x45, MOX::_screen_seg);
    }

    void __cdecl Load_Colony_Combat_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);

        _anims[17] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 53 /* 0x35 */, MOX::_screen_seg);
        _anims[15] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 54 /* 0x36 */, MOX::_screen_seg);
        _anims[16] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 55 /* 0x37 */, MOX::_screen_seg);
        _anims[18] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 60 /* 0x3c */, MOX::_screen_seg);
        _anims[3] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 78 /* 0x4e */, MOX::_screen_seg);
        _anims[19] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 79 /* 0x4f */, MOX::_screen_seg);
        _anims[20] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 80 /* 0x50 */, MOX::_screen_seg);
        _anims[21] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 81 /* 0x51 */, MOX::_screen_seg);
        _anims[9] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 82 /* 0x52 */, MOX::_screen_seg);

        _anims[22] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLONY2_LBX, 46 /* 0x2e */, MOX::_screen_seg);
        _anims[23] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLONY2_LBX, 47 /* 0x2f */, MOX::_screen_seg);
        _anims[24] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLONY2_LBX, 48 /* 0x30 */, MOX::_screen_seg);

        _anims[25] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 61 /* 0x3d */, MOX::_screen_seg);
        _anims[26] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 73 /* 0x49 */, MOX::_screen_seg);

        Load_Combat_Colony_Picture_To_Screen_Seg_(_g_data_ptr->colony_idx);
    }

    void __cdecl Load_System_Discovery_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);

        _anims[0] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0, MOX::_screen_seg);
        _anims[1] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 1, MOX::_screen_seg);
    }

    void __cdecl Get_Picture_Draw_XY_(s_animation_header* anim) {
        _g_x = 0x140 - (animate::Get_Width_(anim) / 2);
        _g_y = 0xF0 - (animate::Get_Height_(anim) / 2);
    }

    void __cdecl Do_Popup_Setup_(int16_t star_idx, s_animation_header* popup_anim) {
        if (MOX::_current_screen != 0) {
            _explode_flag = 0;
        }

        _g_refresh_mode = video::Get_Refresh_Mode_();
        video::Set_Refresh_Stencil_();
        Get_Picture_Draw_XY_(popup_anim);

        if (star_idx == -1) {
            star_idx = 0;
            if (MOX::_player[MOX::_PLAYER_NUM].home_planet_id > 0) {
                star_idx = MOX::_planet[MOX::_player[MOX::_PLAYER_NUM].home_planet_id].star_index;
            }
        }

        _g_star_n = star_idx;
        COLONY::_sys_disp_star_handle = star_idx;

        fields::Clear_Mouse_Buffer_();
        fields::Set_Input_Delay_(3);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        graphics::Reset_Window_();
        video::Clip_Off_();
        video::Copy_Off_To_Back_();
        video::Set_Page_Off_();

        if (_explode_flag != 0) {
            if (MAINSCR::Star_On_Screen_(star_idx)) {
                MAINSCR::Center_Map_(MOX::_star[star_idx].x, MOX::_star[star_idx].y);
            }
        }

        if (_autofunction_ctr == 0) {
            MOX2::Get_Auto_Function_(&_eric_saved_autofunction);
            MOX2::Execute_Auto_Function_(&_eric_saved_autofunction);

            bool skip_update = false;
            if (_explode_flag != 0) {
                if (MAINSCR::Star_On_Screen_(star_idx)) {
                    skip_update = true;
                }
            }

            if (!skip_update && _no_flicker_between_mainpups_flag == 0) {
                RUSS::Mox_Sync_Update_();
                COLONY::_full_draw = 1;
                MOX2::Execute_Auto_Function_(&_eric_saved_autofunction);
            }
        } else if (_autofunction_ctr == 1) {
            MOX2::Get_Auto_Function_(&_eric_saved_autofunction2);
            COLONY::_full_draw = 1;
            MOX2::Execute_Auto_Function_(&_eric_saved_autofunction);
            COLONY::_full_draw = 1;
            MOX2::Execute_Auto_Function_(&_eric_saved_autofunction2);
        }

        if (_autofunction_ctr == 0 || _autofunction_ctr == 1) {
            _autofunction_ctr++;
        }

        if (_explode_flag != 0) {
            int16_t draw_x, draw_y;
            MAINSCR::Get_Star_Draw_Coords_(star_idx, &draw_x, &draw_y);
            fields::Draw_Visible_Fields_();

            if (MOX::_settings.expanding_help != 0 && _num_field_saves < 1) {
                if (MOX::_cur_map_scale != MOX::_max_map_scale) {
                    if (!MAINSCR::Star_On_Screen_(star_idx)) {
                        MAINSCR::Center_Map_(MOX::_star[star_idx].x, MOX::_star[star_idx].y);
                        MAINSCR::Get_Star_Draw_Coords_(star_idx, &draw_x, &draw_y);
                    }
                }

                int16_t height = animate::Get_Height_(popup_anim);
                int16_t width = animate::Get_Width_(popup_anim);
                MAINSCR::Draw_Exploding_Plasma_Box_(draw_x, draw_y, _g_x, _g_y, width, height, -1);
            }
        }

        if (_no_flicker_between_mainpups_flag != 0) {
            _no_flicker_between_mainpups_flag = 0;
        }

        COLONY::_full_draw = 1;
    }

    void __cdecl Eric_Push_Fields_() {
        FIELDSAV::Save_Field_Stats_(&_stats_array[_num_field_saves]);
        _num_field_saves++;
    }

    void __cdecl Add_Tax_Rate_Fields_() {
        Eric_Push_Fields_();

        _mp_fields[25] = fields::Add_Button_Field_(
            0xed, 0xfe, ESTRINGS::E_Strings_(0xc), _anims[35], ESTRINGS::E_Strings_(0x87), 0x28
        );

        _mp_fields[19] = fields::Add_Multi_Button_Field_(
            0x93, 0xc6, ESTRINGS::E_Strings_(0xc), _anims[27], &_g_temp_tax_rate, 0, ESTRINGS::E_Strings_(0x8e), 0x29
        );

        _mp_fields[20] = fields::Add_Multi_Button_Field_(
            0xc1, 0xc6, ESTRINGS::E_Strings_(0xc), _anims[28], &_g_temp_tax_rate, 10, ESTRINGS::E_Strings_(0x93), 0x29
        );

        _mp_fields[21] = fields::Add_Multi_Button_Field_(
            0xea, 0xc6, ESTRINGS::E_Strings_(0xc), _anims[29], &_g_temp_tax_rate, 20, ESTRINGS::E_Strings_(0x95), 0x29
        );

        _mp_fields[22] = fields::Add_Multi_Button_Field_(
            0x113, 0xc6, ESTRINGS::E_Strings_(0xc), _anims[30], &_g_temp_tax_rate, 30, ESTRINGS::E_Strings_(0x97), 0x29
        );

        _mp_fields[23] = fields::Add_Multi_Button_Field_(
            0x13c, 0xc6, ESTRINGS::E_Strings_(0xc), _anims[31], &_g_temp_tax_rate, 40, ESTRINGS::E_Strings_(0x99), 0x29
        );

        _mp_fields[24] = fields::Add_Multi_Button_Field_(
            0x165, 0xc6, ESTRINGS::E_Strings_(0xc), _anims[32], &_g_temp_tax_rate, 50, ESTRINGS::E_Strings_(0x9b), 0x29
        );
    }

    void __cdecl Draw_Tax_Rate_Popup_() {
        if (COLONY::_full_draw != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MOX::_draw_main_screen_to_back = 1;
            Mainpups_Tested_Execute_Autofunction_();
            COLONY::_full_draw = 1;
            animate::Draw_(0, 0, _anims[36]);
            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }
    }

    void __cdecl Evaluate_Tax_Rate_Input_(int16_t* input_val, int16_t* scan_val, int16_t* exit_flag) {
        if (_mp_fields[25] == *input_val) {
            *exit_flag = 1;
            MOX::_player[MOX::_PLAYER_NUM].tax_rate = static_cast<uint8_t>(_g_temp_tax_rate);
        } else if (*input_val != 0) {
            MOX::_player[MOX::_PLAYER_NUM].tax_rate = static_cast<uint8_t>(_g_temp_tax_rate);
            COLCALC::Do_Player_Colony_Calculations_(MOX::_PLAYER_NUM);
            COLONY::_full_draw = 1;
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            COLONY::_full_draw = 1;
        }
    }

    void __cdecl Eric_Pop_Fields_() {
        if (_num_field_saves > 0) {
            _num_field_saves--;
            FIELDSAV::Restore_Field_Stats_(&_stats_array[_num_field_saves]);
        }
    }

    void __cdecl Do_Popup_Cleanup_() {
        Eric_Pop_Fields_();
        fields::Deactivate_Auto_Function_();

        if (_autofunction_ctr == 1) {
            MOX2::Put_Auto_Function_(&_eric_saved_autofunction);
            _autofunction_ctr--;
        } else if (_autofunction_ctr == 2) {
            MOX2::Put_Auto_Function_(&_eric_saved_autofunction2);
            _autofunction_ctr--;
        }

        video::Copy_Back_To_Off_();
        MOX::_FULL_DRAW_SCREEN_FLAG = 1;
        allocate::Pop_Block_(MOX::_screen_seg);
        ERIC::Set_Refresh_Mode_(_g_refresh_mode);
        MOX::_draw_system_display_without_button = 0;
        _explode_flag = 1;
        fields::Deactivate_Help_List_();
        MOX::_need_to_restore_help_list = 1;
    }

    void __cdecl Clear_System_Special_Slots_() {
        _anims[7] = nullptr;
        _anims[0] = nullptr;
        _g_msg_ptr = nullptr;
    }

    void __cdecl Clear_New_Officer_Slots_() {
        _anims[10] = nullptr;
        MAINPUPS::_g_msg_ptr = nullptr;
    }

    void __cdecl Update_Officer_Gains_Level_Fields_() {
        MAINPUPS::Eric_Push_Fields_();
        MAINPUPS::Add_Officer_Gains_Level_Fields_();
        ERICHELP::Set_Officer_Information_Popup_Help_List_();
    }

    /**
     * @address 0045b620
     */
    void __cdecl Add_Officer_Gains_Level_Fields_() {
        char* hotkey;
        int16_t sound;

        sound = 0;
        hotkey = ESTRINGS::E_Strings_(12 /* 0xc */);

        // Creates a full-screen (640x480) hidden input field
        MAINPUPS::_mp_fields[0] = fields::Add_Hidden_Field_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, hotkey, (int8_t)sound);
    }

    void __cdecl Build_Generic_Planet_Or_Colony_Description_String_Formatted_(int16_t planet_idx, char* buffer) {
        s_planet_data* p = &MOX::_planet[planet_idx];

        if (p->colony_index == -1) {
            if (p->planet_type != 3) {
                char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
                int len = sprintf(buffer, ESTRINGS::E_Strings_(0x56), planet_name);
                sprintf(buffer + len, ESTRINGS::E_Strings_(0x133));
                return;
            }

            s_potential_colony_info potential_info;
            COLCALC::Compute_Potential_Colony_Info_(&MOX::_player[MOX::_PLAYER_NUM], p, &potential_info);

            char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
            int len = sprintf(buffer, ESTRINGS::E_Strings_(0x52), s_0_0055110c, planet_name);
            buffer += len;

            char* climate_str = MOX::_planet_climate_string[p->climate];
            char* size_str = MOX::_planet_size_string[p->size];
            len = sprintf(buffer, ESTRINGS::E_Strings_(0x4D), s_0_0055110c, size_str, climate_str);
            buffer += len;

            char* mineral_str = MOX::_mineral_class_string[p->mineral_class];
            len = sprintf(buffer, ESTRINGS::E_Strings_(0x6F), s_0_0055110c, mineral_str);
            buffer += len;

            char* gravity_str = MOX::_planet_gravity_string[p->gravity_class];
            len = sprintf(buffer, ESTRINGS::E_Strings_(0x53), s_0_0055110c, gravity_str);
            buffer += len;

            if ((potential_info.food % 2) == 0) {
                len = sprintf(buffer, ESTRINGS::E_Strings_(0x65), s_0_0055110c, s_1_00551110, potential_info.food / 2);
                buffer += len;
            } else {
                len = sprintf(buffer, ESTRINGS::E_Strings_(0x64), s_0_0055110c, s_1_00551110, potential_info.food / 2, potential_info.food % 2);
                buffer += len;
            }

            len = sprintf(buffer, ESTRINGS::E_Strings_(0x6B), s_0_0055110c, s_1_00551110, potential_info.industry);
            buffer += len;

            len = sprintf(buffer, ESTRINGS::E_Strings_(0x74), s_0_0055110c, s_1_00551110, potential_info.research);
            buffer += len;

            len = sprintf(buffer, ESTRINGS::E_Strings_(0x6C), s_0_0055110c, s_1_00551110, potential_info.maintenance);
            buffer += len;

            len = sprintf(buffer, ESTRINGS::E_Strings_(0x79), s_0_0055110c, s_1_00551110, potential_info.worker_bonus);
            buffer += len;

            int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(p, MOX::_PLAYER_NUM);
            len = sprintf(buffer, ESTRINGS::E_Strings_(0x6D), s_0_0055110c, s_1_00551110, max_pop);
            buffer += len;
        } else {
            s_colony* c = &MOX::_colony[p->colony_index];
            char* race_name = MOX::_player[c->owner].race_name;
            char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);

            int len = sprintf(buffer, ESTRINGS::E_Strings_(0x49), s_0_0055110c, planet_name, race_name);
            buffer += len;

            char* climate_str = MOX::_planet_climate_string[static_cast<uint8_t>(c->climate)];
            char* size_str = MOX::_planet_size_string[p->size];
            len = sprintf(buffer, ESTRINGS::E_Strings_(0x4D), s_0_0055110c, size_str, climate_str);
            buffer += len;

            char* mineral_str = MOX::_mineral_class_string[p->mineral_class];
            len = sprintf(buffer, ESTRINGS::E_Strings_(0x6F), s_0_0055110c, mineral_str);
            buffer += len;

            char* gravity_str = MOX::_planet_gravity_string[p->gravity_class];
            len = sprintf(buffer, ESTRINGS::E_Strings_(0x53), s_0_0055110c, gravity_str);
            buffer += len;

            len = sprintf(buffer, ESTRINGS::E_Strings_(0x5E), s_0_0055110c, s_1_00551110, static_cast<uint8_t>(c->food2_per_farmer) / 2);
            buffer += len;

            len = sprintf(buffer, ESTRINGS::E_Strings_(0x5F), s_0_0055110c, s_1_00551110, static_cast<uint8_t>(c->industry_per_worker));
            buffer += len;

            len = sprintf(buffer, ESTRINGS::E_Strings_(0x60), s_0_0055110c, s_1_00551110, static_cast<uint8_t>(c->research_per_scientist));
            buffer += len;

            if (c->owner == MOX::_PLAYER_NUM) {
                len = sprintf(buffer, ESTRINGS::E_Strings_(0x70), s_0_0055110c, s_1_00551110, c->morale * 5);
                buffer += len;
            }

            int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(p, c->owner);
            len = sprintf(buffer, ESTRINGS::E_Strings_(0x71), s_0_0055110c, s_1_00551110, static_cast<uint8_t>(c->n_pops), max_pop);
            buffer += len;
        }

        if (p->planet_special != 0) {
            char* special_str = MOX::_planet_special_string[p->planet_special];
            sprintf(buffer, ESTRINGS::E_Strings_(0x50), s_0_0055110c, special_str);
        }
    }

    int16_t __cdecl Dragon_New_Star_Popup_(int16_t player_idx, int16_t star_idx) {
        int16_t ret_val;
        int16_t scanned_input;
        int16_t input_val;
        int16_t flag;
        s_colors colors;

        colors.colors[0] = 0x9f;
        colors.colors[1] = 0xa0;
        colors.colors[2] = 0xa1;
        colors.colors[3] = 0xa2;
        colors.colors[4] = 0;
        colors.colors[5] = 0;
        colors.colors[6] = 0;
        colors.colors[7] = 0;

        input_val = 0;
        flag = 0;

        _g_player_n = player_idx;
        _field_mode = 0;
        _last_scanned_field = -1;

        fonts::Set_Font_Style_Shadow_Down_(1, &colors);
        Load_Dragon_New_Star_Popup_();
        Do_Popup_Setup_(star_idx, _anims[37]);
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Dragon_New_Star_Popup_, 1);
        ERICHELP::Set_Dragon_New_Star_Popup_Help_List_();
        Update_Dragon_New_Star_Fields_();
        Draw_Dragon_New_Star_Popup_();
        RUSS::Mox_Sync_Update_();
        video::Set_Refresh_Differential_();

        do {
            input_val = fields::Get_Input_();
            scanned_input = fields::Scan_Input_();
            ret_val = Evaluate_Dragon_New_Star_Input_(&input_val, &scanned_input, &flag, star_idx);
            Draw_Dragon_New_Star_Popup_();
            RUSS::Mox_Sync_Update_();
        } while (flag == 0);

        Do_Popup_Cleanup_();
        fields::Deactivate_Help_List_();

        _g_player_n = -1;
        _g_star_n = -1;

        return ret_val;
    }

    void __cdecl Player_Attacked_Popup_(int16_t attacker_idx, int16_t defender_idx, int16_t star_idx, int16_t colony_idx) {
        char attacker_name[500];
        char location_name[500];
        char defender_name[500];
        char formatted_msg[500];

        MOX2::Get_Auto_Function_(&_eric_saved_autofunction);

        if (colony_idx == -1) {
            strcpy(location_name, MOX::_star[star_idx].name);
        } else {
            strcpy(location_name, HACCESS::Get_Planet_Name_(MOX::_colony[colony_idx].planet));
        }

        if (attacker_idx >= 0 && attacker_idx < 8) {
            if (MOX::_settings.language == 4) {
                sprintf(attacker_name, "%s", MOX::_player[attacker_idx].race_name);
            } else {
                sprintf(attacker_name, ESTRINGS::E_Strings_(0x3A), MOX::_player[attacker_idx].race_name);
            }
        } else {
            int16_t str_id;
            switch (attacker_idx) {
                case 8:  str_id = 0x1FF; break;
                case 9:  str_id = 0x202; break;
                case 10: str_id = 0x9F;  break;
                case 11: str_id = 0xA0;  break;
                case 12: str_id = 0xA1;  break;
                case 13: str_id = 0xA2;  break;
                case 14: str_id = 0xA3;  break;
                default: str_id = 0xA4;  break;
            }
            sprintf(attacker_name, ESTRINGS::E_Strings_(str_id));
        }

        if (defender_idx >= 0 && defender_idx < 8) {
            sprintf(defender_name, ESTRINGS::E_Strings_(0x46), MOX::_player[defender_idx].race_name);
        } else {
            sprintf(defender_name, ESTRINGS::E_Strings_(0xA5));
        }

        if (MOX::_settings.language == 4) {
            sprintf(
                formatted_msg,
                "SI COMBATTE NEL SISTEMA %s\n\n  Attaccante: %s\n  Difensore: %s",
                location_name,
                attacker_name,
                defender_name
            );
        } else {
            int16_t format_id = (colony_idx == -1) ? 0x31 : 0x30;
            sprintf(formatted_msg, ESTRINGS::E_Strings_(format_id), attacker_name, defender_name, location_name);
        }

        ERICHELP::Set_Player_Attacked_Popup_Help_List_();
        GENDRAW::Message_Box_Exploding_Star_(formatted_msg, star_idx);
        fields::Deactivate_Help_List_();
        fields::Deactivate_Auto_Function_();
        MOX2::Put_Auto_Function_(&_eric_saved_autofunction);
    }

    int16_t __cdecl Defense_Colony_Selection_Popup_(int16_t star_idx, int16_t player_idx) {
        int16_t selected_defense;
        s_animation_header* popup_buffer;
        int16_t scanned_field = 0;
        int16_t input_event = 0;
        int16_t leave_popup = 0;
        s_colors colors;

        colors.colors[0] = 0x9F;
        colors.colors[1] = 0xA0;
        colors.colors[2] = 0xA1;
        colors.colors[3] = 0xA2;
        colors.colors[4] = 0;
        colors.colors[5] = 0;
        colors.colors[6] = 0;
        colors.colors[7] = 0;

        _last_scanned_field = -1;
        COLONY::_sys_disp_star_handle = star_idx;
        MOX::_PLAYER_NUM = player_idx;
        _g_player_n = player_idx;

        fonts::Set_Font_Style_Shadow_Down_(1, &colors);
        Load_Colony_Defense_Selection_Popup_();

        popup_buffer = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
        Do_Popup_Setup_(star_idx, popup_buffer);

        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Defense_Selection_Popup_, 1);

        Update_Colony_Defense_Fields_();
        _last_scanned_field = _mp_fields[1];

        Draw_Defense_Selection_Popup_();
        RUSS::Mox_Sync_Update_();

        do {
            timer::Release_Time_(2);
            timer::Mark_Time_();

            input_event = fields::Get_Input_();
            scanned_field = fields::Scan_Input_();

            selected_defense = Evaluate_Defense_Selection_Input_(&input_event, &scanned_field, &leave_popup);

            Draw_Defense_Selection_Popup_();
            RUSS::Mox_Sync_Update_();
        } while (leave_popup == 0);

        Do_Popup_Cleanup_();

        return selected_defense;
    }

    /**
     * Adds UI fields for a slider control including the scroll bar, directional buttons, 
     * and a background hidden field.
     * 
     * @param min_val The lower limit and initial value of the slider.
     * @param max_val The upper limit of the slider range.
     * @param value_ptr Pointer to the variable that will store the slider's state.
     */
    void __cdecl Add_Slider_Fields_(int16_t min_val, int16_t max_val, int16_t* value_ptr) {
        char* tooltip_generic = ESTRINGS::E_Strings_(12);

        // Main scroll bar field
        _mp_fields[11] = fields::Add_Scroll_Field_(
            _g_x + 37 /* 0x25 */,
            _g_y + 149 /* 0x95 */,
            min_val,
            max_val + 1,
            min_val,
            max_val,
            207 /* 0xcf */,
            13 /* 0xd */,
            value_ptr,
            tooltip_generic,
            0
        );

        // Slider navigation buttons
        _mp_fields[10] = fields::Add_Button_Field_(
            _g_x + 15 /* 0xf */,
            _g_y + 154 /* 0x9a */,
            tooltip_generic,
            _anims[34],
            ESTRINGS::E_Strings_(137 /* 0x89 */),
            40 /* 0x28 */
        );

        _mp_fields[9] = fields::Add_Button_Field_(
            _g_x + 256 /* 0x100 */,
            _g_y + 154 /* 0x9a */,
            tooltip_generic,
            _anims[33],
            ESTRINGS::E_Strings_(11 /* 0xb */),
            40 /* 0x28 */
        );

        _mp_fields[2] = fields::Add_Button_Field_(
            _g_x + 15 /* 0xf */,
            _g_y + 174 /* 0xae */,
            tooltip_generic,
            _anims[35],
            ESTRINGS::E_Strings_(135 /* 0x87 */),
            40 /* 0x28 */
        );

        _mp_fields[3] = fields::Add_Button_Field_(
            _g_x + 208 /* 0xd0 */,
            _g_y + 174 /* 0xae */,
            tooltip_generic,
            _anims[4],
            ESTRINGS::E_Strings_(12 /* 0xc */),
            40 /* 0x28 */
        );

        // Hidden field covering the screen to block underlying interactions
        _mp_fields[0] = fields::Add_Hidden_Field_(
            0,
            0,
            639 /* 0x27f */,
            479 /* 0x1df */,
            tooltip_generic,
            0
        );
    }

    void __cdecl Load_Slider_Fields_() {
        Eric_Push_Fields_();
        Add_Slider_Fields_(_g_min, _g_max, &_g_var);
    }

    int16_t __cdecl Slider_Popup_(int16_t min, int16_t max, int16_t var, char* msg, int16_t param_5) {
        int16_t input = 0;
        _last_scanned_field = -1;
        int16_t done = 0;
        int16_t scanned_field;

        _g_min = min;
        _g_max = max;
        _g_msg = msg;
        _g_var = var;

        Load_SLIDER_Popup_();
        Do_Popup_Setup_(param_5, _anims[14]);
        remap::Calculate_Picture_Remap_Colors_(_anims[14]);
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Slider_Popup_, 1);
        Load_Slider_Fields_();
        Draw_Slider_Popup_();
        RUSS::Mox_Sync_Update_();

        do {
            input = fields::Get_Input_();
            scanned_field = fields::Scan_Input_();
            Draw_Slider_Popup_();
            RUSS::Mox_Sync_Update_();
            Evaluate_Slider_Input_(&input, &scanned_field, &done);
        } while (done == 0);

        Do_Popup_Cleanup_();
        return _g_var;
    }

    int16_t __cdecl New_Colony_Selection_Popup_(int16_t star_idx, uint8_t using_colony_ship) {
        s_colors colors;
        colors.colors[0] = 0x9f;
        colors.colors[1] = 0xa0;
        colors.colors[2] = 0xa1;
        colors.colors[3] = 0xa2;
        colors.colors[4] = 0;
        colors.colors[5] = 0;
        colors.colors[6] = 0;
        colors.colors[7] = 0;

        int16_t input = 0;
        int16_t done = 0;
        int16_t scan_input;

        _g_using_colony_ship = using_colony_ship;
        COLONY::_sys_disp_star_handle = star_idx;
        _last_scanned_field = -1;

        fonts::Set_Font_Style_Shadow_Down_(1, &colors);
        Load_Colony_Fields_();
        Set_G_Special_Value_(star_idx);
        Clear_System_Special_Slots_();

        s_animation_header* popup_bg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
        Do_Popup_Setup_(star_idx, popup_bg);

        fields::Deactivate_Help_List_();
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Selection_Popup_, 1);

        Update_Selection_Fields_();
        Draw_Selection_Popup_();
        RUSS::Mox_Sync_Update_();

        int16_t result;
        do {
            timer::Release_Time_(2);
            timer::Mark_Time_();
            input = fields::Get_Input_();
            scan_input = fields::Scan_Input_();
            result = Evaluate_Selection_Input_(&input, &scan_input, &done);
            Draw_Selection_Popup_();
            RUSS::Mox_Sync_Update_();
        } while (done == 0);

        if (namestar::Can_Change_Star_Name_(star_idx) != 0 &&
            MOX::_settings.animations_on == 0 &&
            result != -1) {
            
            int16_t y = namestar::Star_Name_Popup_Screen_Center_Y_();
            int16_t x = namestar::Star_Name_Popup_Screen_Center_X_();
            namestar::Change_Star_Name_Popup_(x, y, star_idx, 0);
        }

        Do_Popup_Cleanup_();

        return result;
    }

    int16_t __cdecl New_Outpost_Selection_Popup_(int16_t star_id) {
        _selection_state_flag = 1;
        int16_t result = New_Colony_Selection_Popup_(star_id, 0);
        _selection_state_flag = 0;
        return result;
    }

    int16_t __cdecl Drop_Troops_Selection_Popup_(int16_t target_idx) {
        _selection_state_flag = 2;
        int16_t result = New_Colony_Selection_Popup_(target_idx, 0);
        _selection_state_flag = 0;
        return result;
    }

    bool __cdecl Random_New_Officer_Popup_(int16_t player_idx) {
        int16_t star_idx = MOX::_planet[ERIC::Player_Capitol_World_(player_idx)].star_index;

        s_colors colors;
        colors.colors[0] = 0x9F;
        colors.colors[1] = 0xA0;
        colors.colors[2] = 0xA1;
        colors.colors[3] = 0xA2;
        colors.colors[4] = 0;
        colors.colors[5] = 0;
        colors.colors[6] = 0;
        colors.colors[7] = 0;

        _g_player_n = player_idx;

        if (_on_officer_screen_flag == 0) {
            _g_officer = MOX::_player[player_idx].officer_for_hire;
        }

        int16_t cost = static_cast<int16_t>(OFFICER::Calculate_Officer_Cost_(player_idx, _g_officer));

        _field_state = 2;
        if (cost <= MOX::_player[player_idx].bc) {
            _field_state = 0;
        }

        if (star_idx == -1) {
            int16_t colony_idx = ERIC::Pick_Random_Colony_(player_idx);
            star_idx = MOX::_planet[MOX::_colony[colony_idx].planet].star_index;
        }

        COLONY::_sys_disp_star_handle = star_idx;

        fonts::Set_Font_Style_Shadow_Down_(1, &colors);
        Clear_New_Officer_Slots_();

        if (_on_officer_screen_flag != 0) {
            Load_New_Officer_Popup_For_Officer_Screen_();
        } else {
            Load_New_Officer_Popup_();
        }

        Do_Popup_Setup_(star_idx, _anims[10]);

        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Random_New_Officer_Popup_, 1);
        Update_Random_New_Officer_Fields_();

        _last_scanned_field = _mp_fields[1];

        video::Set_Refresh_Stencil_();
        video::Set_Page_Off_();

        Draw_Random_New_Officer_Popup_();
        RUSS::Mox_Sync_Update_();

        bool done = false;
        bool accepted = false;

        do {
            int16_t input = fields::Get_Input_();
            int16_t scan_input = fields::Scan_Input_();

            OFFICER::Wrapper_For_Officer_Popup_Help_Fields_(input, _g_officer);

            if (_field_state == 0) {
                if (input == _mp_fields[7]) {
                    int16_t hire_cost = static_cast<int16_t>(OFFICER::Calculate_Officer_Cost_(MOX::_PLAYER_NUM, _g_officer));
                    if (hire_cost <= MOX::_player[_g_player_n].bc) {
                        _field_state = 3;
                        accepted = true;
                    } else {
                        _field_state = 2;
                        accepted = false;
                    }

                    Eric_Pop_Fields_();
                    Update_Random_New_Officer_Fields_();

                    if (MOX::_settings.game_difficulty <= 0 && _on_officer_screen_flag != 0) {
                        COLONY::_full_draw = 0;
                    } else {
                        done = true;
                    }

                    fields::Clear_Mouse_Buffer_();
                    fields::Set_Input_Delay_(3);
                } else if (input == _mp_fields[8]) {
                    _field_state = 1;
                    accepted = false;

                    Eric_Pop_Fields_();
                    Update_Random_New_Officer_Fields_();

                    if (MOX::_settings.game_difficulty <= 0 && _on_officer_screen_flag != 0) {
                        COLONY::_full_draw = 0;
                    } else {
                        _field_state = 0;
                        done = true;
                    }

                    fields::Clear_Mouse_Buffer_();
                    fields::Set_Input_Delay_(3);
                } else if (input == -_mp_fields[8]) {
                    int16_t info_cost = static_cast<int16_t>(OFFICER::Calculate_Officer_Cost_(_g_player_n, _g_officer));
                    s_leader_data* leader = &MOX::_leaders[_g_officer];
                    char* level_name = MOX::_officer_level_names[leader->level];
                    char buffer[500];

                    sprintf(buffer, ESTRINGS::E_Strings_(0x1fb), level_name, leader, info_cost);
                    TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x1bf), buffer);

                    fields::Deactivate_Auto_Function_();
                    fields::Assign_Auto_Function_(Draw_Random_New_Officer_Popup_, 1);
                    Eric_Pop_Fields_();
                    Update_Random_New_Officer_Fields_();

                    COLONY::_full_draw = 1;
                } else if (input == -_mp_fields[7]) {
                    int16_t info_cost = static_cast<int16_t>(OFFICER::Calculate_Officer_Cost_(_g_player_n, _g_officer));
                    s_leader_data* leader = &MOX::_leaders[_g_officer];
                    char* level_name = MOX::_officer_level_names[leader->level];
                    char buffer[500];

                    sprintf(buffer, ESTRINGS::E_Strings_(0x1fa), level_name, leader, info_cost);
                    TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x141), buffer);

                    fields::Deactivate_Auto_Function_();
                    fields::Assign_Auto_Function_(Draw_Random_New_Officer_Popup_, 1);
                    Eric_Pop_Fields_();
                    Update_Random_New_Officer_Fields_();

                    COLONY::_full_draw = 1;
                }
            } else if (input > 0) {
                done = true;
            }

            Draw_Random_New_Officer_Popup_();
            RUSS::Mox_Sync_Update_();

        } while (!done);

        _g_player_n = -1;
        Do_Popup_Cleanup_();

        return accepted;
    }

    void __cdecl New_System_Discovery_Popup_(int16_t star_idx, uint8_t special_flag) {
        int16_t input_val = 0;
        int16_t scanned_val = 0;
        int16_t leader_idx = 0;
        s_colors colors;

        colors.colors[0] = 0x9f;
        colors.colors[1] = 0xa0;
        colors.colors[2] = 0xa1;
        colors.colors[3] = 0xa2;
        colors.colors[4] = 0;
        colors.colors[5] = 0;
        colors.colors[6] = 0;
        colors.colors[7] = 0;

        COLONY::_sys_disp_star_handle = star_idx;

        fonts::Set_Font_Style_Shadow_Down_(1, &colors);
        Load_System_Discovery_Popup_();

        _g_special = special_flag;
        Clear_System_Special_Slots_();

        s_animation_header* anim = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
        Do_Popup_Setup_(star_idx, anim);

        video::Copy_Off_To_Back_();
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Discovery_Popup_, 1);
        fields::Deactivate_Help_List_();
        Update_Discovery_Fields_();

        _last_scanned_field = _mp_fields[1];

        Draw_Discovery_Popup_();
        RUSS::Mox_Sync_Update_();

        do {
            timer::Release_Time_(2);
            timer::Mark_Time_();

            input_val = fields::Get_Input_();
            scanned_val = fields::Scan_Input_();

            if (_g_special == 0) {
                Evaluate_Discovery_Input_(&input_val, &scanned_val);
            } else {
                if (_g_special == 8) {
                    int16_t prev_x = _g_x;
                    int16_t prev_y = _g_y;
                    
                    _g_special = 0;

                    uint8_t can_join = OFFICER::Marooned_Hero_At_Star_Can_Join_(
                        star_idx, &leader_idx, MOX::_PLAYER_NUM);

                    if (can_join == 0) {
                        Marooned_Officer_Fails_Popup_(star_idx, leader_idx);
                    } else {
                        Marooned_Officer_Joins_Popup_(star_idx, leader_idx);
                    }

                    input_val = 0;
                    COLONY::_full_draw = 1;
                    _g_x = prev_x;
                    _g_y = prev_y;

                    fields::Deactivate_Auto_Function_();
                    fields::Assign_Auto_Function_(Draw_Discovery_Popup_, 1);
                } else if (input_val != 0) {
                    input_val = 0;
                    _g_special = 0;
                    COLONY::_full_draw = 1;
                }

                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    COLONY::_full_draw = 1;
                }
            }

            Draw_Discovery_Popup_();
            RUSS::Mox_Sync_Update_();

        } while (input_val < 1 || _g_special != 0);

        Do_Popup_Cleanup_();
    }

    void __cdecl Officer_Gains_Level_Popup_(int16_t star_handle, int16_t officer_idx) {
        s_colors colors;
        int16_t input;

        colors.colors[0] = 0x9f;
        colors.colors[1] = 0xa0;
        colors.colors[2] = 0xa1;
        colors.colors[3] = 0xa2;
        colors.colors[4] = 0;
        colors.colors[5] = 0;
        colors.colors[6] = 0;
        colors.colors[7] = 0;

        COLONY::_sys_disp_star_handle = star_handle;
        fonts::Set_Font_Style_Shadow_Down_(1, &colors);
        _g_officer = officer_idx;

        Clear_New_Officer_Slots_();
        Load_Officer_Gains_Level_Popup_();
        Do_Popup_Setup_(star_handle, _anims[10]);

        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Officer_Gains_Level_Popup_, 1);

        Update_Officer_Gains_Level_Fields_();
        _last_scanned_field = _mp_fields[1];

        video::Set_Page_Off_();
        Draw_Officer_Gains_Level_Popup_();
        RUSS::Mox_Sync_Update_();

        do {
            input = fields::Get_Input_();
            fields::Scan_Input_();
            Draw_Officer_Gains_Level_Popup_();
            RUSS::Mox_Sync_Update_();
        } while (input == 0);

        Do_Popup_Cleanup_();
    }

    const char* __cdecl System_Special_Desc_(int16_t desc_id) {
        if (_g_msg_ptr == nullptr) {
            _g_msg_ptr = (char*)farload::Far_Reload_Next_Data_(
                ESTRINGS::E_Strings_(0x170),
                desc_id,
                MOX::_screen_seg,
                0,
                1,
                0x258 /* 600 */
            );
        }
        return _g_msg_ptr;
    }

    s_animation_header* __cdecl System_Special_Anim_(int16_t star_idx, int16_t anim_type) {
        if (_anims[7] == nullptr) {
            if (anim_type == 9) {
                int16_t monster_idx = RUSS::Star_To_Monster_(star_idx);
                _anims[7] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, monster_idx + 15, MOX::_screen_seg);
            } else {
                _anims[7] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, anim_type + 4, MOX::_screen_seg);
            }
        }

        if (_anims[0] == nullptr) {
            _anims[0] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0, MOX::_screen_seg);
        }

        return _anims[7];
    }

    void __cdecl Draw_Special_Info_Popup_(int16_t x, int16_t y, s_animation_header* anim, const char* title, const char* text) {
        s_colors font_colors;
        font_colors.colors[0] = 0x6E; /* 'n' */
        font_colors.colors[1] = 0x71; /* 'q' */
        font_colors.colors[2] = 0x73; /* 's' */
        font_colors.colors[3] = 0;
        font_colors.colors[4] = 0;
        font_colors.colors[5] = 0;
        font_colors.colors[6] = 0;
        font_colors.colors[7] = 0;

        int16_t h = animate::Get_Height_(_anims[0]);
        int16_t w = animate::Get_Width_(_anims[0]);

        graphics::Fill_(x + 3, y + 3, x + w - 3, y + h - 3, 0);

        animate::Draw_(x, y, _anims[0]);
        animate::Draw_(x + 4, y + 4, anim);

        fonts::Set_Font_Style_(1, &font_colors);
        fonts::Print_Centered_(x + 0xDE, y + 10, title);

        BILL::_Print_Paragraph_(x + 0x78, y + 0x17, 0xCD, text, 2);
    }

    int16_t __cdecl Colony_Selection_Popup_(
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombardment_result,
        int8_t has_stellar_converter
    ) {
        s_colony_combat_popup_data data;

        _g_data_ptr = &data;

        MOX2::Get_Auto_Function_(&_temp_af);
        MOX2::Execute_Auto_Function_(&_temp_af);

        RUSS::Mox_Sync_Update_();

        Update_CCP_Display_Data_Invasion_(
            _g_data_ptr,
            attacker,
            defender,
            random_seed,
            colony_idx,
            bombardment_result,
            has_stellar_converter
        );

        return Colony_Combat_Popup_(&data, 0);
    }

    void __cdecl Update_CCP_Display_Data_Invasion_(
        s_colony_combat_popup_data* ccp_data,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombardment_result,
        int8_t has_stellar_converter)
    {
        ccp_data->bombardment_result = bombardment_result;
        ccp_data->attacker = attacker;
        ccp_data->colony_idx = colony_idx;
        ccp_data->defender = defender;
        ccp_data->random_seed = random_seed;
        ccp_data->has_stellar_converter = has_stellar_converter;
        ccp_data->attacker_player_id = static_cast<int8_t>(attacker->player_id);
        ccp_data->defender_player_id = static_cast<int8_t>(defender->player_id);

        for (int i = 0; i < 4; ++i) {
            ccp_data->attacker_unit_counts[i] = attacker->unit_counts[i];
            ccp_data->defender_unit_counts[i] = defender->unit_counts[i];
        }

        ccp_data->n_pops = static_cast<int16_t>(static_cast<uint8_t>(MOX::_colony[colony_idx].n_pops));
        ccp_data->n_bldgs = COLONY::N_Bldgs_(colony_idx);
        ccp_data->best_shield_bldg = Get_Colony_Best_Shield_Bldg_(colony_idx);
    }

    int16_t __cdecl Colony_Combat_Popup_(s_colony_combat_popup_data* popup_data, int16_t popup_mode) {
        bool invasion_started = false;
        int16_t done = 0;
        int16_t var_1C = 0;
        int16_t scan_val = 0;
        int16_t input_val = 0;
        bool invasion_done = false;
        int16_t bomb_hit_val1;
        int16_t bomb_hit_val2;
        s_colors colors;
        char buffer[500];

        colors.colors[0] = 0x9F;
        colors.colors[1] = 0xA0;
        colors.colors[2] = 0xA1;
        colors.colors[3] = 0xA2;
        colors.colors[4] = 0;
        colors.colors[5] = 0;
        colors.colors[6] = 0;
        colors.colors[7] = 0;

        COLGCBT::_g_ctr = 0;
        MAINPUPS::_last_scanned_field = -1;
        _g_data_ptr = popup_data;

        fonts::Load_Palette_(0, 0, 0xFF);
        palette::Clear_Palette_Changes_(0, 0xFF);
        fonts::Set_Font_Style_Shadow_Down_(1, &colors);

        MAINPUPS::Load_Colony_Combat_Popup_();
        FLCQUEUE::Free_Flic_Queue_And_Pointers_();

        s_animation_header* anim = COLONY::C_Anims_(0xD);
        remap::Calculate_Picture_Remap_Colors_(anim);

        int16_t star_idx = MOX::_planet[MOX::_colony[MAINPUPS::_g_data_ptr->colony_idx].planet].star_index;
        MAINPUPS::Do_Popup_Setup_(star_idx, _anims[3]);

        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(MAINPUPS::Draw_Colony_Combat_Popup_, 1);

        MAINPUPS::Update_Colony_Combat_Fields_(popup_mode, &var_1C);
        MAINPUPS::Draw_Colony_Combat_Popup_();
        RUSS::Mox_Sync_Update_();
        video::Set_Refresh_Stencil_();

        if (popup_mode == 2) {
            invasion_started = true;
        }
        else if (popup_mode == 1) {
            MAINPUPS::Setup_To_Apply_Bomb_Hits_(MAINPUPS::_g_data_ptr->bomb, &bomb_hit_val1, &bomb_hit_val2);
        }

        random::Set_Random_Seed_(MAINPUPS::_g_data_ptr->random_seed);
        MAINPUPS::_g_state = popup_mode;

        while (true) {
            COLONY::_full_draw = 1;
            MAINPUPS::Eric_Pop_Fields_();
            MAINPUPS::Update_Colony_Combat_Fields_(popup_mode, &var_1C);

            input_val = fields::Get_Input_();
            scan_val = fields::Scan_Input_();

            if (popup_mode == 2) {
                if (MAINPUPS::_g_data_ptr->n_pops <= 0) {
                    invasion_done = true;
                }
                while (true) {
                    bool is_combat_active = ((uint8_t)MAINPUPS::_g_data_ptr->attacker->current_troop_type_idx < 4) &&
                        ((uint8_t)MAINPUPS::_g_data_ptr->defender->current_troop_type_idx < 4);

                    if (is_combat_active && invasion_started) {
                        if (COLGCBT::_g_ctr % 10 != 0 && !invasion_done) {
                            break;
                        }
                        invasion::Ground_Combat_Round_(MAINPUPS::_g_data_ptr->attacker, MAINPUPS::_g_data_ptr->defender);
                        MAINPUPS::Update_CCP_Display_Data_Invasion_(
                            MAINPUPS::_g_data_ptr,
                            MAINPUPS::_g_data_ptr->attacker,
                            MAINPUPS::_g_data_ptr->defender,
                            random::Get_Random_Seed_(),
                            MAINPUPS::_g_data_ptr->colony_idx,
                            MAINPUPS::_g_data_ptr->bombardment_result,
                            MAINPUPS::_g_data_ptr->has_stellar_converter
                        );
                        COLONY::_full_draw = 1;
                    }
                    else {
                        invasion_started = true;
                        if (input_val != 0) {
                            if (!invasion_done) {
                                input_val = 0;
                                invasion_done = true;
                            }
                        }
                    }

                    if (!invasion_done) {
                        break;
                    }

                    if ((uint8_t)MAINPUPS::_g_data_ptr->attacker->current_troop_type_idx >= 4 ||
                        (uint8_t)MAINPUPS::_g_data_ptr->defender->current_troop_type_idx >= 4) {
                        break;
                    }
                }
            }
            else if (popup_mode == 1) {
                MAINPUPS::Apply_Next_Bomb_Hit_(MAINPUPS::_g_data_ptr->bomb, &var_1C, &bomb_hit_val1, &bomb_hit_val2);
            }

            int16_t result = MAINPUPS::Evaluate_Colony_Combat_Input_(&input_val, &scan_val, &done, &var_1C);
            MAINPUPS::Draw_Colony_Combat_Popup_();
            RUSS::Mox_Sync_Update_();

            if (popup_mode == 2) {
                s_invasion_side_info* attacker = MAINPUPS::_g_data_ptr->attacker;
                s_invasion_side_info* defender = MAINPUPS::_g_data_ptr->defender;

                if ((uint8_t)attacker->current_troop_type_idx >= 4 || (uint8_t)defender->current_troop_type_idx >= 4) {
                    int16_t str_id;
                    int16_t player_id = attacker->player_id;

                    if ((uint8_t)attacker->current_troop_type_idx < 4) {
                        str_id = 0x3F;
                    }
                    else {
                        str_id = 0x3C;
                    }

                    char* race_name = MOX::_player[player_id].race_name;
                    char* planet_name = HACCESS::Get_Planet_Name_(MOX::_colony[MAINPUPS::_g_data_ptr->colony_idx].planet);

                    char* fmt = ESTRINGS::E_Strings_(str_id);
                    sprintf(buffer, fmt, planet_name, race_name);
                    GENDRAW::Help_(buffer);

                    COLONY::_full_draw = 1;
                    done = 1;
                }
            }

            COLGCBT::_g_ctr++;
            timer::Release_Time_(1);

            if (done != 0) {
                MAINPUPS::Do_Popup_Cleanup_();
                FLCQUEUE::Allocate_Flic_Queue_And_Pointers_();
                sound::Stop_All_Sounds_();
                if (result == 3) {
                    ERIC::Fast_Fade_Out_();
                }
                return result;
            }
        }
    }

    void __cdecl Update_Colony_Combat_Fields_(int16_t popup_mode, int16_t* combat_state_ptr) {
        Eric_Push_Fields_();
        Add_Colony_Combat_Fields_(popup_mode, combat_state_ptr);
    }

    void __cdecl Update_CCP_Display_Data_Bombing_(
        s_colony_combat_popup_data* ccp_data,
        s_combat_bomb_result* bomb,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombing_result,
        int8_t has_stellar_converter
    ) {
        ccp_data->bomb = bomb;

        Update_CCP_Display_Data_Invasion_(
            _g_data_ptr,
            attacker,
            defender,
            random_seed,
            colony_idx,
            bombing_result,
            has_stellar_converter
        );

        ccp_data->defender_unit_counts[1] -= _g_data_ptr->bomb->infantry_killed;
        ccp_data->defender_unit_counts[0] -= _g_data_ptr->bomb->tanks_killed;
    }

    int16_t __cdecl Colony_Bombing_Popup_(
        s_combat_bomb_result* bomb,
        s_invasion_side_info* attacker,
        s_invasion_side_info* defender,
        int32_t random_seed,
        int16_t colony_idx,
        int16_t bombing_result,
        int8_t has_stellar_converter)
    {
        s_colony_combat_popup_data local_30;

        _g_data_ptr = &local_30;

        Update_CCP_Display_Data_Bombing_(
            &local_30,
            bomb,
            attacker,
            defender,
            random_seed,
            colony_idx,
            bombing_result,
            has_stellar_converter
        );

        _explode_flag = 0;

        return Colony_Combat_Popup_(&local_30, 1);
    }

    /**
     * @address 0045a290
     */
    int8_t __cdecl Player_Has_Colony_Or_Outpost_At_Star_(int16_t player_idx, int16_t star_idx) {
        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx != -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                if (colony_idx != -1) {
                    if (MOX::_colony[colony_idx].owner == player_idx) {
                        return 1;
                    }
                }
            }
        }

        return 0;
    }

    void __cdecl Apply_Next_Bomb_Hit_(s_combat_bomb_result *bomb, int16_t *state, int16_t *bomb_hits, int16_t *bio_hits) {
        s_colony *colony = &MOX::_colony[bomb->colony_idx];

        if (*state == 1 || *state == 3 || COLGCBT::_g_ctr % 5 != 0) {
            return;
        }

        do {
            if (*bio_hits + *bomb_hits <= 0) {
                if (*state == 2) {
                    *bio_hits = 0;
                    _g_data_ptr->bombardment_result = 0;
                }
                *state = 3;
            } else {
                if (*bio_hits == 0) {
                    int32_t hits_processed = BOMB::Resolve_Bomb_Hit_(colony, bomb, *bomb_hits);
                    if (hits_processed == 0) {
                        *bomb_hits = 0;
                    } else {
                        int loops = hits_processed;
                        if (*bomb_hits == hits_processed) {
                            *bomb_hits -= hits_processed;
                            _g_data_ptr->bombardment_result -= hits_processed;
                            loops = 1;
                        } else {
                            *bomb_hits -= hits_processed;
                            _g_data_ptr->bombardment_result -= hits_processed;
                        }

                        if (loops > 5) {
                            loops = 5;
                        }

                        for (int i = loops; i > 0; --i) {
                            int combat_planet_w = animate::Get_Width_(MOX::_combat_planet_seg);
                            int anim25_w = animate::Get_Width_(_anims[25]);
                            
                            int diff = (combat_planet_w / 2) - anim25_w;
                            int abs_diff = diff < 0 ? -diff : diff;

                            int anim3_h = animate::Get_Height_(_anims[3]);
                            int base_y = (anim3_h + _g_y * 2 - 30) / 2;
                            int rand_y = base_y + random::Random_(abs_diff * 2) - abs_diff;

                            int anim3_w = animate::Get_Width_(_anims[3]);
                            int base_x = (anim3_w + _g_x * 2 - 30) / 2;
                            int rand_x = base_x + random::Random_(abs_diff * 2) - abs_diff;

                            FLCQUEUE::Add_Flic_To_End_Of_Queue_(rand_x, rand_y, _anims[25], 1, 0);
                            ERICNET::Mox_Play_Sound_(24 /* 0x18 */);
                        }
                    }
                } else {
                    BOMB::Resolve_Biological_Hit_(colony, bomb);
                    (*bio_hits)--;
                    _g_data_ptr->bombardment_result--;

                    int anim3_h = animate::Get_Height_(_anims[3]);
                    int base_y = (anim3_h + _g_y * 2 - 30) / 2;
                    int rand_y = base_y + random::Random_(40) - 20;

                    int anim3_w = animate::Get_Width_(_anims[3]);
                    int base_x = (anim3_w + _g_x * 2 - 30) / 2;
                    int rand_x = base_x + random::Random_(40) - 20;

                    FLCQUEUE::Add_Flic_To_End_Of_Queue_(rand_x, rand_y, _anims[26], 1, 0);
                    ERICNET::Mox_Play_Sound_(32 /* 0x20 */);
                }

                Update_CCP_Display_Data_Bombing_(
                    _g_data_ptr,
                    _g_data_ptr->bomb,
                    _g_data_ptr->attacker,
                    _g_data_ptr->defender,
                    _g_data_ptr->random_seed,
                    _g_data_ptr->colony_idx,
                    _g_data_ptr->bombardment_result,
                    _g_data_ptr->has_stellar_converter
                );
                COLONY::_full_draw = 1;
            }
        } while (*state == 2);
    }

    void __cdecl Setup_To_Apply_Bomb_Hits_(s_combat_bomb_result* result, int16_t* p_bomb_hits_remaining, int16_t* p_bio_hits_remaining) {
        *p_bomb_hits_remaining = result->bomb_hits_remaining;
        *p_bio_hits_remaining = result->bio_hits_remaining;

        result->infantry_killed = 0;
        result->tanks_killed = 0;
        result->population_killed = 0;

        memset(result->buildings_destroyed, 0, sizeof(result->buildings_destroyed));

        result->bio_pop_killed = 0;
        result->damage_occurred = 0;
        result->colony_wiped_out = 0;
    }

    void __cdecl Update_Discovery_Fields_() {
        Eric_Push_Fields_();
        COLONY::Clear_Sys_Disp_Fields_();

        MOX::_draw_system_display_without_button = 1;

        SYS::Wrapper_For_Informational_System_Fields_(
            static_cast<uint32_t>(COLONY::_sys_disp_star_handle),
            _g_x,
            _g_y,
            13,
            47,
            nullptr
        );

        char* hotkey = ESTRINGS::E_Strings_(12);
        _mp_fields[0] = fields::Add_Hidden_Field_(0, 0, 639, 479, hotkey, 0);
    }

    void __cdecl Draw_Discovery_Popup_() {
        char msg_buffer[500];
        s_star_data* star_ptr;
        const char* format;
        s_animation_header* anim;

        video::Set_Page_Off_();

        if (COLONY::_full_draw != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MAINPUPS::Mainpups_Tested_Execute_Autofunction_();
            video::Copy_Off_To_Back_();
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            COLONY::_full_draw = 0;
        }

        star_ptr = &MOX::_star[COLONY::_sys_disp_star_handle];
        format = ESTRINGS::E_Strings_(456 /* 0x1c8 */);
        sprintf(msg_buffer, format, star_ptr->name);

        anim = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 73 /* 0x49 */, MOX::_global_cache_seg);
        MAINPUPS::Get_Picture_Draw_XY_(anim);

        SYS::Draw_System_Display_Popup_(
            (short)COLONY::_sys_disp_star_handle,
            1,
            msg_buffer,
            MAINPUPS::_g_x,
            MAINPUPS::_g_y,
            0,
            0,
            13 /* 0xd */,
            47 /* 0x2f */
        );

        if (MAINPUPS::_g_special != 0 && MAINPUPS::_g_special != 8) {
            SYS::Draw_System_Special_Popup_(
                static_cast<int16_t>(MAINPUPS::_g_x + 6),
                static_cast<int16_t>(MAINPUPS::_g_y + 76 /* 0x4c */),
                (int16_t)MAINPUPS::_g_special,
                MAINPUPS::_g_star_n
            );
        }
    }

    void __cdecl Update_Selection_Fields_() {
        Eric_Push_Fields_();
        COLONY::Clear_Sys_Disp_Fields_();
        Add_Selection_Fields_();
        ERICHELP::Set_Planet_Selection_Popup_Help_List_();
    }

    int16_t __cdecl Evaluate_Selection_Input_(int16_t* selected_field, int16_t* field_data, int16_t* refresh_flag) {
        int16_t saved_x = _g_x;
        int16_t saved_y = _g_y;
        int16_t selection_result = 0;

        if (_g_special == 0) {
            if (*selected_field == _mp_fields[2]) {
                selection_result = -1;
                *refresh_flag = 1;
            } else {
                selection_result = Check_System_Display_Fields_Selection_(selected_field, field_data, refresh_flag);
            }
        } else if (_g_special == 8) {
            int16_t marooned_hero_idx;
            uint8_t can_join;

            _g_special = 0;
            can_join = OFFICER::Marooned_Hero_At_Star_Can_Join_(_g_star_n, &marooned_hero_idx, MOX::_PLAYER_NUM);
            if (can_join == 0) {
                Marooned_Officer_Fails_Popup_(_g_star_n, marooned_hero_idx);
            } else {
                Marooned_Officer_Joins_Popup_(_g_star_n, marooned_hero_idx);
            }
            _g_x = saved_x;
            COLONY::_full_draw = 1;
            _g_y = saved_y;
            Eric_Pop_Fields_();
            Update_Selection_Fields_();
            video::Copy_Back_To_Off_();
            Draw_Selection_Popup_();
            fields::Assign_Auto_Function_(Draw_Selection_Popup_, 1);
            ERICHELP::Set_New_System_Discovery_Popup_Help_List_();
        } else if (*selected_field != 0) {
            _g_special = 0;
            COLONY::_full_draw = 1;
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            COLONY::_full_draw = 1;
        }

        return selection_result;
    }

    void __cdecl Load_New_Officer_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);

        _anims[10] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x30, MOX::_screen_seg);
        _anims[12] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x31, MOX::_screen_seg);
        _anims[13] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x32, MOX::_screen_seg);
        _anims[11] = OFFICER::Load_Officer_Picture_(_g_player_n, _g_officer, MOX::_screen_seg, 0, 0);

        OFFICER::Load_Officer_Skill_Icons_(MOX::_screen_seg);
    }

    void __cdecl Load_New_Officer_Popup_For_Officer_Screen_() {
        allocate::Push_Block_(MOX::_screen_seg);
        _anims[10] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x39, MOX::_screen_seg);
        _anims[12] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x3A, MOX::_screen_seg);
        _anims[13] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x3B, MOX::_screen_seg);
        _anims[11] = OFFICER::Load_Officer_Picture_(_g_player_n, _g_officer, MOX::_screen_seg, 0, 0);
        if (_on_officer_screen_flag == 0) {
            OFFICER::Load_Officer_Skill_Icons_(MOX::_screen_seg);
        }
    }

    void __cdecl Draw_Selection_Popup_() {
        char popup_text[500];
        bool draw_selection_popup = false;
        int16_t string_id = 0;

        fonts::Set_Font_Style_(4, &_main_scr_font_colors);

        if (COLONY::_full_draw != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            Mainpups_Tested_Execute_Autofunction_();
            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }

        switch (_selection_state_flag) {
        case 0:
            string_id = (_g_using_colony_ship != 0) ? 0x1ce : 0x1cd;
            draw_selection_popup = true;
            break;

        case 1:
            string_id = 0x1cb;
            draw_selection_popup = true;
            break;

        case 2:
            string_id = 0x1cf;
            draw_selection_popup = true;
            break;

        case 3:
            string_id = 0x1cc;
            draw_selection_popup = true;
            break;

        default:
            break;
        }

        if (draw_selection_popup) {
            s_star_data* selected_star = &MOX::_star[_g_star_n];
            const char* format = ESTRINGS::E_Strings_(string_id);
            sprintf(popup_text, format, selected_star->name);

            SYS::Draw_System_Display_Popup_(
                (short)COLONY::_sys_disp_star_handle,
                1,
                popup_text,
                _g_x,
                _g_y,
                0,
                0,
                0x0d,
                0x2f);
        }

        fonts::Set_Font_Style_(1, &_main_scr_font_colors);

        if (_g_special != 0 && _g_special != 8) {
            SYS::Draw_System_Special_Popup_(
                _g_x,
                static_cast<int16_t>(_g_y + 0x32),
                _g_special,
                _g_star_n);
        }
    }

    void __cdecl Update_Random_New_Officer_Fields_() {
        char* button_text;
        char* base_text;
        s_animation_header* animation;
        char hotkey = '(';
        int16_t hidden_field_id;

        MAINPUPS::Eric_Push_Fields_();
        fields::Clear_Mouse_Buffer_();
        fields::Set_Input_Delay_(3);

        if (MAINPUPS::_field_state == 0) {
            hotkey = '(';
            button_text = ESTRINGS::E_Strings_(0x1b4);
            animation = MAINPUPS::_anims[12];
            base_text = ESTRINGS::E_Strings_(0xc);
            MAINPUPS::_mp_fields[8] = fields::Add_Button_Field_(MAINPUPS::_g_x + 0x2c, MAINPUPS::_g_y + 0xe2, base_text, animation, button_text, hotkey);

            hotkey = '(';
            button_text = ESTRINGS::E_Strings_(0x13c);
            animation = MAINPUPS::_anims[13];
            base_text = ESTRINGS::E_Strings_(0xc);
            MAINPUPS::_mp_fields[7] = fields::Add_Button_Field_(MAINPUPS::_g_x + 0xaf, MAINPUPS::_g_y + 0xe2, base_text, animation, button_text, hotkey);

            MAINPUPS::_mp_fields[0] = -1000;
        } else {
            hotkey = '(';
            if (MAINPUPS::_field_state == 3) {
                button_text = ESTRINGS::E_Strings_(0x180);
                animation = MAINPUPS::_anims[12];
                base_text = ESTRINGS::E_Strings_(0xc);
                MAINPUPS::_mp_fields[8] = fields::Add_Button_Field_(MAINPUPS::_g_x + 0x2c, MAINPUPS::_g_y + 0xe2, base_text, animation, button_text, hotkey);

                hotkey = '(';
                button_text = ESTRINGS::E_Strings_(0x231);
                animation = MAINPUPS::_anims[13];
                base_text = ESTRINGS::E_Strings_(0xc);
                MAINPUPS::_mp_fields[7] = fields::Add_Button_Field_(MAINPUPS::_g_x + 0xaf, MAINPUPS::_g_y + 0xe2, base_text, animation, button_text, hotkey);
            } else {
                button_text = ESTRINGS::E_Strings_(0x180);
                animation = MAINPUPS::_anims[12];
                base_text = ESTRINGS::E_Strings_(0xc);
                MAINPUPS::_mp_fields[8] = fields::Add_Button_Field_(MAINPUPS::_g_x + 0x2c, MAINPUPS::_g_y + 0xe2, base_text, animation, button_text, hotkey);
                MAINPUPS::_mp_fields[7] = -1000;
            }

            hidden_field_id = 0;
            base_text = ESTRINGS::E_Strings_(0xc);
            MAINPUPS::_mp_fields[0] = fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, base_text, (int8_t)hidden_field_id);
        }

        OFFICER::Wrapper_To_Add_Officer_Popup_Help_Fields_(MAINPUPS::_g_x, MAINPUPS::_g_y + 0x35, MAINPUPS::_g_officer);
    }

    void __cdecl Evaluate_Slider_Input_(int16_t* input, int16_t* scanned_field, int16_t* done) {
        (void)scanned_field;

        const int16_t value = *input;

        if (value == _mp_fields[3]) {
            *done = 1;
        }

        if (value == _mp_fields[2]) {
            *done = 1;
            _g_var = -1;
        }

        if (value == _mp_fields[9]) {
            ++_g_var;
        }

        if (value == _mp_fields[10]) {
            --_g_var;
        }
    }

    void __cdecl Mainpups_Tested_Execute_Autofunction_() {
        if (MAINPUPS::_autofunction_ctr == 1) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 1;
            COLONY::_full_draw = 1;
            MOX::_dont_draw_main_screen_to_back = 1;
            MOX2::Execute_Auto_Function_(&MAINPUPS::_eric_saved_autofunction);
        } else if (MAINPUPS::_autofunction_ctr == 2) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 1;
            COLONY::_full_draw = 1;
            MAINPUPS::_autofunction_ctr = 0;
            MOX::_dont_draw_main_screen_to_back = 1;
            MOX2::Execute_Auto_Function_(&MAINPUPS::_eric_saved_autofunction);

            MOX::_FULL_DRAW_SCREEN_FLAG = 1;
            COLONY::_full_draw = 1;
            MOX::_dont_draw_main_screen_to_back = 1;
            MOX2::Execute_Auto_Function_(&MAINPUPS::_eric_saved_autofunction2);
            MAINPUPS::_autofunction_ctr = 2;
        }

        MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        MOX::_dont_draw_main_screen_to_back = 0;
    }

    void __cdecl Draw_Random_New_Officer_Popup_() {
        char message_buffer[500];
        char tech_buffer[500];
        char* leader_name;
        char* format_string;
        int16_t officer_cost;
        int16_t maintenance_cost;

        if (COLONY::_full_draw == 0 && MOX::_FULL_DRAW_SCREEN_FLAG == 0) {
            return;
        }

        MOX::_draw_main_screen_to_back = 1;
        Mainpups_Tested_Execute_Autofunction_();

        animate::Draw_(MAINPUPS::_g_x, MAINPUPS::_g_y, _anims[10]);
        OFFICER::Wrapper_For_Colony_Officer_Popup_Data_(MAINPUPS::_g_x,
                                                        MAINPUPS::_g_y + 0x35,
                                                        MAINPUPS::_g_officer);
        animate::Draw_(MAINPUPS::_g_x + 0x0d, MAINPUPS::_g_y + 0x37, _anims[11]);

        if (MAINPUPS::_on_officer_screen_flag != 0) {
            ERIC::Print_Paragraph_Centered_(MAINPUPS::_g_x + 0x0e,
                                            MAINPUPS::_g_y + 0x95,
                                            0x119,
                                            0x3e,
                                            MAINPUPS::_g_str_ptr);
        } else {
            const int16_t field_state = MAINPUPS::_field_state;

            if (field_state == 0) {
                MAINPUPS::Leader_Technology_String_(MAINPUPS::_g_player_n,
                                                    MAINPUPS::_g_officer,
                                                    tech_buffer);
                maintenance_cost = OFFICER::Officer_Maintenance_(MAINPUPS::_g_player_n,
                                                                 MAINPUPS::_g_officer);
                officer_cost = (int16_t)OFFICER::Calculate_Officer_Cost_(MAINPUPS::_g_player_n,
                                                              MAINPUPS::_g_officer);
                leader_name = OFFICER::Leader_Name_(MAINPUPS::_g_officer, 0);
                format_string = ESTRINGS::E_Strings_(2);
                sprintf(message_buffer,
                        format_string,
                        leader_name,
                        officer_cost,
                        maintenance_cost,
                        tech_buffer);
            } else if (field_state == 1) {
                officer_cost = (int16_t)OFFICER::Calculate_Officer_Cost_(MAINPUPS::_g_player_n,
                                                              MAINPUPS::_g_officer);
                leader_name = OFFICER::Leader_Name_(MAINPUPS::_g_officer, 0);
                format_string = ESTRINGS::E_Strings_(6);
                sprintf(message_buffer, format_string, leader_name, officer_cost);
            } else if (field_state == 2) {
                officer_cost = (int16_t)OFFICER::Calculate_Officer_Cost_(MAINPUPS::_g_player_n,
                                                              MAINPUPS::_g_officer);
                leader_name = OFFICER::Leader_Name_(MAINPUPS::_g_officer, 0);
                format_string = ESTRINGS::E_Strings_(5);
                sprintf(message_buffer, format_string, leader_name, officer_cost);
            } else if (field_state == 3) {
                officer_cost = (int16_t)OFFICER::Calculate_Officer_Cost_(MAINPUPS::_g_player_n,
                                                              MAINPUPS::_g_officer);
                leader_name = OFFICER::Leader_Name_(MAINPUPS::_g_officer, 0);
                format_string = ESTRINGS::E_Strings_(4);
                sprintf(message_buffer, format_string, leader_name, officer_cost);
            }

            BILL::Squeeze_Paragraph_Centered_(MAINPUPS::_g_x + 0x0e,
                                              MAINPUPS::_g_y + 0x95,
                                              0x119,
                                              0x3e,
                                              message_buffer,
                                              2);
        }

        leader_name = OFFICER::Leader_Name_(MAINPUPS::_g_officer, 1);
        ERIC::Print_Paragraph_Centered_(MAINPUPS::_g_x + 0x0b,
                                        MAINPUPS::_g_y + 0x0c,
                                        0x11d,
                                        0x21,
                                        leader_name);
        video::Copy_Off_To_Back_();
    }

    void __cdecl Evaluate_Discovery_Input_(int16_t* input_1, int16_t* input_2) {
        Do_Evaluate_Discovery_Input_(input_1, input_2);
    }

    /**
     * @address 0045bc00
     */
    void __cdecl Do_Evaluate_Discovery_Input_(int16_t* mouse_x, int16_t* mouse_y) {
        int16_t out_val1;
        int16_t out_val2;
        int16_t scanned_ship_idx;
        int16_t out_val4;
        int16_t scanned_icon_idx;

        // Scans the display fields at the given mouse coordinates.
        // Arg order derived from disassembly: *param_2 (Y), *param_1 (X)
        MAINSCR::Scan_System_Display_Fields_(*mouse_y, *mouse_x, MOX::_planet_buttons, 
                                             &out_val1, &out_val2, &scanned_ship_idx, 
                                             &out_val4, &scanned_icon_idx);

        MOX::_system_display_ship_scanned = scanned_ship_idx;

        if (scanned_icon_idx > -1) {
            MOX::_system_icon_scanned = scanned_icon_idx;
        }
    }

    /**
     * @address 0045b470
     */
    void __cdecl Marooned_Officer_Fails_Popup_(int16_t star_idx, int16_t officer_idx) {
        _officer_gains_level_state = 2;
        Officer_Gains_Level_Popup_(star_idx, officer_idx);
        _officer_gains_level_state = 0;
    }

    /**
     * @address 0045b440
     */
    void __cdecl Marooned_Officer_Joins_Popup_(int16_t star_idx, int16_t officer_idx) {
        _officer_gains_level_state = 1;
        Officer_Gains_Level_Popup_(star_idx, officer_idx);
        _officer_gains_level_state = 0;
    }

    void __cdecl Load_Dragon_New_Star_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);

        _anims[4] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x46, MOX::_screen_seg);
        _anims[35] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x47, MOX::_screen_seg);
        _anims[37] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x48, MOX::_screen_seg);

        COLONY::Load_Galaxy_Map_Anims_();
    }

    /**
     * @address 0045fa20
     */
    void __cdecl Update_Dragon_New_Star_Fields_() {
        Eric_Push_Fields_();
        COLONY::Clear_Galaxy_Map_Fields_();

        _mp_fields[2] = fields::Add_Button_Field_(
            186,
            313,
            ESTRINGS::E_Strings_(12),
            _anims[4],
            ESTRINGS::E_Strings_(135),
            40
        );

        if (_field_mode == 0) {
            _mp_fields[25] = fields::Add_Button_Field_(
                381,
                313,
                ESTRINGS::E_Strings_(12),
                _anims[35],
                ESTRINGS::E_Strings_(561),
                40
            );
        } else {
            _mp_fields[25] = -1000;
        }

        MOVEBOX::Add_Galaxy_Map_Fields_2_(
            302,
            156,
            148,
            94,
            0,
            0,
            0,
            0,
            0,
            &COLONY::_galaxy_map_name_field,
            MOX::_galaxy_map_star_field,
            &MOX::_galaxy_map_grid_field
        );
    }

    void __cdecl Load_Colony_Defense_Selection_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);

        _anims[8] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 2, MOX::_screen_seg);
        _anims[9] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x2f /* 47 */, MOX::_screen_seg);
    }

    /**
     * @address 0045c610
     */
    void __cdecl Update_Colony_Defense_Fields_() {
        Eric_Push_Fields_();
        COLONY::Clear_Sys_Disp_Fields_();

        SYS::Wrapper_For_Informational_System_Fields_(
            static_cast<uint32_t>(COLONY::_sys_disp_star_handle),
            _g_x,
            _g_y,
            0xd,
            0x2f,
            &_mp_fields[6]
        );
    }

    void __cdecl Draw_Defense_Selection_Popup_() {
        char buffer[500];

        if (COLONY::_full_draw != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MAINPUPS::Mainpups_Tested_Execute_Autofunction_();
            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }

        s_star_data* star = &MOX::_star[COLONY::_sys_disp_star_handle];
        char* race_name = MOX::_player[MAINPUPS::_g_player_n].race_name;
        char* format = ESTRINGS::E_Strings_(0x45);

        sprintf(buffer, format, race_name, star->name);

        SYS::Draw_System_Display_Popup_(
            (int16_t)COLONY::_sys_disp_star_handle,
            1,
            buffer,
            MAINPUPS::_g_x,
            MAINPUPS::_g_y,
            0,
            0,
            13,
            47
        );
    }

    /**
     * @address 0045c710
     */
    int16_t __cdecl Evaluate_Defense_Selection_Input_(int16_t* field_idx, int16_t* mouse_status, int16_t* selection_made) {
        if (*field_idx == _mp_fields[6]) {
            *selection_made = 1;
            return -1;
        }

        return Check_System_Display_Fields_Defense_Selection_(field_idx, mouse_status, selection_made);
    }

    void __cdecl Draw_Officer_Gains_Level_Popup_() {
        char message_buffer[500];

        if (COLONY::_full_draw == 0 && MOX::_FULL_DRAW_SCREEN_FLAG == 0) {
            return;
        }

        COLONY::_full_draw = 1;
        MOX::_dont_draw_main_screen_to_back = 1;
        MOX::_FULL_DRAW_SCREEN_FLAG = 1;

        Mainpups_Tested_Execute_Autofunction_();
        Get_Picture_Draw_XY_(_anims[10]);

        animate::Draw_(_g_x, _g_y, _anims[10]);
        OFFICER::Wrapper_For_Colony_Officer_Popup_Data_(_g_x, _g_y + 53, _g_officer);
        animate::Draw_(_g_x + 13, _g_y + 55, _anims[11]);

        if (_officer_gains_level_state == 0) {
            char* name = OFFICER::Leader_Name_(_g_officer, 0);
            char* format = ESTRINGS::E_Strings_(1);
            sprintf(message_buffer, format, name);
        }
        else if (_officer_gains_level_state == 1) {
            char* name = OFFICER::Leader_Name_(_g_officer, 0);
            char* format = ESTRINGS::E_Strings_(584);
            sprintf(message_buffer, format, name);
        }
        else if (_officer_gains_level_state == 2) {
            char* name = OFFICER::Leader_Name_(_g_officer, 0);
            char* format = ESTRINGS::E_Strings_(585);
            sprintf(message_buffer, format, name);
        }
        // Preserve the original binary behavior for unexpected state values by
        // intentionally leaving message_buffer untouched.

        ERIC::Print_Paragraph_Centered_(_g_x + 14, _g_y + 149, 281, 62, message_buffer);

        char* leader_full_name = OFFICER::Leader_Name_(_g_officer, 1);
        ERIC::Print_Paragraph_Centered_(_g_x + 11, _g_y + 12, 285, 33, leader_full_name);

        video::Copy_Off_To_Back_();
        COLONY::_full_draw = 0;
    }

    int16_t __cdecl Get_Colony_Best_Shield_Bldg_(int16_t colony_idx) {
        int16_t best_shield_id = -1;

        if (MOX::_colony[colony_idx].buildings[28] != 0) {
            best_shield_id = 28;
        }

        if (MOX::_colony[colony_idx].buildings[24] != 0) {
            best_shield_id = 24;
        }

        if (MOX::_colony[colony_idx].buildings[23] != 0) {
            best_shield_id = 23;
        }

        return best_shield_id;
    }

    int16_t __cdecl Check_System_Display_Fields_Selection_(int16_t* mouse_x, int16_t* mouse_y, int16_t* selection_confirmed) {
        int16_t planet_scanned;
        int16_t icon_scanned;
        int16_t ship_scanned;
        int16_t dummy_a;
        int16_t dummy_8;

        auto redraw_selection_popup = []() {
            Eric_Pop_Fields_();
            Update_Selection_Fields_();
            video::Copy_Back_To_Off_();
            Draw_Selection_Popup_();
            fields::Assign_Auto_Function_(Draw_Selection_Popup_, 1);
        };

        auto fail_selection = [&]() -> int16_t {
            redraw_selection_popup();
            return -1;
        };

        auto fail_with_help = [&](int16_t help_string_id, bool mark_full_draw, bool redraw_twice) -> int16_t {
            GENDRAW::Help_(ESTRINGS::E_Strings_(help_string_id));
            if (mark_full_draw) {
                COLONY::_full_draw = 1;
            }
            if (redraw_twice) {
                redraw_selection_popup();
            }
            return fail_selection();
        };

        int16_t scan_res = MAINSCR::Scan_System_Display_Fields_(
            *mouse_y, *mouse_x, MOX::_planet_buttons,
            &dummy_a, &planet_scanned, &ship_scanned, &dummy_8, &icon_scanned
        );

        MOX::_system_display_ship_scanned = ship_scanned;
        if (icon_scanned > -1) {
            MOX::_system_icon_scanned = icon_scanned;
        }

        if (scan_res == 0 && planet_scanned > -1) {
            int16_t planet_idx = HACCESS::System_Planet_Scanned_To_Planet_Id_((int16_t)COLONY::_sys_disp_star_handle, planet_scanned);

            if (_selection_state_flag == 0) {
                if (MOX::_planet[planet_idx].planet_type != 3) {
                    return fail_with_help(0x236, true, true);
                }

                int16_t col_idx = MOX::_planet[planet_idx].colony_index;
                if (col_idx != -1 && MOX::_colony[col_idx].outpost_flag == 0) {
                    return fail_with_help(0x236, true, true);
                }
            }

            if (_selection_state_flag == 1) {
                if (MOX::_planet[planet_idx].colony_index != -1) {
                    return fail_with_help(0x236, true, true);
                }
            }

            if (_selection_state_flag == 2) {
                int16_t col_idx = MOX::_planet[planet_idx].colony_index;
                if (col_idx == -1) {
                    return fail_with_help(0x23b, true, true);
                }

                if (MOX::_colony[col_idx].outpost_flag != 0) {
                    return fail_with_help(0x23b, true, true);
                }

                if (MOX::_colony[col_idx].owner != MOX::_PLAYER_NUM) {
                    return fail_with_help(0x23b, true, true);
                }
            }

            if (_selection_state_flag == 3) {
                if (MOX::_planet[planet_idx].planet_type == 3) {
                    return fail_with_help(0x235, false, false);
                }

                *selection_confirmed = 1;
                return planet_idx;
            }

            if (_selection_state_flag != 0) {
                *selection_confirmed = 1;
                return planet_idx;
            }

            if (New_Colony_Confirmation_Box_(planet_idx) != 0) {
                *selection_confirmed = 1;
                return planet_idx;
            }

            return fail_selection();
        }

        return -1;
    }

    void __cdecl Draw_Slider_Popup_() {
        uint8_t font_style[8];
        int16_t slider_offset;
        int16_t draw_x;

        memset(font_style, 0, sizeof(font_style));
        font_style[0] = 0x6e; /* 'n' */
        font_style[1] = 0x71; /* 'q' */
        font_style[2] = 0x73; /* 's' */

        fonts::Set_Remap_Font_Style_(2, (s_colors*)font_style);

        if (COLONY::_full_draw != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MAINPUPS::Mainpups_Tested_Execute_Autofunction_();

            s_animation_header* anim = _anims[14];
            animate::Remap_Draw_(MAINPUPS::_g_x, MAINPUPS::_g_y, anim);

            BILL::Squeeze_Formatted_Paragraph_Centered_(MAINPUPS::_g_x + 19, MAINPUPS::_g_y + 19, 252 /* 0xFC */, 92 /* 0x5C */, MAINPUPS::_g_msg, 2);
            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }

        fonts::Print_Integer_Centered_(MAINPUPS::_g_x + 32, MAINPUPS::_g_y + 132, (int)MAINPUPS::_g_min);
        fonts::Print_Integer_Centered_(MAINPUPS::_g_x + 145, MAINPUPS::_g_y + 130, (int)MAINPUPS::_g_var);
        fonts::Print_Integer_Centered_(MAINPUPS::_g_x + 257, MAINPUPS::_g_y + 132, (int)MAINPUPS::_g_max);

        // Calculate handle horizontal position: 207 is track width
        slider_offset = (int16_t)((207 * (MAINPUPS::_g_var - MAINPUPS::_g_min)) / (MAINPUPS::_g_max - MAINPUPS::_g_min) + 37);

        draw_x = MAINPUPS::_g_x + slider_offset;
        line::Line_(draw_x, MAINPUPS::_g_y + 153, draw_x, MAINPUPS::_g_y + 164, 32 /* ' ' */);

        draw_x++;
        line::Line_(draw_x, MAINPUPS::_g_y + 153, draw_x, MAINPUPS::_g_y + 164, 36 /* '$' */);

        draw_x++;
        line::Line_(draw_x, MAINPUPS::_g_y + 153, draw_x, MAINPUPS::_g_y + 164, 40 /* '(' */);

        draw_x++;
        line::Line_(draw_x, MAINPUPS::_g_y + 153, draw_x, MAINPUPS::_g_y + 164, 36 /* '$' */);

        draw_x++;
        line::Line_(draw_x, MAINPUPS::_g_y + 153, draw_x, MAINPUPS::_g_y + 164, 32 /* ' ' */);
    }

    void __cdecl Leader_Technology_String_(int16_t player_idx, int16_t leader_idx, char* output_buf) {
        char* current_dest = output_buf;

        for (int16_t i = 0; i < 3; ++i) {
            uint8_t tech_id = MOX::_leaders[leader_idx].tech_application[i];
            if (tech_id != 0) {
                const char* prefix;
                if (i == 0) {
                    prefix = ESTRINGS::E_Strings_(208 /* 0xD0 */);
                } else {
                    prefix = ", ";
                }

                current_dest += sprintf(current_dest, prefix);

                char* tech_name = TECHDATA::_technology_applications[tech_id].name;
                const char* format = ESTRINGS::E_Strings_(7);
                current_dest += sprintf(current_dest, format, tech_name);
            }
        }

        if (leader_idx == 47 /* 0x2F */) {
            int8_t trait0 = MOX::_player[player_idx].traits[0];
            // Check if the trait index is even
            if ((trait0 % 2) == 0) {
                const char* gov_label = MOX::_government_labels[trait0 + 1];
                const char* format = ESTRINGS::E_Strings_(207 /* 0xCF */);
                current_dest += sprintf(current_dest, format, gov_label);
            }
        }

        if (current_dest == output_buf) {
            const char* format = ESTRINGS::E_Strings_(12 /* 0xC */);
            sprintf(current_dest, format);
        }
    }

    void __cdecl Add_Selection_Fields_() {
        MOX::_draw_system_display_without_button = 1;

        SYS::Wrapper_For_Informational_System_Fields_(
            static_cast<uint32_t>(COLONY::_sys_disp_star_handle),
            _g_x,
            _g_y,
            13,
            47,
            &_mp_fields[2]
        );

        char* hotkey = ESTRINGS::E_Strings_(12);
        _mp_fields[0] = fields::Add_Hidden_Field_(0, 0, 639, 479, hotkey, 0);
    }

    void __cdecl Load_Combat_Colony_Picture_To_Screen_Seg_(int16_t colony_idx) {
        int16_t planet_idx = MOX::_colony[colony_idx].planet;
        int16_t climate = MOX::_planet[planet_idx].climate;
        int16_t size = MOX::_planet[planet_idx].size;

        int16_t pic_idx1 = size + (climate * 6);
        char* filename = ESTRINGS::E_Strings_(0xDF);
        MOX::_combat_planet_seg = (s_animation_header*)farload::Far_Reload_Next_(filename, pic_idx1, MOX::_screen_seg);

        int16_t pic_idx2 = (climate * 6) + 5;
        filename = ESTRINGS::E_Strings_(0xDF);
        MOX::_combat_planet_seg2 = (s_animation_header*)farload::Far_Reload_Next_(filename, pic_idx2, MOX::_screen_seg);

        animate::Draw_Palette_(MOX::_combat_planet_seg2);
        palette::Refresh_Palette_();
        palette::Set_Palette_Changes_(0, 255 /* 0xFF */);
        remap::Update_Glass_Remap_Colors_();
    }

    int16_t __cdecl Evaluate_Colony_Combat_Input_(int16_t *field_id_ptr, int16_t* unused_input, int16_t *close_popup_flag, int16_t *selection_idx_ptr) {
        (void)unused_input;
        int16_t field_id = *field_id_ptr;

        if (field_id == _mp_fields[6]) {
            *close_popup_flag = 1;
            return -1;
        }

        if (field_id == -_mp_fields[6]) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x182), ESTRINGS::E_Strings_(0x260));
        } else if (field_id == _mp_fields[12]) {
            *close_popup_flag = 1;
            return 3;
        } else if (field_id == -_mp_fields[12]) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x150), ESTRINGS::E_Strings_(0x261));
        } else if (field_id == _mp_fields[13]) {
            *close_popup_flag = 1;
            MAINPUPS::_no_flicker_between_mainpups_flag = 1;
            return 2;
        } else if (field_id == -_mp_fields[13]) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0xcd), ESTRINGS::E_Strings_(0x262));
        } else if (field_id == _mp_fields[14]) {
            *close_popup_flag = 1;
            return 1;
        } else if (field_id == -_mp_fields[14]) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x108), ESTRINGS::E_Strings_(0x263));
        } else if (field_id == _mp_fields[15]) {
            if (ERIC::Player_Can_Mind_Control_Colony_With_Help_(MAINPUPS::_g_data_ptr->attacker_player_id, MAINPUPS::_g_data_ptr->colony_idx)) {
                *close_popup_flag = 1;
                return 4;
            }
        } else if (field_id == -_mp_fields[15]) {
            if (ERIC::Player_Can_Mind_Control_Colony_(MAINPUPS::_g_data_ptr->attacker_player_id, MAINPUPS::_g_data_ptr->colony_idx)) {
                TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x175), ESTRINGS::E_Strings_(0x264));
            } else {
                ERIC::Player_Can_Mind_Control_Colony_With_Help_(MAINPUPS::_g_data_ptr->attacker_player_id, MAINPUPS::_g_data_ptr->colony_idx);
            }
        } else if (field_id == _mp_fields[16]) {
            *selection_idx_ptr = 2;
        } else if (field_id == -_mp_fields[16]) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x114), ESTRINGS::E_Strings_(0x265));
        } else if (field_id == _mp_fields[17]) {
            *selection_idx_ptr = 1;
        } else if (field_id == -_mp_fields[17]) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0xd9), ESTRINGS::E_Strings_(0x266));
        } else if (field_id == _mp_fields[18]) {
            *selection_idx_ptr = 0;
            fields::Clear_Mouse_Buffer_();
            fields::Set_Input_Delay_(1);
            timer::Mark_Time_();
        } else if ((int32_t)_mp_fields[17] + field_id == 0) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0xc7), ESTRINGS::E_Strings_(0x267));
        } else {
            *close_popup_flag = 0;
            return 0;
        }

        COLONY::_full_draw = 1;
        *close_popup_flag = 0;
        return 0;
    }

    void __cdecl Draw_Dragon_New_Star_Popup_()
    {
        char msg_buffer[300];

        if (_g_scanned_star != _g_old_scanned_star) {
            _g_old_scanned_star = _g_scanned_star;
            COLONY::_full_draw = 1;
        }

        if (COLONY::_full_draw != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MOX::_draw_main_screen_to_back = 1;
            Mainpups_Tested_Execute_Autofunction_();
            COLONY::_full_draw = 1;

            animate::Draw_(0, 0, _anims[37]);

            s_animation_header* anim_c = COLONY::C_Anims_(12);
            s_animation_header* anim_b = COLONY::C_Anims_(11);

            MOVEBOX::Draw_Galaxy_Map_Box_(anim_b, anim_c, 302, 156, 148, 94, 0, 0, 0, 0, 0, 0);

            if (_field_mode == 1) {
                sprintf(msg_buffer, ESTRINGS::E_Strings_(464), MOX::_player[_g_player_n].name);
            } else {
                int16_t str_idx = (_field_mode == 2) ? 583 : 555;
                sprintf(msg_buffer, ESTRINGS::E_Strings_(str_idx), 500, MOX::_player[_g_player_n].name, MOX::_star[_g_star_n].name);
            }

            BILL::Squeeze_Formatted_Paragraph_Centered_(188, 271, 262, 35, msg_buffer, 2);
            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }

        if (_g_scanned_star != (int16_t)-1) {
            fonts::Print_Centered_(376, 254, MOX::_star[_g_scanned_star].name);
        }
    }

    /**
     * @address 0045fcf0
     */
    int16_t __cdecl Evaluate_Dragon_New_Star_Input_(int16_t* field_idx_ptr, int16_t* mouse_pos_ptr, int16_t* status_ptr, int16_t target_star_val) {
        MAINPUPS::_g_scanned_star = -1;
        int16_t field_idx = *field_idx_ptr;

        if (MAINPUPS::_mp_fields[2] == field_idx) {
            *status_ptr = 1;
            return -1;
        }

        if (MAINPUPS::_mp_fields[25] == field_idx) {
            // Check if player has at least 500 BC
            if (MOX::_player[MAINPUPS::_g_player_n].bc >= 500) {
                MAINPUPS::_field_mode = 1;
                COLONY::_full_draw = 1;
                return 0;
            } else {
                COLONY::_full_draw = 1;
                MAINPUPS::_field_mode = 2;
                return 0;
            }
        }

        if (MAINPUPS::_field_mode == 1) {
            int16_t scanned_star_id;
            int16_t scanned_val;
            int16_t scan_res = MAINSCR::Scan_Galaxy_Map_Fields_(*mouse_pos_ptr, field_idx, &scanned_star_id, &scanned_val);

            if (scan_res == 4 /* FIELD_SCAN_STAR */) {
                MAINPUPS::_g_scanned_star = scanned_star_id;
            } else if (scan_res == 0 /* FIELD_SCAN_EMPTY */) {
                if (scanned_val != target_star_val) {
                    *status_ptr = 1;
                    return scanned_val;
                }
                
                // Show help string 576 (0x240)
                GENDRAW::Help_(ESTRINGS::E_Strings_(576 /* 0x240 */));
                COLONY::_full_draw = 1;
            }
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            COLONY::_full_draw = 1;
        }

        return 0;
    }

    /**
     * @address 0045b7f0
     */
    void __cdecl Load_Officer_Gains_Level_Popup_() {
        allocate::Push_Block_(MOX::_screen_seg);

        _anims[10] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_MAINPUPS_LBX, 0x33, MOX::_screen_seg);

        _anims[11] = OFFICER::Load_Officer_Picture_(
            MOX::_PLAYER_NUM,
            MAINPUPS::_g_officer,
            MOX::_screen_seg,
            0,
            0
        );

        OFFICER::Load_Officer_Skill_Icons_(MOX::_screen_seg);
    }

    /**
     * @address 0045d230
     */
    int8_t __cdecl New_Colony_Confirmation_Box_(int16_t planet_idx) {
        s_potential_colony_info potential_info;
        char msg_buffer[2000];
        char* msg_ptr;
        int len;
        int16_t food_val;
        int16_t food_rem;

        ERICHELP::Set_New_Colony_Confirmation_Box_Help_List_();

        COLCALC::Compute_Potential_Colony_Info_(
            &MOX::_player[MOX::_PLAYER_NUM], 
            &MOX::_planet[planet_idx], 
            &potential_info
        );

        char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
        char* format_title = ESTRINGS::E_Strings_(0xD1); /* 209 */
        len = sprintf(msg_buffer, format_title, planet_name);
        msg_ptr = msg_buffer + len;

        food_val = potential_info.food;
        food_rem = food_val % 2;

        if (food_rem != 0) {
            char* format_food_odd = ESTRINGS::E_Strings_(0x64); /* 100 */
            len = sprintf(msg_ptr, format_food_odd, ESTRINGS::s_0_0055110c, ESTRINGS::s_1_00551110, (int)(food_val / 2), (int)food_rem);
        } else {
            char* format_food_even = ESTRINGS::E_Strings_(0x65); /* 101 */
            len = sprintf(msg_ptr, format_food_even, ESTRINGS::s_0_0055110c, ESTRINGS::s_1_00551110, (int)(food_val / 2));
        }
        msg_ptr += len;

        char* format_ind = ESTRINGS::E_Strings_(0x6B); /* 107 */
        len = sprintf(msg_ptr, format_ind, ESTRINGS::s_0_0055110c, ESTRINGS::s_1_00551110, (int)potential_info.industry);
        msg_ptr += len;

        char* format_res = ESTRINGS::E_Strings_(0x73); /* 115 */
        len = sprintf(msg_ptr, format_res, ESTRINGS::s_0_0055110c, ESTRINGS::s_1_00551110, (int)potential_info.research);
        msg_ptr += len;

        char* format_maint = ESTRINGS::E_Strings_(0x6C); /* 108 */
        len = sprintf(msg_ptr, format_maint, ESTRINGS::s_0_0055110c, ESTRINGS::s_1_00551110, (int)potential_info.maintenance);
        msg_ptr += len;

        char* format_bonus = ESTRINGS::E_Strings_(0x78); /* 120 */
        len = sprintf(msg_ptr, format_bonus, ESTRINGS::s_0_0055110c, ESTRINGS::s_1_00551110, (int)potential_info.worker_bonus);
        msg_ptr += len;

        int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(&MOX::_planet[planet_idx], MOX::_PLAYER_NUM);
        char* format_pop = ESTRINGS::E_Strings_(0x6E); /* 110 */
        sprintf(msg_ptr, format_pop, ESTRINGS::s_0_0055110c, ESTRINGS::s_1_00551110, (int)max_pop);

        int8_t result = (int8_t)GENDRAW::Confirmation_Box_(msg_buffer);
        fields::Deactivate_Help_List_();

        return result;
    }

    void __cdecl Draw_Colony_Combat_Popup_() {
        s_colors colors_cfg = {0};
        colors_cfg.colors[1] = 0x11;
        colors_cfg.colors[2] = 0x13;
        
        char text_buffer[500];

        timer::Release_Time_(1);
        timer::Mark_Time_();

        if (COLONY::_full_draw != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            Mainpups_Tested_Execute_Autofunction_();

            int16_t anim_w = animate::Get_Width_(_anims[3]);
            int16_t anim_h = animate::Get_Height_(_anims[3]);

            graphics::Fill_(_g_x + 5, _g_y + 5, anim_w - 10, anim_h - 10, 0);
            animate::Draw_(_g_x, _g_y, _anims[3]);

            int16_t x_center = (_g_x * 2 + anim_w) / 2;
            int16_t y_center = (_g_y * 2 + anim_h) / 2;
            ERIC::Draw_Centered_(x_center, y_center, MOX::_combat_planet_seg);

            s_animation_header* c_anims = COLONY::C_Anims_(13);
            remap::Calculate_Picture_Remap_Colors_(c_anims);
            fonts::Set_Font_Style_(3, &colors_cfg);

            if (_g_state == 0 || _g_state == 1) {
                char* planet_name = HACCESS::Get_Planet_Name_(MOX::_colony[_g_data_ptr->colony_idx].planet);
                sprintf(text_buffer, ESTRINGS::E_Strings_(458), planet_name);
            } else if (_g_state == 2) {
                char* planet_name = HACCESS::Get_Planet_Name_(MOX::_colony[_g_data_ptr->colony_idx].planet);
                sprintf(text_buffer, ESTRINGS::E_Strings_(337), planet_name);
            }

            fonts::Print_Centered_(x_center, _g_y + 20, text_buffer);

            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }

        fonts::Set_Font_Style_(1, &colors_cfg);

        int16_t list_idx = 0;

        if (_g_data_ptr->n_bldgs > 0) {
            animate::Remap_Draw_(_g_x + 20, _g_y + 50, _anims[23]);
            sprintf(text_buffer, ESTRINGS::E_Strings_(19), _g_data_ptr->n_bldgs);
            fonts::Print_(_g_x + 55, _g_y + 50, text_buffer);
            list_idx = 1;
        }

        if (_g_data_ptr->best_shield_bldg != -1) {
            int16_t y_offs = list_idx * 30;
            animate::Remap_Draw_(_g_x + 20, _g_y + y_offs + 50, _anims[24]);
            
            char* bldg_name = COLONY::CR_Building_Name_(_g_data_ptr->best_shield_bldg);
            sprintf(text_buffer, ESTRINGS::E_Strings_(70), bldg_name);
            fonts::Print_(_g_x + 55, _g_y + y_offs + 50, text_buffer);
            list_idx++;
        }

        if (_g_data_ptr->n_pops > 0) {
            int16_t y_offs = list_idx * 30;
            
            uint32_t pop_val = MOX::_colony[_g_data_ptr->colony_idx].pop[0];
            int16_t anim_idx = (pop_val >> 7) & 3;
            uint8_t race = MOX::_player[_g_data_ptr->defender_player_id].race;
            
            s_animation_header* pop_anim = COLONY::People_Anim_(anim_idx, 2, race);
            animate::Remap_Draw_(_g_x + 20, _g_y + y_offs + 50, pop_anim);
            
            sprintf(text_buffer, ESTRINGS::E_Strings_(20), _g_data_ptr->n_pops);
            fonts::Print_(_g_x + 55, _g_y + y_offs + 50, text_buffer);
            list_idx++;
        }

        int16_t y_offs = list_idx * 30;
        for (int16_t unit_idx = 1; unit_idx >= 0; unit_idx--) {
            if (_g_data_ptr->defender_unit_counts[unit_idx] > 0) {
                uint8_t race = MOX::_player[_g_data_ptr->defender_player_id].race;
                s_animation_header* unit_anim = COLONY::Player_Troop_Anim_(_g_data_ptr->defender_player_id, unit_idx, race);
                
                animate::Remap_Draw_(_g_x + 20, _g_y + y_offs + 50, unit_anim);
                
                char* troop_name = COLONY::Player_Troop_Name_(_g_data_ptr->defender_player_id, unit_idx, race);
                
                if (MOX::_settings.language == 4) {
                    sprintf(text_buffer, "%s: %d", troop_name, _g_data_ptr->defender_unit_counts[unit_idx]);
                } else {
                    sprintf(text_buffer, ESTRINGS::E_Strings_(14), _g_data_ptr->defender_unit_counts[unit_idx], troop_name);
                }
                
                fonts::Print_(_g_x + 55, _g_y + y_offs + 50, text_buffer);
                y_offs += 30;
            }
        }

        int16_t right_list_idx = 0;
        if (_g_data_ptr->attacker_unit_counts[1] > 0) {
            uint8_t attacker_race = MOX::_player[_g_data_ptr->attacker_player_id].race;
            s_animation_header* att_anim = COLONY::Player_Troop_Anim_(_g_data_ptr->attacker_player_id, 1, attacker_race);
            
            int16_t anim_w = animate::Get_Width_(_anims[3]);
            animate::Remap_Draw_(_g_x + anim_w - 40, _g_y + 50, att_anim);
            
            char* att_troop_name = COLONY::Player_Troop_Name_(_g_data_ptr->attacker_player_id, 1, attacker_race);
            char* fmt = ESTRINGS::E_Strings_(14);
            sprintf(text_buffer, fmt, _g_data_ptr->attacker_unit_counts[1], att_troop_name);
            
            anim_w = animate::Get_Width_(_anims[3]);
            fonts::Print_Right_(_g_x + anim_w - 45, _g_y + 50, text_buffer);
            right_list_idx = 1;
        }

        if (_g_data_ptr->bombardment_result >= 0) {
            int16_t right_y_offs = right_list_idx * 30;
            int16_t anim_w = animate::Get_Width_(_anims[3]);
            
            animate::Remap_Draw_(_g_x + anim_w - 40, _g_y + right_y_offs + 50, _anims[22]);
            
            if (_g_state == 0 || _g_state == 2) {
                sprintf(text_buffer, ESTRINGS::E_Strings_(21), _g_data_ptr->bombardment_result);
            } else {
                sprintf(text_buffer, ESTRINGS::E_Strings_(18), _g_data_ptr->bombardment_result);
            }
            
            anim_w = animate::Get_Width_(_anims[3]);
            fonts::Print_Right_(_g_x + anim_w - 45, _g_y + right_y_offs + 50, text_buffer);
        }

        FLCQUEUE::Draw_Flic_Queue_();
    }

    void __cdecl Add_Colony_Combat_Fields_(int16_t popup_mode, int16_t* combat_state_ptr) {
        int16_t si = 1;

        _mp_fields[6] = fields::Add_Button_Field_(
            _g_x + 0xDD,
            animate::Get_Height_(_anims[3]) + _g_y - 0x27,
            ESTRINGS::E_Strings_(0xC),
            _anims[9],
            ESTRINGS::E_Strings_(0x87),
            0x28
        );

        if (popup_mode == 1) {
            if (_g_data_ptr->bombardment_result < 0 || *combat_state_ptr == 0 || *combat_state_ptr == 3 || _g_data_ptr->n_pops <= 0 || *combat_state_ptr == 2) {
                _mp_fields[18] = -1000;
            } else {
                _mp_fields[18] = fields::Add_Button_Field_(
                    _g_x + 0xDD,
                    animate::Get_Height_(_anims[3]) + _g_y - 0x40,
                    ESTRINGS::E_Strings_(0xC),
                    _anims[16],
                    ESTRINGS::E_Strings_(0xC1),
                    0x28
                );
                si = 2;
            }

            if (_g_data_ptr->bombardment_result <= 0 || *combat_state_ptr == 1 || *combat_state_ptr == 3 || _g_data_ptr->n_pops <= 0 || *combat_state_ptr == 2) {
                _mp_fields[17] = -1000;
            } else {
                _mp_fields[17] = fields::Add_Button_Field_(
                    _g_x + 0xDD,
                    animate::Get_Height_(_anims[3]) - (si * 25) + _g_y - 0x27,
                    ESTRINGS::E_Strings_(0xC),
                    _anims[18],
                    ESTRINGS::E_Strings_(0x13C),
                    0x28
                );
                si++;
            }

            if (_g_data_ptr->bombardment_result <= 0 || *combat_state_ptr == 2 || *combat_state_ptr == 3 || _g_data_ptr->n_pops <= 0) {
                _mp_fields[16] = -1000;
            } else {
                _mp_fields[16] = fields::Add_Button_Field_(
                    _g_x + 0xDD,
                    animate::Get_Height_(_anims[3]) - (si * 25) + _g_y - 0x27,
                    ESTRINGS::E_Strings_(0xC),
                    _anims[19],
                    ESTRINGS::E_Strings_(0xA6),
                    0x28
                );
                si++;
            }
        }

        if (_g_data_ptr->bombardment_result < 0 || popup_mode == 1 || popup_mode == 2 || _g_data_ptr->n_pops <= 0) {
            if (_mp_fields[13] != -1000) {
                fields::Clear_Mouse_Buffer_();
                fields::Set_Input_Delay_(3);
            }
            _mp_fields[13] = -1000;
        } else {
            _mp_fields[13] = fields::Add_Button_Field_(
                _g_x + 0xDD,
                animate::Get_Height_(_anims[3]) - (si * 25) + _g_y - 0x27,
                ESTRINGS::E_Strings_(0xC),
                _anims[16],
                ESTRINGS::E_Strings_(0xC1),
                0x28
            );
            si++;
        }

        if (_g_data_ptr->attacker_unit_counts[1] <= 0 || _g_data_ptr->n_pops <= 0 || popup_mode == 2) {
            _mp_fields[12] = -1000;
        } else {
            _mp_fields[12] = fields::Add_Button_Field_(
                _g_x + 0xDD,
                animate::Get_Height_(_anims[3]) - (si * 25) + _g_y - 0x27,
                ESTRINGS::E_Strings_(0xC),
                _anims[15],
                ESTRINGS::E_Strings_(0x147),
                0x28
            );
            si++;
        }

        if (MOX::_player[_g_data_ptr->attacker_player_id].traits[25] == 0 || popup_mode == 2 || _g_data_ptr->n_pops <= 0) {
            _mp_fields[15] = -1000;
        } else {
            s_animation_header* anim;
            if (ERIC::Player_Can_Mind_Control_Colony_(_g_data_ptr->attacker_player_id, _g_data_ptr->colony_idx)) {
                anim = _anims[20];
            } else {
                anim = _anims[21];
            }
            _mp_fields[15] = fields::Add_Button_Field_(
                _g_x + 0xDD,
                animate::Get_Height_(_anims[3]) - (si * 25) + _g_y - 0x27,
                ESTRINGS::E_Strings_(0xC),
                anim,
                ESTRINGS::E_Strings_(0x16D),
                0x28
            );
            si++;
        }

        if (_g_data_ptr->has_stellar_converter == 0 || popup_mode == 2 || _g_data_ptr->n_pops <= 0) {
            _mp_fields[14] = -1000;
        } else {
            _mp_fields[14] = fields::Add_Button_Field_(
                _g_x + 0xDD,
                animate::Get_Height_(_anims[3]) - (si * 25) + _g_y - 0x27,
                ESTRINGS::E_Strings_(0xC),
                _anims[17],
                ESTRINGS::E_Strings_(0xFB),
                0x28
            );
        }

        _mp_fields[0] = fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, ESTRINGS::E_Strings_(0xC), 0);
    }

    int16_t __cdecl Check_System_Display_Fields_Defense_Selection_(int16_t* mouse_x, int16_t* mouse_y, int16_t* selection_made) {
        int16_t scanned_planet_field;
        int16_t scanned_planet_icon;
        int16_t scanned_ship_field;
        int16_t scanned_ship_icon;
        int16_t scanned_system_icon;

        int16_t scan_result = MAINSCR::Scan_System_Display_Fields_(
            *mouse_y,
            *mouse_x,
            MOX::_planet_buttons,
            &scanned_planet_field,
            &scanned_planet_icon,
            &scanned_ship_field,
            &scanned_ship_icon,
            &scanned_system_icon
        );

        MOX::_system_display_ship_scanned = scanned_ship_field;

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            COLONY::_full_draw = 1;
            MAINPUPS::Eric_Pop_Fields_();
            MAINPUPS::Update_Colony_Defense_Fields_();
            video::Copy_Back_To_Off_();
            MAINPUPS::Draw_Defense_Selection_Popup_();
            fields::Assign_Auto_Function_(MAINPUPS::Draw_Defense_Selection_Popup_, 1);
        }

        if (scanned_system_icon > -1) {
            MOX::_system_icon_scanned = scanned_system_icon;
        }

        if (scan_result == 0) {
            if (scanned_planet_icon > -1) {
                int16_t planet_id = HACCESS::System_Planet_Scanned_To_Planet_Id_(
                    static_cast<int16_t>(COLONY::_sys_disp_star_handle),
                    scanned_planet_icon
                );

                int16_t colony_idx = MOX::_planet[planet_id].colony_index;

                if (colony_idx == -1) {
                    int16_t str_id;
                    if (MOX::_planet[planet_id].planet_type == 3) {
                        str_id = 0x250;
                    } else {
                        str_id = 0x24F;
                    }

                    GENDRAW::Help_(ESTRINGS::E_Strings_(str_id));
                    COLONY::_full_draw = 1;
                    MAINPUPS::Eric_Pop_Fields_();
                    MAINPUPS::Update_Colony_Defense_Fields_();
                    video::Copy_Back_To_Off_();
                    MAINPUPS::Draw_Defense_Selection_Popup_();
                    fields::Assign_Auto_Function_(MAINPUPS::Draw_Defense_Selection_Popup_, 1);
                } else {
                    if (MOX::_colony[colony_idx].owner != MAINPUPS::_g_player_n) {
                        *selection_made = 1;
                        return colony_idx + 15;
                    }

                    GENDRAW::Help_(ESTRINGS::E_Strings_(0x252));
                    COLONY::_full_draw = 1;
                    MAINPUPS::Eric_Pop_Fields_();
                    MAINPUPS::Update_Colony_Defense_Fields_();
                    video::Copy_Back_To_Off_();
                    MAINPUPS::Draw_Defense_Selection_Popup_();
                    fields::Assign_Auto_Function_(MAINPUPS::Draw_Defense_Selection_Popup_, 1);
                }
            }

            if (scanned_ship_icon > -1) {
                int16_t ship_id = HACCESS::System_Ship_Scanned_To_Ship_Id_(scanned_ship_icon);
                int16_t owner = (int16_t)MOX::_ship[ship_id].owner;

                if (MAINPUPS::_g_player_n == owner) {
                    GENDRAW::Help_(ESTRINGS::E_Strings_(0x253));
                    COLONY::_full_draw = 1;
                    MAINPUPS::Eric_Pop_Fields_();
                    MAINPUPS::Update_Colony_Defense_Fields_();
                    video::Copy_Back_To_Off_();
                    MAINPUPS::Draw_Defense_Selection_Popup_();
                    fields::Assign_Auto_Function_(MAINPUPS::Draw_Defense_Selection_Popup_, 1);
                } else {
                    if (MAINPUPS::Player_Has_Colony_Or_Outpost_At_Star_(owner, COLONY::_sys_disp_star_handle) != 0) {
                        GENDRAW::Help_(ESTRINGS::E_Strings_(0x24E));
                        COLONY::_full_draw = 1;
                        MAINPUPS::Eric_Pop_Fields_();
                        MAINPUPS::Update_Colony_Defense_Fields_();
                        video::Copy_Back_To_Off_();
                        MAINPUPS::Draw_Defense_Selection_Popup_();
                        fields::Assign_Auto_Function_(MAINPUPS::Draw_Defense_Selection_Popup_, 1);
                        return -1;
                    } else {
                        *selection_made = 1;
                        return owner;
                    }
                }
            }
        }

        return -1;
    }

    void __cdecl Set_G_Special_Value_(int16_t star_idx) {
        if (((MOX::_star[star_idx].just_visited >> static_cast<uint8_t>(MOX::_PLAYER_NUM)) & 1) != 0) {
            _g_special = (uint8_t)REPORT::Draw_Special_(&MOX::_star[star_idx], MOX::_PLAYER_NUM);
            MOX::_star[star_idx].just_visited &= static_cast<uint8_t>(~(1 << static_cast<uint8_t>(MOX::_PLAYER_NUM)));

            if (MOX::_star[star_idx].system_special >= 50) {
                MOX::_star[star_idx].system_special = 0;
            }
        } else {
            _g_special = 0;
        }
    }
}