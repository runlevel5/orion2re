#include "deps.h"
#include <cstring>

namespace LOADSAVE {
    /**
     * @address 00511020
     */
    void __cdecl Allocate_Data_Space_For_Game_Popup_() {
        // Allocate space for game animations and clear the block
        _game_anims = (s_game_anims*)allocate::Allocate_First_Block_(MOX::_screen_seg, 0x65);

        uint8_t* anim_ptr = reinterpret_cast<uint8_t*>(_game_anims);
        for (int i = 0; i < 0x65; ++i) {
            anim_ptr[i] = 0;
        }

        // Allocate buffers for game dates and stardates (1000 bytes each)
        MOX::_save_game_dates = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 1000);
        MOX::_save_game_stardates = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 1000);

        // Allocate space for the popup fields structure (0x278 = 632 bytes)
        _game_popup_fields = (s_game_popup_fields*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x278);
    }

    /**
     * @address 0050fa10
     */
    void __cdecl Load_Month_Names_() {
        auto copy_message = [](int32_t msg_id, char* dest) {
            const char* src = HAROLD::H_Message_((int16_t)msg_id);
            if (src) {
                const char* s = src;
                char* d = dest;
                while ((*d++ = *s++) != '\0');
            }
        };

        copy_message(0xBE, _months[0]);  // January
        copy_message(0xBF, _months[1]);  // February
        copy_message(0xC0, _months[2]);  // March
        copy_message(0xC1, _months[3]);  // April
        copy_message(0xC2, _months[4]);  // May
        copy_message(0xC3, _months[5]);  // June
        copy_message(0xC4, _months[6]);  // July
        copy_message(0xC5, _months[7]);  // August
        copy_message(0xC6, _months[8]);  // September
        copy_message(0xC7, _months[9]);  // October
        copy_message(0xC8, _months[10]); // November
        copy_message(0xC9, _months[11]); // December
    }

    /**
     * @address 0050f630
     */
    void __cdecl Restore_Main_Screen_() {
        video::Set_Page_Off_();
        MAINSCR::Draw_Mini_Main_Screen_();
        video::Copy_Off_To_Back_();
        video::Set_Refresh_Stencil_();
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;
    }

    /**
     * @address 0050f5d0
     */
    void __cdecl Restore_Game_Popup_() {
        if (MOX::_current_screen == MOX::SCREEN_MAIN_MENU) {
            video::Set_Page_Back_();
            graphics::Fill_(0, 0, 639, 479, 0);
            animate::Draw_(0, 0, reinterpret_cast<s_animation_header*>(MAINMENU::_mainmenu_first_seg));
        } else {
            LOADSAVE::Restore_Main_Screen_();
        }

        video::Set_Page_Off_();
        fields::Invoke_Auto_Function_();
        RUSS::Mox_Sync_Update_();
    }

    /**
     * @address 0050f530
     * @brief Displays a popup message box based on the specified type.
     * 
     * @param message The message content.
     * @param type The type of popup: 0 = Message, 1 = Warning with result (Yes/No), 3 = Warning Notification.
     * @return For type 1, returns the user selection; otherwise returns 0.
     */
    int16_t __cdecl Game_Popup_User_Box_(char* message, int16_t type) {
        int16_t result = 0;

        if (type == 0) {
            video::Set_Refresh_Full_();
            _draw_game_popup_to_back = 1;
            GENDRAW::Message_Box_(message);
            fields::Clear_Fields_();
            Restore_Game_Popup_();
        } else if (type == 1) {
            video::Set_Refresh_Full_();
            _draw_game_popup_to_back = 1;
            result = GENDRAW::Warning_Box_00479700(message);
            fields::Clear_Fields_();
            Restore_Game_Popup_();
        } else if (type == 3) {
            video::Set_Refresh_Full_();
            _draw_game_popup_to_back = 1;
            GENDRAW::Warning_Box_004796E0(message);
            fields::Clear_Fields_();
            Restore_Game_Popup_();
        }

        return result;
    }

    /**
     * @address 00512200
     */
    int8_t __cdecl Get_Save_Slot_() {
        using Fn = int8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00512200});
        return fn();
    }

    /**
     * @address 005120e0
     */
    void __cdecl Set_Up_Load_Save_Popup_() {
        FILEDEF::Get_Saved_Game_Descriptions_(LOADSAVE::_game_popup_fields);

        for (int i = 0; i < 10; ++i) {
            char* desc = MOX::_save_game_description[i].description;
            int32_t len = 0;
            while (len < 37 && desc[len] != '\0') {
                len++;
            }
            if (len < 37) {
                for (int j = len; j < 37; ++j) {
                    desc[j] = '\0';
                }
            }
        }

        sound::Sound_Update_();
        Get_Save_Game_Date_Strings_((char*)MOX::_save_game_dates);
        sound::Sound_Update_();

        Get_Star_Date_Strings_(LOADSAVE::_game_popup_fields, MOX::_save_game_stardates);

        for (int i = 0; i < 10; ++i) {
            char* star_date_entry = MOX::_save_game_stardates + (i * 25);
            char* desc_entry = MOX::_save_game_description[i].description;

            if (desc_entry[0] == '\0' && star_date_entry[0] != '\0') {
                const char* fmt = HAROLD::H_Message_(203);
                _sprintf(desc_entry, fmt, i);
            }
        }

        if (LOADSAVE::Get_Save_Slot_() != 0) {
            MOX::_settings.active_save_slot = -1;
        }
    }

    /**
     * @address 0050ebf0
     */
    void __cdecl Add_Game_Popup_Fields_(int16_t *out_x, int16_t *out_y) {
        _music_level_field = -1000;
        _sfx_level_field = -1000;

        switch (MOX::_screen_data) {
        case 0: {
            MOX::_popup_base_x = 0x90;
            MOX::_popup_base_y = 0x19;
            *out_x = animate::Get_Width_(_game_anims->pictures[0]);
            *out_y = animate::Get_Height_(_game_anims->pictures[0]);

            if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                _game_popup_fields->field_l = fields::Add_Button_Field_(MOX::_popup_base_x + 0x93, MOX::_popup_base_y + 0x2b, s__00551100, _game_anims->pictures[2], s_L_005560e8, '(');
                _game_popup_fields->field_n = fields::Add_Button_Field_(MOX::_popup_base_x + 0x28, MOX::_popup_base_y + 0x58, s__00551100, _game_anims->pictures[3], s_N_0055765c, '(');
            } else {
                _game_popup_fields->field_l = -1000;
                _game_popup_fields->field_n = -1000;
            }

            _game_popup_fields->field_s = fields::Add_Button_Field_(MOX::_popup_base_x + 0x28, MOX::_popup_base_y + 0x2b, s__00551100, _game_anims->pictures[1], s_S_005569cc, '(');
            _game_popup_fields->field_q = fields::Add_Button_Field_(MOX::_popup_base_x + 0x93, MOX::_popup_base_y + 0x58, s__00551100, _game_anims->pictures[4], s_Q_005598f0, '(');
            _game_popup_fields->field_o = fields::Add_Button_Field_(MOX::_popup_base_x + 0x28, MOX::_popup_base_y + 0x133, s__00551100, _game_anims->pictures[5], s_O_00556afc, '(');
            _game_popup_fields->field_esc = fields::Add_Button_Field_(MOX::_popup_base_x + 0x97, MOX::_popup_base_y + 0x133, s__00551100, _game_anims->pictures[6], s__00551120, '(');

            _music_level_field = fields::Add_Scroll_Field_(MOX::_popup_base_x + 0x3e, MOX::_popup_base_y + 0xc2, 0, 0x9c, 0, 0x9c, 0x9b, 0xc, &_game_popup_fields->field22_0x74, s__00551100, '\0');
            _sfx_level_field = fields::Add_Scroll_Field_(MOX::_popup_base_x + 0x3e, MOX::_popup_base_y + 0xd8, 0, 0x9c, 0, 0x9c, 0x9b, 0xc, &_game_popup_fields->field23_0x76, s__00551100, '\0');
            break;
        }
        case 1: {
            MOX::_popup_base_x = 0x90;
            MOX::_popup_base_y = 0x19;
            *out_x = animate::Get_Width_(_game_anims->pictures[8]);
            *out_y = animate::Get_Height_(_game_anims->pictures[8]);

            int16_t offset_y = 0;
            for (int16_t i = 0; i < 13; ++i) {
                int16_t bx = MOX::_popup_base_x + 0x1a;
                int16_t by = MOX::_popup_base_y + 0x29 + offset_y;
                s_animation_header* pic = _game_anims->pictures[9];
                _game_popup_fields->field17_0x52[i] = fields::Add_Hidden_Field_(bx, by, animate::Get_Width_(pic) + bx, animate::Get_Height_(pic) + by, s__00551100, '(');

                _game_popup_fields->field431_0x244[i] = fields::Add_Hidden_Field_(MOX::_popup_base_x + 0x31, by, MOX::_popup_base_x + 0xf3, animate::Get_Height_(pic) + by, s__00551100, ')');
                offset_y += 0x11;
            }
            _game_popup_fields->field_A = fields::Add_Button_Field_(MOX::_popup_base_x + 0x61, MOX::_popup_base_y + 0x152, s__00551100, _game_anims->pictures[10], s_A_00557ed4, '(');
            break;
        }
        case 2:
        case 3: {
            if (MOX::_current_screen == MOX::SCREEN_MAIN_MENU) {
                int16_t w = animate::Get_Width_(_game_anims->pictures[0xb]);
                MOX::_popup_base_x = (0x280 - w) / 2;
                int16_t h = animate::Get_Height_(_game_anims->pictures[0xb]);
                MOX::_popup_base_y = (0x1e0 - h) / 2;
            } else {
                MOX::_popup_base_x = 0x90;
                MOX::_popup_base_y = 0x19;
            }

            s_animation_header* main_pic;
            if (MOX::_screen_data == 2) {
                main_pic = _game_anims->pictures[0xb];
            } else {
                main_pic = _game_anims->pictures[0xe];
            }
            *out_x = animate::Get_Width_(main_pic);
            *out_y = animate::Get_Height_(main_pic);

            Check_For_Saved_Games_(_game_popup_fields->saved_games);
            s_colors colors_style, colors_special;
            Get_Game_Popup_Colors_(2, &colors_style, &colors_special);
            fonts::Set_Font_Special_Colors_(&colors_special);

            for (int16_t i = 0; i < 10; ++i) {
                if (MOX::_settings.active_save_slot == i) {
                    Embed_Special_Color_Codes_(MOX::_save_game_description[i].description, 0x25);
                } else if (MOX::_save_game_description[i].description[0] == 0x03) {
                    Remove_Embedded_Special_Codes_(MOX::_save_game_description[i].description);
                }
            }

            if (MOX::_screen_data == 2) {
                for (int16_t i = 0; i < 10; ++i) {
                    if (_game_popup_fields->saved_games[i] == 2) {
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58);
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description + 4) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58 + 4);
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description + 8) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58 + 8);
                    }
                    _game_popup_fields->saved_game_types[i] = FILEDEF::Get_Game_Type_From_Saved_Game_(i);
                    _game_popup_fields->saved_game_fields[i] = fields::Add_Hidden_Field_(MOX::_popup_base_x + 0x1d, MOX::_popup_base_y + 0x18 + i * 0x1f, MOX::_popup_base_x + 0xe2, MOX::_popup_base_y + 0x18 + i * 0x1f + 0x18, s__00551100, ')');
                }
                _game_popup_fields->field_d = fields::Add_Hidden_Field_(0x275, 0x1d5, 0x27f, 0x1df, s_D_005569dc, '3');
                _game_popup_fields->field_l2 = fields::Add_Button_Field_(MOX::_popup_base_x + 0x25, MOX::_popup_base_y + 0x151, s__00551100, _game_anims->pictures[0xc], s_L_005560e8, '(');
            } else {
                for (int16_t i = 0; i < 10; ++i) {
                    fonts::Set_Font_Style_(2, &colors_style);
                    if (_game_popup_fields->saved_games[i] == 2) {
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58);
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description + 4) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58 + 4);
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description + 8) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58 + 8);
                    }
                    _game_popup_fields->saved_game_types[i] = FILEDEF::Get_Game_Type_From_Saved_Game_(i);
                    _game_popup_fields->saved_game_fields[i] = (int16_t)fields::Add_Continuous_String_Input_Field_(MOX::_popup_base_x + 0x1a, MOX::_popup_base_y + 0x18 + i * 0x1f, 200, fonts::Get_Font_Height_(), (char*)&MOX::_save_game_description[i], 0x1e, 0, 0, ')', 0);
                }
                for (int16_t i = 0; i < 10; ++i) {
                    int16_t y_start = MOX::_popup_base_y + 0x18 + i * 0x1f;
                    int16_t font_h = fonts::Get_Font_Height_();
                    _game_popup_fields->field28_0x7c[i] = fields::Add_Hidden_Field_(MOX::_popup_base_x + 0x1d, y_start + font_h + 1, MOX::_popup_base_x + 0xf4, y_start + font_h * 2, s__00551100, ')');
                }
                _game_popup_fields->field_d = -1000;
                _game_popup_fields->field_s2 = fields::Add_Button_Field_(MOX::_popup_base_x + 0x25, MOX::_popup_base_y + 0x152, s__00551100, _game_anims->pictures[0xf], s_S_005569cc, '(');
            }
            _game_popup_fields->field19_0x6e = fields::Add_Button_Field_(MOX::_popup_base_x + 0xab, MOX::_popup_base_y + 0x152, s__00551100, _game_anims->pictures[0xd], s_C_00552420, '(');
            break;
        }
        default:
            break;
        }

        _game_popup_fields->field_space = fields::Add_Hidden_Field_(MOX::_popup_base_x, MOX::_popup_base_y, *out_x + MOX::_popup_base_x, *out_y + MOX::_popup_base_y, s__00551100, '\0');
        _game_popup_fields->field_esc_2 = fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, s__00551120, ')');
    }

    /**
     * @address 0050e910
     */
    int8_t __cdecl Do_Load_Game_Popup_(int16_t* exit_flag) {
        int16_t exit_loop = 0;
        int8_t load_success = 0;
        int16_t unused1, unused2;
        char msg_buffer[80];

        Load_Month_Names_();

        const char* empty_msg_src = HAROLD::H_Message_(388); // 0x184
        char* empty_msg_dst = _empty_save_slot_message;
        while (*empty_msg_src) {
            *empty_msg_dst++ = *empty_msg_src++;
        }
        *empty_msg_dst = '\0';

        bool from_menu = (MOX::_current_screen == MOX::SCREEN_MAIN_MENU);
        _loaded_debug_save = 0;

        if (!from_menu) {
            EVANHELP::Set_Load_Game_Screen_Help_List_();
            fields::Clear_Fields_();
            MOX::_global_var = static_cast<int8_t>(fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551120, 41));
        }

        fields::Assign_Auto_Function_(_Draw_Load_Save_Game_Popup_, 1);
        fields::Clear_Fields_();
        Add_Game_Popup_Fields_(&unused1, &unused2);
        MOX::_global_var = 0;

        do {
            int16_t input_id = fields::Get_Input_();
            int16_t selected_slot = -1;

            for (int16_t i = 0; i < 10; ++i) {
                if (_game_popup_fields->saved_game_fields[i] == input_id) {
                    selected_slot = i;
                    break;
                }
            }

            if (_game_popup_fields->field_esc_2 + 256 == input_id || _game_popup_fields->field_l2 == input_id) {
                if (selected_slot == -1) {
                    if (MOX::_settings.active_save_slot < 10) {
                        _draw_down = 1;
                        selected_slot = static_cast<int16_t>((uint8_t)MOX::_settings.active_save_slot);
                    }
                }
            }

            if (selected_slot >= 0 && selected_slot < 10) {
                if (_game_popup_fields->saved_games[selected_slot] == 0) {
                    if (MOX::_settings.animations_on != 0 && MOX::_previous_screen == MOX::SCREEN_MAIN) {
                        fields::Clear_Fields_();
                        _game_anims->field1_0x64 = 1;
                        video::Set_Page_Off_();
                        Animate_Game_Popup_Pic_Height_(_game_anims->pictures[11], 1);
                    }

                    load_success = static_cast<int8_t>(FILEDEF::Load_Game_(selected_slot));
                    MOX::_settings.active_save_slot = static_cast<char>(selected_slot);
                    Save_Session_Related_Settings_();

                    if (load_success == 0) {
                        const char* msg = HAROLD::H_Message_(184); // 0xb8
                        _sprintf(msg_buffer, msg, static_cast<int>(selected_slot));
                        Game_Popup_User_Box_(msg_buffer, 3);
                        MOX::_return_screen = MOX::_previous_screen;
                    } else {
                        _loaded_new_game = 1;
                        MOX::_return_screen = (MOX::_game_type == 1) ? MOX::SCREEN_HOTSEAT_SELECT_PLAYER : MOX::SCREEN_REPORTS;
                    }

                    if (!from_menu && MOX::_stardate_at_beg_of_turn != 0) {
                        MISC::Fade_Into_Screen_(MAINSCR::Draw_Mini_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
                    }
                    exit_loop = 1;
                    *exit_flag = 1;
                } else {
                    Bad_Game_File_Message_(static_cast<uint16_t>(static_cast<uint8_t>(_game_popup_fields->saved_games[selected_slot])), selected_slot);
                    exit_loop = 2;
                }
            }

            if (_game_popup_fields->field19_0x6e == input_id) {
                MOX::_screen_data = 0;
                exit_loop = 1;
                if (from_menu) {
                    *exit_flag = 1;
                }
            } else if (_game_popup_fields->field_esc_2 == input_id) {
                load_success = 0;
                exit_loop = 1;
                *exit_flag = 1;
            }

            if (exit_loop == 0) {
                video::Set_Page_Off_();
                fields::Invoke_Auto_Function_();
                RUSS::Mox_Sync_Update_();
            }
        } while (exit_loop == 0);

        for (int i = 0; i < 10; ++i) {
            Remove_Embedded_Special_Codes_(MOX::_save_game_description[i].description, 37);
        }

        return load_success;
    }

    /**
     * @address 005113f0
     */
    void __cdecl Do_Options_Game_Popup_(int16_t* out_val) {
        int8_t original_language = MOX::_settings.language;
        bool should_exit = false;

        EVANHELP::Set_Options_Game_Screen_Help_List_();
        fields::Clear_Fields_();

        MOX::_global_var = static_cast<int8_t>(fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s__00551120, 0x29));

        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(reinterpret_cast<void*>(_Draw_Options_Game_Popup_), 1);

        int16_t dummy1, dummy2;
        Add_Game_Popup_Fields_(&dummy1, &dummy2);
        MOX::_global_var = 0;

        while (true) {
            int16_t input = fields::Get_Input_();

            for (int16_t i = 0; i < 13; ++i) {
                if (_game_popup_fields->field17_0x52[i] == input || _game_popup_fields->field431_0x244[i] == input) {
                    _game_popup_fields->field28_0x7c[i] = (_game_popup_fields->field28_0x7c[i] == 0);
                    break;
                }
            }

            if (_game_popup_fields->field_esc_2 == input) {
                should_exit = true;
                *out_val = 1;
            } else if (_game_popup_fields->field_A == input) {
                should_exit = true;
                MOX::_screen_data = 0;
            }

            if (should_exit) {
                if (static_cast<uint8_t>(original_language) != static_cast<uint8_t>(_game_popup_fields->language)) {
                    MOX::_settings.language = static_cast<int8_t>(_game_popup_fields->language);
                    if (static_cast<uint8_t>(MOX::_settings.language) >= 5) {
                        MOX::_settings.language = 0;
                    }
                    Reload_Language_Strings_();
                    Write_Language_Setting_File_();
                }
                return;
            }

            video::Set_Page_Off_();
            _Draw_Options_Game_Popup_();
            RUSS::Mox_Sync_Update_();
        }
    }

    /**
     * @address 00511530
     */
    void __cdecl Do_Save_Game_Popup_(int16_t* out_should_close) {
        char temp_buffer[36];
        int16_t sStack_6c, sStack_6a;
        bool saved_successfully = false;
        bool confirm_save = false;
        bool trigger_save = false;
        int16_t active_slot_idx = -1;

        const char* msg = HAROLD::H_Message_(0x184); // "Empty Slot" message
        // Inline strcpy for _empty_save_slot_message
        {
            char* dst = _empty_save_slot_message;
            const char* src = msg;
            while ((*dst++ = *src++) != '\0');
        }

        Load_Month_Names_();
        EVANHELP::Set_Save_Game_Screen_Help_List_();
        fields::Clear_Fields_();

        MOX::_global_var = (int8_t)fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, s__00551120, 0x29);
        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(_Draw_Load_Save_Game_Popup_, 1);
        Add_Game_Popup_Fields_(&sStack_6c, &sStack_6a);
        MOX::_global_var = 0;

        bool exit_popup = false;
        do {
            int16_t input_id = fields::Get_Input_();
            if (input_id != 0) {
                for (int16_t i = 0; i < 10; ++i) {
                    if (input_id == _game_popup_fields->saved_game_fields[i]) {
                        if (active_slot_idx == i) {
                            confirm_save = true;
                            trigger_save = true;
                        }
                        active_slot_idx = i;
                        break;
                    }
                    if (input_id == _game_popup_fields->field28_0x7c[i]) {
                        if (active_slot_idx == i) {
                            trigger_save = true;
                            _draw_down = 2;
                            break;
                        }
                        fields::_input_field_active = 1;
                        fields::_cursor_position = 0;
                        fields::_cursor_on = 0;
                        fields::_active_input_field_number = _game_popup_fields->saved_game_fields[i];
                        
                        // Inline strcpy to fields::_continuous_string
                        {
                            char* dst = reinterpret_cast<char*>(&fields::_continuous_string);
                            char* src = MOX::_save_game_description[i].description;
                            while ((*dst++ = *src++) != '\0');
                        }
                        
                        fields::_active_input_field_first = 1;
                        active_slot_idx = i;
                        break;
                    }
                }
            }

            if (input_id == _game_popup_fields->field_s) { // Save button / Space
                if (active_slot_idx < 0 || active_slot_idx >= 10) {
                    if (MOX::_settings.active_save_slot >= 0 && MOX::_settings.active_save_slot < 10) {
                        active_slot_idx = (int16_t)MOX::_settings.active_save_slot;
                        trigger_save = true;
                    }
                } else {
                    trigger_save = true;
                }
            }

            if (trigger_save && active_slot_idx >= 0 && active_slot_idx < 10) {
                trigger_save = false;
                MOX::_settings.active_save_slot = (char)active_slot_idx;
                char* target_desc = MOX::_save_game_description[active_slot_idx].description;

                bool is_empty = (target_desc[0] == '\0');
                if (!is_empty) {
                    // Manual strcmp logic with _empty_save_slot_message
                    const char* empty_msg = _empty_save_slot_message;
                    char* ptr1 = target_desc;
                    const char* ptr2 = empty_msg;
                    int diff = 0;
                    while (true) {
                        if (*ptr1 != *ptr2) {
                            diff = (*ptr1 < *ptr2) ? -1 : 1;
                            break;
                        }
                        if (*ptr1 == '\0') break;
                        if (ptr1[1] != ptr2[1]) {
                            diff = (ptr1[1] < ptr2[1]) ? -1 : 1;
                            break;
                        }
                        ptr1 += 2; ptr2 += 2;
                        if (ptr1[-1] == '\0') break;
                    }
                    if (diff == 0) is_empty = true;
                }

                if (is_empty) {
                    int16_t colonies = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);
                    s_player* plr = &MOX::_player[MOX::_PLAYER_NUM];
                    const char* colony_str = HAROLD::H_Message_(colonies == 1 ? 0xbc : 0xbd);
                    
                    _sprintf(temp_buffer, s_format_0055ad30, plr->name, plr->race_name, (int)colonies, colony_str);
                    
                    // Length check (max 36 chars)
                    int len = 0;
                    while (temp_buffer[len] != '\0') len++;
                    if (len > 0x24) temp_buffer[0x24] = '\0';
                    
                    // strcpy result to target_desc
                    char* src = temp_buffer;
                    char* dst = target_desc;
                    while ((*dst++ = *src++) != '\0');
                } else if (confirm_save) {
                    // strcpy from fields::_continuous_string to target_desc
                    char* src = reinterpret_cast<char*>(&fields::_continuous_string);
                    char* dst = target_desc;
                    while ((*dst++ = *src++) != '\0');
                }

                FILEDEF::Save_Game_(active_slot_idx);
                Save_Session_Related_Settings_();
                saved_successfully = true;
                Get_Save_Game_Date_Strings_((char*)MOX::_save_game_dates);
                Get_Star_Date_Strings_(_game_popup_fields, (char*)MOX::_save_game_stardates);
                exit_popup = true;
                *out_should_close = 1;
            }

            if (input_id == _game_popup_fields->field_o || (input_id < 0 && (active_slot_idx = -input_id) >= 0)) {
                // Potential check against popup field range omitted for clarity as per decompiler
                MOX::_screen_data = 0;
                exit_popup = true;
            } else if (input_id == _game_popup_fields->field_esc) {
                exit_popup = true;
                *out_should_close = 1;
            }

            if (!exit_popup) {
                video::Set_Page_Off_();
                _Draw_Load_Save_Game_Popup_();
                RUSS::Mox_Sync_Update_();
            }
        } while (!exit_popup);

        for (int i = 0; i < 10; ++i) {
            Remove_Embedded_Special_Codes_(MOX::_save_game_description[i].description);
        }

        if (saved_successfully && MOX::_settings.animations_on != '\0') {
            fields::Clear_Fields_();
            Animate_Game_Popup_Pic_Height_(_game_anims->pictures[0xe], 1);
            _game_anims->field1_0x64 = 1;
        }
    }

    /**
     * @address 0050e7e0
     */
    void __cdecl Check_For_Saved_Games_(int8_t* games) {
        char filename_buffer[20];
        char itoa_buffer[12];
        int16_t game_index = 0;

        do {
            __itoa(game_index + 1, itoa_buffer, 10);

            char* dest = filename_buffer;
            const char* src = s_SAVE_005568fc;
            while ((*dest++ = *src++) != '\0');
            dest--;

            src = itoa_buffer;
            while ((*dest++ = *src++) != '\0');
            dest--;

            src = s__GAM_005568f4;
            while ((*dest++ = *src++) != '\0');

            games[game_index] = 0;

            if (!dos::DIR_(filename_buffer, filename_buffer)) {
                games[game_index] = 1;
            } else {
                if (LOADSAVE::Check_Magic_Number_(filename_buffer) < 1) {
                    games[game_index] = 2;
                } else {
                    if (LOADSAVE::Get_Game_Type_From_Saved_Game_(filename_buffer)) {
                        int8_t game_type = (int8_t)FILEDEF::Get_Game_Type_From_Saved_Game_(game_index);
                        if (game_type == 3 || game_type == 2) {
                            games[game_index] = 3;
                        }
                    } else {
                        games[game_index] = 2;
                    }
                }
            }

            game_index++;
        } while (game_index < 10);
    }

    /**
     * @address 0050f8d0
     */
    void __cdecl Get_Star_Date_Strings_(s_game_popup_fields* fields, char* output_buf) {
        for (int16_t i = 0; i < 10; ++i) {
            char local_str_buf[25];
            helpers::memset(local_str_buf, 0, 25);

            int32_t star_date_value = fields->field0_0x0[i];

            if (star_date_value > 0) {
                // Fetch the prefix (e.g., "Stardate " or "D ")
                const char* prefix = HAROLD::H_Message_(202);
                helpers::strcpy(local_str_buf, prefix);

                char itoa_buf[12];
                __itoa(star_date_value, itoa_buf, 10);

                size_t current_len = helpers::strlen(local_str_buf);
                size_t itoa_len = helpers::strlen(itoa_buf);

                // Append itoa result while inserting a decimal point before the last digit
                for (size_t j = 0; j < itoa_len; ++j) {
                    if ((itoa_len - j) == 1) {
                        local_str_buf[current_len++] = '.';
                    }
                    local_str_buf[current_len++] = itoa_buf[j];
                }
                local_str_buf[current_len] = '\0';

            } else if (star_date_value < 0) {
                // If the value is negative, mark it as invalid
                helpers::memcpy(local_str_buf, s_INVALID_STR, 12);
            }

            // Copy the resulting string (25 bytes) to the correct slot in the output buffer
            helpers::memcpy(output_buf + (i * 25), local_str_buf, 25);
        }
    }

    /**
     * @address 0050f7d0
     */
    void __cdecl Get_Save_Game_Date_Strings_(char* output_buffer) {
        char slot_num_str[12];
        char filename[20];
        char date_str[28];

        for (int16_t i = 0; i < 10; ++i) {
            __itoa(i + 1, slot_num_str, 10);

            char* dest = filename;
            const char* src = s_SAVE_005568fc;
            while ((*dest++ = *src++) != '\0');
            dest--;

            src = slot_num_str;
            while ((*dest++ = *src++) != '\0');
            dest--;

            src = s_dot_GAM_005568f4;
            while ((*dest++ = *src++) != '\0');

            for (int j = 0; j < 25; ++j) {
                date_str[j] = 0;
            }

            if (accss::_access_(filename, 0) == 0) {
                Get_File_Date_String_(filename, date_str);
            }

            char* out_dest = &output_buffer[i * 25];
            for (int j = 0; j < 25; ++j) {
                out_dest[j] = date_str[j];
            }
        }
    }

    /**
     * @address 0043d0d0
     */
    int32_t __cdecl Check_Magic_Number_(const char* filename) {
        int32_t magic_number = 0;
        FILE* file_ptr = _wfopen(filename, s_rb_0055117c);

        if (file_ptr != nullptr) {
            if (_fseek(file_ptr, 0x29, SEEK_SET) == 0) {
                _fread(&magic_number, 4, 1, file_ptr);
            }
            _fclose(file_ptr);
        }

        return magic_number;
    }

    /**
     * @address 0050fe70
     */
    void __cdecl _Draw_Main_Game_Popup_To_Bitmap_(s_animation_header* bitmap_ptr) {
        int16_t saved_x = MOX::_popup_base_x;
        int16_t saved_y = MOX::_popup_base_y;

        _bitmap = bitmap_ptr;
        _print_data_to_bitmap = 1;
        MOX::_popup_base_x = 0;
        MOX::_popup_base_y = 0;

        _Draw_Main_Game_Popup_();

        _print_data_to_bitmap = 0;
        MOX::_popup_base_x = saved_x;
        MOX::_popup_base_y = saved_y;
    }

    /**
     * @address 0050fe50
     */
    void __cdecl _Draw_Load_Save_Game_Popup_To_Bitmap_(s_animation_header* bitmap) {
        _print_data_to_bitmap = 1;
        _bitmap = bitmap;
        _Draw_Load_Save_Game_Popup_();
        _print_data_to_bitmap = 0;
    }

    /**
     * @address 0050fec0
     */
    void __cdecl _Draw_Options_Game_Popup_To_Bitmap_(s_animation_header* bitmap) {
        _print_data_to_bitmap = 1;
        _bitmap = bitmap;
        _Draw_Options_Game_Popup_();
        _print_data_to_bitmap = 0;
    }

    /**
     * @address 0043d070
     */
    bool __cdecl Get_Game_Type_From_Saved_Game_(const char* filename) {
        bool is_valid = false;
        FILE* file = _wfopen(filename, s_rb_0055117c);

        if (file != nullptr) {
            uint32_t game_type_header;
            _fread(&game_type_header, 4, 1, file);

            if (Is_Valid_Game_Type_(game_type_header)) {
                is_valid = true;
            }

            _fclose(file);
        }

        return is_valid;
    }

    void __cdecl _Draw_Load_Save_Game_Popup_() {
        void* popup_frame;
        if (MOX::_screen_data == 2) {
            popup_frame = _game_anims->pictures[0xB];
        } else {
            popup_frame = _game_anims->pictures[0xE];
        }

        if (MOX::_current_screen == MOX::SCREEN_MAIN_MENU) {
            animate::Draw_(0, 0, (s_animation_header*)MAINMENU::_mainmenu_first_seg);
            animate::Draw_(MOX::_popup_base_x, MOX::_popup_base_y, (s_animation_header*)popup_frame);
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        } else if (_print_data_to_bitmap == 0) {
            if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                Restore_Main_Screen_();
                MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            }
            animate::Draw_(MOX::_popup_base_x, MOX::_popup_base_y, (s_animation_header*)popup_frame);
        }

        for (int i = 0; i < 10; ++i) {
            char stardate_buf[25];
            char date_buf[25];

            _memcpy(stardate_buf, MOX::_save_game_stardates + (i * 25), 25);

            bool is_valid_slot = (_game_popup_fields->saved_games[i] != 2);
            int16_t x_icon, y_icon;
            if (_print_data_to_bitmap == 0) {
                x_icon = MOX::_popup_base_x + 0xE4;
                y_icon = MOX::_popup_base_y + 0x24 + (i * 31);
            } else {
                x_icon = 0xE4;
                y_icon = (i * 31) + 0x24;
            }

            void* game_type_icon = nullptr;
            switch (_game_popup_fields->saved_game_types[i]) {
            case 0: game_type_icon = _game_anims->pictures[0x10]; break;
            case 1: game_type_icon = _game_anims->pictures[0x11]; break;
            case 2:
            case 3: game_type_icon = _game_anims->pictures[0x12]; break;
            }

            if (game_type_icon) {
                if (_print_data_to_bitmap == 0) {
                    animate::Draw_(x_icon, y_icon, (s_animation_header*)game_type_icon);
                } else {
                    bitmap::Add_Picture_To_Bitmap_(x_icon, y_icon, (s_animation_header*)game_type_icon, _bitmap);
                }
            }

            int16_t text_x, text_y;
            if (_print_data_to_bitmap == 0) {
                text_x = MOX::_popup_base_x + 0x20;
                text_y = MOX::_popup_base_y + 0x18 + (i * 31);
                if (MOX::_screen_data == 2) {
                    Get_Game_Popup_Colors_(2, &_game_popup_fields->popup_color1, &_game_popup_fields->popup_color2);
                    s_colors* color = (MOX::_settings.active_save_slot == (char)i) ? &_game_popup_fields->popup_color1 : &_game_popup_fields->popup_color2;
                    fonts::Set_Font_Style_(2, color);
                    const char* desc = is_valid_slot ? MOX::_save_game_description[i].description : s_INVALID_STR;
                    fonts::Print_(text_x, text_y, desc);
                }
            } else {
                text_x = 0x20;
                text_y = (i * 31) + 0x18;
                Get_Game_Popup_Colors_(2, &_game_popup_fields->popup_color1, &_game_popup_fields->popup_color2);
                s_colors* color = (MOX::_settings.active_save_slot == (char)i) ? &_game_popup_fields->popup_color1 : &_game_popup_fields->popup_color2;
                fonts::Set_Font_Style_(2, color);

                int16_t print_x = (MOX::_screen_data == 3) ? 0x24 : 0x20;
                if (is_valid_slot) {
                    if (MOX::_save_game_description[i].description[0] != '\0') {
                        fonts::Print_To_Bitmap_(print_x, text_y, MOX::_save_game_description[i].description, _bitmap);
                    }
                } else {
                    fonts::Print_To_Bitmap_(print_x, text_y, s_INVALID_STR, _bitmap);
                }
            }

            // Print stardate and real-world date
            Get_Game_Popup_Colors_(1, &_game_popup_fields->popup_color1, &_game_popup_fields->popup_color2);
            s_colors* color_1 = (MOX::_settings.active_save_slot == (char)i) ? &_game_popup_fields->popup_color1 : &_game_popup_fields->popup_color2;
            fonts::Set_Font_Style_(1, color_1);

            if (MOX::_screen_data == 3) text_x += 2;

            if (_print_data_to_bitmap == 0) {
                if (helpers::strcmp(_empty_save_slot_message, MOX::_save_game_description[i].description) != 0) {
                    fonts::Print_(text_x, text_y + 14, stardate_buf);
                }
            } else {
                fonts::Print_To_Bitmap_(text_x, text_y + 14, stardate_buf, _bitmap);
            }

            _memcpy(date_buf, MOX::_save_game_dates + (i * 25), 25);

            if (_print_data_to_bitmap == 0) {
                fonts::Print_(text_x + 100, text_y + 14, date_buf);
            } else {
                fonts::Print_To_Bitmap_(text_x + 100, text_y + 14, date_buf, _bitmap);
            }
        }
    }

    /**
     * @address 0050f730
     */
    void __cdecl Get_Game_Popup_Colors_(int16_t font_idx, s_colors* out_colors_primary, s_colors* out_colors_secondary) {
        s_colors* colors;

        if (MOX::_current_screen == MOX::SCREEN_MAIN_MENU) {
            // Colors for main menu context
            colors = MISC::Font_Colors2_(font_idx, 0xFE, 0xFF);
            *out_colors_primary = *colors;

            colors = MISC::Font_Colors2_(font_idx, 0xFB, 0xFC);
            *out_colors_secondary = *colors;
        } else {
            // Colors for general game context
            MOX::_mox_font_colors_offset = 0x59; // 'Y'
            colors = MISC::Font_Colors2_(font_idx, 0x56, 0x5B); // 'V', '['
            *out_colors_primary = *colors;

            MOX::_mox_font_colors_offset = 0x5B; // '['
            colors = MISC::Font_Colors2_(font_idx, 0x59, 0x7E); // 'Y', '~'
            *out_colors_secondary = *colors;
        }
    }

    /**
     * @address 0050f4c0
     */
    void __cdecl Bad_Game_File_Message_(int16_t type, int16_t file_index) {
        char buffer[256];
        int16_t message_id = 0;

        if (type == 1) {
            message_id = 0xB2; // 178
        } else if (type == 2) {
            message_id = 0xB3; // 179
        } else if (type == 3) {
            message_id = 0xB4; // 180
        }

        if (message_id != 0) {
            const char* format = HAROLD::H_Message_(message_id);
            _sprintf(buffer, format, (int)(file_index + 1));
        }

        Game_Popup_User_Box_(buffer, 3);
    }

    /**
     * @address 0043d130
     */
    bool __cdecl Is_Valid_Game_Type_(uint32_t game_type) {
        return game_type == 0xe0;
    }

    /**
     * @address 0050fc70
     */
    void __cdecl Animate_Game_Popup_Pic_Height_(s_animation_header* animation, uint16_t type) {
        int16_t height = animate::Get_Height_(animation);
        LOADSAVE::Animate_Game_Popup_(animation, height, -1, type);
    }

    /**
     * @address 0050fca0
     */
    void __cdecl Animate_Game_Popup_(s_animation_header* header, int16_t y_offset, int16_t direction, uint16_t type) {
        uint16_t w = animate::Get_Width_(header);
        uint16_t h = animate::Get_Height_(header);

        allocate::Push_Block_(MOX::_screen_seg);
        int32_t bitmap_size = bitmap::Get_Bitmap_Size_(w, h);
        s_animation_header* temp_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size + 100);

        bitmap::Create_Blank_Bitmap_(w, h, temp_bitmap);
        bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)temp_bitmap, header);

        switch (type) {
            case 0:
                LOADSAVE::_Draw_Main_Game_Popup_To_Bitmap_(temp_bitmap);
                break;
            case 1:
            case 2:
                LOADSAVE::_Draw_Load_Save_Game_Popup_To_Bitmap_(temp_bitmap);
                break;
            case 3:
                LOADSAVE::_Draw_Options_Game_Popup_To_Bitmap_(temp_bitmap);
                break;
            default:
                break;
        }

        MAINSCR::Draw_Main_Main_Screen_();
        video::Copy_Off_To_Back_();
        video::Set_Refresh_Stencil_();

        int16_t frame_index = 1;
        bool finished = false;

        do {
            // Calculate the step progress based on percentage (frame_index / 100)
            y_offset += (int16_t)((frame_index * (int32_t)h) / 100) * direction;

            // Clamp to height if sliding in
            if (direction > 0 && y_offset > h) {
                y_offset = h;
            }

            int16_t window_bottom = y_offset + 0x1A; // 26 pixels offset
            video::Set_Page_Off_();
            graphics::Set_Window_(0x90, 0x19, w + 0x90, window_bottom);
            video::Clip_On_();

            int16_t draw_y;
            if (frame_index == 14 && direction > 0) {
                draw_y = 0x19; // Draw at fixed top (25)
            } else {
                draw_y = (y_offset - h) + 0x15; // Vertical slide calculation
            }

            animate::Draw_(0x90, draw_y, (s_animation_header*)temp_bitmap);
            video::Clip_Off_();

            if (direction < 1) {
                // Sliding out condition: window has moved up past boundary
                finished = (window_bottom - 0x19) <= 0x19;
            } else {
                // Sliding in condition: full height reached
                finished = (window_bottom - 0x19) > h;
            }

            RUSS::Mox_Sync_Update_();
            frame_index++;
        } while (!finished);

        video::Copy_Off_To_Back_();
        allocate::Pop_Block_(MOX::_screen_seg);
    }

    /**
     * @address 0043d1d0
     */
    void __cdecl Save_Session_Related_Settings_() {
        uint8_t file_buffer[553]; // 0x229 bytes
        bool success = false;

        FILE* file = _wfopen(s_MOX_SET_00556918, s_rb_0055117c);
        if (file != nullptr) {
            _fread(file_buffer, 0x229, 1, file);
            _fclose(file);

            RUSS::Assert_Settings_();

            // Update the session-related portion of the settings (211 bytes)
            for (int i = 0; i < 211; ++i) {
                file_buffer[i] = reinterpret_cast<uint8_t*>(&MOX::_settings)[i];
            }

            success = true;
            RUSS::Assert_Settings_();

            file = _wfopen(s_MOX_SET_00556918, s_wb_00551178);
            if (file != nullptr) {
                _fwrite(file_buffer, 0x229, 1, file);
                _fclose(file);
            } else {
                success = false;
            }
        }

        if (!success) {
            exitm::Exit_With_Message(s_Settings_file_error_00556950);
        }
    }

    /**
     * @address 00512490
     */
    void __cdecl Draw_Popup_Pic1_() {
        animate::Draw_(
            (int16_t)MOX::_popup_base_x + 147,
            (int16_t)MOX::_popup_base_y + 43,
            _game_anims->pictures[23]
        );
    }

    /**
     * @address 005124C0
     */
    void __cdecl Draw_Popup_Pic2_() {
        animate::Draw_(
            (int16_t)MOX::_popup_base_x + 40,
            (int16_t)MOX::_popup_base_y + 88,
            _game_anims->pictures[24]
        );
    }

    /**
     * @address 00510420
     */
    void __cdecl _Draw_Main_Game_Popup_() {
        int16_t input_result = fields::Auto_Input_();

        int16_t x1 = MOX::_popup_base_x + 62;      // 0x3e
        int16_t y1 = MOX::_popup_base_y + 194;     // 0xc2
        int16_t y1_end = MOX::_popup_base_y + 207; // 0xcf
        int16_t x1_end = _game_popup_fields->field22_0x74 + x1;

        int16_t x2 = x1;
        int16_t y2 = MOX::_popup_base_y + 216;     // 0xd8
        int16_t y2_end = MOX::_popup_base_y + 229; // 0xe5
        int16_t x2_end = _game_popup_fields->field23_0x76 + x1;

        // Junk/Optimization artifact loop detected in assembly
        volatile char stack_buf[8];
        for (int i = 1; i < 8; ++i) {
            stack_buf[i] = static_cast<char>(i / 2) - 103;
        }

        if (_print_data_to_bitmap == 0) {
            MAINSCR::Draw_Mini_Main_Screen_();
            animate::Draw_(MOX::_popup_base_x, MOX::_popup_base_y, _game_anims->pictures[0]);

            if (_dont_draw_darkened_buttons == 0) {
                Draw_Popup_Pic1_();
                Draw_Popup_Pic2_();
                _dont_draw_darkened_buttons = 0;
            }
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        if (MOX::_game_type != 0) {
            if (_print_data_to_bitmap != 0) {
                // Draw picture 0x17 (23)
                s_animation_header* pic23 = _game_anims->pictures[23];
                int16_t h23 = animate::Get_Height_(pic23);
                int16_t w23 = animate::Get_Width_(pic23);
                bitmap::Create_Blank_Bitmap_(w23, h23, MAINSCR::_temp_bitmap);
                bitmap::Add_Picture_To_Bitmap_(0, 0, pic23, (s_animation_header*)MAINSCR::_temp_bitmap);
                bitmap::Add_Picture_To_Bitmap_(147, 43, (s_animation_header*)MAINSCR::_temp_bitmap, _bitmap); // 0x93, 0x2b

                // Draw picture 0x18 (24)
                s_animation_header* pic24 = _game_anims->pictures[24];
                int16_t h24 = animate::Get_Height_(pic24);
                int16_t w24 = animate::Get_Width_(pic24);
                bitmap::Create_Blank_Bitmap_(w24, h24, MAINSCR::_temp_bitmap);
                bitmap::Add_Picture_To_Bitmap_(0, 0, pic24, (s_animation_header*)MAINSCR::_temp_bitmap);
                bitmap::Add_Picture_To_Bitmap_(40, 88, (s_animation_header*)MAINSCR::_temp_bitmap, _bitmap); // 0x28, 0x58
            }
        }

        // Draw picture 7 in Window 1
        if (_print_data_to_bitmap != 0) {
            bitmap::Create_Blank_Bitmap_(x1_end - x1 + 1, y1_end - y1 + 1, MAINSCR::_temp_bitmap);
            bitmap::Add_Picture_To_Bitmap_(0, 0, _game_anims->pictures[7], (s_animation_header*)MAINSCR::_temp_bitmap);
            bitmap::Add_Picture_To_Bitmap_(x1, y1, (s_animation_header*)MAINSCR::_temp_bitmap, _bitmap);
        } else {
            graphics::Set_Window_(x1, y1, x1_end, y1_end);
            video::Clip_On_();
            animate::Draw_(x1, y1, _game_anims->pictures[7]);
            graphics::Reset_Window_();
            video::Clip_Off_();
        }

        // Draw picture 7 in Window 2
        if (_print_data_to_bitmap != 0) {
            bitmap::Create_Blank_Bitmap_(x2_end - x1 + 1, y2_end - y2 + 1, MAINSCR::_temp_bitmap);
            bitmap::Add_Picture_To_Bitmap_(0, 0, _game_anims->pictures[7], (s_animation_header*)MAINSCR::_temp_bitmap);
            bitmap::Add_Picture_To_Bitmap_(x1, y2, (s_animation_header*)MAINSCR::_temp_bitmap, _bitmap);
        } else {
            graphics::Set_Window_(x1, y2, x2_end, y2_end);
            video::Clip_On_();
            animate::Draw_(x1, y2, _game_anims->pictures[7]);
            graphics::Reset_Window_();
            video::Clip_Off_();
        }

        if (_music_level_field == input_result) {
            Set_Music_For_Game_Popup_();
            return;
        }

        if (_sfx_level_field == input_result) {
            Set_Sound_For_Game_Popup_(0);
        }
    }

    /**
     * @address 00510db0
     */
    void __cdecl _Game_Popup_() {
        int16_t exit_condition = 0;
        bool first_loop_drawn = true;
        int16_t dummy_p1;
        int16_t dummy_p2;
        s_settings settings_copy;

        MOX::_first_time_drawn = 1;
        _loaded_new_game = 0;
        fields::Clear_Fields_();
        MOX::_global_var = 0;

        _memcpy(&settings_copy, &MOX::_settings, sizeof(s_settings));

        if (MOX::_screen_data < 0 || MOX::_screen_data > 3) {
            MOX::_screen_data = 0;
        }

        Allocate_Data_Space_For_Game_Popup_();
        Restore_Main_Screen_();
        Load_Game_Popup_Pictures_();
        allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);

        for (int i = 0; i < 158; ++i) {
            ((int32_t*)_game_popup_fields)[i] = -1;
        }

        Set_Current_Game_Option_Flags_();

        do {
            fields::Deactivate_Auto_Function_();
            _draw_game_popup_to_back = 1;

            if (first_loop_drawn) {
                if (MOX::_settings.animations_on == 0) {
                    first_loop_drawn = false;
                    Restore_Main_Screen_();
                } else {
                    first_loop_drawn = false;
                    video::Set_Page_Off_();
                    Animate_Game_Popup_Pic_(LOADSAVE::_game_anims->pictures[0]);
                }
            } else {
                Restore_Main_Screen_();
            }

            if (MOX::_screen_data == 2 || MOX::_screen_data == 3) {
                sound::Sound_Update_();
                Set_Up_Load_Save_Popup_();
                MOX::_return_screen = static_cast<MOX::SCREEN>(static_cast<int8_t>(MOX::_previous_screen));
            }

            fields::Clear_Fields_();
            Add_Game_Popup_Fields_(&dummy_p1, &dummy_p2);

            switch (MOX::_screen_data) {
                case 0: Do_Main_Game_Popup_(&exit_condition); break;
                case 1: Do_Options_Game_Popup_(&exit_condition); break;
                case 2: Do_Load_Game_Popup_(&exit_condition); break;
                case 3: Do_Save_Game_Popup_(&exit_condition); break;
            }
        } while (exit_condition == 0);

        if (_loaded_debug_save == 0) {
            Update_Game_Settings_();
            Save_Session_Related_Settings_();
        }

        MOX::_current_screen = MOX::_return_screen;
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();

        if (MOX::_settings.animations_on != 0 && _game_anims->field1_0x64 == 0 && _loaded_debug_save == 0) {
            _dont_draw_darkened_buttons = 1;
            video::Set_Page_Off_();
            _Draw_Main_Game_Popup_();
            RUSS::Mox_Sync_Update_();
            timer::Mark_Time_();
            timer::Release_Time_(7);
            video::Set_Page_Off_();
            Animate_Game_Popup_Pic_Height_(_game_anims->pictures[0], 0);
        }

        if (MOX::_current_screen == MOX::SCREEN_NEW_GAME) {
            ERIC::Fast_Fade_Out_();
        } else if (_loaded_new_game == 0) {
            fields::Clear_Fields_();
            fields::Disable_Draw_All_Fields_();
            MOX::_draw_main_screen_to_back = 1;
            allocate::Allocate_First_Screen_Seg_();
            video::Set_Page_Off_();
            MAINSCR::Draw_Mini_Main_Screen_();
            RUSS::Mox_Sync_Update_();
            fields::Enable_Draw_All_Fields_();
        }
    }
}