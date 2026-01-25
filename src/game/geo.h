#pragma once
#include <cstdint>

namespace GEO {
    inline void __cdecl Initialize_Black_Hole_Blocks_(int arg) {
        using Fn = void(__cdecl*)(int);
        reinterpret_cast<Fn>(std::uintptr_t{0x004128E0})(arg);
    }

    inline void __cdecl Initialize_Star_In_Nebula_Info_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00412BA0})();
    }

    inline int16_t __cdecl Parsecs_Between_Stars_(int16_t star_idx1, int16_t star_idx2) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004130F0});
        return fn(star_idx1, star_idx2);
    }
}