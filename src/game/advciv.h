#pragma once
#include <cstdint>

namespace ADVCIV {
    inline void __cdecl Advanced_Civilization_Colonies_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004AB460})();
    }

    inline void __cdecl Assign_Advanced_Civilization_Starting_Ships_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004AC2A0})();
    }
}