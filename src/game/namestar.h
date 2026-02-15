#pragma once
#include <cstdint>

namespace namestar {
    int16_t Star_Name_Popup_Screen_Center_X_();
    int16_t Star_Name_Popup_Screen_Center_Y_();
    inline void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, uint8_t mode) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, uint8_t);
        reinterpret_cast<Fn>(0x00411e80)(x, y, star_idx, mode);
    }
    int16_t Star_Name_Popup_Map_Center_X_();
    int16_t Star_Name_Popup_Map_Center_Y_();
    inline void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00411eb0});
        fn(x, y, star_idx);
    }
}