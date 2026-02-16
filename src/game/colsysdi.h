#pragma once
#include <cstdint>

namespace COLSYSDI {
    inline void __cdecl Draw_Col_Sys_Disp_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047ddf0});
        fn(x, y);
    }

    inline void __cdecl Draw_Col_Sys_Disp_Last_Scanned_(int32_t x, int32_t y) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047e530});
        fn(x, y);
    }
}