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
}