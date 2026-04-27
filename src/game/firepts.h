#pragma once

namespace FIREPTS {
    inline uint8_t* (&_fp_data_ptr)[8] = *reinterpret_cast<uint8_t*(*)[8]>(std::uintptr_t{0x00571E48});
    inline uint8_t*& _fp_seg = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00571E68});

    void __cdecl Load_FP_Data_From_LBX_();

    inline int16_t __cdecl N_Fire_Points_(int16_t weapon_id, int16_t shot_count) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005084E0});
        return fn(weapon_id, shot_count);
    }

    inline void __cdecl Calc_FP_Offsets_(int16_t* fp_x, int16_t* fp_y, int16_t weapon_id, int16_t shot_count, int16_t attacker_owner, int16_t attacker_ship_id, int16_t attacker_picture_num, int16_t attacker_facing_dir, int16_t target_bearing, int32_t fp_idx) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00508550});
        fn(fp_x, fp_y, weapon_id, shot_count, attacker_owner, attacker_ship_id, attacker_picture_num, attacker_facing_dir, target_bearing, fp_idx);
    }
}