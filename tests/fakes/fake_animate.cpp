#include "pch.h"

#include "fakes/fake_test_support.h"

namespace animate {
    s_palette_header* _palette_header = nullptr;
    s_animation_header* _animation_header = nullptr;
    int16_t _clipping_flag = 0;
    int16_t _dot_stencil_flag = 0;
    char s_Screen_Background_cant_be_clipp[35] = "";
    char s_Refresh_Mask_cant_be_drawn[28] = "";

    short __cdecl Get_Full_Store_Flag_(s_animation_header* header) {
        (void)header;
        return test::fakes::g_anim_full_store_flag;
    }

    void __cdecl Set_Animation_Frame_(s_animation_header* anim, int32_t frame) {
        (void)anim;
        (void)frame;
    }

    void __cdecl Reset_Animation_Frame_(s_animation_header* anim) {
        (void)anim;
    }

    int16_t __cdecl Get_Animation_Frame_Count_(s_animation_header* anim) {
        (void)anim;
        return 1;
    }

    int16_t __cdecl Get_Current_Frame_(s_animation_header* animation) {
        (void)animation;
        return 0;
    }

    int16_t __cdecl Get_Width_(s_animation_header* anim) {
        (void)anim;
        return test::fakes::g_anim_width;
    }

    int16_t __cdecl Get_Height_(s_animation_header* anim) {
        (void)anim;
        return test::fakes::g_anim_height;
    }

    void __cdecl Draw_(int16_t x, int16_t y, s_animation_header* anim) {
        test::fakes::g_draw_calls.push_back({x, y, anim});
    }

    void __cdecl Remap_Draw_(int16_t x, int16_t y, s_animation_header* anim) {
        Draw_(x, y, anim);
    }

    void __cdecl Remap_Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* frame_ptr) {
        (void)x;
        (void)y;
        (void)frame_ptr;
    }

    void __cdecl Remap_Clipped_Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* frame_ptr) {
        (void)x;
        (void)y;
        (void)frame_ptr;
    }

    void __cdecl Draw_No_Glass_(int16_t x, int16_t y, s_animation_header* header) {
        Draw_(x, y, header);
    }

    void __cdecl Draw_No_Glassed_Animated_Sprite_(int16_t x, int16_t y, uint8_t* frame_data) {
        (void)x;
        (void)y;
        (void)frame_data;
    }

    void __cdecl Draw_No_Glassed_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* bitmap) {
        (void)x;
        (void)y;
        (void)bitmap;
    }

    void __cdecl Set_Animation_Palette_() {
    }

    void __cdecl Draw_Animated_Sprite_Back_Diff_(int16_t x, int16_t y, uint8_t* frame_data) {
        (void)x;
        (void)y;
        (void)frame_data;
    }

    void __cdecl Draw_Glassed_Animated_Sprite_(int16_t x, int16_t y, uint8_t* frame_data) {
        (void)x;
        (void)y;
        (void)frame_data;
    }

    void __cdecl Draw_Glassed_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* sprite) {
        (void)x;
        (void)y;
        (void)sprite;
    }

    void __cdecl Draw_Glassed_Screen_Background_Off_(uint8_t* sprite) {
        (void)sprite;
    }

    void __cdecl Draw_Screen_Background_Off_(uint8_t* sprite_data) {
        (void)sprite_data;
    }

    void __cdecl Draw_Palette_(s_animation_header* header) {
        (void)header;
    }
}