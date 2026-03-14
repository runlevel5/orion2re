#pragma once
#include <cstdint>

namespace CMBTMIS {
    inline bool __cdecl On_Combat_Screen_(int16_t x, int16_t y) {
        using Fn = bool(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ff300});
        return fn(x, y);
    }

    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t step, int16_t* out_x, int16_t* out_y);

    inline int16_t __cdecl Missile_Owner_(int16_t m) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004fd8d0})(m);
    }

    inline void __cdecl Retarget_Missile_(int16_t m) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00500070})(m);
    }

    inline uint8_t __cdecl Move_Missile_(int16_t missile_idx, int16_t x, int16_t y, int8_t speed) {
        using Fn = uint8_t(__cdecl*)(int16_t, int16_t, int16_t, int8_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004FEF70})(missile_idx, x, y, speed);
    }

    inline bool __cdecl Missile_On_Combat_Screen_(int16_t missile_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004FEF30})(missile_idx);
    }

    inline void __cdecl Resolve_Missile_(int16_t missile_idx, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004FF3C0})(missile_idx, x, y);
    }

    inline void __cdecl Kill_Missile_(int16_t missile_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004FDA60})(missile_idx);
    }

    inline void __cdecl Draw_Missile_(int16_t missile_idx, int16_t x, int16_t y, int16_t param_4) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004FDB50})(missile_idx, x, y, param_4);
    }
}