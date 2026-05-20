#pragma once
#include <cstdint>

struct s_animation_header;

namespace animate {
    struct s_palette_header {
        int16_t start_index;
        int16_t count;
    };

    inline s_palette_header*& _palette_header =
        *reinterpret_cast<s_palette_header**>(std::uintptr_t{0x00576328});

    inline s_animation_header*& _animation_header =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x0057632C });

    inline int16_t& _clipping_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055B0E0 });
    inline int16_t& _dot_stencil_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055B0E4 });

    inline const char* s_Screen_Background_cant_be_clipp = reinterpret_cast<const char*>(0x0055B0E8);
    inline const char* s_Refresh_Mask_cant_be_drawn = reinterpret_cast<const char*>(0x0055B10C);

    short __cdecl Get_Full_Store_Flag_(s_animation_header* header);
    void __cdecl Set_Animation_Frame_(s_animation_header* anim, int16_t frame);
    void __cdecl Reset_Animation_Frame_(s_animation_header* anim);
    int16_t __cdecl Get_Animation_Frame_Count_(s_animation_header* seg);
    int16_t __cdecl Get_Current_Frame_(s_animation_header* animation);

    int16_t __cdecl Get_Width_(s_animation_header* anim);
    int16_t __cdecl Get_Height_(s_animation_header* anim);

    void __cdecl Draw_(int16_t x, int16_t y, s_animation_header* resource);
    void __cdecl Remap_Draw_(int16_t x, int16_t y, s_animation_header* anim);

    void __cdecl Remap_Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* frame_ptr);

    void __cdecl Remap_Clipped_Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* frame_ptr);

    void __cdecl Draw_No_Glass_(int16_t x, int16_t y, s_animation_header* header);

    void __cdecl Draw_No_Glassed_Animated_Sprite_(int16_t x, int16_t y, uint8_t* frame_data);

    void __cdecl Draw_No_Glassed_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* bitmap);

    void __cdecl Set_Animation_Palette_();

    void __cdecl Draw_Animated_Sprite_Back_Diff_(int16_t x, int16_t y, uint8_t* frame_data);

    void __cdecl Draw_Glassed_Animated_Sprite_(int16_t x, int16_t y, uint8_t* data);

    void __cdecl Draw_Glassed_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* sprite);

    void __cdecl Draw_Screen_Background_On_(uint32_t* sprite);

    void __cdecl Draw_Glassed_Screen_Background_On_(uint8_t* sprite);

    void __cdecl Draw_Glassed_Screen_Background_Off_(uint8_t* sprite);

    void __cdecl Draw_Screen_Background_Off_(uint8_t* data);

    void __cdecl Draw_Palette_(s_animation_header* header);
}