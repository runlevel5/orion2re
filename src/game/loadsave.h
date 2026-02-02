#pragma once
#include <cstdint>

namespace LOADSAVE {
    inline void __cdecl _Game_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00510db0})();
    }
}