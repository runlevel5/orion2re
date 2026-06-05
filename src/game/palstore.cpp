#include "pch.h"

namespace palstore {
    void __cdecl Store_Palette_Block_(int32_t first_entry, int32_t entry_count) {
        if (first_entry < first_entry + entry_count) {
            int32_t palette_index = first_entry;
            int32_t remaining = entry_count;

            do {
                video::g_palette_entries[palette_index].peRed = video::_current_palette[palette_index].r << 2;
                video::g_palette_entries[palette_index].peGreen = video::_current_palette[palette_index].g << 2;
                video::g_palette_entries[palette_index].peBlue = video::_current_palette[palette_index].b << 2;
                ++palette_index;
            } while (--remaining != 0);
        }

        video::g_palette->SetEntries(0, first_entry, entry_count, &video::g_palette_entries[first_entry]);
    }

    void __cdecl Darken_Palette_(int32_t darken_percent) {
        tagPALETTEENTRY entries[256];
        int32_t multiplier = 100 - darken_percent;

        for (int32_t i = 1; i < 256; ++i) {
            entries[i].peRed   = (uint8_t)(((video::_current_palette[i].r * multiplier) / 100) << 2);
            entries[i].peGreen = (uint8_t)(((video::_current_palette[i].g * multiplier) / 100) << 2);
            entries[i].peBlue  = (uint8_t)(((video::_current_palette[i].b * multiplier) / 100) << 2);
        }

        video::g_palette->SetEntries(0, 1, 255, &entries[1]);
    }

    void __cdecl Store_Single_Palette_Color_(int32_t palette_index, uint8_t red, uint8_t green, uint8_t blue) {
        video::g_palette_entries[palette_index].peRed = red << 2;
        video::g_palette_entries[palette_index].peGreen = green << 2;
        video::g_palette_entries[palette_index].peBlue = blue * 4;
        video::g_palette->SetEntries(0, palette_index, 1, &video::g_palette_entries[palette_index]);
        video::_current_palette[palette_index].r = red;
        video::_current_palette[palette_index].g = green;
        video::_current_palette[palette_index].b = blue;
    }

    int16_t Slow_Fade_In_() {
        int16_t intensity = 100;
        int16_t input_occurred = 0;
        int16_t exit_loop = 0;

        video::Set_Refresh_Full_();

        while (exit_loop == 0) {
            if (key::Keyboard_Status_() == 1) {
                exit_loop = 1;
                key::Read_Key_();
                input_occurred = 1;
            }

            if (mouse::Mouse_Button_() != 0) {
                exit_loop = 1;
                input_occurred = 1;
            }

            Darken_Palette_((int32_t)intensity);
            RUSS::Mox_Update_();

            if (intensity-- == 0) {
                break;
            }
        }

        video::Set_Refresh_Stencil_();

        if (input_occurred == 1) {
            JIM::_darken_palette_value = intensity;
        }

        return input_occurred;
    }
}