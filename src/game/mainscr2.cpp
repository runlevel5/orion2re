#include "pch.h"

namespace MAINSCR2 {
    s_mouse_list _mouse_list_from[5] = {
        {17, 0, 0, 0, 639, 479},
        {1, 0, 0, 21, 0, 479},
        {1, 0, 0, 0, 639, 21},
        {1, 0, 527, 0, 639, 479},
        {1, 0, 11, 434, 617, 479},
    };
    s_mouse_list _mouse_list_to[5] = {
        {18, 0, 0, 0, 639, 479},
        {1, 0, 0, 21, 0, 479},
        {1, 0, 0, 0, 639, 21},
        {1, 0, 527, 0, 639, 479},
        {1, 0, 11, 434, 617, 479},
    };
    uint8_t _main_screen_fade_in_flag;
    uint8_t _force_main_screen_fade;
    uint8_t _in_reports_phase;

    void Show_Command_Points_Screen_() {
        fields::Deactivate_Help_List_();
        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
        fields::Clear_Fields_();

        // Screen bounds are 640x480 (0..639, 0..479)
        fields::Add_Hidden_Field_(0, 0, 639, 479, "\x1B", 41);

        fields::Disable_Draw_All_Fields_();
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);

        MAINSCR::Draw_Mini_Main_Screen_();
        RUSS::Mox_Sync_Update_();
        fields::Enable_Draw_All_Fields_();

        COLCALC::Show_Command_Points_((int)MOX::_PLAYER_NUM);

