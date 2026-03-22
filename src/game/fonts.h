#pragma once
#include "misc.h"

namespace fonts {
    inline uint16_t& _PSTR_style = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005787B4});

    inline void*& _palette_block =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x005787B8 });

    inline char* const _font_name =
        reinterpret_cast<char*>(std::uintptr_t{ 0x005787C0 });

    inline int16_t& _print_xpos = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00576430});
    inline int16_t& _print_ypos = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00576440});

    inline int16_t& _current_font_style =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0057643C});
    inline int16_t& _current_font_style_2 = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00578e1c});

    inline uintptr_t& _font_data = *reinterpret_cast<uintptr_t*>(std::uintptr_t{0x00576434});

    inline s_font& _font_header =
        *reinterpret_cast<s_font*>(std::uintptr_t{0x00576448});

    void __cdecl Print_Character_ASM_(void* screen, const uint8_t* glyph_data, const uint8_t* selected_colors, uint32_t height);
    void __cdecl Load_Palette_(int16_t palette_id, int16_t start_index, int16_t end_index);

    inline int16_t& _draw_alias_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B150});

    inline s_colors& _current_font_color_array =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x005787A8});

    inline void __cdecl Print_Character_No_Alias_ASM_(void* dest, void* glyph, s_font* header, int height) {
        using Fn = void(__cdecl*)(void*, void*, s_font*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005366AE});
        fn(dest, glyph, header, height);
    }

    void __cdecl Set_PSTR_Style_(uint16_t style);
    void __cdecl Set_Font_Style_(int font_style, s_colors *new_colors);

    inline void __cdecl Set_Remap_Font_Style_(int style, s_colors* colors) {
        using Fn = void(__cdecl*)(int, s_colors*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005254c0});
        fn(style, colors);
    }

    int16_t __cdecl Get_String_Width_(const char* str);

    int16_t __cdecl Print_Character_(int16_t x, int16_t y, char c);

    inline void __cdecl Print_Integer_(int16_t x, int16_t y, int32_t value) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005257b0});
        fn(x, y, value);
    }

    void __cdecl Print_Character_To_Bitmap_(int16_t x, int16_t y, char ch, s_animation_header* seg);

    inline void __cdecl Print_Clipped_Letter_To_Bitmap_(short draw_x, short draw_y, char char_idx, short src_x, short clipped_width, short src_y, short clipped_height, s_animation_header* header) {
        using Fn = void(__cdecl*)(short, short, char, short, short, short, short, s_animation_header*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00526bd0});
        fn(draw_x, draw_y, char_idx, src_x, clipped_width, src_y, clipped_height, header);
    }

    int16_t __cdecl Print_Character_No_Alias_(int16_t x, int16_t y, char character);

    void __cdecl Set_Normal_Colors_On_(void);
    void __cdecl Set_Highlight_Colors_On_(void);
    void __cdecl Set_Special_Colors_On_(void);

    int16_t __cdecl Print_String_(int16_t x, int16_t y, const char* str, int16_t process_colors, int16_t justified_width);

    inline void __cdecl Print_Display_(int16_t x, int16_t y, const char* text, int32_t flags) {
        using Fn = void(__cdecl*)(int16_t, int16_t, const char*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00525930});
        fn(x, y, text, flags);
    }

    void __cdecl Print_(int16_t x, int16_t y, const char* str);

    inline void __cdecl PSTRS_(int16_t x, int16_t y, const char* str) {
        using Fn = void(__cdecl*)(int16_t, int16_t, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00533670});
        fn(x, y, str);
    }

    void __cdecl Set_Font_Style_Outline_Heavy_(int16_t font_index, s_colors* style_data);

    inline uint16_t& _outline_color =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B148});

    inline uint16_t& _alias_outline_color =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B14C});

    void __cdecl Set_Outline_Color_(uint16_t color, uint16_t alias_color);

    inline void __cdecl Print_Display_To_Bitmap_(int16_t x, int16_t y, const char* text, int32_t param_4, void* bitmap) {
        using Fn = void(__cdecl*)(int16_t, int16_t, const char*, int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00526630});
        fn(x, y, text, param_4, bitmap);
    }

    void __cdecl Print_To_Bitmap_(int16_t x, int16_t y, const char* text, void* bitmap);

    // Suggested name: fonts::Update_Palette_Internal
    inline void __cdecl FUN_00526f80() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00526F80 });
        fn();
    }

    int16_t __cdecl Get_Current_Font_Style_();

    s_colors* __cdecl Get_Current_Font_Color_();

    int16_t __cdecl Get_Font_Height_();

    void __cdecl Set_Font_Style_Outline_(int16_t style, s_colors* colors);

    void __cdecl Print_Centered_(int16_t x, int16_t y, const char* str);

    inline void __cdecl Print_Integer_Centered_(int x, int y, int value) {
        using Fn = void(__cdecl*)(int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005258a0});
        fn(x, y, value);
    }
     
    void __cdecl Print_Right_(int16_t x, int16_t y, const char* text);

    void __cdecl Print_Integer_Right_(int16_t x, int16_t y, int32_t value);

    void __cdecl Set_Font_Spacing_(int16_t spacing);

    void __cdecl Set_Font_Spacing_Width_(int spacing_width);

    void __cdecl Get_Colors_(s_colors* colors);
    void __cdecl Set_Font_Special_Colors_(s_colors* colors);
    void __cdecl Set_Font_Highlight_Colors_(s_colors* colors);

    void __cdecl Set_Font_Style_Shadow_Down_(int16_t style, s_colors* colors);

    void __cdecl Set_Alias_Color_(int32_t color);
}
