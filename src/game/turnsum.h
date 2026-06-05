#pragma once
#include <cstdint>

struct s_list_state;
struct s_list_item;
struct s_colors;

namespace TURNSUM {
    extern s_colors _turn_sum_normal_color;

    extern s_colors _turn_sum_high_color;

    extern s_list_state _g_turn_sum_header;
    extern uint8_t _g_turn_message_ix;
    extern s_list_item* _g_selected_msg_p;

    /**
     * Updates the turn summary popup selection state and redraws the visible list text.
     */
    void __cdecl Draw_Turn_Summary_Popup_();

    /**
     * Opens the turn summary popup, handles paging and message selection, and restores the caller's screen state.
     */
    void __cdecl Turn_Summary_Popup_();
}