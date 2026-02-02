#pragma once
#include <cstdint>

namespace NEWGAME {
    inline uint16_t __cdecl Newgame_Screen_() {
        using Fn = uint16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x004867a0})();
    }
}