#pragma once
#include <cstdint>

struct s_list_state;
struct s_list_item;
struct s_colors;

namespace s_turnsum {
    extern const char lbx_00558c70[12];
}

namespace TURNSUM {
    extern s_colors _turn_sum_normal_color;

    extern s_colors _turn_sum_high_color;

    extern s_list_state _g_turn_sum_header;
    inline uint8_t& _g_turn_message_ix = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056ce70});
    inline s_list_item*& _g_selected_msg_p = *reinterpret_cast<s_list_item**>(std::uintptr_t{0x0056ce74});

    void __cdecl Draw_Turn_Summary_Popup_();

    void __cdecl Turn_Summary_Popup_();
}