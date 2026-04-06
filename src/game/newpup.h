#pragma once

namespace NEWPUP {
    inline void __cdecl Troop_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00487060});
        fn();
    }
}