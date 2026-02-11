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

    bool __cdecl Ok_To_View_System_(int16_t star_idx, int16_t player_idx);

    inline void __cdecl Draw_System_Display_Popup_(int16_t system_id, uint8_t flag, const char* label, int32_t x, int32_t y, int32_t w, int32_t h, int32_t p8, int32_t p9) {
        using Fn = void(__cdecl*)(int16_t, uint8_t, const char*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00494050});
        fn(system_id, flag, label, x, y, w, h, p8, p9);
    }
}