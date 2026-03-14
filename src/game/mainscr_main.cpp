#include "pch.h"

namespace MAINSCR {
    void __cdecl Print_Main_Screen_Data_() {
        char sep = (MOX::_settings.language == 1 || MOX::_settings.language == 2) ? ',' : '.';
        s_colors colors1, colors2;
        char buffer[128];
        char monetary_unit[16];
        char research_unit[16];

        memset(&colors1, 0, sizeof(colors1));
        memset(&colors2, 0, sizeof(colors2));

        fonts::Set_Font_Style_(0, &colors1);
        fonts::Get_Font_Height_();
        _sprintf(buffer, s__d_c_d_0055996c, (int)(MOX::_stardate / 10), (int)sep, (int)(MOX::_stardate % 10));
        MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, -1, 0x43, 1);
        int16_t stardate_font_style = MISC::Get_Font_Style_();
        s_colors* p_stardate_colors = MISC::Font_Colors2_(stardate_font_style, 14, 23);
        memcpy(&colors2, p_stardate_colors, 8);
        fonts::Set_Outline_Color_(2, 1);
        fonts::Set_Font_Style_Outline_(stardate_font_style, &colors2);
        fonts::Print_Centered_(580, 29, buffer); // 0x244, 0x1d

        if (EVENTS::Event_Check_Hyperspace_Flux_()) {
            MISC::Get_Mox_Font_Colors_(3, 'I', 'L', &colors1);
            fonts::Set_Font_Style_(3, &colors1);
            MISC::Increase_Font_Color_Range_(((int)MOX::_red_color_pulse_value * 5) / 9);
            const char* flux_msg = HAROLD::H_Message_(0xe0);
            char flux_buffer[128];
            strcpy(flux_buffer, flux_msg);
            int16_t flux_width = fonts::Get_String_Width_(flux_buffer);
            int16_t flux_x = 526 - flux_width; // 0x20e

            uint32_t line_color1 = *(uint32_t*)colors1.colors;
            uint32_t line_color2 = *((uint32_t*)colors1.colors + 1);

            line::Line_(flux_x, 21, 526, 21, line_color1);
            line::Line_(flux_x, 22, 526, 22, line_color2);
            fonts::Print_Right_(526, 24, flux_buffer);

            int16_t fh = fonts::Get_Font_Height_();
            line::Line_(flux_x, fh + 25, 526, fh + 25, line_color2);
            line::Line_(flux_x, fh + 26, 526, fh + 26, line_color1);
        }

        fonts::Get_Font_Height_();
        HAROLD::Get_Monetary_Unit_String_(monetary_unit);
        s_player* plr = &MOX::_player[MOX::_PLAYER_NUM];

        char* bc_comma = MISC::Get_Comma_String_(plr->bc);
        _sprintf(buffer, s__s__s_00558014, bc_comma, monetary_unit);

        if (plr->bc < 0) {
            MISC::Set_Fitted_Font_Style_(buffer, &colors1, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_('K', 'M');
            MISC::Increase_Font_Color_Range_(((int)MOX::_red_color_pulse_value * 4) / 7);
            fonts::Set_Outline_Color_(0x49, 0x49);
        }
        else {
            MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
            fonts::Set_Outline_Color_(2, 1);
        }
        fonts::Print_Centered_(579, 96, buffer); // 0x243, 0x60

        char* surplus_bc_comma = MISC::Get_Comma_String_(plr->surplus_bc);
        const char* bc_sign = (plr->surplus_bc < 0) ? s__00551100 : s_plus_00551118;
        _sprintf(buffer, s__s_s__s_00559964, bc_sign, surplus_bc_comma, monetary_unit);
        MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
        int16_t bc_style = MISC::Get_Font_Style_();
        int16_t bc_spacing = MISC::Get_Font_Spacing_();
        MISC::Get_Mox_Font_Colors_(bc_style, 75, 77, &colors1);
        MISC::Get_Mox_Font_Colors_(bc_style, 14, 23, &colors2);

        if (plr->surplus_bc < 0) {
            fonts::Set_Outline_Color_(0x49, 0x49);
            fonts::Set_Font_Style_Outline_(bc_style, &colors1);
            fonts::Set_Font_Spacing_(bc_spacing);
            MISC::Increase_Font_Color_Range_(((int)MOX::_red_color_pulse_value * 4) / 7);
        }
        else {
            fonts::Set_Font_Style_Outline_(bc_style, &colors2);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Set_Font_Spacing_(bc_spacing);
        }
        fonts::Print_Centered_(579, fonts::Get_Font_Height_() + 96, buffer);

        int16_t cp_surplus = plr->command_points - plr->command_points_used;
        const char* cp_sign = (cp_surplus < 0) ? s__00551100 : s_plus_00551118;
        _sprintf(buffer, s__s_d___d__00559958, cp_sign, (int)cp_surplus, (int)plr->command_points);
        MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 0);
        int16_t current_style = (int16_t)fonts::_current_font_style;
        int16_t current_spacing = MISC::Get_Font_Spacing_();
        MISC::Get_Mox_Font_Colors_(current_style, 'K', 'M', &colors1);
        MISC::Get_Mox_Font_Colors_(current_style, 14, 23, &colors2);

        char cp_left[64];
        _sprintf(cp_left, s__s_d_00556134, cp_sign, (int)cp_surplus);
        int16_t cp_left_w = fonts::Get_String_Width_(cp_left);
        char cp_right[64];
        _sprintf(cp_right, s___d__00559950, (int)plr->command_points);
        int16_t cp_total_w = (int16_t)(fonts::Get_String_Width_(cp_right) + cp_left_w + 4);

