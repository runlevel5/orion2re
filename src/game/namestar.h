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

    int8_t __cdecl Input_Box_Popup_(char* input_text, int16_t input_type, int16_t max_length, char* prompt_text, int16_t y, int16_t x, int8_t show_cancel_anim);

    int8_t __cdecl Input_Box_Popup_Animated_(char* prompt, int16_t x, int16_t y, char* buffer, int16_t param_5, int16_t param_6);
}