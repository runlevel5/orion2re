#pragma once
#include <cstdint>

namespace NETTURN {
    inline void __cdecl Net_Next_Turn_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0047d5b0})();
    }
}