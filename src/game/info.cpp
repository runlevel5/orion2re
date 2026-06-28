#include "pch.h"

namespace {
    int16_t _turn_summary_list_y[16] = {
        0, 10, 20, 30, 40, 50, 60, 70,
        80, 90, 100, 110, 120, 130, 140, 0,
    };

    int16_t _turn_summary_field_y2[16] = {
        9, 19, 29, 39, 49, 59, 69, 79,
        89, 99, 109, 119, 129, 139, 149, 0,
    };

    int16_t _category_list_y[2] = {0, 17};
    int16_t _category_field_y2[1] = {17};

    void __cdecl Draw_Tech_Review_App_(int16_t x, int16_t y, uint8_t tech_app_id) {
        char* name = TECH::Technology_Applications_Name_(tech_app_id, MOX::_PLAYER_NUM);
        FMTPARA::Print_Formatted_Paragraph_(x, y, 197, name, 0);
    }

    void __cdecl Draw_Turn_Summary_App_(int16_t x, int16_t y, uint8_t msg_idx) {
        if (msg_idx <= 0xFA) {
            char buffer[256];

            fonts::Set_Font_LF_(10);
            MSG_::sprintf_msg_(buffer, &MSG_::_msgs[msg_idx]);
            fonts::Print_(x, y, "^");
            FMTPARA::Print_Formatted_Paragraph_(x + 13, y, 345, buffer, 0);
        }
    }
}

namespace INFO {
    s_tech_review_achievement _review_achievements[60] = {
        {1, 4}, {55, 2}, {16, 2}, {186, 2}, {189, 2}, {109, 2}, {69, 2}, {41, 2}, {40, 2}, {2, 4},
        {43, 2}, {173, 2}, {147, 2}, {182, 2}, {114, 2}, {166, 2}, {3, 4}, {170, 2}, {99, 2}, {195, 2},
        {29, 2}, {8, 2}, {7, 2}, {6, 2}, {3, 2}, {113, 2}, {108, 2}, {193, 2}, {107, 2}, {5, 4},
        {89, 2}, {46, 2}, {59, 2}, {4, 2}, {179, 2}, {4, 4}, {24, 2}, {144, 2}, {124, 2}, {9, 2},
        {138, 2}, {128, 2}, {73, 2}, {101, 2}, {145, 2}, {0, 4}, {91, 2}, {176, 2}, {180, 2}, {92, 2},
        {77, 2}, {65, 2}, {42, 2}, {200, 2}, {84, 2}, {62, 2}, {32, 2}, {0, 8}, {0, 0}, {0, 0},
    };

    s_tech_review_achievement _review_colony[56] = {
        {6, 4}, {198, 2}, {68, 2}, {162, 2}, {87, 2}, {74, 2}, {178, 2}, {183, 2}, {7, 4}, {50, 2},
        {19, 2}, {142, 2}, {9, 4}, {49, 2}, {154, 2}, {152, 2}, {156, 2}, {22, 2}, {11, 4}, {76, 2},
        {136, 2}, {21, 2}, {155, 2}, {12, 4}, {75, 2}, {135, 2}, {164, 2}, {10, 4}, {15, 2}, {129, 2},
        {130, 2}, {134, 2}, {169, 2}, {27, 2}, {168, 2}, {133, 2}, {132, 2}, {67, 2}, {14, 2}, {103, 2},
        {8, 4}, {141, 2}, {86, 2}, {0, 4}, {61, 2}, {197, 2}, {52, 2}, {5, 2}, {131, 2}, {39, 2},
        {163, 2}, {18, 2}, {0, 8}, {0, 0}, {0, 0}, {0, 0},
    };

    s_tech_review_achievement _review_weapons[44] = {
        {13, 4}, {174, 2}, {47, 2}, {123, 2}, {105, 2}, {54, 2}, {137, 2}, {79, 2}, {127, 2}, {97, 2},
        {115, 2}, {78, 2}, {70, 2}, {104, 2}, {100, 2}, {14, 4}, {139, 2}, {146, 2}, {12, 2}, {202, 2},
        {149, 2}, {106, 2}, {121, 2}, {15, 4}, {28, 2}, {48, 2}, {118, 2}, {10, 2}, {71, 2}, {119, 2},
        {16, 4}, {83, 2}, {31, 2}, {66, 2}, {17, 2}, {17, 4}, {165, 2}, {30, 2}, {171, 2}, {148, 2},
        {140, 2}, {80, 2}, {13, 2}, {0, 8},
    };

    s_tech_review_achievement _review_equipment[80] = {
        {18, 4}, {81, 2}, {112, 2}, {161, 2}, {37, 2}, {36, 2}, {35, 2}, {34, 2}, {33, 2}, {19, 4},
        {56, 2}, {82, 2}, {201, 2}, {2, 2}, {117, 2}, {203, 2}, {191, 2}, {187, 2}, {20, 4}, {20, 2},
        {95, 2}, {88, 2}, {11, 2}, {96, 2}, {72, 2}, {120, 2}, {22, 4}, {63, 2}, {184, 2}, {194, 2},
        {98, 2}, {51, 2}, {167, 2}, {21, 4}, {110, 2}, {44, 2}, {143, 2}, {122, 2}, {58, 2}, {24, 4},
        {160, 2}, {116, 2}, {181, 2}, {157, 2}, {23, 4}, {45, 2}, {153, 2}, {60, 2}, {53, 2}, {102, 2},
        {126, 2}, {38, 2}, {93, 2}, {94, 2}, {199, 2}, {111, 2}, {57, 2}, {25, 4}, {1, 2}, {175, 2},
        {85, 2}, {151, 2}, {26, 2}, {0, 4}, {150, 2}, {125, 2}, {185, 2}, {90, 2}, {177, 2}, {190, 2},
        {188, 2}, {23, 2}, {172, 2}, {159, 2}, {64, 2}, {196, 2}, {158, 2}, {25, 2}, {192, 2}, {0, 8},
    };

    int16_t _tech_review_list_y[16] = {
        16, 29, 42, 55, 68, 81, 94, 107,
        120, 133, 146, 159, 172, 185, 198, 211,
    };

    int16_t _tech_review_field_y1[16] = {
        0, 27, 40, 53, 66, 79, 92, 105,
        118, 131, 144, 157, 170, 183, 196, 209,
    };

    int16_t _tech_review_field_y2[16] = {
        26, 39, 52, 65, 78, 91, 104, 117,
        130, 143, 156, 169, 182, 195, 208, 221,
    };

    s_colors _info_jim_hack_color = {{178, 190, 183, 185, 187, 188, 189, 189}};
    s_colors _orange_bar_colors = {{176, 152, 153, 154, 155, 156, 157, 0}};
    s_colors _grey_bar_colors = {{176, 13, 15, 17, 19, 20, 22, 0}};
    s_colors _red_bar_colors = {{176, 164, 165, 166, 167, 168, 169, 0}};
    s_colors _purple_bar_colors = {{176, 170, 171, 172, 173, 174, 175, 0}};
    s_colors _yellow_bar_colors = {{176, 158, 159, 160, 161, 162, 163, 0}};
    s_colors _blue_bar_colors = {{176, 146, 147, 148, 149, 150, 151, 0}};
    s_colors _green_bar_colors = {{176, 181, 182, 183, 184, 185, 186, 0}};

    s_colors _orange_text_colors = {{155, 2, 2, 2, 3, 3, 4, 4}};
    s_colors _grey_text_colors = {{13, 2, 2, 2, 3, 3, 4, 4}};
    s_colors _red_text_colors = {{164, 2, 2, 2, 3, 3, 4, 4}};
    s_colors _purple_text_colors = {{170, 2, 2, 2, 3, 3, 4, 4}};
    s_colors _yellow_text_colors = {{158, 2, 2, 2, 3, 3, 4, 4}};
    s_colors _blue_text_colors = {{146, 2, 3, 3, 3, 3, 4, 4}};
    s_colors _green_text_colors = {{181, 2, 2, 2, 3, 3, 4, 4}};

    s_tech_review_achievement* _review_list[4] = {
        _review_achievements,
        _review_colony,
        _review_weapons,
        _review_equipment,
    };

