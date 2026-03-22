#pragma once
#include <cstdint>

namespace BILLHELP {
    inline void __cdecl Races_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B5310})();
    }
}