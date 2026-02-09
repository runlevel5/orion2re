#pragma once
#include <cstdint>

namespace SYS {
    inline int8_t& _skip_draw_system_planets = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A57CC});
    inline void __cdecl Clear_System_Display_Loaded_Flag_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00493f30});
        fn();
    }

    inline void __cdecl Init_System_Display_(int16_t system_id) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00496250});
        fn(system_id);
    }

    inline void __cdecl Potential_Colony_Info_Popup_(int16_t orbit) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004967f0 });
        fn(orbit);
    }
}