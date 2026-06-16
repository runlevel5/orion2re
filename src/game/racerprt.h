#pragma once
#include <cstdint>

struct s_racescrn_icon_layout;

namespace RACERPRT {
    extern s_colors _report_normal_color;
    extern uint8_t _race_report_jim_hack;
    extern s_racescrn_icon_layout _spy_group_data;

    /**
     * Draws the race report screen for the selected player.
     * @param player_idx Index of the player whose race report is displayed.
     */
    void __cdecl Race_Report_Screen_(int16_t player_idx);
}