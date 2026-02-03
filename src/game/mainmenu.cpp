#include "deps.h"

namespace MAINMENU {
    void __cdecl Main_Menu_Fade_Function_() {
        animate::Draw_(0, 0, _main_menu_background);
    }

    void __cdecl Main_Menu_Fade_Function2_() {
        animate::Draw_(0, 0, reinterpret_cast<s_animation_header*>(_mainmenu_first_seg));
    }

    uint16_t __cdecl Do_Main_Menu_File_Animation_() {
        bool finished = false;
        bool input_detected = false;
        uint16_t result_code = 0xFFFF;
        int32_t current_sound_handle = 0;

        file_ani::Open_File_Animation_(s_MAINMENU_LBX_0055a630, 0);
        uint32_t buffer_size = file_ani::Get_File_Animation_Block_Space_();
        uint16_t total_frames = (uint16_t)file_ani::Get_File_Animation_Total_Frames_();

        void* buffer = allocate::Allocate_Next_Block_(MOX::_screen_seg, buffer_size);
        file_ani::Set_File_Animation_Buffer_(buffer);

        palette::Set_Palette_Changes_(0, 0xFF);
        animate::Draw_Palette_(_main_menu_background);

        fields::Clear_Fields_();

        int16_t button_fields[6];
        button_fields[0] = fields::Add_Hidden_Field_(10, 20, 25, 35, s_C_00552420, ')');
        button_fields[1] = fields::Add_Hidden_Field_(20, 30, 35, 45, s_S_005569CC, ')');
        button_fields[2] = fields::Add_Hidden_Field_(30, 40, 45, 55, s_L_005560E8, ')');
        button_fields[3] = fields::Add_Hidden_Field_(40, 50, 55, 65, s_M_00557ED8, ')');
        button_fields[4] = fields::Add_Hidden_Field_(50, 60, 65, 75, s_H_005569D4, ')');
        button_fields[5] = fields::Add_Hidden_Field_(60, 70, 75, 85, s_Q_005598F0, ')');

        int16_t hotkey_field = fields::Add_Hot_Key_(s_hotkeys_005510f0);
        int16_t fullscreen_field = fields::Add_Hidden_Field_(0, 0, 639, 479, s_empty_00551120, ')');

        fields::Assign_Auto_Function_(Draw_File_Animation_, 1);

        while (!finished) {
            timer::Mark_Time_();

            int32_t input_raw = fields::Get_Input_();
            int16_t input = (int16_t)input_raw;
            uint16_t current_frame = (uint16_t)file_ani::Get_File_Animation_Frame_();

            if (current_frame == 5) {
                current_sound_handle = sound::Mox_Play_Sound_(0x35); // 53
            } else if (current_frame == 46) {
                sound::Stop_Sound_(current_sound_handle);
                current_sound_handle = sound::Mox_Play_Sound_(0x36); // 54
            }

            if (input == fullscreen_field || input == hotkey_field) {
                finished = true;
                input_detected = true;
            } else if (current_frame == total_frames) {
                finished = true;
            } else {
                for (int i = 0; i < 6; ++i) {
                    if (input == button_fields[i]) {
                        static const uint16_t mapping[] = { 0x43, 0x53, 0x4C, 0x4D, 0x48, 0x51 }; // C, S, L, M, H, Q
                        result_code = mapping[i];
                        finished = true;
                        input_detected = true;
                        break;
                    }
                }
            }

            if (!finished) {
                video::Set_Page_Off_();
                Draw_File_Animation_();
                RUSS::Mox_Sync_Update_();
            }

            timer::Release_Time_(1);
        }

        if (input_detected) {
            sound::Stop_Sound_(current_sound_handle);
            sound::Mox_Play_Sound_(0x36);
        }

        return result_code;
    }

