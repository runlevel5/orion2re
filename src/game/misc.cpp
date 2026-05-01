#include "pch.h"

namespace MISC {
    void __cdecl Increment_Mouse_Clock_Image_() {
        if (_mouse_clock_image_num >= 16) {
            _mouse_clock_image_num = 0;
        }

        uint32_t clock_index = _mouse_clock_image_num >> 2;
        s_mouse_list* cursor_entry = &_mouse_list_clock[clock_index];

        mouse::Set_Mouse_List_(cursor_entry, 1);

        _mouse_clock_image_num++;
    }

    int16_t Get_Font_Style_() {
        return fonts::_current_font_style;
    }

    void __cdecl Set_Font_Colors_(const s_colors* colors) {
        for (int i = 0; i < 8; ++i) {
            uint8_t color = colors->colors[i];
            fonts::_font_header.selected_colors.colors[i] = color;
            fonts::_font_header.normal_colors.colors[i] = color;
        }
    }

    s_colors* __cdecl Font_Colors2_(int16_t mode, uint8_t param_2, uint8_t param_3) {
        for (int i = 0; i < 8; ++i) {
            _mox_font_colors.colors[i] = 0;
        }

        switch (mode) {
        case 0:
        case 1:
            _mox_font_colors.colors[0] = param_2;
            for (int i = 1; i < 8; ++i) {
                _mox_font_colors.colors[i] = param_3;
            }
            break;

        case 2:
        case 3:
            _mox_font_colors.colors[0] = param_2;
            for (int i = 1; i < 8; ++i) {
                _mox_font_colors.colors[i] = param_3;
            }
            if (MOX::_mox_font_colors_offset != 0) {
                _mox_font_colors.colors[2] = MOX::_mox_font_colors_offset;
            } else {
                int val = (int)param_2 + 1;
                if (val > (int)param_3) {
                    val = (int)param_3;
                }
                _mox_font_colors.colors[2] = static_cast<uint8_t>(val);
            }
            break;

        case 4:
            _mox_font_colors.colors[0] = param_2;
            for (int i = 1; i < 8; ++i) {
                _mox_font_colors.colors[i] = param_3;
            }
            if (MOX::_mox_font_colors_offset != 0) {
                _mox_font_colors.colors[2] = MOX::_mox_font_colors_offset;
            } else {
                _mox_font_colors.colors[2] = param_3 - 1;
            }
            break;

        case 5:
            _mox_font_colors.colors[1] = param_2;
            _mox_font_colors.colors[2] = param_3 - 1;
            _mox_font_colors.colors[3] = param_3 - 1;
            _mox_font_colors.colors[4] = param_3 - 1;
            _mox_font_colors.colors[5] = param_3;
            _mox_font_colors.colors[6] = param_3;
            _mox_font_colors.colors[7] = param_3;
            break;

        default:
            break;
        }

        MOX::_mox_font_colors_offset = 0;
        return &_mox_font_colors;
    }

    void __cdecl Get_Mox_Font_Colors_(int16_t font_idx, uint8_t color_idx_1, uint8_t color_idx_2, s_colors* out_colors) {
        s_colors* colors_ptr = MISC::Font_Colors2_(font_idx, color_idx_1, color_idx_2);

        uint32_t* src = reinterpret_cast<uint32_t*>(colors_ptr);
        uint32_t* dst = reinterpret_cast<uint32_t*>(out_colors);

        dst[0] = src[0];
        dst[1] = src[1];
    }

    int16_t __cdecl Get_Font_Spacing_() {
        return fonts::_font_header.font_style_spacing;
    }

    void __cdecl Increase_Font_Color_Range_(int16_t increment) {
        s_colors colors;
        fonts::Get_Colors_(&colors);

        for (int i = 1; i < 8; ++i) {
            int32_t val = static_cast<int32_t>(colors.colors[i]) + increment;
            if (val > 255) {
                val = 255;
            }
            colors.colors[i] = static_cast<uint8_t>(val);
        }

        MISC::Set_Font_Colors_(&colors);
    }

