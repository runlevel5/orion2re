#pragma once
#include <cstdint>

namespace SYS {
    inline uint32_t& _sys_data_buffer = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A57D0});
    inline const char* s_fmt_star_display = reinterpret_cast<const char*>(std::uintptr_t{0x00557F18}); // "%d> %s"

    inline uint8_t& _draw_sun_before_planet = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566438});
    inline int8_t& _skip_draw_system_planets = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A57CC});
    inline uint8_t& _skip_draw_system_display = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A57D4});
    inline s_animation_header*& _system_display_sun_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0056654C});

    void __cdecl Clear_System_Display_Loaded_Flag_();

    bool __cdecl Need_To_Reload_System_Display_(int16_t star_id);

    void __cdecl Set_System_Display_Loaded_Flag_(short star_idx);

    void __cdecl Load_New_System_Display_(int16_t star_idx);

    void __cdecl Init_System_Display_(int16_t star_id);

    struct s_potential_colony_info {
        int16_t unknown_0;
        int16_t maintenance;    // local_5be
        int16_t unknown_4;
        int16_t worker_bonus;   // local_5ba
        int16_t food;           // local_5b8
        int16_t industry;       // local_5b6
        int16_t research;       // local_5b4
    };

    void __cdecl Potential_Colony_Info_Popup_(int16_t planet_idx);

    inline void __cdecl Get_Rotating_Planet_Segment_(s_animation_header** out_anim, int16_t planet_idx, int16_t star_idx, int32_t unknown) {
        using Fn = void(*)(s_animation_header**, int16_t, int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00495a40});
        fn(out_anim, planet_idx, star_idx, unknown);
    }

    inline void __cdecl Calculate_Remap_Gray_Palettes_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00497660});
        fn();
    }

    inline int __cdecl qsort_sys_disp_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00497640});
        return fn(a, b);
    }

    bool __cdecl Ok_To_View_System_(int16_t star_idx, int16_t player_idx);

    inline void __cdecl N_Ships_By_Type_At_Location_(int16_t x, int16_t y, int16_t owner, int16_t* total, int16_t* colony, int16_t* outpost, int16_t* transport, int32_t* hull_counts) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t*, int16_t*, int16_t*, int16_t*, int32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00496620});
        fn(x, y, owner, total, colony, outpost, transport, hull_counts);
    }

    inline void __cdecl Draw_System_Display_Popup_(int16_t system_id, uint8_t flag, const char* label, int32_t x, int32_t y, int32_t w, int32_t h, int32_t p8, int32_t p9) {
        using Fn = void(__cdecl*)(int16_t, uint8_t, const char*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00494050});
        fn(system_id, flag, label, x, y, w, h, p8, p9);
    }
}