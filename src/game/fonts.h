#pragma once
#include "misc.h"

namespace fonts {
    extern uint16_t _PSTR_style;

    extern uint8_t* _current_font_blob;

    extern void* _palette_block;

    extern char _font_name[80];

    extern int16_t _print_xpos;
    extern int16_t _print_ypos;

    extern int16_t _current_font_style;

    extern uint8_t* _font_data;

    extern s_font _font_header;

    void __cdecl Print_Character_ASM_(void* screen, const uint8_t* glyph_data, const uint8_t* selected_colors, uint32_t height);
    void __cdecl Load_Palette_(int16_t palette_id, int16_t start_index, int16_t end_index);

    extern int16_t _draw_alias_flag;

    extern s_colors _current_font_color_array;

    void __cdecl Set_PSTR_Style_(uint16_t style);
    void __cdecl Set_Font_Style_(int font_style, const s_colors *new_colors);

    void __cdecl Set_Remap_Font_Style_(int font_idx, s_colors* colors);

    void __cdecl Load_Font_File_(char* filename);

    int16_t __cdecl Get_String_Width_(const char* str);

    int16_t __cdecl Print_Character_(int16_t x, int16_t y, int8_t c);

    void __cdecl Print_Integer_(int16_t x, int16_t y, int32_t value);

    void __cdecl Print_Character_To_Bitmap_(int16_t x, int16_t y, char ch, s_animation_header* seg);

    void __cdecl Print_Clipped_Letter_To_Bitmap_(int16_t x, int16_t y, char letter, int16_t clip_x, int16_t clip_width, int16_t clip_y, uint16_t clip_height, s_animation_header* anim);

    int16_t __cdecl Print_Character_No_Alias_(int16_t x, int16_t y, char character);

    void __cdecl Set_Normal_Colors_On_(void);
    void __cdecl Set_Highlight_Colors_On_(void);
    void __cdecl Set_Special_Colors_On_(void);

    int16_t __cdecl Print_String_(uint16_t x, int16_t y, const char* str, int16_t process_colors, int16_t justified_width);

    void __cdecl Print_Display_(int16_t x, int16_t y, const char* str, int16_t width);

    int16_t __cdecl Print_String_Clipped_(int16_t x, int16_t y, const char* str, int16_t process_colors, int16_t justified_width);

    void __cdecl Print_Clipped_Letter_(int16_t x, int16_t y, int8_t letter, int16_t clip_left, int16_t clip_width, int16_t start_y, int16_t end_y);

    int16_t __cdecl Print_Character_Clipped_(int16_t x, int16_t y, int8_t c);

    void __cdecl Print_(int16_t x, int16_t y, const char* str);

    void __cdecl PSTRS_(int16_t x, int16_t y, char* str);

    void __cdecl Set_Font_Style_Outline_Heavy_(int16_t font_index, s_colors* style_data);

    extern uint16_t _outline_color;

    extern uint16_t _alias_outline_color;

    void __cdecl Set_Outline_Color_(uint16_t color, uint16_t alias_color);

    extern const char s_Print_only_works_on_bitmaps[];

    void __cdecl Print_String_To_Bitmap_(int16_t x, int16_t y, const char* string, int16_t color_flags, int16_t max_width, s_animation_header* bitmap);

    void __cdecl Print_Display_To_Bitmap_(int16_t x, int16_t y, const char* str, int16_t param_4, s_animation_header* bitmap);

    void __cdecl Print_To_Bitmap_(int16_t x, int16_t y, const char* text, void* bitmap);

    void __cdecl Print_Centered_To_Bitmap_(int16_t x, int16_t y, const char* text, s_animation_header* dest);

    int16_t __cdecl Get_Current_Font_Style_();

    s_colors* __cdecl Get_Current_Font_Color_();

    s_colors* __cdecl Get_Current_Highlight_Color_();

    s_colors* __cdecl Get_Current_Special_Color_();

    int16_t __cdecl Get_Font_Height_();

    uint16_t __cdecl Get_Font_Vertical_Spacing_();

    void __cdecl Set_Font_Style_Outline_(int16_t style, s_colors* colors);

    void __cdecl Print_Centered_(int16_t x, int16_t y, const char* text);

    void __cdecl Print_Integer_Centered_(int16_t x, int16_t y, int32_t value);
     
    void __cdecl Print_Right_(int16_t x, int16_t y, const char* text);

    void __cdecl Print_Integer_Right_(int16_t x, int16_t y, int32_t value);

    void __cdecl Set_Font_Spacing_(int16_t spacing);
    void __cdecl Set_Font_LF_(uint16_t line_height);

    void __cdecl Set_Font_Spacing_Width_(int8_t spacing_width);

    void __cdecl Get_Colors_(s_colors* colors);
    void __cdecl Set_Font_Special_Colors_(s_colors* colors);
    void __cdecl Set_Font_Highlight_Colors_(s_colors* colors);

    void __cdecl Set_Font_Style_Shadow_Down_(int16_t style, const s_colors* colors);
    void __cdecl Set_Font_Style_Shadow_Up_(int16_t font_idx, s_colors* colors);

    void __cdecl Set_Alias_Color_(uint8_t color);

    void __cdecl Disp_PSTR_(int x, int y, const char* format, ...);
}