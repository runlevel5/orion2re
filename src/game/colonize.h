#pragma once
#include <cstdint>

namespace COLONIZE {
    inline void __cdecl Allocate_New_Colony_Slots_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004B9BA0})();
    }
}