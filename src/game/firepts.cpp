#include "pch.h"

namespace FIREPTS {
    void __cdecl Load_FP_Data_From_LBX_() {
        _fp_seg = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x1B90);

        for (int16_t i = 0; i < 8; ++i) {
            if (i == 0) {
                _fp_data_ptr[i] = (uint8_t*)farload::Far_Reload_Data_(
                    COLONY::_FIREPTS_LBX, 0, _fp_seg, i, 1, 0x310);
            } else {
                _fp_data_ptr[i] = (uint8_t*)farload::Far_Reload_Next_Data_(
                    COLONY::_FIREPTS_LBX, 0, _fp_seg, i, 1, 0x310);
            }
        }
    }

    int16_t __cdecl N_Fire_Points_(int16_t param_1, int16_t param_2) {
        int16_t result = 0; // BUG: Original code uses uninitialized stack memory if param_2 > 99

        if (param_2 <= 10 || BEAMS::_g_firing_reflected_beam_flag != 0) {
            result = 1;
        } else if (param_2 < 30) {
            result = 2;
        } else if (param_2 < 60) {
            result = 3;
        } else if (param_2 <= 98) {
            result = 4;
        } else if (param_2 == 99) {
            result = 5;
        }

        if (param_1 == 27 && result <= 2) {
            result = 2;
        }

        return result;
    }
}