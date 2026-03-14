#pragma once
#include <cstdint>

namespace CMBTMIS {
    int8_t __cdecl On_Combat_Screen_(int16_t x, int16_t y);

    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t dist, int16_t* dx, int16_t* dy);

    int16_t __cdecl Missile_Owner_(int16_t missile_idx);

    void __cdecl Retarget_Missile_(int16_t missile_idx);

    char __cdecl Move_Missile_(int16_t missile_idx, int16_t target_x, int16_t target_y, int16_t move_dist);

    inline char __cdecl Did_Missile_Hit_(int16_t missile_idx, int16_t target_x, int16_t target_y) {
        using Fn = char(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004fdad0});
        return fn(missile_idx, target_x, target_y);
    }

    inline int16_t __cdecl Get_Facing_(int16_t missile_idx, int16_t target_x, int16_t target_y) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004fec90});
        return fn(missile_idx, target_x, target_y);
    }

    inline int16_t __cdecl Missile_Speed_(int16_t type, uint16_t specials, int16_t owner) {
        using Fn = int16_t(__cdecl*)(int16_t, uint16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004fed80});
        return fn(type, specials, owner);
    }

    int8_t __cdecl Missile_On_Combat_Screen_(int16_t missile_idx);

    int16_t __cdecl Resolve_Missile_(int16_t missile_idx, int16_t x, int16_t y);

    inline int16_t __cdecl Is_Missile_Target_Alive_(int16_t missile_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004ff340})(missile_idx);
    }

    inline int16_t __cdecl Find_Weakest_Shield_(int16_t target_unit_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004ffe00})(target_unit_idx);
    }

    inline void __cdecl Fighter_Combat_SFX_(int16_t missile_idx, int16_t x, int16_t y, int16_t dmg, int16_t sys_dmg) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00500580})(missile_idx, x, y, dmg, sys_dmg);
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