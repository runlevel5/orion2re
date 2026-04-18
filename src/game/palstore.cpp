#include "pch.h"

namespace palstore {
    void __cdecl Darken_Palette_(int32_t darken_percent) {
        tagPALETTEENTRY entries[256];
        int32_t multiplier = 100 - darken_percent;

        for (int i = 1; i < 256; ++i) {
            entries[i].peRed   = static_cast<uint8_t>(((video::_current_palette[i].r * multiplier) / 100) << 2);
            entries[i].peGreen = static_cast<uint8_t>(((video::_current_palette[i].g * multiplier) / 100) << 2);
            entries[i].peBlue  = static_cast<uint8_t>(((video::_current_palette[i].b * multiplier) / 100) << 2);
        }

        g_palette->SetEntries(0, 1, 255, &entries[1]);
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

            Darken_Palette_(static_cast<int32_t>(intensity));
            MOX::Mox_Update_();

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