        if (cp_surplus < 0) {
            fonts::Set_Font_Style_Outline_(current_style, &colors1);
            fonts::Set_Outline_Color_(0x49, 0x49);
            fonts::Set_Font_Spacing_(current_spacing);
            MISC::Increase_Font_Color_Range_(((int)MOX::_red_color_pulse_value * 4) / 7);
        }
        else {
            fonts::Set_Font_Style_Outline_(current_style, &colors2);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Set_Font_Spacing_(current_spacing);
        }
        int16_t cp_half_w = (int16_t)(cp_total_w / 2);
        fonts::Print_(581 - cp_half_w, 181, cp_left);
        fonts::Set_Font_Style_(current_style, &colors2);
        fonts::Set_Font_Spacing_(current_spacing);
        fonts::Print_(581 + cp_left_w + 4 - cp_half_w, 181, cp_right);

        fonts::Get_Font_Height_();
        char* food_comma = MISC::Get_Comma_String_(plr->surplus_food);
        const char* food_sign = (plr->surplus_food < 0) ? s__00551100 : s_plus_00551118;
        _sprintf(buffer, s__s_s_00556aec, food_sign, food_comma);
        if (plr->surplus_food < 0) {
            MISC::Set_Fitted_Font_Style_(buffer, &colors1, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_('K', 'M');
            fonts::Set_Outline_Color_(0x49, 0x49);
            MISC::Increase_Font_Color_Range_(((int)MOX::_red_color_pulse_value * 4) / 7);
        }
        else {
            MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
        }
        fonts::Print_Centered_(579, 255, buffer);

        const char* fr_sign = (plr->surplus_freighters < 0) ? s__00551100 : s_plus_00551118;
        _sprintf(buffer, s__s_d___d__00559958, fr_sign, (int)plr->surplus_freighters, (int)plr->n_freighters);
        MISC::Set_Fitted_Font_Style_(buffer, &colors1, 2, 2, 4, 0x43, 1);
        int16_t fr_style = MISC::Get_Font_Style_();
        MISC::Get_Mox_Font_Colors_(fr_style, 'K', 'M', &colors1);
        MISC::Get_Mox_Font_Colors_(fr_style, 14, 23, &colors2);
        int16_t fr_spacing = MISC::Get_Font_Spacing_();
        int16_t fr_total_w = fonts::Get_String_Width_(buffer);

        _sprintf(buffer, s__s_d_00556134, fr_sign, (int)plr->surplus_freighters);
        if (plr->surplus_freighters < 0) {
            fonts::Set_Font_Style_Outline_(fr_style, &colors1);
            fonts::Set_Font_Spacing_(fr_spacing);
            MISC::Increase_Font_Color_Range_(((int)MOX::_red_color_pulse_value * 4) / 7);
            fonts::Set_Outline_Color_(0x49, 0x49);
        }
        else {
            fonts::Set_Font_Style_Outline_(fr_style, &colors2);
            fonts::Set_Font_Spacing_(fr_spacing);
            fonts::Set_Outline_Color_(2, 1);
        }
        int16_t fr_half_w = (int16_t)((fr_total_w + 4) / 2);
        fonts::Print_(581 - fr_half_w, 330, buffer);

        fonts::Set_Outline_Color_(2, 1);
        int16_t fr_left_w = fonts::Get_String_Width_(buffer);
        _sprintf(buffer, s___d__00559950, (int)plr->n_freighters);
        fonts::Set_Font_Style_Outline_(fr_style, &colors2);
        fonts::Set_Font_Spacing_(fr_spacing);
        fonts::Print_(fr_left_w - fr_half_w + 583, 330, buffer);

        Get_Research_Unit_String_(research_unit);
        if (plr->research_breakthrough != 0) {
            fonts::Set_Outline_Color_(2, 1);
            fonts::Set_Font_Style_Outline_(0, &colors2);
            fonts::Print_Centered_(579, 403, HAROLD::H_Message_(0x183));
            return;
        }
        if (plr->current_research_field == 0) {
            fonts::Set_Outline_Color_(2, 1);
            MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Print_Centered_(579, 403, HAROLD::H_Message_(0x188));
            return;
        }

        int16_t turns = COLCALC::Player_N_Turns_Until_Research_Complete_(plr);
        if (turns > 0) {
            int32_t chance = COLCALC::Chance_For_Research_Breakthrough_(plr);
            if (chance > 0) {
                _sprintf(buffer, s__d___0055612c, (int)chance);
                MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
                Set_Post_Fitted_Font_Style_(14, 23);
                fonts::Set_Outline_Color_(2, 1);
                fonts::Print_Centered_(579, (int16_t)(403 - 2 * fonts::Get_Font_Height_()), buffer);
            }
            const char* turns_msg = HAROLD::H_Message_(turns == 1 ? 0xe1 : 0xe2);
            _sprintf(buffer, turns_msg, (int)turns);
            char at_buffer[64];
            _sprintf(at_buffer, s___s_0055994c, buffer);
            MISC::Set_Fitted_Font_Style_(at_buffer, &colors2, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            int16_t fh = fonts::Get_Font_Height_();
            int16_t turns_y = (int16_t)(403 - fh);
            fonts::Set_Outline_Color_(2, 1);
            int16_t msg_w = fonts::Get_String_Width_(buffer);
            int16_t start_x = (int16_t)((62 - msg_w) / 2 + 548);
            fonts::Print_(start_x, turns_y + 3, s___00559948);

            const char* turns_msg2 = HAROLD::H_Message_(turns == 1 ? 0xe1 : 0xe2);
            _sprintf(buffer, turns_msg2, (int)turns);
            fonts::Print_((int16_t)(fonts::Get_String_Width_(s___00559948) + start_x), turns_y, buffer);

            char* res_comma = MISC::Get_Comma_String_(plr->research_produced);
            _sprintf(buffer, s__s__s_00558014, res_comma, research_unit);
            MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_Centered_(579, turns_y + fonts::Get_Font_Height_(), buffer);
        }
        else if (turns < 0) {
            _sprintf(buffer, s_0__s_00559940, research_unit);
            MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_Centered_(579, 403, buffer);
        }
        else {
            int32_t chance = COLCALC::Chance_For_Research_Breakthrough_(plr);
            _sprintf(buffer, s__d___0055612c, (int)chance);
            MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_Centered_(579, 391, buffer);

            _sprintf(buffer, s__d__s_005569f0, (int)plr->research_produced, research_unit);
            MISC::Set_Fitted_Font_Style_(buffer, &colors2, 2, 2, 4, 0x43, 1);
            Set_Post_Fitted_Font_Style_(14, 23);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Print_Centered_(579, (int16_t)(391 + fonts::Get_Font_Height_()), buffer);
        }
    }

    void __cdecl Main_Screen_() {
        int16_t exit_flag = 0;
        int16_t mouse_field_1 = 0;
        int16_t boxes_saved_flag = 0;
        uint8_t icon_selected_flag = 0;
        int16_t planet_idx = 0;
        int16_t mouse_field_2 = 0;
        int16_t sel = 0;
        int16_t m_planet_idx = 0;
        int16_t m_ship_idx = 0;
        int16_t ship_idx = 0;

        int16_t ship_ids[500];
        char text_buffer[256];
        char msg_buffer[512];

        bool is_main_palette = MAINSCR::Is_Main_Screen_Palette_();
        _moving_a_stack = 0;
        _track_time_flag = 1;
        fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_, 1);
        MAINSCR::Allocate_First_Screen_Seg_(5);
        SYS::Clear_System_Display_Loaded_Flag_();
        mouse::Clear_Mouse_XY_();
        EVANHELP::Set_Main_Game_Screen_Help_List_();
        MAINSCR::Clear_Twinkling_Star_Data_();
        MOX::_fleet_ship_scanned = -1;
        MOX::_galaxy_map_scanned_star = -1;
        fields::Set_Field_Outline_Color_(20);
        fields::Disable_Cancel_();
        _zoom_box_increment_value = 1;
        _officer_screen_return_screen = -1;
        MOX::_draw_system_display_without_button = 0;
        MOX::_skip_fields = 0;
        _main_screen_scanned_star = -1;

        if (MOX::_game_type != 1) {
            if (MOVEBOX::Moveable_Box_Selected_(2)) {
                if (HACCESS::Get_Stack_Start_From_Saved_Ship_Array_() == 0) {
                    MOVEBOX::Vanish_Fleet_Movement_Box_();
                }
                MOX::_ship_icon_player_id = -1;
            }
        }

        video::Set_Refresh_Stencil_();
        MOX::_draw_main_screen_to_back = 1;
        icon_selected_flag = 0;
        MOX::_zoom_count = HAROLD::Map_Scale_To_Zoom_Level_();
        exit_flag = 0;

        fields::Clear_Fields_();
        MAINSCR::Clear_Main_Screen_Fields_();
        MAINSCR::Add_Cheats_();

        if (!is_main_palette && MOX::_previous_screen == SCREEN_REPORTS) {
            if (MOVEBOX::Moveable_Box_Selected_(0) && MOX::_at_least_one_report) {
                SYS::_skip_draw_system_planets = 1;
            }
        }

        MAINSCR::Update_Star_Owners_();
        SHIPS::Build_Ship_Icons_();
        SHIPS::Set_Ship_Icon_XYs_();

        if (_clear_fleet_fields == 0) {
            if (FLEETPOP::Some_Icon_Selected_()) {
                FLEETPOP::Reload_Ship_Selection_Status_();
            }
        }
        else {
            MOX::_first_node_ptr_in_stack_ = -1;
        }

        if (MAINSCR2::_force_main_screen_fade == 0 &&
            (is_main_palette || (MAINSCR2::_main_screen_fade_in_flag == 0 && MOX::_at_least_one_tactical_combat == 0))) {

            if (MOVEBOX::Moveable_Box_Selected_(0)) {
                MOX::_system_star_id = -1;
                SYS::Clear_System_Display_Loaded_Flag_();
            }
            fonts::Load_Palette_(0, 0, 0xad);
            if (MOX::_previous_screen == 0x24) {
                video::Set_Page_Off_();
                MAINSCR::Draw_Mini_Main_Screen_();
                video::Copy_Off_To_Back_();
                RUSS::Mox_Sync_Update_();
                video::Set_Refresh_Stencil_();
            }
            MAINSCR::Add_Map_Fields_();
        }
        else {
            MAINSCR::Fade_Into_Main_Screen_();
            MOX::_draw_fleet_screen_to_back = 1;
        }

        _track_time_flag = 0;
        _clear_fleet_fields = 1;

        if (MOX::_given_chance_to_rename_home_star[MOX::_PLAYER_NUM] == 0) {
            if (MOX::_stardate == 35000) {
                fields::Deactivate_Help_List_();
                MAINSCR::Change_Home_Star_Name_();
                EVANHELP::Set_Main_Game_Screen_Help_List_();
            }
            MOX::_given_chance_to_rename_home_star[MOX::_PLAYER_NUM] = 1;
        }

        MAINSCR2::_main_screen_fade_in_flag = 1;
        MOX::_at_least_one_tactical_combat = 0;

        if (MOX::_game_type != 0) {
            int player_bit = 1 << MOX::_PLAYER_NUM;
            if (_warn_about_cheaters & player_bit) {
                _warn_about_cheaters &= ~player_bit;
                MAINSCR::Warn_About_Cheating_Players_();
            }
        }

        while (exit_flag == 0 && MOX::_global_esc == 0) {
            if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                if (MAINSCR::Check_For_Diplomacy_Request_() == 1) {
                    fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_, 1);
                    fields::Clear_Fields_();
                    MAINSCR::Clear_Main_Screen_Fields_();
                    MAINSCR::Add_Cheats_();
                    ERIC::Fast_Fade_Out_();
                    MAINSCR::Add_Map_Fields_();

                    if (MOVEBOX::Moveable_Box_Selected_(2)) {
                        _already_faded_into_main_screen = 0;
                        MOX::_reloaded_ship_palette = 0;
                    }
                    else if (MOVEBOX::Moveable_Box_Selected_(0)) {
                        SYS::Clear_System_Display_Loaded_Flag_();
                    }
                    MAINSCR::Fade_Into_Main_Screen_();
                }
            }

