#pragma once
#include <cstdint>

namespace NEWPUP {
    // Suggested name: NEWPUP::Troop_Popup_
    inline void __cdecl Troop_Popup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00487060});
        fn();
    }
}