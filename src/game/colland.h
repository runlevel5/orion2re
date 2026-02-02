#pragma once
#include <cstdint>

namespace COLLAND {
    inline void __cdecl Colony_Landing_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00429a90})();
    }
}