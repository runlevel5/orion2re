#pragma once
#include <cstdint>

namespace bolt {
    inline void __cdecl Bolt_Segment_(int16_t p1, int16_t p2, int16_t p3, int16_t p4,
                                      int16_t p5, int16_t p6, int16_t p7, int16_t p8,
                                      int16_t p9, int16_t p10, int16_t p11, int16_t p12,
                                      int16_t p13, int16_t p14) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t,
                                  int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00532e30});
        fn(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }
}