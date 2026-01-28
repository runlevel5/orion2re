#pragma once
#include <cstdint>

namespace setvesa {
    inline void __cdecl Add_Square_To_Scan_List_(int x1, int y1, int x2, int y2) {
        using Fn = void(__cdecl*)(int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005287E0 });
        fn(x1, y1, x2, y2);
    }
}