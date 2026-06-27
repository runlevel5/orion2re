#include "pch.h"

namespace namestar {

s_input_box_state _input_box;

int16_t __cdecl Star_Name_Popup_Screen_Center_X_() {
    return 0xb1;
}

int16_t __cdecl Star_Name_Popup_Screen_Center_Y_() {
    return 0x7D;
}

int16_t __cdecl Star_Name_Popup_Map_Center_X_() {
    return 0x83;
}

int16_t __cdecl Star_Name_Popup_Map_Center_Y_() {
    return 0x92;
}

void __cdecl Setup_Input_Box_Popup_() {
    fields::Clear_Fields_();
    
    allocate::Push_Block_(MOX::_screen_seg);
    
    _input_box.bg = (s_animation_header*)farload::Far_Reload_Next_("inbox.lbx", 0, MOX::_screen_seg);
    _input_box.pic = (s_animation_header*)farload::Far_Reload_Next_("inbox.lbx", 1, MOX::_screen_seg);
    
    video::Set_Page_Off_();
    fields::Invoke_Saved_Auto_Function_();
    video::Copy_Off_To_Back_();
    RUSS::Mox_Sync_Update_();
    video::Set_Refresh_Stencil_();
    video::Set_Page_Off_();
    
    int16_t animate_width = animate::Get_Width_(_input_box.bg);
    animate::Get_Height_(_input_box.bg);
    
    if (_input_box.show_cancel_anim != 0) {
        remap::Calculate_Picture_Remap_Colors_(_input_box.bg);
        remap::Calculate_Picture_Remap_Colors_(_input_box.pic);
        animate::Remap_Draw_(_input_box.x, _input_box.y, _input_box.bg);
        animate::Reset_Animation_Frame_(_input_box.pic);
        animate::Remap_Draw_(_input_box.cancel_x, _input_box.cancel_y, _input_box.pic);
    } else {
        animate::Draw_(_input_box.x, _input_box.y, _input_box.bg);
        _input_box.colors2 = _input_box.font_colors;
    }
    
    MISC::Set_Fitted_Font_Style_(_input_box.prompt_text, &_input_box.font_colors, 4, 2, 0, animate_width - 54, 1);
    
    int16_t font_style = MISC::Get_Font_Style_();
    int16_t font_spacing = MISC::Get_Font_Spacing_();
    
    if (MOX::_current_screen == SCREEN_NEW_GAME) {
        MOX::_mox_font_colors_offset = 20;
        s_colors* new_colors = MISC::Font_Colors2_(font_style, 10, 26);
        _input_box.colors2 = *new_colors;
    } else {
        MOX::_mox_font_colors_offset = 19;
        s_colors* new_colors = MISC::Font_Colors2_(font_style, 12, 24);
        _input_box.colors2 = *new_colors;
    }
    
    if (_input_box.show_cancel_anim == 0) {
        fonts::Set_Font_Style_(font_style, &_input_box.colors2);
    } else {
        fonts::Set_Remap_Font_Style_(4, &_input_box.colors2);
    }
    
    fonts::Set_Font_Spacing_(font_spacing);
    
    int16_t string_width = fonts::Get_String_Width_(_input_box.prompt_text);
    int16_t font_height = fonts::Get_Font_Height_();
    
    int16_t text_x = _input_box.x + (animate_width - string_width) / 2;
    int16_t text_y = (_input_box.bg_height - font_height) / 2 + _input_box.prompt_y;
    
    fonts::Print_(text_x, text_y, _input_box.prompt_text);
    
    video::Copy_Off_To_Back_();
    RUSS::Mox_Sync_Update_();
    video::Set_Page_Off_();
    
    _input_box.colors2.colors[0] = 1;
    
    if (_input_box.show_cancel_anim == 0) {
        fonts::Set_Font_Style_(3, &_input_box.font_colors);
    } else {
        fonts::Set_Remap_Font_Style_(3, &_input_box.font_colors);
    }
    
    if (_input_box.show_cancel_anim == 0) {
        _input_box.cancel_field_id = fields::Add_Button_Field_(
            _input_box.cancel_x,
            _input_box.cancel_y,
            MOX::s__00551100,
            _input_box.pic,
            MOX::s__00551100,
            40
        );
    } else {
        int16_t pic_height = animate::Get_Height_(_input_box.pic);
        int16_t pic_width = animate::Get_Width_(_input_box.pic);
        
        _input_box.cancel_field_id = fields::Add_Hidden_Field_(
            _input_box.cancel_x,
            _input_box.cancel_y,
            _input_box.cancel_x + pic_width,
            _input_box.cancel_y + pic_height,
            MOX::s__00551100,
            40
        );
    }
    
    _input_box.accept_field_id = fields::Add_Continuous_String_Input_Field_(
        _input_box.input_x,
        _input_box.input_y,
        _input_box.max_input_length,
        _input_box.unk1,
        _input_box.input_text,
        _input_box.input_type,
        0,
        0,
        41,
        0
    );
    
    fields::_active_input_field_number = _input_box.accept_field_id;
    fields::_input_field_active = 1;
    fields::_cursor_position = 0;
    fields::_cursor_on = 0;
    
    
    strlcpy(fields::_continuous_string, _input_box.input_text, sizeof(fields::_continuous_string));
    MOX::_FULL_DRAW_SCREEN_FLAG = 0;
    fields::_active_input_field_first = 1;
    fields::Assign_Auto_Function_(Draw_Input_Box_Popup_, 1);
}

void __cdecl Draw_Input_Box_Popup_() {
    if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
        allocate::Pop_Block_(MOX::_screen_seg);
        Setup_Input_Box_Popup_();
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;
    }
    RUSS::Mox_Update_();
}

