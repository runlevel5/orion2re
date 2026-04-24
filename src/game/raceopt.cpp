#include "pch.h"

namespace raceopt {

    void __cdecl Setup_Evolutionary_Upgrade_(short player_idx) {
        s_player& plr = MOX::_player[player_idx];
        int8_t original_traits0 = plr.traits[0];

        if (plr.objectives == 100) {
            // Copy 30 bytes (traits index 1 to 30)
            struct_::Copy_Structure_(&plr.evolutionary_upgrade_traits[1], &plr.traits[1], 30);

            // Apply mask to trait 0 for the evolutionary upgrade
            plr.evolutionary_upgrade_traits[0] = original_traits0 & 0x0E;

            Evolutionary_Mutation_Screen_(plr.evolutionary_upgrade_traits);

            // Explicitly restore original traits[0]
            plr.traits[0] = original_traits0;
        }
        else {
            // Revert traits from evolutionary buffer if objectives condition not met
            struct_::Copy_Structure_(&plr.traits[1], &plr.evolutionary_upgrade_traits[1], 30);
        }

        plr.picked_evolutionary_upgrade = 1;
    }

    void __cdecl Evolutionary_Mutation_Screen_(int8_t *traits) {
        void *local_164;
        int16_t asStack_160[10];
        int8_t local_14c[31];
        char acStack_12c[200];
        char acStack_64[100];
        
        MOX2::Get_Auto_Function_(&local_164);
        ERIC::Fast_Fade_Out_();
        fonts::Load_Palette_(9, 0, 255);
        Convert_Player_To_Flags_EM_(traits);
        struct_::Copy_Structure_(local_14c, traits, 31);
        
        for (int16_t i = 1; i < 10; ++i) {
            for (int16_t j = 2; j >= 0; --j) {
                if (MOX::_special_bonus[i - 1][j] == local_14c[i]) {
                    local_14c[i] = j + 1;
                }
            }
        }
        
        Set_Race_Option_Help_List_();
        fields::Clear_Fields_();
        Reload_Option_Strings_();
        Reload_Racial_Option_Screen_();
        Calculate_EM_Picks_();
        
        int16_t orig_picks = _picks;
        fields::Assign_Auto_Function_(Draw_Racial_Option_Screen_, 1);
        
        _color_opt.colors[0] = 0x8E;
        _color_opt.colors[1] = 0x9D;
        _color_opt.colors[2] = 0x9C;
        _color_opt.colors[3] = 0x9D;
        _color_opt.colors[4] = 0x9D;
        _color_opt.colors[5] = 0x9D;
        _color_opt.colors[6] = 0x9D;
        _color_opt.colors[7] = 0x9D;
        fonts::Set_Font_Style_(4, &_color_opt);
        
        Add_Race_Options_Fields_EM_(local_14c, orig_picks);
        
        for (int i = 0; i < 10; ++i) {
            asStack_160[i] = _button_variable[i];
        }
        
        video::Set_Page_Back_();
        Draw_Race_Option_Background_();
        video::Copy_Back_To_Off_();
        video::Set_Page_Off_();
        Draw_Racial_Option_Screen_();
        palstore::Darken_Palette_(100);
        palette::Clear_Palette_Changes_(0, 255);
        RUSS::Mox_Sync_Update_();
        ERIC::Fast_Fade_In_();
        
        KEN::Ken_Get_Text_Message_(77, acStack_64, 100);
        strcpy(acStack_12c, acStack_64);
        KEN::Ken_Get_Text_Message_(78, acStack_64, 100);
        strcat(acStack_12c, acStack_64);
        GENDRAW::Message_Box_(acStack_12c);
        
        video::Set_Page_Back_();
        Draw_Race_Option_Background_();
        video::Copy_Back_To_Off_();
        video::Set_Page_Off_();
        Draw_Racial_Option_Screen_();
        RUSS::Mox_Sync_Update_();
        
        bool done = false;
        while (!done) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            int16_t input = fields::Get_Input_();
            
            for (int i = 0; i < 36; ++i) {
                if (_group_button[i] == input) {
                    int group = i / 4;
                    int choice = i % 4;
                    
                    if (group == 1 && _option_toggle[9] == 1) {
                        KEN::Ken_Get_Text_Message_(73, acStack_12c, 120);
                        GENDRAW::Warning_Box_(acStack_12c);
                        video::Set_Page_Back_();
                        Draw_Race_Option_Background_();
                        video::Copy_Back_To_Off_();
                        video::Set_Page_Off_();
                        Draw_Racial_Option_Screen_();
                        RUSS::Mox_Sync_Update_();
                        _button_variable[1] = 0;
                    } else {
                        if (asStack_160[group] == choice) {
                            _button_variable[group] = 0;
                            Calculate_EM_Picks_();
                        } else {
                            int cost_idx = i + (-3 - i) / 4;
                            if (_ability_costs[cost_idx] + _negative_picks < -10) {
                                KEN::Ken_Get_Text_Message_(74, acStack_12c, 80);
                                GENDRAW::Warning_Box_(acStack_12c);
                                video::Set_Page_Back_();
                                Draw_Race_Option_Background_();
                                video::Copy_Back_To_Off_();
                                video::Set_Page_Off_();
                                Draw_Racial_Option_Screen_();
                                RUSS::Mox_Sync_Update_();
                                _button_variable[group] = asStack_160[group];
                            } else {
                                _button_variable[group] = choice;
                            }
                        }
                    }
                    
                    int cost_idx2 = i + (-3 - i) / 4;
                    if (_button_variable[group] < local_14c[group + 1] && _ability_costs[cost_idx2] >= 0) {
                        _button_variable[group] = local_14c[group + 1];
                    }
                }
            }
            
            for (int i = 0; i < 22; ++i) {
                if (_option_button[i] == input) {
                    _option_toggle[i] = 1 - _option_toggle[i];
                    
                    if (_option_toggle[i] == 0) {
                        if (_ability_costs[i + 31] < 0) {
                            _option_toggle[i] = 1;
                        }
                        Calculate_EM_Picks_();
                    } else {
                        Calculate_EM_Picks_();
                        if (_negative_picks < -10) {
                            KEN::Ken_Get_Text_Message_(74, acStack_12c, 80);
                            GENDRAW::Warning_Box_(acStack_12c);
                            video::Set_Page_Back_();
                            Draw_Race_Option_Background_();
                            video::Copy_Back_To_Off_();
                            video::Set_Page_Off_();
                            Draw_Racial_Option_Screen_();
                            RUSS::Mox_Sync_Update_();
                            _option_toggle[i] = 0;
                        }
                        
                        if (i == 0) _option_toggle[1] = 0;
                        else if (i == 1) _option_toggle[0] = 0;
                        else if (i == 9) {
                            _option_toggle[8] = 0;
                            _button_variable[1] = 0;
                        }
                        else if (i == 8) _option_toggle[9] = 0;
                        else if (i == 10) _option_toggle[11] = 0;
                        else if (i == 11) _option_toggle[10] = 0;
                        else if (i == 12) _option_toggle[13] = 0;
                        else if (i == 13) _option_toggle[12] = 0;
                    }
                }
            }
            
            if (_accept == input) {
                if (_picks < 0) {
                    KEN::Ken_Get_Text_Message_(75, acStack_12c, 80);
                    GENDRAW::Warning_Box_(acStack_12c);
                    video::Set_Page_Back_();
                    Draw_Race_Option_Background_();
                    video::Copy_Back_To_Off_();
                    video::Set_Page_Off_();
                    Draw_Racial_Option_Screen_();
                    RUSS::Mox_Sync_Update_();
                } else {
                    done = true;
                }
            }
            
            Calculate_EM_Picks_();
            
            if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                video::Set_Page_Back_();
                Draw_Race_Option_Background_();
                video::Copy_Back_To_Off_();
            }
            
