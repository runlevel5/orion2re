#include "pch.h"

namespace INFO {
    void __cdecl Colorize_Bitmap_(s_animation_header* anim, s_colors* color_table, uint8_t range_start, uint8_t range_end) {
        int32_t total_pixels = (int32_t)anim->height * (int32_t)anim->width;

        if (total_pixels > 0) {
            uint8_t* pixel_ptr = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(anim) + anim->frame_offsets[anim->current_frame]);

            while (total_pixels > 0) {
                uint8_t pixel = *pixel_ptr;

                if (pixel >= range_start && pixel <= range_end) {
                    *pixel_ptr = color_table->colors[pixel - range_start];
                }

                pixel_ptr++;
                total_pixels--;
            }
        }
    }

    int16_t __cdecl Initialize_Maintenance_Values_(int32_t* result_buffer) {
        int32_t sources[7];
        int32_t max_val = 0;
        int16_t non_zero_count = 0;

        // Populate source data from current player
        sources[0] = static_cast<int32_t>(MOX::_cur_plyr_ptr->bc_produced);
        for (int i = 0; i < 6; ++i) {
            sources[i + 1] = static_cast<int32_t>(MOX::_cur_plyr_ptr->maintenance[i]);
        }

        // Find the maximum value among the sources
        for (int i = 0; i < 7; ++i) {
            if (sources[i] > max_val) {
                max_val = sources[i];
            }
        }

        // Calculate normalized maintenance percentages if a max exists
        if (max_val > 0) {
            for (int i = 0; i < 7; ++i) {
                // Calculation includes rounding: ((val * 1000) / max + 5) / 10
                int32_t calculated = ((sources[i] * 1000) / max_val + 5) / 10;
                result_buffer[i] = calculated;
                if (calculated != 0) {
                    non_zero_count++;
                }
            }
        } else {
            // Zero out the destination buffer if no positive source value found
            for (int i = 0; i < 7; ++i) {
                result_buffer[i] = 0;
            }
        }

        return non_zero_count;
    }

    int16_t __cdecl Get_Plyr_Info_Btns_() {
        return (int16_t)((MOX::_player[MOX::_PLAYER_NUM].history_btns & 0x70) >> 4);
    }

    void __cdecl Get_Plyr_Hist_Btns_(int16_t* btns) {
        for (int16_t i = 0; i < 4; ++i) {
            btns[i] = MOX::_player[MOX::_PLAYER_NUM].history_btns & (1 << i);
        }
    }

    void Info_Screen_() {
        void* auto_func_var;
        int16_t current_tab;
        char stardate_str[16];
        s_animation_header* info_lbx[26];
        s_animation_header* maint_chart_labels[7];
        s_animation_header* maint_chart_bars[7];
        int32_t maint_values[7];

        MOX::_cur_plyr_ptr = &MOX::_player[MOX::_PLAYER_NUM];
        MOX2::Get_Auto_Function_(&auto_func_var);

        palette::Save_Palette_();
        fields::Clear_Fields_();
        ERIC::Fast_Fade_Out_();
        video::Set_Page_Off_();
        video::Set_Refresh_Stencil_();

        _g_reference_screen_type = 0;
        if (MOX::_return_screen == 0) {
            _g_turn_message_ix = 0;
        }

        *MOX::_active_player = static_cast<uint8_t>(MOX::_PLAYER_NUM);
        BILL::Get_Active_Players_(1, 0);

        info_lbx[0] = static_cast<s_animation_header*>(farload::Far_Reload_(s_INFO_LBX_00556000, 0, MOX::_screen_seg));
        for (int i = 1; i < 26; ++i) {
            info_lbx[i] = static_cast<s_animation_header*>(farload::Far_Reload_Next_(s_INFO_LBX_00556000, i, MOX::_screen_seg));
        }

        for (int i = 0; i < 7; ++i) {
            uint32_t size = bitmap::Get_Bitmap_Size_(0x18, 0x6C);
            maint_chart_bars[i] = static_cast<s_animation_header*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, size));
        }

        for (int i = 0; i < 7; ++i) {
            uint32_t size = bitmap::Get_Bitmap_Size_(0xA0, 0x14);
            maint_chart_labels[i] = static_cast<s_animation_header*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, size));
        }

        int16_t maint_values_count = Initialize_Maintenance_Values_(maint_values);
        Build_Maint_Chart_Bitmaps_(info_lbx, maint_chart_bars, maint_chart_labels, maint_values);

        palette::Set_Palette_Changes_(0, 255);
        animate::Draw_Palette_(info_lbx[1]);
        BILL::Turn_Off_Palettes_(0, 255);
        JIM::Bill_Picture_To_Mouse_(info_lbx[1]);

        current_tab = Get_Plyr_Info_Btns_();

        _g_exit_field = fields::Add_Button_Field_(0x217, 0x1B2, s__00551100, info_lbx[2], s__00551120, '(');

        for (int i = 0; i < 5; ++i) {
            fields::Add_Multi_Button_Field_(
                _sub_scr_field[i * 2],
                _sub_scr_field[i * 2 + 1],
                s__00551100,
                info_lbx[3 + i],
                &current_tab,
                i,
                s__00551100,
                0x29
            );
        }

        int16_t field_counter = fields::Get_Field_Counter_();

        const char* format = (MOX::_settings.language == 0) ? s_fmt_dot_00555ff8 : s_fmt_comma_00555ff0;
        _sprintf(stardate_str, format, (int)(MOX::_stardate / 10), (int)(MOX::_stardate % 10));

        _race_statistics_page = 1;
        int16_t input_field = static_cast<int16_t>(reinterpret_cast<std::uintptr_t>(info_lbx[0]));

        do {
            *MOX::_active_player = static_cast<uint8_t>(MOX::_PLAYER_NUM);
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            BILL::Get_Active_Players_(1, 0);

            graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
            fonts::Set_Outline_Color_(0xB0, 0xB0);
            graphics::Tri_Interlaced_Fill_(0xD4, 0x17, 0x26C, 0x1C9, 0xB2, 0xB3, 0xB3);

            animate::Draw_(0, 0, info_lbx[0]);

            fonts::Set_Font_Style_(4, &_info_normal_color);
            fonts::Print_Centered_(0x97, 0x1B, stardate_str);

            Draw_Maint_Income_Chart_(maint_chart_bars, maint_chart_labels, maint_values_count - 1);

            allocate::Push_Block_(MOX::_screen_seg);

            uint32_t saved_mark_pos = allocate::_mark_stack_pos[0];

            fields::Clear_Fields_Above_(field_counter - 1);
            palette::Reset_Cycle_Palette_Color_();
            fonts::Set_Font_Style_(5, &_info_normal_color);

            switch (current_tab) {
                case 0:
                    BILLHELP::History_Graph_Help_();
                    input_field = History_Subscreen_(info_lbx, field_counter);
                    break;
                case 1:
                    BILLHELP::Tech_Review_Help_();
                    input_field = Tech_Review_Subscreen_(info_lbx, field_counter);
                    RACERPRT::_race_report_jim_hack = 0;
                    break;
                case 2:
                    BILLHELP::Race_Statistics_Help_();
                    input_field = Race_Stats_Subscreen_(info_lbx, field_counter);
                    break;
                case 3:
                    BILLHELP::Turn_Summary_Help_();
                    input_field = Turn_Summary_Subscreen_(info_lbx, field_counter);
                    break;
                case 4:
                    input_field = Reference_Subscreen_(info_lbx, field_counter);
                    break;
                default:
                    break;
            }

            if (saved_mark_pos < allocate::_mark_stack_pos[0]) {
                allocate::_mark_stack_pos[0] = saved_mark_pos;
            }

            allocate::Pop_Block_(MOX::_screen_seg);
            fields::Deactivate_Help_List_();

        } while (_g_exit_field != input_field);

        BILL::Set_Plyr_Info_Btns_(current_tab);

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();

        MOX::_current_screen = 0;
    }

    void __cdecl Build_Maint_Chart_Bitmaps_(s_animation_header** sprites, s_animation_header** bars, s_animation_header** labels, int32_t* values) {
        s_animation_header* mask_sprite;
        s_animation_header* bottom_sprite;
        s_animation_header* tall_bg_sprite;
        s_animation_header* label_bg_sprite;
        char buffer[64];
        char lbx_msg[64];

        allocate::Push_Block_(MOX::_screen_seg);

        uint32_t size_mask = bitmap::Get_Bitmap_Size_(24, 9);
        mask_sprite = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size_mask);
        bitmap::Create_Blank_Bitmap_(24, 9, mask_sprite);
        bitmap::Add_Picture_To_Bitmap_(0, 0, sprites[23], mask_sprite);

        uint32_t size_bottom = bitmap::Get_Bitmap_Size_(24, 6);
        bottom_sprite = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size_bottom);
        bitmap::Create_Blank_Bitmap_(24, 6, bottom_sprite);
        bitmap::Add_Picture_To_Bitmap_(0, 0, sprites[25], bottom_sprite);

        uint32_t size_tall = bitmap::Get_Bitmap_Size_(24, 108);
        tall_bg_sprite = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size_tall);
        bitmap::Create_Blank_Bitmap_(24, 108, tall_bg_sprite);
        bitmap::Add_Picture_To_Bitmap_(0, 0, sprites[22], tall_bg_sprite);

        label_bg_sprite = sprites[24];

        for (int16_t i = 0; i < 7; ++i) {
            if (values[i] < 1 && i != 0) {
                bars[i] = nullptr;
            } else {
                s_animation_header* bar = bars[i];
                bitmap::Create_Blank_Bitmap_(24, 108, bar);
                bitmap::Add_Picture_To_Bitmap_(0, (int16_t)(100 - values[i]), tall_bg_sprite, bar);
                bitmap::Mask_Out_Bitmap_With_Bitmap_(0, 99, bar, mask_sprite, 1);
                bitmap::Add_Picture_To_Bitmap_(0, 102, bottom_sprite, bar);
                INFO::Colorize_Bitmap_(bar, INFO::_maintenance_bar_colors[i], 1, 7);
            }
        }

        for (int16_t i = 0; i < 7; ++i) {
            s_animation_header* label = labels[i];
            bitmap::Create_Blank_Bitmap_(160, 20, label);
            bitmap::Add_Picture_To_Bitmap_(0, 0, label_bg_sprite, label);
            INFO::Colorize_Bitmap_(label, INFO::_maintenance_bar_colors[i], 1, 7);
            fonts::Set_Font_Style_(2, INFO::PTR__orange_text_colors[i]);

            if (i == 0) {
                _sprintf(buffer, s_int_fmt, (int)MOX::_cur_plyr_ptr->bc_produced);
                JIM::Get_Text_Message_(s_billtext_lbx, 19, lbx_msg, 40);
                strcat(buffer, lbx_msg);

                if (fonts::Get_String_Width_(buffer) > 126) {
                    fonts::Set_Font_Spacing_(1);
                }

                fonts::Print_Centered_To_Bitmap_(92, 3, buffer, label);
                fonts::_font_header.font_style_spacing = fonts::_font_header.font_spacing[2];
            } else {
                _sprintf(buffer, s_pct_fmt, (int)values[i]);
                fonts::Print_To_Bitmap_(27, 3, buffer, label);

                s_colors text_colors;
                for (int j = 0; j < 8; ++j) {
                    text_colors.colors[j] = (uint8_t)(j / 2) + 1;
                }

                uint8_t base_color = 0;
                switch (i) {
                    case 1: base_color = 0x14; break;
                    case 2: base_color = 0xA8; break;
                    case 3: base_color = 0xAE; break;
                    case 4: base_color = 0xA2; break;
                    case 5: base_color = 0x97; break;
                    case 6: base_color = 0xB9; break;
                }

                text_colors.colors[0] = base_color;
                fonts::Set_Font_Style_(2, &text_colors);
                JIM::Get_Text_Message_(s_billtext_lbx, (int16_t)(i + 19), lbx_msg, 40);
                fonts::Print_Centered_To_Bitmap_(108, 3, lbx_msg, label);
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
        INFO::_info_dark_color = 0xB0;
    }

    void __cdecl Draw_Maint_Income_Chart_(s_animation_header** headers_a, s_animation_header** headers_b, int16_t param_3) {
        char msg_top[40];
        char msg_bottom[40];
        char msg_stat[64];

        animate::Draw_(30, 212, headers_a[0]);

        if (param_3 > 0) {
            int16_t draw_x = (int16_t)(((param_3 * 18) - 42) / 2 + 120);
            for (int i = 6; i >= 1; --i) {
                if (headers_a[i] != nullptr) {
                    animate::Draw_(draw_x, 212, headers_a[i]);
                    draw_x -= 18;
                }
            }
        }

        int16_t draw_y = 344;
        for (int i = 0; i < 7; ++i) {
            animate::Draw_(25, draw_y, headers_b[i]);
            draw_y += 16;
        }

        fonts::Set_Font_Style_(1, &INFO::_info_normal_color);

        JIM::Get_Text_Message_(s_billtext_lbx, 17, msg_top, 40);
        fonts::Print_Centered_(42, 324, msg_top);

        JIM::Get_Text_Message_(s_billtext_lbx, 18, msg_bottom, 40);
        fonts::Print_Centered_(120, 324, msg_bottom);

        int32_t net_income = (int32_t)MOX::_cur_plyr_ptr->bc_produced - MOX::_cur_plyr_ptr->total_maintenance;

        s_colors* stat_color;
        if (net_income < 0) {
            stat_color = &INFO::_info_red_color;
        } else {
            stat_color = &INFO::_info_normal_color;
        }

        fonts::Set_Font_Style_(2, stat_color);

        JIM::Get_Text_Message_(s_billtext_lbx, 27, msg_stat, 64);
        JIM::Decode_Text_Stat_(msg_stat, net_income, 64);
        fonts::Print_Centered_(105, 196, msg_stat);
    }

    void __cdecl Set_Up_Player_Colors_(s_animation_header** anim_ptrs) {
        palette::Set_Palette_Changes_(0xC0, 0xFF);
        animate::Draw_Palette_(anim_ptrs[1]);

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            uint8_t color_idx = static_cast<uint8_t>(MOX::_player[i].color);
            INFO::_hist_graph_palettes[i] = &INFO::_hist_palettes[color_idx];
        }
    }

    void __cdecl Draw_History_Subscreen_() {
        // This function is a no-op in the original binary
    }

    int16_t __cdecl History_Subscreen_(s_animation_header** lbx_images, int16_t exit_threshold) {
        char msg_buffer[40];
        int16_t player_hist_buttons[4];
        int16_t input;

        JIM::Get_Text_Message_(lbx_billtext, 5, msg_buffer, 40);
        fonts::Print_Centered_(416 /* 0x1a0 */, 31 /* 0x1f */, msg_buffer);

        INFO::Set_Up_Player_Colors_(lbx_images);
        *MOX::_active_player = static_cast<int8_t>(MOX::_PLAYER_NUM);

        BILL::Get_Players_Alive_Or_Omniscient_(1, 0);
        INFO::Get_Plyr_Hist_Btns_(player_hist_buttons);

        // Ensure at least one graph is active
        if ((player_hist_buttons[0] + player_hist_buttons[1] + player_hist_buttons[2] + player_hist_buttons[3]) == 0) {
            player_hist_buttons[0] = 1;
            player_hist_buttons[1] = 1;
            player_hist_buttons[2] = 1;
            player_hist_buttons[3] = 1;
            INFO::Set_Plyr_Hist_Btns_(player_hist_buttons);
        }

        for (int i = 0; i < 4; ++i) {
            fields::Add_Radio_Button_Field_(
                INFO::_graph_button_x[i],
                427 /* 0x1ab */,
                empty_string,
                lbx_images[i + 8],
                &player_hist_buttons[i],
                empty_string,
                40 /* 0x28 */
            );
        }

        fields::Assign_Auto_Function_(INFO::Draw_History_Subscreen_, 2);

        while (true) {
            INFO::Draw_Multiple_Drop_Areas_(INFO::_history_graph_drops, 2);
            INFO::Display_Graphed_Players_Names_();
            int16_t graph_data = INFO::Draw_The_History_Graph_();
            INFO::Draw_Histories_(graph_data);
            video::Copy_Off_To_Back_();
            INFO::Introduce_Subscreen_();

            while (true) {
                timer::Mark_Time_();
                input = fields::Get_Input_();

                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    return 0;
                }

                if (input < exit_threshold) {
                    if (input == 0) {
                        INFO::Draw_History_Subscreen_();
                        RUSS::Mox_Sync_Update_();
                        timer::Release_Time_(2);
                        // Continue input waiting
                    } else {
                        return input;
                    }
                } else {
                    // Update state and refresh entire screen
                    INFO::Set_Plyr_Hist_Btns_(player_hist_buttons);
                    break;
                }
            }
        }
    }

    void __cdecl Set_Plyr_Hist_Btns_(int16_t* btns_state) {
        for (int16_t i = 0; i < 4; ++i) {
            // Re-fetch current player index and state on each iteration as per assembly
            int16_t player_idx = MOX::_PLAYER_NUM;
            uint8_t mask = 1 << i;

            if (btns_state[i] != 0) {
                MOX::_player[player_idx].history_btns |= mask;
            } else {
                MOX::_player[player_idx].history_btns &= ~mask;
            }
        }
    }

    void __cdecl Draw_Histories_(int16_t offset_x) {
        int16_t btns[4];
        int32_t history_data[8][350];
        int16_t n_points;
        char buffer[32];

        Get_Plyr_Hist_Btns_(btns);

        int16_t active_btns = 0;
        for (int i = 0; i < 4; ++i) {
            if (btns[i] != 0) {
                active_btns++;
            }
        }

        if (active_btns == 0) {
            return;
        }

        int16_t use_pop = btns[0];
        int16_t use_prod = btns[1];
        int16_t use_fleet = btns[2];
        int16_t use_tech = btns[3];

        for (int16_t p = MOX::_active_count - 1; p >= 0; --p) {
            uint8_t player_idx = MOX::_active_player[p];
            s_player* plr = &MOX::_player[player_idx];

            int16_t hist_idx = BILL::Get_History_Params_(&n_points);
            if (n_points > 0) {
                for (int16_t t = 0; t < n_points; ++t) {
                    int32_t val = 0;

                    if (use_pop != 0) {
                        val += static_cast<uint8_t>(plr->population_history[hist_idx]) * MOX::_bill_savegame_2;
                    }
                    if (use_tech != 0) {
                        val += static_cast<uint8_t>(plr->tech_history[hist_idx]) * MOX::_bill_savegame_5;
                    }
                    if (use_fleet != 0) {
                        val += static_cast<uint8_t>(plr->fleet_history[hist_idx]) * MOX::_bill_savegame_4;
                    }
                    if (use_prod != 0) {
                        val += static_cast<uint8_t>(plr->production_history[hist_idx]) * MOX::_bill_savegame_3;
                    }

                    history_data[player_idx][t] = val;

                    hist_idx++;
                    if (hist_idx >= 350) {
                        hist_idx = 0;
                    }
                }
            }
        }

        for (int16_t p = MOX::_active_count - 1; p >= 0; --p) {
            uint8_t player_idx = MOX::_active_player[p];
            for (int i = 0; i < 10; ++i) {
                if (n_points > 0) {
                    for (int16_t t = 0; t < n_points; ++t) {
                        Smooth_History_Data_(history_data[player_idx], t, n_points);
                    }
                }
            }
        }

        int32_t max_val = 0;
        for (int16_t p = MOX::_active_count - 1; p >= 0; --p) {
            uint8_t player_idx = MOX::_active_player[p];
            if (n_points > 0) {
                for (int16_t t = 0; t < n_points; ++t) {
                    if (history_data[player_idx][t] > max_val) {
                        max_val = history_data[player_idx][t];
                    }
                }
            }
        }

        int32_t max_scale = 25;
        bool scale_found = false;
        while (!scale_found) {
            if (max_val < max_scale) {
                scale_found = true;
            } else {
                if (max_scale == 25) max_scale = 50;
                else if (max_scale == 50) max_scale = 100;
                else if (max_scale == 100) max_scale = 250;
                else if (max_scale == 250) max_scale = 500;
                else if (max_scale == 500) max_scale = 1000;
                else if (max_scale == 1000) max_scale = 2000;
                else max_scale += 2000;
            }
        }

        int32_t divisor = (max_scale * 100) / 250;
        for (int16_t p = MOX::_active_count - 1; p >= 0; --p) {
            uint8_t player_idx = MOX::_active_player[p];
            if (n_points > 0) {
                for (int16_t t = 0; t < n_points; ++t) {
                    history_data[player_idx][t] = (history_data[player_idx][t] * 100) / divisor;
                }
            }
        }

        fonts::Set_Font_Style_(1, &INFO::_info_normal_color);
        __itoa(max_scale, buffer, 10);
        fonts::Print_(0xF6, 0x8A, buffer);

        int16_t draw_count = n_points - 1;
        for (int16_t p = MOX::_active_count - 1; p >= 0; --p) {
            uint8_t player_idx = MOX::_active_player[p];
            uint8_t color = INFO::_hist_graph_palettes[player_idx]->colors[7];

            if (draw_count > 0) {
                int16_t x = 0xEE;
                for (int16_t t = 0; t < draw_count; ++t) {
                    int16_t y1 = 0x186 - (int16_t)history_data[player_idx][t];
                    int16_t y2 = 0x186 - (int16_t)history_data[player_idx][t + 1];

                    line::Line_(x + 1, y1 + 1, offset_x + x + 1, y2 + 1, 0xB1);
                    line::Line_(x, y1, offset_x + x, y2, color);
                    x = offset_x + x;
                }
            }
        }
    }

    void __cdecl Display_Graphed_Players_Names_() {
        int16_t x_pos = 351;
        int16_t y_pos = 64;
        int16_t spacing;
        uint16_t active_count = (uint16_t)MOX::_active_count;

        if (active_count > 3) {
            spacing = 150;
            x_pos = 295;
        }

        if (active_count > 6) {
            spacing = 122;
            x_pos = 233;
        }

        if (active_count > 0) {
            for (int16_t i = 0; i < (int16_t)active_count; ++i) {
                int8_t player_idx = (int8_t)MOX::_active_player[i];

                fonts::Set_Font_Style_(4, INFO::_hist_graph_palettes[player_idx]);

                fonts::Print_(x_pos, y_pos, MOX::_player[player_idx].race_name);

                y_pos += 19;

                if (i == 2 || i == 5) {
                    // BUG: spacing might be uninitialized if active_count is exactly 3
                    x_pos += spacing;
                    y_pos = 64;
                }
            }
        }
    }

    void __cdecl Draw_Multiple_Drop_Areas_(s_drop_area* areas, uint32_t count) {
        // The count is treated as a 16-bit unsigned integer.
        uint16_t num_areas = static_cast<uint16_t>(count);
        s_drop_area* end = areas + num_areas;

        // Iterate through the array of drop areas and draw each one.
        while (areas < end) {
            Draw_Drop_Area_(areas);
            areas++;
        }
    }

    void __cdecl Draw_Drop_Area_(s_drop_area* area) {
        // Top section
        graphics::Tri_Interlaced_Fill_(
            area->x1 + 2, area->y1, area->x2 - 2, area->y1 + 2,
            area->colors_main[0], area->colors_main[1], area->colors_main[2]
        );
        graphics::Tri_Interlaced_Fill_(
            area->x1 + 2, area->y1, area->x1 + 2, area->y1 + 2,
            area->colors_left[0], area->colors_left[1], area->colors_left[2]
        );
        graphics::Tri_Interlaced_Fill_(
            area->x2 - 2, area->y1, area->x2 - 2, area->y1 + 2,
            area->colors_right[0], area->colors_right[1], area->colors_right[2]
        );

        // Middle section
        graphics::Tri_Interlaced_Fill_(
            area->x1, area->y1 + 3, area->x2, area->y2 - 3,
            area->colors_main[0], area->colors_main[1], area->colors_main[2]
        );
        graphics::Tri_Interlaced_Fill_(
            area->x1, area->y1 + 3, area->x1, area->y2 - 3,
            area->colors_left[0], area->colors_left[1], area->colors_left[2]
        );
        graphics::Tri_Interlaced_Fill_(
            area->x2, area->y1 + 3, area->x2, area->y2 - 3,
            area->colors_right[0], area->colors_right[1], area->colors_right[2]
        );

        // Bottom section
        graphics::Tri_Interlaced_Fill_(
            area->x1 + 2, area->y2 - 2, area->x2 - 2, area->y2,
            area->colors_main[0], area->colors_main[1], area->colors_main[2]
        );
        graphics::Tri_Interlaced_Fill_(
            area->x1 + 2, area->y2 - 2, area->x1 + 2, area->y2,
            area->colors_left[0], area->colors_left[1], area->colors_left[2]
        );
        graphics::Tri_Interlaced_Fill_(
            area->x2 - 2, area->y2 - 2, area->x2 - 2, area->y2,
            area->colors_right[0], area->colors_right[1], area->colors_right[2]
        );
    }

    int32_t __cdecl Smooth_History_Data_(int32_t* data, int16_t start_idx, int16_t end_idx) {
        if (end_idx <= start_idx) {
            return 0;
        }

        int32_t val0 = data[start_idx];

        int32_t val1;
        if (start_idx + 1 < end_idx) {
            val1 = data[start_idx + 1];
        } else {
            val1 = val0;
        }

        int32_t val2;
        if (start_idx + 2 < end_idx) {
            val2 = data[start_idx + 2];
        } else {
            val2 = val0;
        }

        uint32_t sum = (uint32_t)val0 + (uint32_t)val1 + (uint32_t)val2;
        int32_t average = (int32_t)(sum / 3);

        int32_t result = abs(val1 - average);

        if (start_idx + 1 < end_idx) {
            data[start_idx + 1] = average;
        }

        return result;
    }

    void __cdecl Introduce_Subscreen_() {
        if (ERIC::Get_Fade_Percent_() != 0) {
            palette::Clear_Palette_Changes_(0, 0xFF);
            RUSS::Mox_Sync_Update_();
            BILL::Turn_Off_Palettes_(0, 0xFF);
            ERIC::Fast_Fade_In_();
        } else {
            fade::Blinds_Fade_(0, 0, 639, 479, 3);
        }
    }

    int16_t __cdecl Draw_The_History_Graph_() {
        uint16_t offset_idx = 0;
        uint16_t x = 0xed; /* 237 */

        do {
            uint8_t offset = (uint8_t)INFO::_graph_offsets[offset_idx++];
            if (offset_idx >= 10) {
                offset_idx = 0;
            }

            line::Line_(x, 0x185 /* 389 */, x, offset + 0x186 /* 390 */, 0xb1 /* 177 */);
            line::Line_(x + 1, 0x185, x + 1, offset + 0x186, 0xb9 /* 185 */);
            x += 10;
        } while (x <= 0x24c /* 588 */);

        line::Line_(0xe8 /* 232 */, 0x186 /* 390 */, 0x24c /* 588 */, 0x186, 0xb9);
        line::Line_(0xed /* 237 */, 0x8c /* 140 */, 0xed, 0x18c /* 396 */, 0xb1);

        offset_idx = 0;
        line::Line_(0xee /* 238 */, 0x8c, 0xee, 0x18c, 0xb9);

        uint16_t y = 0x186; /* 390 */
        do {
            uint8_t offset = (uint8_t)INFO::_graph_offsets[offset_idx++];
            if (offset_idx >= 10) {
                offset_idx = 0;
            }

            graphics::Dot_(0xed - offset - 1, y, 0xb1);
            line::Line_(0xee - offset, y, 0xef /* 239 */, y, 0xb9);
            y -= 10;
        } while (y >= 0x8c /* 140 */);

        fonts::Set_Font_Style_(1, &INFO::_info_normal_color);

        int32_t current_val = 0x88b8; /* 35000 */
        int16_t return_val;
        int16_t step;

        uint16_t check_date = (uint16_t)MOX::_stardate + 0x7748 /* 30536 */;

        if (check_date <= 0x23 /* 35 */) {
            return_val = 10;
            step = 5;
        } else if (check_date <= 0x46 /* 70 */) {
            return_val = 5;
            step = 10;
        } else if (check_date <= 0xaf /* 175 */) {
            return_val = 2;
            step = 25 /* 0x19 */;
        } else {
            return_val = 1;
            step = 50 /* 0x32 */;
            if (check_date > 0x15e /* 350 */) {
                current_val = ((int32_t)(MOX::_stardate - 0x159 /* 345 */) / 10) * 10;
            }
        }

        uint16_t label_x = 0xef; /* 239 */
        char buffer[16];
        for (int i = 0; i < 8; ++i) {
            int32_t whole = current_val / 10;
            int32_t fraction = current_val % 10;

            if ((current_val % 5 == 0) && (fraction != 0)) {
                if (MOX::_settings.language == 0) {
                    _sprintf(buffer, INFO::s_fmt_dot, whole, fraction);
                } else {
                    _sprintf(buffer, INFO::s_fmt_comma, whole, fraction);
                }
            } else {
                __itoa(whole, buffer, 10);
            }

            fonts::Print_Centered_(label_x, 400, buffer);
            label_x += 0x32;
            current_val += step;
        }

        return return_val;
    }

}