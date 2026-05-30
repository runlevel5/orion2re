#include "pch.h"

namespace sysfont {

uint32_t _thin_column_lookup[8] = {
    0, 2, 4, 6, 8, 10, 11, 14,
};

uint32_t _font_pixel_data[4096] = {
#include "sysfont_font_pixel_data.inc"
};

uint8_t _default_mouse_palette[576];

uint32_t _mouse_cursor_color = 15;

s_mouse_list _default_system_mouse_list = {1, 0, 0, 0, 0, 0};

uint16_t _default_mouse_image[576] = {
    2, 2, 2, 2, 2, 2, 2, 2, 255, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 1, 1, 1, 1, 1, 2, 255, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 1, 1, 1, 1, 2, 255, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 1, 1, 1, 1, 1, 2, 255, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 1, 1, 1, 1, 1, 1, 2, 255, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 1, 2, 1, 1, 1, 1, 1, 2, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 0, 2, 1, 1, 1, 1, 2, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 2, 1, 1, 1, 2, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 1, 1, 2, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 1, 2, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2, 2, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 2, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

uint8_t _default_palette_colors[48] = {
    0, 0, 0, 0, 0, 21, 0, 21, 42, 21, 42, 42,
    0, 42, 21, 21, 0, 21, 42, 21, 21, 0, 21, 0,
    21, 63, 21, 63, 63, 21, 21, 0, 0, 63, 0, 0,
    42, 21, 0, 63, 42, 0, 21, 21, 21, 63, 63, 42,
};

void __cdecl Load_System_Mouse_() {
    for (int pal_idx = 0; pal_idx < 16; ++pal_idx) {
        video::_current_palette[pal_idx].r = sysfont::_default_palette_colors[pal_idx * 3 + 0];
        video::_current_palette[pal_idx].g = sysfont::_default_palette_colors[pal_idx * 3 + 1];
        video::_current_palette[pal_idx].b = sysfont::_default_palette_colors[pal_idx * 3 + 2];

        video::_current_palette[240 + pal_idx].r = sysfont::_default_palette_colors[pal_idx * 3 + 0];
        video::_current_palette[240 + pal_idx].g = sysfont::_default_palette_colors[pal_idx * 3 + 1];
        video::_current_palette[240 + pal_idx].b = sysfont::_default_palette_colors[pal_idx * 3 + 2];
    }

    for (int y = 0; y < 576; y += 24) {
        for (int x = 0; x < 24; ++x) {
            uint16_t pixel = sysfont::_default_mouse_image[y + x];
            if (pixel == 0xFF) {
                video::_mouse_palette[y + x] = 0x00;
            } else if (pixel != 0) {
                if (pixel == 2) {
                    video::_mouse_palette[y + x] = static_cast<uint8_t>(sysfont::_mouse_cursor_color);
                } else {
                    video::_mouse_palette[y + x] = static_cast<uint8_t>(sysfont::_mouse_cursor_color);
                }
            }
        }
    }

    mouse::Set_Mouse_List_(&sysfont::_default_system_mouse_list, 1);
    palette::Set_Palette_Changes_(0, 0xFF);
    palette::Refresh_Palette_();
}

void __cdecl Load_System_Font_() {
    video::_mouse_palette = sysfont::_default_mouse_palette;
    sysfont::Load_System_Mouse_();

    fonts::_font_data = static_cast<uint8_t*>(allocate::Allocate_Space_(20000));

    fonts::_font_header.font_spacing[0] = 0;
    fonts::_font_header.leading_extra[0] = 0;

    memset(fonts::_font_header.font_glyph_widths[0], 4, 255);

    fonts::_font_header.font_heights[0] = 8;

    uint32_t current_offset = 0;
    uint32_t char_data_offset = 0;

    for (int char_idx = 0; char_idx < 256; char_idx++) {
        fonts::_font_header.font_glyph_offsets[0][char_idx] = current_offset;

        for (int col = 0; col < 8; col++) {
            uint32_t col_idx = sysfont::_thin_column_lookup[col];
            uint32_t pixel_data = sysfont::_font_pixel_data[col_idx + char_data_offset];

            uint32_t pixels[8];
            memset(pixels, 0, sizeof(pixels));

            if (pixel_data & 0x1000000) pixels[0] = 1;
            if (pixel_data & 0x10000)   pixels[1] = 1;
            if (pixel_data & 0x100)     pixels[2] = 1;
            if (pixel_data & 0x1)       pixels[3] = 1;

            uint32_t transparent_count = 0;
            for (int p = 0; p < 4; p++) {
                if (pixels[p] == 0) {
                    transparent_count++;
                } else {
                    if (transparent_count > 0) {
                        fonts::_font_data[current_offset++] = (uint8_t)(transparent_count + 0x80);
                    }
                    fonts::_font_data[current_offset++] = 1;
                    transparent_count = 0;
                }
            }
            fonts::_font_data[current_offset++] = 0x80;
        }

        while ((current_offset & 3) != 0) {
            current_offset++;
        }

        char_data_offset += 16;
    }
}

} // namespace sysfont