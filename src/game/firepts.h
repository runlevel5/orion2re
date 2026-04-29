#pragma once

namespace FIREPTS {
    inline uint8_t* (&_fp_data_ptr)[8] = *reinterpret_cast<uint8_t*(*)[8]>(std::uintptr_t{0x00571E48});
    inline uint8_t*& _fp_seg = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00571E68});

    void __cdecl Load_FP_Data_From_LBX_();

    int16_t __cdecl N_Fire_Points_(int16_t weapon_id, int16_t shot_count);

    void __cdecl Calc_FP_Offsets_(int16_t* fp_x, int16_t* fp_y, int16_t weapon_id, int16_t shot_count, int16_t attacker_owner, int16_t attacker_ship_id, int16_t attacker_picture_num, int16_t attacker_facing_dir, int16_t target_bearing, int32_t fp_idx);
}