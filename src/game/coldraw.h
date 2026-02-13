#pragma once
#include <cstdint>

namespace COLDRAW {
    // Suggested name: Calculate_Squish_Step
    inline void __cdecl Calculate_Squish_Step_(int16_t x1, int16_t x2, int16_t count, int16_t base_step) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458410});
        fn(x1, x2, count, base_step);
    }
}