#pragma once
#include <cstdint>

namespace COLMOVE {
    inline int16_t& _cluster_colony_n = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00558284});
    inline uint8_t& _g_hacky_dont_draw_cluster_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558288});

    void __cdecl Draw_Cluster_(int16_t x, int16_t y);

    int16_t __cdecl Get_Num_Unemployed_(int16_t colony_idx);

    int16_t __cdecl Get_First_Unemployed_(int16_t colony_idx);

    void __cdecl Clear_Cluster_();

    void __cdecl Get_Cluster_(int16_t colony_idx, int16_t start_pop_idx);

    inline bool __cdecl Pops_Identical_(int16_t col1, int16_t pop1, int16_t col2, int16_t pop2) {
        using Fn = bool(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049f4f0});
        return fn(col1, pop1, col2, pop2);
    }

    inline void __cdecl Send_Cluster_(int16_t colony_idx, int16_t param_2) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049F680});
        fn(colony_idx, param_2);
    }
}