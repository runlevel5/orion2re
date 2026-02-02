#pragma once
#include <cstdint>

namespace RACESCRN {
    inline void __cdecl Race_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0049b760})();
    }
}