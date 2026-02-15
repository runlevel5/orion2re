#pragma once
#include <cstdint>

namespace COLMOVE {
    inline int16_t& _cluster_colony_n = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00558284});

    inline void __cdecl Draw_Cluster_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049F2E0});
        fn(x, y);
    }

    inline void __cdecl Clear_Cluster_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049F1B0});
        fn();
    }

    inline void __cdecl Get_Cluster_(int16_t col_n, int16_t pop_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049F430});
        fn(col_n, pop_idx);
    }

    inline void __cdecl Send_Cluster_(int16_t colony_idx, int16_t param_2) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049F680});
        fn(colony_idx, param_2);
    }
}