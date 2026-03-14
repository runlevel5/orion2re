#pragma once
#include <cstdint>

namespace CMBTMOV1 {
    bool __cdecl Ship_Is_In_Stasis_(int16_t ship_idx);

    void __cdecl Rotate_Ship_(int16_t ship_idx, int16_t target_facing, int16_t screen_x, int16_t screen_y);

    void __cdecl Wait_Until_Net_Opponent_Finished_(int16_t show_message);

    int16_t __cdecl Move_Cost_(int16_t ship_idx, int16_t target_x, int16_t target_y);

    inline int16_t __cdecl Turn_Cost_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00470440});
        return fn(ship_idx);
    }

    int16_t __cdecl Move_Ship_(int16_t ship_idx, int16_t dest_x, int16_t dest_y);

    void __cdecl Turn_To_Face_(int16_t ship_idx, int16_t target_x, int16_t target_y);

    inline bool __cdecl Ship_In_Black_Hole_(int16_t ship_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046EE10});
        return fn(ship_idx);
    }

    inline int16_t __cdecl Get_Facing_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00470070});
        return fn(x1, y1, x2, y2);
    }

    inline void __cdecl Draw_Teleporting_Ship_(int16_t ship_idx, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0046ee40})(ship_idx, x, y);
    }

    inline int16_t __cdecl Test_Move_Until_Impact_(int16_t ship_idx, int16_t* target_x, int16_t* target_y, int16_t* cur_facing) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t*, int16_t*, int16_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0046f3f0})(ship_idx, target_x, target_y, cur_facing);
    }
}