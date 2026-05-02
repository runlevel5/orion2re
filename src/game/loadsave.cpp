#include "pch.h"

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
        if (MOX::_current_screen == SCREEN_MAIN_MENU) {
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
            result = GENDRAW::Confirmation_Box_(message);
            fields::Clear_Fields_();
            Restore_Game_Popup_();
        } else if (type == 3) {
            video::Set_Refresh_Full_();
            _draw_game_popup_to_back = 1;
            GENDRAW::Warning_Box_(message);
            fields::Clear_Fields_();
            Restore_Game_Popup_();
        }

        return result;
    }

    /**
     * @address 00512200
     */
    bool __cdecl Active_Save_Slot_Invalid_() {
        const int8_t save_slot = static_cast<int8_t>(MOX::_settings.active_save_slot);

        if (save_slot >= 0 && save_slot < 10 &&
            LOADSAVE::_game_popup_fields->slot_occupied_flags[save_slot] > 0) {
            return false;
        }

        return true;
    }

    /**
     * @address 005120e0
     */
    void __cdecl Set_Up_Load_Save_Popup_() {
        FILEDEF::Get_Saved_Game_Descriptions_(LOADSAVE::_game_popup_fields->slot_occupied_flags);

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

        Get_Star_Date_Strings_(LOADSAVE::_game_popup_fields->slot_occupied_flags, MOX::_save_game_stardates);

        for (int i = 0; i < 10; ++i) {
            char* star_date_entry = MOX::_save_game_stardates + (i * 25);
            char* desc_entry = MOX::_save_game_description[i].description;

            if (desc_entry[0] == '\0' && star_date_entry[0] != '\0') {
                const char* fmt = HAROLD::H_Message_(203);
                _sprintf(desc_entry, fmt, i);
            }
        }

        if (LOADSAVE::Active_Save_Slot_Invalid_() != 0) {
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
                _game_popup_fields->btn_main_load_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x93, MOX::_popup_base_y + 0x2b, s__00551100, _game_anims->pictures[2], s_L_005560e8, '(');
                _game_popup_fields->btn_main_new_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x28, MOX::_popup_base_y + 0x58, s__00551100, _game_anims->pictures[3], s_N_0055765c, '(');
            } else {
                _game_popup_fields->btn_main_load_id = -1000;
                _game_popup_fields->btn_main_new_id = -1000;
            }

            _game_popup_fields->btn_main_save_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x28, MOX::_popup_base_y + 0x2b, s__00551100, _game_anims->pictures[1], s_S_005569cc, '(');
            _game_popup_fields->btn_main_quit_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x93, MOX::_popup_base_y + 0x58, s__00551100, _game_anims->pictures[4], s_Q_005598f0, '(');
            _game_popup_fields->btn_main_options_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x28, MOX::_popup_base_y + 0x133, s__00551100, _game_anims->pictures[5], s_O_00556afc, '(');
            _game_popup_fields->btn_main_close_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x97, MOX::_popup_base_y + 0x133, s__00551100, _game_anims->pictures[6], s__00551120, '(');

            _music_level_field = fields::Add_Scroll_Field_(MOX::_popup_base_x + 0x3e, MOX::_popup_base_y + 0xc2, 0, 0x9c, 0, 0x9c, 0x9b, 0xc, &_game_popup_fields->music_level, s__00551100, '\0');
            _sfx_level_field = fields::Add_Scroll_Field_(MOX::_popup_base_x + 0x3e, MOX::_popup_base_y + 0xd8, 0, 0x9c, 0, 0x9c, 0x9b, 0xc, &_game_popup_fields->sound_fx_level, s__00551100, '\0');
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
                _game_popup_fields->option_names[i] = fields::Add_Hidden_Field_(bx, by, animate::Get_Width_(pic) + bx, animate::Get_Height_(pic) + by, s__00551100, '(');

                _game_popup_fields->option_toggles[i] = fields::Add_Hidden_Field_(MOX::_popup_base_x + 0x31, by, MOX::_popup_base_x + 0xf3, animate::Get_Height_(pic) + by, s__00551100, ')');
                offset_y += 0x11;
            }
            _game_popup_fields->btn_settings_accept_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x61, MOX::_popup_base_y + 0x152, s__00551100, _game_anims->pictures[10], s_A_00557ed4, '(');
            break;
        }
        case 2:
        case 3: {
            if (MOX::_current_screen == SCREEN_MAIN_MENU) {
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

            Check_For_Saved_Games_(_game_popup_fields->slot_status);
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
                    if (_game_popup_fields->slot_status[i] == 2) {
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58);
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description + 4) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58 + 4);
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description + 8) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58 + 8);
                    }
                    _game_popup_fields->saved_game_types[i] = FILEDEF::Get_Game_Type_From_Saved_Game_(i);
                    _game_popup_fields->saved_game_fields[i] = fields::Add_Hidden_Field_(MOX::_popup_base_x + 0x1d, MOX::_popup_base_y + 0x18 + i * 0x1f, MOX::_popup_base_x + 0xe2, MOX::_popup_base_y + 0x18 + i * 0x1f + 0x18, s__00551100, ')');
                }
                _game_popup_fields->debug_hotspot_id = fields::Add_Hidden_Field_(0x275, 0x1d5, 0x27f, 0x1df, s_D_005569dc, '3');
                _game_popup_fields->btn_popup_load_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x25, MOX::_popup_base_y + 0x151, s__00551100, _game_anims->pictures[0xc], s_L_005560e8, '(');
            } else {
                for (int16_t i = 0; i < 10; ++i) {
                    fonts::Set_Font_Style_(2, &colors_style);
                    if (_game_popup_fields->slot_status[i] == 2) {
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
                    _game_popup_fields->save_slot_fields[i] = fields::Add_Hidden_Field_(MOX::_popup_base_x + 0x1d, y_start + font_h + 1, MOX::_popup_base_x + 0xf4, y_start + font_h * 2, s__00551100, ')');
                }
                _game_popup_fields->debug_hotspot_id = -1000;
                _game_popup_fields->btn_popup_save_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0x25, MOX::_popup_base_y + 0x152, s__00551100, _game_anims->pictures[0xf], s_S_005569cc, '(');
            }
            _game_popup_fields->btn_popup_close_id = fields::Add_Button_Field_(MOX::_popup_base_x + 0xab, MOX::_popup_base_y + 0x152, s__00551100, _game_anims->pictures[0xd], s_C_00552420, '(');
            break;
        }
        default:
            break;
        }

        _game_popup_fields->field_space = fields::Add_Hidden_Field_(MOX::_popup_base_x, MOX::_popup_base_y, *out_x + MOX::_popup_base_x, *out_y + MOX::_popup_base_y, s__00551100, '\0');
        _game_popup_fields->screen_cancel_field_id = fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, s__00551120, ')');
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

        bool from_menu = (MOX::_current_screen == SCREEN_MAIN_MENU);
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

            if (_game_popup_fields->screen_cancel_field_id + 256 == input_id || _game_popup_fields->btn_popup_load_id == input_id) {
                if (selected_slot == -1) {
                    if (MOX::_settings.active_save_slot < 10) {
                        _draw_down = 1;
                        selected_slot = static_cast<int16_t>((uint8_t)MOX::_settings.active_save_slot);
                    }
                }
            }

            if (selected_slot >= 0 && selected_slot < 10) {
                if (_game_popup_fields->slot_status[selected_slot] == 0) {
                    if (MOX::_settings.animations_on != 0 && MOX::_previous_screen == SCREEN_MAIN) {
                        fields::Clear_Fields_();
                        _game_anims->animating = 1;
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
                        MOX::_return_screen = (MOX::_game_type == 1) ? SCREEN_HOTSEAT_SELECT_PLAYER : SCREEN_REPORTS;
                    }

                    if (!from_menu && MOX::_stardate_at_beg_of_turn != 0) {
                        MISC::Fade_Into_Screen_(MAINSCR::Draw_Mini_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
                    }
                    exit_loop = 1;
                    *exit_flag = 1;
                } else {
                    Bad_Game_File_Message_(static_cast<uint16_t>(static_cast<uint8_t>(_game_popup_fields->slot_status[selected_slot])), selected_slot);
                    exit_loop = 2;
                }
            }

            if (_game_popup_fields->btn_popup_close_id == input_id) {
                MOX::_screen_data = 0;
                exit_loop = 1;
                if (from_menu) {
                    *exit_flag = 1;
                }
            } else if (_game_popup_fields->screen_cancel_field_id == input_id) {
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
    void __cdecl Do_Options_Game_Popup_(int16_t* exit_code) {
        int8_t initial_language = MOX::_settings.language;
        bool exit_loop = false;
        int16_t dummy_x;
        int16_t dummy_y;

        EVANHELP::Set_Options_Game_Screen_Help_List_();
        fields::Clear_Fields_();
        int16_t hidden_field = fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s__00551120, 0x29);
        MOX::_global_var = hidden_field;

        fields::Clear_Fields_();
        fields::Assign_Auto_Function_(_Draw_Options_Game_Popup_, 1);
        Add_Game_Popup_Fields_(&dummy_x, &dummy_y);

        MOX::_global_var = 0;

        while (true) {
            int16_t input = fields::Get_Input_();

            for (int16_t i = 0; i < 13; ++i) {
                if (_game_popup_fields->option_names[i] == input || _game_popup_fields->option_toggles[i] == input) {
                    _game_popup_fields->options[i] = (_game_popup_fields->options[i] == 0) ? 1 : 0;
                    break;
                }
            }

            if (_game_popup_fields->screen_cancel_field_id == input) {
                exit_loop = true;
                *exit_code = 1;
            } else if (_game_popup_fields->btn_settings_accept_id == input) {
                exit_loop = true;
                MOX::_screen_data = 0;
            }

            if (exit_loop) {
                if ((uint16_t)(uint8_t)initial_language != _game_popup_fields->language) {
                    MOX::_settings.language = (int8_t)_game_popup_fields->language;
                    if (MOX::_settings.language > 4) {
                        MOX::_settings.language = 0;
                    }
                    LOADSAVE::Reload_Language_Strings_();
                    HAROLD::Write_Language_Setting_File_();
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
                    if (input_id == _game_popup_fields->save_slot_fields[i]) {
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

            if (input_id == _game_popup_fields->btn_main_save_id) { // Save button / Space
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
                Get_Star_Date_Strings_(_game_popup_fields->slot_occupied_flags, (char*)MOX::_save_game_stardates);
                exit_popup = true;
                *out_should_close = 1;
            }

            if (input_id == _game_popup_fields->btn_main_options_id || (input_id < 0 && (active_slot_idx = -input_id) >= 0)) {
                // Potential check against popup field range omitted for clarity as per decompiler
                MOX::_screen_data = 0;
                exit_popup = true;
            } else if (input_id == _game_popup_fields->btn_main_close_id) {
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
            _game_anims->animating = 1;
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
    void __cdecl Get_Star_Date_Strings_(int32_t* dates, char* output_buf) {
        for (int16_t i = 0; i < 10; ++i) {
            char local_str_buf[25];
            memset(local_str_buf, 0, 25);

            int32_t star_date_value = dates[i];

            if (star_date_value > 0) {
                // Fetch the prefix (e.g., "Stardate " or "D ")
                const char* prefix = HAROLD::H_Message_(202);
                strcpy(local_str_buf, prefix);

                char itoa_buf[12];
                __itoa(star_date_value, itoa_buf, 10);

                size_t current_len = strlen(local_str_buf);
                size_t itoa_len = strlen(itoa_buf);

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
                memcpy(local_str_buf, s_INVALID_STR, 12);
            }

            // Copy the resulting string (25 bytes) to the correct slot in the output buffer
            memcpy(output_buf + (i * 25), local_str_buf, 25);
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

    int32_t __cdecl Check_Magic_Number_(const char* filename) {
        int32_t magic_number = 0;
        FILE* file_ptr = fopen(filename, s_rb_0055117c);

        if (file_ptr != nullptr) {
            if (fseek(file_ptr, 0x29, SEEK_SET) == 0) {
                fread(&magic_number, 4, 1, file_ptr);
            }
            fclose(file_ptr);
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
        FILE* file = fopen(filename, s_rb_0055117c);

        if (file != nullptr) {
            uint32_t game_type_header;
            fread(&game_type_header, 4, 1, file);

            if (Is_Valid_Game_Type_(game_type_header)) {
                is_valid = true;
            }

            fclose(file);
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

        if (MOX::_current_screen == SCREEN_MAIN_MENU) {
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

            bool is_valid_slot = (_game_popup_fields->slot_status[i] != 2);
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
                    Get_Game_Popup_Colors_(2, &_game_popup_fields->color_normal, &_game_popup_fields->color_active);
                    s_colors* color = (MOX::_settings.active_save_slot == (char)i) ? &_game_popup_fields->color_normal : &_game_popup_fields->color_active;
                    fonts::Set_Font_Style_(2, color);
                    const char* desc = is_valid_slot ? MOX::_save_game_description[i].description : s_INVALID_STR;
                    fonts::Print_(text_x, text_y, desc);
                }
            } else {
                text_x = 0x20;
                text_y = (i * 31) + 0x18;
                Get_Game_Popup_Colors_(2, &_game_popup_fields->color_normal, &_game_popup_fields->color_active);
                s_colors* color = (MOX::_settings.active_save_slot == (char)i) ? &_game_popup_fields->color_normal : &_game_popup_fields->color_active;
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
            Get_Game_Popup_Colors_(1, &_game_popup_fields->color_normal, &_game_popup_fields->color_active);
            s_colors* color_1 = (MOX::_settings.active_save_slot == (char)i) ? &_game_popup_fields->color_normal : &_game_popup_fields->color_active;
            fonts::Set_Font_Style_(1, color_1);

            if (MOX::_screen_data == 3) text_x += 2;

            if (_print_data_to_bitmap == 0) {
                if (strcmp(_empty_save_slot_message, MOX::_save_game_description[i].description) != 0) {
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

        if (MOX::_current_screen == SCREEN_MAIN_MENU) {
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

        FILE* file = fopen(s_MOX_SET_00556918, s_rb_0055117c);
        if (file != nullptr) {
            fread(file_buffer, 0x229, 1, file);
            fclose(file);

            RUSS::Assert_Settings_();

            // Update the session-related portion of the settings (211 bytes)
            for (int i = 0; i < 211; ++i) {
                file_buffer[i] = reinterpret_cast<uint8_t*>(&MOX::_settings)[i];
            }

            success = true;
            RUSS::Assert_Settings_();

            file = fopen(s_MOX_SET_00556918, s_wb_00551178);
            if (file != nullptr) {
                fwrite(file_buffer, 0x229, 1, file);
                fclose(file);
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
     * @address 00511c50
     */
    void __cdecl Animate_Game_Popup_Pic_(s_animation_header* animation_header) {
        LOADSAVE::Animate_Game_Popup_(animation_header, 0, 1, 0);
    }

    /**
     * @address 00511e80
     */
    void __cdecl Update_Game_Settings_() {
        s_game_popup_fields* fields = LOADSAVE::_game_popup_fields;

        MOX::_settings.end_of_turn_summary = static_cast<int8_t>(fields->options[0]);
        MOX::_settings.end_of_turn_wait = static_cast<int8_t>(fields->options[1]);
        MOX::_settings.enemy_moves = static_cast<int8_t>(fields->options[2]);
        MOX::_settings.expanding_help = static_cast<int8_t>(fields->options[3]);
        MOX::_settings.auto_select_ships = static_cast<int8_t>(fields->options[4]);
        MOX::_settings.animations_on = static_cast<int8_t>(fields->options[5]);
        MOX::_settings.auto_select_colony = static_cast<int8_t>(fields->options[6]);
        MOX::_settings.show_relocation_lines = static_cast<int8_t>(fields->options[7]);
        MOX::_settings.show_gnn_report = static_cast<int8_t>(fields->options[8]);
        MOX::_settings.auto_delete_tg_housing = static_cast<int8_t>(fields->options[9]);
        MOX::_settings.auto_saves = static_cast<int8_t>(fields->options[10]);
        MOX::_settings.only_show_serious_turn_summary_reports = static_cast<int8_t>(fields->options[11]);
        MOX::_settings.ship_initiative = static_cast<int8_t>(fields->options[12]);

        MOX::_settings.language = (int8_t)fields->language;

        if (static_cast<uint8_t>(MOX::_settings.language) >= 5) {
            MOX::_settings.language = 0;
        }
    }

    /**
     * @address 00511c70
     */
    void __cdecl Set_Current_Game_Option_Flags_() {
        // Scaling logic for levels: ((level * 100) / 100) * 155) / 100
        // Simplifies to (level * 155) / 100
        _game_popup_fields->music_level = (int16_t)(((int32_t)MOX::_settings.music_level * 155) / 100);
        _game_popup_fields->sound_fx_level = (int16_t)(((int32_t)MOX::_settings.sound_fx_level * 155) / 100);

        _game_popup_fields->music_on = (int16_t)MOX::_settings.music_on;
        _game_popup_fields->sound_fx_on = (uint16_t)MOX::_settings.sound_fx_on;

        _game_popup_fields->options[0] = (uint16_t)MOX::_settings.end_of_turn_summary;

        if (MOX::_game_type == 0) {
            _game_popup_fields->options[1] = (uint16_t)MOX::_settings.end_of_turn_wait;
        } else {
            _game_popup_fields->options[1] = 0;
        }

        _game_popup_fields->options[2] = (uint16_t)MOX::_settings.enemy_moves;
        _game_popup_fields->options[3] = (uint16_t)MOX::_settings.expanding_help;
        _game_popup_fields->options[4] = (uint16_t)MOX::_settings.auto_select_ships;
        _game_popup_fields->options[5] = (uint16_t)MOX::_settings.animations_on;
        _game_popup_fields->options[6] = (uint16_t)MOX::_settings.auto_select_colony;
        _game_popup_fields->options[7] = (uint16_t)MOX::_settings.show_relocation_lines;
        _game_popup_fields->options[8] = (uint16_t)MOX::_settings.show_gnn_report;
        _game_popup_fields->options[9] = (uint16_t)MOX::_settings.auto_delete_tg_housing;
        _game_popup_fields->options[10] = (uint16_t)MOX::_settings.auto_saves;
        _game_popup_fields->options[11] = (uint16_t)MOX::_settings.only_show_serious_turn_summary_reports;
        _game_popup_fields->options[12] = (uint16_t)MOX::_settings.ship_initiative;

        _game_popup_fields->language = (uint16_t)MOX::_settings.language;
        if (_game_popup_fields->language >= 5) {
            _game_popup_fields->language = 0;
        }
    }

    /**
     * @address 00511930
     */
    void __cdecl Load_Game_Popup_Pictures_() {
        RUSS::Mox_Update_();

        _game_anims->pictures[0] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 0, MOX::_screen_seg);
        _game_anims->pictures[1] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 1, MOX::_screen_seg);
        _game_anims->pictures[2] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 2, MOX::_screen_seg);
        _game_anims->pictures[3] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 3, MOX::_screen_seg);
        _game_anims->pictures[4] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 4, MOX::_screen_seg);
        _game_anims->pictures[5] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 5, MOX::_screen_seg);

        RUSS::Mox_Update_();

        _game_anims->pictures[6] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 6, MOX::_screen_seg);
        _game_anims->pictures[7] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 7, MOX::_screen_seg);
        _game_anims->pictures[8] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 29, MOX::_screen_seg);
        _game_anims->pictures[9] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 9, MOX::_screen_seg);
        _game_anims->pictures[10] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 10, MOX::_screen_seg);

        RUSS::Mox_Update_();

        _game_anims->pictures[11] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 11, MOX::_screen_seg);
        _game_anims->pictures[12] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 12, MOX::_screen_seg);
        _game_anims->pictures[13] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 13, MOX::_screen_seg);
        _game_anims->pictures[14] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 14, MOX::_screen_seg);
        _game_anims->pictures[15] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 15, MOX::_screen_seg);
        _game_anims->pictures[16] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 16, MOX::_screen_seg);

        RUSS::Mox_Update_();

        _game_anims->pictures[17] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 17, MOX::_screen_seg);
        _game_anims->pictures[18] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 18, MOX::_screen_seg);
        _game_anims->pictures[19] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 19, MOX::_screen_seg);
        _game_anims->pictures[23] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 27, MOX::_screen_seg);
        _game_anims->pictures[24] = (s_animation_header*)farload::Far_Reload_Next_(PTR_s_GAME_LBX_0055ac4c, 28, MOX::_screen_seg);

        RUSS::Mox_Update_();

        s_animation_header* first_pic = _game_anims->pictures[0];
        int16_t h = animate::Get_Height_(first_pic) + 1;
        int16_t w = animate::Get_Width_(first_pic) + 1;
        uint32_t size = bitmap::Get_Bitmap_Size_(w, h);

        MAINSCR::_temp_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, size);
    }

    /**
     * @address 005110a0
     */
    void __cdecl Do_Main_Game_Popup_(int16_t* out_finished) {
        bool closed_with_exit = false;
        bool event_processed = false;
        char text_buffer[200];

        fields::Assign_Auto_Function_(LOADSAVE::_Draw_Main_Game_Popup_, 1);
        EVANHELP::Set_Main_Game_Screen_Help_List_();

        while (true) {
            int16_t input = fields::Get_Input_();

            if (input == _game_popup_fields->btn_main_close_id) {
                _draw_game_popup_to_back = 1;
                event_processed = true;
                if (MOX::_screen_data == 0) {
                    *out_finished = 1;
                }
            } else if (input == _game_popup_fields->btn_main_new_id) {
                const char* msg = HAROLD::H_Message_(0xBA); // "QUIT TO MAIN MENU?"
                strcpy(text_buffer, msg);

                if (Game_Popup_User_Box_(text_buffer, 1)) {
                    MOX::_game_type = 0;
                    MOX::_return_screen = SCREEN_NEW_GAME;
                    event_processed = true;
                    *out_finished = 1;
                } else {
                    event_processed = true;
                    video::Set_Page_Back_();
                    graphics::Fill_(0, 0, 639, 479, 0);
                    MAINSCR::Draw_Mini_Main_Screen_();
                    MAINSCR::Draw_Main_Main_Screen_();
                    video::Copy_Back_To_Off_();
                }
            } else if (input == _game_popup_fields->btn_main_quit_id) {
                const char* msg = HAROLD::H_Message_(0xBB); // "QUIT TO WINDOWS?"
                strcpy(text_buffer, msg);

                if (Game_Popup_User_Box_(text_buffer, 1)) {
                    FILEDEF::Save_Game_(9);
                    MOX::_settings.active_save_slot = 10;
                    Save_Session_Related_Settings_();

                    if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                        NETMOX::Human_Abdicates_(static_cast<int32_t>(MOX::_PLAYER_NUM));
                        netcode::Net_Uninit_();
                    }

                    MOX::_return_screen = SCREEN_EXIT;
                    event_processed = true;
                    *out_finished = 1;
                } else {
                    event_processed = true;
                    video::Set_Page_Back_();
                    graphics::Fill_(0, 0, 639, 479, 0);
                    MAINSCR::Draw_Mini_Main_Screen_();
                    MAINSCR::Draw_Main_Main_Screen_();
                    video::Copy_Back_To_Off_();
                }
            } else if (input == _game_popup_fields->btn_main_options_id) {
                _draw_game_popup_to_back = 1;
                MOX::_screen_data = 1;
                event_processed = true;
            } else if (input == _game_popup_fields->btn_main_load_id) {
                event_processed = true;
                fields::Assign_Auto_Function_(LOADSAVE::_Draw_Load_Save_Game_Popup_, 1);
                _draw_game_popup_to_back = 1;
                MOX::_screen_data = 2;
            } else if (input == _game_popup_fields->btn_main_save_id) {
                if (MOX::_disallow_saves_code == 0) {
                    _draw_game_popup_to_back = 1;
                    MOX::_screen_data = 3;
                    event_processed = true;
                } else {
                    const char* msg = HAROLD::H_Message_(0xB9); // "CAN'T SAVE NOW"
                    strcpy(text_buffer, msg);
                    event_processed = true;
                    Game_Popup_User_Box_(text_buffer, 0);
                    *out_finished = 1;
                }
            } else if (input == _music_level_field) {
                Set_Music_For_Game_Popup_();
            } else if (input == _sfx_level_field) {
                Set_Sound_For_Game_Popup_(1);
            }

            if (input == _game_popup_fields->screen_cancel_field_id) {
                closed_with_exit = true;
                event_processed = true;
                *out_finished = 1;
            }

            if (event_processed) {
                if (closed_with_exit && MOX::_settings.animations_on != 0) {
                    fields::Clear_Fields_();
                    Animate_Game_Popup_Pic_Height_(_game_anims->pictures[0], 0);
                    _game_anims->animating = 1;
                }
                return;
            }

            video::Set_Page_Off_();
            LOADSAVE::_Draw_Main_Game_Popup_();
            RUSS::Mox_Sync_Update_();
        }
    }

    /**
     * @address 00510420
     */
    void __cdecl _Draw_Main_Game_Popup_() {
        int16_t input_result = fields::Auto_Input_();

        int16_t x1 = MOX::_popup_base_x + 62;      // 0x3e
        int16_t y1 = MOX::_popup_base_y + 194;     // 0xc2
        int16_t y1_end = MOX::_popup_base_y + 207; // 0xcf
        int16_t x1_end = _game_popup_fields->music_level + x1;

        int16_t x2 = x1;
        int16_t y2 = MOX::_popup_base_y + 216;     // 0xd8
        int16_t y2_end = MOX::_popup_base_y + 229; // 0xe5
        int16_t x2_end = _game_popup_fields->sound_fx_level + x1;

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
     * @address 00510750
     */
    void __cdecl _Draw_Options_Game_Popup_() {
        s_colors local_8;
        MOX::_mox_font_colors_offset = 0x55; // 'U'
        s_colors* palette = MISC::Font_Colors2_(2, 0x55, 0x7E); // 'U', '~'

        // Copy the palette colors locally
        *reinterpret_cast<uint32_t*>(&local_8.colors[0]) = *reinterpret_cast<uint32_t*>(&palette->colors[0]);
        *reinterpret_cast<uint32_t*>(&local_8.colors[4]) = *reinterpret_cast<uint32_t*>(&palette->colors[4]);

        if (_print_data_to_bitmap == 0) {
            if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                Restore_Main_Screen_();
            }
            if (_print_data_to_bitmap == 0) {
                animate::Draw_(MOX::_popup_base_x, MOX::_popup_base_y, _game_anims->pictures[8]);
            }
        }

        fonts::Set_Font_Style_Outline_(2, &local_8);
        fonts::Set_Outline_Color_(0, static_cast<uint16_t>(local_8.colors[0]));
        fonts::Set_Font_Spacing_(1);

        for (int i = 0; i < 13; ++i) {
            int16_t x = MOX::_popup_base_x + 0x1A;
            int16_t y = MOX::_popup_base_y + 0x29 + (i * 0x11);

            animate::Set_Animation_Frame_(_game_anims->pictures[9], _game_popup_fields->options[i]);

            if (_print_data_to_bitmap == 0) {
                animate::Draw_(x, y, _game_anims->pictures[9]);
            } else {
                bitmap::Add_Picture_To_Bitmap_(0, 0, _game_anims->pictures[9], reinterpret_cast<s_animation_header*>(MAINSCR::_temp_bitmap));
                bitmap::Add_Picture_To_Bitmap_(x - MOX::_popup_base_x, y - MOX::_popup_base_y, reinterpret_cast<s_animation_header*>(MAINSCR::_temp_bitmap), _bitmap);
            }
        }

        int16_t draw_x, draw_y;
        if (_print_data_to_bitmap == 0) {
            draw_x = MOX::_popup_base_x + 0x1A;
            draw_y = MOX::_popup_base_y + 0x29;
        } else {
            draw_x = 0x1A;
            draw_y = 0x29;
        }

        if (_print_data_to_bitmap != 0) {
            int16_t btn_width = animate::Get_Width_(_game_anims->pictures[9]);
            int16_t text_x1 = draw_x + 4 + btn_width;
            int16_t text_x2 = text_x1 + 0x96;

            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x02, HAROLD::H_Message_(0xA6), _bitmap);
            fonts::Print_To_Bitmap_(text_x2, draw_y + 0x02, s__ALT_F1__0055ad24, _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x13, HAROLD::H_Message_(0xA7), _bitmap);
            fonts::Print_To_Bitmap_(text_x2, draw_y + 0x13, s__ALT_F2__0055ad18, _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x24, HAROLD::H_Message_(0xA8), _bitmap);
            fonts::Print_To_Bitmap_(text_x2, draw_y + 0x24, s__ALT_F3__0055ad0c, _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x35, HAROLD::H_Message_(0xA9), _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x46, HAROLD::H_Message_(0xAA), _bitmap);
            fonts::Print_To_Bitmap_(text_x2, draw_y + 0x46, s__ALT_F5__0055ad00, _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x57, HAROLD::H_Message_(0xAB), _bitmap);
            fonts::Print_To_Bitmap_(text_x2, draw_y + 0x57, s__ALT_F6__0055acf4, _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x68, HAROLD::H_Message_(0xAC), _bitmap);
            fonts::Print_To_Bitmap_(text_x2, draw_y + 0x68, s__ALT_F7__0055ace8, _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x79, HAROLD::H_Message_(0xAD), _bitmap);
            fonts::Print_To_Bitmap_(text_x2, draw_y + 0x79, s__ALT_F8__0055addc, _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x8A, HAROLD::H_Message_(0xAE), _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0x9B, HAROLD::H_Message_(0xAF), _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0xAC, HAROLD::H_Message_(0xB0), _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0xBD, HAROLD::H_Message_(0xB1), _bitmap);
            fonts::Print_To_Bitmap_(text_x1, draw_y + 0xCE, HAROLD::H_Message_(0x187), _bitmap);

            for (int i = 0; i < 13; ++i) {
                animate::Set_Animation_Frame_(_game_anims->pictures[9], _game_popup_fields->options[i]);
                bitmap::Add_Picture_To_Bitmap_(0x1A, draw_y + (i * 0x11), _game_anims->pictures[9], _bitmap);
            }
        } else {
            int16_t btn_width = animate::Get_Width_(_game_anims->pictures[9]);
            int16_t text_x1 = draw_x + 4 + btn_width;
            int16_t text_x2 = text_x1 + 0x96;

            fonts::Print_(text_x1, draw_y + 0x02, HAROLD::H_Message_(0xA6));
            fonts::Print_(text_x2, draw_y + 0x02, s__ALT_F1__0055acd0);
            fonts::Print_(text_x1, draw_y + 0x13, HAROLD::H_Message_(0xA7));
            fonts::Print_(text_x2, draw_y + 0x13, s__ALT_F2__0055acc4);
            fonts::Print_(text_x1, draw_y + 0x24, HAROLD::H_Message_(0xA8));
            fonts::Print_(text_x2, draw_y + 0x24, s__ALT_F3__0055acb8);
            fonts::Print_(text_x1, draw_y + 0x35, HAROLD::H_Message_(0xA9));
            fonts::Print_(text_x1, draw_y + 0x46, HAROLD::H_Message_(0xAA));
            fonts::Print_(text_x2, draw_y + 0x46, s__ALT_F4__0055acac);
            fonts::Print_(text_x1, draw_y + 0x57, HAROLD::H_Message_(0xAB));
            fonts::Print_(text_x2, draw_y + 0x57, s__ALT_F5__0055aca0);
            fonts::Print_(text_x1, draw_y + 0x68, HAROLD::H_Message_(0xAC));
            fonts::Print_(text_x2, draw_y + 0x68, s__ALT_F6__0055ac94);
            fonts::Print_(text_x1, draw_y + 0x79, HAROLD::H_Message_(0xAD));
            fonts::Print_(text_x2, draw_y + 0x79, s__ALT_F7__0055ac88);
            fonts::Print_(text_x1, draw_y + 0x8A, HAROLD::H_Message_(0xAE));
            fonts::Print_(text_x2, draw_y + 0x8A, s__ALT_F8__0055ac7c);
            fonts::Print_(text_x1, draw_y + 0x9B, HAROLD::H_Message_(0xAF));
            fonts::Print_(text_x1, draw_y + 0xAC, HAROLD::H_Message_(0xB0));
            fonts::Print_(text_x1, draw_y + 0xBD, HAROLD::H_Message_(0xB1));
            fonts::Print_(text_x1, draw_y + 0xCE, HAROLD::H_Message_(0x187));
        }
    }

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
                MOX::_return_screen = static_cast<SCREEN>(static_cast<int8_t>(MOX::_previous_screen));
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

        if (MOX::_settings.animations_on != 0 && _game_anims->animating == 0 && _loaded_debug_save == 0) {
            _dont_draw_darkened_buttons = 1;
            video::Set_Page_Off_();
            _Draw_Main_Game_Popup_();
            RUSS::Mox_Sync_Update_();
            timer::Mark_Time_();
            timer::Release_Time_(7);
            video::Set_Page_Off_();
            Animate_Game_Popup_Pic_Height_(_game_anims->pictures[0], 0);
        }

        if (MOX::_current_screen == SCREEN_NEW_GAME) {
            ERIC::Fast_Fade_Out_();
        } else if (_loaded_new_game == 0) {
            fields::Clear_Fields_();
            fields::Disable_Draw_All_Fields_();
            MOX::_draw_main_screen_to_back = 1;
            MAINSCR::Allocate_First_Screen_Seg_(5);
            video::Set_Page_Off_();
            MAINSCR::Draw_Mini_Main_Screen_();
            RUSS::Mox_Sync_Update_();
            fields::Enable_Draw_All_Fields_();
        }
    }

    /**
     * @address 005123d0
     */
    void __cdecl Auto_Save_Last_Saved_Game_(int16_t* result_flag) {
        int32_t save_slots[10];
        char message_buffer[0xC8];

        FILEDEF::Get_Saved_Game_Descriptions_(save_slots);

        int save_slot = MOX::_settings.active_save_slot;
        s_save_game_description* save_description = &MOX::_save_game_description[save_slot];
        const char* message_format = HAROLD::H_Message_(0xCD, save_slot, save_description);

        _sprintf(message_buffer, message_format);

        int16_t choice = HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, message_buffer, 1);
        if (choice != 0) {
            FILEDEF::Load_Game_(save_slot);
            MOX::_return_screen = SCREEN_HOTSEAT_SELECT_PLAYER;
            if (MOX::_game_type != 1) {
                MOX::_return_screen = SCREEN_REPORTS;
            }
            *result_flag = 1;
            return;
        }

        MOX::_FULL_DRAW_SCREEN_FLAG = 1;
        *result_flag = 0;
    }

    /**
     * @address 00512290
     */
    void __cdecl Embed_Special_Color_Codes_(char* param_1, int16_t param_2) {
        int16_t length = static_cast<int16_t>(strlen(param_1));
        char encoded[202];

        memset(encoded, 0, sizeof(encoded) - 1);

        if (length <= 0xC8 && length < param_2 - 2 && *param_1 != '\x03') {
            encoded[0] = '\x03';
            memcpy(encoded + 1, param_1, length);
            encoded[length + 1] = '\x01';
            memcpy(param_1, encoded, length + 2);
        }
    }

    /**
     * @address 00512230
     */
    void __cdecl Remove_Embedded_Special_Codes_(char* text, int max_len) {
        if (*text == '\x03') {
            char special_code[2] = { '\x01', '\0' };
            int16_t copy_length = static_cast<int16_t>(strlen(text));
            memmove(text, text + 1, static_cast<size_t>(copy_length));
            int16_t special_code_pos = MISC::Substring_(text, special_code);
            text[special_code_pos] = '\0';
        }
    }

    /**
     * @address 005124f0
     */
    void __cdecl Set_Music_For_Game_Popup_() {
        short music_volume = static_cast<short>((static_cast<int32_t>(_game_popup_fields->music_level) * 100) / 0x9B);

        if (music_volume <= 5) {
            music_volume = 0;
            sound::Stop_Stream_();
            JIM::Stop_Background_Music_();
            MOX::_settings.music_on = 0;
            _game_popup_fields->music_on = 0;
        } else {
            _game_popup_fields->music_on = 1;
            if (MOX::_settings.music_on == 0) {
                sound::Stream_On_();
                MOX::_settings.music_on = 1;
                JIM::Play_Background_Music_();
            }
        }

        MOX::_settings.music_level = static_cast<int8_t>(music_volume);
        sound::Set_Stream_Max_Volume_(music_volume);
        sound::Set_Stream_Volume_(music_volume);
    }

    /**
     * @address 00512590
     */
    void __cdecl Set_Sound_For_Game_Popup_(char play_sound_effect) {
        s_game_popup_fields* popup = LOADSAVE::_game_popup_fields;

        MOX::_global_var = 0;

        int32_t volume = popup->sound_fx_level;
        volume = (volume * 100) / 0x9b;
        volume *= 4;
        volume = (volume * 100) / 100;
        MOX::_settings.sound_fx_level = static_cast<int8_t>(volume);

        if (MOX::_settings.sound_fx_level > 5) {
            if (MOX::_settings.sound_fx_on == 0) {
                sound::Sound_On_();
            }

            MOX::_settings.sound_fx_on = 1;
            sound::Set_Sound_Max_Volume_(MOX::_settings.sound_fx_level);

            if (play_sound_effect != 0) {
                sound::Play_Sound_(0x21);
            }

            popup->sound_fx_on = 1;
            return;
        }

        MOX::_settings.sound_fx_level = 0;
        sound::Set_Sound_Max_Volume_(0);
        MOX::_settings.sound_fx_on = 0;
        popup->sound_fx_on = 0;
    }

    /**
     * @address 004f2470
     */
    /**
     * @address 0050f650
     */
    void __cdecl Get_File_Date_String_(const char* file_path, char* out_string) {
        FILE* file = fopen(file_path, s_r_00556db0);
        if (file == nullptr) {
            return;
        }

        fclose(file);

        int16_t second;
        int16_t minute;
        int16_t hour;
        int16_t year;
        int16_t day;
        int16_t month;

        MISC::Get_Time_Stamp_(file_path, &month, &day, &year, &hour, &minute, &second);

        if ((((month == 0) && (day == 0)) && (year == 0)) && ((hour == 0) && (second == 0))) {
            out_string[0] = '\0';
            return;
        }

        _sprintf(
                out_string,
                s__3s__2d____4d__2d__2d_0055ac64,
                LOADSAVE::_months[month],
                static_cast<int>(day),
                static_cast<int>(year),
                static_cast<int>(hour),
                static_cast<int>(minute));

        if (out_string[0x0e] == ' ') {
            out_string[0x0e] = '0';
        }
        if (out_string[0x11] == ' ') {
            out_string[0x11] = '0';
        }
    }

    void __cdecl Reload_Language_Strings_() {
        MOX::_settings.language = 0;
        HAROLD::Load_H_Strings_();
        RUSS::Load_Russ_Strings_();
        ESTRINGS::Load_E_Strings_();
        HAROLD::Load_Language_Fonts_();
        HAROLD::Load_Leader_Titles();
        HAROLD::Set_Language_Race_Names_();
        TECHINIT::Load_Tech_Names_();
        INITSHIP::Update_Ship_Names_To_New_Language_();
    }

    void __cdecl Auto_Load_Last_Saved_Game_(int16_t* out_flag) {
        int32_t save_descriptions_buffer[10];
        char text_buffer[200];

        FILEDEF::Get_Saved_Game_Descriptions_(save_descriptions_buffer);

        int32_t slot = MOX::_settings.active_save_slot;
        const char* format_str = HAROLD::H_Message_(0xcc);

        _sprintf(text_buffer, format_str, slot, MOX::_save_game_description[slot].description);

        int16_t box_res = HAROLD::User_Box_(&MOX::_draw_main_screen_to_back, text_buffer, 1);

        if (box_res != 0) {
            FILEDEF::Save_Game_(static_cast<int16_t>(MOX::_settings.active_save_slot));
            MOX::_return_screen = MOX::_current_screen;
            MAINSCR2::_force_main_screen_fade = 1;
            *out_flag = 1;
        } else {
            MOX::_FULL_DRAW_SCREEN_FLAG = 1;
            *out_flag = 0;
        }
    }
}