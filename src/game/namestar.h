#pragma once

namespace namestar {
    int16_t __cdecl Star_Name_Popup_Screen_Center_X_();
    int16_t __cdecl Star_Name_Popup_Screen_Center_Y_();
    void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, int8_t mode);
    int16_t __cdecl Star_Name_Popup_Map_Center_X_();
    int16_t __cdecl Star_Name_Popup_Map_Center_Y_();
    int8_t __cdecl Do_Change_Star_Name_(int16_t x, int16_t y, int16_t star_idx, int8_t use_popup2, char* title);
    void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx);

    int16_t __cdecl Max_Pixel_Width_Star_Name_Can_Be_(int16_t star_idx);
    int8_t __cdecl Can_Change_Star_Name_(int16_t star_idx);

    inline s_input_box_state& _input_box =
        *reinterpret_cast<s_input_box_state*>(std::uintptr_t{0x00564760});

    void __cdecl Setup_Input_Box_Popup_();

    void __cdecl Draw_Input_Box_Popup_();

    inline int8_t __cdecl Input_Box_Popup_(char* dest, int16_t max_len, int16_t max_width, char* title, int16_t x, int16_t y, uint8_t unknown) {
        using Fn = int8_t(__cdecl*)(char*, int16_t, int16_t, char*, int16_t, int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00411710});
        return fn(dest, max_len, max_width, title, x, y, unknown);
    }

    inline int8_t __cdecl Input_Box_Popup_Animated_(char* dest, int16_t max_len, int16_t max_width, char* title, int16_t x, int16_t y) {
        using Fn = int8_t(__cdecl*)(char*, int16_t, int16_t, char*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004116E0});
        return fn(dest, max_len, max_width, title, x, y);
    }
}