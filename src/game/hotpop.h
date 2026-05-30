#pragma once
#include <cstdint>

struct s_animation_header;

namespace HOTPOP {
    extern int16_t _hotseat_human_players;
    extern s_animation_header* _hot_pop_background_seg;
    extern s_animation_header* _grid_background[4];

    void __cdecl Reload_Hotseat_Screen_();

    void __cdecl Fade_Into_Hotseat_Screen_();

    void __cdecl Init_Hotseat_Screen_(int16_t* btn_field_1, int16_t* btn_field_2, int16_t* btn_field_3);

    void __cdecl Draw_Hotseat_Screen_(void);

    void __cdecl Hotseat_Screen_(void);
}