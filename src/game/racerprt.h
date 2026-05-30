#pragma once
#include <cstdint>

namespace RACERPRT {
    extern char _TREATIES_FMT[12];
    extern char _PERSONALITY_FMT[6];
    extern s_colors _report_normal_color;
    extern uint8_t _race_report_jim_hack;
    extern s_racescrn_icon_layout _spy_group_data;

    void __cdecl Race_Report_Screen_(int16_t player_idx);
}