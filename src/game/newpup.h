#pragma once

namespace NEWPUP {
    inline void*& _saved_autofunction =
        *reinterpret_cast<void**>(std::uintptr_t{0x005663D0});

    inline void __cdecl Draw_Troop_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00487130});
        fn();
    }

    inline void __cdecl Reset_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004870F0});
        fn();
    }

    void __cdecl Troop_Popup_();
}