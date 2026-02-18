#pragma once

namespace aitech {
    inline void __cdecl Apply_Evolutionary_Upgrades_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00501690 });
        fn();
    }

    inline void __cdecl All_AI_Tech_Select_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005014c0 });
        fn();
    }
}