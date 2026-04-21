#include "pch.h"

namespace namestar {

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
    
    _input_box.bg = (s_animation_header*)farload::Far_Reload_Next_(s_inbox_lbx_00551370, 0, MOX::_screen_seg);
    _input_box.pic = (s_animation_header*)farload::Far_Reload_Next_(s_inbox_lbx_00551370, 1, MOX::_screen_seg);
    
    video::Set_Page_Off_();
    fields::Invoke_Saved_Auto_Function_();
    video::Copy_Off_To_Back_();
    RUSS::Mox_Sync_Update_();
    video::Set_Refresh_Stencil_();
    video::Set_Page_Off_();
    
    int16_t animate_width = animate::Get_Width_(_input_box.bg);
    animate::Get_Height_(_input_box.bg);
    
    if (_input_box.show_cancel_anim == 0) {
        animate::Draw_(_input_box.y, _input_box.x, _input_box.bg);
        _input_box.colors2 = _input_box.font_colors;
    } else {
        remap::Calculate_Picture_Remap_Colors_(_input_box.bg);
        remap::Calculate_Picture_Remap_Colors_(_input_box.pic);
        animate::Remap_Draw_(_input_box.y, _input_box.x, _input_box.bg);
        animate::Reset_Animation_Frame_(_input_box.pic);
        animate::Remap_Draw_(_input_box.cancel_y, _input_box.cancel_x, _input_box.pic);
    }
    
    MISC::Set_Fitted_Font_Style_(_input_box.prompt_text, &_input_box.font_colors, 4, 2, 0, animate_width - 54, 1);
    
    int16_t font_style = MISC::Get_Font_Style_();
    int16_t font_spacing = MISC::Get_Font_Spacing_();
    
    if (MOX::_current_screen == 13) {
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
    
    int16_t text_x = _input_box.y + (animate_width - string_width) / 2;
    int16_t text_y = _input_box.x_offset1 + (_input_box.bg_width - font_height) / 2;
    
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
            _input_box.cancel_y,
            _input_box.cancel_x,
            MOX::s__00551100,
            _input_box.pic,
            MOX::s__00551100,
            40
        );
    } else {
        int16_t pic_height = animate::Get_Height_(_input_box.pic);
        int16_t pic_width = animate::Get_Width_(_input_box.pic);
        
        _input_box.cancel_field_id = fields::Add_Hidden_Field_(
            _input_box.cancel_y,
            _input_box.cancel_x,
            _input_box.cancel_y + pic_width,
            _input_box.cancel_x + pic_height,
            MOX::s__00551100,
            40
        );
    }
    
    _input_box.accept_field_id = fields::Add_Continuous_String_Input_Field_(
        _input_box.y_offset2,
        _input_box.x_offset3,
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
    
    strcpy(reinterpret_cast<char*>(&fields::_continuous_string), _input_box.input_text);
    
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

void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, int8_t mode) {
    char* message = const_cast<char*>(HAROLD::H_Message_(HESTR_100_ENTER_STAR_NAME));
    Do_Change_Star_Name_(x, y, star_idx, mode, message);
}

void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx) {
    char* title = const_cast<char*>(HAROLD::H_Message_(HESTR_101_ENTER_HOME_STAR_NAME));
    Do_Change_Star_Name_(x, y, star_idx, 0, title);
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

            if (abs(i_y - target_y) < font_h) {
                int16_t x_diff = i_x - target_x;
                if (x_diff < 0) {
                    if (abs(x_diff) < dist_left) {
                        dist_left = abs(x_diff);
                        left_star_width = i_w;
                        left_star_idx = i;
                    }
                } else {
                    if (abs(x_diff) < dist_right) {
                        dist_right = abs(x_diff);
                        right_star_idx = i;
                    }
                }
            }
        }

        if (left_star_idx == -1) {
            left_star_width = 0;
            dist_left = abs(target_x - 21);
        }

        if (right_star_idx == -1) {
            dist_right = abs(target_x - 527);
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

int8_t __cdecl Do_Change_Star_Name_(int16_t x, int16_t y, int16_t star_idx, int8_t is_animated, char* title) {
    s_colors colors;
    char backup_name[20];

    fonts::Set_Font_Style_(3, &colors);

    int16_t w_star = fonts::Get_String_Width_(MOX::_star[star_idx].name);
    int16_t w_www = fonts::Get_String_Width_(s_WWWWWWW_0055137c);
    int16_t w_max = Max_Pixel_Width_Star_Name_Can_Be_(star_idx);

    if (w_star > w_max) {
        w_max = w_star;
    }

    if (w_max > w_www) {
        w_max = w_www;
    }

    if (w_max > 0xCD) {
        w_max = 0xCD;
    }

    strcpy(backup_name, MOX::_star[star_idx].name);

    int8_t result;
    if (is_animated != 0) {
        result = Input_Box_Popup_Animated_(MOX::_star[star_idx].name, 15, w_max, title, x, y);
    } else {
        result = Input_Box_Popup_(MOX::_star[star_idx].name, 15, w_max, title, x, y, 0);
    }

    if (MOX::_star[star_idx].name[0] == '\0') {
        strcpy(MOX::_star[star_idx].name, backup_name);
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

} // namespace namestar