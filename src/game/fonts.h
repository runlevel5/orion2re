#pragma once
#include "misc.h"

namespace fonts {
#pragma pack(push, 1)
struct s_font {
    uint8_t selected_colors2[8];      // 0x00
    int16_t font_style_height;        // 0x08
    int16_t font_style;               // 0x0A
    uint8_t selected_colors1[8];      // 0x0C
    uint8_t highlight_colors[8];      // 0x14
    uint8_t special_colors[8];        // 0x1C
    uint16_t current_height;          // 0x24
    int16_t line_height;              // 0x26
    uint16_t field24_0x28;            // 0x28
    int16_t font_style_spacing;       // 0x2A
    int8_t font_style_widths[256];    // 0x2C
    uint32_t font_style_offsets[256]; // 0x12C
    int16_t font_heights[8];          // 0x52C
    int16_t font_spacing[8];          // 0x53C
    int16_t field31_0x54c[8];         // 0x54C
    uint8_t font_glyph_widths[6][256];// 0x55C
    uint32_t font_glyph_offsets[6][256];// 0xB5C
};
#pragma pack(pop)
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

    inline int16_t& _draw_alias_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B150});

    inline MISC::s_colors& _current_font_color_array =
        *reinterpret_cast<MISC::s_colors*>(std::uintptr_t{0x005787A8});

    void Load_Palette_(int16_t palette_id, int16_t start_index, int16_t end_index);

    inline void __cdecl Print_Character_No_Alias_ASM_(void* dest, void* glyph, s_font* header, int height) {
        using Fn = void(__cdecl*)(void*, void*, s_font*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005366AE});
        fn(dest, glyph, header, height);
    }

    void __cdecl Set_PSTR_Style_(uint16_t style);
    void __cdecl Set_Font_Style_(int font_style, MISC::s_colors *new_colors);

    int16_t __cdecl Get_String_Width_(const char* str);

    int16_t __cdecl Print_Character_(int16_t x, int16_t y, char c);

    int16_t __cdecl Print_Character_No_Alias_(int16_t x, int16_t y, char character);

    void __cdecl Set_Normal_Colors_On_(void);
    void __cdecl Set_Highlight_Colors_On_(void);
    void __cdecl Set_Special_Colors_On_(void);

    int16_t __cdecl Print_String_(int16_t x, int16_t y, const char* str, int16_t process_colors, int16_t justified_width);

    inline void __cdecl Print_(int16_t x, int16_t y, const char* str) {
        using Fn = void(__cdecl*)(int16_t, int16_t, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00525910 });
        fn(x, y, str);
    }

    void __cdecl Set_Font_Style_Outline_Heavy_(int16_t font_index, MISC::s_colors* style_data);

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

    inline int16_t __cdecl Get_Font_Height_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00526060 })();
    }

    inline void __cdecl Set_Font_Style_Outline_(int16_t style, void* color_data) {
        using Fn = void(__cdecl*)(int16_t, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x005256b0})(style, color_data);
    }

    void __cdecl Print_Centered_(int16_t x, int16_t y, const char* str);
     
    void __cdecl Print_Right_(int16_t x, int16_t y, const char* text);

    void __cdecl Set_Font_Spacing_(int16_t spacing);

    inline void __cdecl Print_ASM_(int16_t x, int16_t y, const char* text) {
        using Fn = void(__cdecl*)(int16_t, int16_t, const char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00525910})(x, y, text);
    }

    inline uint32_t __cdecl Get_String_Width_ASM_(const void* text) {
        using Fn = uint32_t(__cdecl*)(const void*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00454ae0})(text);
    }

    void __cdecl Get_Colors_(uint8_t* colors);
    void __cdecl Set_Colors_(const uint8_t* colors);

    inline void __cdecl Set_Font_Special_Colors_(uint8_t* colors) {
        using Fn = void(__cdecl*)(uint8_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00525770})(colors);
    }
}
