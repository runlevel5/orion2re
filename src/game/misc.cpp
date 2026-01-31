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

    int16_t __cdecl Get_Font_Spacing_() {
        return fonts::_font_header.font_style_spacing;
    }

    void __cdecl Increase_Font_Color_Range_(int16_t increment) {
        uint8_t colors[8];
        fonts::Get_Colors_(colors);

        for (int i = 1; i < 8; ++i) {
            int32_t val = static_cast<int32_t>(colors[i]) + increment;
            if (val > 255) {
                val = 255;
            }
            colors[i] = static_cast<uint8_t>(val);
        }

        fonts::Set_Colors_(colors);
    }

    void __cdecl Decrease_Font_Color_Range_(int16_t amount) {
        uint8_t colors[8];
        fonts::Get_Colors_(colors);

        for (int32_t i = 1; i < 8; ++i) {
            int32_t new_val = static_cast<int32_t>(colors[i]) - static_cast<int32_t>(amount);

            if (new_val <= 0) {
                new_val = 0;
            }

            colors[i] = static_cast<uint8_t>(new_val);
        }

        fonts::Set_Colors_(colors);
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
}