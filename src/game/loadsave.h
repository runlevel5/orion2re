#pragma once
#include <cstdint>

namespace LOADSAVE {
    inline char (&_months)[12][6] = *reinterpret_cast<char(*)[12][6]>(std::uintptr_t{0x005722F8});

    void __cdecl Load_Month_Names_();

    inline uint32_t& _game_popup_fields = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0059537c});

    inline void __cdecl _Game_Popup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00510db0})();
    }
}