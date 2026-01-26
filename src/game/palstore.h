#pragma once

#include <cstdint>

namespace palstore {
    int16_t Slow_Fade_In_();

    inline void __cdecl Darken_Palette_(int32_t amount) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052B850});
        fn(amount);
    }
}