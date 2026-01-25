#pragma once
#include <cstdint>

namespace COLBLDG {
    inline void __cdecl Create_Ship_(int16_t player_idx, int16_t design_idx, int16_t colony_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005094a0});
        fn(player_idx, design_idx, colony_idx);
    }
}