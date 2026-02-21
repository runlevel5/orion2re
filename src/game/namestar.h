#pragma once
#include <cstdint>

namespace namestar {
    int16_t Star_Name_Popup_Screen_Center_X_();
    int16_t Star_Name_Popup_Screen_Center_Y_();
    void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, int8_t mode);
    int16_t Star_Name_Popup_Map_Center_X_();
    int16_t Star_Name_Popup_Map_Center_Y_();
    inline void __cdecl Do_Change_Star_Name_(int16_t x, int16_t y, int16_t star_idx, int8_t mode, char* prompt) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int8_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00411ee0});
        fn(x, y, star_idx, mode, prompt);
    }
    void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx);
}