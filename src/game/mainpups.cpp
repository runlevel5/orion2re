#include "pch.h"

namespace MAINPUPS {
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

    void __cdecl Get_Picture_Draw_XY_(s_animation_header* anim) {
        _g_x = 0x140 - (animate::Get_Width_(anim) / 2);
        _g_y = 0xF0 - (animate::Get_Height_(anim) / 2);
    }

    void __cdecl Do_Popup_Setup_(int16_t param_1, s_animation_header* param_2) {
        if (MOX::_current_screen != 0) {
            _explode_flag = 0;
        }

        _g_refresh_mode = video::Get_Refresh_Mode_();
        video::Set_Refresh_Stencil_();
        Get_Picture_Draw_XY_(param_2);

        if (param_1 == -1) {
            param_1 = 0;
            if (MOX::_player[MOX::_PLAYER_NUM].home_planet_id > 0) {
                param_1 = MOX::_planet[MOX::_player[MOX::_PLAYER_NUM].home_planet_id].star_index;
            }
        }

        int16_t star_idx = param_1;
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
            MOX2::Execute_Auto_Function_(&_eric_saved_autofunction); // TYPE MISMATCH: expected void*, got void**

            bool skip_update = false;
            if (_explode_flag != 0) {
                if (MAINSCR::Star_On_Screen_(star_idx)) {
                    skip_update = true;
                }
            }

            if (!skip_update && _no_flicker_between_mainpups_flag == 0) {
                RUSS::Mox_Sync_Update_();
                COLONY::_full_draw = 1;
                MOX2::Execute_Auto_Function_(&_eric_saved_autofunction); // TYPE MISMATCH: expected void*, got void**
            }
        } else if (_autofunction_ctr == 1) {
            MOX2::Get_Auto_Function_(&_eric_saved_autofunction2);
            COLONY::_full_draw = 1;
            MOX2::Execute_Auto_Function_(&_eric_saved_autofunction); // TYPE MISMATCH: expected void*, got void**
            COLONY::_full_draw = 1;
            MOX2::Execute_Auto_Function_(&_eric_saved_autofunction2); // TYPE MISMATCH: expected void*, got void**
        }

        _autofunction_ctr++;

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