    s_colors _info_normal_color = {{178, 185, 183, 182, 184, 185, 185, 186}};

    s_colors _info_high_color = {{178, 191, 183, 185, 187, 188, 191, 191}};

    int16_t _tech_rev_field[8] = {216, 427, 318, 427, 373, 427, 438, 427};

    s_drop_area _tech_review_drops[4] = {
        {218, 60, 425, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {433, 60, 612, 104, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {433, 114, 612, 254, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {433, 264, 612, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
    };

    uint16_t _list_sizes[4] = {9164, 8374, 6952, 12640};

    s_list_state _tech_review_list_header = {
        223,
        64,
        187,
        409,
        _tech_review_list_y,
        _tech_review_field_y1,
        _tech_review_field_y2,
        6,
        0,
        nullptr,
        nullptr,
        0,
        1,
        3,
        2,
        reinterpret_cast<void*>(Draw_Tech_Review_App_),
        10,
        0xBF,
        &_info_high_color,
        {&_info_normal_color, &_info_normal_color, &_info_normal_color, &_info_normal_color},
        &_info_jim_hack_color,
    };

    int16_t _tech_review_tab_index;

    void* _g_tech_image_seg;

    int16_t _g_last_list_field;

    uint16_t _g_selected_app_id;

    s_list_state* _g_header_p;

    s_animation_header* _g_bitmap1_p;

    s_animation_header* _g_bitmap2_p;

    int16_t _g_prior_choice;

    s_list_item* _g_selected_list_p;

    uint8_t _g_list_update_needed;

    int16_t _g_first_list_field;

    s_colors _info_red_color = {{178, 168, 166, 167, 167, 168, 168, 169}};

    uint8_t _sub_scr_field[10] = {21, 50, 21, 77, 21, 102, 21, 128, 21, 154};

    uint8_t _g_reference_screen_type;

    uint8_t _g_turn_message_ix;

    int16_t _g_reference_ix;

    char (*_g_how_to_str_)[48];

    int16_t _g_first_how_to_field;

    int16_t _g_first_category_field;

    char (*_g_category_str_p)[48];

    uint8_t _race_statistics_page;

    s_drop_area _turn_summary_drop = {
        218, 60, 612, 414, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}
    };

    s_drop_area _reference_drops[4] = {
        {218, 60, 414, 86, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {218, 93, 414, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {418, 60, 613, 86, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {418, 93, 613, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
    };

    s_list_state _g_turn_header = {
        223,
        96,
        345,
        408,
        _turn_summary_list_y,
        _turn_summary_list_y,
        _turn_summary_field_y2,
        8,
        0,
        nullptr,
        nullptr,
        0,
        0,
        4,
        4,
        reinterpret_cast<void*>(Draw_Turn_Summary_App_),
        13,
        0,
        &_info_high_color,
        {&_info_normal_color, &_info_normal_color, &_info_normal_color, &_info_normal_color},
        &_info_normal_color,
    };

    s_list_state _g_category_header = {
        223,
        96,
        175,
        410,
        _category_list_y,
        _category_list_y,
        _category_field_y2,
        0,
        0,
        nullptr,
        nullptr,
        0,
        0,
        3,
        3,
        nullptr,
        0,
        0,
        &_info_high_color,
        {&_info_normal_color, &_info_normal_color, &_info_normal_color, &_info_normal_color},
        &_info_normal_color,
    };

    uint16_t _g_exit_field;

    s_list_item* _g_selected_msg_p;

    s_drop_area _how_to_drops[2] = {
        {218, 60, 610, 86, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {218, 93, 610, 416, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
    };

    s_drop_area _category_drops[3] = {
        {218, 60, 613, 86, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {218, 93, 414, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {418, 93, 613, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
    };

    s_drop_area _race_stats_drops[4] = {
        {223, 60, 410, 230, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {419, 60, 606, 230, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {223, 243, 410, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {419, 243, 606, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
    };

    int16_t _race_bitmap_x[4] = {226, 422, 226, 422};

    int16_t _race_bitmap_y[4] = {64, 64, 247, 247};

    int16_t _race_statistics_specials_help_count;

    s_help_box _race_statistics_specials_help[50];

    s_colors _info_dark_color = {{178, 2, 2, 3, 3, 3, 4, 5}};

    s_colors* _maintenance_bar_colors[7] = {
        &_orange_bar_colors,
        &_grey_bar_colors,
        &_red_bar_colors,
        &_purple_bar_colors,
        &_yellow_bar_colors,
        &_blue_bar_colors,
        &_green_bar_colors,
    };

    s_colors* PTR__orange_text_colors[8] = {
        &_orange_text_colors,
        &_grey_text_colors,
        &_red_text_colors,
        &_purple_text_colors,
        &_yellow_text_colors,
        &_blue_text_colors,
        &_green_text_colors,
        nullptr,
    };

    s_colors _hist_palettes[MAX_PLAYERS] = {
        {{178, 198, 167, 195, 196, 197, 198, 199}},
        {{178, 206, 203, 203, 204, 205, 206, 207}},
        {{178, 214, 211, 211, 212, 213, 214, 215}},
        {{178, 222, 219, 219, 220, 221, 222, 223}},
        {{178, 230, 227, 227, 228, 229, 230, 231}},
        {{178, 238, 235, 235, 236, 237, 238, 239}},
        {{178, 246, 243, 243, 244, 245, 246, 247}},
        {{178, 254, 251, 251, 252, 253, 254, 255}},
    };

    s_colors* _hist_graph_palettes[MAX_PLAYERS];

    s_drop_area _history_graph_drops[2] = {
        {220, 60, 609, 122, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
        {220, 132, 609, 413, {179, 177, 177}, {180, 178, 178}, {178, 176, 176}},
    };

    int16_t _graph_button_x[4] = {233, 323, 404, 457};

    uint8_t _graph_offsets[10] = {7, 3, 3, 3, 3, 5, 3, 3, 3, 3};

    static int Compare_Unsigned_Strings_(const char* lhs, const char* rhs) {
        while ((uint8_t)*lhs == (uint8_t)*rhs) {
            if (*lhs == '\0') {
                return 0;
            }
            ++lhs;
            ++rhs;
        }

        return (int)(uint8_t)*lhs - (int)(uint8_t)*rhs;
    }

    void __cdecl Print_Player_Specials_To_Bitmap_(int16_t x, int16_t y, int16_t player_idx, s_animation_header* bitmap) {
        uint16_t idx = (uint16_t)player_idx;
        if (idx >= (uint16_t)MOX::_NUM_PLAYERS) {
            return;
        }

        int8_t* traits = MOX::_player[idx].traits;

        for (int trait_idx = 0; trait_idx < TRAIT_COUNT; ++trait_idx) {
            int8_t trait_val = traits[trait_idx];

            if (trait_val != 0 || trait_idx == TRAIT_CURRENT_GOVERNMENT) {
                char trait_label[64];
                char formatted_trait_text[64];

                strlcpy(trait_label, "^ ", sizeof(trait_label));

                if (trait_idx == TRAIT_CURRENT_GOVERNMENT) {
                    strlcat(trait_label, MOX::_government_labels[trait_val], sizeof(trait_label));
                } else if (trait_idx == TRAIT_RICH_HOME_WORLD) {
                    if (trait_val < 0) {
                        strlcat(trait_label, MOX::_special_ability_names[31], sizeof(trait_label));
                    } else {
                        strlcat(trait_label, MOX::_special_ability_names[15], sizeof(trait_label));
                    }
                } else {
                    strlcat(trait_label, MOX::_special_ability_names[trait_idx], sizeof(trait_label));
                }

                char* print_str = trait_label;

                if (trait_idx > TRAIT_CURRENT_GOVERNMENT && trait_idx < TRAIT_LOW_G_WORLD) {
                    if (trait_idx == TRAIT_FOOD_BONUS) {
                        strlcpy(formatted_trait_text, trait_label, sizeof(formatted_trait_text));
                        if (trait_val < 0) {
                            strlcat(formatted_trait_text, "-0.5", sizeof(formatted_trait_text));
                        } else if (trait_val == 2) {
                            strlcat(formatted_trait_text, "+1", sizeof(formatted_trait_text));
                        } else {
                            strlcat(formatted_trait_text, "+2", sizeof(formatted_trait_text));
                        }
                        print_str = formatted_trait_text;
                    } else if (trait_idx == TRAIT_TAX_BONUS) {
                        strlcpy(formatted_trait_text, trait_label, sizeof(formatted_trait_text));
                        if (trait_val < 0) {
                            strlcat(formatted_trait_text, "-", sizeof(formatted_trait_text));
                        } else {
                            strlcat(formatted_trait_text, "+", sizeof(formatted_trait_text));
                        }

                        if (trait_val == 2) {
                            strlcat(formatted_trait_text, "1 BC", sizeof(formatted_trait_text));
                        } else {
                            strlcat(formatted_trait_text, "0.5 BC", sizeof(formatted_trait_text));
                        }
                        print_str = formatted_trait_text;
                    } else {
                        snprintf(formatted_trait_text, sizeof(formatted_trait_text), "%s%+i", trait_label, (int)trait_val);
                        print_str = formatted_trait_text;
                    }
                }

                fonts::Print_To_Bitmap_(x, y, print_str, bitmap);
                y += fonts::_font_header.line_height;
            }
        }
    }

    void __cdecl Race_Statistics_Specials_Help_(int16_t x, int16_t y, uint8_t player_idx) {
        s_player* player = &MOX::_player[player_idx];

        for (int i = 0; i < TRAIT_COUNT; ++i) {
            int8_t trait_val = player->traits[i];

            if (trait_val != 0 || i == TRAIT_CURRENT_GOVERNMENT) {
                int16_t help_id;

                if (i == TRAIT_CURRENT_GOVERNMENT) {
                    help_id = (int16_t)(trait_val / 2) + 474;
                } else if (i == TRAIT_RICH_HOME_WORLD) {
                    if (trait_val < 0) {
                        help_id = 458;
                    } else {
                        help_id = 457;
                    }
                } else if (i < TRAIT_RICH_HOME_WORLD) {
                    help_id = (int16_t)i + 442;
                } else {
                    help_id = (int16_t)i + 443;
                }

                int count = INFO::_race_statistics_specials_help_count;

                INFO::_race_statistics_specials_help[count].help_id = help_id;
                INFO::_race_statistics_specials_help[count].x1 = x;
                INFO::_race_statistics_specials_help[count].y1 = y;
                INFO::_race_statistics_specials_help[count].x2 = x + 175;
                INFO::_race_statistics_specials_help[count].y2 = y + 9;

                y += 13;
                INFO::_race_statistics_specials_help_count++;
            }
        }
    }

    void __cdecl Colorize_Bitmap_(s_animation_header* anim, s_colors* color_table, uint8_t range_start, uint8_t range_end) {
        int32_t total_pixels = (int32_t)anim->height * (int32_t)anim->width;

        if (total_pixels > 0) {
            uint8_t* pixel_ptr = Animation_Frame_Data_(anim, anim->current_frame);

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
        MOX2::action_func_t auto_func_var;
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
        if (MOX::_return_screen == SCREEN_MAIN) {
            _g_turn_message_ix = 0;
        }

        *MOX::_active_player = static_cast<uint8_t>(MOX::_PLAYER_NUM);
        BILL::Get_Active_Players_(1, 0);

        info_lbx[0] = static_cast<s_animation_header*>(farload::Far_Reload_("INFO.LBX", 0, MOX::_screen_seg));
        for (int i = 1; i < 26; ++i) {
            info_lbx[i] = static_cast<s_animation_header*>(farload::Far_Reload_Next_("INFO.LBX", i, MOX::_screen_seg));
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

        _g_exit_field = fields::Add_Button_Field_(0x217, 0x1B2, "", info_lbx[2], "\x1B", '(');

        for (int i = 0; i < 5; ++i) {
            fields::Add_Multi_Button_Field_(
                _sub_scr_field[i * 2],
                _sub_scr_field[i * 2 + 1],
                "",
                info_lbx[3 + i],
                &current_tab,
                i,
                "",
                0x29
            );
        }

        int16_t field_counter = fields::Get_Field_Counter_();

        const char* format = (MOX::_settings.language == 0) ? "%i.%i" : "%i,%i";
        snprintf(stardate_str, sizeof(stardate_str), format, (int)(MOX::_stardate / 10), (int)(MOX::_stardate % 10));

        _race_statistics_page = 1;
        int16_t input_field = 0;

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

            int32_t saved_mark_pos = allocate::_mark_stack_pos[0];

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

        MOX::_current_screen = SCREEN_MAIN;
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
                snprintf(buffer, sizeof(buffer), "%i", (int)MOX::_cur_plyr_ptr->bc_produced);
                JIM::Get_Text_Message_("billtext.lbx", 19, lbx_msg, 40);
                strlcat(buffer, lbx_msg, sizeof(buffer));

                if (fonts::Get_String_Width_(buffer) > 126) {
                    fonts::Set_Font_Spacing_(1);
                }

                fonts::Print_Centered_To_Bitmap_(92, 3, buffer, label);
                fonts::_font_header.font_style_spacing = fonts::_font_header.font_spacing[2];
            } else {
                snprintf(buffer, sizeof(buffer), "%3i%%", (int)values[i]);
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
                JIM::Get_Text_Message_("billtext.lbx", (int16_t)(i + 19), lbx_msg, 40);
                fonts::Print_Centered_To_Bitmap_(108, 3, lbx_msg, label);
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
        INFO::_info_dark_color.colors[0] = 0xB0;
    }

    void __cdecl Draw_Maint_Income_Chart_(s_animation_header** chart_bars, s_animation_header** chart_labels, int16_t visible_bar_count) {
        char msg_top[40];
        char msg_bottom[40];
        char msg_stat[64];

        animate::Draw_(30, 212, chart_bars[0]);

        if (visible_bar_count > 0) {
            int16_t draw_x = (int16_t)(((visible_bar_count * 18) - 42) / 2 + 120);
            for (int i = 6; i >= 1; --i) {
                if (chart_bars[i] != nullptr) {
                    animate::Draw_(draw_x, 212, chart_bars[i]);
                    draw_x -= 18;
                }
            }
        }

        int16_t draw_y = 344;
        for (int i = 0; i < 7; ++i) {
            animate::Draw_(25, draw_y, chart_labels[i]);
            draw_y += 16;
        }

        fonts::Set_Font_Style_(1, &INFO::_info_normal_color);

        JIM::Get_Text_Message_("billtext.lbx", 17, msg_top, 40);
        fonts::Print_Centered_(42, 324, msg_top);

        JIM::Get_Text_Message_("billtext.lbx", 18, msg_bottom, 40);
        fonts::Print_Centered_(120, 324, msg_bottom);

        int32_t net_income = (int32_t)MOX::_cur_plyr_ptr->bc_produced - MOX::_cur_plyr_ptr->total_maintenance;

        s_colors* stat_color;
        if (net_income < 0) {
            stat_color = &INFO::_info_red_color;
        } else {
            stat_color = &INFO::_info_normal_color;
        }

        fonts::Set_Font_Style_(2, stat_color);

        JIM::Get_Text_Message_("billtext.lbx", 27, msg_stat, 64);
        JIM::Decode_Text_Stat_(msg_stat, net_income, 64);
        fonts::Print_Centered_(105, 196, msg_stat);
    }

    int16_t __cdecl _Tech_Review_Subscreen_(s_animation_header** anims, int16_t exit_threshold, int16_t player_idx) {
        s_list_state list_headers[4];
        char billtext[40];
        char help_lbx_name[20];
        void* text_buffer;
        int16_t field_down;
        int16_t field_up;
        bool is_tech_image_loaded;

        for (int i = 0; i < 4; ++i) {
            _g_header_p = &list_headers[i];
            memcpy(_g_header_p, &_tech_review_list_header, sizeof(*_g_header_p));
            _g_header_p->items = reinterpret_cast<s_list_item*>(
                allocate::Allocate_Next_Block_(MOX::_screen_seg, _list_sizes[i])
            );
            Init_Tech_Review_List_(_g_header_p, i);
            _g_header_p->items[0].is_selectable = 1;
            _g_header_p->items[0].current_app_index = 0;
        }

        text_buffer = allocate::Allocate_Next_Block_For_No_Header_Pool_(MOX::_screen_seg, sizeof(s_help_record));
        _g_bitmap1_p = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(180, 150));
        _g_bitmap2_p = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap::Get_Bitmap_Size_(180, 48));

        s_animation_header* app_pics = (s_animation_header*)(
            farload::Far_Reload_Next_("app_pics.lbx", 0, MOX::_screen_seg)
        );

        is_tech_image_loaded = false;
        fields::Assign_Auto_Function_(Draw_Tech_Review_Subscreen_, 2);

        int16_t field_multi_last;
        for (int i = 0; i < 4; ++i) {
            field_multi_last = fields::Add_Multi_Button_Field_(
                _tech_rev_field[i * 2],
                _tech_rev_field[i * 2 + 1],
                "",
                anims[i + 4],
                &_tech_review_tab_index,
                i,
                "",
                40
            );
        }

        field_up = fields::Add_Button_Field_(410, 65, "", anims[2], "\x1B", 40);
        field_down = fields::Add_Button_Field_(410, 395, "", anims[3], "\x1B", 40);

        JIM::Get_Text_Message_("billtext.lbx", 6, billtext, 40);
        fonts::Print_Centered_(416, 31, billtext);

        Draw_Multiple_Drop_Areas_(_tech_review_drops, 4);
        graphics::Fill_(433, 115, 612, 253, 0);
        animate::Draw_(433, 115, app_pics);

        bool list_changed = false;
        bool refresh_list_content = true;

        while (true) {
            if (refresh_list_content) {
                _g_header_p = &list_headers[_tech_review_tab_index];
            }

            RUSS::Mox_Update_();
            Draw_Drop_Area_(_tech_review_drops);
            animate::Draw_(410, 65, anims[0]);
            animate::Draw_(410, 395, anims[1]);
            LIST::Display_List_Page_(_g_header_p);
            video::Copy_Off_To_Back_();

            fields::Clear_Fields_Above_(field_down);
            _g_first_list_field = fields::Get_Field_Counter_();
            _g_last_list_field = LIST::Add_Fields_To_List_Page_(_g_header_p);

            if (list_changed) {
                LIST::Set_Selected_List_Entry_(_g_header_p, _g_first_list_field);
            }

            LIST::Set_List_Up_Down_Field_Drawing_(_g_header_p, field_up, field_down);
            _g_selected_list_p = LIST::Get_Selected_List_Entry_(_g_header_p);
            _g_list_update_needed = 1;
            _g_prior_choice = 0;

            Introduce_Subscreen_();

            bool inner_break = false;
            while (!inner_break) {
                timer::Mark_Time_();

                if (_g_list_update_needed != 0 && _g_header_p->num_items != 0) {
                    bitmap::Create_Blank_Bitmap_(180, 150, _g_bitmap1_p);
                    bitmap::Create_Blank_Bitmap_(180, 48, _g_bitmap2_p);

                    uint8_t tech_app_id = _g_selected_list_p->tech_app_ids[_g_selected_list_p->current_app_index];

                    TEXTBOX::Get_Help_Lbx_Name_(help_lbx_name);
                    farload::Far_Reload_Data_(help_lbx_name, 0, text_buffer, tech_app_id, 1, sizeof(s_help_record));

                    fonts::Set_Font_Style_(2, &_info_normal_color);
                    FMTPARA::Print_Formatted_Paragraph_To_Bitmap_(2, 4, 173, ((s_help_record*)text_buffer)->body, 0, _g_bitmap1_p);

                    _g_selected_app_id = tech_app_id;
                    fonts::Set_Font_Style_(4, &_info_normal_color);

                    char* name_str = TECH::Technology_Applications_Name_(_g_selected_app_id, player_idx);
                    FMTPARA::Print_Formatted_Paragraph_To_Bitmap_(0, 12, 176 /*0xb0*/, name_str, 2, _g_bitmap2_p);

                    RUSS::Mox_Update_();

                    if (is_tech_image_loaded) {
                        allocate::Pop_Block_(MOX::_screen_seg);
                    }

                    allocate::Push_Block_(MOX::_screen_seg);
                    _g_tech_image_seg = farload::Far_Reload_Next_("APP_PICS.LBX", tech_app_id, MOX::_screen_seg);
                    is_tech_image_loaded = true;

                    animate::Draw_Palette_(reinterpret_cast<s_animation_header*>(_g_tech_image_seg));
                    palette::Refresh_Palette_();
                }

                int16_t input = fields::Get_Input_();

                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    RUSS::Mox_Update_();
                    return input;
                }

                if (input <= field_multi_last) {
                    if (input == 0) {
                        Draw_Tech_Review_Subscreen_();
                        RUSS::Mox_Sync_Update_();
                        timer::Release_Time_(2);
                    }
                } else {
                    if (input == field_down) {
                        if (LIST::Next_List_Page_(_g_header_p) != 0) {
                            list_changed = true;
                            refresh_list_content = false;
                            inner_break = true;
                            continue;
                        }
                    } else if (input == field_up) {
                        if (LIST::Previous_List_Page_(_g_header_p) != 0) {
                            list_changed = true;
                            refresh_list_content = false;
                            inner_break = true;
                            continue;
                        }
                    }
                }

                if (input == 0 || input > field_multi_last) {
                    continue;
                }

                if (input >= exit_threshold) {
                    list_changed = false;
                    refresh_list_content = true;
                    inner_break = true;
                    continue;
                } else {
                    animate::Draw_(434, 264, reinterpret_cast<s_animation_header*>(_g_bitmap1_p));
                    animate::Draw_(434, 60, reinterpret_cast<s_animation_header*>(_g_bitmap2_p));
                    platform::Toggle_Pages_Stub_();
                    RUSS::Mox_Update_();
                    return input;
                }
            }
        }
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

    void __cdecl Draw_Race_Stats_Subscreen_() {
        // This function is a no-op in the original binary.
        return;
    }

    void __cdecl Draw_Dummy_Turn_Summary_Subscreen_() {
        return;
    }

    void __cdecl Draw_Turn_Summary_Subscreen_() {
        _g_turn_header.group_colors[0] = &_info_high_color;

        int16_t field_idx = fields::Scan_Input_();

        if (field_idx >= _g_first_list_field && field_idx <= _g_last_list_field) {
            LIST::Set_Selected_List_Entry_(&_g_turn_header, field_idx);
        }

        _g_selected_msg_p = LIST::Get_Selected_List_Entry_(&_g_turn_header);
        LIST::Display_List_Text_(&_g_turn_header, _g_selected_msg_p);

        _g_turn_header.group_colors[0] = &_info_normal_color;

        palette::Cycle_Palette_Color_(0xbf, 8, 0x20, 0, 0x28, 0x3f, 8, 8);
    }

    int16_t __cdecl Reference_Category_Subscreen_(s_animation_header** anims, int16_t unused_exit_code) {
        char header_text[80];
        char lbx_name[20];
        uint32_t record_offsets[60];
        s_list_item items[60];
        void* record_buffer;
        int16_t i, j;

        (void)unused_exit_code;

        Draw_Drop_Area_(INFO::_category_drops);
        JIM::Get_Text_Message_("billtext.lbx", 12 /* BILL_MSG_CATEGORY_HEADER */, header_text, 80);
        strlcat(header_text, MOX::_help_category_labels[INFO::_g_reference_ix], sizeof(header_text));
        fonts::Print_Centered_(416, 67, header_text);

        INFO::_g_category_header.items = items;
        uint32_t bmp_size = bitmap::Get_Bitmap_Size_(190, 314);
        INFO::_g_bitmap1_p = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bmp_size);
        record_buffer = allocate::Allocate_Next_Block_For_No_Header_Pool_(MOX::_screen_seg, sizeof(s_help_record));

        TEXTBOX::Get_Help_Lbx_Name_(lbx_name);
        int count = farload::Get_Farload_Data_Record_Count_(lbx_name, INFO::_g_reference_ix);
        INFO::_g_category_header.num_items = (uint8_t)count;

        for (i = 0; i < count; i++) {
            s_help_topic_record* topic = (s_help_topic_record*)
                farload::Far_Reload_Data_(lbx_name, INFO::_g_reference_ix, record_buffer, i, 1, sizeof(s_help_topic_record))
            ;
            strlcpy(items[i].group_name, topic->text, sizeof(items[i].group_name));
            record_offsets[i] = topic->id;
            items[i].group_id = (uint8_t)i;
            items[i].group_color_index = 0;
            items[i].app_count = 1;
        }

        // Alphabetical sort of categories
        for (i = 0; i < count; i++) {
            for (j = 0; j < (count - 1); j++) {
                if (Compare_Unsigned_Strings_(items[j].group_name, items[j + 1].group_name) > 0) {
                    char temp_name[64];
                    strlcpy(temp_name, items[j].group_name, sizeof(temp_name));
                    strlcpy(items[j].group_name, items[j + 1].group_name, sizeof(items[j].group_name));
                    strlcpy(items[j + 1].group_name, temp_name, sizeof(items[j + 1].group_name));

                    uint32_t temp_off = record_offsets[j];
                    record_offsets[j] = record_offsets[j + 1];
                    record_offsets[j + 1] = temp_off;
                }
            }
        }

        for (i = 0; i < count; ++i) {
            items[i].group_id = (uint8_t)i;
        }

        LIST::Init_List_Data_(&INFO::_g_category_header);
        int16_t close_btn_id = fields::Add_Button_Field_(386, 427, "", anims[21], "", '(');
        int16_t up_btn_id = fields::Add_Button_Field_(398, 96, "", anims[14], "\x1B", '(');
        int16_t down_btn_id = fields::Add_Button_Field_(398, 396, "", anims[15], "\x1B", '(');

        fields::Assign_Auto_Function_(INFO::Draw_Reference_Category_Subscreen_, 2);
        INFO::_g_reference_screen_type = 0;

        while (true) {
            INFO::Draw_Multiple_Drop_Areas_(&INFO::_category_drops[1], 2);
            animate::Draw_(398, 96, anims[12]);
            animate::Draw_(398, 396, anims[13]);
            LIST::Display_List_Page_(&INFO::_g_category_header);
            video::Copy_Off_To_Back_();

            fields::Clear_Fields_Above_(down_btn_id);
            INFO::_g_first_list_field = fields::Get_Field_Counter_();
            INFO::_g_last_list_field = LIST::Add_Fields_To_List_Page_(&INFO::_g_category_header);
            LIST::Set_List_Up_Down_Field_Drawing_(&INFO::_g_category_header, up_btn_id, down_btn_id);

            INFO::_g_prior_choice = fields::Scan_Input_();
            if (INFO::_g_prior_choice < INFO::_g_first_list_field || INFO::_g_prior_choice > INFO::_g_last_list_field) {
                INFO::_g_prior_choice = INFO::_g_first_list_field;
            }

            LIST::Set_Selected_List_Entry_(&INFO::_g_category_header, INFO::_g_prior_choice);
            INFO::_g_selected_list_p = LIST::Get_Selected_List_Entry_(&INFO::_g_category_header);
            INFO::_g_list_update_needed = 1;

            INFO::Introduce_Subscreen_();

            while (true) {
                timer::Mark_Time_();
                if (INFO::_g_list_update_needed != 0) {
                    bitmap::Create_Blank_Bitmap_(190, 314, INFO::_g_bitmap1_p);
                    s_help_record* data = (s_help_record*)farload::Far_Reload_Data_(lbx_name, 0, record_buffer, record_offsets[INFO::_g_selected_list_p->group_id], 1, sizeof(s_help_record));

                    fonts::Set_Font_Style_(4, &INFO::_info_normal_color);
                    BILL::Print_1_Or_More_Lines_Centered_To_Bitmap_(0, 24, 186, data->title, INFO::_g_bitmap1_p);

                    fonts::Set_Font_Style_(3, &INFO::_info_normal_color);
                    FMTPARA::Print_Formatted_Paragraph_To_Bitmap_(0, 64, 186, data->body, 3, INFO::_g_bitmap1_p);
                }

                int16_t input = fields::Get_Input_();
                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    return 0;
                }

                if (input == down_btn_id) {
                    if (LIST::Next_List_Page_(&INFO::_g_category_header) != 0) break;
                } else if (input == up_btn_id) {
                    if (LIST::Previous_List_Page_(&INFO::_g_category_header) != 0) break;
                } else if (input == 0) {
                    INFO::Draw_Reference_Category_Subscreen_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(2);
                } else if (input <= close_btn_id) {
                    return input;
                }
            }
        }
    }

    void __cdecl Draw_Reference_Category_Subscreen_() {
        int16_t field_id = fields::Scan_Input_();
        int16_t mouse_button = mouse::Mouse_Button_();

        if (mouse_button == 0 && INFO::_g_list_update_needed == 0 &&
            field_id >= INFO::_g_first_list_field && field_id <= INFO::_g_last_list_field &&
            field_id != INFO::_g_prior_choice) {

            LIST::Set_Selected_List_Entry_(&INFO::_g_category_header, field_id);
            INFO::_g_selected_list_p = LIST::Get_Selected_List_Entry_(&INFO::_g_category_header);
            INFO::_g_prior_choice = field_id;
            INFO::_g_list_update_needed = 1;
        } else {
            INFO::_g_list_update_needed = 0;
            animate::Draw_(422, 96, (s_animation_header*)INFO::_g_bitmap1_p);
        }

        LIST::Display_Selected_List_Text_(&INFO::_g_category_header, INFO::_g_selected_list_p);
        palette::Cycle_Palette_Color_(191, 8, 32, 0, 40, 63, 8, 8);
    }

    int16_t __cdecl History_Subscreen_(s_animation_header** lbx_images, int16_t exit_threshold) {
        char msg_buffer[40];
        int16_t player_hist_buttons[4];
        int16_t input;

        JIM::Get_Text_Message_("billtext.lbx", 5, msg_buffer, 40);
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
                "",
                lbx_images[i + 8],
                &player_hist_buttons[i],
                "",
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
        int32_t history_data[MAX_PLAYERS][350];
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
                        val += static_cast<uint8_t>(plr->population_history[hist_idx]) * MOX::_bill_savegame[2];
                    }
                    if (use_tech != 0) {
                        val += static_cast<uint8_t>(plr->tech_history[hist_idx]) * MOX::_bill_savegame[5];
                    }
                    if (use_fleet != 0) {
                        val += static_cast<uint8_t>(plr->fleet_history[hist_idx]) * MOX::_bill_savegame[4];
                    }
                    if (use_prod != 0) {
                        val += static_cast<uint8_t>(plr->production_history[hist_idx]) * MOX::_bill_savegame[3];
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
        itoa(max_scale, buffer, 10);
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
        int16_t spacing = 0;
        int16_t active_count = MOX::_active_count;

        if (active_count > 3) {
            spacing = 150;
            x_pos = 295;
        }

        if (active_count > 6) {
            spacing = 122;
            x_pos = 233;
        }

        if (active_count > 0) {
            int16_t i = 0;

            do {
                uint8_t player_idx = MOX::_active_player[i];

                fonts::Set_Font_Style_(4, INFO::_hist_graph_palettes[player_idx]);

                fonts::Print_(x_pos, y_pos, MOX::_player[player_idx].race_name);

                y_pos += 19;

                if (i == 2 || i == 5) {
                    x_pos += spacing;
                    y_pos = 64;
                }

                ++i;
            } while (i < MOX::_active_count);
        }
    }

    void __cdecl Draw_Multiple_Drop_Areas_(s_drop_area* areas, uint16_t count) {
        s_drop_area* end = areas + count;
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

        int32_t diff = val1 - average;
        // Match the original 32-bit SUB/CDQ/XOR/SUB sequence and avoid abs(INT_MIN) UB.
        int32_t sign = diff >> 31;
        int32_t result = (diff ^ sign) - sign;

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

    int16_t __cdecl Tech_Review_Subscreen_(s_animation_header** headers, int16_t exit_threshold) {
        RACERPRT::_race_report_jim_hack = 1;
        return _Tech_Review_Subscreen_(&headers[12], exit_threshold, MOX::_PLAYER_NUM);
    }

    /**
     * @brief Initializes a technical review list for the given player based on discovered technology.
     * 
     * @param state The list state structure to populate.
     * @param review_idx Index into the achievement list pointers.
     */
    void __cdecl Init_Tech_Review_List_(s_list_state* state, int16_t review_idx) {
        s_tech_review_achievement* ach = INFO::_review_list[review_idx];
        s_list_item* item = state->items;
        int16_t app_idx;

        item->app_count = 0;
        state->num_items = 1;

        // Loop until "End of Array" flag (0x08) is encountered
        while (!(ach->flags & 0x08 /* End of Array */)) {
            if (ach->flags & 0x04 /* Group Header */) {
                // If the previous item had any technology applications, start a new list item
                if (item->app_count != 0) {
                    item++;
                    state->num_items++;
                }
                
                // Fetch group name from LBX and initialize item header fields
                JIM::Get_Text_Message_("billtex2.lbx", static_cast<int16_t>(ach->id), item->group_name, 0x40);
                item->group_id = ach->id;
                item->group_color_index = 0;
                item->app_count = 0;
                app_idx = 0;
            } else {
                // If the player has researched the technology application, add it to the item's application list
                if (MOX::_cur_plyr_ptr->tech_applications[ach->id] == 3 /* Researched */) {
                    item->tech_app_ids[app_idx] = ach->id;
                    item->tech_app_status[app_idx] = MOX::_cur_plyr_ptr->tech_applications[ach->id];
                    app_idx++;
                    item->app_count++;
                }
            }
            ach++;
        }

        // Clean up: if the last initialized item remained empty, exclude it from the list
        if (item->app_count == 0) {
            state->num_items--;
        }

        LIST::Init_List_Data_(state);
    }

    void __cdecl Draw_Tech_Review_Subscreen_() {
        int16_t input_field = fields::Scan_Input_();
        int16_t mouse_btn = mouse::Mouse_Button_();

        if (mouse_btn == 0 && INFO::_g_list_update_needed == 0 &&
            input_field >= INFO::_g_first_list_field && input_field <= INFO::_g_last_list_field &&
            input_field != INFO::_g_prior_choice) {

            LIST::Set_Selected_List_Entry_(INFO::_g_header_p, input_field);
            INFO::_g_selected_list_p = LIST::Get_Selected_List_Entry_(INFO::_g_header_p);
            INFO::_g_prior_choice = input_field;
            INFO::_g_list_update_needed = 1;
        }
        else if (INFO::_g_header_p->num_items != 0) {
            animate::Draw_(0x1B1, 0x73, reinterpret_cast<s_animation_header*>(INFO::_g_tech_image_seg));
            INFO::_g_list_update_needed = 0;
            animate::Draw_(0x1B2, 0x108, reinterpret_cast<s_animation_header*>(INFO::_g_bitmap1_p));
            animate::Draw_(0x1B2, 0x3C, reinterpret_cast<s_animation_header*>(INFO::_g_bitmap2_p));
        }

        LIST::Display_Selected_List_Text_(INFO::_g_header_p, INFO::_g_selected_list_p);
        palette::Cycle_Palette_Color_(0xBF, 8, 0x20, 0, 0x28, 0x3F, 8, 8);
    }

    int16_t __cdecl Draw_The_History_Graph_() {
        uint16_t offset_idx = 0;
        uint16_t x = 0xed; /* 237 */

        do {
            uint8_t offset = INFO::_graph_offsets[offset_idx++];
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
            uint8_t offset = INFO::_graph_offsets[offset_idx++];
            if (offset_idx >= 10) {
                offset_idx = 0;
            }

            graphics::Dot_(0xed - offset, y, 0xb1);
            line::Line_(0xee - offset, y, 0xef /* 239 */, y, 0xb9);
            y -= 10;
        } while (y >= 0x8c /* 140 */);

        fonts::Set_Font_Style_(1, &INFO::_info_normal_color);

        int32_t current_val = 0x88b8; /* 35000 */
        int16_t return_val;
        int16_t step;
        int32_t stardate = MOX::_stardate;

        uint16_t check_date = (uint16_t)(stardate + 0x7748 /* 30536 */);

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
                current_val = ((stardate - 0x159 /* 345 */) / 10) * 10;
            }
        }

        uint16_t label_x = 0xef; /* 239 */
        char buffer[16];
        for (int i = 0; i < 8; ++i) {
            int32_t whole = current_val / 10;
            int32_t fraction = current_val % 10;

            if ((current_val % 5 == 0) && (fraction != 0)) {
                if (MOX::_settings.language == 0) {
                    snprintf(buffer, sizeof(buffer), "%i.%i", whole, fraction);
                } else {
                    snprintf(buffer, sizeof(buffer), "%i,%i", whole, fraction);
                }
            } else {
                itoa(whole, buffer, 10);
            }

            fonts::Print_Centered_(label_x, 400, buffer);
            label_x += 0x32;
            current_val += step;
        }

        return return_val;
    }

    int16_t __cdecl Race_Stats_Subscreen_(s_animation_header** anims, int16_t exit_threshold) {
        char race_name[32];
        char title_msg[40];
        char dead_msg[40];
        char none_msg[40];
        char unused_msg[40];

        s_animation_header* player_panels[MAX_PLAYERS];
        s_animation_header* eliminated_banner;

        JIM::Get_Text_Message_("billtext.lbx", 14, dead_msg, 40);
        JIM::Get_Text_Message_("billtext.lbx", 15, none_msg, 40);
        JIM::Get_Text_Message_("billtext.lbx", 16, unused_msg, 40);
        JIM::Get_Text_Message_("billtext.lbx", 7, title_msg, 40);

        fonts::Print_Centered_(416, 31, title_msg);
        INFO::Set_Up_Player_Colors_(anims);

        MOX::_active_player[0] = static_cast<char>(MOX::_PLAYER_NUM);
        BILL::Get_Players_Dead_Or_Alive_Or_Omniscient_(1, 1);

        uint32_t size = bitmap::Get_Bitmap_Size_(183, 20);
        eliminated_banner = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size);
        bitmap::Create_Blank_Bitmap_(183, 20, eliminated_banner);

        fonts::Set_Font_Style_Outline_(2, &INFO::_info_red_color);
        fonts::Print_Centered_To_Bitmap_(91, 2, dead_msg, eliminated_banner);

        for (int i = 0; i < MAX_PLAYERS; ++i) {
            size = bitmap::Get_Bitmap_Size_(183, 164);
            player_panels[i] = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size);
            bitmap::Create_Blank_Bitmap_(183, 164, player_panels[i]);

            if (i < MOX::_active_count) {
                uint8_t active_idx = MOX::_active_player[i];
                fonts::Set_Font_Style_(4, INFO::_hist_graph_palettes[active_idx]);

                strlcpy(race_name, MOX::_player[active_idx].race_name, sizeof(race_name));
                strupr(race_name);

                fonts::Print_Centered_To_Bitmap_(91, 0, race_name, player_panels[i]);

                fonts::Set_Font_Style_(2, INFO::_hist_graph_palettes[active_idx]);
                INFO::Print_Player_Specials_To_Bitmap_(0, 22, active_idx, player_panels[i]);
            } else {
                fonts::Set_Font_Style_(4, &INFO::_info_normal_color);
                fonts::Print_Centered_To_Bitmap_(91, 0, none_msg, player_panels[i]);
            }
        }

        if (MOX::_active_count > 4) {
            fields::Add_Button_Field_(385, 427, MOX::s__00551100, anims[20], MOX::s__00551100, 40);
        }

        fields::Assign_Auto_Function_(INFO::Draw_Race_Stats_Subscreen_, 2);

        int16_t input;
        while (true) {
            INFO::_race_statistics_specials_help_count = 0;
            INFO::Draw_Multiple_Drop_Areas_(INFO::_race_stats_drops, 4);

            int offset = (INFO::_race_statistics_page == 1) ? 0 : 4;

            for (int j = 0; j < 4; ++j) {
                int16_t x = INFO::_race_bitmap_x[j];
                int16_t y = INFO::_race_bitmap_y[j];

                line::Line_(x + 10, y + 17, x + 173, y + 17, INFO::_info_normal_color.colors[6]);

                animate::Draw_(x, y, player_panels[offset]);

                uint8_t active_idx = MOX::_active_player[offset];
                if (MOX::_player[active_idx].eliminated != 0) {
                    animate::Draw_(x, y + 11, eliminated_banner);
                } else if (offset < MOX::_active_count) {
                    INFO::Race_Statistics_Specials_Help_(x, y + 22, active_idx);
                }

                offset++;
            }

            int count = INFO::_race_statistics_specials_help_count++;
            INFO::_race_statistics_specials_help[count].help_id = 0xf2;
            INFO::_race_statistics_specials_help[count].x1 = 0;
            INFO::_race_statistics_specials_help[count].y1 = 40;
            INFO::_race_statistics_specials_help[count].x2 = 205;
            INFO::_race_statistics_specials_help[count].y2 = 197;

            count = INFO::_race_statistics_specials_help_count++;
            INFO::_race_statistics_specials_help[count].help_id = 0xf3;
            INFO::_race_statistics_specials_help[count].x1 = 0;
            INFO::_race_statistics_specials_help[count].y1 = 198;
            INFO::_race_statistics_specials_help[count].x2 = 205;
            INFO::_race_statistics_specials_help[count].y2 = 479;

            count = INFO::_race_statistics_specials_help_count++;
            INFO::_race_statistics_specials_help[count].help_id = 0xf8;
            INFO::_race_statistics_specials_help[count].x1 = 206;
            INFO::_race_statistics_specials_help[count].y1 = 0;
            INFO::_race_statistics_specials_help[count].x2 = 639;
            INFO::_race_statistics_specials_help[count].y2 = 479;

            fields::Deactivate_Help_List_();
            fields::Set_Help_List_(INFO::_race_statistics_specials_help, INFO::_race_statistics_specials_help_count);

            video::Copy_Off_To_Back_();
            INFO::Introduce_Subscreen_();

            do {
                input = fields::Get_Input_();

                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    return 0;
                }

                if (input >= exit_threshold) {
                    INFO::_race_statistics_page = (INFO::_race_statistics_page == 0) ? 1 : 0;
                    break;
                } else if (input == 0) {
                    timer::Mark_Time_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(2);
                }
            } while (input == 0);

            if (input < exit_threshold) {
                return input;
            }
        }
    }

    int16_t __cdecl Reference_Main_Subscreen_(s_animation_header** unused_anims, int16_t exit_code) {
        char header_text[40];
        char lbx_name[20];
        char category_strs[16][48];
        char how_to_strs[16][48];
        int16_t category_indices[16];
        uint32_t topic_ids[16];
        int16_t y_pos;
        int16_t input;
        uint16_t record_count;
        s_help_topic_record* record;
        int16_t category_count;

        (void)unused_anims;

        INFO::_g_category_str_p = category_strs;
        INFO::_g_how_to_str_ = how_to_strs;

        INFO::Draw_Multiple_Drop_Areas_(INFO::_reference_drops, 4);

        JIM::Get_Text_Message_("billtext.lbx", 10, header_text, 0x28);
        fonts::Print_(0xe0, 0x43, header_text);

        JIM::Get_Text_Message_("billtext.lbx", 11, header_text, 0x28);
        fonts::Print_(0x1a8, 0x43, header_text);

        if (MOX::_settings.language == 1 || MOX::_settings.language == 2) {
            fonts::Set_Font_Style_(3, &INFO::_info_normal_color);
        } else {
            fonts::Set_Font_Style_(4, &INFO::_info_normal_color);
        }

        y_pos = 100;
        INFO::_g_first_category_field = fields::Get_Field_Counter_();
        category_count = 0;

        for (int i = 1; i < 17; ++i) {
            if ((uint8_t)MOX::_help_category_labels[i][0] > 0x39 /* '9' */) {
                snprintf(category_strs[category_count], sizeof(category_strs[category_count]), "^ %s", MOX::_help_category_labels[i]);
                fonts::Print_(0xdd, y_pos, category_strs[category_count]);
                fields::Add_Hidden_Field_(0xdd, y_pos - 2, 0x19b, y_pos + 18, "", 0);
                category_indices[category_count] = (int16_t)i;
                category_count++;
                y_pos += 20;
            }
        }

        void* reload_buffer = allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(s_help_topic_record));
        INFO::_g_first_how_to_field = fields::Get_Field_Counter_();
        TEXTBOX::Get_Help_Lbx_Name_(lbx_name);
        record_count = farload::Get_Farload_Data_Record_Count_(lbx_name, 0xf);

        y_pos = 100;
        for (int i = 0; i < (int)record_count; ++i) {
            record = (s_help_topic_record*)farload::Far_Reload_Data_(lbx_name, 0xf, reload_buffer, i, 1, sizeof(s_help_topic_record));
            topic_ids[i] = record->id;
            snprintf(how_to_strs[i], sizeof(how_to_strs[i]), "^ %s", record->text);
            fonts::Print_(0x1a5, y_pos, how_to_strs[i]);
            fields::Add_Hidden_Field_(0x1a5, y_pos - 2, 0x263, y_pos + 18, "", 0);
            y_pos += 20;
        }

        video::Copy_Off_To_Back_();
        fields::Assign_Auto_Function_(INFO::Draw_Reference_Main_Subscreen_, 2);
        INFO::Introduce_Subscreen_();

        if (MOX::_settings.language == 1 || MOX::_settings.language == 2) {
            fonts::Set_Font_Style_(3, &INFO::_info_high_color);
        } else {
            fonts::Set_Font_Style_(4, &INFO::_info_high_color);
        }

        while (true) {
            input = fields::Get_Input_();
            if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                return 0;
            }
            if (input >= exit_code) {
                break;
            }
            if (input == 0) {
                timer::Mark_Time_();
                INFO::Draw_Reference_Main_Subscreen_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(2);
                continue;
            }
            if (input < exit_code) {
                return input;
            }
        }

        if (input < INFO::_g_first_how_to_field) {
            INFO::_g_reference_screen_type = 1;
            INFO::_g_reference_ix = category_indices[input - INFO::_g_first_category_field];
        } else {
            INFO::_g_reference_screen_type = 2;
            INFO::_g_reference_ix = (int16_t)topic_ids[input - INFO::_g_first_how_to_field];
        }

        return 0;
    }

    int16_t __cdecl Reference_How_To_Subscreen_(s_animation_header** anims, int16_t exit_code) {
        char help_lbx_name[20];
        char title_combined[80];
        s_help_record* loaded_help_text;
        int16_t input_code;

        (void)exit_code;

        TEXTBOX::Get_Help_Lbx_Name_(help_lbx_name);
        INFO::Draw_Multiple_Drop_Areas_(INFO::_how_to_drops, 2);

        loaded_help_text = static_cast<s_help_record*>(farload::Far_Reload_Next_Data_(
            help_lbx_name, 0, MOX::_screen_seg, (int32_t)INFO::_g_reference_ix, 1, sizeof(s_help_record)
        ));

        JIM::Get_Text_Message_("billtext.lbx", 13, title_combined, 80);
        strlcat(title_combined, loaded_help_text->title, sizeof(title_combined));

        fonts::Print_Centered_(414, 67, title_combined);

        if (MOX::_settings.language == 1 || MOX::_settings.language == 2) {
            fonts::Set_Font_Style_(3, &INFO::_info_normal_color);
        }

        FMTPARA::Print_Formatted_Paragraph_(224, 99, 381, loaded_help_text->body, 3);

        video::Copy_Off_To_Back_();

        fields::Add_Button_Field_(386, 427, "", anims[21], "", 40);

        fields::Assign_Auto_Function_(INFO::Draw_Dummy_Turn_Summary_Subscreen_, 2);
        
        INFO::_g_reference_screen_type = 0;
        INFO::Introduce_Subscreen_();

        do {
            input_code = fields::Get_Input_();
            if (input_code != 0 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                break;
            }
            RUSS::Mox_Sync_Update_();
        } while (true);
        
        return input_code;
    }

    int16_t __cdecl Reference_Subscreen_(s_animation_header** anims, int16_t exit_threshold) {
        char text_buffer[40];

        JIM::Get_Text_Message_("billtext.lbx", 9, text_buffer, 40);
        fonts::Print_Centered_(416, 31, text_buffer);
        fonts::Set_Font_Style_(4, &_info_normal_color);

        if (_g_reference_screen_type == 0) {
            BILLHELP::Reference_Help_();
            return Reference_Main_Subscreen_(anims, exit_threshold);
        }

        if (_g_reference_screen_type == 1) {
            BILLHELP::Ref_How_To_Help_();
            return Reference_Category_Subscreen_(anims, exit_threshold);
        }

        if (_g_reference_screen_type == 2) {
            BILLHELP::Ref_Category_Help_();
            return Reference_How_To_Subscreen_(anims, exit_threshold);
        }

        return 0;
    }

    void __cdecl _Init_Turn_Summary_List_(s_list_state* state, int16_t num_items, int16_t font_style, int16_t font_lf) {
        if (num_items != 0) {
            state->num_items = (uint8_t)num_items;

            int16_t msg_idx = MSG_::First_Msg_For_Player_(MOX::_PLAYER_NUM);
            int16_t end_msg_idx = msg_idx + num_items;

            fonts::Set_Font_Style_(font_style, &INFO::_info_normal_color);
            fonts::Set_Font_LF_(font_lf);

            s_list_item* item = state->items;
            char buffer[256];

            while (msg_idx < end_msg_idx) {
                item->group_name[0] = '\0';
                item->group_id = (uint8_t)msg_idx;
                item->group_color_index = 0;
                item->tech_app_ids[0] = (uint8_t)msg_idx;

                MSG_::sprintf_msg_(buffer, &MSG_::_msgs[msg_idx]);

                int16_t height = FMTPARA::Get_Formatted_Paragraph_Max_Height_(state->item_width, buffer);
                item->app_count = (uint8_t)(height / font_lf);

                // Binary executes this initialization loop at least once.
                uint8_t i = 0;
                do {
                    item->tech_app_status[i] = 0;
                    item->tech_app_ids[i + 1] = 0xff;
                    ++i;
                } while (i < item->app_count);

                msg_idx++;
                item++;
            }

            LIST::Init_List_Data_(state);
        }
    }

    void __cdecl Init_Turn_Summary_List_(s_list_state* state, int16_t val) {
        INFO::_Init_Turn_Summary_List_(state, val, 4, 10);
    }

    void __cdecl Draw_Reference_Main_Subscreen_() {
        int16_t field_idx = fields::Scan_Input_();

        if (field_idx >= INFO::_g_first_how_to_field) {
            int16_t offset = field_idx - INFO::_g_first_how_to_field;
            fonts::Print_(0x1a5, (offset + 5) * 20, INFO::_g_how_to_str_[offset]);
        } else if (field_idx >= INFO::_g_first_category_field) {
            int16_t offset = field_idx - INFO::_g_first_category_field;
            fonts::Print_(0xdd, (offset + 5) * 20, INFO::_g_category_str_p[offset]);
        }

        palette::Cycle_Palette_Color_(0xbf, 8, 0x20, 0, 0x28, 0x3f, 8, 8);
    }

    int16_t __cdecl Turn_Summary_Subscreen_(s_animation_header** anim_data, int16_t min_field) {
        char str_turn_summary[40];
        char str_year[40];
        char str_date[64];
        s_list_item list_items[100];

        JIM::Get_Text_Message_("billtext.lbx", 0x10, str_turn_summary, 40);
        JIM::Get_Text_Message_("billtext.lbx", 8, str_year, 40);
        fonts::Print_Centered_(0x1a0, 0x1f, str_year);
        JIM::Get_Text_Message_("billtext.lbx", 0x1a, str_date, 60);
        JIM::Decode_Text_Year_(str_date, 60);

        int16_t msg_count = MSG_::N_Msgs_For_Player_(MOX::_PLAYER_NUM);
        int16_t up_button;
        int16_t down_button = 0;

        if (msg_count > 0) {
            up_button = fields::Add_Button_Field_(0x250, 0x54, "", anim_data[0xe], "\x1B", '(');
            down_button = fields::Add_Button_Field_(0x250, 0x18b, "", anim_data[0xf], "\x1B", '(');

            _g_turn_header.items = list_items;
            Init_Turn_Summary_List_(&_g_turn_header, msg_count);
            _g_selected_msg_p = LIST::Select_And_Page_By_Head_ID_(&_g_turn_header, _g_turn_message_ix);
        } else {
            down_button = list_items[0].x;
        }

        while (true) {
            Draw_Drop_Area_(&_turn_summary_drop);
            fonts::Set_Font_Style_(4, &_info_normal_color);
            fonts::Print_Centered_(0x19e, 0x42, str_date);
            line::Line_(0xee, 0x51, 0x244, 0x51, 0xb8);
            animate::Draw_(0x250, 0x54, anim_data[0xc]);
            animate::Draw_(0x250, 0x18b, anim_data[0xd]);

            int16_t input_field;

            if (msg_count == 0) {
                fonts::Set_Font_Style_(4, &_info_normal_color);
                fonts::Print_Centered_(0x19e, 0xd8, str_turn_summary);
                fields::Assign_Auto_Function_(Draw_Dummy_Turn_Summary_Subscreen_, 2);
                video::Copy_Off_To_Back_();
                Introduce_Subscreen_();

                while (true) {
                    input_field = fields::Get_Input_();
                    if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                        return 0;
                    }
                    timer::Mark_Time_();
                    RUSS::Mox_Sync_Update_();
                    timer::Release_Time_(2);
                    if (input_field != 0) {
                        return input_field;
                    }
                }
            }

            fields::Clear_Fields_Above_(down_button);
            _g_first_list_field = fields::Get_Field_Counter_();
            _g_last_list_field = LIST::Add_Fields_To_List_Page_(&_g_turn_header);
            LIST::Display_List_Page_(&_g_turn_header);
            LIST::Set_List_Up_Down_Field_Drawing_(&_g_turn_header, up_button, down_button);
            fields::Assign_Auto_Function_(Draw_Turn_Summary_Subscreen_, 2);
            video::Copy_Off_To_Back_();
            Introduce_Subscreen_();

            bool refresh_outer = false;

            while (true) {
                input_field = fields::Get_Input_();
                if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                    return 0;
                }

                if (input_field >= _g_first_list_field && input_field <= _g_last_list_field) {
                    if (MSG_::Goto_Msg_Colony_(&MSG_::_msgs[_g_selected_msg_p->group_id])) {
                        _g_turn_message_ix = _g_selected_msg_p->group_id;
                        MOX::_current_screen = SCREEN_COLONY;
                        MOX::_return_screen = SCREEN_INFO;
                        input_field = _g_exit_field;
                    }
                } else {
                    if (input_field < min_field || msg_count <= 0) {
                        if (input_field == 0) {
                            timer::Mark_Time_();
                            Draw_Turn_Summary_Subscreen_();
                            RUSS::Mox_Sync_Update_();
                            timer::Release_Time_(2);
                        }
                    } else {
                        if (input_field == down_button) {
                            if (LIST::Next_List_Page_(&_g_turn_header)) {
                                refresh_outer = true;
                                break;
                            }
                        } else if (input_field == up_button) {
                            if (LIST::Previous_List_Page_(&_g_turn_header)) {
                                refresh_outer = true;
                                break;
                            }
                        }
                    }
                }

                if (input_field != 0) {
                    if (input_field < min_field) {
                        return input_field;
                    }
                }
            }

            if (refresh_outer) {
                continue;
            }
        }
    }

}