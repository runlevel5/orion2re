#pragma once
#include <cstdint>

namespace COLREFIT {
    inline int16_t __cdecl Colony_Refit_Popup_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a1180});
        return fn();
    }
    inline int16_t __cdecl Colony_Refit2_Popup_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a1e00});
        return fn(ship_idx);
    }
}