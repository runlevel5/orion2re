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
}