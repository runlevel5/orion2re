#include "deps.h"

namespace MAINMENU {
    void __cdecl Main_Menu_Fade_Function_() {
        animate::Draw_(0, 0, reinterpret_cast<animate::s_animation_header*>(_main_menu_background));
    }

    void __cdecl Add_Main_Menu_Fields_() {
        char save_filename[16];
        char find_data[16];

        uint8_t game_type = FILEDEF::Get_Game_Type_From_Saved_Game_(9);
        _sprintf(save_filename, s_save_percent_d_gam, 10);
        fields::Init_Fields_State_();

        bool can_continue = false;
        if (game_type == 0 || game_type == 1) {
            if (dos::DIR_(save_filename, find_data) == 1) {
                can_continue = true;
            }
        }

        if (can_continue) {
            _mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field = fields::Add_Hidden_Field_(0x19F, 0xAC, 0x237, 0xC1, s_C_00552420, 0x29);
        } else {
            _mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field = -1000;
        }

        if (MOX::_disallow_saves_code == 0 && dos::DIR_(s_save_question_gam, find_data) == 1) {
            _mainmenu->fields[MAIN_MENU_BTN_LOAD].field = fields::Add_Hidden_Field_(0x19F, 0xC3, 0x237, 0xD7, s_L_005560e8, 0x29);
        } else {
            _mainmenu->fields[MAIN_MENU_BTN_LOAD].field = -1000; // -1000
        }

        _mainmenu->fields[MAIN_MENU_BTN_NEW].field = fields::Add_Hidden_Field_(0x19F, 0xD9, 0x237, 0xEE, s_N_0055765C, 0x29);
        _mainmenu->fields[MAIN_MENU_BTN_MULTIPLAY].field = fields::Add_Hidden_Field_(0x19F, 0xF0, 0x237, 0x104, s_M_00557ED8, 0x29);
        _mainmenu->fields[MAIN_MENU_BTN_HELP].field = fields::Add_Hidden_Field_(0x19F, 0x106, 0x237, 0x11B, s_H_005569D4, 0x29);
        _mainmenu->fields[MAIN_MENU_BTN_QUIT].field = fields::Add_Hidden_Field_(0x19F, 0x11D, 0x237, 0x132, s_Q_005598f0, 0x29);
    }

