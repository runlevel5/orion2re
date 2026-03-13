#pragma once
#include <cstdint>

namespace CMBTMOV1 {
    inline int8_t __cdecl Ship_Is_In_Stasis_(int16_t ship_idx) {
        using Fn = int8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046EDE0});
        return fn(ship_idx);
    }

    inline void __cdecl Rotate_Ship_(int16_t ship_idx, int16_t new_dir, int16_t rot_x, int16_t rot_y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004700D0});
        fn(ship_idx, new_dir, rot_x, rot_y);
    }

    inline void __cdecl Wait_Until_Net_Opponent_Finished_(int16_t param_1) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004706E0});
        fn(param_1);
    }

    inline int16_t __cdecl Move_Cost_(int16_t ship_idx, int16_t x, int16_t y) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00470480});
        return fn(ship_idx, x, y);
    }

    inline int16_t __cdecl Move_Ship_(int16_t ship_idx, int16_t x, int16_t y, int16_t cost) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046F810});
        return fn(ship_idx, x, y, cost);
    }

    inline void __cdecl Turn_To_Face_(int16_t ship_idx, int16_t target_x, int16_t target_y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00470560});
        fn(ship_idx, target_x, target_y);
    }
}