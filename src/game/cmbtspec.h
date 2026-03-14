#pragma once
#include <cstdint>

namespace CMBTSPEC {
    void __cdecl Draw_Cloak_(int16_t ship_idx, int16_t cloak_type);

    inline void __cdecl Draw_Continuing_Tractor_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004A6B20})(ship_idx);
    }

    inline void __cdecl Start_Ship_Engine_SFX_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004a7e20)();
    }

    inline void __cdecl End_Ship_Engine_SFX_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004a7e50)();
    }

    inline void __cdecl Draw_Moving_Tractor_(int16_t tractor_idx, int16_t cur_pixel_x, int16_t cur_pixel_y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(0x004a6cb0)(tractor_idx, cur_pixel_x, cur_pixel_y);
    }

    inline void __cdecl Lightning_Field_(int16_t target, int16_t missile_idx, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004a6190})(target, missile_idx, x, y);
    }
}