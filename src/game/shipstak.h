#pragma once
#include <cstdint>

namespace SHIPSTAK {
    inline int8_t __cdecl Ship_Has_Stealth_Device_(int16_t ship_idx) {
        using Fn = int8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00460660});
        return fn(ship_idx);
    }

    short __cdecl Ship_Stack_Star_Id_(short node_idx);

    inline void __cdecl Find_Ship_Stacks_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004606a0});
        fn();
    }
}