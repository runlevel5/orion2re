#pragma once
#include <cstdint>

namespace NEWGAME {
    inline s_colors& _color_ken = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005A6260});
    inline uint16_t __cdecl Newgame_Screen_() {
        using Fn = uint16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x004867a0})();
    }
}