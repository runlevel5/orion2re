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
    for (int32_t palette_index = 0; palette_index < 16; ++palette_index) {
        video::_current_palette[palette_index].r = sysfont::_default_palette_colors[palette_index * 3 + 0];
        video::_current_palette[palette_index].g = sysfont::_default_palette_colors[palette_index * 3 + 1];
        video::_current_palette[palette_index].b = sysfont::_default_palette_colors[palette_index * 3 + 2];

        video::_current_palette[240 + palette_index].r = sysfont::_default_palette_colors[palette_index * 3 + 0];
        video::_current_palette[240 + palette_index].g = sysfont::_default_palette_colors[palette_index * 3 + 1];
        video::_current_palette[240 + palette_index].b = sysfont::_default_palette_colors[palette_index * 3 + 2];
    }

    for (int32_t row_offset = 0; row_offset < (int32_t)(sizeof(sysfont::_default_mouse_image) / sizeof(sysfont::_default_mouse_image[0])); row_offset += 24) {
        for (int32_t column_index = 0; column_index < 24; ++column_index) {
            uint16_t cursor_pixel = sysfont::_default_mouse_image[row_offset + column_index];
            if (cursor_pixel == 0xFF) {
                video::_mouse_palette[row_offset + column_index] = 0x00;
            } else if (cursor_pixel != 0) {
                if (cursor_pixel == 2) {
                    video::_mouse_palette[row_offset + column_index] = (uint8_t)sysfont::_mouse_cursor_color;
                } else {
                    video::_mouse_palette[row_offset + column_index] = (uint8_t)sysfont::_mouse_cursor_color;
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

    fonts::_font_data = (uint8_t*)allocate::Allocate_Space_(20000);

    fonts::_font_header.font_spacing[0] = 0;
    fonts::_font_header.leading_extra[0] = 0;

    memset(
        fonts::_font_header.font_glyph_widths[0],
        4,
        sizeof(fonts::_font_header.font_glyph_widths[0]) -
            sizeof(fonts::_font_header.font_glyph_widths[0][255]));

    fonts::_font_header.font_heights[0] = 8;

    uint32_t glyph_write_offset = 0;
    uint32_t glyph_data_offset = 0;

    for (int32_t glyph_index = 0; glyph_index < (int32_t)(sizeof(fonts::_font_header.font_glyph_offsets[0]) / sizeof(fonts::_font_header.font_glyph_offsets[0][0])); ++glyph_index) {
        fonts::_font_header.font_glyph_offsets[0][glyph_index] = glyph_write_offset;

        for (int32_t column_index = 0; column_index < (int32_t)(sizeof(sysfont::_thin_column_lookup) / sizeof(sysfont::_thin_column_lookup[0])); ++column_index) {
            uint32_t pixel_column_index = sysfont::_thin_column_lookup[column_index];
            uint32_t pixel_data = sysfont::_font_pixel_data[pixel_column_index + glyph_data_offset];

            uint32_t pixels[8];
            memset(pixels, 0, sizeof(pixels));

            if (pixel_data & 0x1000000) pixels[0] = 1;
            if (pixel_data & 0x10000)   pixels[1] = 1;
            if (pixel_data & 0x100)     pixels[2] = 1;
            if (pixel_data & 0x1)       pixels[3] = 1;

            uint32_t transparent_count = 0;
            for (int32_t pixel_index = 0; pixel_index < 4; ++pixel_index) {
                if (pixels[pixel_index] == 0) {
                    transparent_count++;
                } else {
                    if (transparent_count > 0) {
                        fonts::_font_data[glyph_write_offset++] = (uint8_t)(transparent_count + 0x80);
                    }
                    fonts::_font_data[glyph_write_offset++] = 1;
                    transparent_count = 0;
                }
            }
            fonts::_font_data[glyph_write_offset++] = 0x80;
        }

        while ((glyph_write_offset & 3) != 0) {
            glyph_write_offset++;
        }

        glyph_data_offset += 16;
    }
}

} // namespace sysfont