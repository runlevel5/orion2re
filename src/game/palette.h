#pragma once

namespace palette {
    inline void __cdecl Set_Palette_Changes_(int16_t start, int16_t end) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052B280 });
        fn(start, end);
    }
}