#pragma once
#include <cstdint>

namespace newtech {
    inline void __cdecl Set_Competition_Tech_Values_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bbb40});
        fn(player_idx);
    }

    inline uint32_t __cdecl Calc_Tech_Value_(int16_t player_idx, int32_t tech_idx) {
        using Fn = uint32_t(__cdecl*)(int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004bb2a0});
        return fn(player_idx, tech_idx);
    }

    inline int16_t __cdecl Choose_Tech_Application_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BBD20});
        return fn(player_idx);
    }
}