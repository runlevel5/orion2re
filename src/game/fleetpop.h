#pragma once
#include <cstdint>

namespace FLEETPOP {
    inline void __cdecl Reload_Fleet_Box_Data_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00405990})();
    }
}