            _fleet_ship_selection_status = -1;
            timer::Mark_Time_();

            int16_t input = fields::Get_Input_();
            if (input < 0) {
                if (_merging_relocations != 0) {
                    _merging_relocations = 0;
                }
                if (MOX::_zoom_in == 1) {
                    MAINSCR::Exit_Zooming_Mode_(&boxes_saved_flag);
                }
            }

            if (RUSS::_all_ai_game != 0) {
                video::Set_Page_Off_();
                MAINSCR::Draw_Main_Screen_();
                RUSS::Mox_Sync_Update_();
                input = _next_button;
            }
            else {
                if (MOX::_player[MOX::_PLAYER_NUM].surrender_to != -1) {
                    video::Set_Page_Off_();
                    MAINSCR::Draw_Main_Screen_();
                    RUSS::Mox_Sync_Update_();
                    input = _next_button;
                }
            }

            int16_t mouse_input = fields::Scan_Input_();
            if (MOX::_main_screen_grid_field == input || (-MOX::_main_screen_grid_field == input)) {
                int mx = mouse::Pointer_X_();
                int my = mouse::Pointer_Y_();
                int box_mode = (input <= 0) + 1;

                icon_selected_flag = box_mode;

                if (MOVEBOX::Moveable_Box_Selected_(box_mode)) {
                    MAINSCR::Check_Ships_XY_(mx, my, icon_selected_flag);
                    if (MOX::_mouse_XY.ship_icon_idx == -1) {
                        MAINSCR::Check_Stars_XY_(mx, my, icon_selected_flag);
                    }
                }
                else {
                    MAINSCR::Check_Stars_XY_(mx, my, icon_selected_flag);
                    if (MOX::_mouse_XY.star_idx == -1) {
                        MAINSCR::Check_Ships_XY_(mx, my, icon_selected_flag);
                    }
                }
            }
            else {
                MOX::_mouse_XY.ship_icon_idx = -1;
                MOX::_mouse_XY.star_idx = -1;
            }

