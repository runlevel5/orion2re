#pragma once
#include <cstdint>

namespace BILLHELP {
    inline void __cdecl History_Graph_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B5270})();
    }

    inline void __cdecl Tech_Review_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B5230})();
    }

    inline void __cdecl Race_Statistics_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B51F0})();
    }

    inline void __cdecl Turn_Summary_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B51B0})();
    }

    inline void __cdecl Races_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B5310})();
    }
}