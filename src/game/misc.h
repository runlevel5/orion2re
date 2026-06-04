#pragma once

struct s_animation_header;

namespace MISC {
    extern uint8_t _mouse_clock_image_num;

    extern s_mouse_list _mouse_list_clock[4];

    void __cdecl Increment_Mouse_Clock_Image_();

    void __cdecl Fade_To_Black_();

    void __cdecl Fade_Into_Screen_(void (*draw_callback)(), uint8_t* status_flag, int16_t palette_id);

    void __cdecl Change_Font_Style_(int16_t font_id);

    extern s_colors _mox_font_colors;

    s_colors* __cdecl Font_Colors2_(int16_t mode, uint8_t primary_color, uint8_t secondary_color);

    void __cdecl Set_Fitted_Font_Style_(char* text, s_colors* colors, int16_t initial_style, int16_t initial_spacing, int16_t variant, int16_t max_width, int16_t min_style);

    int16_t __cdecl Get_Font_Style_();

    void __cdecl Get_Mox_Font_Colors_(int16_t mode, uint8_t primary_color, uint8_t secondary_color, s_colors* out_colors);

    void __cdecl Set_Font_Default_Spacing_();

    void __cdecl Increase_Font_Color_Range_(int16_t increment);
    void __cdecl Decrease_Font_Color_Range_(int16_t amount);

    char* __cdecl Get_Comma_String_(int32_t value);

    int16_t __cdecl Get_Font_Spacing_();

    void __cdecl Draw_Advancing_Fill_Message_(uint8_t font_style, int16_t* p_frame_index, int16_t total_frames,
                                              s_colors* p_colors, s_colors* target_color, int16_t x, int16_t y,
                                              char* text, s_animation_header* anim, uint8_t alignment);

    void __cdecl Get_Advancing_Bitmap_String_Range_(int16_t tick, int16_t max_width, int16_t* out_start_x, int16_t* out_start_y, int16_t* out_end_x, int16_t* out_end_y, const char* text, uint8_t is_full_range);

    void __cdecl Replace_Bitmap_Colors_(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end,
                                       uint8_t full_bitmap, uint8_t* old_colors, uint8_t* new_colors,
                                       int8_t color_count, s_animation_header* anim);

    void __cdecl Save_Window_Status_(int16_t* status);

    void __cdecl Restore_Window_Status_(int16_t status);

    void __cdecl Get_LBX_Message_(int16_t message_id, char* buffer);

    void __cdecl Get_Bitmap_String_Range_(int16_t segment_idx, int16_t divisor, int16_t* out_start_x, int16_t* out_start_y, int16_t* out_end_x, int16_t* out_end_y, char* text);

    void __cdecl Swap_Bitmap_Colors_(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, void* old_colors, void* new_colors, int16_t num_colors, s_animation_header* bitmap);

    void __cdecl Replace_Char_(char* str, char target, char replacement);

    void __cdecl _Darken_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    int16_t __cdecl Bit_Value_To_Num_(int32_t value, int16_t bit_limit);

    bool __cdecl Occluded_(int16_t x1_min, int16_t y1_min, int16_t x1_max, int16_t y1_max,
                           int16_t x2_min, int16_t y2_min, int16_t x2_max, int16_t y2_max);

    void __cdecl Set_Font_Colors_(const s_colors* colors);

    bool __cdecl Is_Vowel_(char c);

    void __cdecl Draw_Highlight_Box_(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t* colors);

    void __cdecl Draw_Darkened_Box_(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t color_base);

    void __cdecl Tint_Bitmap_Colors_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool full_bitmap, uint8_t tint_value, s_animation_header* anim);
    void __cdecl Get_Bitmap_Dimensions_(s_animation_header* header, int16_t* width, int16_t* height);

    void __cdecl Diagonal_Line_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t spacing, uint8_t color1, uint8_t color2);

    void __cdecl _Print_Centered_Shadow_Down_(int16_t x, int16_t y, uint8_t color, const char* text);

    void __cdecl _Print_Shadow_Down_(int16_t x, int16_t y, uint8_t shadow_color, const char* text);

    int32_t __cdecl Substring_(char* haystack, char* needle);

    void __cdecl Get_Time_Stamp_(
        const char* input,
        int16_t* out_month,
        int16_t* out_day,
        int16_t* out_year,
        int16_t* out_hour,
        int16_t* out_minute,
        int16_t* out_second);
}