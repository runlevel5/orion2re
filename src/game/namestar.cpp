#include "pch.h"
#include "fonts.h"
#include "mox.h"
#include "namestar.h"
#include "funcs.h"
#include <cstring>

namespace namestar {
    int16_t Star_Name_Popup_Screen_Center_X_() {
        return 0xb1;
    }

    int16_t Star_Name_Popup_Screen_Center_Y_() {
        return 125;
    }

    int16_t Star_Name_Popup_Map_Center_X_() {
        return 131;
    }

    int16_t Star_Name_Popup_Map_Center_Y_() {
        return 0x92;
    }

    void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, int8_t mode) {
        char* message = const_cast<char*>(HAROLD::H_Message_(HESTR_100_ENTER_STAR_NAME));
        Do_Change_Star_Name_(x, y, star_idx, mode, message);
    }

    void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx) {
        char* prompt = const_cast<char*>(HAROLD::H_Message_(HESTR_101_ENTER_HOME_STAR_NAME));
        Do_Change_Star_Name_(x, y, star_idx, 0, prompt);
    }

    int8_t __cdecl Do_Change_Star_Name_(int16_t x, int16_t y, int16_t star_idx, int8_t use_popup2, char* title) {
        s_colors font_colors;
        char original_name[20];

        fonts::Set_Font_Style_(3, &font_colors);

        s_star_data* star = &MOX::_star[star_idx];

        int16_t current_name_width = fonts::Get_String_Width_(star->name);
        int16_t www_width = fonts::Get_String_Width_(s_WWWWWWW_0055137c);
        int16_t max_allowed_width = Max_Pixel_Width_Star_Name_Can_Be_(star_idx);

        int16_t input_width = max_allowed_width;
        if (current_name_width > input_width) {
            input_width = current_name_width;
        }
        if (input_width > www_width) {
            input_width = www_width;
        }
        if (input_width > 205) {
            input_width = 205;
        }

        strcpy(original_name, star->name);

        int8_t popup_result;
        if (use_popup2 != 0) {
            popup_result = Input_Box_Popup2_(star->name, 15, input_width, title, x, y);
        } else {
            popup_result = Input_Box_Popup_(star->name, 15, input_width, title, x, y, 0);
        }

        if (star->name[0] == '\0') {
            strcpy(star->name, original_name);
        }

        fonts::Get_String_Width_(star->name);

        return popup_result;
    }
}