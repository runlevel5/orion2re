#pragma once
#include <cstdint>

namespace COLSUM {
    inline void __cdecl Colony_Summary_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00402800})();
    }
}