void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, int8_t use_animated_popup) {
    char* prompt_text = (char*)HAROLD::H_Message_(HESTR_100_ENTER_STAR_NAME);
    Do_Change_Star_Name_(x, y, star_idx, use_animated_popup, prompt_text);
}

void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx) {
    char* prompt_text = (char*)HAROLD::H_Message_(HESTR_101_ENTER_HOME_STAR_NAME);
    Do_Change_Star_Name_(x, y, star_idx, 0, prompt_text);
}

int16_t __cdecl Max_Pixel_Width_Star_Name_Can_Be_(int16_t star_idx) {
    int16_t target_x;
    int16_t target_y;
    int16_t target_w;
    MAINSCR::Get_Star_Name_Info_(star_idx, &target_x, &target_y, &target_w);

    int16_t font_h = fonts::Get_Font_Height_() + 2;

    int16_t dist_left = 999;
    int16_t dist_right = 999;
    int16_t left_star_idx = -1;
    int16_t right_star_idx = -1;
    int16_t left_star_width = 0;

    for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
        if (i != star_idx) {
            int16_t i_x;
            int16_t i_y;
            int16_t i_w;
            MAINSCR::Get_Star_Name_Info_(i, &i_x, &i_y, &i_w);

            int32_t y_diff = (int32_t)i_y - (int32_t)target_y;
            if (abs(y_diff) < font_h) {
                int32_t x_diff = (int32_t)i_x - (int32_t)target_x;
                if (x_diff < 0) {
                    int32_t abs_x_diff = abs(x_diff);
                    if (abs_x_diff < dist_left) {
                        dist_left = (int16_t)abs_x_diff;
                        left_star_width = i_w;
                        left_star_idx = i;
                    }
                } else {
                    int32_t abs_x_diff = abs(x_diff);
                    if (abs_x_diff < dist_right) {
                        dist_right = (int16_t)abs_x_diff;
                        right_star_idx = i;
                    }
                }
            }
        }

        if (left_star_idx == -1) {
            left_star_width = 0;
            dist_left = (int16_t)abs((int32_t)target_x - 21);
        }

        if (right_star_idx == -1) {
            dist_right = (int16_t)abs((int32_t)target_x - 527);
            if (dist_right < target_w) {
                dist_right = target_w;
            }
        }
    }

    if (dist_right < target_w) {
        dist_right = target_w;
    }

    int16_t left_space = dist_left - left_star_width;
    int16_t right_space = dist_right - target_w;

    int16_t min_space = left_space;
    if (right_space < min_space) {
        min_space = right_space;
    }

    int16_t max_w = min_space * 2 + target_w;
    if (max_w < target_w) {
        max_w = target_w + 5;
    }

    return max_w;
}

int8_t __cdecl Do_Change_Star_Name_(int16_t x, int16_t y, int16_t star_idx, int8_t use_animated_popup, char* prompt_text) {
    s_colors colors;
    char backup_name[20];

    fonts::Set_Font_Style_(3, &colors);

    int16_t current_name_width = fonts::Get_String_Width_(MOX::_star[star_idx].name);
    int16_t max_placeholder_width = fonts::Get_String_Width_("WWWWWWW");
    int16_t max_name_width = Max_Pixel_Width_Star_Name_Can_Be_(star_idx);

    if (current_name_width > max_name_width) {
        max_name_width = current_name_width;
    }

    if (max_name_width > max_placeholder_width) {
        max_name_width = max_placeholder_width;
    }

    if (max_name_width > 0xCD) {
        max_name_width = 0xCD;
    }

    strlcpy(backup_name, MOX::_star[star_idx].name, sizeof(backup_name));

    int8_t result;
    if (use_animated_popup != 0) {
        result = Input_Box_Popup_Animated_(MOX::_star[star_idx].name, 15, max_name_width, prompt_text, x, y);
    } else {
        result = Input_Box_Popup_(MOX::_star[star_idx].name, 15, max_name_width, prompt_text, x, y, 0);
    }

    if (MOX::_star[star_idx].name[0] == '\0') {
        strlcpy(MOX::_star[star_idx].name, backup_name, sizeof(MOX::_star[star_idx].name));
    }

    fonts::Get_String_Width_(MOX::_star[star_idx].name);

    return result;
}

