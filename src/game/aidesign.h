#pragma once
#include <cstdint>

namespace aidesign {
    inline void __cdecl Auto_Design_Ship_(int16_t player_idx, char* name, int16_t design_idx) {
        using Fn = void(__cdecl*)(int16_t, char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0046e0e0 });
        fn(player_idx, name, design_idx);
    }

    int16_t __cdecl Cost_Reduction_For_Govt_Hull_Type(int16_t govt_type, struct s_ship_design* design);
}