#pragma once
#include <cstdint>

namespace LOADSAVE {
    inline uint32_t& _game_popup_fields = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0059537c});

    inline void __cdecl _Game_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00510db0})();
    }
}