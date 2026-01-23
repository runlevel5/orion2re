#pragma once
#include <cstdint>

namespace HOMEGEN {
    inline int16_t __cdecl Generate_Home_Worlds_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x0051A620})();
    }

    inline void __cdecl Make_System_Monsters_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0051C3F0})();
    }
}