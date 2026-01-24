#pragma once

namespace KEN {
    inline void __cdecl Ken_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004AA050 })();
    }

    inline void __cdecl Init_NPC_Personalities_Objectives_Themes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004C1FC0 })();
    }
}