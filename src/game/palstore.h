#pragma once

#include <cstdint>

namespace palstore {
    inline void __cdecl Darken_Palette_(int16_t level) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0052b850})(level);
    }

    inline int16_t __cdecl Slow_Fade_In_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00425dc0})();
    }
}