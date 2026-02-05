#pragma once
#include <cstdint>

namespace GEO {
    void __cdecl Initialize_Black_Hole_Blocks_(int16_t distance);
    void __cdecl Initialize_Star_In_Nebula_Info_();

    inline uint8_t __cdecl Star_Is_In_Some_Nebula_(int32_t star_idx) {
        using Fn = uint8_t(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412B70});
        return fn(star_idx);
    }

    int16_t __cdecl Parsecs_Between_Stars_(int16_t star_idx1, int16_t star_idx2);
    int16_t __cdecl Parsecs_Between_Points_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    inline bool __cdecl Map_Is_Connected_() {
        using Fn = bool(__cdecl*)();
        return reinterpret_cast<Fn>(0x00412F40)();
    }

    bool __cdecl Point_Is_In_Some_Nebula_(int16_t x, int16_t y);

    inline bool __cdecl Point_Is_In_Nebula_N_(int16_t x, int16_t y, int16_t nebula_index) {
        using Fn = bool(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412A90});
        return fn(x, y, nebula_index);
    }
}