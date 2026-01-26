#pragma once
#include "misc.h"

namespace fonts {
#pragma pack(push, 1)
struct s_font {
    uint8_t selected_colors2[8];      // 0x00
    int16_t font_style_height;        // 0x08
    int16_t font_style;               // 0x0A
    uint8_t selected_colors1[8];      // 0x0C
    uint8_t field4_0x14[16];          // 0x14
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

    inline uint16_t& _current_font_style =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0057643C});

    inline uintptr_t& _font_data = *reinterpret_cast<uintptr_t*>(std::uintptr_t{0x00576434});

    inline s_font& _font_header =
        *reinterpret_cast<s_font*>(std::uintptr_t{0x00576448});

    void __cdecl Print_Character_ASM_(void* screen, const uint8_t* glyph_data, const uint8_t* selected_colors, int16_t height);

    inline int16_t& _draw_alias_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B150});

    inline uint8_t (&_current_font_color_array)[8] =
        *reinterpret_cast<uint8_t (*)[8]>(std::uintptr_t{0x005787A8});

    void Load_Palette_(int16_t palette_id, int16_t start_index, int16_t end_index);

    void Set_PSTR_Style_(uint16_t style);
    void __cdecl Set_Font_Style_(int font_style, uint8_t *new_colors);

    int16_t __cdecl Get_String_Width_(const char* str);

    int16_t __cdecl Print_Character_(int16_t x, int16_t y, char c);

    inline int16_t __cdecl Print_Character_No_Alias_(int16_t x, int16_t y, char c) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, char);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00525FD0})(x, y, c);
    }

    inline void __cdecl Set_Normal_Colors_On_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00525F00})();
    }

    inline void __cdecl Set_Highlight_Colors_On_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00525F20})();
    }

    inline void __cdecl Set_Special_Colors_On_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00525F40})();
    }

    int16_t __cdecl Print_String_(int16_t x, int16_t y, const char* str, int16_t process_colors, int16_t justified_width);

    inline void Print_(int32_t x, int32_t y, const char* str) {
        using Fn = void(*)(int32_t, int32_t, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00525910 });
        fn(x, y, str);
    }

    inline void __cdecl Set_Font_Style_Outline_Heavy_(int32_t style, void* color_data) {
        using Fn = void(__cdecl*)(int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005256D0});
        fn(style, color_data);
    }

    inline void __cdecl Set_Outline_Color_(int32_t color1, int32_t color2) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005256F0});
        fn(color1, color2);
    }

    inline void __cdecl Print_To_Bitmap_(int32_t x, int32_t y, uint8_t* text, void* bitmap) {
        using Fn = void(__cdecl*)(int32_t, int32_t, uint8_t*, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00526610});
        fn(x, y, text, bitmap);
    }

    // Suggested name: fonts::Update_Palette_Internal
    inline void __cdecl FUN_00526f80() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00526F80 });
        fn();
    }
}
