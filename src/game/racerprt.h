#pragma once
#include <cstdint>

namespace RACERPRT {
    inline void __cdecl Race_Report_Screen_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00471930})(player_idx);
    }
}