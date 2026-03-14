#pragma once
#include <cstdint>

namespace DIP_SCRN {
    inline s_mouse_list& _diplomacy_hand_mouse = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005569A8});

    void Resolve_Delayed_Diplomacy_Orders_();

    void __cdecl Set_Opportunity_Attacks_();

    inline void __cdecl Add_Diplomacy_Turn_Summary_Messages_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041D890});
        fn();
    }

    inline void __cdecl Npc_Diplomacy_Screen_(int16_t player_idx, int32_t mode) {
        using Fn = void(__cdecl*)(int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00417260});
        fn(player_idx, mode);
    }

    void __cdecl Show_Sneak_Attack_Message_(int16_t player_idx, int16_t defender_idx);
}