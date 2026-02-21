#pragma once
#include <cstdint>

namespace DIP_SCRN {
    inline uint16_t& _diplomacy_hand_mouse = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005569A8});

    void Resolve_Delayed_Diplomacy_Orders_();

    void __cdecl Set_Opportunity_Attacks_();

    inline void __cdecl Add_Diplomacy_Turn_Summary_Messages_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041D890});
        fn();
    }
}