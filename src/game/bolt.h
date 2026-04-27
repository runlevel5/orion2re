#pragma once
#include <cstdint>

namespace bolt {
    inline int16_t (&_head_wide_bolts)[192] = *reinterpret_cast<int16_t(*)[192]>(std::uintptr_t{0x0055BA20});
    inline int16_t (&_tail_wide_bolts)[192] = *reinterpret_cast<int16_t(*)[192]>(std::uintptr_t{0x0055BBA0});

    inline void __cdecl Bolt_Segment_(int16_t p1, int16_t p2, int16_t p3, int16_t p4,
                                      int16_t p5, int16_t p6, int16_t p7, int16_t p8,
                                      int16_t p9, int16_t p10, int16_t p11, int16_t p12,
                                      int16_t p13, int16_t p14) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t,
                                  int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00532e30});
        fn(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }

    void __cdecl Wide_Bolt_Line_(
        int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t param_5, int16_t param_6,
        int16_t param_7, int16_t param_8, uint16_t param_9, int16_t param_10, int16_t param_11, int16_t param_12,
        int16_t param_13, int16_t param_14, int16_t param_15, int16_t param_16, int16_t param_17, int16_t param_18,
        int16_t param_19, int16_t param_20, int16_t param_21);

    void __cdecl Bolt_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t p5,
        int16_t p6, int16_t p7, int16_t p8, int16_t p9, int16_t draw_caps,
        int16_t unused_11, int16_t p12, int16_t p13, int16_t p14, int16_t p15,
        int16_t color_base, int16_t p17, int16_t p18, int16_t p19);
}