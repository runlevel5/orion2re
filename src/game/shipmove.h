#pragma once
#include <cstdint>
#include "types.h"

namespace SHIPMOVE {
    void __cdecl Allocate_New_Ship_Slots_();
    void __cdecl Move_All_Ships_Toward_Stars_();
    void __cdecl Reset_Needless_Ignore_Combat_Ships_Flags_();

    void __cdecl Update_All_Ship_Ranges_();

    bool __cdecl Star_In_Normal_Range_Of_Player_(s_star_data* star, int16_t player_idx);

    bool __cdecl Star_In_Range_Of_Player_Aux1_(s_star_data* star, int16_t player_idx, int16_t dist);
    bool __cdecl Star_In_Range_Of_Player_Aux2_(s_star_data* star, int16_t player_idx, int16_t dist);
    bool __cdecl Star_In_Range_Of_Player_(s_star_data* star, int16_t player_idx, int16_t dist);

    void __cdecl Update_Player_Ship_Range_(s_player* player);

    inline void __cdecl Ships_Try_To_Move_To_(int16_t* ids, int16_t target, s_ship_move_info* out_info) {
        using Fn = void(__cdecl*)(int16_t*, int16_t, s_ship_move_info*);
        reinterpret_cast<Fn>(0x004363E0)(ids, target, out_info);
    }

    inline void __cdecl Make_Ships_Move_To_(int16_t* ids, int16_t target, s_ship_move_info* info) {
        using Fn = void(__cdecl*)(int16_t*, int16_t, s_ship_move_info*);
        reinterpret_cast<Fn>(0x00436A50)(ids, target, info);
    }

    inline void __cdecl Move_All_Ships_Toward_Stars_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00436b60 });
        fn();
    }

    inline void __cdecl Reset_Needless_Ignore_Combat_Ships_Flags_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004377d0 });
        fn();
    }

    inline int16_t __cdecl Create_Nonplayer_Ship_(int16_t owner, s_ship_design* design, int16_t x, int16_t y) {
        using Fn = int16_t(__cdecl*)(int16_t, s_ship_design*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00436E30})(owner, design, x, y);
    }

    inline s_ship_move_info& _g_ship_move_info = *reinterpret_cast<s_ship_move_info*>(std::uintptr_t{0x0059C440});
}