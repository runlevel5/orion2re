#pragma once
#include <cstdint>

struct s_list_state;
struct s_list_item;
struct s_colors;

namespace s_turnsum {
    inline char* lbx_00558c70 = reinterpret_cast<char*>(std::uintptr_t{0x00558c70}); // "turnsum.lbx"
}

namespace TURNSUM {
    inline s_colors& _turn_sum_normal_color =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00558BE0});

    inline s_colors& _turn_sum_high_color =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00558BE8});

    inline s_list_state& _g_turn_sum_header = *reinterpret_cast<s_list_state*>(std::uintptr_t{0x00558c30});
    inline uint8_t& _g_turn_message_ix = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056ce70});
    inline s_list_item*& _g_selected_msg_p = *reinterpret_cast<s_list_item**>(std::uintptr_t{0x0056ce74});

    void __cdecl Draw_Turn_Summary_Popup_();

    void __cdecl Turn_Summary_Popup_();
}