#pragma once
#include <cstdint>

namespace MAINSCR {
    inline void __cdecl Update_Star_Owners_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004CD560})();
    }

    inline void __cdecl Draw_Mini_Main_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004C6780})();
    }
}