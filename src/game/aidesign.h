#pragma once
#include <cstdint>

namespace aidesign {
    inline void __cdecl Auto_Design_Ship_(int16_t player_idx, char* name, int16_t design_idx) {
        using Fn = void(__cdecl*)(int16_t, char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004825c0});
        fn(player_idx, name, design_idx);
    }

    inline void __cdecl Auto_Strategic_Design_Ship_(int16_t player_idx, int16_t design_idx, char* name) {
        using Fn = void(__cdecl*)(int16_t, int16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0046e0e0});
        fn(player_idx, design_idx, name);
    }
}