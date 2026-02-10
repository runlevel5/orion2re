#include "deps.h"

namespace MISC {
    void __cdecl Increment_Mouse_Clock_Image_() {
        if (_mouse_clock_image_num >= 16) {
            _mouse_clock_image_num = 0;
        }

        uint32_t clock_index = _mouse_clock_image_num >> 2;
        uint8_t* cursor_entry = &_mouse_list_clock[clock_index * 12];

        mouse::Set_Mouse_List_(cursor_entry, 1);

        _mouse_clock_image_num++;
    }

    int16_t Get_Font_Style_() {
        return fonts::_current_font_style;
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

        fonts::Set_Colors_(&colors);
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

        fonts::Set_Colors_(&colors);
    }

    void __cdecl Save_Window_Status_(int16_t* status) {
        graphics::Save_Window_();
        *status = animate::_clipping_flag;
    }

    void __cdecl Restore_Window_Status_(int16_t clip_enabled) {
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

    void __cdecl Fade_Into_Screen_(void (*draw_callback)(), uint8_t* status_flag, int16_t palette_id) {
        fields::Disable_Draw_All_Fields_();

        *status_flag = 1;
        palette::Clear_Palette_Changes_(0, 0xff);
        mouse::Set_Mouse_List_(mouse::_mouse_list_none, 1);

        ERIC::Fast_Fade_Out_();
        fonts::Load_Palette_(palette_id, 0, 0xff);
        palette::Set_Palette_Changes_(0, 0xff);

        FUN_0052b660();
        palstore::Darken_Palette_(100);

        mouse::Set_Mouse_List_(mouse::_mouse_list_arrow, 1);
        video::Set_Page_Off_();

        if (draw_callback) {
            draw_callback();
        }

        palette::Clear_Palette_Changes_(0, 0xff);

        if (MOX::_reloaded_ship_palette == 0) {
            JIM::Fast_Fade_In_();
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
}