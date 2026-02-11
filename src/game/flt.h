#pragma once
#include <cstdint>

namespace FLT {
    inline void __stdcall Set_Fltscrn_Small_Ship_Icon_XYs_(int p1, int p2, int p3, int p4) {
        using Fn = void(__stdcall*)(int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00489610 });
        fn(p1, p2, p3, p4);
    }
}