#include "deps.h"

namespace palstore {
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

            if (Mouse_Button_() != 0) {
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