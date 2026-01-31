#include "deps.h"

namespace fonts {
    void __cdecl Set_Normal_Colors_On_(void) {
        for (int i = 0; i < 8; ++i) {
            _font_header.selected_colors2[i] = _font_header.selected_colors1[i];
        }
    }

    void __cdecl Set_Highlight_Colors_On_(void) {
        for (int i = 0; i < 8; ++i) {
            _font_header.selected_colors2[i] = _font_header.highlight_colors[i];
        }
    }

    void __cdecl Set_Special_Colors_On_(void) {
        for (int i = 0; i < 8; ++i) {
            _font_header.selected_colors2[i] = _font_header.special_colors[i];
        }
    }

    void __cdecl Print_Character_ASM_(void* screen, const uint8_t* glyph_data, const uint8_t* selected_colors, uint32_t height) {
        uint8_t* current_row = static_cast<uint8_t*>(screen);
        const uint8_t* data = glyph_data;

        for (int h = 0; h < (int)height; ++h) {
            uint8_t* dest = current_row;

            while (true) {
                uint8_t cmd = *data++;

                if (cmd == 0x80) {
                    break;
                }

                if (cmd < 0x80) {
                    *dest++ = selected_colors[cmd - 1];
                } else {
                    dest += (cmd & 0x7F);
                }
            }

            current_row += platform::g_screen_res_x;
        }
    }

    void Load_Palette_(int16_t palette_id, int16_t start_index, int16_t end_index) {
        // Load the font palette resource block
        void* loaded_palette = farload::Far_Reload_(_font_name, static_cast<int32_t>(palette_id) + 1, _palette_block);

        // Calculate the range to copy. Colors are assumed to be 4-byte entries.
        video::s_palette_entry* src = reinterpret_cast<video::s_palette_entry*>(loaded_palette) + start_index;
        video::s_palette_entry* dst = &video::_current_palette[start_index];
        uint32_t byte_count = static_cast<uint32_t>(end_index - start_index + 1) * sizeof(uint32_t);

        struct_::Copy_Structure_(dst, src, byte_count);

        // The mouse palette is offset by 1024 bytes (256 colors * 4 bytes) into the loaded buffer.
        video::_mouse_palette = reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(loaded_palette) + 0x400);

        // Signal that the specified color range has been modified.
        palette::Set_Palette_Changes_(start_index, end_index);

