#pragma once
#include <cstdint>

namespace BILL {
    inline void __cdecl Bill_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00406780})();
    }

    inline void __cdecl Cache_Player_Scientist_Anim_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00407910})();
    }
}