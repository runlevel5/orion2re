#pragma once
#include <cstdint>

namespace DESIGN {
    inline int16_t __cdecl Cost_Of_Design_(int16_t player_idx, char* design_name) {
        using Fn = int16_t(__cdecl*)(int16_t, char*);
        return reinterpret_cast<Fn>(0x0042e320)(player_idx, design_name);
    }

    inline void __cdecl Design_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0042e6b0})();
    }
}