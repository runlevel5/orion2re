#pragma once

struct s_animation_header;

namespace remap {
    extern uint8_t _picture_remap_color_list[256];

    extern const char s_Remapped_Picture_Must_Have_Palet_0055b2ac[];

    void __cdecl Update_Glass_Remap_Colors_();
    void __cdecl Sort_Palette_List_();

    uint8_t __cdecl Find_Closest_Color_(uint8_t r, uint8_t g, uint8_t b);

    void __cdecl Create_Picture_Remap_Palette_(uint8_t* src_palette, uint8_t* remap_table);

    void __cdecl Calculate_Picture_Remap_Colors_(s_animation_header* header);

    uint8_t __cdecl Get_Remap_Palette_Color_(uint8_t index);

    void __cdecl Get_Palette_Animation_Color_(
        s_animation_header* anim, int16_t index, uint8_t* red, uint8_t* green, uint8_t* blue);
}