    void __cdecl Decrease_Font_Color_Range_(int16_t amount) {
        s_colors colors;
        fonts::Get_Colors_(&colors);

        for (int32_t i = 1; i < 8; ++i) {
            int32_t new_val = static_cast<int32_t>(colors.colors[i]) - static_cast<int32_t>(amount);

            if (new_val <= 0) {
                new_val = 0;
            }

            colors.colors[i] = static_cast<uint8_t>(new_val);
        }

        MISC::Set_Font_Colors_(&colors);
    }

    void __cdecl Save_Window_Status_(void* status) {
        graphics::Save_Window_();
        *(int16_t*)status = animate::_clipping_flag;
    }

    void __cdecl Restore_Window_Status_(uint32_t status) {
        int16_t clip_enabled = (int16_t)status;
        graphics::Restore_Window_();
        if (clip_enabled != 0) {
            video::Clip_On_();
        } else {
            video::Clip_Off_();
        }
    }

    void __cdecl Get_LBX_Message_(int16_t message_id, char* buffer) {
        const char* src = HAROLD::H_Message_(message_id);

        if (src != nullptr) {
            char* dst = buffer;
            while ((*dst++ = *src++) != '\0');
        }
    }

    void __cdecl Replace_Bitmap_Colors_(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end,
                                       uint8_t unused_flag, s_colors* old_colors, s_colors* new_colors,
                                       int color_count, s_animation_header* anim) {
        int16_t width;
        int16_t height;

        MISC::Get_Bitmap_Dimensions_(anim, &width, &height);

        if (color_count != 0) {
            x_start = 0;
            y_start = 0;
            x_end = width;
            y_end = height;
        }

        uint8_t* pixel_data = reinterpret_cast<uint8_t*>(anim) + 16;

        for (int16_t y = y_start; y < y_end; ++y) {
            int32_t y_offset = (int32_t)y * width;
            for (int16_t x = x_start; x < x_end; ++x) {
                int32_t offset = y_offset + x;
                uint8_t current_pixel = pixel_data[offset];

                if (color_count > 0) {
                    for (int i = 0; i < color_count; ++i) {
                        if (static_cast<int8_t>(old_colors->colors[i]) == static_cast<int8_t>(current_pixel)) {
                            current_pixel = new_colors->colors[i];
                            pixel_data[offset] = current_pixel;
                        }
                    }
                }
            }
        }
    }

    void __cdecl Fade_Into_Screen_(void (*draw_callback)(), uint8_t* status_flag, int16_t palette_id) {
        fields::Disable_Draw_All_Fields_();

        *status_flag = 1;
        palette::Clear_Palette_Changes_(0, 0xff);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);

        ERIC::Fast_Fade_Out_();
        fonts::Load_Palette_(palette_id, 0, 0xff);
        palette::Set_Palette_Changes_(0, 0xff);

        palette::Refresh_Palette_();
        palstore::Darken_Palette_(100);

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        video::Set_Page_Off_();

        if (draw_callback) {
            draw_callback();
        }

        palette::Clear_Palette_Changes_(0, 0xff);

        if (MOX::_reloaded_ship_palette == 0) {
            ERIC::Fast_Fade_In_();
        } else {
            ERIC::Set_Fade_Percent_(0);
            palstore::Darken_Palette_(0);
        }

        RUSS::Mox_Sync_Update_();
        MOX::_reloaded_ship_palette = 0;

