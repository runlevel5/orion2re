#pragma once
#include <cstdint>

namespace COLLAND {
    extern s_animation_header* _l_anims;
    extern int32_t _pod1l_sfx;
    extern int32_t _pod2d_sfx;
    extern int32_t _pod2l_sfx;
    extern int32_t _pod1d_sfx;
    extern int16_t _fields;

    extern const char s_format_comma_space[];

    void __cdecl Load_Anims_();
    void Add_Fields_();

    void __cdecl Draw_Colony_Landing_Screen_();

    void __cdecl Print_Planet_Data_(int16_t planet_idx);

    void __cdecl Colony_Landing_Screen_();
}