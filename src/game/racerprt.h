#pragma once
#include <cstdint>

namespace RACERPRT {
    inline uint8_t& _race_report_jim_hack =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005574C0});

    inline void __cdecl Race_Report_Screen_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00471930})(player_idx);
    }
}