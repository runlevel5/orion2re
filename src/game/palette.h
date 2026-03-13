#pragma once

namespace palette {
    inline void __cdecl Set_Palette_Gradient_(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0052b330})(a, b, c, d, e, f, g, h);
    }

    void __cdecl Set_Palette_Changes_(int32_t start, int32_t end);

    void __cdecl Clear_Palette_Changes_(int32_t start, int32_t end);

    inline void __cdecl Refresh_Palette_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052B660});
        fn();
    }

    inline void __cdecl Cycle_Palette_Color_(uint8_t arg1, uint8_t arg2, uint8_t arg3, uint8_t arg4,
                                           uint8_t arg5, uint8_t arg6, uint8_t arg7, uint8_t arg8) {
        using Fn = void(__cdecl*)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052b3d0});
        fn(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    }
}