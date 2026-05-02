#include "pch.h"

namespace fonts {
    void __cdecl Set_Normal_Colors_On_(void) {
        for (int i = 0; i < 8; ++i) {
            _font_header.selected_colors.colors[i] = _font_header.normal_colors.colors[i];
        }
    }

    void __cdecl Set_Highlight_Colors_On_(void) {
        for (int i = 0; i < 8; ++i) {
            _font_header.selected_colors.colors[i] = _font_header.highlight_colors.colors[i];
        }
    }

    void __cdecl Set_Special_Colors_On_(void) {
        for (int i = 0; i < 8; ++i) {
            _font_header.selected_colors.colors[i] = _font_header.special_colors.colors[i];
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
        s_palette_entry* src = reinterpret_cast<s_palette_entry*>(loaded_palette) + start_index;
        s_palette_entry* dst = &video::_current_palette[start_index];
        uint32_t byte_count = static_cast<uint32_t>(end_index - start_index + 1) * sizeof(uint32_t);

        struct_::Copy_Structure_(dst, src, byte_count);

        // The mouse palette is offset by 1024 bytes (256 colors * 4 bytes) into the loaded buffer.
        video::_mouse_palette = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(loaded_palette) + 0x400);

        // Signal that the specified color range has been modified.
        palette::Set_Palette_Changes_(start_index, end_index);

        // Internal state update.
        platform::Update_Cursor_Image_();
    }

    void Set_PSTR_Style_(uint16_t style) {
        _PSTR_style = style;
    }

    void __cdecl Set_Font_Spacing_(int16_t spacing) {
        _font_header.font_style_spacing = spacing;
    }

    void __cdecl Set_Font_LF_(uint16_t line_height) {
        _font_header.line_height = line_height;
        _font_header.font_style_height = _font_header.current_height + line_height;
    }

    void __cdecl Set_Font_Style_Outline_Heavy_(int16_t font_index, s_colors* style_data) {
        Set_Font_Style_(static_cast<int32_t>(font_index), style_data);
        _font_header.font_style = 5;
    }

    void __cdecl Set_Outline_Color_(uint16_t color, uint16_t alias_color) {
        _outline_color = color;
        _alias_outline_color = alias_color;
    }

    void __cdecl Set_Remap_Font_Style_(int font_idx, s_colors* colors) {
        _current_font_style = static_cast<int16_t>(font_idx);

        for (int i = 0; i < 8; i++) {
            uint8_t color = remap::_picture_remap_color_list[colors->colors[i]];
            _font_header.normal_colors.colors[i] = color;
            _font_header.selected_colors.colors[i] = color;
            _current_font_color_array.colors[i] = color;
        }

        _font_header.current_height = static_cast<uint16_t>(_font_header.font_heights[font_idx]);
        _font_header.font_style_height = _font_header.font_heights[font_idx];
        _font_header.font_style = 0;
        _font_header.leading_offset = static_cast<uint16_t>(_font_header.leading_extra[font_idx]);
        _font_header.font_style_spacing = _font_header.font_spacing[font_idx];
        _font_header.line_height = _font_header.font_style_height + _font_header.leading_offset;

        // Potential bug: The loop only copies 255 characters, skipping index 255.
        for (int i = 0; i < 255; i++) {
            _font_header.font_style_widths[i] = static_cast<char>(_font_header.font_glyph_widths[font_idx][i]);
            _font_header.font_style_offsets[i] = _font_header.font_glyph_offsets[font_idx][i];
        }
    }

