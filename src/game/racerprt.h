#pragma once
#include <cstdint>

namespace RACERPRT {
    inline char* _TREATIES_FMT = reinterpret_cast<char*>(std::uintptr_t{0x005574A0});
    inline char* _PERSONALITY_FMT = reinterpret_cast<char*>(std::uintptr_t{0x005574B0});
    inline s_colors& _report_normal_color = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005574B8});
    inline uint8_t& _race_report_jim_hack =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005574C0});
    inline s_racescrn_icon_layout& _spy_group_data = *reinterpret_cast<s_racescrn_icon_layout*>(std::uintptr_t{0x005574C8});

    void __cdecl Race_Report_Screen_(int16_t player_idx);
}