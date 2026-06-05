#pragma once

#include <cstdint>

struct s_mouse_list;

namespace sysfont {
    extern uint32_t _thin_column_lookup[8];
    extern uint32_t _font_pixel_data[4096];
    extern uint8_t _default_mouse_palette[576];

    extern uint32_t _mouse_cursor_color;
    extern s_mouse_list _default_system_mouse_list;
    extern uint16_t _default_mouse_image[576];
    extern uint8_t _default_palette_colors[48];

    /* Initializes the built-in cursor palette, image, and default mouse list. */
    void __cdecl Load_System_Mouse_();
    /* Installs the built-in mouse assets and builds the encoded default font glyph stream. */
    void __cdecl Load_System_Font_();
}