#pragma once
#include <cstdint>

namespace DIP_SCRN {
    inline uint16_t& _diplomacy_hand_mouse = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005569A8});

    inline void __cdecl Resolve_Delayed_Diplomacy_Orders_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0041d9e0 });
        fn();
    }

    inline void __cdecl Set_Opportunity_Attacks_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0041d6f0 });
        fn();
    }
}