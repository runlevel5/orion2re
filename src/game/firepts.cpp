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
}