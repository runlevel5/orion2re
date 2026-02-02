#pragma once
#include <cstdint>

namespace TURNSUM {
    inline void __cdecl Turn_Summary_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004c1790})();
    }
}