        fields::Enable_Draw_All_Fields_();
    }

    void __cdecl _Darken_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        if (animate::_clipping_flag == 1) {
            if (x1 < video::_screen_window_x1) {
                x1 = video::_screen_window_x1;
            }
            if (y1 < video::_screen_window_y1) {
                y1 = video::_screen_window_y1;
            }
            if (x2 > video::_screen_window_x2) {
                x2 = video::_screen_window_x2;
            }
            if (y2 > video::_screen_window_y2) {
                y2 = video::_screen_window_y2;
            }
        }

        graphics::Tint_Fill_(x1, y1, x2, y2, 0);
    }

    void __cdecl Change_Font_Style_(int16_t font_id) {
        s_colors colors;
        int16_t saved_style = fonts::_font_header.font_style;

        fonts::Get_Colors_(&colors);
        fonts::Set_Font_Style_(static_cast<int32_t>(font_id), &colors);

        fonts::_font_header.font_style = saved_style;
    }

    void __cdecl Set_Font_Default_Spacing_() {
        fonts::_font_header.font_style_spacing =
            fonts::_font_header.font_spacing[fonts::_current_font_style];
    }

    int16_t __cdecl Bit_Value_To_Num_(int32_t value, int16_t bit_limit) {
        int16_t first_bit_index = 0;
        bool bit_found = false;
        int16_t current_bit = 0;

        if (bit_limit > 0) {
            do {
                if (bit_found) {
                    break;
                }

                if ((value >> (current_bit & 0x1F)) & 1) {
                    first_bit_index = current_bit;
                    bit_found = true;
                }

                current_bit++;
            } while (current_bit < bit_limit);
        }

        return first_bit_index;
    }

    void __cdecl Set_Fitted_Font_Style_(char* text, s_colors* colors, int16_t initial_style, int16_t initial_spacing, int16_t variant, int16_t max_width, int16_t min_style) {
        int16_t current_style = initial_style;
        int16_t current_spacing = initial_spacing;
        bool is_finished = false;

        if (variant == -1) {
            variant = 0;
        }

        do {
            fonts::Set_Font_Style_(current_style, colors);
            fonts::Set_Font_Spacing_(current_spacing);
            fonts::_font_header.font_style = variant;

            if (fonts::Get_String_Width_(text) < max_width) {
                is_finished = true;
            } else {
                current_spacing--;

                if (current_spacing < 1) {
                    current_spacing = initial_spacing;
                    current_style--;

                    if (current_style < min_style) {
                        is_finished = true;
                    }
                }
            }
        } while (!is_finished);
    }

    void __cdecl Replace_Char_(char* str, char target, char replacement) {
        int16_t length = static_cast<int16_t>(strlen(str));

        if (length <= 0) {
            return;
        }

        for (int16_t i = 0; i < length; ++i) {
            if (str[i] == target) {
                str[i] = replacement;
            }
        }
    }

    bool __cdecl Occluded_(int16_t x1_min, int16_t y1_min, int16_t x1_max, int16_t y1_max,
                           int16_t x2_min, int16_t y2_min, int16_t x2_max, int16_t y2_max) {
        if (((x2_min <= x1_min && x1_min <= x2_max) ||
             (x2_min <= x1_max && x1_max <= x2_max) ||
             (x1_min <= x2_min && x2_min <= x1_max) ||
             (x1_min <= x2_max && x2_max <= x1_max))) {

            if (((y2_min <= y1_min && y1_min <= y2_max) ||
                 (y2_min <= y1_max && y1_max <= y2_max) ||
                 (y1_min <= y2_min && y2_min <= y1_max) ||
                 (y1_min <= y2_max && y2_max <= y1_max))) {
                return true;
            }
        }

        return false;
    }

    bool __cdecl Is_Vowel_(char c) {
        bool is_vowel = false;

        switch (c) {
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            is_vowel = true;
            break;
        default:
            break;
        }

        if (MOX::_settings.language == 2) {
            switch (c) {
            case '#':
            case '$':
            case '&':
            case '=':
            case '>':
            case '[':
            case ']':
            case '{':
            case '|':
            case '}':
                is_vowel = true;
                break;
            default:
                break;
            }
        }

        return is_vowel;
    }

    void __cdecl Draw_Highlight_Box_(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t* colors) {
        int16_t x2 = x + w;
        int16_t y2 = y + h;
        uint8_t c0 = colors[0];
        uint8_t c1 = colors[1];
        uint8_t c2 = colors[2];
        uint8_t c3 = colors[3];

        // Outer Top and Right edges
        line::Line_(x, y, x2, y, c3);
        line::Line_(x2, y, x2, y2, c3);

        int16_t x_inner = x + 1;
        int16_t y_inner = y + 1;
        int16_t x2_inner = x2 - 1;
        int16_t y2_inner = y2 - 1;

        // Inner Top and Right edges
        line::Line_(x_inner, y_inner, x2_inner, y_inner, c2);
        line::Line_(x2_inner, y_inner, x2_inner, y2_inner, c2);

        // Outer Left and Bottom edges
        line::Line_(x, y_inner, x, y2, c0);
        line::Line_(x, y2, x2, y2, c0);

        // Inner Left and Bottom edges
        line::Line_(x_inner, y_inner, x_inner, y2_inner, c1);
        line::Line_(x_inner, y2_inner, x2_inner, y2_inner, c1);
    }

    void __cdecl Draw_Darkened_Box_(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t color_base) {
        uint8_t colors[4];

        colors[0] = color_base;
        colors[1] = color_base + 1;
        colors[2] = color_base + 2;
        colors[3] = color_base + 3;

        // Darken the inner area of the box, offset by 1 pixel from the highlight border
        MISC::_Darken_Fill_(x + 3, y + 3, x + width + 1, y + height + 1);

        // Draw the decorative highlighted border
        MISC::Draw_Highlight_Box_(x + 2, y + 2, width, height, colors);
    }

    void __cdecl Get_Bitmap_Dimensions_(s_animation_header* header, int16_t* width, int16_t* height) {
        *width = header->width;
        *height = header->height;
    }

    void __cdecl Swap_Bitmap_Colors_(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, void* old_colors_ptr, void* new_colors_ptr, int16_t num_colors, s_animation_header* bitmap) {
        int16_t width;
        int16_t height;

        Get_Bitmap_Dimensions_(bitmap, &width, &height);

        uint8_t* old_colors = static_cast<uint8_t*>(old_colors_ptr);
        uint8_t* new_colors = static_cast<uint8_t*>(new_colors_ptr);

        // Pixel data starts 16 bytes after the start of the header.
        uint8_t* pixel_data = reinterpret_cast<uint8_t*>(bitmap) + 16;

        for (int16_t y = y_start; y < y_end; ++y) {
            int y_offset = (int)y * (int)width;
            for (int16_t x = x_start; x < x_end; ++x) {
                int pixel_idx = (int)x + y_offset;
                uint8_t current_color = pixel_data[pixel_idx];

                if (num_colors > 0) {
                    for (int16_t i = 0; i < num_colors; ++i) {
                        if (old_colors[i] == current_color) {
                            pixel_data[pixel_idx] = new_colors[i];
                            break;
                        }
                    }
                }
            }
        }
    }

    void __cdecl Tint_Bitmap_Colors_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool full_bitmap, uint8_t tint_value, s_animation_header* anim) {
        int16_t width;
        int16_t height;

        Get_Bitmap_Dimensions_(anim, &width, &height);

        if (full_bitmap) {
            x1 = 0;
            y2 = height;
            y1 = 0;
            x2 = width;
        }

        // Bitmap pixels start after the header (12 bytes) and the first frame offset (4 bytes)
        uint8_t* pixels = reinterpret_cast<uint8_t*>(anim) + 16;

        for (int16_t y = y1; y < y2; ++y) {
            for (int16_t x = x1; x < x2; ++x) {
                // Calculation: x + width * y
                uint8_t& pixel = pixels[x + static_cast<int32_t>(width) * y];
                if (pixel != 0) {
                    pixel += tint_value;
                }
            }
        }
    }

    void __cdecl Get_Bitmap_String_Range_(int16_t segment_idx, int16_t divisor, int16_t* out_start_x, int16_t* out_start_y, int16_t* out_end_x, int16_t* out_end_y, char* str) {
        int16_t string_width = fonts::Get_String_Width_(str);

        // This loop represents the REPNE SCASB instruction (0049e89a).
        // It advances the pointer to the end of the string, but the result is not used.
        char* scan_ptr = str;
        while (*scan_ptr++) {}

        // Calculate scaled width of a single segment
        int32_t scaled_width = (((int32_t)string_width * 10) / (int32_t)divisor + 9) / 10;
        int16_t font_height = fonts::Get_Font_Height_();

        // Calculate starting X coordinate and clamp to string width
        int16_t start_x = segment_idx * (int16_t)scaled_width;
        *out_start_x = start_x;
        if (string_width < start_x) {
            *out_start_x = string_width;
        }

        // Calculate ending X coordinate and clamp to string width
        int16_t end_x = *out_start_x + (int16_t)scaled_width;
        *out_end_x = end_x;
        if (string_width < end_x) {
            *out_end_x = string_width;
        }

        *out_start_y = 0;
        *out_end_y = font_height;
    }

    void __cdecl Get_Advancing_Bitmap_String_Range_(int16_t tick, int16_t max_width, int16_t* out_start_x, int16_t* out_start_y, int16_t* out_end_x, int16_t* out_end_y, const char* text, uint8_t is_full_range) {
        int16_t string_width = fonts::Get_String_Width_(text);
        int16_t font_height = fonts::Get_Font_Height_();

        if (string_width < max_width) {
            *out_start_x = 0;
            *out_end_x = 0;

            int divisor = ((max_width * 10 / (int)string_width) + 9) / 10;
            if ((tick % divisor == 0) || (is_full_range != 0)) {
                int32_t start_x_calc = (tick * (int32_t)string_width * 10) / max_width;
                int16_t start_x = (int16_t)(start_x_calc / 10);

                if (start_x > string_width) {
                    start_x = string_width;
                }
                *out_start_x = start_x;

                if (is_full_range != 0) {
                    *out_end_x = string_width;
                } else {
                    *out_end_x = start_x + 1;
                }

                if (*out_end_x > string_width) {
                    *out_end_x = string_width;
                }
            }
        } else {
            int16_t current_tick;
            if (is_full_range != 0) {
                current_tick = max_width - 1;
            } else {
                current_tick = tick;
            }

            int step = ((string_width * 10 / (int)max_width) + 9) / 10;

            int16_t start_x = (int16_t)(current_tick * step);
            if (start_x > string_width) {
                start_x = string_width;
            }
            *out_start_x = start_x;

            int16_t end_x = start_x + (int16_t)step;
            if (end_x > string_width) {
                end_x = string_width;
            }
            *out_end_x = end_x;
        }

        *out_start_y = 0;
        *out_end_y = font_height;
    }

    void __cdecl Draw_Advancing_Fill_Message_(uint8_t font_style, int16_t* p_frame_index, int16_t total_frames,
                                              s_colors* p_colors, s_colors* target_color, int16_t x, int16_t y,
                                              char* text, s_animation_header* anim, uint8_t alignment) {
        int16_t rect_x1;
        int16_t rect_y1;
        int16_t rect_x2;
        int16_t rect_y2;

        fonts::Set_Font_Style_(static_cast<uint16_t>(font_style), p_colors);

        MISC::Get_Advancing_Bitmap_String_Range_(*p_frame_index, total_frames,
                                                 &rect_x1, &rect_y1, &rect_x2, &rect_y2,
                                                 text, alignment);

        RUSS::Mox_Update_();

        MISC::Replace_Bitmap_Colors_(0, 0, rect_x2, rect_y2, 0, p_colors, target_color, 8, anim);

        RUSS::Mox_Update_();

        animate::Draw_(x, y, anim);

        *p_frame_index += 1;
    }

    /**
     * @address 0049df10
     */
    char* __cdecl Get_Comma_String_(int32_t val) {
        char separator = ',';
        if (settings::language == 1 /* French/German separator variant */) {
            separator = ' ';
        }

        // Calculate absolute value manually as in assembly sequence
        int32_t abs_val = (val < 0) ? -val : val;

        char raw_buf[32];
        _sprintf(raw_buf, strings::s_ld_00558280, abs_val);

        int16_t len = (int16_t)strlen(raw_buf);
        int16_t num_separators = (len - 1) / 3;

        MOX::_temp_string[0] = '\0';

        if (num_separators == 0) {
            // Using raw_buf as format string to copy into temp_string
            _sprintf(MOX::_temp_string, raw_buf);
        } else {
            char grouped_buf[32];
            int16_t src_idx = len - 1;
            int16_t group_counter = 0;
            int16_t sep_count = num_separators;
            int16_t total_len = len + num_separators;

            // Build the string with separators backwards
            for (int16_t dst_idx = total_len; dst_idx >= 0; --dst_idx) {
                if (dst_idx == total_len) {
                    grouped_buf[dst_idx] = '\0';
                } else {
                    // Check if current position is a grouping boundary (every 4th char from end)
                    if ((group_counter % 4 == 0) && (group_counter > 1) && (sep_count > 0)) {
                        grouped_buf[dst_idx] = separator;
                        sep_count--;
                    } else {
                        grouped_buf[dst_idx] = raw_buf[src_idx];
                        src_idx--;
                    }
                }
                group_counter++;
            }

            // Binary uses hand-rolled MOVSD/MOVSB rep sequence to copy result
            strcpy(MOX::_temp_string, grouped_buf);
        }

        // Apply negative sign and shift string if necessary
        if (val < 0) {
            size_t current_len = strlen(MOX::_temp_string);
            // Shift string to the right by 1 to make room for '-'
            memcpy(&MOX::_temp_string[1], MOX::_temp_string, current_len);
            MOX::_temp_string[0] = '-';
            MOX::_temp_string[current_len + 1] = '\0';
        }

        return MOX::_temp_string;
    }

    void __cdecl Fade_To_Black_() {
        ERIC::Fast_Fade_Out_();
        video::Set_Page_Off_();

        // Fill the screen (640x480) with black color (index 0)
        graphics::Fill_(0, 0, 0x27f, 0x1df, 0);

        RUSS::Mox_Sync_Update_();
    }

    /**
     * @address 0049f120
     */
    void __cdecl _Print_Shadow_Down_(int16_t x, int16_t y, uint8_t shadow_color, const char* text) {
        s_colors orig_colors;
        s_colors shadow_colors;

        fonts::Get_Colors_(&orig_colors);

        shadow_colors.colors[0] = orig_colors.colors[0];
        memset(&shadow_colors.colors[1], shadow_color, 7);

        Set_Font_Colors_(&shadow_colors);
        fonts::Print_(x + 1, y + 1, text);

        Set_Font_Colors_(&orig_colors);
        fonts::Print_(x, y, text);
    }

    /**
     * @address 0049f0e0
     */
    void __cdecl _Print_Centered_Shadow_Down_(int16_t x, int16_t y, uint8_t color, const char* text) {
        int16_t width = fonts::Get_String_Width_(text);

        // Calculate horizontal center
        int16_t centered_x = x - (width / 2);

        MISC::_Print_Shadow_Down_(centered_x, y, color, text);
    }

    int16_t __cdecl Substring_(char* haystack, char* needle) {
        char* found_ptr = strstr(haystack, needle);

        if (found_ptr != nullptr) {
            return static_cast<int16_t>(found_ptr - haystack);
        }

        return static_cast<int16_t>(strlen(haystack));
    }

    void __cdecl Diagonal_Line_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t spacing, uint8_t color1, uint8_t color2) {
        uint16_t window_status;
        Save_Window_Status_(&window_status);

        if (spacing == 0) {
            spacing = 16;
        }

        if (((640 / spacing) * spacing != 640) || ((480 / spacing) * spacing != 480)) {
            spacing = 16;
        }

        graphics::Set_Window_(x1, y1, x2, y2);
        video::Clip_On_();

        int16_t num_steps_y = 480 / spacing;
        int16_t num_steps_x = 640 / spacing;

        if (num_steps_x > 0) {
            int16_t current_x = 0;
            for (int16_t i = num_steps_x; i > 0; --i) {
                line::Line_(current_x - 1, 0, current_x + 479, 480, color2);
                line::Line_(current_x + 1, 0, current_x + 481, 480, color2);
                line::Line_(current_x, 0, current_x + 480, 480, color1);
                current_x += spacing;
            }
        }

        if (num_steps_y > 0) {
            int16_t current_y = 0;
            for (int16_t i = num_steps_y; i > 0; --i) {
                line::Line_(-1, current_y, 479 - current_y, 480, color2);
                line::Line_(1, current_y, 481 - current_y, 480, color2);
                line::Line_(0, current_y, 480 - current_y, 480, color1);
                current_y += spacing;
            }

            if (num_steps_y > 0) {
                current_y = 0;
                for (int16_t i = num_steps_y; i > 0; --i) {
                    line::Line_(current_y - 1, 0, 1, current_y, color2);
                    line::Line_(current_y + 1, 0, 1, current_y, color2);
                    line::Line_(current_y, 0, 0, current_y, color1);
                    current_y += spacing;
                }
            }
        }

        if (num_steps_x > 0) {
            int16_t current_x = 0;
            for (int16_t i = num_steps_x; i > 0; --i) {
                line::Line_(current_x - 1, 480, current_x + 479, 0, color2);
                line::Line_(current_x + 1, 480, current_x + 481, 0, color2);
                line::Line_(current_x, 480, current_x + 480, 0, color1);
                current_x += spacing;
            }
        }

        Restore_Window_Status_(window_status);
    }
}