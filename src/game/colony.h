#pragma once
#include <cstdint>

namespace COLONY {
    inline void __cdecl Init_Colonies_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004dbfd0});
        fn();
    }

    inline void __cdecl Colony_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004dfc10})();
    }

    inline int16_t& _orbit_temp =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B22});
}