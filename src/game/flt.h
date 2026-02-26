#pragma once

namespace FLT {
    inline void __cdecl Set_Fltscrn_Small_Ship_Icon_XYs_(short p1, short p2, short p3, short p4) {
        using Fn = void(__cdecl*)(short, short, short, short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00489610 });
        fn(p1, p2, p3, p4);
    }
}