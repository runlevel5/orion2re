#pragma once
#include <cstdint>

namespace COLMOVE {
    inline int16_t& _cluster_colony_n = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00558284});

    inline void __cdecl Draw_Cluster_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049F2E0});
        fn(x, y);
    }
}