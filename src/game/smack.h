#pragma once

#include <cstdint>

struct s_mouse_list;

namespace SMACK {
    extern s_mouse_list _smack_mouse;

    extern MOX2::action_func_t _smack_auto_function;

    extern int16_t _smack_x;

    extern int16_t _smack_y;

    void __cdecl Smack_Fade_(int16_t mode);

    void __cdecl Play_Cinematic_(const char* smacker_name, uint32_t play_flags, char use_fade);
}