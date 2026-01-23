#pragma once
#include <cstdint>

namespace ERIC {
    inline void __cdecl Eric_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004BAA30})();
    }
}