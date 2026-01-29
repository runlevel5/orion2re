#pragma once
#include <cstdint>

namespace INITSHIP {
    void __cdecl Init_Ship_Designs_(int16_t player_idx);
    int16_t __cdecl Best_Ship_Fuel_(int16_t player_idx);

    void __cdecl Update_Player_Ship_Designs_(int16_t player_idx);

    inline void __cdecl Update_Player_Strategic_Ship_Designs_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00482160});
        fn(player_idx);
    }

    inline void __cdecl Update_Automatic_Design_Equipment_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00482020});
        fn(player_idx);
    }

    inline void __cdecl Update_Equipment_On_Ships_Being_Built_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00481ef0});
        fn(player_idx);
    }

    int16_t __cdecl Best_Warp_Drive_(int16_t player_idx);

    inline int16_t __cdecl Calc_Player_FTL_Speed_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00482260});
        return fn(player_idx);
    }
}