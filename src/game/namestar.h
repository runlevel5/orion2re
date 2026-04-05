#pragma once
#include <cstdint>

namespace namestar {
    int16_t Star_Name_Popup_Screen_Center_X_();
    int16_t Star_Name_Popup_Screen_Center_Y_();
    void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, int8_t mode);
    int16_t Star_Name_Popup_Map_Center_X_();
    int16_t Star_Name_Popup_Map_Center_Y_();
    int8_t __cdecl Do_Change_Star_Name_(int16_t x, int16_t y, int16_t star_idx, int8_t use_popup2, char* title);
    void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx);

    int16_t __cdecl Max_Pixel_Width_Star_Name_Can_Be_(int16_t star_idx);
    int8_t __cdecl Can_Change_Star_Name_(short star_idx);

    inline int8_t __cdecl Input_Box_Popup_(char* buffer, int max_chars, int16_t width, char* title, int16_t x, int16_t y, int unknown) {
        using Fn = int8_t(__cdecl*)(char*, int, int16_t, char*, int16_t, int16_t, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00411710});
        return fn(buffer, max_chars, width, title, x, y, unknown);
    }

    inline int8_t __cdecl Input_Box_Popup2_(char* buffer, int max_chars, int16_t width, char* title, int16_t x, int16_t y) {
        using Fn = int8_t(__cdecl*)(char*, int, int16_t, char*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004116e0});
        return fn(buffer, max_chars, width, title, x, y);
    }
}