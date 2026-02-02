#pragma once
#include <cstdint>

namespace FLT1 {
    inline void __cdecl Fleet_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004b0d20})();
    }
}