            bool process_star_eta = false;

            if (MOX::_skip_fields == 0) {
                icon_selected_flag = 0;

                int16_t mouse_status = MAINSCR::Get_Mouse_Field_(0, &mouse_field_1, &mouse_field_2);
                _main_screen_scanned_star = mouse_field_1;

                if ((mouse_status == 0 || mouse_status == 4) && MOVEBOX::Moveable_Box_Selected_(2)) {
                    icon_selected_flag = FLEETPOP::Some_Icon_Selected_();
                }

                if (mouse_status == 0 && MOX::_star[mouse_field_2].spectral_class != STAR_CLASS_BLACK_HOLE && _merging_relocations != 0) {
                    _merging_relocations = 0;
                    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                    HACCESS::Set_All_Star_Relocations_(mouse_field_2, MOX::_PLAYER_NUM);

                    const char* msg = HAROLD::H_Message_(HESTR_0DD_ALL_NEW_SHIPS_BUILT_WI);
                    _sprintf(text_buffer, msg, &MOX::_star[mouse_field_2]);
                    HAROLD::User_Box_((uint8_t*)MOX::_draw_main_screen_to_back, text_buffer, 0);
                }
                else {
                    if (mouse_status == 0 && mouse_field_2 >= 0) {
                        if (!icon_selected_flag) {
                            int16_t colony_idx = -1;
                            if (MOX::_settings.auto_select_colony && MOX::_star[mouse_field_2].spectral_class != STAR_CLASS_BLACK_HOLE) {
                                colony_idx = HACCESS::Last_Colony_Selected_(mouse_field_2, MOX::_PLAYER_NUM);
                            }
                            if (colony_idx == -1) {
                                MAINSCR::Do_System_Popup_(mouse_field_2);
                            }
                            else {
                                MAINSCR::Do_Colony_Screen_(colony_idx, &exit_flag);
                            }
                        }
                        else {
                            process_star_eta = true;
                        }
                    }
                    else {
                        process_star_eta = true;
                    }
                }

                // Icon selected processing
                if (icon_selected_flag) {
                    process_star_eta = true;
                }

                if (process_star_eta) {
                    bool valid_target = false;
                    if (mouse_field_2 < 0 || mouse_status != 0 || MOX::_star[mouse_field_2].spectral_class == STAR_CLASS_BLACK_HOLE) {
                        if (icon_selected_flag && mouse_status != 0 && MOVEBOX::Moveable_Box_Selected_(2)) {
                            MOX::_fleet_ship_scanned = -1;
                            if (MOX::_star[mouse_field_1].spectral_class == STAR_CLASS_BLACK_HOLE) {
                                MOX::_eta_star_id = -1;
                            }
                            else {
                                MOX::_eta_star_id = mouse_field_1;
                            }
                        }
                    }
                    else {
                        valid_target = true;
                    }

                    if (valid_target) {
                        HACCESS::Get_Fleet_Box_Selected_Ship_Ids_((int16_t*)ship_ids, 500, 0xff);
                        SHIPMOVE::Ships_Try_To_Move_To_(ship_ids, mouse_field_2, &SHIPMOVE::_g_ship_move_info);

                        if (SHIPMOVE::_g_ship_move_info.moving == 0) {
                            int msg_id = 0;
                            if (SHIPMOVE::_g_ship_move_info.blackhole_blocks != 0) msg_id = HESTR_021_A_BLACK_HOLE_BLOCKS_MO;
                            else if (SHIPMOVE::_g_ship_move_info.insufficient_fuel != 0) msg_id = HESTR_022_ONE_OR_MORE_SHIPS_DOES;
                            else if (SHIPMOVE::_g_ship_move_info.immobile != 0) msg_id = HESTR_023_ONE_OR_MORE_SHIPS_CHOS;
                            else if (SHIPMOVE::_g_ship_move_info.hyperspace_flux != 0) msg_id = HESTR_024_SHIP_MOVEMENT_IS_NOT_A;

                            if (msg_id != 0) {
                                const char* msg = HAROLD::H_Message_(msg_id);
                                strcpy(msg_buffer, msg);
                                HAROLD::User_Box_((uint8_t*)MOX::_draw_main_screen_to_back, msg_buffer, 3);
                            }
                        }
                        else {
                            int ships_count = FLEETPOP::Fleet_Buttons_Chosen_();
                            SHIPMOVE::Make_Ships_Move_To_(ship_ids, mouse_field_2, &SHIPMOVE::_g_ship_move_info);
                            MOX::_eta_star_id = -1;

                            int16_t next_icon = 0;
                            if (ships_count == MOX::_box_off_flag_) {
                                MOX::_just_moved_stack = 1;
                            }
                            else {
                                next_icon = FLEETPOP::First_Unselected_Icon_();
                            }

                            SHIPS::Build_Ship_Icons_();
                            SHIPS::Set_Ship_Icon_XYs_();

                            int16_t stack_ship = next_icon;
                            if (ships_count == MOX::_box_off_flag_) {
                                stack_ship = 0; // Use dummy or 0
                            }

                            int16_t ship_id = HACCESS::Fleet_Movement_Ship_Id_(stack_ship);
                            MOX::_fleet_box_ship_stack = HACCESS::Ship_Stack_From_Ship_Id_(ship_id);

                            FLEETPOP::Set_Fleet_Movement_Icon_Selections_(0, 0);
                            HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();

                            if (MOX::_fleet_box_ship_stack == -1) {
                                MOVEBOX::Vanish_Fleet_Movement_Box_();
                            }

                            MOX::_draw_main_screen_to_back = 1;
                            MOX::_first_node_ptr_in_stack_ = -1;
                        }
                    }
                }
            }