        MOX::_current_screen = MOX::_return_screen;
    }

    void __cdecl Draw_Main_Screen_For_Star_Distan() {
        int16_t star_idx = MOX::_screen_mouse_list_type;
        int16_t previous_temp_star_handle = MOX::_temp_star_handle;
        s_colors distance_outline_colors = {{0x70, 0x72, 0x71, 0x72, 0x72, 0x72, 0x72, 0x72}};
        s_colors blocked_path_outline_colors = {{0x4C, 0x50, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50}};

        MAINSCR::Draw_Mini_Main_Screen_();
        MAINSCR::Draw_Main_Main_Screen_();
        graphics::Set_Window_(22, 22, 527, 421);
        video::Clip_On_();

        if (star_idx > -1) {
            MAINSCR::Highlight_Star_(star_idx);
            bool found = false;

            if (MOX::_NUM_STARS > 0) {
                for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                    if (found) {
                        break;
                    }

                    if (MOX::_star_button[i] == MOX::_scanned_field && i != star_idx) {
                        found = true;
                        MOX::_temp_star_handle = i;
                    }

                    if (MOX::_temp_star_handle > -1 && MOX::_temp_star_handle != star_idx) {
                        if (previous_temp_star_handle == MOX::_temp_star_handle) {
                            int16_t x1, y1;
                            int16_t x2, y2;

                            MAINSCR::Get_Star_Draw_Coords_(star_idx, &x1, &y1);
                            MAINSCR::Get_Star_Draw_Coords_(MOX::_temp_star_handle, &x2, &y2);

                            int32_t distance = GEO::Parsecs_Between_Stars_(star_idx, MOX::_temp_star_handle);

                            char distance_text[80];
                            if (struct_::Test_Bit_Field_(MOX::_star[star_idx].black_hole_blocks, MOX::_temp_star_handle) != 0) {
                                fonts::Set_Outline_Color_(0, 0);
                                fonts::Set_Font_Style_Outline_(2, &blocked_path_outline_colors);
                                strlcpy(distance_text, HAROLD::H_Message_(234), sizeof(distance_text));
                                MISC::Change_Font_Style_(1);
                            } else {
                                fonts::Set_Outline_Color_(0, 0);
                                fonts::Set_Font_Style_Outline_(2, &distance_outline_colors);
                                snprintf(distance_text, sizeof(distance_text), HAROLD::H_Message_(235), distance);
                            }

                            int16_t width = fonts::Get_String_Width_(distance_text);
                            if (x2 + width + 10 <= 522) {
                                fonts::Print_(x2 + 10, y2, distance_text);
                            } else {
                                fonts::Print_Right_(522, y2, distance_text);
                            }

                            SHIPS::Draw_Directional_Multi_Colored_Line_(
                                x1, y1, x2, y2,
                                &MAINSCR::_ship_direction_line_green_colors,
                                MOX::_multi_colored_line_start
                            );
                        }
                    }
                }
            }
        }

        MOX::_multi_colored_line_start = (MOX::_multi_colored_line_start + 1) % 8;

        video::Clip_Off_();
        graphics::Reset_Window_();
    }

    void __cdecl Reports_Screen_() {
        int8_t report_mode;
        bool should_fade = false;

        MOX::_current_screen = SCREEN_MAIN;
        report_mode = REPORT::Must_Display_Reports_(MOX::_PLAYER_NUM);

        if (report_mode != 0) {
            mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        }

        video::Set_Refresh_Stencil_();
        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);

        if (MOX::_stardate_at_beg_of_turn == 0) {
            if (report_mode == 2 || report_mode == 3 || report_mode == 5 || report_mode == 6 ||
                (report_mode == 7 && MOX::_settings.show_gnn_report)) {
                should_fade = false;
            } else {
                should_fade = true;
            }
        } else {
            if (MOX::_previous_report == 3 && report_mode != 3) {
                should_fade = true;
            }
        }

        if (report_mode == 3) {
            if ((int8_t)MOX::_previous_screen != SCREEN_MAIN_MENU) {
                should_fade = false;
            } else if (MOX::_at_least_one_tactical_combat != 0 && report_mode != 0) {
                should_fade = true;
            }
        } else if (MOX::_at_least_one_tactical_combat != 0 && report_mode != 0) {
            should_fade = true;
        }

        if (should_fade && MAINSCR::_already_faded_into_main_screen == 0) {
            MOX::_at_least_one_tactical_combat = 0;
            MAINSCR::Allocate_First_Screen_Seg_(5);
            fields::Clear_Fields_();
            MAINSCR::Clear_Main_Screen_Fields_();
            MAINSCR::Update_Star_Owners_();
            SHIPS::Build_Ship_Icons_();
            ERIC::Fast_Fade_Out_();
            MAINSCR::Add_Map_Fields_();
            MAINSCR::Fade_Into_Main_Screen_();
        }

        MAINSCR2::Do_Begin_Of_Turn_();

        if (MOX::_current_screen == SCREEN_COLONY) {
            MOX::_return_screen = SCREEN_REPORTS;
        }

        if (MOX::_at_least_one_report != 0) {
            MAINSCR2::_main_screen_fade_in_flag = 1;
            SYS::Clear_System_Display_Loaded_Flag_();
        } else {
            MAINSCR2::_main_screen_fade_in_flag = 0;
        }

        if (MOVEBOX::Moveable_Box_Selected_(2) != 0 && MOX::_at_least_one_report != 0) {
            MAINSCR2::_force_main_screen_fade = 1;
            MOX::_reloaded_ship_palette = 1;
        }

        if (report_mode == 0) {
            MOX::_stardate_at_beg_of_turn = MOX::_stardate;
        }

        fields::Deactivate_Auto_Function_();
    }

    void __cdecl Distance_Between_Stars_Screen_() {
        fields::Clear_Fields_();
        MAINSCR::Clear_Main_Screen_Fields_();

        graphics::Set_Window_(22, 22, 527, 421);
        video::Clip_On_();
        MAINSCR::Add_Star_Fields_();
        video::Clip_Off_();
        graphics::Reset_Window_();

        int16_t hotkey = fields::Add_Hot_Key_("\x1B");
        int16_t starmap_field = fields::Add_Hidden_Field_(22, 22, 506, 400, "", 41);

        bool done = false;
        int16_t full_screen_field = fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0);

        fields::Assign_Auto_Function_(Draw_Main_Screen_For_Star_Distan, 2);

        MOX::_screen_mouse_list_type = -1;
        MOX::_temp_star_handle = -1;

        video::Set_Page_Off_();
        MAINSCR::Draw_Main_Screen_Filled_(0);
        palette::Set_Palette_Changes_(0, 255);
        remap::Update_Glass_Remap_Colors_();

        MISC::_Darken_Fill_(0, 0, 21, 479);
        MISC::_Darken_Fill_(0, 0, 639, 21);
        MISC::_Darken_Fill_(527, 0, 639, 479);
        MISC::_Darken_Fill_(21, 421, 527, 479);

        video::Copy_Off_To_Back_();

        do {
            timer::Mark_Time_();

            int16_t input = fields::Get_Input_();
            MOX::_scanned_field = fields::Scan_Input_();

            if (MOX::_screen_mouse_list_type == -1) {
                mouse::Set_Mouse_List_(_mouse_list_from, sizeof(_mouse_list_from) / sizeof(_mouse_list_from[0]));
            } else {
                mouse::Set_Mouse_List_(_mouse_list_to, sizeof(_mouse_list_to) / sizeof(_mouse_list_to[0]));
            }

            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                int16_t btn = MOX::_star_button[i];
                if (input == btn) {
                    MOX::_screen_mouse_list_type = i;
                } else if (input + btn == 0) {
                    done = true;
                }
            }

            if (input == full_screen_field ||
                input + full_screen_field == 0 ||
                input + starmap_field == 0 ||
                input == -1009 ||
                input == hotkey) {
                done = true;
            }

            if (!done) {
                video::Set_Page_Off_();
                Draw_Main_Screen_For_Star_Distan();
                RUSS::Mox_Sync_Update_();
            }

            timer::Release_Time_(1);
        } while (!done);

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        MOX::_current_screen = SCREEN_MAIN;
    }

    void __cdecl Scrap_Freighters_() {
        char message_text[200];

        int16_t selected_box = MOVEBOX::Moveable_Box_Selected_(2);
        int16_t scrap_amount = SETTLER::N_Freighters_Player_Can_Scrap_(MOX::_PLAYER_NUM);

        if (scrap_amount <= 0) {
            strlcpy(message_text, HAROLD::H_Message_(0xe6 /* HESTR_0E6_NO_SHIPS_SELECTED */), sizeof(message_text));
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, message_text, 3);
        } else {
            fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
            EVANHELP::Set_Scrap_Freighters_Help_List_();
            fields::Clear_Fields_();
            MOX::_moveable_box[2].id = 0;
            fields::Disable_Draw_All_Fields_();
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 0x27f, 0x1df, 0);
            MAINSCR::Draw_Mini_Main_Screen_();
            RUSS::Mox_Sync_Update_();
            fields::Enable_Draw_All_Fields_();

            int16_t count = MAINPUPS::Slider_Popup_(0, scrap_amount, 0, (char*)HAROLD::H_Message_(0xed), 0);
            if (count >= 0) {
                s_player* plr = &MOX::_player[MOX::_PLAYER_NUM];
                int16_t box_result = 1;

                if (plr->n_freighters <= count || plr->surplus_freighters <= count) {
                    const char* str1 = HAROLD::H_Message_((count == 1) ? 0xf0 /* HESTR_0F0_FREIGHTER */ : 0xf1 /* HESTR_0F1_FREIGHTERS */);
                    const char* str2 = HAROLD::H_Message_((plr->surplus_freighters == 1) ? 0xf0 : 0xf1);
                    snprintf(message_text, sizeof(message_text), HAROLD::H_Message_(0xee), plr->surplus_freighters, str2, count, str1);
                    box_result = HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, message_text, 1);
                }

                if (box_result != 0 && count > 0) {
                    const char* str3 = HAROLD::H_Message_((count == 1) ? 0xf0 : 0xf1);
                    snprintf(message_text, sizeof(message_text), HAROLD::H_Message_(0xef), count, str3);
                    HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, message_text, 0);

                    int16_t new_freighters = plr->n_freighters - count;
                    if (new_freighters <= 0) {
                        new_freighters = 0;
                    }
                    plr->n_freighters = new_freighters;

                    COLCALC::Do_Player_Colony_Calculations_(MOX::_PLAYER_NUM);
                }
            }
        }

        MOX::_current_screen = MOX::_return_screen;
        if (selected_box != 0) {
            MOX::_moveable_box[2].id = 1;
        }

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
    }

    void __cdecl Building_Outposts_In_Main_Screen_() {
        int8_t saved_expanding_help = MOX::_settings.expanding_help;
        bool skip_draw = false;

        int16_t star_id = SHIPSTAK::Ship_Stack_Star_Id_(MOX::_ship_stack_start[MOX::_fleet_box_ship_stack]);

        if (MOX::_settings.expanding_help != 0) {
            MOX::_settings.expanding_help = 0;
        }

        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
        fields::Clear_Fields_();
        MOVEBOX::Vanish_Fleet_Movement_Box_();
        fields::Disable_Draw_All_Fields_();
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 0x27f, 0x1df, 0);
        MAINSCR::Draw_Mini_Main_Screen_();
        RUSS::Mox_Sync_Update_();
        fields::Enable_Draw_All_Fields_();

        int16_t new_outpost_star_id = MAINPUPS::New_Outpost_Selection_Popup_(star_id);

        if (new_outpost_star_id > -1) {
            bool found_outpost_ship = false;
            int16_t outpost_ship_id = -1;

            if (MOX::_fleet_box.count > 0) {
                for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
                    int16_t ship_id = HACCESS::Fleet_Movement_Ship_Id_(i);

                    if (MOX::_ship[ship_id].d.ship_type == SHIP_TYPE_OUTPOST) {
                        found_outpost_ship = true;
                        outpost_ship_id = ship_id;
                        break;
                    }
                }
            }

            if (found_outpost_ship) {
                COLONIZE::Make_New_Outpost_(new_outpost_star_id, MOX::_PLAYER_NUM);
                MOX::_ship[outpost_ship_id].status = 5;

                if (MOX::_fleet_box.count == 1) {
                    skip_draw = true;
                }

                MOX::_first_node_ptr_in_stack_ = -1;
                MOX::_fleet_icon_buttons_added--;
            }
        }

        if (!skip_draw) {
            video::Set_Page_Off_();
            MAINSCR::Draw_Main_Screen_Filled_(1);
            MAINSCR::Draw_Main_Screen_();
            RUSS::Mox_Sync_Update_();
            KEN::Load_Player_Ship_Palette_(MOX::_PLAYER_NUM);

            MOX::_moveable_box[2].id = 1;

            SHIPS::Build_Ship_Icons_();
            MOX::_eta_star_id = -1;
            MOX::_fleet_box_ship_stack = HACCESS::Ship_Stack_From_Saved_Array_();

            if (MOX::_fleet_box_ship_stack == -1) {
                MOVEBOX::Vanish_Fleet_Movement_Box_();
            }
        }

        MOX::_current_screen = MOX::_return_screen;
        MOX::_settings.expanding_help = saved_expanding_help;

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        SYS::Clear_System_Display_Loaded_Flag_();
    }

    void __cdecl Planet_Colonization_In_Main_Screen_() {
        int8_t saved_expanding_help = MOX::_settings.expanding_help;
        bool do_animation = false;
        bool single_fleet_ship = false;

        int16_t star_idx = SHIPSTAK::Ship_Stack_Star_Id_(MOX::_ship_stack_start[MOX::_fleet_box_ship_stack]);
        namestar::Can_Change_Star_Name_(star_idx);

        if (MOX::_settings.expanding_help != 0) {
            MOX::_settings.expanding_help = 0;
        }

        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
        fields::Clear_Fields_();
        MOVEBOX::Vanish_Fleet_Movement_Box_();
        fields::Disable_Draw_All_Fields_();
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);
        MAINSCR::Draw_Mini_Main_Screen_();
        RUSS::Mox_Sync_Update_();
        fields::Enable_Draw_All_Fields_();

        int16_t planet_idx = MAINPUPS::New_Colony_Selection_Popup_(star_idx, 1);

        if (planet_idx > -1) {
            bool found_colony_ship = false;
            int16_t ship_idx = -1;

            if (MOX::_fleet_box.count > 0) {
                for (int16_t box_idx = 0; box_idx < MOX::_fleet_box.count; ++box_idx) {
                    int16_t current_ship_idx = HACCESS::Fleet_Movement_Ship_Id_(box_idx);
                    if (MOX::_ship[current_ship_idx].d.ship_type == SHIP_TYPE_COLONY) {
                        found_colony_ship = true;
                        ship_idx = current_ship_idx;
                        break;
                    }
                }
            }

            if (found_colony_ship) {
                if (MOX::_settings.animations_on != 0) {
                    do_animation = true;
                }

                COLONIZE::Make_New_Colony_(planet_idx, MOX::_PLAYER_NUM);
                MOX::_ship[ship_idx].status = 5;

                if (MOX::_fleet_box.count == 1) {
                    single_fleet_ship = true;
                }

                MOX::_first_node_ptr_in_stack_ = -1;
                MOX::_fleet_icon_buttons_added--;
            }
        }

        if (do_animation) {
            MOX::_screen_data = star_idx;
            COLONY::_orbit_temp = HACCESS::Planet_Orbit_(planet_idx);
            COLLAND::Colony_Landing_Screen_();
            MISC::Fade_Into_Screen_(MAINSCR::Draw_Mini_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
        }

        if (!single_fleet_ship) {
            video::Set_Page_Off_();
            MAINSCR::Draw_Main_Screen_Filled_(1);
            MAINSCR::Draw_Main_Screen_();
            RUSS::Mox_Sync_Update_();
            KEN::Load_Player_Ship_Palette_(MOX::_PLAYER_NUM);
            MOX::_moveable_box[2].id = 1;
            SHIPS::Build_Ship_Icons_();
            MOX::_fleet_box_ship_stack = HACCESS::Ship_Stack_From_Saved_Array_();

            if (MOX::_fleet_box_ship_stack == -1) {
                MOVEBOX::Vanish_Fleet_Movement_Box_();
            }
        }

        MOX::_current_screen = MOX::_return_screen;
        MOX::_settings.expanding_help = saved_expanding_help;
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        SYS::Clear_System_Display_Loaded_Flag_();
    }

    void __cdecl Begin_Of_Turn_(int8_t* reports_available) {
        bool is_main_palette = true;

        *reports_available = 0;
        MAINSCR2::Main_Screen_Report_Handler_(reports_available);

        if (MOX::_global_esc == 1) {
            return;
        }

        MAINSCR::Allocate_First_Screen_Seg_(5);

        MOX::_ship_icon_player_id = -1;
        MOX::_eta_star_id = -1;
        MOX::_icon_overlay_x = -1;
        MOX::_icon_overlay_y = -1;

        MAINSCR::Clear_Main_Screen_Fields_();
        MAINSCR::Update_Star_Owners_();
        SHIPS::Build_Ship_Icons_();

        if (MOX::_game_type != 1) {
            if (MOVEBOX::Box_Was_Open_(2) || MOVEBOX::Moveable_Box_Selected_(2) != 0) {
                MAINSCR2::Check_Fleet_Movement_Box_Status_();
            }
        }

        fields::Disable_Draw_All_Fields_();
        fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_, 2);

        if (reports_available == nullptr) {
            is_main_palette = MAINSCR::Is_Main_Screen_Palette_();
        }

        MOVEBOX::Box_Was_Open_(0);
        bool box_2_was_open = MOVEBOX::Box_Was_Open_(2);
        MOVEBOX::Restore_Boxes_();

        if (box_2_was_open || MOVEBOX::Moveable_Box_Selected_(2) != 0) {
            FLEETPOP::Reload_Ship_Selection_Status_();
        }

        if (reports_available != nullptr) {
            if (box_2_was_open) {
                MAINSCR2::Restore_Fleet_Box_Palette_();
                fields::Enable_Draw_All_Fields_();
                return;
            }
        } else {
            if (box_2_was_open) {
                MOX::_moveable_box[2].id = 1;
            }
            MAINSCR::Redraw_Main_Screen_(!is_main_palette);
        }

        fields::Enable_Draw_All_Fields_();
    }

    void __cdecl Do_Begin_Of_Turn_() {
        int8_t cancel_turn;
        int16_t next_turn_out;
        s_colors font_colors;
        char msg_buffer[80];

        _in_reports_phase = 1;
        MOX::_global_var = 0;

        Begin_Of_Turn_(&cancel_turn);

        MAINSCR::_officer_screen_return_screen = SCREEN_REPORTS;

        bool enemy_ship_coming = HACCESS::Enemy_Ship_Going_To_My_Colony_();

        if (MOX::_settings.end_of_turn_wait != 0 ||
            cancel_turn != 0 ||
            REPORT::_something_interesting_happened[MOX::_PLAYER_NUM] != 0 ||
            MAINSCR::_next_turn_hit == 0 ||
            MOX::_game_type != 0 ||
            enemy_ship_coming) {

            MAINSCR::_check_next_turn_exit_field = 0;
            mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
            MAINSCR::_next_turn_hit = 0;
            fields::Clear_Fields_();

        } else {
            MAINSCR::_check_next_turn_exit_field = 1;
            MOX::_animated_star = -1;

            fields::Disable_Draw_All_Fields_();
            fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
            video::Set_Page_Off_();
            MAINSCR::Draw_Mini_Main_Screen_();

            HAROLD::Get_Player_Font_Colors_(MOX::_PLAYER_NUM, 3, &font_colors, false);
            fonts::Set_Font_Style_(3, &font_colors);

            int32_t stardate_mod = MOX::_stardate % 10;
            int32_t stardate_div = MOX::_stardate / 10;
            char* race_name = MOX::_player[MOX::_PLAYER_NUM].race_name;
            char* format = (char*)HAROLD::H_Message_(0xE9);

            snprintf(msg_buffer, sizeof(msg_buffer), format, race_name, stardate_div, stardate_mod);

            int16_t str_width = fonts::Get_String_Width_(msg_buffer);
            int16_t font_height = fonts::Get_Font_Height_();

            int16_t box_w = str_width + 8;
            int16_t box_h = font_height + 4;

            int16_t box_x = (506 - box_w) / 2 + 21;
            int16_t box_y = (400 - box_h) / 2 + 21;

            palette::Set_Palette_Changes_(0, 0xFF);
            remap::Update_Glass_Remap_Colors_();

            MISC::Draw_Darkened_Box_(box_x, box_y, box_w, box_h, font_colors.colors[1]);
            fonts::Print_(box_x + 8, box_y + 6, msg_buffer);

            RUSS::Mox_Sync_Update_();
            fields::Enable_Draw_All_Fields_();

            MAINSCR::Do_Next_Turn_(&next_turn_out, 1);
        }

        _in_reports_phase = 0;
    }

    void __cdecl Unloading_Transports_In_Main_Screen_() {
        int8_t original_expanding_help = MOX::_settings.expanding_help;
        bool unloaded_all = false;

        int16_t star_id = SHIPSTAK::Ship_Stack_Star_Id_(MOX::_ship_stack_start[MOX::_fleet_box_ship_stack]);

        if (MOX::_settings.expanding_help != 0) {
            MOX::_settings.expanding_help = 0;
        }

        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
        fields::Clear_Fields_();
        MOVEBOX::Vanish_Fleet_Movement_Box_();
        fields::Disable_Draw_All_Fields_();
        video::Set_Page_Off_();

        graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);
        MAINSCR::Draw_Mini_Main_Screen_();
        RUSS::Mox_Sync_Update_();
        fields::Enable_Draw_All_Fields_();

        star_id = MAINPUPS::Drop_Troops_Selection_Popup_(star_id);

        if (star_id >= 0) {
            int16_t colony_index = MOX::_planet[star_id].colony_index;
            if (colony_index >= 0) {
                if (MOX::_colony[colony_index].owner == MOX::_PLAYER_NUM) {
                    int16_t ship_ids[MAX_SHIPS];
                    int32_t count = HACCESS::Get_Fleet_Box_Selected_Ship_Ids_(ship_ids, sizeof(ship_ids) / sizeof(ship_ids[0]), 2);
                    int16_t unloaded = invasion::Unload_Transports_(&MOX::_colony[colony_index], ship_ids, count);

                    if (MOX::_fleet_box.count == unloaded) {
                        unloaded_all = true;
                    }
                    MOX::_first_node_ptr_in_stack_ = -1;
                    MOX::_fleet_icon_buttons_added--;
                }
            }
        }

        if (!unloaded_all) {
            MAINSCR::Update_Ship_Icons_In_Between_Turns_();
            video::Set_Page_Off_();
            MAINSCR::Draw_Main_Screen_Filled_(1);
            MAINSCR::Draw_Main_Screen_();
            RUSS::Mox_Sync_Update_();
            KEN::Load_Player_Ship_Palette_(MOX::_PLAYER_NUM);
            MOX::_moveable_box[2].id = 1;

            MOX::_fleet_box_ship_stack = HACCESS::Ship_Stack_From_Saved_Array_();
            if (MOX::_fleet_box_ship_stack == -1) {
                MOVEBOX::Vanish_Fleet_Movement_Box_();
            }
        }

        MOX::_current_screen = MOX::_return_screen;
        MOX::_settings.expanding_help = original_expanding_help;
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        SYS::Clear_System_Display_Loaded_Flag_();
    }

    void __cdecl Redraw_Main_Screen_After_Popups_(uint8_t redraw) {
        if (redraw) {
            SYS::_skip_draw_system_planets = (int8_t)MOVEBOX::Moveable_Box_Selected_(0);
            MAINSCR::Fade_Into_Main_Screen_();
        }
    }

    void __cdecl Main_Screen_Report_Handler_(int8_t* reports_available) {
        uint8_t pending_reports;
        bool is_popup_report;

        pending_reports = REPORT::Must_Display_Reports_(MOX::_PLAYER_NUM);
        *reports_available = pending_reports;

        if (MOX::_stardate_at_beg_of_turn != 0 && MOX::_stardate != MOX::_stardate_at_beg_of_turn) {
            MAINSCR::_already_faded_into_main_screen = 0;
        }

        if (*reports_available == 0) {
            if (MOX::_at_least_one_tactical_combat != 0) {
                if (MOVEBOX::Moveable_Box_Selected_(2) != 0 || MOVEBOX::Moveable_Box_Selected_(0) != 0) {
                    MAINSCR2::_force_main_screen_fade = 1;
                }
                MOX::_at_least_one_tactical_combat = 0;
            }
            return;
        }

        MOX::_at_least_one_report = 1;
        MOX::_mainscr_palette = 0xff;
        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);

        if (MOX::_stardate != MOX::_stardate_at_beg_of_turn) {
            if (MAINSCR::Is_Main_Screen_Palette_() != 0) {
                video::Set_Page_Off_();
                MAINSCR::Draw_Mini_Main_Screen_();
                RUSS::Mox_Sync_Update_();
            } else {
                MAINSCR::Fade_Into_Main_Screen_();
            }
            MOX::_stardate_at_beg_of_turn = MOX::_stardate;
        }

        do {
            MAINSCR::_already_faded_into_main_screen = 0;
            SYS::Clear_System_Display_Ship_Fields_();
            SYS::Clear_System_Display_Loaded_Flag_();
            MAINSCR::Allocate_First_Screen_Seg_(5);

            MOX::_previous_report = REPORT::Display_Report_(MOX::_PLAYER_NUM);

            if (MOX::_previous_report == 2 ||
                MOX::_previous_report == 5 ||
                MOX::_previous_report == 6 ||
                (MOX::_previous_report == 7 && MOX::_settings.show_gnn_report != 0)) {
                is_popup_report = true;
            } else {
                is_popup_report = false;
            }

            if (MOX::_global_esc == 1) {
                return;
            }

            pending_reports = REPORT::Must_Display_Reports_(MOX::_PLAYER_NUM);

            if (is_popup_report && pending_reports != 3) {
                MAINSCR2::Redraw_Main_Screen_After_Popups_(1);
                MAINSCR::_already_faded_into_main_screen = 1;
            }

        } while (pending_reports != 0);

        fields::Assign_Auto_Function_(MAINSCR::Draw_Main_Screen_, 1);
    }

    void __cdecl Check_Fleet_Movement_Box_Status_() {
        MOX::_fleet_box_ship_stack = HACCESS::Ship_Stack_From_Saved_Array_();

        if (MOX::_fleet_box_ship_stack == -1) {
            MOVEBOX::Vanish_Fleet_Movement_Box_();
            MOX::_saved_boxes[2].is_open = 0;
            MOX::_saved_boxes[2].field_id = MOX::_box_off_flag[2];
        }
    }

    void __cdecl Restore_Fleet_Box_Palette_() {
        MOX::_fleet_box_ship_stack = HACCESS::Ship_Stack_From_Saved_Array_();

        if (MOX::_fleet_box_ship_stack == -1) {
            MOX::_moveable_box[2].id = MOX::_box_off_flag[2];
        } else {
            MOX::_moveable_box[2].id = 1;
            MAINSCR::Fade_Into_Main_Screen_();
            _force_main_screen_fade = 0;
        }
    }
}