int8_t __cdecl Can_Change_Star_Name_(int16_t star_idx) {
    int8_t can_change = 1;
    bool found = false;
    int16_t i = 0;

    do {
        if (found) {
            break;
        }

        int16_t planet_idx = MOX::_star[star_idx].planet_index[i];
        if (planet_idx > -1) {
            if (MOX::_planet[planet_idx].colony_index > -1) {
                can_change = 0;
                found = true;
            }
        }

        i++;
    } while (i < 5);

    return can_change;
}

int8_t __cdecl Input_Box_Popup_(char* input_text, int16_t input_type, int16_t max_length, char* prompt_text, int16_t x, int16_t y, int8_t show_cancel_anim) {
    EVANHELP::Save_Help_List_();
    fields::Save_Auto_Function_();
    EVANHELP::Set_Input_Box_Help_List_();

    memset(&_input_box, 0, sizeof(s_input_box_state));

    if (strlen(input_text) < sizeof(_input_box.input_text)) {
        strlcpy(_input_box.input_text, input_text, sizeof(_input_box.input_text));
    } else {
        strncpy(_input_box.input_text, input_text, sizeof(_input_box.input_text));
        _input_box.input_text[sizeof(_input_box.input_text) - 1] = '\0';
    }

    if (strlen(prompt_text) < sizeof(_input_box.prompt_text)) {
        strlcpy(_input_box.prompt_text, prompt_text, sizeof(_input_box.prompt_text));
    } else {
        strncpy(_input_box.prompt_text, prompt_text, sizeof(_input_box.prompt_text));
        _input_box.prompt_text[sizeof(_input_box.prompt_text) - 1] = '\0';
    }

    _input_box.bg_height = 0x36;
    _input_box.input_type = input_type;
    _input_box.prompt_y = y + 3;
    _input_box.show_cancel_anim = show_cancel_anim;
    _input_box.cancel_x = x + 0x60;
    _input_box.cancel_y = y + 0x64;
    _input_box.input_x = x + 0x22;
    _input_box.input_y = y + 0x36;

    if (max_length == 0) {
        _input_box.max_input_length = 205;
    } else {
        if (max_length >= 205) {
            _input_box.max_input_length = 205;
        } else {
            _input_box.max_input_length = max_length;
        }
    }

    _input_box.unk1 = 26;
    _input_box.y = y;
    _input_box.x = x;
    _input_box.max_length = max_length;

    s_colors* colors2;
    if (MOX::_current_screen == SCREEN_NEW_GAME) {
        MOX::_mox_font_colors_offset = 0x14;
        colors2 = MISC::Font_Colors2_(3, 0, 0x14);
    } else {
        MOX::_mox_font_colors_offset = 0x10;
        colors2 = MISC::Font_Colors2_(3, 0x0C, 0x18);
    }

    _input_box.font_colors = *colors2;

    char original_input_text[sizeof(_input_box.input_text)];
    if (strlen(_input_box.input_text) < sizeof(original_input_text)) {
        strlcpy(original_input_text, _input_box.input_text, sizeof(original_input_text));
    } else {
        strncpy(original_input_text, _input_box.input_text, sizeof(original_input_text));
        original_input_text[sizeof(original_input_text) - 1] = '\0';
    }

    Setup_Input_Box_Popup_();

    int8_t accepted = 1;
    int16_t popup_action = 0;
    int16_t accept_count = 1;

    do {
        popup_action = 0;
        int16_t input = fields::Get_Input_();

        if (input == _input_box.accept_field_id) {
            if (accept_count > 0) {
                popup_action = 1;
            }
            accept_count++;
        }

        if (input < 0) {
            popup_action = 1;
            strlcpy(_input_box.input_text, original_input_text, sizeof(_input_box.input_text));
            accepted = 0;
        }

        if (input == _input_box.cancel_field_id) {
            popup_action = 2;
        }

        if (popup_action == 0) {
            video::Set_Page_Off_();
            Draw_Input_Box_Popup_();
            RUSS::Mox_Sync_Update_();
        } else if (popup_action == 2) {
            if (show_cancel_anim != 0) {
                video::Set_Page_Off_();
                animate::Set_Animation_Frame_(_input_box.pic, 1);
                animate::Remap_Draw_(_input_box.cancel_x, _input_box.cancel_y, _input_box.pic);
                Draw_Input_Box_Popup_();
                RUSS::Mox_Sync_Update_();
            }
        }
    } while (popup_action == 0);

    strings::Trim_(_input_box.input_text);
    strcpy(input_text, _input_box.input_text);

    allocate::Pop_Block_(MOX::_screen_seg);
    fields::Clear_Fields_();
    fields::Deactivate_Auto_Function_();
    EVANHELP::Restore_Help_List_();
    fields::Restore_Auto_Function_();
    MOX::_FULL_DRAW_SCREEN_FLAG = 1;

    return accepted;
}

int8_t __cdecl Input_Box_Popup_Animated_(char* input_text, int16_t input_type, int16_t max_length, char* prompt_text, int16_t x, int16_t y) {
    return Input_Box_Popup_(input_text, input_type, max_length, prompt_text, x, y, 1);
}

} // namespace namestar