            if (!done) {
                Add_Race_Options_Fields_EM_(local_14c, orig_picks);
                video::Set_Page_Off_();
                Draw_Racial_Option_Screen_();
                RUSS::Mox_Sync_Update_();
                
                for (int i = 0; i < 10; ++i) {
                    asStack_160[i] = _button_variable[i];
                }
            }
        }
        
        Convert_Flags_To_Player_EM_(traits);
        fields::Deactivate_Help_List_();
        fields::Deactivate_Auto_Function_();
        MOX2::Put_Auto_Function_(&local_164);
        fields::Clear_Fields_();
        allocate::Pop_Block_(MOX::_screen_seg);
        MISC::Fade_Into_Screen_(MAINSCR::Draw_Mini_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
    }

    int16_t __cdecl Player_Score_Modifier_(int16_t player_idx) {
        int16_t score_modifier = 100;

        _ability_costs = reinterpret_cast<int8_t*>(farload::Far_Reload_Next_(
            s_racestuf_lbx_00556310,
            6,
            MOX::_screen_seg
        ));

        Convert_Player_To_Flags_EM_(MOX::_player[player_idx].traits);
        Calculate_Picks_();

        if (MOX::_player[player_idx].tech_applications[62] == 3) {
            _picks += 4;
        }

        if (_picks > 0) {
            score_modifier = (_picks * 5 + 50) * 2;
        }

        return score_modifier;
    }

    void __cdecl Reload_Option_Strings_() {
        char* strings_data = static_cast<char*>(farload::Far_Reload_Next_(
            s_racestuf_lbx_00556310,
            static_cast<uint32_t>(static_cast<uint8_t>(MOX::_settings.language)),
            MOX::_screen_seg));

        _ability_costs = static_cast<int8_t*>(farload::Far_Reload_Next_(
            s_racestuf_lbx_00556310,
            6,
            MOX::_screen_seg));

        int16_t offset = 0;
        int16_t index = 1;

        _opt_string[0] = strings_data;

        do {
            while (strings_data[offset] != '\0') {
                offset++;
            }
            while (strings_data[offset] == '\0') {
                offset++;
            }
            _opt_string[index] = strings_data + offset;
            index++;
        } while (index < 64);
    }

    void __cdecl Calculate_EM_Picks_() {
        Calculate_Picks_();
        _picks += 4;
    }

    void __cdecl Draw_Race_Option_Background_() {
        int16_t col_x[4] = { 80, 254, 442, 643 };
        int16_t y_coords[5] = { 72, 147, 222, 297, 372 };
        s_colors color1 = {{ 0x91, 0x95, 0x95, 0x95, 0x95, 0x95, 0x95, 0x95 }};
        
        s_animation_header* bg1 = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_RACEOPT_LBX_00556320, 4, MOX::_global_cache_seg));
        animate::Draw_(0, 0, bg1);
        
        s_animation_header* bg2 = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_patch13_lbx_00556330, 0, MOX::_global_cache_seg));
        animate::Draw_(131 /* 0x83 */, 0, bg2);

        int16_t col_idx = 0;
        int16_t y_idx = 0;
        int16_t opt_string_idx = 0;

        // First section: columns 0 and 1
        while (opt_string_idx < 36) {
            fonts::Set_Font_Style_Shadow_Down_(4, &_color_opt);
            fonts::Set_Outline_Color_(119 /* 0x77 */, 0);

            int16_t current_y = y_coords[y_idx];
            int16_t header_x = col_x[col_idx] - 22;
            
            fonts::Print_(header_x, current_y - 22, _opt_string[opt_string_idx]);
            opt_string_idx++;

            int16_t col_start_x = col_x[col_idx];
            int16_t right_align_x = col_x[col_idx + 1] - 50;

            for (int i = 0; i < 3; i++) {
                if (_group_button[opt_string_idx] > 0) {
                    animate::Set_Animation_Frame_(_special_button_seg, 0);
                    animate::Draw_(col_start_x - 20, current_y, _special_button_seg);
                }

                fonts::Set_Font_Style_Shadow_Down_(2, &color1);

                char num_buf[12];
                char str_buf[12];
                
                str_buf[0] = s__00551100[0];

                int cost_idx = opt_string_idx + (-3 - opt_string_idx) / 4;
                __itoa(_ability_costs[cost_idx], num_buf, 10);
                strcat(str_buf, num_buf);

                fonts::Print_(col_start_x, current_y, _opt_string[opt_string_idx]);

                int16_t dots_start = col_start_x + fonts::Get_String_Width_(_opt_string[opt_string_idx]) - 1;
                int16_t dots_end = col_x[col_idx + 1] - fonts::Get_String_Width_(str_buf) - 50;

                while (dots_start < dots_end - 4) {
                    fonts::Print_(dots_start, current_y, const_cast<char*>(s___0055632c));
                    dots_start += 4;
                }

                fonts::Print_Right_(right_align_x, current_y, str_buf);
                
                opt_string_idx++;
                current_y += 13;
            }

            y_idx++;
            if (y_idx >= 5) {
                y_idx = 0;
                col_idx++;
            }
        }

        // Second section: column 1 bottom piece
        fonts::Set_Font_Style_Shadow_Down_(4, &_color_opt);
        fonts::Set_Outline_Color_(119 /* 0x77 */, 0);

        int16_t current_y = y_coords[y_idx];
        int16_t header_x = col_x[col_idx] - 22;
        fonts::Print_(header_x, current_y - 22, _opt_string[opt_string_idx]);
        opt_string_idx++;

        int16_t col_start_x = col_x[col_idx];
        int16_t right_align_x = col_x[col_idx + 1] - 50;

        for (int i = 0; i < 4; i++) {
            animate::Set_Animation_Frame_(_special_button_seg, 0);
            animate::Draw_(col_start_x - 20, current_y, _special_button_seg);

            fonts::Set_Font_Style_Shadow_Down_(2, &color1);

            char num_buf[12];
            char str_buf[12];

            int cost_idx = opt_string_idx + (-3 - opt_string_idx) / 4;
            __itoa(_ability_costs[cost_idx], num_buf, 10);
            strcpy(str_buf, num_buf);

            fonts::Print_(col_start_x, current_y, _opt_string[opt_string_idx]);

            int16_t dots_start = col_start_x + fonts::Get_String_Width_(_opt_string[opt_string_idx]) - 1;
            int16_t dots_end = col_x[col_idx + 1] - fonts::Get_String_Width_(str_buf) - 50;

            while (dots_start < dots_end - 4) {
                fonts::Print_(dots_start, current_y, const_cast<char*>(s___0055632c));
                dots_start += 4;
            }

            fonts::Print_Right_(right_align_x, current_y, str_buf);
            opt_string_idx++;
            current_y += 13;
        }

        // Third section: column 2
        col_idx++;
        fonts::Set_Font_Style_Shadow_Down_(4, &_color_opt);
        fonts::Set_Outline_Color_(119 /* 0x77 */, 0);

        fonts::Print_(col_x[col_idx] - 22, 50, _opt_string[opt_string_idx]);
        opt_string_idx++;

        current_y = 72;
        int16_t block_sizes[4];
        block_sizes[0] = 22;

        for (int loop_idx = 0; opt_string_idx < 64; loop_idx++) {
            int16_t count = block_sizes[loop_idx];
            if (count > 0) {
                int16_t col_2_start_x = col_x[col_idx];
                int16_t col_2_right_align_x = col_x[col_idx + 1] - 50;

                for (int i = 0; i < count; i++) {
                    animate::Set_Animation_Frame_(_special_button_seg, 0);
                    animate::Draw_(col_2_start_x - 20, current_y, _special_button_seg);

                    fonts::Set_Font_Style_Shadow_Down_(2, &color1);

                    char num_buf[12];
                    char str_buf[12];

                    __itoa(_ability_costs[opt_string_idx - 11], num_buf, 10);
                    strcpy(str_buf, num_buf);

                    fonts::Print_(col_2_start_x, current_y, _opt_string[opt_string_idx]);

                    int16_t dots_start = col_2_start_x + fonts::Get_String_Width_(_opt_string[opt_string_idx]) - 1;
                    int16_t dots_end = col_x[col_idx + 1] - fonts::Get_String_Width_(str_buf) - 50;

                    while (dots_start < dots_end - 4) {
                        fonts::Print_(dots_start, current_y, const_cast<char*>(s___0055632c));
                        dots_start += 4;
                    }

                    fonts::Print_Right_(col_2_right_align_x, current_y, str_buf);
                    opt_string_idx++;
                    current_y += 16;
                }
            }
            current_y += 9;
        }

        fonts::Set_Font_Style_Shadow_Down_(4, &_color_opt);
    }

    void __cdecl Calculate_Picks_() {
        _negative_picks = 0;
        _picks = 10;
        
        for (int i = 0; i < 9; ++i) {
            if (_button_variable[i] != 0) {
                int8_t cost = _ability_costs[i * 3 + _button_variable[i] - 1];
                _picks -= cost;
                if (cost < 0) {
                    _negative_picks += cost;
                }
            }
        }
        
        int8_t cost = _ability_costs[_button_variable[9] + 27];
        _picks -= cost;
        if (cost < 0) {
            _negative_picks += cost;
        }
        
        for (int i = 0; i < 22; ++i) {
            if (_option_toggle[i] == 1) {
                int8_t opt_cost = _ability_costs[i + 31];
                _picks -= opt_cost;
                if (opt_cost < 0) {
                    _negative_picks += opt_cost;
                }
            }
        }
    }

    void __cdecl Reload_Racial_Option_Screen_() {
        allocate::Push_Block_(MOX::_screen_seg);

        _special_button_seg = reinterpret_cast<s_animation_header*>(
            farload::Far_Reload_Next_(s_RACEOPT_LBX_00556320, 1, MOX::_screen_seg)
        );
        RUSS::Mox_Update_();

        _default_button_seg = static_cast<uint8_t*>(farload::Far_Reload_Next_(s_RACEOPT_LBX_00556320, 2, MOX::_screen_seg));
        RUSS::Mox_Update_();

        _accept_button_seg = static_cast<uint8_t*>(farload::Far_Reload_Next_(s_RACEOPT_LBX_00556320, 3, MOX::_screen_seg));
        RUSS::Mox_Update_();

        _off_button_seg = reinterpret_cast<s_animation_header*>(
            farload::Far_Reload_Next_(s_RACEOPT_LBX_00556320, 5, MOX::_screen_seg)
        );
        RUSS::Mox_Update_();

        Reload_Option_Strings_();
    }

    void __cdecl Draw_Racial_Option_Screen_() {
        int16_t x_coords[4] = { 0x50, 0xFE, 0x1BA, 0x283 };
        int16_t y_coords[5] = { 0x48, 0x93, 0xDE, 0x129, 0x174 };

        int16_t stack_vars[4];
        stack_vars[0] = 0x16;

        s_colors local_64;
        local_64.colors[0] = 0x89;
        local_64.colors[1] = 0x91;
        local_64.colors[2] = 0x91;
        local_64.colors[3] = 0x91;
        local_64.colors[4] = 0x91;
        local_64.colors[5] = 0x91;
        local_64.colors[6] = 0x91;
        local_64.colors[7] = 0x91;

        char local_50[12];
        memset(&local_50[1], 0x95, 7);

        fonts::Set_Font_Style_Shadow_Down_(3, &_color_opt);
        fonts::Set_Outline_Color_(0x77, 0);
        
        fonts::Print_Integer_Right_(0x12F, 0x1C1, _picks);
        
        int16_t modified_picks = (_picks + 10) * 10;
        if (modified_picks < 0) {
            modified_picks = 0;
        }
        
        fonts::Print_Integer_Right_(0x1B7, 0x1C1, modified_picks);
        fonts::Print_Right_(0x1C5, 0x1C1, s_percent_0055633c);

        int16_t col_idx = 0;
        int16_t item_idx = 0;
        int16_t loop1_i = 0;

        do {
            int16_t loop1_j = 0;
            int16_t y = y_coords[loop1_i];
            
            item_idx++;
            
            do {
                int group_idx = item_idx / 4;
                int16_t btn_var = _button_variable[group_idx];
                int16_t cost;
                
                if (btn_var == 0) {
                    cost = _ability_costs[loop1_j + group_idx * 3];
                } else {
                    cost = _ability_costs[loop1_j + group_idx * 3] - _ability_costs[btn_var + group_idx * 3 - 1];
                }
                
                int neg_idx = item_idx + (-3 - item_idx) / 4;
                
                bool draw_off = false;
                if (_group_button[item_idx] < 1 && (btn_var - loop1_j != 1)) draw_off = true;
                else if (_picks - cost < 0 && (btn_var - loop1_j != 1)) draw_off = true;
                else if (_ability_costs[neg_idx] + _negative_picks < -10 && (btn_var - loop1_j != 1)) draw_off = true;
                
                if (draw_off) {
                    animate::Draw_(x_coords[col_idx] - 0x14, y, _off_button_seg);
                } else if (btn_var - loop1_j == 1) {
                    animate::Set_Animation_Frame_(_special_button_seg, 1);
                    animate::Draw_(x_coords[col_idx] - 0x14, y, _special_button_seg);
                } else if (item_idx > 3 && item_idx < 8 && _option_toggle[9] == 1) {
                    animate::Draw_(x_coords[col_idx] - 0x14, y, _off_button_seg);
                }

                bool draw_text = true;
                if (btn_var - loop1_j == 1) {
                    fonts::Set_Font_Style_Shadow_Down_(2, &_color_opt);
                } else {
                    if (_ability_costs[neg_idx] + _negative_picks < -10 ||
                        _picks - cost < 0 ||
                        (item_idx > 3 && item_idx < 8 && _option_toggle[9] == 1) ||
                        _group_button[item_idx] == -1000) 
                    {
                        fonts::Set_Font_Style_Shadow_Down_(2, &local_64);
                    } else {
                        draw_text = false;
                    }
                }

                if (draw_text) {
                    char local_5c[12];
                    local_50[0] = s__00551100[0];
                    __itoa(_ability_costs[neg_idx], local_5c, 10);
                    strcpy(local_50, local_5c);

                    fonts::Print_(x_coords[col_idx], y, _opt_string[item_idx]);
                    int16_t str_w = fonts::Get_String_Width_(_opt_string[item_idx]);
                    
                    int dots_start = x_coords[col_idx] - 1 + str_w;
                    int target_x = x_coords[col_idx + 1] - fonts::Get_String_Width_(local_50) - 0x32;
                    
                    for (int x = dots_start; x < target_x - 4; x += 4) {
                        fonts::Print_(x, y, s_dot_0055632c);
                    }
                    
                    fonts::Print_Right_(x_coords[col_idx + 1] - 0x32, y, local_50);
                }
                
                y += 0xD;
                item_idx++;
                loop1_j++;
            } while (loop1_j < 3);

            loop1_i++;
            if (loop1_i >= 5) {
                loop1_i = 0;
                col_idx++;
            }
        } while (item_idx < 36);

        int16_t loop2_j = 0;
        int16_t y2 = y_coords[loop1_i];

        item_idx++;

        do {
            int16_t neg_idx = item_idx + (-3 - item_idx) / 4;
            int16_t cost_diff = _ability_costs[neg_idx] - _ability_costs[_button_variable[9] + 0x1B];
            stack_vars[3] = cost_diff;

            bool draw_off = false;
            if (_group_button[item_idx] < 0) draw_off = true;
            else if (_picks - cost_diff < 0 && loop2_j != _button_variable[9]) draw_off = true;
            else if (_negative_picks + _ability_costs[neg_idx] < -10 && loop2_j != _button_variable[9]) draw_off = true;

            if (draw_off) {
                animate::Draw_(x_coords[col_idx] - 0x14, y2, _off_button_seg);
            } else if (loop2_j == _button_variable[9]) {
                animate::Set_Animation_Frame_(_special_button_seg, 1);
                animate::Draw_(x_coords[col_idx] - 0x14, y2, _special_button_seg);
            }

            bool draw_text2 = true;
            if (loop2_j == _button_variable[9]) {
                fonts::Set_Font_Style_Shadow_Down_(2, &_color_opt);
            } else {
                if (_ability_costs[neg_idx] + _negative_picks < -10 || _picks - cost_diff < 0) {
                    fonts::Set_Font_Style_Shadow_Down_(2, &local_64);
                } else {
                    draw_text2 = false;
                }
            }

            if (draw_text2) {
                char local_5c[12];
                __itoa(_ability_costs[neg_idx], local_5c, 10);
                strcpy(local_50, local_5c);

                fonts::Print_(x_coords[col_idx], y2, _opt_string[item_idx]);
                int16_t str_w = fonts::Get_String_Width_(_opt_string[item_idx]);

                int dots_start = x_coords[col_idx] - 1 + str_w;
                int target_x = x_coords[col_idx + 1] - fonts::Get_String_Width_(local_50) - 0x32;

                for (int x = dots_start; x < target_x - 4; x += 4) {
                    fonts::Print_(x, y2, s_dot_0055632c);
                }

                fonts::Print_Right_(x_coords[col_idx + 1] - 0x32, y2, local_50);
            }

            y2 += 0xD;
            loop2_j++;
            item_idx++;
        } while (loop2_j < 4);

        stack_vars[2] = 0; // loop3_i
        stack_vars[1] = 0; // loop3_j
        int16_t y3 = 0x48;
        col_idx++;

        item_idx++;
        if (item_idx < 64) {
            do {
                int16_t k = 0;
                int16_t limit = (stack_vars[2] == 0) ? 22 : 0;

                if (limit > 0) {
                    do {
                        bool draw_text3 = true;
                        
                        if (_option_toggle[stack_vars[1]] == 1) {
                            animate::Set_Animation_Frame_(_special_button_seg, 1);
                            animate::Draw_(x_coords[col_idx] - 0x14, y3, _special_button_seg);
                            fonts::Set_Font_Style_Shadow_Down_(2, &_color_opt);
                        } else {
                            stack_vars[3] = Real_Ability_Cost_(k);
                            if (_option_button[stack_vars[1]] < 0 ||
                                _picks - stack_vars[3] < 0 ||
                                _ability_costs[item_idx - 11] + _negative_picks < -10) 
                            {
                                animate::Draw_(x_coords[col_idx] - 0x14, y3, _off_button_seg);
                                fonts::Set_Font_Style_Shadow_Down_(2, &local_64);
                            } else {
                                draw_text3 = false;
                            }
                        }

                        if (draw_text3) {
                            char local_5c[12];
                            __itoa(_ability_costs[item_idx - 11], local_5c, 10);
                            strcpy(local_50, local_5c);

                            fonts::Print_(x_coords[col_idx], y3, _opt_string[item_idx]);
                            int16_t str_w = fonts::Get_String_Width_(_opt_string[item_idx]);

                            int dots_start = x_coords[col_idx] - 1 + str_w;
                            int target_x = x_coords[col_idx + 1] - fonts::Get_String_Width_(local_50) - 0x32;

                            for (int x = dots_start; x < target_x - 4; x += 4) {
                                fonts::Print_(x, y3, s_dot_0055632c);
                            }

                            fonts::Print_Right_(x_coords[col_idx + 1] - 0x32, y3, local_50);
                        }

                        y3 += 0x10;
                        stack_vars[1]++;
                        k++;
                        item_idx++;
                    } while (k < limit);
                }

                y3 += 9;
                stack_vars[2]++;
            } while (item_idx < 64);
        }

        fonts::Set_Font_Style_Shadow_Down_(4, &_color_opt);
    }

    void __cdecl Convert_Player_To_Flags_EM_(int8_t* player_data) {
        _button_variable[9] = player_data[0] / 2;

        for (int16_t i = 0; i < 9; i++) {
            uint8_t val = player_data[i + 1];
            
            if (MOX::_special_bonus[i][0] == val) {
                _button_variable[i] = 1;
            } else if (MOX::_special_bonus[i][1] == val) {
                _button_variable[i] = 2;
            } else {
                _button_variable[i] = 3;
                if (MOX::_special_bonus[i][2] != val) {
                    _button_variable[i] = 0;
                }
            }
        }

        int16_t offset = 0;
        for (int16_t i = 10; i < 31; i++) {
            raceopt::_option_toggle[i + offset - 10] = player_data[i];
            
            if (i == 15) {
                if (raceopt::_option_toggle[5] == -1) {
                    raceopt::_option_toggle[5] = 0;
                    raceopt::_option_toggle[6] = 1;
                } else {
                    raceopt::_option_toggle[6] = 0;
                }
                offset = 1;
            }
        }
    }

    void __cdecl Set_Race_Option_Help_List_() {
        MOX::_help_entry_list[0] = 0x1bb;
        MOX::_help_entry_list[1] = 0x36;
        MOX::_help_entry_list[2] = 0x31;
        MOX::_help_entry_list[3] = 0xd1;
        MOX::_help_entry_list[4] = 0x6f;
        MOX::_help_entry_list[5] = 0x1bc;
        MOX::_help_entry_list[6] = 0x36;
        MOX::_help_entry_list[7] = 0x7b;
        MOX::_help_entry_list[8] = 0xd1;
        MOX::_help_entry_list[9] = 0xb9;
        MOX::_help_entry_list[10] = 0x1bd;
        MOX::_help_entry_list[11] = 0x36;
        MOX::_help_entry_list[12] = 0xc5;
        MOX::_help_entry_list[13] = 0xd0;
        MOX::_help_entry_list[14] = 0x103;
        MOX::_help_entry_list[15] = 0x1be;
        MOX::_help_entry_list[16] = 0x36;
        MOX::_help_entry_list[17] = 0x10f;
        MOX::_help_entry_list[18] = 0xd0;
        MOX::_help_entry_list[19] = 0x14d;
        MOX::_help_entry_list[20] = 0x1bf;
        MOX::_help_entry_list[21] = 0x36;
        MOX::_help_entry_list[22] = 0x159;
        MOX::_help_entry_list[23] = 0xd0;
        MOX::_help_entry_list[24] = 0x19b;
        MOX::_help_entry_list[25] = 0x1c0;
        MOX::_help_entry_list[26] = 0xe5;
        MOX::_help_entry_list[27] = 0x31;
        MOX::_help_entry_list[28] = 0x18b;
        MOX::_help_entry_list[29] = 0x70;
        MOX::_help_entry_list[30] = 0x1c1;
        MOX::_help_entry_list[31] = 0xe5;
        MOX::_help_entry_list[32] = 0x7b;
        MOX::_help_entry_list[33] = 0x18b;
        MOX::_help_entry_list[34] = 0xba;
        MOX::_help_entry_list[35] = 0x1c2;
        MOX::_help_entry_list[36] = 0xe5;
        MOX::_help_entry_list[37] = 0xc5;
        MOX::_help_entry_list[38] = 0x18b;
        MOX::_help_entry_list[39] = 0x104;
        MOX::_help_entry_list[40] = 0x1c3;
        MOX::_help_entry_list[41] = 0xe5;
        MOX::_help_entry_list[42] = 0x10f;
        MOX::_help_entry_list[43] = 0x18b;
        MOX::_help_entry_list[44] = 0x14e;
        MOX::_help_entry_list[45] = 0x1c4;

        MOX::_help_entry_list[50] = 0x1c5;
        MOX::_help_entry_list[55] = 0x1c6;
        MOX::_help_entry_list[60] = 0x1c7;
        MOX::_help_entry_list[65] = 0x1c8;
        MOX::_help_entry_list[70] = 0x1c9;
        MOX::_help_entry_list[75] = 0x1ca;
        MOX::_help_entry_list[80] = 0x1cb;
        MOX::_help_entry_list[85] = 0x1cc;
        MOX::_help_entry_list[90] = 0x1cd;
        MOX::_help_entry_list[95] = 0x1ce;
        MOX::_help_entry_list[100] = 0x1cf;
        MOX::_help_entry_list[105] = 0x1d0;
        MOX::_help_entry_list[110] = 0x1d1;
        MOX::_help_entry_list[115] = 0x1d2;
        MOX::_help_entry_list[120] = 0x1d3;
        MOX::_help_entry_list[125] = 0x1d4;
        MOX::_help_entry_list[130] = 0x1d5;
        MOX::_help_entry_list[135] = 0x1d6;
        MOX::_help_entry_list[140] = 0x1d7;
        MOX::_help_entry_list[145] = 0x1d8;
        MOX::_help_entry_list[150] = 0x1d9;

        int16_t list_val = 0x48;
        for (int i = 0; i < 22; ++i) {
            int16_t next_val = list_val + 9;
            MOX::_help_entry_list[i * 5 + 46] = 0x1a6;
            MOX::_help_entry_list[i * 5 + 47] = list_val;
            MOX::_help_entry_list[i * 5 + 48] = 0x252;
            MOX::_help_entry_list[i * 5 + 49] = next_val;
            list_val += 0x10;
        }

        MOX::_help_entry_list[155] = 0x1df;
        MOX::_help_entry_list[156] = 0xbe;
        MOX::_help_entry_list[157] = 0x1ba;
        MOX::_help_entry_list[158] = 0x135;
        MOX::_help_entry_list[159] = 0x1d0;
        MOX::_help_entry_list[160] = 0x1e0;
        MOX::_help_entry_list[161] = 0x146;
        MOX::_help_entry_list[162] = 0x1ba;
        MOX::_help_entry_list[163] = 0x1c9;
        MOX::_help_entry_list[164] = 0x1d1;
        MOX::_help_entry_list[165] = 0x1de;
        MOX::_help_entry_list[166] = 0x49;
        MOX::_help_entry_list[167] = 0x1be;
        MOX::_help_entry_list[168] = 0x92;
        MOX::_help_entry_list[169] = 0x1d7;
        MOX::_help_entry_list[170] = 0x1e1;
        MOX::_help_entry_list[171] = 0x1f8;
        MOX::_help_entry_list[172] = 0x1bf;
        MOX::_help_entry_list[173] = 0x23e;
        MOX::_help_entry_list[174] = 0x1d6;
        MOX::_help_entry_list[175] = 0x1da;
        MOX::_help_entry_list[176] = 0xe3;
        MOX::_help_entry_list[177] = 0x173;
        MOX::_help_entry_list[178] = 0x18e;
        MOX::_help_entry_list[179] = 0x17e;
        MOX::_help_entry_list[180] = 0x1db;
        MOX::_help_entry_list[181] = 0xe3;
        MOX::_help_entry_list[182] = 0x180;
        MOX::_help_entry_list[183] = 0x18e;
        MOX::_help_entry_list[184] = 0x18b;
        MOX::_help_entry_list[185] = 0x1dc;
        MOX::_help_entry_list[186] = 0xe3;
        MOX::_help_entry_list[187] = 0x18d;
        MOX::_help_entry_list[188] = 0x18e;
        MOX::_help_entry_list[189] = 0x198;
        MOX::_help_entry_list[190] = 0x1dd;
        MOX::_help_entry_list[191] = 0xe3;
        MOX::_help_entry_list[192] = 0x19a;
        MOX::_help_entry_list[193] = 0x18e;
        MOX::_help_entry_list[194] = 0x1a5;

        fields::Set_Help_List_(MOX::_help_entry_list, 0x27);
    }

    void __cdecl Convert_Flags_To_Player_EM_(int8_t *flags_out) {
        for (int i = 1; i < 10; ++i) {
            int16_t val = _button_variable[i - 1];
            if (val == 0) {
                flags_out[i] = 0;
            } else {
                flags_out[i] = MOX::_special_bonus[i - 1][val - 1];
            }
        }

        int offset = 0;
        for (int i = 10; i < 32; ++i) {
            if (i == 15) {
                if (_option_toggle[5] == 1) {
                    flags_out[15] = 1;
                } else if (_option_toggle[6] == 1) {
                    flags_out[15] = -1;
                } else {
                    flags_out[15] = 0;
                }
                offset = 1;
                i += offset;
            } else {
                flags_out[i - offset] = static_cast<int8_t>(_option_toggle[i - 10]);
            }
        }
    }

    int16_t __cdecl Real_Ability_Cost_(int16_t ability_idx) {
        int16_t cost = _ability_costs[ability_idx + 0x1F];

        if (_option_toggle[ability_idx] == 1) {
            cost = -cost;
        }

        if (ability_idx == 5 && _option_toggle[6] == 1) {
            cost -= _ability_costs[0x25];
        }

        if (ability_idx == 6 && _option_toggle[5] == 1) {
            cost -= _ability_costs[0x24];
        }

        if (ability_idx == 0 && _option_toggle[1] == 1) {
            cost -= _ability_costs[0x20];
        } else if (ability_idx == 1 && _option_toggle[0] == 1) {
            cost -= _ability_costs[0x1F];
        }

        if (ability_idx == 9) {
            if (_option_toggle[8] == 1) {
                cost -= _ability_costs[0x27];
            }
            if (_button_variable[1] != 0) {
                cost -= _ability_costs[_button_variable[1] + 2];
            }
        } else if (ability_idx == 8 && _option_toggle[9] == 1) {
            cost -= _ability_costs[0x28];
        }

        if (ability_idx == 10 && _option_toggle[11] == 1) {
            cost -= _ability_costs[0x2A];
        } else if (ability_idx == 11 && _option_toggle[10] == 1) {
            cost -= _ability_costs[0x29];
        }

        if (ability_idx == 13 && _option_toggle[12] == 1) {
            cost -= _ability_costs[0x2B];
        } else if (ability_idx == 12 && _option_toggle[13] == 1) {
            cost -= _ability_costs[0x2C];
        }

        return cost;
    }

    void __cdecl Add_Race_Options_Fields_EM_(int8_t *param_1, int16_t param_2) {
        int16_t x_coords[4] = { 80, 254, 442, 643 };
        int16_t y_coords[5] = { 72, 147, 222, 297, 372 };
        int16_t count_arr[7] = { 22 };

        fields::Clear_Fields_();
        _exit_ken = -1000;
        _accept = fields::Add_Button_Field_(506, 448, s__00551100, reinterpret_cast<s_animation_header*>(_accept_button_seg), s__00551100, 40);
        _defalt = -1000;

        int16_t col_idx = 0;
        int16_t row_idx = 0;
        int16_t group_btn_idx = 0;

        do {
            if ((group_btn_idx % 4) == 0) {
                _group_button[group_btn_idx] = -1000;
                group_btn_idx++;
            }

            int16_t sub_idx = 0;
            int16_t y = y_coords[row_idx] + 9;

            do {
                int16_t param1_idx = group_btn_idx / 4;
                int16_t cost = 0;

                if (param_1[param1_idx + 1] != 0) {
                    cost = _ability_costs[param_1[param1_idx] + param1_idx * 3 - 1];
                }

                int8_t ability_cost = _ability_costs[param1_idx * 3 + sub_idx];

                bool is_valid = false;
                if (ability_cost < 0) {
                    if (param_1[param1_idx + 1] == 1) {
                        is_valid = true;
                    }
                } else if (ability_cost > 0) {
                    if (param_1[param1_idx + 1] <= sub_idx + 1 || param_1[param1_idx + 1] < _button_variable[param1_idx]) {
                        is_valid = true;
                    }
                }

                if (is_valid && (ability_cost - cost <= param_2)) {
                    _group_button[group_btn_idx] = fields::Add_Hidden_Field_(
                        x_coords[col_idx] - 20, y - 9,
                        x_coords[col_idx + 1] - 50, y,
                        s__00551100, 41
                    );
                } else {
                    _group_button[group_btn_idx] = -1000;
                }

                y += 13;
                sub_idx++;
                group_btn_idx++;
            } while (sub_idx < 3);

            row_idx++;
            if (row_idx >= 5) {
                row_idx = 0;
                col_idx++;
            }
        } while (group_btn_idx < 36);

        _group_button[group_btn_idx++] = -1000;

        for (int16_t i = 0; i < 4; i++) {
            _group_button[group_btn_idx++] = -1000;
        }

        int16_t arr_idx = 0;
        int16_t y2 = 72;
        int16_t opt_btn_idx = 0;
        
        col_idx++;
        group_btn_idx++;

        while (group_btn_idx < 64) {
            int16_t sub2_idx = 0;
            int16_t inner_cnt = 0;
            int16_t count = count_arr[arr_idx];

            if (count > 0) {
                group_btn_idx += count;
                do {
                    if (opt_btn_idx >= 4 && opt_btn_idx <= 7) {
                        _option_button[opt_btn_idx] = -1000;
                    } else {
                        int8_t cost2 = _ability_costs[opt_btn_idx + 31];
                        bool valid2 = true;
                        
                        if (cost2 < 0) {
                            if (param_1[sub2_idx + opt_btn_idx + 10] == 0) {
                                valid2 = false;
                            }
                        } else if (cost2 > 0) {
                            if (param_1[sub2_idx + opt_btn_idx + 10] == 1) {
                                valid2 = false;
                            }
                        }

                        if (valid2 && cost2 <= param_2) {
                            _option_button[opt_btn_idx] = fields::Add_Hidden_Field_(
                                x_coords[col_idx] - 20, y2,
                                x_coords[col_idx] + 152, y2 + 9,
                                s__00551100, 41
                            );
                        } else {
                            _option_button[opt_btn_idx] = -1000;
                        }
                    }

                    if (inner_cnt == 6) {
                        sub2_idx = -1;
                    }
                    
                    y2 += 16;
                    opt_btn_idx++;
                    inner_cnt++;
                } while (inner_cnt < count);
            }
            
            y2 += 9;
            arr_idx++;
        }
    }

}