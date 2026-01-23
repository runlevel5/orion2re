#pragma once
#include <cstdint>

namespace REPORT {
    inline void __cdecl Initialize_Reports_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004C1D40})();
    }
}