            if (MOVEBOX::Moveable_Box_Selected_(0) && MOX::_skip_fields == 0) {
                if (MOX::_system_star_id > -1 && MOX::_system_star_id < MOX::_NUM_STARS && input != MOX::_moveable_box[0].field8_0x10) {
                    int mx = mouse::Pointer_X_();
                    int my = mouse::Pointer_Y_();

                    int res = MAINSCR::Scan_System_Display_Fields_(mouse_input, input, (int16_t*)MOX::_planet_buttons, &m_planet_idx, &planet_idx, &m_ship_idx, &ship_idx, &sel);

                    MOX::_system_display_ship_scanned = m_ship_idx;
                    if (sel == -1 && MOX::_system_icon_scanned > 2000 && MOX::_system_icon_scanned < 2004) {
                        MOX::_system_icon_scanned = -1;
                    }
                    else if (sel > -1) {
                        MOX::_system_icon_scanned = sel;
                    }

                    // Boundary check against moveable box
                    if (mx < MOX::_moveable_box[0].x || mx >= MOX::_moveable_box[0].x + MOX::_moveable_box[0].width ||
                        my < MOX::_moveable_box[0].y || my >= MOX::_moveable_box[0].y + MOX::_moveable_box[0].height) {
                        MOX::_system_icon_scanned = -1;
                    }

                    if (res == 0) {
                        if (ship_idx < 0) {
                            int16_t p_idx = HAROLD::Planet_Index_(MOX::_moveable_box[0].off_flag, planet_idx);

                            int16_t colony_id = MOX::_planet[p_idx].colony_index;
                            if (colony_id > -1 && colony_id < MOX::_NUM_COLONIES && MOX::_colony[colony_id].owner == MOX::_PLAYER_NUM) {
                                MAINSCR::Do_Colony_Screen_(colony_id, &exit_flag);
                            }
                        }
                        else {
                            int16_t ship_icon = HACCESS::Ship_Icon_Id_From_Node_Ptr_(ship_idx);
                            MAINSCR::Do_Fleet_Popup_(ship_icon, 0);
                        }
                    }
                }
            }

            MAINSCR::Scan_Moveable_Box_Fields(MOX::_skip_fields, input, mouse_input, (uint16_t*)&exit_flag);

            if (MOX::_skip_fields == 0) {
                int field_res = MAINSCR::Get_Mouse_Field_(1, &mouse_field_1, &mouse_field_2);
                _main_screen_ship_icon_scanned = mouse_field_1;
                if (mouse_field_1 != -1) {
                    MOX::_eta_star_id = -1;
                }
                if (field_res == 0) {
                    MAINSCR::Do_Fleet_Popup_(mouse_field_2, 1);
                }

                MAINSCR::Check_Function_Keys_(input, &exit_flag);
                MAINSCR::Check_Hot_Keys_(input, (int)&exit_flag);
            }

            if (input == _exit_game_hot_key && MOX::_skip_fields == 0) {
                const char* msg = HAROLD::H_Message_(0xde);
                exitm::Exit_With_Message(msg);
                // This will exit application
            }

            if (input == _game_button && MOX::_skip_fields == 0) {
                MOX::_current_screen = 8;
                exit_flag = 1;
                MOX::_return_screen = 0;
            }

