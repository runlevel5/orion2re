#pragma once
#include <cstdint>

namespace COLCALC {
    inline void __cdecl Twiddle_Initial_Homeworlds_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004FC140})();
    }

    inline void __cdecl Compute_All_Star_Colony_Stuff_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004FBF00})();
    }

    inline void __cdecl Init_Surrenders_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004FB7F0})();
    }
}