    void __cdecl Add_Main_Menu_Fields_() {
        char save_filename[16];
        char find_data[16];

        uint8_t game_type = FILEDEF::Get_Game_Type_From_Saved_Game_(9);
        _sprintf(save_filename, s_save_percent_d_gam, 10);
        fields::Clear_Fields_();

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

    void __cdecl Reload_Main_Menu_Screen_() {
        int16_t entry_index = 3;

        MOX::Mox_Update_();

        _main_menu_background = (s_animation_header*)farload::Far_Reload_Next_(s_MAINMENU_LBX_0055a630, 2, MOX::_screen_seg);

        MOX::Mox_Update_();

        for (int i = 0; i < 6; ++i) {
            _mainmenu->fields[i].field0_0x0 = (s_animation_header*)farload::Far_Reload_Next_(s_MAINMENU_LBX_0055a630, entry_index, MOX::_screen_seg);
            _mainmenu->fields[i].field1_0x4 = (s_animation_header*)farload::Far_Reload_Next_(s_MAINMENU_LBX_0055a630, entry_index + 1, MOX::_screen_seg);
            _mainmenu->fields[i].field2_0x8 = (s_animation_header*)farload::Far_Reload_Next_(s_MAINMENU_LBX_0055a630, entry_index + 2, MOX::_screen_seg);

            _mainmenu->fields[i].field = -1000;

            entry_index += 3;
            MOX::Mox_Update_();
        }
    }

    void __cdecl Set_Main_Menu_Screen_Help_List_Aux_(void* help_data, int16_t count) {
        fields::Deactivate_Help_List_();
        fields::Set_Help_List_(help_data, count);
    }

    void __cdecl Set_Main_Menu_Screen_Help_List_() {
        Set_Main_Menu_Screen_Help_List_Aux_(&EVANHELP::_main_menu_screen_help_list, 6);
    }

    void __cdecl Load_Mainmenu_Load_Game_Popup_() {
        LOADSAVE::s_game_anims* anims = LOADSAVE::_game_anims;
        const char* lbx = PTR_s_GAME_LBX_0055ac4c;
        void* seg = MOX::_screen_seg;

        anims->pictures[11] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x14, seg);
        anims->pictures[12] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x15, seg);
        anims->pictures[13] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x16, seg);
        anims->pictures[16] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x17, seg);
        anims->pictures[17] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x18, seg);
        anims->pictures[18] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x19, seg);
        anims->pictures[19] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x1a, seg);
    }

    /**
     * @address 00511f40
     */
    int8_t __cdecl Mainmenu_Load_Game_Popup_(int16_t* save_game_list) {
        LOADSAVE::_draw_game_popup_to_back = 1;
        MOX::_screen_data = 2;

        _mainmenu_first_seg = (s_animation_header*)farload::Far_Reload_(s_MAINMENU_LBX_0055a630, 0x15, MOX::_screen_seg);

        LOADSAVE::_game_anims = (LOADSAVE::s_game_anims*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x65);
        uint8_t* anims_ptr = (uint8_t*)LOADSAVE::_game_anims;
        for (int i = 0; i < 0x65; ++i) {
            anims_ptr[i] = 0;
        }

        LOADSAVE::_game_popup_fields = (LOADSAVE::s_game_popup_fields*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x278);
        uint32_t* fields_ptr = (uint32_t*)LOADSAVE::_game_popup_fields;
        for (int i = 0; i < 0x9E; ++i) {
            fields_ptr[i] = 0xFFFFFFFF;
        }

        LOADSAVE::Load_Month_Names_();
        Load_Mainmenu_Load_Game_Popup_();
        LOADSAVE::Set_Up_Load_Save_Popup_();

        int8_t result = LOADSAVE::Do_Load_Game_Popup_(save_game_list);

        fields::Clear_Fields_();

        return result;
    }

    /**
     * Loads and displays the main menu load game screen popup until a selection is confirmed.
     * @address 004e94d0
     */
    int8_t __cdecl Do_Mainmenu_Load_Screen_() {
        int8_t result;
        int16_t status = 0;

        _mainmenu_last_seg = (s_animation_header*)farload::Far_Reload_(s_MAINMENU_LBX_0055a630, 21, MOX::_screen_seg);

        video::Set_Refresh_Stencil_();
        video::Set_Page_Back_();

        // Clear background with black (0)
        graphics::Fill_(0, 0, 649, 479, 0);

        animate::Draw_(0, 0, _mainmenu_last_seg);
        video::Copy_Back_To_Off_();
        RUSS::Mox_Sync_Update_();
        fields::Deactivate_Help_List_();

        MOX::_previous_screen = MOX::SCREEN_MAIN_MENU;

        // Loop until the popup returns 1 (selection made or closed)
        do {
            result = Mainmenu_Load_Game_Popup_(&status);
        } while (status != 1);

        return result;
    }

    void __cdecl Draw_Main_Menu_Screen_() {
        uint8_t color_buffer[8];

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
            animate::Draw_(0, 0, _main_menu_background);
            video::Copy_Off_To_Back_();
            RUSS::Mox_Sync_Update_();
            _credit_x = 0x42;
            _credit_y = 0x1A6;
            _delay_count = 10;
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        s_animation_header* anim_to_draw = nullptr;
        int16_t draw_y = 0;

        if (_mainmenu->fields[0].field == MOX::_scanned_field) {
            anim_to_draw = _mainmenu->fields[0].field1_0x4; // MAIN_MENU_BTN_CONTINUE
            draw_y = 0xAC;
        } else if (_mainmenu->fields[1].field == MOX::_scanned_field) { // MAIN_MENU_BTN_LOAD
            anim_to_draw = _mainmenu->fields[1].field1_0x4;
            draw_y = 0xC3;
        } else if (_mainmenu->fields[2].field == MOX::_scanned_field) { // MAIN_MENU_BTN_NEW
            anim_to_draw = _mainmenu->fields[2].field1_0x4;
            draw_y = 0xD9;
        } else if (_mainmenu->fields[3].field == MOX::_scanned_field) { // MAIN_MENU_BTN_MULTIPLAY
            anim_to_draw = _mainmenu->fields[3].field1_0x4;
            draw_y = 0xF0;
        } else if (_mainmenu->fields[4].field == MOX::_scanned_field) { // MAIN_MENU_BTN_HELP
            anim_to_draw = _mainmenu->fields[4].field1_0x4;
            draw_y = 0x106;
        } else if (_mainmenu->fields[5].field == MOX::_scanned_field) { // MAIN_MENU_BTN_QUIT
            anim_to_draw = _mainmenu->fields[5].field1_0x4;
            draw_y = 0x11D;
        }

        if (anim_to_draw != nullptr) {
            animate::Draw_(0x19F, draw_y, anim_to_draw);
        }

        // Special handling for disabled/state-specific buttons (ID -1000)
        if (_mainmenu->fields[0].field == -1000) { // -1000
            animate::Draw_(0x19F, 0xAC, _mainmenu->fields[0].field2_0x8);
        }
        if (_mainmenu->fields[1].field == -1000) { // -1000
            animate::Draw_(0x19F, 0xC3, _mainmenu->fields[1].field2_0x8);
        }

        if (_credit_num != 0) {
            Draw_Credits_();
        }

        MISC::s_colors* colors = MISC::Font_Colors2_(1, 0xD2, 0xD7);
        _memcpy(color_buffer, colors, 8);

        fonts::Set_Font_Style_(1, (MISC::s_colors*)color_buffer);
        int16_t font_h = fonts::Get_Font_Height_();
        fonts::Print_Centered_(0x205, 0x1BB - (font_h + 1), s_Version_1_31_0055a658);
    }

    void __cdecl Fade_Credit_Colors_(int8_t amount, uint8_t* colors) {
        for (int16_t i = 1; i < 8; ++i) {
            colors[i] += amount;
        }
    }

    void __cdecl Draw_Credits_() {
        uint8_t local_colors[8];

        if (_delay_count <= 0) {
            _credit_y -= 2;
            DAT_0056ece4 = 0x1A6 - _credit_y;

            int16_t start_fade_idx = (_credit_y < 0xB3) ? (int16_t)((0xB3 - _credit_y) / 14) : (int16_t)-1;
            int16_t end_fade_idx = (int16_t)(DAT_0056ece4 / 14);
            int16_t current_y = (int16_t)_credit_y;

            for (int16_t i = 0; i < _max_credits; ++i) {
                if (i == start_fade_idx) {
                    local_colors[0] = 0xD4;
                    local_colors[1] = 0xD9;
                    local_colors[2] = 0xD7;
                    local_colors[3] = 0xD9;
                    local_colors[4] = 0xD9;
                    local_colors[5] = 0xD9;
                    local_colors[6] = 0xD9;
                    local_colors[7] = 0xD9;
                    int8_t intensity = (int8_t)((((_credit_y - 0xB3) % 14) / 3) * 2);
                    Fade_Credit_Colors_(intensity, local_colors);
                } else if (i == end_fade_idx) {
                    int32_t rem = (0x1A6 - _credit_y) % 14;
                    local_colors[0] = 0xD3;
                    local_colors[1] = 0xD5;
                    local_colors[2] = 0xD4;
                    local_colors[3] = 0xD5;
                    local_colors[4] = 0xD5;
                    local_colors[5] = 0xD5;
                    local_colors[6] = 0xD5;
                    local_colors[7] = 0xD5;
                    int8_t intensity = (int8_t)(((rem + (rem < 0 ? 3 : 0)) / 4) * 2);
                    Fade_Credit_Colors_(intensity, local_colors);
                } else {
                    local_colors[0] = 0xD5;
                    local_colors[1] = 0xDC;
                    local_colors[2] = 0xDA;
                    local_colors[3] = 0xDC;
                    local_colors[4] = 0xDC;
                    local_colors[5] = 0xDC;
                    local_colors[6] = 0xDC;
                    local_colors[7] = 0xDC;
                }

                fonts::Set_Outline_Color_(0xFD, 0xFD);
                fonts::Set_Font_Style_Outline_((int16_t)DAT_0056ecf4, local_colors);
                fonts::Set_Font_Spacing_((int16_t)_credits_font_spacing);
                
                bitmap::Create_Blank_Bitmap_(0x121, 14, _bitmap);
                Copy_Credit_String_To_Bitmap_(i, _bitmap);
                
                graphics::Set_Window_(0x42, 0xB3, 0x162, 0x1A6);
                video::Clip_On_();
                animate::Draw_(0x42, current_y, (s_animation_header*)_bitmap);
                video::Clip_Off_();
                
                current_y += 14;
            }
        } else {
            _delay_count--;
        }

        int32_t abs_offset = _credit_y - 0xB3;
        if (abs_offset < 0) abs_offset = -abs_offset;

        if (abs_offset > (int32_t)(_max_credits * 14)) {
            _credit_x = 0x42;
            _credit_y = 0x1A6;
            _delay_count = 10;
        }
    }

    void __cdecl Main_Menu_Screen_() {
        uint8_t fade_data;
        int16_t last_input_scan = -1;

        video::Set_Refresh_Full_();
        fields::Clear_Fields_();
        MOX::Mox_Update_();

        _mainmenu_first_seg = (s_animation_header*)farload::Far_Reload_(s_MAINMENU_LBX_0055a630, 1, MOX::_screen_seg);
        MOX::Mox_Update_();
        MOX::Load_Credits_();
        MOX::Mox_Update_();

        MOX::_game_type = 0;
        _mainmenu = (s_main_menu*)allocate::Allocate_First_Block_(MOX::_global_data_seg, 210); // 0xd2
        __gametype = allocate::Allocate_Next_Block_(MOX::_global_data_seg, 40); // 0x28
        MOX::Mox_Update_();

        // Initialize main menu structure fields to -1
        helpers::memset(_mainmenu, 0, sizeof(s_main_menu));

        LOADSAVE::_game_popup_fields = (LOADSAVE::s_game_popup_fields*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, 632); // 0x278
        MOX::_save_game_dates = allocate::Allocate_Next_Block_(MOX::_global_data_seg, 1000);
        MOX::_save_game_stardates = (char*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, 1000);
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
                animate::Draw_(0, 0, (s_animation_header*)_main_menu_background);
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
        animate::Draw_(0, 0, (s_animation_header*)_main_menu_background);
        video::Copy_Back_To_Off_();
        RUSS::Mox_Sync_Update_();

        int16_t hotkey_n = fields::Add_Hot_Key_(s_N_0055765C);
        bool screen_exit = false;

        fields::Assign_Auto_Function_(Draw_Main_Menu_Screen_, 2);
        Set_Main_Menu_Screen_Help_List_();
        _credit_x = 66; // 0x42
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