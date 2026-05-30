#pragma once
#include <cstdint>

struct s_animation_header;

namespace bitmap {
    void __cdecl Get_Bitmap_Actual_Size_(s_animation_header* header, int16_t* out_min_x, int16_t* out_min_y, int16_t* out_actual_width, int16_t* out_actual_height);

    int32_t __cdecl Get_Bitmap_Size_(int16_t width, int16_t height);

    void __cdecl Create_Bitmap_Header_(int16_t width, int16_t height, s_animation_header* h);

    void __cdecl Create_Blank_Bitmap_(int16_t w, int16_t h, void* sprite);

    void __cdecl Draw_Picture_To_Bitmap_(s_animation_header* h1, s_animation_header* h2);

    void __cdecl Gray_Scale_Bitmap_(
        s_animation_header* animation_header,
        uint32_t unused,
        int16_t start_color,
        const uint8_t* color_translation_table,
        int16_t end_color
    );

    extern const char s_Add_Picture_To_Bitmap_targetmust_0055b770[];

    void __cdecl Add_Picture_To_Bitmap_(int16_t x, int16_t y, s_animation_header* src, s_animation_header* dest);

    void __cdecl Screen_Bitmap_Capture_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_animation_header* header);

    void __cdecl Mask_Out_Colors_(s_animation_header* anim, int16_t start_color, int16_t end_color);

    void __cdecl Tint_Bitmap_(s_animation_header* dest, s_animation_header* src, int16_t tint_color_index);

    extern uint32_t _pixel_count;

    extern uint8_t* _bitmap_temp1;

    extern uint32_t _bitmap_temp2;

    void __cdecl Tint_Bitmap_Pixels_(uint8_t* src, uint8_t* tint_table, int unused, uint8_t* dest, int width, int height);

    void __cdecl Clear_Mouse_Image_(int16_t image_idx);

    void __cdecl Vanish_Bitmap_(s_animation_header* dest, s_animation_header* src, int16_t intensity);

    void __cdecl Set_Picture_Glassed_(s_animation_header* anim);

    void __cdecl Flip_Bitmap_(s_animation_header* dst_hdr, s_animation_header* src_hdr, int16_t flip_mode);

    void __cdecl Outline_Bitmap_(s_animation_header* dst, s_animation_header* src, int16_t color, int16_t no_glass);

    void __cdecl Mask_Out_Bitmap_With_Bitmap_(int16_t x, int16_t y, s_animation_header* dest, s_animation_header* src, int16_t type);

    void __cdecl Copy_Bitmap_To_Bitmap_(s_animation_header* dst, s_animation_header* src);

    extern const char s_Brighten_perecnt_needs_to_be_bet_0055b714[];

    extern const char s_Not_Enough_Space_For_Brighten_Pa_0055b744[];

    extern uint8_t* _brighten_palette_seg;

    extern int8_t _brighten_palette_levels;

    extern int32_t _brighten_start_color;

    void __cdecl Scale_Bitmap_(s_animation_header* dst, s_animation_header* src, int16_t scale_x, int16_t scale_y);

    void __cdecl Draw_Bitmap_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_animation_header* img);

    void __cdecl Draw_Bitmap_To_Mouse_(int16_t x, int16_t y, s_animation_header* anim, int16_t mouse_type);

    void __cdecl Create_Brighten_Bitmap_Palettes_(s_animation_header* anim, int8_t* levels, int8_t count);

    void __cdecl Mask_Brighten_Bitmap_(int32_t x, int32_t y, s_animation_header* dst_anim, s_animation_header* src_anim, uint8_t color);

    void __cdecl Jumble_Bitmap_(s_animation_header* dest, s_animation_header* src, int16_t param_3, int16_t param_4);
}