#pragma once
#include <cstdint>

namespace replace {
    inline void __cdecl Interlaced_Gray_Scale_Off_Page_(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t unknown) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00530b40});
        fn(x1, y1, x2, y2, unknown);
    }
}