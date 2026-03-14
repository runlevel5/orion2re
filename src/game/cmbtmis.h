#pragma once
#include <cstdint>

namespace CMBTMIS {
    inline int16_t (&_missile_group_offset)[16][16] =
        *reinterpret_cast<int16_t(*)[16][16]>(std::uintptr_t{0x0054F1C8});
    inline int16_t (&_torpedo_group_offset)[16][8] =
        *reinterpret_cast<int16_t (*)[16][8]>(std::uintptr_t{0x0054F3C8});

    int16_t __cdecl Missile_Type_(int16_t missile_idx);

    int8_t __cdecl On_Combat_Screen_(int16_t x, int16_t y);

    void __cdecl Coord_Movement_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t dist, int16_t* dx, int16_t* dy);

    int16_t __cdecl Missile_Owner_(int16_t missile_idx);

    void __cdecl Retarget_Missile_(int16_t missile_idx);

    char __cdecl Move_Missile_(int16_t missile_idx, int16_t target_x, int16_t target_y, int16_t move_dist);

    int8_t __cdecl Did_Missile_Hit_(int16_t missile_idx, int16_t x, int16_t y);

    int16_t __cdecl Get_Facing_(int16_t missile_idx, int16_t target_x, int16_t target_y);

    int16_t __cdecl Missile_Speed_(int16_t type, uint16_t specials, int16_t owner);

    int8_t __cdecl Missile_On_Combat_Screen_(int16_t missile_idx);

    int16_t __cdecl Resolve_Missile_(int16_t missile_idx, int16_t x, int16_t y);

    int16_t __cdecl Is_Missile_Target_Alive_(int16_t missile_idx);

    int16_t __cdecl Find_Weakest_Shield_(int16_t ship_idx);

    void __cdecl Fighter_Combat_SFX_(int16_t missile_idx, int16_t x, int16_t y, int16_t dmg, int16_t sys_dmg);

    void __cdecl Get_Multiple_Missile_Coords_(int16_t missile_idx, int16_t quantity, int16_t* out_x, int16_t* out_y);

    void __cdecl Get_Multiple_Torpedo_Coords_(int16_t missile_idx, int16_t group_size, int16_t* out_x, int16_t* out_y);

    void __cdecl Draw_Fighter_(int16_t m_idx, int16_t base_x, int16_t base_y, int16_t combat_view_type);

    void __cdecl Kill_Missile_(int16_t missile_idx);

    int16_t __cdecl Picture_Count_(int16_t missile_idx);

    void __cdecl Draw_Missile_(int16_t missile_idx, int16_t screen_x, int16_t screen_y, int16_t flag);
}