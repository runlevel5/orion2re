#include "deps.h"
#include <cstring>

namespace LOADSAVE {
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
     */
    bool __cdecl Game_Popup_User_Box_(char* text, int16_t box_type) {
        bool result = false;

        switch (box_type) {
        case 0:
            video::Set_Refresh_Full_();
            _draw_game_popup_to_back = 1;
            GENDRAW::Message_Box_(text);
            fields::Clear_Fields_();
            Restore_Game_Popup_();
            break;

        case 1:
            video::Set_Refresh_Full_();
            _draw_game_popup_to_back = 1;
            result = GENDRAW::Warning_Box_00479700(text);
            fields::Clear_Fields_();
            Restore_Game_Popup_();
            break;

        case 3:
            video::Set_Refresh_Full_();
            _draw_game_popup_to_back = 1;
            GENDRAW::Warning_Box_004796e0(text);
            fields::Clear_Fields_();
            Restore_Game_Popup_();
            break;

        default:
            // Do nothing
            break;
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

                _game_popup_fields->field445_0x244[i] = fields::Add_Hidden_Field_(MOX::_popup_base_x + 0x31, by, MOX::_popup_base_x + 0xf3, animate::Get_Height_(pic) + by, s__00551100, ')');
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
            int colors_style[2], colors_special[2];
            Get_Game_Popup_Colors_(2, colors_style, colors_special);
            fonts::Set_Font_Special_Colors_(reinterpret_cast<uint8_t*>(colors_special));

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
                    fonts::Set_Font_Style_(2, reinterpret_cast<MISC::s_colors*>(colors_style));
                    if (_game_popup_fields->saved_games[i] == 2) {
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58);
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description + 4) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58 + 4);
                        *reinterpret_cast<uint32_t*>(MOX::_save_game_description[i].description + 8) = *reinterpret_cast<const uint32_t*>(s___INVALID___0055ac58 + 8);
                    }
                    _game_popup_fields->saved_game_types[i] = FILEDEF::Get_Game_Type_From_Saved_Game_(i);
                    _game_popup_fields->saved_game_fields[i] = (int16_t)fields::Add_Continuous_String_Input_Field_(MOX::_popup_base_x + 0x1a, MOX::_popup_base_y + 0x18 + i * 0x1f, 200, fonts::Get_Font_Height_(), &MOX::_save_game_description[i], 0x1e, 0, 0, ')', 0);
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
        int16_t status = 0;
        int8_t loaded_success = 0;
        char msg_buffer[80];
        int16_t unused1, unused2;

        Load_Month_Names_();

        const char* empty_msg = HAROLD::H_Message_(0x184);
        int32_t len = 0;
        while (empty_msg[len] != '\0') len++;
        for (int i = 0; i <= len; i++) {
            _empty_save_slot_message[i] = empty_msg[i];
        }

        bool not_main_menu = (MOX::_current_screen != MOX::SCREEN_MAIN_MENU);
        _loaded_debug_save = 0;

        if (not_main_menu) {
            EVANHELP::Set_Load_Game_Screen_Help_List_();
            fields::Clear_Fields_();
            MOX::_global_var = (int8_t)fields::Add_Hidden_Field_(0, 0, 0x27F, 0x1DF, s__00551120, 0x29); // 0x29 = ')'
        }

        fields::Assign_Auto_Function_(_Draw_Load_Save_Game_Popup_, 1);
        fields::Clear_Fields_();
        Add_Game_Popup_Fields_(&unused1, &unused2);
        MOX::_global_var = 0;

        while (true) {
            int32_t input = fields::Get_Input_();
            int16_t clicked_slot = -1;

            for (int16_t i = 0; i < 10; i++) {
                if (_game_popup_fields->saved_game_fields[i] == (int16_t)input) {
                    clicked_slot = i;
                    break;
                }
            }

            if (_game_popup_fields->field_l == (int16_t)input) {
                if (clicked_slot == -1) {
                    uint8_t active_slot = (uint8_t)MOX::_settings.active_save_slot;
                    if (active_slot < 10) {
                        _draw_down = 1;
                        clicked_slot = (int16_t)active_slot;
                    }
                }
            }

            if (clicked_slot >= 0 && clicked_slot < 10) {
                if (_game_popup_fields->saved_games[clicked_slot] == 0) {
                    if (MOX::_settings.animations_on && MOX::_previous_screen == MOX::SCREEN_MAIN) {
                        fields::Clear_Fields_();
                        _game_anims->field1_0x64 = 1;
                        video::Set_Page_Off_();
                        Animate_Game_Popup_(_game_anims->pictures[11], 1);
                    }

                    loaded_success = (int8_t)FILEDEF::Load_Game_(clicked_slot);
                    MOX::_settings.active_save_slot = (char)clicked_slot;
                    Save_Session_Related_Settings_();

                    if (loaded_success == 0) {
                        const char* load_err_msg = HAROLD::H_Message_(0xB8);
                        _sprintf(msg_buffer, load_err_msg, (int32_t)clicked_slot);
                        Game_Popup_User_Box_(msg_buffer, 3);
                        MOX::_return_screen = (uint16_t)MOX::_previous_screen;
                    } else {
                        _loaded_new_game = 1;
                        MOX::_return_screen = (MOX::_game_type == 1) ? 0x11 : 0x27;
                    }

                    if (not_main_menu && MOX::_stardate_at_beg_of_turn != 0) {
                        MISC::Fade_Into_Screen_(MAINSCR::Draw_Mini_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
                    }

                    status = 1;
                    *exit_flag = 1;
                } else {
                    Bad_Game_File_Message_((uint16_t)_game_popup_fields->saved_games[clicked_slot], clicked_slot);
                    status = 2;
                }
            }

            if (_game_popup_fields->field_o == (int16_t)input) {
                MOX::_screen_data = 0;
                status = 1;
                if (MOX::_current_screen == MOX::SCREEN_MAIN_MENU) {
                    *exit_flag = 1;
                }
            } else if (_game_popup_fields->field_esc == (int16_t)input) {
                loaded_success = 0;
                status = 1;
                *exit_flag = 1;
            }

            if (status != 0) break;

            video::Set_Page_Off_();
            fields::Invoke_Auto_Function_();
            RUSS::Mox_Sync_Update_();
        }

        for (int i = 0; i < 10; i++) {
            Remove_Embedded_Special_Codes_(MOX::_save_game_description[i].description);
        }

        return loaded_success;
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

            int32_t star_date_value = fields->values[i];

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
}