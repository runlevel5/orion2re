#pragma once
#include <cstdint>

namespace RACECUST {
    inline void __cdecl Race_Customization_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004121d0 })(player_idx);
    }
}