#include "deps.h"

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
}