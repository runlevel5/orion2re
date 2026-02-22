#pragma once
#include <cstdint>

namespace AISTAGE {
    inline void __cdecl Compute_Defensive_Stage_Points_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c3f60});
        fn(player_idx);
    }
}