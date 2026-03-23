#pragma once
#include <cstdint>

namespace fade {
    inline void __cdecl Blinds_Fade_(int x1, int y1, int x2, int y2, int step) {
        using Fn = void(__cdecl*)(int, int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00533770});
        fn(x1, y1, x2, y2, step);
    }
}