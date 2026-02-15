#include "deps.h"

namespace COLSUM {
    void __cdecl Check_Galaxy_Map_Fields_(int16_t* param_1, int16_t* param_2) {
        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            int16_t best_planet = Pick_Best_Planet_To_Goto_At_Star_(star_idx);

            if (*param_1 == MOX::_galaxy_map_star_field[star_idx] && best_planet != -1) {
                for (int16_t colony_list_idx = 0; colony_list_idx < MOX::_NUM_COLONIES; ++colony_list_idx) {
                    int16_t col_idx = COLXPORT::_g_colony_list_ptr[colony_list_idx];

                    if (col_idx != -1 && MOX::_colony[col_idx].planet == best_planet) {
                        _first = colony_list_idx;

                        while (COLXPORT::_g_colony_list_ptr[_first + 9] == -1) {
                            Increment_First_(10);
                        }

                        int16_t n_colonies = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);
                        _slider_bar_position = n_colonies - _first - 10;

                        Update_First_(10, &_first, &_slider_bar_position);
                        Update_Col_List_();
                        COLONY::_full_draw = 1;
                    }
                }
            }

            if (*param_2 == MOX::_galaxy_map_star_field[star_idx]) {
                best_planet = Pick_Best_Planet_To_Goto_At_Star_(star_idx);
                MOX::_galaxy_map_scanned_star = star_idx;

                if (best_planet != -1) {
                    COLONY::_g_colony_n = MOX::_planet[best_planet].colony_index;
                }
            }
        }
    }

    void __cdecl Draw_Scan_Info_() {
        Draw_Colony_Scan_Info_();
        Set_Colony_Font_To_(1);

        if (MOX::_galaxy_map_scanned_star != -1) {
            const char* star_name = reinterpret_cast<const char*>(&MOX::_star[MOX::_galaxy_map_scanned_star]);
            fonts::Print_Centered_(444, 431, star_name);
        }
    }

    void __cdecl Colony_Summary_Screen_() {
        void* saved_auto_fn = nullptr;
        int16_t input_val = 0;
        int16_t scan_val = 0;
        short colony_list_buffer[250];

        COLSUM::_draw_planet_sum = 0;
        COLXPORT::_g_colony_list_ptr = colony_list_buffer;

        if (MOX::_return_screen != MOX::_current_screen &&
            COLBLDG::_return_to_build_popup_from_custom_design == 0 &&
            MOX::_return_screen != SCREEN_QUEUE_POPUP)
        {
            COLSUM::_colony_summary_saved_screen = MOX::_return_screen;
        }

        COLSUM::Load_Col_Sum_Anims_();
        COLSUM::Load_Colsum_Strings_();
        COLONY::Allocate_Mini_Planet_Cache_();
        COLXPORT::Build_Global_Colony_List_();
        COLSUM::Sort_Col_List_();
        COLSUM::Update_Col_List_();

        if (COLSUM::_list_col[9] == -1) {
            COLSUM::Reset_List_Col_Array_();
            COLSUM::Update_Col_List_();
        }

        fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
        MOX2::Get_Auto_Function_(&saved_auto_fn);
        mouse::Set_Mouse_List_(COLGCBT::_mouse_list_blank, 1);
        fields::Deactivate_Auto_Function_();

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            ERIC::Fast_Fade_Out_();
            graphics::Fill_(0, 0, 639, 479, 0);
        }

        fonts::Load_Palette_(1, 0, 255);
        animate::Draw_Palette_(COLSUM::_anims[0]);
        remap::Update_Glass_Remap_Colors_();

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            palstore::Darken_Palette_(100);
            palette::Clear_Palette_Changes_(0, 255);
        }

        palette::Refresh_Palette_();

        COLONY::_g_colony_n = COLSUM::_list_col[0];
        COLONY::_using_colony_screen_palette = 1;
        MOX::_galaxy_map_scanned_star = (int16_t)MOX::_planet[MOX::_colony[COLONY::_g_colony_n].planet].star_index;
        COLONY::_full_draw = 1;

        ERICHELP::Set_Colony_Summary_Screen_Help_List_();
        fields::Assign_Auto_Function_(COLSUM::Draw_Colony_Summary_Screen_, 1);
        COLSUM::Draw_Colony_Summary_Screen_();
        palette::Clear_Palette_Changes_(0, 255);
        video::Set_Refresh_Stencil_();
        fields::Draw_Visible_Fields_();
        RUSS::Mox_Sync_Update_();
        COLONY::_full_draw = 0;
        ERIC::Fast_Fade_In_();
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);

        while (true) {
            video::Set_Refresh_Stencil_();
            COLSUM::Update_Col_List_();
            timer::Mark_Time_();
            input_val = fields::Get_Input_();
            scan_val = fields::Scan_Input_();

            if (!COLSUM::Evaluate_Input_(&input_val, &scan_val)) {
                FAKEBUTS::Clear_Fake_Buttons_();
                COLONY::Clear_Galaxy_Map_Fields_();
                fields::Clear_Fields_();
                fields::Deactivate_Auto_Function_();
                COLONY::_using_colony_screen_palette = 0;
                return;
            }

            COLSUM::Draw_Colony_Summary_Screen_();
            if (COLONY::_full_draw != 0) {
                COLSUM::Update_Col_List_();
                COLONY::_full_draw = 0;
            }
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);

            if (input_val == COLSUM::_x_fields_3) {
                break;
            }
        }

        FAKEBUTS::Clear_Fake_Buttons_();
        COLONY::Clear_Galaxy_Map_Fields_();
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        fields::Deactivate_Help_List_();
        MOX2::Put_Auto_Function_(&saved_auto_fn);
        COLONY::_using_colony_screen_palette = 0;
    }

    void __cdecl Update_First_(int16_t param_1, int16_t* param_2, int16_t* param_3) {
        if (COLXPORT::N_Colonies_(MOX::_PLAYER_NUM) < param_1) {
            *param_3 = -1;
            *param_2 = 0;
            return;
        }

        if ((int32_t)COLXPORT::N_Colonies_(MOX::_PLAYER_NUM) - *param_3 - *param_2 != param_1) {
            COLONY::_full_draw = 1;
            *param_2 = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM) - *param_3 - param_1;
            COLONY::_scanned_pop = -1;
        }
    }

    void __cdecl Increment_First_(int16_t num_items) {
        int16_t colonies_count = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);

        if (colonies_count >= num_items) {
            int32_t first_val = static_cast<int32_t>(_first) - 1;
            if (first_val < 1) {
                first_val = 0;
            }
            _first = static_cast<int16_t>(first_val);

            int16_t updated_colonies_count = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);
            _slider_bar_position = (updated_colonies_count - _first) - num_items;
            COLONY::_full_draw = 1;
        }
    }

    void __cdecl Decrement_First_(int16_t count) {
        int16_t num_colonies = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);

        if (num_colonies >= count) {
            _first++;
            num_colonies = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);
            _slider_bar_position = num_colonies - _first - count;
            COLONY::_full_draw = 1;
        }
    }

    void __cdecl Clear_List_Col_Array_() {
        memset(COLSUM::_list_col, 0xff, sizeof(COLSUM::_list_col));
    }

    void __cdecl Reset_List_Col_Array_() {
        _first = 0;
        Clear_List_Col_Array_();
        Update_Col_List_();
    }

    void __cdecl Init_Fields_() {
        fields::Clear_Fields_();
        FAKEBUTS::Clear_Fake_Buttons_();
        COLONY::Clear_Galaxy_Map_Fields_();
        Add_Fields_();
    }

    void __cdecl Add_Fields_() {
        int16_t num_colonies = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);

        static const int16_t buy_btn_y_coords[] = {35, 65, 96, 128, 159, 190, 221, 251, 283, 314};

        _x_fields[1] = fields::Add_Button_Field_(619, 15, ESTRINGS::E_Strings_(12), _anims[2], ESTRINGS::E_Strings_(137), 40);
        _x_fields[2] = fields::Add_Button_Field_(619, 316, ESTRINGS::E_Strings_(11), _anims[3], ESTRINGS::E_Strings_(12), 40);
        _x_fields[3] = fields::Add_Button_Field_(531, 445, ESTRINGS::E_Strings_(12), _anims[1], ESTRINGS::E_Strings_(12), 40);
        _x_fields[4] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(135));
        _x_fields[5] = fields::Add_Multi_Button_Field_(89, 446, ESTRINGS::E_Strings_(12), _anims[4], &_g_sort_index, 0, ESTRINGS::E_Strings_(384), 41);
        _x_fields[6] = fields::Add_Multi_Button_Field_(140, 446, s_p_005510fc, _anims[5], &_g_sort_index, 1, ESTRINGS::E_Strings_(12), 41);
        _x_fields[7] = fields::Add_Multi_Button_Field_(219, 446, s_f_005510f8, _anims[6], &_g_sort_index, 2, ESTRINGS::E_Strings_(12), 41);
        _x_fields[8] = fields::Add_Multi_Button_Field_(262, 446, ESTRINGS::E_Strings_(327), _anims[7], &_g_sort_index, 3, ESTRINGS::E_Strings_(12), 41);
        _x_fields[9] = fields::Add_Multi_Button_Field_(326, 446, s_s_005510f4, _anims[8], &_g_sort_index, 4, ESTRINGS::E_Strings_(12), 41);
        _x_fields[10] = fields::Add_Multi_Button_Field_(393, 446, ESTRINGS::E_Strings_(436), _anims[9], &_g_sort_index, 5, ESTRINGS::E_Strings_(12), 41);
        _x_fields[11] = fields::Add_Multi_Button_Field_(480, 446, ESTRINGS::E_Strings_(193), _anims[10], &_g_sort_index, 6, ESTRINGS::E_Strings_(12), 41);

        if (num_colonies < 10) {
            _x_fields[12] = -1000;
        } else {
            _x_fields[12] = fields::Add_Scroll_Field_(621, 40, 0, num_colonies, 0, num_colonies - 10, 5, 271, &_slider_bar_position, ESTRINGS::E_Strings_(12), 0);
        }

        _x_fields[13] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(596));

        int16_t y_row_end = 65;
        for (int16_t i = 0; i < 10; ++i) {
            if (_list_col[i] == -1) {
                _list_fields[i] = -1000;
                _list_col_prod_fields[i] = -1000;
                _list_buy_fields[i] = -1000;
            } else {
                int16_t y1 = y_row_end - 30;
                _list_fields[i] = fields::Add_Hidden_Field_(12, y1, 101, y_row_end, ESTRINGS::E_Strings_(12), 0);
                _list_col_prod_fields[i] = fields::Add_Hidden_Field_(512, y1, 597, y_row_end, ESTRINGS::E_Strings_(12), 0);

                int16_t col_idx = _list_col[i];
                if (COLONY::Colony_Can_Buy_Product_0_(col_idx) == 0 && MOX::_colony[col_idx].bought_outright == 0) {
                    short bx1 = 599;
                    short by1 = buy_btn_y_coords[i];
                    short w = animate::Get_Width_(_anims[i + 11]);
                    short h = animate::Get_Height_(_anims[i + 11]);
                    _list_buy_fields[i] = fields::Add_Hidden_Field_(bx1, by1, bx1 + w, by1 + h, ESTRINGS::E_Strings_(12), 0);
                } else {
                    _list_buy_fields[i] = fields::Add_Locked_Button_Field_(599, buy_btn_y_coords[i], ESTRINGS::E_Strings_(12), _anims[i + 11], &_list_buy_data[i], ESTRINGS::E_Strings_(12), 41);
                }
            }
            y_row_end += 31;
            Add_Fields_Pop_For_(i);
        }

        _x_fields[0] = fields::Add_Hidden_Field_(0, 0, 639, 479, s__005510f0, 0);
    }

    void __cdecl Add_Fields_Pop_For_(int16_t colony_idx) {
        int16_t col_id = _list_col[colony_idx];
        for (int16_t i = 0; i < 3; ++i) {
            int32_t idx = colony_idx * 3 + i;
            if (col_id == -1) {
                _list_col_job_fields[idx] = -1000;
            } else {
                int16_t val1, val2;
                if (i == 0) {
                    val2 = 101;
                    val1 = 236;
                } else if (i == 1) {
                    val2 = 236;
                    val1 = 378;
                } else {
                    val2 = 378;
                    val1 = 512;
                }

                COLDRAW::Do_Colony_Info_Pop_Stuff_For_Pop_(
                    col_id,
                    i,
                    1,
                    val2,
                    (int16_t)(colony_idx * 31 + 34),
                    (int16_t)(val1 - 10),
                    0,
                    _list_col_pop_ref[idx].refs,
                    &_list_col_pop_count[idx],
                    &_list_col_coord_vars[idx],
                    &_list_col_job_fields[idx]
                );
            }
        }
    }

    void __cdecl Update_Col_List_() {
        for (int i = 0; i < 10; ++i) {
            int16_t colony_idx = COLXPORT::_g_colony_list_ptr[COLSUM::_first + i];
            COLSUM::_list_col[i] = colony_idx;

            if (colony_idx == -1) {
                COLSUM::_list_buy_data[i] = 0;
            } else {
                COLSUM::_list_buy_data[i] = MOX::_colony[colony_idx].bought_outright;
            }
        }

        COLSUM::Init_Fields_();
    }

    void __cdecl Sort_Col_List_() {
        int16_t outer_count = 250;
        do {
            for (int16_t i = 0; i < 249; ++i) {
                int16_t val1 = COLXPORT::_g_colony_list_ptr[i];
                int16_t val2 = COLXPORT::_g_colony_list_ptr[i + 1];

                if (Switched_cmp_(val1, val2) > 0) {
                    COLXPORT::_g_colony_list_ptr[i] = val2;
                    COLXPORT::_g_colony_list_ptr[i + 1] = val1;
                }
            }
            outer_count--;
        } while (outer_count != 0);
    }

    void __cdecl Load_Col_Sum_Anims_() {
        COLBLDG::Load_Build_Anims_();

        for (int i = 0; i < 21; ++i) {
            _anims[i] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLSUM_LBX, i, MOX::_screen_seg);
        }

        COLONY::Load_Galaxy_Map_Anims_();
    }

    void __cdecl Draw_Galaxy_Map_() {
        MOVEBOX::Draw_Galaxy_Map_Box_(nullptr, 0, 0x17c, 0x15d, 0x80, 0x5b, 0, 0, 0, 0, 3, 0);
    }

    void __cdecl Draw_Empire_Info_() {
        char buffer[200];

        Set_Colony_Font_To_(2);

        auto& player = MOX::_player[MOX::_PLAYER_NUM];

        // Part 0: BC
        ERIC::String_Builder2_(buffer, 0, ESTRINGS::E_Strings_(0x76 /* 118 */), s_0_0055110c, s_1_00551110, player.bc);

        // Part 1: Surplus BC (with color formatting)
        const char* red_fmt = ERIC::Red_If_Negative_Fmt_String_(player.surplus_bc);
        ERIC::String_Builder2_(buffer, 1, ESTRINGS::E_Strings_(0x6a /* 106 */), s_0_0055110c, s_1_00551110, red_fmt, (int)player.surplus_bc);

        // Part 2: Total Population
        ERIC::String_Builder2_(buffer, 2, ESTRINGS::E_Strings_(0x72 /* 114 */), s_0_0055110c, s_1_00551110, (int)player.total_pop);

        // Part 3: Surplus Freighters
        ERIC::String_Builder2_(buffer, 3, ESTRINGS::E_Strings_(0x67 /* 103 */), s_0_0055110c, s_1_00551110, (int)player.surplus_freighters);

        // Part 4: Surplus Food
        ERIC::String_Builder2_(buffer, 4, ESTRINGS::E_Strings_(0x66 /* 102 ESTR_SFOOD_SD */), s_0_0055110c, s_1_00551110, (int)player.surplus_food);

        // Part 5: Research Produced
        ERIC::String_Builder2_(buffer, 5, ESTRINGS::E_Strings_(0x75 /* 117 */), s_0_0055110c, s_1_00551110, (int)player.research_produced);

        // Render the built strings
        FMTPARA::Print_Formatted_Paragraph_(520 /* 0x208 */, 354 /* 0x162 */, 104 /* 0x68 */, buffer, 3);

        COLONY::Set_Colony_Font_();
    }

    void __cdecl Draw_Colony_Summary_Screen_() {
        COLONY::_g_count++;
        video::Set_Page_Off_();
        graphics::Set_Window_(0, 0, 639, 479);
        video::Clip_On_();
        Update_First_(10, &_first, &_slider_bar_position);

        if (COLONY::_full_draw != 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
            graphics::Fill_(0, 0, 639, 479, 0);
            animate::Draw_(0, 0, _anims[0]);
            Clear_List_Col_Array_();
            Update_Col_List_();
            Draw_Empire_Info_();
        }

        Draw_Galaxy_Map_();
        for (int16_t i = 0; i < 10; ++i) {
            if (_list_col[i] != -1) {
                Draw_Colony_Summary_For_Colony_(i);
            }
        }

        if (COLONY::_full_draw != 0) {
            if (fields::_draw_fields_flag == 0) {
                fields::Enable_Draw_All_Fields_();
                fields::Draw_Visible_Fields_();
                fields::Disable_Draw_All_Fields_();
            } else {
                fields::Draw_Visible_Fields_();
            }
            video::Copy_Off_To_Back_();
        }

        Draw_Scan_Info_();

        if (COLMOVE::_cluster_colony_n != -1 && COLONY::_g_colony_n != -1) {
            s_colors colors = { 0xE8, 0xE9, 0xEA, 0xEB, 0xEA, 0xE9, 0, 0 };
            int16_t star1 = MOX::_planet[MOX::_colony[COLMOVE::_cluster_colony_n].planet].star_index;
            int16_t star2 = MOX::_planet[MOX::_colony[COLONY::_g_colony_n].planet].star_index;
            Colsum_Connect_Galaxy_Map_Stars_(star1, star2, &colors);
        }

        if (_draw_planet_sum == 1) {
            int16_t index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
            COLONY::Draw_Planet_Summary_(229, 135, index);
        }

        Draw_Bar_Indicator_();
        FAKEBUTS::Draw_Fake_Buttons_();
        fields::Draw_Visible_Fields_();

        int16_t pointer_y = mouse::Pointer_Y_();
        int16_t pointer_x = mouse::Pointer_X_();
        COLMOVE::Draw_Cluster_(pointer_x, pointer_y);

        video::Clip_Off_();
    }

    void __cdecl Draw_Colony_Summary_() {
        fields::Deactivate_Auto_Function_();

        // Sets up the auto-redraw function for the colony summary screen
        fields::Assign_Auto_Function_(Draw_Colony_Summary_Screen_, 1);

        COLONY::_full_draw = 1;
        video::Set_Page_Off_();

        Draw_Colony_Summary_Screen_();

        video::Copy_Off_To_Back_();
        video::Set_Refresh_Stencil_();

        // Updates context-sensitive help for the current screen
        ERICHELP::Set_Colony_Summary_Screen_Help_List_();
    }

    bool __cdecl Event_Good_(int16_t event_id) {
        return static_cast<bool>(_event_good_lookup[event_id]);
    }

    void __cdecl Draw_Colony_Summary_For_Colony_(int16_t list_idx) {
        char buffer[500];
        char sub_buffer1[500];
        char sub_buffer2[500];

        int16_t colony_idx = _list_col[list_idx];
        s_colony* colony = &MOX::_colony[colony_idx];
        int16_t planet_idx = colony->planet;

        fonts::Set_Outline_Color_(0x50, 0x50);

        if (colony_idx == COLONY::_g_colony_n) {
            Draw_Info_Pop_For_(list_idx);
        }

        int16_t event_id = EVENTS::Colony_Has_Event_(colony_idx);
        COLONY::Set_Colony_Font_To_Blue_(2, colony_idx == COLONY::_g_colony_n);

        uint16_t paragraph_type;
        if (event_id == -1) {
            s_planet_data* planet = &MOX::_planet[planet_idx];
            s_star_data* star = &MOX::_star[planet->star_index];

            if ((star->blockaded >> (MOX::_PLAYER_NUM & 0x1F) & 1) == 0) {
                _sprintf(sub_buffer1, ESTRINGS::E_Strings_(0x0C));
                _sprintf(sub_buffer2, ESTRINGS::E_Strings_(0x0C));
            } else {
                _sprintf(sub_buffer1, ESTRINGS::E_Strings_(0x46), s_2_00551108);
                _sprintf(sub_buffer2, ESTRINGS::E_Strings_(0x86));
            }

            _sprintf(buffer, ESTRINGS::E_Strings_(0x51), sub_buffer1, HACCESS::Get_Planet_Name_No_Prime_(planet_idx), sub_buffer2);
            paragraph_type = 0x59;
        } else {
            const char* event_prefix = Event_Good_(event_id) ? s_1_00551114 : s_2_00551108;
            _sprintf(sub_buffer1, ESTRINGS::E_Strings_(0x46), event_prefix);
            _sprintf(buffer, ESTRINGS::E_Strings_(0x55), HACCESS::Get_Planet_Name_No_Prime_(planet_idx), sub_buffer1, _wee_event_strings[event_id]);
            paragraph_type = 0x57;
        }

        int16_t y_pos = list_idx * 0x1F + 0x26;
        BILL::Squeeze_Formatted_Paragraph_Centered_(0x0C, y_pos, paragraph_type, 0x17, buffer, 0);

        if (COLONY::_full_draw != 0) {
            fonts::Set_Font_Style_Shadow_Down_(0, &COLONY::_font_color_array);
            Draw_Info_Pop_For_(list_idx);

            int16_t turn_count = COLONY::Calculate_Current_Production_Turn_Count_(colony_idx);
            Set_Colony_Font_To_(1);

            int16_t item_producing = colony->producing[0];
            if (item_producing != -1) {
                if (turn_count < 1 || turn_count > 9999) {
                    if (colony->auto_building == 0) {
                        _sprintf(buffer, ESTRINGS::E_Strings_(0x46), COLBLDG::Selection_Name_(item_producing));
                    } else {
                        _sprintf(buffer, ESTRINGS::E_Strings_(0x5D), s_1_00551114, s_0_00551104, COLBLDG::Selection_Name_(item_producing));
                    }
                } else {
                    if (colony->auto_building == 0) {
                        _sprintf(buffer, ESTRINGS::E_Strings_(0x27), COLBLDG::Selection_Name_(item_producing), (int)turn_count);
                    } else {
                        _sprintf(buffer, ESTRINGS::E_Strings_(0x5C), s_1_00551114, s_0_00551104, COLBLDG::Selection_Name_(item_producing), (int)turn_count);
                    }
                }

                COLONY::Set_Colony_Font_To_Blue_(2, 0);
                BILL::Squeeze_Print_Formatted_Paragraph_(0x200, y_pos, 0x55, 0x16, buffer, 0);
                fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
                return;
            }

            if (colony->auto_building != 0) {
                _sprintf(buffer, ESTRINGS::E_Strings_(0x5D), s_1_00551114, s_0_00551104, COLBLDG::Selection_Name_(-1));
                BILL::Squeeze_Print_Formatted_Paragraph_(0x200, y_pos, 0x55, 0x16, buffer, 0);
                fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
            }
        }
    }

    void __cdecl Load_Colsum_Strings_() {
        if (_colsum_strings_been_loaded != 0) {
            return;
        }

        _colsum_strings_been_loaded = 1;

        _wee_event_strings[0] = ESTRINGS::E_Strings_(0x104);
        _wee_event_strings[1] = ESTRINGS::E_Strings_(0xdd);
        _wee_event_strings[2] = ESTRINGS::E_Strings_(0xf0);
        _wee_event_strings[3] = ESTRINGS::E_Strings_(0x227);
        _wee_event_strings[4] = ESTRINGS::E_Strings_(0xcc);
        _wee_event_strings[5] = ESTRINGS::E_Strings_(0x171);
        _wee_event_strings[6] = ESTRINGS::E_Strings_(0x10e);
        _wee_event_strings[7] = ESTRINGS::E_Strings_(0x117);
        _wee_event_strings[8] = ESTRINGS::E_Strings_(0xa7);
        _wee_event_strings[9] = ESTRINGS::E_Strings_(0x127);
        _wee_event_strings[10] = ESTRINGS::E_Strings_(0xa7);
        _wee_event_strings[11] = ESTRINGS::E_Strings_(0x102);
        _wee_event_strings[12] = ESTRINGS::E_Strings_(0x103);
        _wee_event_strings[13] = ESTRINGS::E_Strings_(0x17d);
        _wee_event_strings[14] = ESTRINGS::E_Strings_(0x19e);
        _wee_event_strings[15] = ESTRINGS::E_Strings_(0x19d);
        _wee_event_strings[16] = ESTRINGS::E_Strings_(0x19f);
        _wee_event_strings[17] = ESTRINGS::E_Strings_(0xce);
        _wee_event_strings[18] = ESTRINGS::E_Strings_(0x1d2);
        _wee_event_strings[19] = ESTRINGS::E_Strings_(0xad);
        _wee_event_strings[20] = ESTRINGS::E_Strings_(0xf6);
        _wee_event_strings[21] = ESTRINGS::E_Strings_(0x110);
        _wee_event_strings[22] = ESTRINGS::E_Strings_(0x119);
        _wee_event_strings[23] = ESTRINGS::E_Strings_(0x144);
        _wee_event_strings[24] = ESTRINGS::E_Strings_(0x18e);
        _wee_event_strings[25] = ESTRINGS::E_Strings_(0xb6);
        _wee_event_strings[26] = ESTRINGS::E_Strings_(0xc6);
        _wee_event_strings[27] = ESTRINGS::E_Strings_(0x12e);
        _wee_event_strings[28] = ESTRINGS::E_Strings_(0x22e);
        _wee_event_strings[29] = ESTRINGS::E_Strings_(0x1b6);
        _wee_event_strings[30] = ESTRINGS::E_Strings_(0x1a1);
        _wee_event_strings[31] = ESTRINGS::E_Strings_(0x135);
        _wee_event_strings[32] = ESTRINGS::E_Strings_(0x193);
        _wee_event_strings[33] = ESTRINGS::E_Strings_(0xff);
        _wee_event_strings[34] = ESTRINGS::E_Strings_(0x1f4);
        _wee_event_strings[35] = ESTRINGS::E_Strings_(0x186);
    }

    void __cdecl Draw_Info_Pop_For_(int16_t colony_idx) {
        int16_t colony_id = _list_col[colony_idx];
        int16_t y_pos = colony_idx * 31 + 38;

        for (int16_t i = 0; i < 3; ++i) {
            int32_t x1;
            int32_t x2_base;

            if (i == 0) {
                x1 = 101;
                x2_base = 236;
            } else if (i == 1) {
                x1 = 236;
                x2_base = 378;
            } else {
                x1 = 378;
                x2_base = 512;
            }

            int32_t index = i + (colony_idx * 3);

            COLDRAW::Do_Colony_Info_Pop_Stuff_For_Pop_(
                colony_id,
                i,
                0,
                x1,
                y_pos,
                x2_base - 10,
                0,
                _list_col_pop_ref[index].refs,
                &_list_col_pop_count[index],
                &_list_col_coord_vars[index],
                &_list_col_job_fields[index]
            );
        }
    }

    void __cdecl Colsum_Connect_Galaxy_Map_Stars_(int16_t star_idx1, int16_t star_idx2, s_colors* pattern) {
        int16_t x1, y1, x2, y2;

        MOVEBOX::Get_Galaxy_Map_Star_XY_(star_idx1, 380, 349, 128, 91, 0, 0, 0, &x1, &y1);
        MOVEBOX::Get_Galaxy_Map_Star_XY_(star_idx2, 380, 349, 128, 91, 0, 0, 0, &x2, &y2);

        SHIPS::Draw_Directional_Multi_Colored_Line_(
            x1 + 2,
            y1 + 2,
            x2 + 2,
            y2 + 2,
            pattern,
            static_cast<int16_t>(COLONY::_g_count % 6)
        );
    }

    /**
     * Draws the scroll bar indicator (thumb) for the colony summary list.
     */
    void __cdecl Draw_Bar_Indicator_() {
        int16_t num_colonies = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);
        COLSUM::Update_First_(10, &COLSUM::_first, &COLSUM::_slider_bar_position);

        if (num_colonies > 10) {
            int32_t y1 = (271 * (int32_t)COLSUM::_first) / (int32_t)num_colonies + 40;
            int32_t y2 = (271 * ((int32_t)COLSUM::_first + 10)) / (int32_t)num_colonies + 40;

            graphics::Set_Window_(0, 0, 639, 479);
            video::Clip_On_();

            // Draw the indicator bar body
            graphics::Fill_(621, (int16_t)y1, 626, (int16_t)y2, 229);

            // Draw the indicator bar borders (shaded edges)
            line::Line_(621, (int16_t)y1, 626, (int16_t)y1, 230);
            line::Line_(621, (int16_t)y1, 621, (int16_t)y2, 230);
            line::Line_(626, (int16_t)y1, 626, (int16_t)y2, 228);
            line::Line_(621, (int16_t)y2, 626, (int16_t)y2, 228);

            // Draw corner markers for the scroll track area
            graphics::Dot_(621, 40, 80);
            graphics::Dot_(626, 40, 80);
            graphics::Dot_(621, 311, 80);
            graphics::Dot_(626, 311, 80);
        }

        video::Clip_Off_();
    }

    bool __cdecl Evaluate_Input_(int16_t* field_ptr, int16_t* param_2) {
        int16_t field = *field_ptr;

        if (field == _x_fields[1]) {
            Increment_First_(10);
            return true;
        }

        if (field == _x_fields[2]) {
            if (COLXPORT::_g_colony_list_ptr[_first + 10] != -1) {
                Decrement_First_(10);
                return true;
            }
        }

        if (field == _x_fields[3] || field == _x_fields[4]) {
            if (COLMOVE::_cluster_colony_n != -1) {
                COLMOVE::Clear_Cluster_();
                COLONY::Reset_Mouse_Picture_();
            }

            if (MOX::_return_screen == MOX::_current_screen && COLBLDG::_return_to_build_popup_from_custom_design != 0) {
                MOX::_current_screen = SCREEN_QUEUE_POPUP;
            } else {
                MOX::_current_screen = _colony_summary_saved_screen;
                MOX::_return_screen = _colony_summary_saved_screen;
            }

            if (*field_ptr == _x_fields[4]) {
                *field_ptr = _x_fields[3];
            }
            return true;
        }

        if (field == _x_fields[13]) {
            COLONY::_full_draw = 1;
            return true;
        }

        // Check if field is one of the sorting headers (indices 5-11)
        if (field == _x_fields[5] || field == _x_fields[6] || field == _x_fields[7] ||
            field == _x_fields[8] || field == _x_fields[9] || field == _x_fields[10] ||
            field == _x_fields[11]) {
            Sort_Col_List_();
            Clear_List_Col_Array_();
            _first = 0;
            Decrement_First_(10);
            Update_First_(10, &_first, &_slider_bar_position);
            Update_Col_List_();
            COLONY::_full_draw = 1;
            return true;
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            Draw_Colony_Summary_();
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            return true;
        }

        bool result = Evaluate_Col_List_Input_(field_ptr, param_2);
        Check_Galaxy_Map_Fields_(field_ptr, param_2);

        return result;
    }

    /**
     * Evaluates UI input for the population management section of the colony summary screen.
     * Updates colony scanning and cluster selection based on the active UI fields.
     */
    void __cdecl Evaluate_Colony_Pop_Input_(int16_t* input_field_1, int16_t* input_field_2, int16_t colony_idx_in_list) {
        if (colony_idx_in_list == 0) {
            COLONY::_scanned_pop = -1;
        }

        for (int16_t job_idx = 0; job_idx < 3; ++job_idx) {
            int field_idx = colony_idx_in_list * 3 + job_idx;
            int16_t job_field_id = _list_col_job_fields[field_idx];

            if (*input_field_1 == job_field_id) {
                if (COLMOVE::_cluster_colony_n == -1) {
                    int16_t selected_pop = Get_Selected_Pop_(colony_idx_in_list, job_idx);
                    if (selected_pop != -1) {
                        COLMOVE::Get_Cluster_(_list_col[colony_idx_in_list], selected_pop);
                    }
                    COLONY::_full_draw = 1;
                } else {
                    COLMOVE::Send_Cluster_(_list_col[colony_idx_in_list], job_idx);
                    Draw_Colony_Summary_();
                }
            }

            if (*input_field_2 == job_field_id) {
                COLONY::_scanned_pop = Get_Scanned_Pop_(colony_idx_in_list, job_idx);
            }
        }

        int16_t active_field_2 = *input_field_2;
        if (_list_fields[colony_idx_in_list] == active_field_2 ||
            _list_col_prod_fields[colony_idx_in_list] == active_field_2 ||
            _list_buy_fields[colony_idx_in_list] == active_field_2) 
        {
            int16_t colony_id = _list_col[colony_idx_in_list];
            COLONY::_g_colony_n = colony_id;
            
            // Accessing planet index from colony structure (size 361) and star index from planet structure (size 17)
            int16_t planet_idx = MOX::_colony[colony_id].planet;
            MOX::_galaxy_map_scanned_star = static_cast<int16_t>(MOX::_planet[planet_idx].star_index);
        }
    }

    bool __cdecl Evaluate_Col_List_Input_(int16_t* p_input_field, int16_t* p_input_field_2) {
        if (COLSUM::_draw_planet_sum == 1) {
            if (*p_input_field == 0) {
                return true;
            }
            COLSUM::_draw_planet_sum = 0;
            if (*p_input_field > 0) {
                *p_input_field = 0;
            }
        }

        for (int i = 0; i < 10; ++i) {
            int16_t current_input = *p_input_field;

            if (COLSUM::_list_fields[i] == current_input) {
                if (COLMOVE::_cluster_colony_n != -1) {
                    COLMOVE::Send_Cluster_(COLSUM::_list_col[i], -1);
                    COLSUM::Draw_Colony_Summary_();
                } else {
                    int16_t colony_idx = COLSUM::_list_col[i];
                    int16_t planet_idx = MOX::_colony[colony_idx].planet;

                    MOX::_screen_data = (int16_t)MOX::_planet[planet_idx].star_index;
                    COLONY::_orbit_temp = (int16_t)MOX::_planet[planet_idx].orbit;

                    MOX::_return_screen = MOX::_current_screen;
                    MOX::_current_screen = 1;
                    *p_input_field = _x_fields[3];
                }
            } else if (_list_col_prod_fields[i] == current_input) {
                fields::Deactivate_Auto_Function_();
                FAKEBUTS::Clear_Fake_Buttons_();
                COLONY::Clear_Galaxy_Map_Fields_();
                fields::Clear_Fields_();

                int16_t colony_idx = COLSUM::_list_col[i];
                int16_t planet_idx = MOX::_colony[colony_idx].planet;

                MOX::_screen_data = (int16_t)MOX::_planet[planet_idx].star_index;
                COLONY::_orbit_temp = (int16_t)MOX::_planet[planet_idx].orbit;

                int16_t target_planet = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
                MOX::_planet[target_planet].colony_index = colony_idx;

                if (COLMOVE::_cluster_colony_n != -1) {
                    COLMOVE::Clear_Cluster_();
                    COLONY::Reset_Mouse_Picture_();
                }

                MOX::_return_screen = MOX::_current_screen;
                MOX::_current_screen = 25 /* 0x19 */;
                return false;
            } else if (COLSUM::_list_fields[i] + current_input == 0) {
                int16_t colony_idx = COLSUM::_list_col[i];
                int16_t planet_idx = MOX::_colony[colony_idx].planet;

                MOX::_screen_data = (int16_t)MOX::_planet[planet_idx].star_index;
                COLONY::_orbit_temp = (int16_t)MOX::_planet[planet_idx].orbit;
                COLSUM::_draw_planet_sum = 1;
            } else if (_list_buy_fields[i] == current_input) {
                COLONY::Tested_Colony_Buys_Outright_(COLSUM::_list_col[i]);
                COLSUM::Draw_Colony_Summary_();
            }

            COLSUM::Evaluate_Colony_Pop_Input_(p_input_field, p_input_field_2, i);
        }

        return true;
    }

    /**
     * @address 00403590
     */
    int16_t __cdecl Get_Scanned_Pop_(int16_t param_1, int16_t param_2) {
        int16_t colony_idx = _list_col[param_1];
        int16_t val_ebx;
        int16_t val_ebp;

        if (param_2 == -1) {
            val_ebx = 101 /* 0x65 */;
        } else if (param_2 == 0) {
            val_ebx = 236 /* 0xec */;
        } else {
            val_ebx = (param_2 == 1) ? 378 /* 0x17a */ : 512 /* 0x200 */;
        }

        if (param_2 == 0) {
            val_ebp = 101 /* 0x65 */;
        } else if (param_2 == 1) {
            val_ebp = 236 /* 0xec */;
        } else {
            val_ebp = (param_2 == 2) ? 378 /* 0x17a */ : 512 /* 0x200 */;
        }

        int32_t list_idx = (int32_t)param_1 * 3 + (int32_t)param_2;

        int16_t result = COLDRAW::Do_Colony_Info_Pop_Stuff_For_Pop_(
            colony_idx,
            param_2,
            4,
            val_ebp,
            (int16_t)(param_1 * 31 + 38),
            (int16_t)(val_ebx - 10),
            0,
            _list_col_pop_ref[list_idx].refs,
            &_list_col_pop_count[list_idx],
            &_list_col_coord_vars[list_idx],
            &_list_col_job_fields[list_idx]
        );

        COLONY::_g_colony_n = colony_idx;
        MOX::_galaxy_map_scanned_star = (int16_t)MOX::_planet[MOX::_colony[colony_idx].planet].star_index;

        return result;
    }

    int16_t __cdecl Get_Selected_Pop_(int16_t colony_idx, int16_t job_idx) {
        int32_t ebx_val;
        if (job_idx == -1) {
            ebx_val = 101;
        } else if (job_idx == 0) {
            ebx_val = 236;
        } else {
            ebx_val = (job_idx == 1) ? 378 : 512;
        }

        int32_t ebp_val;
        if (job_idx == 0) {
            ebp_val = 101;
        } else if (job_idx == 1) {
            ebp_val = 236;
        } else {
            ebp_val = (job_idx == 2) ? 378 : 512;
        }

        int32_t index = (int32_t)colony_idx * 3 + (int32_t)job_idx;

        return COLDRAW::Do_Colony_Info_Pop_Stuff_For_Pop_(
            _list_col[colony_idx],
            job_idx,
            3,
            (int16_t)ebp_val,
            (int16_t)(colony_idx * 31 + 38),
            (int16_t)(ebx_val - 10),
            0,
            _list_col_pop_ref[index].refs,
            &_list_col_pop_count[index],
            &_list_col_coord_vars[index],
            &_list_col_job_fields[index]
        );
    }

    void __cdecl Set_Colony_Font_To_(int16_t style) {
        fonts::Set_Outline_Color_(0x50, 0x50);
        fonts::Set_Font_Style_Shadow_Down_(static_cast<int32_t>(style & 0xFF), &COLONY::_font_color_array);
        fonts::Set_Font_Highlight_Colors_(&COLONY::_bright_blue_font_color_array);
        fonts::Set_Font_Special_Colors_(&COLONY::_red_font_color_array[0]);
    }
}