            if (MOX::_skip_fields == 0 && MOX::_settings.active_save_slot >= 0 && MOX::_settings.active_save_slot < 10 && input == -0x455) {
                if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                    LOADSAVE::Auto_Save_Last_Saved_Game_(&exit_flag);
                }
                else {
                    const char* msg = HAROLD::H_Message_(0x20);
                    strcpy(msg_buffer, msg);
                    HAROLD::User_Box_((uint8_t*)MOX::_draw_main_screen_to_back, msg_buffer, 3);
                }
            }

            if (MOX::_skip_fields == 0 && MOX::_settings.active_save_slot >= 0 && MOX::_settings.active_save_slot < 10 && input == -0x456) {
                LOADSAVE::Auto_Load_Last_Saved_Game_(&exit_flag);
            }

            if (input == _fleets_button && MOX::_skip_fields == 0) {
                if (MOVEBOX::Moveable_Box_Selected_(2) || MOVEBOX::Moveable_Box_Selected_(0)) {
                    fields::Clear_Fields_();
                    video::Set_Page_Off_();
                    MAINSCR::Draw_Mini_Main_Screen_();
                    RUSS::Mox_Sync_Update_();
                }
                MOX::_return_screen = 0;
                MOX::_current_screen = SCREEN_FLEET;
                _officer_screen_return_screen = SCREEN_FLEET;
                exit_flag = 1;
            }

            if (input == _colonies_button && MOX::_skip_fields == 0) {
                MOX::_current_screen = SCREEN_COLONY_SUMMARY;
                exit_flag = 1;
                MOX::_return_screen = 0;
            }

            if (input == _info_button && MOX::_skip_fields == 0) {
                MOX::_current_screen = SCREEN_INFO;
                exit_flag = 1;
                MOX::_return_screen = 0;
            }

            if (input == _next_button && MOX::_skip_fields == 0) {
                RUSS::Assert_Settings_();
                MOX::_auto_turn_exit = 0;
                _next_turn_hit = 1;
                MOX::_global_exit_field = -1000;
                MAINSCR::Do_Next_Turn_(&exit_flag, 0);
            }

            if (input == _races_button && MOX::_skip_fields == 0) {
                MOX::_current_screen = 6;
                exit_flag = 1;
                MOX::_return_screen = 0;
            }

            if (input == _planets_button && MOX::_skip_fields == 0) {
                MOX::_current_screen = 0x20;
                exit_flag = 1;
                MOX::_return_screen = 0;
            }

            if (input == _leaders_button && MOX::_skip_fields == 0) {
                int16_t design_num;
                if (MOX::_officer_scrn_type == 0) {
                    if (MOVEBOX::Moveable_Box_Selected_(2)) {
                        design_num = MOX::_fleet_box_ship_stack;
                    }
                    else {
                        design_num = MAINSCR::Get_Ship_Stack_For_Officers_Screen_();
                    }
                }
                else {
                    design_num = MAINSCR::Get_Star_Id_For_Officers_Screen_();
                }
                DESIGN::_player_design_num = design_num;
                _officer_screen_return_screen = 0;
                MOX::_current_screen = SCREEN_OFFICERS;
                exit_flag = 1;
                MOX::_return_screen = 0;
            }

            if (input == _research_window_field && MOX::_skip_fields == 0) {
                MOX::_dont_draw_main_screen_to_back = 1;
                fields::Clear_Fields_();
                MAINSCR::Clear_Main_Screen_Fields_();
                video::Set_Page_Off_();
                MAINSCR::Draw_Mini_Main_Screen_();
                video::Copy_Off_To_Back_();
                RUSS::Mox_Sync_Update_();
                video::Set_Refresh_Stencil_();
                if (MOVEBOX::Moveable_Box_Selected_(2)) {
                    MAINSCR2::_force_main_screen_fade = 1;
                }
                MOX::_return_screen = 0;
                _clear_fleet_fields = 0;
                MOX::_current_screen = 0x24;
                exit_flag = 1;
            }
            else if (input == _freighters_window_field) {
                if (MOX::_player[MOX::_PLAYER_NUM].n_freighters < 1) {
                    const char* msg = HAROLD::H_Message_(HESTR_0DF_YOU_DO_NOT_HAVE_ANY_FR);
                    strcpy(msg_buffer, msg);
                    HAROLD::User_Box_((uint8_t*)&MOX::_draw_main_screen_to_back, msg_buffer, 3);
                    exit_flag = 1;
                }
                else {
                    MOX::_return_screen = 0;
                    _clear_fleet_fields = 0;
                    MOX::_current_screen = HESTR_023_ONE_OR_MORE_SHIPS_CHOS;
                    exit_flag = 1;
                }
            }
            else if (input == _command_points_window_field && MOX::_skip_fields == 0) {
                MOX::_return_screen = 0;
                _clear_fleet_fields = 0;
                MOX::_current_screen = HESTR_02B_COST;
                exit_flag = 1;
            }
            else if (input == _treasury_window_field) {
                input = -1;
                fields::Clear_Fields_();
                MAINSCR::Clear_Main_Screen_Fields_();
                fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
                video::Set_Page_Off_();
                MAINSCR::Draw_Mini_Main_Screen_();
                video::Copy_Off_To_Back_();
                video::Set_Refresh_Stencil_();
                RUSS::Mox_Sync_Update_();
                MAINSCR::Tax_Rate_Popup_();
                COLCALC::Do_Player_Colony_Calculations_(MOX::_PLAYER_NUM);
                MOX::_current_screen = 0;
                exit_flag = 1;
                _clear_fleet_fields = 0;
            }

            // Zoom Logic
            if ((input == _zoom_out_button && MOX::_cur_map_scale < MOX::_max_map_scale) ||
                (input == _zoom_in_button && MOX::_cur_map_scale > 10)) {
                Clear_Twinkling_Star_Data_();
                fields::Deactivate_Help_List_();
            }

            if (input == _zoom_in_button && MOX::_cur_map_scale > 10 && MOX::_skip_fields == 0) {
                if (boxes_saved_flag == 0) {
                    MOVEBOX::Save_And_Remove_Boxes_();
                }
                boxes_saved_flag += 1;

                if (MOX::_zoom_in == 0) {
                    fields::Deactivate_Help_List_();
                    MOX::_zoom_in = 1;
                    palette::Set_Palette_Changes_(0, 0xff);
                    remap::Update_Glass_Remap_Colors_();
                    video::Set_Refresh_Full_();
                    video::Set_Page_Off_();
                    MAINSCR::Draw_Mini_Main_Screen_();
                    allocate::Push_Block_(MOX::_screen_seg);
                    uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(0x1fb, 0x191);
                    _temp_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);
                    bitmap::Screen_Bitmap_Capture_(0x16, 0x16, 0x20f, 0x1a5, _temp_bitmap);
                    allocate::Pop_Block_(MOX::_screen_seg);
                    MISC::_Darken_Fill_(0x16, 0x16, 0x20f, 0x1a5);
                    video::Copy_Off_To_Back_();
                    video::Set_Refresh_Stencil_();
                    fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_For_Zooming_, 1);
                }

                MOX::_zoom_in = 1;
                fields::Clear_Fields_Above_(_last_field);
                MAINSCR::Add_Map_Fields_();
                mouse::Set_Mouse_List_(MOX::_mouse_list_zoom_mode, 5);

                HAROLD::Decrement_Zoom_Count_();

                MOX::_draw_main_screen_to_back = 1;
            }
            else
                if (input == _zoom_out_button && MOX::_zoom_in == 1 && MOX::_skip_fields == 0) {
                    if (HAROLD::Increment_Zoom_Count_With_Limit_() == 0) {
                        MAINSCR::Exit_Zooming_Mode_(&boxes_saved_flag);
                    }
                }
                else
                    if (input == _zoom_out_button && MOX::_cur_map_scale < MOX::_max_map_scale && MOX::_skip_fields == 0) {
                        int old_x = MOX::_cur_map_x;
                        int old_y = MOX::_cur_map_y;
                        int z1 = HAROLD::Map_Scale_To_Zoom_Level_();
                        HAROLD::Increment_Zoom_Count_();
                        MOX::_cur_map_scale = HAROLD::Get_Map_Scale_(MOX::_zoom_count);
                        int z2 = HAROLD::Map_Scale_To_Zoom_Level_();

                        // Centering logic
                        int diff_w = (z1 - z2) * 0x1fa;
                        MOX::_cur_map_x += (diff_w + ((diff_w < 0) ? 3 : 0)) / 4;

                        int diff_h = (z1 - z2) * 400;
                        MOX::_cur_map_y += (diff_h + ((diff_h < 0) ? 3 : 0)) / 4;

                        // Boundaries
                        int scale_diff = MOX::_max_map_scale - MOX::_cur_map_scale;
                        int max_x = (scale_diff * 0x1fa) / 10;
                        int max_y = (scale_diff * 400) / 10;

                        if (max_x < MOX::_cur_map_x) MOX::_cur_map_x = max_x;
                        if (max_y < MOX::_cur_map_y) MOX::_cur_map_y = max_y;
                        if (MOX::_cur_map_x < 0) MOX::_cur_map_x = 0;
                        if (MOX::_cur_map_y < 0) MOX::_cur_map_y = 0;
                        if (MOX::_max_map_scale == MOX::_cur_map_scale) {
                            MOX::_cur_map_x = 0;
                            MOX::_cur_map_y = 0;
                        }

                        int limit_x = HAROLD::Get_Up_Scaled_Value_(MOX::_MAP_MAX_X - 0x280);
                        if (limit_x < MOX::_cur_map_x) {
                            MOX::_cur_map_x = HAROLD::Get_Up_Scaled_Value_(MOX::_MAP_MAX_X - 0x280);
                        }

                        int limit_y = MOX::_MAP_MAX_Y - HAROLD::Get_Up_Scaled_Value_(400);
                        if (limit_y < MOX::_cur_map_y) {
                            MOX::_cur_map_y = limit_y;
                        }

                        MOX::_draw_main_screen_to_back = 1;
                        MAINSCR::Get_New_Paralax_XYs_(MOX::_cur_map_x - old_x, MOX::_cur_map_y - old_y, 1);
                    }
                    else
                        if (input == _whole_screen_field && MOX::_zoom_in == 1 && MOX::_skip_fields == 0) {
                            MOVEBOX::Restore_Boxes_();
                            boxes_saved_flag = 0;
                            MOX::_zoom_in = 0;
                            EVANHELP::Set_Main_Game_Screen_Help_List_();
                            mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                            fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_, 1);

                            MOX::_cur_map_x += HAROLD::Get_Up_Scaled_Value_(_zoom_box_x - 0x15);
                            MOX::_cur_map_y += HAROLD::Get_Up_Scaled_Value_(_zoom_box_y - 0x15);
                            MOX::_cur_map_scale = HAROLD::Get_Map_Scale_(MOX::_zoom_count);

                            MOX::_far_star_x = 0x16; MOX::_mid_star_x = 0x16; MOX::_near_star_x = 0x16;
                            MOX::_far_star_y = 0x16; MOX::_mid_star_y = 0x16; MOX::_near_star_y = 0x16;
                            MOX::_draw_main_screen_to_back = 1;
                        }

            // Scroll map logic
            // TODO: Why this is -main_screen_grid_field?
            if (-MOX::_main_screen_grid_field == input && MOX::_skip_fields == 0 && MOX::_max_map_scale != MOX::_cur_map_scale) {
                int target_y = HAROLD::Get_Up_Scaled_Value_(MOX::_main_screen_grid_y) + MOX::_cur_map_y;
                int target_x = HAROLD::Get_Up_Scaled_Value_(MOX::_main_screen_grid_x) + MOX::_cur_map_x;
                MAINSCR::Scroll_Map_(target_x, target_y);
            }

            // Redraw cycle
            if (exit_flag == 0 && MOX::_global_esc == 0) {
                if (MOX::_skip_fields == 0) {
                    fields::Clear_Fields_Above_(_last_field);
                    fields::Enable_Draw_All_Fields_();
                    fields::Enable_Mouse_Pressed_Field_Update_();
                }
                else {
                    MOX::_skip_fields = 0;
                    fields::Disable_Draw_All_Fields_();
                    fields::Clear_Fields_();
                    MAINSCR::Clear_Main_Screen_Fields_();
                    MAINSCR::Add_Cheats_();
                }
                MAINSCR::Add_Map_Fields_();

                if (MOX::_zoom_in == 1) {
                    video::Set_Page_Off_();
                    MAINSCR::Draw_Main_Screen_For_Zooming_();
                }
                else {
                    video::Set_Page_Off_();
                    MAINSCR::Draw_Main_Screen_();
                }
                RUSS::Mox_Sync_Update_();
            }

            // Mouse click logic
            if (MOVEBOX::Moveable_Box_Selected_(2)) {
                if (_mouse_btn_pressed != 0) {
                    if (mouse::Mouse_Button_() == 0) {
                        _mouse_btn_pressed = 0;
                        fields::Set_Input_Delay_(2);
                    }
                }
                FLEETPOP::Save_Ship_Selection_Status_();
                if (!FLEETPOP::Some_Icon_Selected_()) {
                    MOX::_eta_star_id = -1;
                }
            }

            timer::Release_Time_(1);
        }

        // Cleanup
        MAINSCR::Set_Reload_Fleet_Box_Palette_Flag_();
        fields::Deactivate_Auto_Function_();
        MOX::_first_fleet_movement_icon = 0;
        if (_clear_fleet_fields != 0) {
            MOX::_first_node_ptr_in_stack_ = -1;
        }

        MOX::_eta_star_id = -1;
        MOX::_stencil_box_id = -1;
        MOX::_draw_main_screen_stencilled = 0;

        SYS::Clear_System_Display_Loaded_Flag_();
        MOX::_animated_star = -1;
        fields::Deactivate_Help_List_();
        _already_faded_into_main_screen = 0;
        MOX::_reloaded_ship_palette = 0;
    }


    void __cdecl Draw_Main_Screen_() {
        if (MOX::_stencil_box_id >= 0 && MOX::_stencil_box_id <= 4) {
            Draw_Main_Screen_While_Moving_Boxes_();
            video::Clip_Off_();
            graphics::Reset_Window_();
        }
        else {
            graphics::Set_Window_(0x16, 0x16, 0x20F, 0x1A5);
            video::Clip_On_();

            if (MOX::_draw_main_screen_to_back == 1 || MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                Draw_Main_Screen_Filled_(1);
                MOX::_draw_main_screen_to_back = 0;
            }
            else {
                Draw_Wormhole_Links_();
                Draw_Relocation_Links_();
                Print_Star_Names_();
            }

            graphics::Set_Window_(0x16, 0x16, 0x20F, 0x1A5);
            video::Clip_On_();

            if (MOX::_settings.end_of_turn_wait != 0) {
                s_colors colors = { {0, 10, 12, 14, 16, 18, 20, 22} };
                char text_buffer[80];
                int32_t stardate = (int32_t)MOX::_stardate;
                const char* format = HAROLD::H_Message_(0xDA);

                _sprintf(text_buffer, format, stardate / 10, stardate % 10);

                fonts::Set_Font_Style_(1, &colors);
                fonts::Print_Right_(0x27F, 0, text_buffer);
            }

            Draw_Black_Holes_();
            Draw_Gate_Icons_();
            SHIPS::Do_Ship_Destination_Lines_();

            if ((int16_t)MOX::_eta_star_id > -1 && (int16_t)MOX::_fleet_ship_scanned == -1) {
                Draw_ETA_Destination_Line_();
                Highlight_Star_((int16_t)MOX::_eta_star_id);
            }

            if (_main_screen_scanned_star != -1) {
                Highlight_Star_(_main_screen_scanned_star);
            }

            Draw_Stars_();
            Draw_Ship_Icons_(false);

            if (Mouse_Button_() == 1) {
                if (MOVEBOX::Moveable_Box_Selected_(2)) {
                    if (FLEETPOP::Can_Select_Fleet_Box_Ship_(0) && MOVEBOX::Mouse_Within_Fleet_Box_()) {
                        int16_t scanned_field = fields::Scan_Input_();
                        if (scanned_field != MOX::_moveable_box[2].field) {
                            if (_mouse_btn_pressed == 0) {
                                _fltpop_field_selection_status = (FLEETPOP::Icon_Selection_Status_(scanned_field) == 0);
                            }
                            Set_Painted_Fleet_Fields_(scanned_field);
                        }
                        _mouse_btn_pressed = 1;
                    }
                }
            }

            for (int16_t i = 4; i >= 0; --i) {
                int16_t box_id = MOX::_moveable_box_order[i];
                if (MOVEBOX::Moveable_Box_Selected_(box_id)) {
                    if (box_id == 0) {
                        Highlight_Star_(MOX::_moveable_box[0].off_flag);
                    }
                    if (box_id == 2) {
                        Draw_Fleet_Box_Selected_Ship_Seg_();
                    }
                }
            }

            if (_main_screen_ship_icon_scanned > -1 && _main_screen_ship_icon_scanned < MOX::_ship_icon_count) {
                Draw_Scanned_Ship_Seg_();
            }

            for (int16_t i = 4; i >= 0; --i) {
                int16_t box_id = MOX::_moveable_box_order[i];
                if (MOVEBOX::Moveable_Box_Selected_(box_id) && box_id != (int16_t)MOX::_stencil_box_id) {
                    int16_t btn = Mouse_Button_();
                    if (btn != 0) {
                        int16_t target_field = MOX::_moveable_box[box_id].field;
                        if (target_field == fields::Auto_Input_() || MOX::_moveable_box[box_id].active == 1) {
                            MOVEBOX::Move_Moveable_Box_(box_id, btn);
                        }
                    }
                    MOVEBOX::Draw_Moveable_Box_(box_id);
                }
            }

            if ((int16_t)MOX::_galaxy_map_scanned_star > -1 && (int16_t)MOX::_galaxy_map_scanned_star < MOX::_NUM_STARS) {
                Print_Galaxy_Map_Scanned_Star_();
            }

            video::Clip_Off_();
            graphics::Reset_Window_();
            Print_Main_Screen_Data_();

            int32_t next_idx = (int32_t)MOX::_multi_colored_line_start + 1;
            MOX::_multi_colored_line_start = (int16_t)(next_idx % 8);
        }

        if (MOX::_game_type == 2 || MOX::_game_type == 3) {
            Draw_Diplomacy_Request_Lights_();
        }
        if (MOX::_game_type == 1 || MOX::_game_type == 2 || MOX::_game_type == 3) {
            Draw_Player_Done_Lights_();
        }

        Check_Mouse_XY_Fields_();

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MOX::_skip_fields = 1;
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        MOX::_red_color_pulse_value += MOX::_red_color_pulse_incr;
        if (MOX::_red_color_pulse_value == 0 || MOX::_red_color_pulse_value == 8) {
            MOX::_red_color_pulse_incr = -MOX::_red_color_pulse_incr;
        }
    }
}