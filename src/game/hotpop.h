#pragma once
#include <cstdint>

namespace HOTPOP {
    inline void __cdecl Hotseat_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004db170})();
    }
}