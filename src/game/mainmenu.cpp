#include "pch.h"

namespace MAINMENU {
    s_main_menu* _mainmenu;
    s_animation_header* _main_menu_background;
    s_animation_header* _mainmenu_first_seg;
    s_animation_header* _mainmenu_last_seg;
    void* __gametype;
    int16_t _max_credits;
    int32_t _credit_count;
    int16_t _credits_font_style;
    uint8_t _credit_num;
    uint8_t _credits_font_spacing;
    s_animation_header* _bitmap;
    s_credit_y_slot _credit_y_slot;
    int32_t _credit_x;
    int16_t _delay_count;
    int16_t _fade_in_count;
    int16_t _fade_out_count;
    char (*_credits)[2][127];
    uint8_t _credits_column_one;

    void __cdecl Main_Menu_Fade_Function_() {
        animate::Draw_(0, 0, _main_menu_background);
    }

    void __cdecl Main_Menu_Fade_Function2_() {
        animate::Draw_(0, 0, _mainmenu_first_seg);
    }

    int16_t __cdecl Do_Main_Menu_File_Animation_() {
        bool finished = false;
        bool input_detected = false;
        int16_t result_code = -1;
        int32_t current_sound_handle = 0;

        file_ani::Open_File_Animation_("MAINMENU.LBX", 0);
        uint32_t buffer_size = MAINMENU::Get_File_Animation_Block_Space_();
        int16_t total_frames = file_ani::Get_File_Animation_Total_Frames_();

        void* buffer = allocate::Allocate_Next_Block_(MOX::_screen_seg, buffer_size);
        file_ani::Set_File_Animation_Buffer_(buffer);

        palette::Set_Palette_Changes_(0, 0xFF);
        animate::Draw_Palette_(_main_menu_background);

        fields::Clear_Fields_();

        int16_t button_fields[6];
        button_fields[0] = fields::Add_Hidden_Field_(10, 20, 25, 35, "C", ')');
        button_fields[1] = fields::Add_Hidden_Field_(20, 30, 35, 45, "S", ')');
        button_fields[2] = fields::Add_Hidden_Field_(30, 40, 45, 55, "L", ')');
        button_fields[3] = fields::Add_Hidden_Field_(40, 50, 55, 65, "M", ')');
        button_fields[4] = fields::Add_Hidden_Field_(50, 60, 65, 75, "H", ')');
        button_fields[5] = fields::Add_Hidden_Field_(60, 70, 75, 85, "Q", ')');

        /* fmtpara: "\x1b" begins an inline color-attribute command; it is preserved here as the original non-printing field token. */
        int16_t hotkey_field = fields::Add_Hot_Key_("\x1b");
        int16_t fullscreen_field = fields::Add_Hidden_Field_(0, 0, 639, 479, "\x1b", ')');

        fields::Assign_Auto_Function_(file_ani::thunk_Draw_File_Animation_, 1);

        while (!finished) {
            timer::Mark_Time_();

            int16_t input = fields::Get_Input_();
            int16_t current_frame = file_ani::Get_File_Animation_Frame_();

            if (current_frame == 5) {
                current_sound_handle = ERICNET::Mox_Play_Sound_(0x35);
            } else if (current_frame == 46) {
                sound::Stop_Sound_(current_sound_handle);
                current_sound_handle = ERICNET::Mox_Play_Sound_(0x36);
            }

            if (input == fullscreen_field || input == hotkey_field) {
                finished = true;
                input_detected = true;
            } else if (current_frame == total_frames) {
                finished = true;
            } else {
                for (int32_t i = 0; i < 6; ++i) {
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
                file_ani::thunk_Draw_File_Animation_();
                RUSS::Mox_Sync_Update_();
            }

            timer::Release_Time_(1);
        }

        if (input_detected) {
            sound::Stop_Sound_(current_sound_handle);
            ERICNET::Mox_Play_Sound_(0x36);
        }

        return result_code;
    }

    void __cdecl Add_Main_Menu_Fields_() {
        char save_filename[16];
        char find_data[16];

        uint8_t game_type = FILEDEF::Get_Game_Type_From_Saved_Game_(9);
        snprintf(save_filename, sizeof(save_filename), "save%d.gam", 10);
        fields::Clear_Fields_();

        bool can_continue = false;
        if (game_type == 0 || game_type == 1) {
            if (dos::DIR_(save_filename, find_data) == 1) {
                can_continue = true;
            }
        }

        if (can_continue) {
            _mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field = fields::Add_Hidden_Field_(0x19F, 0xAC, 0x237, 0xC1, "C", 0x29);
        } else {
            _mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field = -1000;
        }

        if (MOX::_disallow_saves_code == 0 && dos::DIR_("save?.gam", find_data) == 1) {
            _mainmenu->fields[MAIN_MENU_BTN_LOAD].field = fields::Add_Hidden_Field_(0x19F, 0xC3, 0x237, 0xD7, "L", 0x29);
        } else {
            _mainmenu->fields[MAIN_MENU_BTN_LOAD].field = -1000;
        }

        _mainmenu->fields[MAIN_MENU_BTN_NEW].field = fields::Add_Hidden_Field_(0x19F, 0xD9, 0x237, 0xEE, "N", 0x29);
        _mainmenu->fields[MAIN_MENU_BTN_MULTIPLAY].field = fields::Add_Hidden_Field_(0x19F, 0xF0, 0x237, 0x104, "M", 0x29);
        _mainmenu->fields[MAIN_MENU_BTN_HELP].field = fields::Add_Hidden_Field_(0x19F, 0x106, 0x237, 0x11B, "H", 0x29);
        _mainmenu->fields[MAIN_MENU_BTN_QUIT].field = fields::Add_Hidden_Field_(0x19F, 0x11D, 0x237, 0x132, "Q", 0x29);
    }

    void __cdecl Reload_Main_Menu_Screen_() {
        int16_t entry_index = 3;

        RUSS::Mox_Update_();

        _main_menu_background = (s_animation_header*)farload::Far_Reload_Next_("MAINMENU.LBX", 2, MOX::_screen_seg);

        RUSS::Mox_Update_();

        for (int32_t i = 0; i < 6; ++i) {
            _mainmenu->fields[i].field0_0x0 = (s_animation_header*)farload::Far_Reload_Next_("MAINMENU.LBX", entry_index, MOX::_screen_seg);
            _mainmenu->fields[i].field1_0x4 = (s_animation_header*)farload::Far_Reload_Next_("MAINMENU.LBX", entry_index + 1, MOX::_screen_seg);
            _mainmenu->fields[i].field2_0x8 = (s_animation_header*)farload::Far_Reload_Next_("MAINMENU.LBX", entry_index + 2, MOX::_screen_seg);

            _mainmenu->fields[i].field = -1000;

            entry_index += 3;
            RUSS::Mox_Update_();
        }
    }

    void __cdecl Set_Main_Menu_Screen_Help_List_Aux_(s_help_box* help_data, int16_t count) {
        fields::Deactivate_Help_List_();
        fields::Set_Help_List_(help_data, count);
    }

    void __cdecl Set_Main_Menu_Screen_Help_List_() {
        Set_Main_Menu_Screen_Help_List_Aux_(EVANHELP::_main_menu_screen_help_list, 6);
    }

    void __cdecl Load_Mainmenu_Load_Game_Popup_() {
        s_game_anims* anims = LOADSAVE::_game_anims;
        const char* lbx = "GAME.LBX";
        void* seg = MOX::_screen_seg;

        anims->pictures[11] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x14, seg);
        anims->pictures[12] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x15, seg);
        anims->pictures[13] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x16, seg);
        anims->pictures[16] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x17, seg);
        anims->pictures[17] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x18, seg);
        anims->pictures[18] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x19, seg);
        anims->pictures[19] = (s_animation_header*)farload::Far_Reload_Next_(lbx, 0x1a, seg);
    }

    int8_t __cdecl Mainmenu_Load_Game_Popup_(int16_t* save_game_list) {
        LOADSAVE::_draw_game_popup_to_back = 1;
        MOX::_screen_data = 2;

        _mainmenu_first_seg = (s_animation_header*)farload::Far_Reload_("MAINMENU.LBX", 0x15, MOX::_screen_seg);

        LOADSAVE::_game_anims = (s_game_anims*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*LOADSAVE::_game_anims));
        memset(LOADSAVE::_game_anims, 0, sizeof(*LOADSAVE::_game_anims));

        LOADSAVE::_game_popup_fields = (s_game_popup_fields*)allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*LOADSAVE::_game_popup_fields));
        memset(LOADSAVE::_game_popup_fields, 0xFF, sizeof(*LOADSAVE::_game_popup_fields));

        LOADSAVE::Load_Month_Names_();
        Load_Mainmenu_Load_Game_Popup_();
        LOADSAVE::Set_Up_Load_Save_Popup_();

        int8_t result = LOADSAVE::Do_Load_Game_Popup_(save_game_list);

        fields::Clear_Fields_();

        return result;
    }

    int8_t __cdecl Do_Mainmenu_Load_Screen_() {
        int8_t result;
        int16_t popup_status = 0;

        _mainmenu_last_seg = (s_animation_header*)farload::Far_Reload_("MAINMENU.LBX", 21, MOX::_screen_seg);

        video::Set_Refresh_Stencil_();
        video::Set_Page_Back_();

        // Clear background with black (0)
        graphics::Fill_(0, 0, 649, 479, 0);

        animate::Draw_(0, 0, _mainmenu_last_seg);
        video::Copy_Back_To_Off_();
        RUSS::Mox_Sync_Update_();
        fields::Deactivate_Help_List_();

        MOX::_previous_screen = SCREEN_MAIN_MENU;

        do {
            result = Mainmenu_Load_Game_Popup_(&popup_status);
        } while (popup_status != 1);

        return result;
    }

    void __cdecl Draw_Main_Menu_Screen_() {
        s_colors color_buffer;

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
            animate::Draw_(0, 0, _main_menu_background);
            video::Copy_Off_To_Back_();
            RUSS::Mox_Sync_Update_();
            _credit_x = 0x42;
            _credit_y_slot.dword = 0x1A6;
            _delay_count = 10;
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        s_animation_header* anim_to_draw = nullptr;
        int16_t draw_y = 0;

        if (_mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field == MOX::_scanned_field) {
            anim_to_draw = _mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field1_0x4;
            draw_y = 0xAC;
        } else if (_mainmenu->fields[MAIN_MENU_BTN_LOAD].field == MOX::_scanned_field) {
            anim_to_draw = _mainmenu->fields[MAIN_MENU_BTN_LOAD].field1_0x4;
            draw_y = 0xC3;
        } else if (_mainmenu->fields[MAIN_MENU_BTN_NEW].field == MOX::_scanned_field) {
            anim_to_draw = _mainmenu->fields[MAIN_MENU_BTN_NEW].field1_0x4;
            draw_y = 0xD9;
        } else if (_mainmenu->fields[MAIN_MENU_BTN_MULTIPLAY].field == MOX::_scanned_field) {
            anim_to_draw = _mainmenu->fields[MAIN_MENU_BTN_MULTIPLAY].field1_0x4;
            draw_y = 0xF0;
        } else if (_mainmenu->fields[MAIN_MENU_BTN_HELP].field == MOX::_scanned_field) {
            anim_to_draw = _mainmenu->fields[MAIN_MENU_BTN_HELP].field1_0x4;
            draw_y = 0x106;
        } else if (_mainmenu->fields[MAIN_MENU_BTN_QUIT].field == MOX::_scanned_field) {
            anim_to_draw = _mainmenu->fields[MAIN_MENU_BTN_QUIT].field1_0x4;
            draw_y = 0x11D;
        }

        if (anim_to_draw != nullptr) {
            animate::Draw_(0x19F, draw_y, anim_to_draw);
        }

        // Special handling for disabled/state-specific buttons (ID -1000)
        if (_mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field == -1000) { // -1000
            animate::Draw_(0x19F, 0xAC, _mainmenu->fields[MAIN_MENU_BTN_CONTINUE].field2_0x8);
        }
        if (_mainmenu->fields[MAIN_MENU_BTN_LOAD].field == -1000) { // -1000
            animate::Draw_(0x19F, 0xC3, _mainmenu->fields[MAIN_MENU_BTN_LOAD].field2_0x8);
        }

        if (_credit_num != 0) {
            Draw_Credits_();
        }

        s_colors* colors = MISC::Font_Colors2_(1, 0xD2, 0xD7);
        color_buffer = *colors;

        fonts::Set_Font_Style_(1, &color_buffer);
        int16_t font_h = fonts::Get_Font_Height_();
        fonts::Print_Centered_(0x205, 0x1BB - (font_h + 1), GAME_VERSION_LABEL);
    }

    void __cdecl Copy_Credit_String_To_Bitmap_(int16_t index, s_animation_header* bitmap) {
        char buffer[100];
        char (*credits)[2][127] = MAINMENU::_credits;
        
        if (credits[index][0][0] == '\0' && credits[index][1][0] == '\0') {
            return;
        }

        strlcpy(buffer, credits[index][0], sizeof(buffer));
        fonts::Print_To_Bitmap_(0, 0, buffer, bitmap);

        int16_t x_pos = 0;
        if (buffer[0] != '\0') {
            x_pos = fonts::Get_String_Width_(buffer) + 4;
        }

        if (credits[index][0][0] != '\0' && credits[index][1][0] != '\0') {
            while (x_pos < (int16_t)MAINMENU::_credits_column_one) {
                fonts::Print_To_Bitmap_(x_pos, 0, ".", bitmap);
                x_pos += fonts::Get_String_Width_(".");
            }
        }

        strlcpy(buffer, credits[index][1], sizeof(buffer));
        fonts::Print_To_Bitmap_((int16_t)MAINMENU::_credits_column_one + 4, 0, buffer, bitmap);
    }

    void __cdecl Fade_Credit_Colors_(int8_t amount, s_colors* colors) {
        for (int16_t i = 1; i < 8; ++i) {
            colors->colors[i] += amount;
        }
    }

    void __cdecl Draw_Credits_() {
        s_colors fade_out_palette = { 0xD3, 0xD5, 0xD4, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5 };
        s_colors fade_in_palette = { 0xD4, 0xD9, 0xD7, 0xD9, 0xD9, 0xD9, 0xD9, 0xD9 };
        s_colors default_palette = { 0xD5, 0xDC, 0xDA, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC };
        s_colors active_palette;

        if (MAINMENU::_delay_count <= 0) {
            MAINMENU::_credit_y_slot.y -= 2;
            MAINMENU::_credit_count = 0x1A6 /* 422 */ - MAINMENU::_credit_y_slot.y;

            int16_t fade_in_line_idx = -1;
            if (MAINMENU::_credit_y_slot.y <= 0xB3 /* 179 */) {
                fade_in_line_idx = (int16_t)((0xB3 - MAINMENU::_credit_y_slot.y) / 14);
            }

            int16_t fade_out_line_idx = (int16_t)(MAINMENU::_credit_count / 14);
            int16_t current_y = MAINMENU::_credit_y_slot.y;

            for (int16_t i = 0; i < MAINMENU::_max_credits; ++i) {
                if (i == fade_in_line_idx) {
                    active_palette = fade_in_palette;
                    int16_t remainder = (MAINMENU::_credit_y_slot.y - 0xB3) % 14;
                    int8_t step = (int8_t)((remainder / 3) * 2);
                    MAINMENU::Fade_Credit_Colors_(step, &active_palette);
                } else if (i == fade_out_line_idx) {
                    active_palette = fade_out_palette;
                    int32_t remainder = MAINMENU::_credit_count % 14;
                    int8_t step = (int8_t)(((remainder + (remainder < 0 ? 3 : 0)) >> 2) * 2);
                    MAINMENU::Fade_Credit_Colors_(step, &active_palette);
                } else {
                    active_palette = default_palette;
                }

                fonts::Set_Outline_Color_(0xFD, 0xFD);
                fonts::Set_Font_Style_Outline_(MAINMENU::_credits_font_style, &active_palette);
                fonts::Set_Font_Spacing_((int16_t)MAINMENU::_credits_font_spacing);
                
                bitmap::Create_Blank_Bitmap_(289 /* 0x121 */, 14, MAINMENU::_bitmap);
                MAINMENU::Copy_Credit_String_To_Bitmap_(i, MAINMENU::_bitmap);
                
                graphics::Set_Window_(66 /* 0x42 */, 179 /* 0xB3 */, 354 /* 0x162 */, 422 /* 0x1A6 */);
                video::Clip_On_();
                animate::Draw_(66 /* 0x42 */, current_y, MAINMENU::_bitmap);
                video::Clip_Off_();

                current_y += 14;
            }
        } else {
            MAINMENU::_delay_count--;
        }

        // Check if credits have finished scrolling off the top
        if (abs(MAINMENU::_credit_y_slot.y - 179) > MAINMENU::_max_credits * 14) {
            MAINMENU::_credit_x = 0x42 /* 66 */;
            MAINMENU::_credit_y_slot.dword = 0x1A6 /* 422 */;
            MAINMENU::_delay_count = 10;
        }
    }

    void __cdecl Main_Menu_Screen_() {
        uint8_t fade_data;
        int16_t last_input_scan = -1;

        video::Set_Refresh_Full_();
        fields::Clear_Fields_();
        RUSS::Mox_Update_();

        _mainmenu_first_seg = (s_animation_header*)farload::Far_Reload_("MAINMENU.LBX", 1, MOX::_screen_seg);
        RUSS::Mox_Update_();
        MAINMENU::Load_Credits_();
        RUSS::Mox_Update_();

        MOX::_game_type = 0;
        _mainmenu = (s_main_menu*)allocate::Allocate_First_Block_(MOX::_global_data_seg, sizeof(s_main_menu));
        __gametype = allocate::Allocate_Next_Block_(MOX::_global_data_seg, 40);
        RUSS::Mox_Update_();

        // Initialize main menu structure fields to -1
        memset(_mainmenu, 0xFF, sizeof(s_main_menu));

        LOADSAVE::_game_popup_fields = (s_game_popup_fields*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, sizeof(*LOADSAVE::_game_popup_fields));
        MOX::_save_game_dates = (char*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, 1000);
        MOX::_save_game_stardates = (char*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, 1000);
        RUSS::Mox_Update_();

        if (MOX2::_pan_main_menu_panel != 0) {
            Reload_Main_Menu_Screen_();
            MOX2::_pan_main_menu_panel = 0;
            MISC::Fade_Into_Screen_(Main_Menu_Fade_Function2_, &fade_data, 5);
            video::Set_Refresh_Full_();
            last_input_scan = Do_Main_Menu_File_Animation_();
        } else {
            bool full_redraw = (MOX::_previous_screen == SCREEN_GAME ||
                                MOX::_previous_screen == SCREEN_HALL_OF_FAME ||
                                MOX::_previous_screen == SCREEN_MULTI_PLAYER ||
                                MOX::_previous_screen == SCREEN_MAIN_NEXT_TURN ||
                                MOX::_previous_screen == SCREEN_NEW_GAME ||
                                MOX::_FULL_DRAW_SCREEN_FLAG != 0);

            if (full_redraw) {
                MOX::_FULL_DRAW_SCREEN_FLAG = 0;
                Reload_Main_Menu_Screen_();
                MISC::Fade_Into_Screen_(Main_Menu_Fade_Function_, &fade_data, 5);
            } else if (MOX::_previous_screen == SCREEN_MAIN_MENU) {
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

        int16_t hotkey_n = fields::Add_Hot_Key_("N");
        bool screen_exit = false;

        fields::Assign_Auto_Function_(Draw_Main_Menu_Screen_, 2);
        Set_Main_Menu_Screen_Help_List_();
        _credit_x = 66; // 0x42
        _credit_y_slot.y = 422; // 0x1a6
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
                        MOX::_current_screen = (MOX::_game_type == 1) ? SCREEN_HOTSEAT_SELECT_PLAYER : SCREEN_REPORTS;
                    } else {
                        GENDRAW::Warning_Box_(HAROLD::H_Message_(207));
                        MOX::_current_screen = SCREEN_MAIN_MENU;
                    }
                } else {
                    MOX::_current_screen = SCREEN_MAIN_MENU;
                }
                screen_exit = true;
            }
            // New Game
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_NEW].field || input_field == hotkey_n || last_input_scan == 0x4e) {
                MOX::_current_screen = SCREEN_NEW_GAME;
                MOX::_return_screen = SCREEN_MAIN;
                screen_exit = true;
            }
            // Load Game
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_LOAD].field || last_input_scan == 0x4c) {
                if (Do_Mainmenu_Load_Screen_() != 0) {
                    if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                        GENDRAW::Warning_Box_(HAROLD::H_Message_(208));
                        MOX::_current_screen = SCREEN_MAIN_MENU;
                    } else {
                        MOX::_current_screen = (MOX::_game_type == 1) ? SCREEN_HOTSEAT_SELECT_PLAYER : SCREEN_REPORTS;
                    }
                } else {
                    if (MOX::_previous_screen == SCREEN_MAIN) {
                        MOX::_current_screen = SCREEN_MAIN;
                    } else {
                        MOX::_current_screen = SCREEN_MAIN_MENU;
                    }
                }
                screen_exit = true;
            }
            // Multi-player
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_MULTIPLAY].field || last_input_scan == 0x4d) {
                MOX::_current_screen = SCREEN_MULTI_PLAYER;
                MOX::_return_screen = SCREEN_MAIN_MENU;
                screen_exit = true;
            }
            // Hall of Fame
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_HELP].field || last_input_scan == 0x48) {
                MOX::_current_screen = SCREEN_HALL_OF_FAME;
                MOX::_return_screen = SCREEN_MAIN_MENU;
                screen_exit = true;
            }
            // Quit
            else if (input_field == _mainmenu->fields[MAIN_MENU_BTN_QUIT].field || last_input_scan == 0x51) {
                MOX::_current_screen = SCREEN_EXIT;
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

    void __cdecl Load_Credits_() {
        char path[20];
        char* data;
        int16_t credit_idx;
        int32_t entry_idx;

        Init_Credits_(path);
        RUSS::Mox_Update_();

        // Load entry 0 to get the total number of credits
        data = (char*)farload::Far_Reload_Next_Data_(path, 0, MOX::_screen_seg, 0, 1, 127);
        entry_idx = 1;
        RUSS::Mox_Update_();

        _max_credits = (int16_t)atoi(data);
        _credits = (char (*)[2][127])allocate::Allocate_Next_Block_(MOX::_screen_seg, sizeof(*_credits) * (uint32_t)_max_credits);

        for (credit_idx = 0; credit_idx < _max_credits; ++credit_idx) {
            RUSS::Mox_Update_();

            // Load first line of credit entry
            allocate::Push_Block_(MOX::_screen_seg);
            data = (char*)farload::Far_Reload_Next_Data_(path, 0, MOX::_screen_seg, entry_idx++, 1, 127);
            allocate::Pop_Block_(MOX::_screen_seg);
            strlcpy(_credits[credit_idx][0], data, sizeof(_credits[credit_idx][0]));

            // Load second line of credit entry
            allocate::Push_Block_(MOX::_screen_seg);
            data = (char*)farload::Far_Reload_Next_Data_(path, 0, MOX::_screen_seg, entry_idx++, 1, 127);
            allocate::Pop_Block_(MOX::_screen_seg);
            strlcpy(_credits[credit_idx][1], data, sizeof(_credits[credit_idx][1]));
        }

        _credit_num = 1;
        _credit_x = 66;
        _delay_count = 0;
        _credit_count = 0;
        _fade_in_count = 0;
        _credit_y_slot.dword = 0x1A6;
        _fade_out_count = 0;

        uint32_t bitmap_size = bitmap::Get_Bitmap_Size_(289, 14);
        _bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);
        RUSS::Mox_Update_();
    }

    int32_t __cdecl Get_File_Animation_Block_Space_() {
        s_animation_header* header = init::_io_buffer;
        int16_t index = header->current_frame;
        const uint32_t* frame_offsets = Animation_Frame_Offsets_(header);
        int32_t current_val = frame_offsets[index];
        int32_t next_val = frame_offsets[index + 1];
        return (next_val - current_val) + 1;
    }

    void __cdecl Init_Credits_(char* filename) {
        switch (MOX::_settings.language) {
            case 1: /* German */
                _credits_font_style = 2;
                _credits_font_spacing = 2;
                _credits_column_one = 0x9a;
                strcpy(filename, "GERCRDTS.LBX");
                return;

            case 2: /* French */
                _credits_column_one = 0xa4;
                _credits_font_spacing = 1;
                _credits_font_style = 2;
                strcpy(filename, "FRECRDTS.LBX");
                return;

            case 3: /* Spanish */
                _credits_column_one = 0x92;
                _credits_font_spacing = 1;
                _credits_font_style = 2;
                strcpy(filename, "SPACRDTS.LBX");
                return;

            case 4:
                _credits_column_one = 0x92;
                _credits_font_spacing = 1;
                _credits_font_style = 2;
                strcpy(filename, "ITACRDTS.LBX");
                return;

            default: /* English */
                _credits_column_one = 0x7c;
                _credits_font_spacing = 1;
                _credits_font_style = 3;
                strcpy(filename, "CREDITS.LBX");
                return;
        }
    }
}