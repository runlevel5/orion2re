#pragma once
#include <cstdint>

namespace GEO {
    inline int16_t*& _dads = *reinterpret_cast<int16_t**>(std::uintptr_t{0x00564840});
    inline int16_t (&_orbit_radiuses)[7][2] = *reinterpret_cast<int16_t (*)[7][2]>(std::uintptr_t{0x0054F008});
    inline void*& _orbit_consts_end = *reinterpret_cast<void**>(std::uintptr_t{0x0054F024});

    void __cdecl Initialize_Black_Hole_Blocks_(int16_t distance);
    void __cdecl Initialize_Star_In_Nebula_Info_();

    uint8_t __cdecl Star_Is_In_Some_Nebula_(int32_t star_idx);

    int16_t __cdecl Parsecs_Between_Stars_(int16_t star_idx1, int16_t star_idx2);
    int16_t __cdecl Parsecs_Between_Points_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    bool __cdecl Black_Hole_Blocks_Points_(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

    int16_t __cdecl Collect_Black_Holes_(int16_t* black_hole_indices);

    bool __cdecl Map_Is_Connected_();

    bool __cdecl Point_Is_In_Some_Nebula_(int16_t x, int16_t y);
    void __cdecl Get_Orbit_Display_X_Y_(int32_t star_idx, int32_t orbit_idx, int32_t* out_x, int32_t* out_y, int32_t* orbit_pos);
    int32_t __cdecl Get_Orbit_From_X_Y_(int32_t x, int32_t y);

    bool __cdecl Point_Is_In_Warp_Field_Interdictor_Of_Star_(int16_t x, int16_t y, uint8_t player_idx, s_star_data* star);

    bool __cdecl Point_Is_In_Nebula_N_(int16_t x, int16_t y, int16_t nebula_idx);

    void __cdecl Move_Player_1_Turn_To_Star_(uint8_t player_num, int16_t start_x, int16_t start_y, int32_t star_idx, int16_t max_turns, int16_t *out_x, int16_t *out_y, int8_t *nebula_flag, int8_t *interdictor_flag);

    bool __cdecl Point_Within_D_Of_Seg_(int16_t px, int16_t py, int16_t d, int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    int32_t __cdecl Points_On_Same_Side_Of_Line_(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t lx1, int32_t ly1, int32_t lx2, int32_t ly2);

    int __cdecl Actual_Expected_Damage_(int min_damage, int max_damage, int min_chance, int base_chance, int reduction);

    bool __cdecl Union_Find_(int16_t node1, int16_t node2, bool do_union);
}