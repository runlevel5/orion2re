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
        int16_t result; // BUG: Original code uses uninitialized stack memory if param_2 > 99

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

    void __cdecl Calc_FP_Offsets_(int16_t* fp_x, int16_t* fp_y, int16_t weapon_id, int16_t shot_count, int16_t attacker_owner, int16_t attacker_ship_id, int16_t attacker_picture_num, uint8_t attacker_facing_dir, uint8_t target_bearing, int16_t fp_idx) {
        (void)attacker_ship_id;

        int16_t n_points = N_Fire_Points_(weapon_id, shot_count);

        if ((n_points % 2) == 1 && (n_points - static_cast<int16_t>(fp_idx)) == 1) {
            *fp_x = 0;
            *fp_y = 0;
            return;
        }

        int16_t color_idx = 0;
        if (attacker_owner < 8) {
            color_idx = MOX::_player[attacker_owner].color;
        }

        int point_idx = ((static_cast<int>(fp_idx) + static_cast<int>(target_bearing)) % 4) + (attacker_picture_num * 4);
        int16_t* fp_data = reinterpret_cast<int16_t*>(_fp_data_ptr[color_idx]);

        int32_t local_x = fp_data[point_idx * 2] - 0x1A;
        int32_t local_y = fp_data[point_idx * 2 + 1] - 0x18;

        int rotation_angle = (static_cast<int>(attacker_facing_dir) * -360) / 16;

        angle::Rotate_Coordinates_(rotation_angle, &local_x, &local_y);

        *fp_x = static_cast<int16_t>(local_x);
        *fp_y = static_cast<int16_t>(local_y);
    }
}