    void __cdecl Set_Font_Style_(int font_style, const s_colors *new_colors) {
        _current_font_style = static_cast<uint16_t>(font_style);

        for (int i = 0; i < 8; ++i) {
            uint8_t color = new_colors->colors[i];
            _font_header.selected_colors.colors[i] = color;
            _font_header.normal_colors.colors[i] = color;
            _current_font_color_array.colors[i] = color;
        }

        _font_header.current_height = _font_header.font_heights[font_style];
        _font_header.font_style_height = _font_header.font_heights[font_style];
        _font_header.font_style = 0;
        _font_header.leading_offset = _font_header.leading_extra[font_style];
        _font_header.font_style_spacing = _font_header.font_spacing[font_style];
        _font_header.line_height = _font_header.font_style_height + _font_header.leading_offset;

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

    int16_t __cdecl Print_String_Clipped_(int16_t x, int16_t y, const char* str, int16_t process_colors, int16_t justified_width) {
        if (y > video::_screen_window_y2) {
            return 1;
        }

        if (y + _font_header.font_style_height < video::_screen_window_y1) {
            return 1;
        }

        _print_xpos = x;
        _print_ypos = y;

        int32_t spaces_remainder = 0;
        int32_t spaces_processed = 0;
        int32_t space_extra_width = 0;

        if (justified_width != 0) {
            int32_t str_len = 0;
            int32_t spaces_count = 0;

            if (str[0] != '\0') {
                do {
                    char c = str[str_len];
                    if (c == '\r' || c == '\x14' || c == '\x19' || c == '\x15' || c == '\x1d') {
                        break;
                    }
                    if (c == ' ') {
                        spaces_count++;
                    }
                    str_len++;
                } while (str[str_len] != '\0');
            }

            if (spaces_count == 0) {
                justified_width = 0;
            }

            justified_width -= Get_String_Width_(str);

            if (justified_width > 0) {
                spaces_remainder = justified_width % spaces_count;
                space_extra_width = justified_width / spaces_count;
            } else {
                justified_width = 0;
            }
        }

        int32_t str_idx = 0;
        while (true) {
            char c = str[str_idx];
            if (c == '\0') {
                break;
            }

            switch (c) {
                case '\x01':
                    if (process_colors != 0) {
                        Set_Normal_Colors_On_();
                    }
                    break;
                case '\x02':
                case '\x04':
                    if (process_colors != 0) {
                        Set_Highlight_Colors_On_();
                    }
                    break;
                case '\x03':
                    if (process_colors != 0) {
                        Set_Special_Colors_On_();
                    }
                    break;
                case '\x08':
                    break;
                case '\r':
                case '\x14':
                    _print_ypos += _font_header.line_height;
                    _print_xpos = x;
                    break;
                case '\x15':
                    return _print_xpos;
                case '\x19':
                case '\x1d':
                    _print_xpos = x + static_cast<uint8_t>(str[str_idx + 1]);
                    str_idx++;
                    break;
                default:
                    _print_xpos = Print_Character_Clipped_(_print_xpos, _print_ypos, c);
                    if (justified_width != 0 && c == ' ') {
                        _print_xpos += (int16_t)space_extra_width;
                        if (spaces_processed < spaces_remainder) {
                            _print_xpos++;
                        }
                        spaces_processed++;
                    }
                    break;
            }
            str_idx++;
        }

        return _print_xpos;
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

        fontdraw::Print_Character_No_Alias_ASM_(
            dest_ptr, 
            glyph_ptr, 
            _font_header.selected_colors.colors, 
            static_cast<int>(_font_header.current_height)
        );

        int16_t updated_x = x + static_cast<int16_t>(_font_header.font_style_widths[char_idx]) + _font_header.font_style_spacing;

        platform::Unlock_Appropriate_Render_Buffer();

        return updated_x;
    }

    void __cdecl Print_String_To_Bitmap_(int16_t x, int16_t y, const char* string, int16_t color_flags, int16_t max_width, s_animation_header* bitmap) {
        _print_xpos = x;
        _print_ypos = y;

        int16_t string_width = Get_String_Width_(string);
        int16_t bitmap_height = animate::Get_Height_(bitmap);

        if (y > bitmap_height) return;
        if (y + _font_header.font_style_height <= 0) return;

        int16_t bitmap_width = animate::Get_Width_(bitmap);
        if (x > bitmap_width) return;
        if (x + string_width <= 0) return;

        int32_t space_index = 0;
        int32_t remainder = 0;
        int32_t quotient = 0;
        int16_t justify_diff = 0;

        if (max_width != 0) {
            int i = 0;
            int space_count = 0;
            if (string[0] != '\0') {
                do {
                    uint8_t c = (uint8_t)string[i];
                    if (c == 0x0D || c == 0x14 || c == 0x19 || c == 0x15 || c == 0x1D) {
                        break;
                    }
                    if (c == 0x20) {
                        space_count++;
                    }
                    i++;
                } while (string[i] != '\0');
            }

            int16_t justify_width = max_width;
            if (space_count == 0) {
                justify_width = 0;
            }

            justify_diff = justify_width - Get_String_Width_(string);

            if (justify_diff <= 0) {
                justify_diff = 0;
            } else {
                remainder = justify_diff % space_count;
                quotient = justify_diff / space_count;
            }
        }

        uint8_t c = (uint8_t)string[0];
        if (c != '\0') {
            int index = 0;
            do {
                switch (c) {
                    case 1:
                        if (color_flags != 0) {
                            Set_Normal_Colors_On_();
                        }
                        break;
                    case 2:
                    case 4:
                        if (color_flags != 0) {
                            Set_Highlight_Colors_On_();
                        }
                        break;
                    case 3:
                        if (color_flags != 0) {
                            Set_Special_Colors_On_();
                        }
                        break;
                    case 8:
                        break;
                    case 0x0D:
                    case 0x14:
                    case 0x15:
                    case 0x19:
                    case 0x1D:
                        return;
                    default:
                        Print_Character_To_Bitmap_(_print_xpos, _print_ypos, (char)c, bitmap);
                        _print_xpos += (int16_t)_font_header.font_style_widths[c] + _font_header.font_style_spacing;
                        if (justify_diff != 0 && c == 0x20) {
                            _print_xpos += (int16_t)quotient;
                            if (space_index < remainder) {
                                _print_xpos++;
                            }
                            space_index++;
                        }
                        break;
                }
                index++;
                c = (uint8_t)string[index];
            } while (c != '\0');
        }
    }

    void __cdecl Print_Display_To_Bitmap_(int16_t x, int16_t y, const char* str, int16_t param_4, s_animation_header* bitmap) {
        animate::_animation_header = bitmap;

        if ((bitmap->flags & 3) != 1) {
            exitm::Exit_With_Message(s_Print_only_works_on_bitmaps);
        }

        if (_font_header.font_style != 0) {
            _font_header.selected_colors.colors[0] = (uint8_t)_alias_outline_color;
            _draw_alias_flag = 1;

            for (int i = 1; i < 8; ++i) {
                _font_header.selected_colors.colors[i] = (uint8_t)_outline_color;
            }

            if (_font_header.font_style != 2) {
                Print_String_To_Bitmap_(x + 1, y + 1, str, 0, param_4, bitmap);
                Print_String_To_Bitmap_(x, y + 1, str, 0, param_4, bitmap);
                Print_String_To_Bitmap_(x + 1, y, str, 0, param_4, bitmap);
            }

            if (_font_header.font_style != 1 && _font_header.font_style != 3) {
                Print_String_To_Bitmap_(x - 1, y, str, 0, param_4, bitmap);
                Print_String_To_Bitmap_(x - 1, y - 1, str, 0, param_4, bitmap);
                Print_String_To_Bitmap_(x, y - 1, str, 0, param_4, bitmap);
            }

            if (_font_header.font_style == 3 || _font_header.font_style == 5) {
                Print_String_To_Bitmap_(x + 2, y + 2, str, 0, param_4, bitmap);
                Print_String_To_Bitmap_(x + 1, y + 2, str, 0, param_4, bitmap);
                Print_String_To_Bitmap_(x + 2, y + 1, str, 0, param_4, bitmap);
            }

            if (_font_header.font_style > 3) {
                Print_String_To_Bitmap_(x + 1, y - 1, str, 0, param_4, bitmap);
                Print_String_To_Bitmap_(x - 1, y + 1, str, 0, param_4, bitmap);
            }

            if (_font_header.font_style == 5) {
                Print_String_To_Bitmap_(x + 2, y, str, 0, param_4, bitmap);
                Print_String_To_Bitmap_(x, y + 2, str, 0, param_4, bitmap);
            }

            for (int i = 0; i < 8; ++i) {
                _font_header.selected_colors.colors[i] = _font_header.normal_colors.colors[i];
            }
        }

        _draw_alias_flag = 0;
        Print_String_To_Bitmap_(x, y, str, 1, param_4, bitmap);
    }

    void __cdecl Print_Character_To_Bitmap_(int16_t x, int16_t y, char ch, s_animation_header* seg) {
        if ((uint8_t)ch > 128) {
            return;
        }

        short font_h = _font_header.font_style_height;
        short glyph_w = (short)_font_header.font_style_widths[(uint8_t)ch];

        short anim_w = animate::Get_Width_(seg);
        short anim_h = animate::Get_Height_(seg);

        short max_x_idx = anim_w - 1;
        short max_y_idx = anim_h - 1;

        short src_x = 0;
        short draw_x = x;
        short clipped_w = glyph_w;

        if (draw_x < 0) {
            src_x = -draw_x;
            if (src_x >= glyph_w) {
                return;
            }
            draw_x = 0;
            clipped_w = glyph_w - src_x;
        }

        if ((int)draw_x + (int)clipped_w > (int)max_x_idx) {
            clipped_w = max_x_idx - draw_x + 1;
            if (clipped_w < 1) {
                return;
            }
        }

        short src_y = 0;
        short draw_y = y;

        if (draw_y < 0) {
            src_y = -draw_y;
            draw_y = 0;
        }

        short clipped_h = font_h;
        if ((int)draw_y + (int)clipped_h > (int)max_y_idx) {
            clipped_h = max_y_idx - draw_y + 1;
        }

        Print_Clipped_Letter_To_Bitmap_(draw_x, draw_y, ch, src_x, clipped_w, src_y, clipped_h, seg);
    }

    void __cdecl Print_To_Bitmap_(int16_t x, int16_t y, const char* text, void* bitmap) {
        Print_Display_To_Bitmap_(x, y, text, 0, (s_animation_header*)bitmap);
    }

    void __cdecl Print_Display_(int16_t x, int16_t y, const char* str, int16_t width) {
        int32_t offset = 0;

        if (_font_header.font_style != 0) {
            for (int i = 1; i < 8; ++i) {
                _font_header.selected_colors.colors[i] = _outline_color;
            }
            _font_header.selected_colors.colors[0] = _alias_outline_color;
            _draw_alias_flag = 1;

            if (_font_header.font_style == 3 || _font_header.font_style == 5) {
                offset = 2;
            } else {
                offset = 1;
            }

            if (_font_header.font_style != 2) {
                if (animate::_clipping_flag == 1) {
                    Print_String_Clipped_(x + 1, y + 1, str, 0, width);
                    Print_String_Clipped_(x, y + 1, str, 0, width);
                    Print_String_Clipped_(x + 1, y, str, 0, width);
                } else {
                    Print_String_(x + 1, y + 1, str, 0, width);
                    Print_String_(x, y + 1, str, 0, width);
                    Print_String_(x + 1, y, str, 0, width);
                }
            }

            if (_font_header.font_style != 1 && _font_header.font_style != 3) {
                if (animate::_clipping_flag == 1) {
                    Print_String_Clipped_(x - 1, y, str, 0, width);
                    Print_String_Clipped_(x - 1, y - 1, str, 0, width);
                    Print_String_Clipped_(x, y - 1, str, 0, width);
                } else {
                    Print_String_(x - 1, y, str, 0, width);
                    Print_String_(x - 1, y - 1, str, 0, width);
                    Print_String_(x, y - 1, str, 0, width);
                }
            }

            if (_font_header.font_style == 3 || _font_header.font_style == 5) {
                if (animate::_clipping_flag == 1) {
                    Print_String_Clipped_(x + 2, y + 2, str, 0, width);
                    Print_String_Clipped_(x + 1, y + 2, str, 0, width);
                    Print_String_Clipped_(x + 2, y + 1, str, 0, width);
                } else {
                    Print_String_(x + 2, y + 2, str, 0, width);
                    Print_String_(x + 1, y + 2, str, 0, width);
                    Print_String_(x + 2, y + 1, str, 0, width);
                }
            }

            if (_font_header.font_style > 3) {
                if (animate::_clipping_flag == 1) {
                    Print_String_Clipped_(x + 1, y - 1, str, 0, width);
                    Print_String_Clipped_(x - 1, y + 1, str, 0, width);
                } else {
                    Print_String_(x + 1, y - 1, str, 0, width);
                    Print_String_(x - 1, y + 1, str, 0, width);
                }
            }

            if (_font_header.font_style == 5) {
                if (animate::_clipping_flag == 1) {
                    Print_String_Clipped_(x + 2, y, str, 0, width);
                    Print_String_Clipped_(x, y + 2, str, 0, width);
                } else {
                    Print_String_(x + 2, y, str, 0, width);
                    Print_String_(x, y + 2, str, 0, width);
                }
            }

            for (int i = 0; i < 8; ++i) {
                _font_header.selected_colors.colors[i] = _font_header.normal_colors.colors[i];
            }
        }

        _draw_alias_flag = 0;

        if (animate::_clipping_flag == 1) {
            Print_String_Clipped_(x, y, str, 1, width);
        } else {
            Print_String_(x, y, str, 1, width);
        }

        int32_t x_start = x - (int16_t)offset;
        int32_t y_start = y - (int16_t)offset;

        int32_t w_val = width;
        if (w_val == 0) {
            w_val = Get_String_Width_(str);
        }

        int32_t x_end = x + w_val + (int16_t)offset;
        int32_t y_end = y + _font_header.font_style_height + (int16_t)offset;

        if (x_start < 0) {
            x_start = 0;
        }
        if (y_start < 0) {
            y_start = 0;
        }

        if (x_end >= platform::g_screen_res_x) {
            x_end = platform::g_screen_res_x - 1;
        }
        if (y_end >= platform::g_screen_res_y) {
            y_end = platform::g_screen_res_y - 1;
        }

        if (video::screen_refresh_flag >= 2) {
            setvesa::Add_Square_To_Scan_List_(x_start, y_start, x_end, y_end);
        }
    }

    void __cdecl Print_(int16_t x, int16_t y, const char* text) {
        Print_Display_(x, y, text, 0);
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

    void __cdecl Get_Colors_(s_colors* colors) {
        for (int i = 0; i < 8; ++i) {
            colors->colors[i] = _font_header.normal_colors.colors[i];
        }
    }

    int16_t __cdecl Get_Current_Font_Style_() {
        return _current_font_style;
    }

    s_colors* __cdecl Get_Current_Font_Color_() {
        return &_current_font_color_array;
    }

    s_colors* __cdecl Get_Current_Highlight_Color_() {
        return &_font_header.highlight_colors;
    }

    s_colors* __cdecl Get_Current_Special_Color_() {
        return &_font_header.special_colors;
    }

    int16_t __cdecl Get_Font_Height_() {
        return _font_header.font_style_height;
    }

    int16_t __cdecl Get_Font_Vertical_Spacing_() {
        return _font_header.leading_offset;
    }

    void __cdecl Set_Font_Style_Outline_(int16_t style, s_colors* colors) {
        Set_Font_Style_(style, colors);
        _font_header.font_style = 4;
    }

    void __cdecl Set_Font_Style_Shadow_Down_(int16_t style, const s_colors* colors) {
        Set_Font_Style_(style, colors);
        _font_header.font_style = 1;
    }

    void __cdecl Set_Font_Style_Shadow_Up_(int16_t font_idx, s_colors* colors) {
        Set_Font_Style_(font_idx, colors);
        _font_header.font_style = 2;
    }

    void __cdecl Set_Font_Special_Colors_(s_colors* colors) {
        for (int i = 0; i < 8; ++i) {
            _font_header.special_colors.colors[i] = colors->colors[i];
        }
    }

    void __cdecl Set_Font_Highlight_Colors_(s_colors* colors) {
        for (int i = 0; i < 8; ++i) {
            _font_header.highlight_colors.colors[i] = colors->colors[i];
        }
    }

    void __cdecl Print_Integer_(int16_t x, int16_t y, int32_t value) {
        char buffer[12];

        itoa(value, buffer, 10);
        Print_(x, y, buffer);
    }

    void __cdecl Print_Integer_Right_(int16_t x, int16_t y, int32_t value) {
        char buffer[12];
        itoa(value, buffer, 10);
        Print_Right_(x, y, buffer);
    }

    void __cdecl Print_Integer_Centered_(int16_t x, int16_t y, int32_t value) {
        char buffer[12];

        itoa(value, buffer, 10);
        Print_Centered_(x, y, buffer);
    }

    void __cdecl Set_Font_Spacing_Width_(int spacing_width) {
        // Sets the width of the space character (ASCII 32)
        _font_header.font_style_widths[32] = (char)spacing_width;
    }

    void __cdecl Set_Alias_Color_(uint8_t color) {
        _font_header.normal_colors.colors[0] = static_cast<uint8_t>(color);
        _font_header.highlight_colors.colors[0] = static_cast<uint8_t>(color);
        _font_header.special_colors.colors[0] = static_cast<uint8_t>(color);
        _font_header.selected_colors.colors[0] = static_cast<uint8_t>(color);
    }

    void __cdecl Print_Centered_To_Bitmap_(int16_t x, int16_t y, const char* text, s_animation_header* bitmap) {
        int16_t width = Get_String_Width_(text);
        Print_To_Bitmap_(x - (width / 2), y, text, bitmap);
    }

    void __cdecl PSTRS_(int16_t x, int16_t y, char* str) {
        Disp_PSTR_(x, y, str);
    }

    void __cdecl Load_Font_File_(char* filename) {
        uint8_t* blob;

        if (_current_font_blob == nullptr) {
            blob = (uint8_t*)farload::Farload_(filename, 0);
            _palette_block = allocate::Allocate_Space_(0x3164);
        } else {
            _free(_current_font_blob);
            _current_font_blob = nullptr;
            blob = (uint8_t*)farload::Farload_(filename, 0);
        }

        _current_font_blob = blob;

        struct_::Copy_Structure_(video::_glass_colors, blob, 0x40);
        blob += 0x40;

        struct_::Copy_Structure_(&_font_header, blob, 0x235C);
        blob += 0x235C;

        _font_data = blob;

        strcpy(_font_name, filename);
    }

    void __cdecl Disp_PSTR_(int x, int y, const char* format, ...) {
        HDC hdc;
        char buffer[100];
        tagSIZE size;

        if (platform::g_primary_surface->GetDC(&hdc) == DD_OK) {
            va_list args;
            va_start(args, format);
            vsprintf(buffer, format, args);
            va_end(args);

            PTR_SetBkMode_005aa3b4(hdc, 1);
            PTR_SetTextColor_005aa3b0(hdc, 0xFFFFFF);

            int len = PTR_lstrlenA_005aa3c4(buffer);
            PTR_GetTextExtentPoint32A_005aa3a8(hdc, buffer, len, &size);

            len = PTR_lstrlenA_005aa3c4(buffer);
            PTR_TextOutA_005aa3ac(hdc, x, y, buffer, len);

            platform::g_primary_surface->ReleaseDC(hdc);

            setvesa::Add_Square_To_Scan_List_(x, y, x + size.cx, y + size.cy);
        }
    }

    void __cdecl Print_Clipped_Letter_To_Bitmap_(int16_t x, int16_t y, char letter, int16_t clip_x, int16_t clip_width, int16_t clip_y, uint16_t clip_height, s_animation_header* anim) {
        animate::_animation_header = anim;

        int16_t current_frame = anim->current_frame;
        int16_t width = anim->width;
        int32_t dest_offset = x + y * width - clip_x;

        // BUG: letter is explicitly sign-extended, which may result in a negative array index for characters >= 128
        int16_t font_offset = static_cast<int16_t>(_font_header.font_style_offsets[static_cast<int8_t>(letter)]);

        if (static_cast<int16_t>(_font_header.current_height) < static_cast<int16_t>(clip_height)) {
            clip_height = _font_header.current_height;
        }

        for (int16_t i = 0; i < clip_y; i++) {
            while (_font_data[font_offset] != 0x80) {
                font_offset++;
            }
            font_offset++;
        }

        int16_t rows = clip_height - clip_y;

        if (rows > 0) {
            uint8_t* frame_ptr = reinterpret_cast<uint8_t*>(anim) + anim->frame_offsets[current_frame];
            do {
                int32_t dest_clip_start = clip_x + dest_offset;
                int32_t dest_clip_end = dest_clip_start + clip_width;

                uint8_t data = _font_data[font_offset];
                if (data != 0x80) {
                    int32_t current_dest_offset = dest_offset;
                    do {
                        if (data <= 0x80) {
                            if (data != 0 && dest_clip_start <= current_dest_offset && current_dest_offset < dest_clip_end) {
                                frame_ptr[current_dest_offset] = (&_palette_block__unk)[data];
                            }
                            current_dest_offset++;
                        } else {
                            current_dest_offset += (data & 0x7F);
                        }
                        font_offset++;
                        data = _font_data[font_offset];
                    } while (data != 0x80);
                }
                font_offset++;
                dest_offset += width;
                rows--;
            } while (rows != 0);
        }
    }

    void __cdecl Print_Clipped_Letter_(int16_t x, int16_t y, char letter, int16_t clip_left, int16_t clip_width, int16_t start_y, int16_t end_y) {
        platform::g_screen_res_x = platform::Lock_Appropriate_Render_Buffer();

        int screen_idx_start = y * platform::g_screen_res_x - clip_left + x;

        if (static_cast<int16_t>(_font_header.current_height) < end_y) {
            end_y = static_cast<int16_t>(_font_header.current_height);
        }

        int16_t offset = static_cast<int16_t>(_font_header.font_style_offsets[static_cast<int>(letter)]);

        for (int i = 0; i < start_y; ++i) {
            while (_font_data[offset] != 0x80) {
                ++offset;
            }
            ++offset;
        }

        int16_t rows_to_draw = end_y - start_y;
        if (rows_to_draw > 0) {
            for (int16_t row = 0; row < rows_to_draw; ++row) {
                int screen_idx = screen_idx_start;
                int clip_start_idx = screen_idx_start + clip_left;
                int clip_end_idx = clip_start_idx + clip_width;

                uint8_t pixel = _font_data[offset];
                if (pixel != 0x80) {
                    do {
                        if (pixel > 0x80) {
                            screen_idx += (pixel & 0x7F);
                        } else {
                            if (screen_idx >= clip_start_idx && screen_idx < clip_end_idx) {
                                video::_current_video_page[screen_idx] = (&_palette_block__unk)[pixel];
                            }
                            ++screen_idx;
                        }
                        ++offset;
                        pixel = _font_data[offset];
                    } while (pixel != 0x80);
                }
                ++offset;
                screen_idx_start += platform::g_screen_res_x;
            }
        }

        platform::Unlock_Appropriate_Render_Buffer();
    }

    int16_t __cdecl Print_Character_Clipped_(int16_t x, int16_t y, char c) {
        int16_t spacing = _font_header.font_style_spacing;

        if (c > 0x80) {
            return x;
        }

        int16_t char_width = static_cast<int16_t>(_font_header.font_style_widths[static_cast<uint8_t>(c)]);

        if (x >= video::_screen_window_x1 &&
            x + char_width <= video::_screen_window_x2 &&
            y >= video::_screen_window_y1 &&
            y + _font_header.font_style_height <= video::_screen_window_y2)
        {
            Print_Character_(x, y, c);
        } else {
            int16_t orig_ret = spacing + char_width + x;
            int16_t clip_x = 0;

            if (x < video::_screen_window_x1) {
                clip_x = video::_screen_window_x1 - x;
                if (char_width <= clip_x) {
                    return orig_ret;
                }
                char_width -= clip_x;
                x = video::_screen_window_x1;
            }

            if (x + char_width > video::_screen_window_x2) {
                char_width = video::_screen_window_x2 - x + 1;
                if (char_width < 1) {
                    return orig_ret;
                }
            }

            int16_t clip_y = 0;
            if (y < video::_screen_window_y1) {
                clip_y = video::_screen_window_y1 - y;
                y = video::_screen_window_y1;
            }

            int16_t print_height = _font_header.font_style_height;
            if (y + _font_header.font_style_height > video::_screen_window_y2) {
                print_height = video::_screen_window_y2 - y + 1;
            }

            Print_Clipped_Letter_(x, y, c, clip_x, char_width, clip_y, print_height);
        }

        return spacing + char_width + x;
    }
}