        // Internal state update.
        FUN_00526f80();
    }

    void Set_PSTR_Style_(uint16_t style) {
        _PSTR_style = style;
    }

    void __cdecl Set_Font_Spacing_(int16_t spacing) {
        _font_header.font_style_spacing = spacing;
    }

    void __cdecl Set_Font_Style_Outline_Heavy_(int16_t font_index, MISC::s_colors* style_data) {
        Set_Font_Style_(static_cast<int32_t>(font_index), style_data);
        _font_header.font_style = 5;
    }

    void __cdecl Set_Outline_Color_(uint16_t color, uint16_t alias_color) {
        _outline_color = color;
        _alias_outline_color = alias_color;
    }

    void __cdecl Set_Font_Style_(int font_style, MISC::s_colors *new_colors) {
        _current_font_style = static_cast<uint16_t>(font_style);

        for (int i = 0; i < 8; ++i) {
            uint8_t color = new_colors->colors[i];
            _font_header.selected_colors1[i] = color;
            _font_header.selected_colors2[i] = color;
            _current_font_color_array.colors[i] = color;
        }

        _font_header.current_height = _font_header.font_heights[font_style];
        _font_header.font_style_height = _font_header.font_heights[font_style];
        _font_header.font_style = 0;
        _font_header.field24_0x28 = _font_header.field31_0x54c[font_style];
        _font_header.font_style_spacing = _font_header.font_spacing[font_style];
        _font_header.line_height = _font_header.font_style_height + _font_header.field24_0x28;

        for (int i = 0; i < 255; ++i) {
            _font_header.font_style_widths[i] = _font_header.font_glyph_widths[font_style][i];
            _font_header.font_style_offsets[i] = _font_header.font_glyph_offsets[font_style][i];
        }
    }

    int16_t __cdecl Get_String_Width_(const char* str) {
        int16_t total_width = 0;

        if (str == nullptr || *str == '\0') {
            return 0;
        }

        for (int32_t i = 0; str[i] != '\0'; ++i) {
            char c = str[i];

            if (c == 0x15) {
                break;
            }

            if (c >= 0x20) {
                uint8_t char_idx = static_cast<uint8_t>(c);
                total_width += static_cast<int16_t>(_font_header.font_style_widths[char_idx]) +
                               _font_header.font_style_spacing;
            }
        }

        return total_width;
    }

    int16_t __cdecl Print_String_(int16_t x, int16_t y, const char* str, int16_t process_colors, int16_t justified_width) {
        _print_xpos = (int16_t)x;
        _print_ypos = (int16_t)y;

        int32_t pixels_per_space = 0;
        int32_t remainder_pixels = 0;
        int32_t space_index = 0;

        if (justified_width != 0) {
            int32_t space_count = 0;
            int32_t k = 0;
            while (str[k] != '\0' && str[k] != '\r' && str[k] != 0x14 && 
                   str[k] != 0x19 && str[k] != 0x15 && str[k] != 0x1D) {
                if (str[k] == ' ') {
                    space_count++;
                }
                k++;
            }

            if (space_count == 0) {
                justified_width = 0;
            } else {
                int16_t text_width = Get_String_Width_(str);
                int32_t extra_width = (int32_t)justified_width - (int32_t)text_width;

                if (extra_width < 1) {
                    justified_width = 0;
                } else {
                    pixels_per_space = extra_width / space_count;
                    remainder_pixels = extra_width % space_count;
                }
            }
        }

        int32_t i = 0;
        while (str[i] != '\0') {
            char c = str[i];
            
            switch (c) {
            case '\x01':
                if (process_colors != 0) Set_Normal_Colors_On_();
                break;
            case '\x02':
                if (process_colors != 0) Set_Highlight_Colors_On_();
                break;
            case '\x03':
                if (process_colors != 0) Set_Special_Colors_On_();
                break;
            case '\x04':
                if (process_colors != 0) Set_Highlight_Colors_On_();
                break;
            case '\x0D': // '\r'
            case '\x14':
                _print_ypos += _font_header.line_height;
                _print_xpos = x;
                break;
            case '\x15':
                return _print_xpos;
            case '\x19':
            case '\x1D':
                _print_xpos = (int16_t)((int8_t)str[i + 1] + x);
                i++;
                break;
            case '\x08':
                break;
            default:
                if (_draw_alias_flag == 0) {
                    _print_xpos = Print_Character_(_print_xpos, _print_ypos, c);
                } else {
                    _print_xpos = Print_Character_No_Alias_(_print_xpos, _print_ypos, c);
                }

                if (justified_width != 0 && c == ' ') {
                    _print_xpos += (int16_t)pixels_per_space;
                    if (space_index < remainder_pixels) {
                        _print_xpos++;
                    }
                    space_index++;
                }
                break;
            }
            i++;
        }

        return _print_xpos;
    }

    int16_t __cdecl Print_Character_(int16_t x, int16_t y, char c) {
        uint32_t res_x = (uint32_t)platform::Lock_Appropriate_Render_Buffer();
        platform::g_screen_res_x = (int32_t)res_x;

        uint8_t* dest_ptr = reinterpret_cast<uint8_t*>(video::_current_video_page) + (static_cast<int32_t>(y) * res_x) + x;

        uint32_t offset = _font_header.font_style_offsets[c];
        uint8_t* glyph_ptr = reinterpret_cast<uint8_t*>(_font_data) + offset;

        Print_Character_ASM_(dest_ptr, glyph_ptr, reinterpret_cast<uint8_t*>(&_font_header), (int16_t)_font_header.current_height);

        int16_t char_width = static_cast<int16_t>(_font_header.font_style_widths[c]);
        int16_t total_width = char_width + _font_header.font_style_spacing;
        int16_t next_x = x + total_width;

        platform::Unlock_Appropriate_Render_Buffer();

        return next_x;
    }

    int16_t __cdecl Print_Character_No_Alias_(int16_t x, int16_t y, char character) {
        platform::g_screen_res_x = (int32_t)platform::Lock_Appropriate_Render_Buffer();

        uint8_t char_idx = static_cast<uint8_t>(character);

        uint32_t pixel_offset = static_cast<uint32_t>(y) * (uint32_t)platform::g_screen_res_x + static_cast<uint32_t>(x);
        uint8_t* dest_ptr = reinterpret_cast<uint8_t*>(video::_current_video_page) + pixel_offset;

        uint8_t* glyph_ptr = reinterpret_cast<uint8_t*>(_font_data) + _font_header.font_style_offsets[char_idx];

        Print_Character_No_Alias_ASM_(
            dest_ptr, 
            glyph_ptr, 
            &_font_header, 
            static_cast<int>(_font_header.current_height)
        );

        int16_t updated_x = x + static_cast<int16_t>(_font_header.font_style_widths[char_idx]) + _font_header.font_style_spacing;

        platform::Unlock_Appropriate_Render_Buffer();

        return updated_x;
    }

    void __cdecl Print_To_Bitmap_(int16_t x, int16_t y, const char* text, void* bitmap) {
        Print_Display_To_Bitmap_(x, y, text, 0, bitmap);
    }

    void __cdecl Print_Centered_(int16_t x, int16_t y, const char* str) {
        int16_t width = Get_String_Width_(str);
        Print_(static_cast<int16_t>(x - (width / 2)), y, str);
    }

    void __cdecl Print_Right_(int16_t x, int16_t y, const char* text) {
        int16_t width = Get_String_Width_(text);
        int16_t adjusted_x = static_cast<int16_t>(x - (width - 1));
        Print_(adjusted_x, y, text);
    }

    void __cdecl Get_Colors_(uint8_t* colors) {
        for (int i = 0; i < 8; ++i) {
            colors[i] = _font_header.selected_colors2[i];
        }
    }

    void __cdecl Set_Colors_(const uint8_t* colors) {
        for (int i = 0; i < 8; ++i) {
            uint8_t color = colors[i];
            _font_header.selected_colors1[i] = color;
            _font_header.selected_colors2[i] = color;
        }
    }
}