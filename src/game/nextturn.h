#pragma once
#include <cstdint>

namespace NEXTTURN {
    inline void __cdecl Next_Turn_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0049a040})();
    }
}