#pragma once
#include <cstdint>

namespace MAINSCR2 {
    inline uint8_t& _main_screen_fade_in_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596013});
    inline uint8_t& _force_main_screen_fade = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596014});

    inline void __cdecl Distance_Between_Stars_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004d9d10})();
    }
    inline void __cdecl Planet_Colonization_In_Main_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004da200})();
    }
    inline void __cdecl Building_Outposts_In_Main_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004d9b40})();
    }
    inline void __cdecl Unloading_Transports_In_Main_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004da6d0})();
    }
    void __cdecl Scrap_Freighters_();
    inline void __cdecl Do_Begin_Of_Turn_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004da9f0});
        fn();
    }
    void __cdecl Reports_Screen_();
    void Show_Command_Points_Screen_();
}