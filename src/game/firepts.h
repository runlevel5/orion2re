#pragma once

namespace FIREPTS {
    extern uint8_t* _fp_data_ptr[8];
    extern uint8_t* _fp_seg;

    void __cdecl Load_FP_Data_From_LBX_();

    int16_t __cdecl N_Fire_Points_(int16_t weapon_id, int16_t shot_count);

    void __cdecl Calc_FP_Offsets_(int16_t* fp_x, int16_t* fp_y, int16_t weapon_id, int16_t shot_count, int16_t attacker_owner, int16_t attacker_ship_id, int16_t attacker_picture_num, uint8_t attacker_facing_dir, uint8_t target_bearing, int16_t fp_idx);
}