                int16_t height = animate::Get_Height_(param_2);
                int16_t width = animate::Get_Width_(param_2);
                MAINSCR::Draw_Exploding_Plasma_Box_(draw_x, draw_y, _g_x, _g_y, width, height, -1);
            }
        }

        if (_no_flicker_between_mainpups_flag != 0) {
            _no_flicker_between_mainpups_flag = 0;
        }

        COLONY::_full_draw = 1;
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
            MOX::_player[MOX::_PLAYER_NUM].tax_rate = static_cast<char>(_g_temp_tax_rate);
        } else if (*input_val != 0) {
            MOX::_player[MOX::_PLAYER_NUM].tax_rate = static_cast<char>(_g_temp_tax_rate);
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
            // 9 shorts = 18 bytes per save
            FIELDSAV::Restore_Field_Stats_(&_stats_array[_num_field_saves * 9]);
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
        _g_msg_ptr = nullptr;
    }

    void __cdecl Build_Generic_Planet_Or_Colony_Description_String_Formatted_(int16_t planet_idx, char* buffer) {
        s_planet_data* p = &MOX::_planet[planet_idx];

        if (p->colony_index == -1) {
            if (p->planet_type != 3) {
                char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
                int len = _sprintf(buffer, ESTRINGS::E_Strings_(0x56), planet_name);
                _sprintf(buffer + len, ESTRINGS::E_Strings_(0x133));
                return;
            }

            s_potential_colony_info potential_info;
            COLCALC::Compute_Potential_Colony_Info_(&MOX::_player[MOX::_PLAYER_NUM], p, &potential_info);

            char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
            int len = _sprintf(buffer, ESTRINGS::E_Strings_(0x52), s_0_0055110c, planet_name);
            buffer += len;

            char* climate_str = MOX::_planet_climate_string[p->climate];
            char* size_str = MOX::_planet_size_string[p->size];
            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x4D), s_0_0055110c, size_str, climate_str);
            buffer += len;

            char* mineral_str = MOX::_mineral_class_string[p->mineral_class];
            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x6F), s_0_0055110c, mineral_str);
            buffer += len;

            char* gravity_str = MOX::_planet_gravity_string[p->gravity_class];
            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x53), s_0_0055110c, gravity_str);
            buffer += len;

            if ((potential_info.food % 2) == 0) {
                len = _sprintf(buffer, ESTRINGS::E_Strings_(0x65), s_0_0055110c, s_1_00551110, potential_info.food / 2);
                buffer += len;
            } else {
                len = _sprintf(buffer, ESTRINGS::E_Strings_(0x64), s_0_0055110c, s_1_00551110, potential_info.food / 2, potential_info.food % 2);
                buffer += len;
            }

            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x6B), s_0_0055110c, s_1_00551110, potential_info.industry);
            buffer += len;

            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x74), s_0_0055110c, s_1_00551110, potential_info.research);
            buffer += len;

            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x6C), s_0_0055110c, s_1_00551110, potential_info.maintenance);
            buffer += len;

            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x79), s_0_0055110c, s_1_00551110, potential_info.worker_bonus);
            buffer += len;

            int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(p, MOX::_PLAYER_NUM);
            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x6D), s_0_0055110c, s_1_00551110, max_pop);
            buffer += len;
        } else {
            s_colony* c = &MOX::_colony[p->colony_index];
            char* race_name = MOX::_player[c->owner].race_name;
            char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);

            int len = _sprintf(buffer, ESTRINGS::E_Strings_(0x49), s_0_0055110c, planet_name, race_name);
            buffer += len;

            char* climate_str = MOX::_planet_climate_string[static_cast<uint8_t>(c->climate)];
            char* size_str = MOX::_planet_size_string[p->size];
            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x4D), s_0_0055110c, size_str, climate_str);
            buffer += len;

            char* mineral_str = MOX::_mineral_class_string[p->mineral_class];
            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x6F), s_0_0055110c, mineral_str);
            buffer += len;

            char* gravity_str = MOX::_planet_gravity_string[p->gravity_class];
            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x53), s_0_0055110c, gravity_str);
            buffer += len;

            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x5E), s_0_0055110c, s_1_00551110, static_cast<uint8_t>(c->food2_per_farmer) / 2);
            buffer += len;

            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x5F), s_0_0055110c, s_1_00551110, static_cast<uint8_t>(c->industry_per_worker));
            buffer += len;

            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x60), s_0_0055110c, s_1_00551110, static_cast<uint8_t>(c->research_per_scientist));
            buffer += len;

            if (c->owner == MOX::_PLAYER_NUM) {
                len = _sprintf(buffer, ESTRINGS::E_Strings_(0x70), s_0_0055110c, s_1_00551110, c->morale * 5);
                buffer += len;
            }

            int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(p, c->owner);
            len = _sprintf(buffer, ESTRINGS::E_Strings_(0x71), s_0_0055110c, s_1_00551110, static_cast<uint8_t>(c->n_pops), max_pop);
            buffer += len;
        }

        if (p->planet_special != 0) {
            char* special_str = MOX::_planet_special_string[p->planet_special];
            _sprintf(buffer, ESTRINGS::E_Strings_(0x50), s_0_0055110c, special_str);
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
                _sprintf(attacker_name, s_format_string, MOX::_player[attacker_idx].race_name);
            } else {
                _sprintf(attacker_name, ESTRINGS::E_Strings_(0x3A), MOX::_player[attacker_idx].race_name);
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
            _sprintf(attacker_name, ESTRINGS::E_Strings_(str_id));
        }

        if (defender_idx >= 0 && defender_idx < 8) {
            _sprintf(defender_name, ESTRINGS::E_Strings_(0x46), MOX::_player[defender_idx].race_name);
        } else {
            _sprintf(defender_name, ESTRINGS::E_Strings_(0xA5));
        }

        if (MOX::_settings.language == 4) {
            _sprintf(formatted_msg, s_SI_COMBATTE_NEL_SISTEMA, location_name, attacker_name, defender_name);
        } else {
            int16_t format_id = (colony_idx == -1) ? 0x31 : 0x30;
            _sprintf(formatted_msg, ESTRINGS::E_Strings_(format_id), attacker_name, defender_name, location_name);
        }

        ERICHELP::Set_Player_Attacked_Popup_Help_List_();
        GENDRAW::Message_Box_Exploding_(formatted_msg, star_idx);
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

        popup_buffer = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
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

        s_animation_header* popup_bg = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
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

                    if (MOX::_settings.game_difficulty < 2 && _on_officer_screen_flag != 1) {
                        COLONY::_full_draw = 1;
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

                    if (MOX::_settings.game_difficulty < 2 && _on_officer_screen_flag != 1) {
                        COLONY::_full_draw = 1;
                    } else {
                        _field_state = 0;
                        done = true;
                    }

                    fields::Clear_Mouse_Buffer_();
                    fields::Set_Input_Delay_(3);
                } else if (input == -_mp_fields[8]) {
                    int16_t info_cost = static_cast<int16_t>(OFFICER::Calculate_Officer_Cost_(_g_player_n, _g_officer));
                    s_leader_data* leader = &MOX::_leaders[_g_officer];
                    char* level_name = MOX::_officer_level_names[static_cast<uint8_t>(leader->level)];
                    char buffer[500];

                    _sprintf(buffer, ESTRINGS::E_Strings_(0x1fb), level_name, leader->name, info_cost);
                    TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x1bf), buffer);

                    fields::Deactivate_Auto_Function_();
                    fields::Assign_Auto_Function_(Draw_Random_New_Officer_Popup_, 1);
                    Eric_Pop_Fields_();
                    Update_Random_New_Officer_Fields_();

                    COLONY::_full_draw = 1;
                } else if (input == -_mp_fields[7]) {
                    int16_t info_cost = static_cast<int16_t>(OFFICER::Calculate_Officer_Cost_(_g_player_n, _g_officer));
                    s_leader_data* leader = &MOX::_leaders[_g_officer];
                    char* level_name = MOX::_officer_level_names[static_cast<uint8_t>(leader->level)];
                    char buffer[500];

                    _sprintf(buffer, ESTRINGS::E_Strings_(0x1fa), level_name, leader->name, info_cost);
                    TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x141), buffer);

                    fields::Deactivate_Auto_Function_();
                    fields::Assign_Auto_Function_(Draw_Random_New_Officer_Popup_, 1);
                    Eric_Pop_Fields_();
                    Update_Random_New_Officer_Fields_();

                    COLONY::_full_draw = 1;
                } else if (input > 0) {
                    done = true;
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

        s_animation_header* anim = (s_animation_header*)Buffer_Reload_(s_BUFFER0_LBX, 0x49, MOX::_global_cache_seg);
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

    int16_t __cdecl Colony_Combat_Popup_(s_colony_combat_popup_data* param_1, int16_t param_2) {
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

        MAINPUPS::Update_Colony_Combat_Fields_(param_2, &var_1C);
        MAINPUPS::Draw_Colony_Combat_Popup_();
        RUSS::Mox_Sync_Update_();
        video::Set_Refresh_Stencil_();

        if (param_2 == 2) {
            invasion_started = true;
        }
        else if (param_2 == 1) {
            MAINPUPS::Setup_To_Apply_Bomb_Hits_(MAINPUPS::_g_data_ptr->bomb, &bomb_hit_val1, &bomb_hit_val2);
        }

        random::Set_Random_Seed_(MAINPUPS::_g_data_ptr->random_seed);
        MAINPUPS::_g_state = param_2;

        while (true) {
            COLONY::_full_draw = 1;
            MAINPUPS::Eric_Pop_Fields_();
            MAINPUPS::Update_Colony_Combat_Fields_(param_2, &var_1C);

            input_val = fields::Get_Input_();
            scan_val = fields::Scan_Input_();

            if (param_2 == 2) {
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
            else if (param_2 == 1) {
                MAINPUPS::Apply_Next_Bomb_Hit_(MAINPUPS::_g_data_ptr->bomb, &var_1C, &bomb_hit_val1, &bomb_hit_val2);
            }

            int16_t result = MAINPUPS::Evaluate_Colony_Combat_Input_(&input_val, &scan_val, &done, &var_1C);
            MAINPUPS::Draw_Colony_Combat_Popup_();
            RUSS::Mox_Sync_Update_();

            if (param_2 == 2) {
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
                    _sprintf(buffer, fmt, planet_name, race_name);
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
}