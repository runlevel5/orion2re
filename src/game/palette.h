#pragma once

namespace palette {
    inline void __cdecl Set_Palette_Changes_(int16_t start, int16_t end) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052B280 });
        fn(start, end);
    }

    inline void __cdecl Clear_Palette_Changes_(int32_t start, int32_t count) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0052b2b0 })(start, count);
    }
}