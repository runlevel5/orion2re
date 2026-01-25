#pragma once
#include <cstdint>

namespace MAINSCR {
    inline void __cdecl Get_PointXY_Draw_Coords_(int16_t x, int16_t y, int16_t* outX, int16_t* outY) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004c7440})(x, y, outX, outY);
    }

    inline void __cdecl Update_Star_Owners_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004CD560})();
    }

    inline void __cdecl Draw_Mini_Main_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004C6780})();
    }
}