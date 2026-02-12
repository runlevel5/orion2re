#pragma once
#include <cstdint>

namespace GEO {
    void __cdecl Initialize_Black_Hole_Blocks_(int16_t distance);
    void __cdecl Initialize_Star_In_Nebula_Info_();

    uint8_t __cdecl Star_Is_In_Some_Nebula_(int32_t star_idx);

    int16_t __cdecl Parsecs_Between_Stars_(int16_t star_idx1, int16_t star_idx2);
    int16_t __cdecl Parsecs_Between_Points_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    int16_t __cdecl Collect_Black_Holes_(int16_t* black_hole_indices);

    inline bool __cdecl Map_Is_Connected_() {
        using Fn = bool(__cdecl*)();
        return reinterpret_cast<Fn>(0x00412F40)();
    }

    bool __cdecl Point_Is_In_Some_Nebula_(int16_t x, int16_t y);
    inline void __cdecl Get_Orbit_Display_X_Y_(int32_t star_idx, int32_t orbit_idx, int32_t* out_x, int32_t* out_y, int32_t* orbit_pos) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t*, int32_t*, int32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00413140});
        fn(star_idx, orbit_idx, out_x, out_y, orbit_pos);
    }
    int16_t __cdecl Get_Orbit_From_X_Y_(int16_t rel_x, int16_t rel_y);

    inline bool __cdecl Point_Is_In_Nebula_N_(int16_t x, int16_t y, int16_t nebula_index) {
        using Fn = bool(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412A90});
        return fn(x, y, nebula_index);
    }

    inline int32_t __cdecl Get_Orbit_From_X_Y_(int32_t x, int32_t y) {
        using Fn = int32_t(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00413220 });
        return fn(x, y);
    }

    inline bool __cdecl Point_Within_D_Of_Seg_(int16_t px, int16_t py, int16_t d, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        using Fn = bool(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004124B0 });
        return fn(px, py, d, x1, y1, x2, y2);
    }
}