    void __cdecl Main_Menu_Screen_() {
        uint8_t fade_data;
        int16_t last_input_scan = -1;

        video::Set_Refresh_Full_();
        fields::Init_Fields_State_();
        MOX::Mox_Update_();

        _mainmenu_first_seg = (uint32_t)farload::Far_Reload_(s_MAINMENU_LBX_0055a630, 1, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::Load_Credits_();
        MOX::Mox_Update_();

        MOX::_game_type = 0;
        _mainmenu = (s_main_menu*)allocate::Allocate_First_Block_(MOX::_global_data_seg, 210); // 0xd2
        __gametype = allocate::Allocate_Next_Block_(MOX::_global_data_seg, 40); // 0x28
        MOX::Mox_Update_();

        // Initialize main menu structure fields to -1
        {
            int16_t* ptr = (int16_t*)_mainmenu;
            for (int i = 0; i < 105; ++i) {
                ptr[i] = -1;
            }
        }

        LOADSAVE::_game_popup_fields = (uint32_t)allocate::Allocate_Next_Block_(MOX::_global_data_seg, 632); // 0x278
        MOX::_save_game_dates = (uint32_t)allocate::Allocate_Next_Block_(MOX::_global_data_seg, 1000);
        MOX::_save_game_stardates = (uint32_t)allocate::Allocate_Next_Block_(MOX::_global_data_seg, 1000);
        MOX::Mox_Update_();

        if (MOX2::_pan_main_menu_panel != 0) {
            Reload_Main_Menu_Screen_();
            MOX2::_pan_main_menu_panel = 0;
            MISC::Fade_Into_Screen_(Main_Menu_Fade_Function2_, &fade_data, 5);
            video::Set_Refresh_Full_();
            last_input_scan = Do_Main_Menu_File_Animation_();
        } else {
            bool full_redraw = (MOX::_previous_screen == MOX::SCREEN_GAME ||
                                MOX::_previous_screen == MOX::SCREEN_HALL_OF_FAME ||
                                MOX::_previous_screen == MOX::SCREEN_MULTI_PLAYER ||
                                MOX::_previous_screen == MOX::SCREEN_MAIN_NEXT_TURN ||
                                MOX::_previous_screen == MOX::SCREEN_NEW_GAME ||
                                MOX::_FULL_DRAW_SCREEN_FLAG != 0);

            if (full_redraw) {
                MOX::_FULL_DRAW_SCREEN_FLAG = 0;
                Reload_Main_Menu_Screen_();
                MISC::Fade_Into_Screen_(Main_Menu_Fade_Function_, &fade_data, 5);
            } else if (MOX::_previous_screen == MOX::SCREEN_MAIN_MENU) {
                Reload_Main_Menu_Screen_();
                video::Set_Page_Off_();
                graphics::Fill_(0, 0, 639, 479, 0);
                animate::Draw_(0, 0, (animate::s_animation_header*)_main_menu_background);
                RUSS::Mox_Sync_Update_();
            } else {
                goto STENCIL_SETUP;
            }
        }

        Add_Main_Menu_Fields_();

    STENCIL_SETUP:
        video::Set_Refresh_Stencil_();
        video::Set_Page_Back_();
        graphics::Fill_(0, 0, 639, 479, 0);
        animate::Draw_(0, 0, (animate::s_animation_header*)_main_menu_background);
        video::Copy_Back_To_Off_();
        RUSS::Mox_Sync_Update_();

        int16_t hotkey_n = fields::Add_Hot_Key_(s_N_0055765C);
        bool screen_exit = false;

        fields::Assign_Auto_Function_(Draw_Main_Menu_Screen_, 2);
        Set_Main_Menu_Screen_Help_List_();
        __credit_x = 66; // 0x42
        _credit_y = 422; // 0x1a6
        _delay_count = 10;

        do {
            int32_t input = fields::Get_Input_();
            int16_t input_field = (int16_t)input;
            MOX::_scanned_field = fields::Scan_Input_();
            timer::Mark_Time_();

            // Continue (Load Game 9)
            if (input_field == _mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field || (last_input_scan == 0x43 && _mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field != -1000)) {
                if (FILEDEF::Load_Game_(9) != 0) {
                    if (MOX::_game_type != 2 && MOX::_game_type != 3) {
                        MOX::_current_screen = (MOX::_game_type == 1) ? MOX::SCREEN_HOTSEAT_SELECT_PLAYER : MOX::SCREEN_REPORTS;
                    } else {
                        GENDRAW::Warning_Box_(HAROLD::H_Message_(207)); // 0xcf
                        MOX::_current_screen = MOX::SCREEN_MAIN_MENU;
                    }
                } else {
                    MOX::_current_screen = MOX::SCREEN_MAIN_MENU;
                }
                screen_exit = true;
            }
            // New Game
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_NEW].field || input_field == hotkey_n || last_input_scan == 0x4e) {
                MOX::_current_screen = MOX::SCREEN_NEW_GAME;
                MOX::_return_screen = 0;
                screen_exit = true;
            }
            // Load Game
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_LOAD].field || last_input_scan == 0x4c) {
                if (Do_Mainmenu_Load_Screen_() != 0) {
                    if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                        GENDRAW::Warning_Box_(HAROLD::H_Message_(208)); // 0xd0
                        MOX::_current_screen = MOX::SCREEN_MAIN_MENU;
                    } else {
                        MOX::_current_screen = (MOX::_game_type == 1) ? MOX::SCREEN_HOTSEAT_SELECT_PLAYER : MOX::SCREEN_REPORTS;
                    }
                } else {
                    if (MOX::_previous_screen == MOX::SCREEN_MAIN) {
                        MOX::_current_screen = MOX::SCREEN_MAIN;
                    } else {
                        MOX::_current_screen = MOX::SCREEN_MAIN_MENU;
                    }
                }
                screen_exit = true;
            }
            // Multi-player
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_MULTIPLAY].field || last_input_scan == 0x4d) {
                MOX::_current_screen = MOX::SCREEN_MULTI_PLAYER;
                MOX::_return_screen = 10;
                screen_exit = true;
            }
            // Hall of Fame
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_HELP].field || last_input_scan == 0x48) {
                MOX::_current_screen = MOX::SCREEN_HALL_OF_FAME;
                MOX::_return_screen = 10;
                screen_exit = true;
            }
            // Quit
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_QUIT].field || last_input_scan == 0x51) {
                MOX::_current_screen = MOX::SCREEN_EXIT;
                screen_exit = true;
            }

            if (!screen_exit) {
                video::Set_Page_Off_();
                Draw_Main_Menu_Screen_();
                RUSS::Mox_Sync_Update_();
            }

            timer::Release_Time_(3);

            if (screen_exit) {
                MAINSCR2::_main_screen_fade_in_flag = 1;
                MOX::_FULL_DRAW_SCREEN_FLAG = 0;
                fields::Deactivate_Auto_Function_();
                return;
            }
        } while (true);
    }
}