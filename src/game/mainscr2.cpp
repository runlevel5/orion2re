#include "pch.h"

namespace MAINSCR2 {
    void Show_Command_Points_Screen_() {
        fields::Deactivate_Help_List_();
        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
        fields::Clear_Fields_();

        // Screen bounds are 640x480 (0..639, 0..479)
        fields::Add_Hidden_Field_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, s__00551120, 41 /* 0x29 */);

        fields::Disable_Draw_All_Fields_();
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);

        MAINSCR::Draw_Mini_Main_Screen_();
        RUSS::Mox_Sync_Update_();
        fields::Enable_Draw_All_Fields_();

        COLCALC::Show_Command_Points_(static_cast<int>(MOX::_PLAYER_NUM));

        MOX::_current_screen = MOX::_return_screen;
    }

    // Suggested name: MAINSCR2::Reports_Screen_
    void __cdecl Reports_Screen_() {
        int8_t report_mode;
        bool should_fade = false;

        MOX::_current_screen = 0;
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
            if ((int8_t)MOX::_previous_screen != 10) {
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

        if (MOX::_current_screen == 1) {
            MOX::_return_screen = 39 /* SCREEN_REPORTS */;
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

    void __cdecl Scrap_Freighters_(void) {
        char buffer[200];
        bool box_selected;
        int16_t max_scrapable;
        int16_t scrap_count;
        int16_t confirm_scrap = 1;
        const char* msg;
        const char* plural_scrap;
        const char* plural_surplus;
        s_player* player;

        box_selected = MOVEBOX::Moveable_Box_Selected_(2);
        max_scrapable = settler::N_Freighters_Player_Can_Scrap_(MOX::_PLAYER_NUM);

        if (max_scrapable <= 0) {
            strcpy(buffer, HAROLD::H_Message_(0xE6));
            HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 3);
        } else {
            fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);
            EVANHELP::Set_Scrap_Freighters_Help_List_();
            fields::Clear_Fields_();
            MOX::_moveable_box[2].off_flag = 0;
            fields::Disable_Draw_All_Fields_();
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
            MAINSCR::Draw_Mini_Main_Screen_();
            RUSS::Mox_Sync_Update_();
            fields::Enable_Draw_All_Fields_();

            msg = HAROLD::H_Message_(0xED);
            scrap_count = MAINPUPS::Slider_Popup_(0, max_scrapable, 0, msg, 0);

            if (scrap_count >= 0) {
                player = &MOX::_player[MOX::_PLAYER_NUM];

                // If player attempts to scrap more than current surplus, or all freighters, ask for confirmation
                if (player->n_freighters <= scrap_count || player->surplus_freighters <= scrap_count) {
                    if (scrap_count == 1) {
                        plural_scrap = HAROLD::H_Message_(0xF0 /* HESTR_0F0_FREIGHTER */);
                    } else {
                        plural_scrap = HAROLD::H_Message_(0xF1 /* HESTR_0F1_FREIGHTERS */);
                    }

                    if (player->surplus_freighters == 1) {
                        plural_surplus = HAROLD::H_Message_(0xF0);
                    } else {
                        plural_surplus = HAROLD::H_Message_(0xF1);
                    }

                    const char* format = HAROLD::H_Message_(0xEE);
                    _sprintf(buffer, format, (int)player->surplus_freighters, plural_surplus, (int)scrap_count, plural_scrap);
                    confirm_scrap = HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 1);
                }
            }

            if (confirm_scrap != 0 && scrap_count > 0) {
                if (scrap_count == 1) {
                    plural_scrap = HAROLD::H_Message_(0xF0);
                } else {
                    plural_scrap = HAROLD::H_Message_(0xF1);
                }

                const char* result_format = HAROLD::H_Message_(0xEF);
                _sprintf(buffer, result_format, (int)scrap_count, plural_scrap);
                HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, buffer, 0);

                player = &MOX::_player[MOX::_PLAYER_NUM];
                int32_t new_count = player->n_freighters - scrap_count;
                if (new_count < 0) {
                    new_count = 0;
                }
                player->n_freighters = (int16_t)new_count;

                COLCALC::Do_Player_Colony_Calculations_(MOX::_PLAYER_NUM);
            }
        }

        MOX::_current_screen = MOX::_return_screen;
        if (box_selected) {
            MOX::_moveable_box[2].off_flag = 1;
        }
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
    }
}