#pragma once
#include <cstdint>

namespace SYS {
    inline uint32_t& _sys_data_buffer = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A57D0});
    inline const char* s_fmt_star_display = reinterpret_cast<const char*>(std::uintptr_t{0x00557F18}); // "%d> %s"
    inline const char* s_1 = reinterpret_cast<const char*>(std::uintptr_t{0x00551110}); // " 1"
    inline const char* s_percent_d_nl = reinterpret_cast<const char*>(std::uintptr_t{0x00557f20}); // "%d%%\n"
    inline const char* s_0s_1 = reinterpret_cast<const char*>(std::uintptr_t{0x00557f28}); // " 0%s 1"
    inline const char* s_0s = reinterpret_cast<const char*>(std::uintptr_t{0x00557f30}); // " 0%s"
    inline const char* s_m25_17 = reinterpret_cast<const char*>(std::uintptr_t{0x00557f38}); // " M25,17."
    inline const char* s_0s_1_dash = reinterpret_cast<const char*>(std::uintptr_t{0x00557f44}); // " 0%s 1--\n"
    inline const char* s_m25_27 = reinterpret_cast<const char*>(std::uintptr_t{0x00557f50}); // " M25,27."
    inline const char* s_0s_1d_nl = reinterpret_cast<const char*>(std::uintptr_t{0x00557f5c}); // " 0%s 1%d\n"
    inline const char* s_0s_10_nl = reinterpret_cast<const char*>(std::uintptr_t{0x00557f68}); // " 0%s 10\n"
    inline const char* s_d_nl = reinterpret_cast<const char*>(std::uintptr_t{0x00557f74}); // "%d\n"
    inline const char* s_m25_30_0s = reinterpret_cast<const char*>(std::uintptr_t{0x00557f78}); // " M25,30. 0%s"

    inline uint8_t& _draw_sun_before_planet = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566438});
    inline int8_t& _skip_draw_system_planets = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A57CC});
    inline uint8_t& _skip_draw_system_display = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A57D4});
    inline s_animation_header*& _system_display_sun_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x0056654C});

    void __cdecl Clear_System_Display_Loaded_Flag_();

    inline void __cdecl Clear_System_Display_Ship_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00493F50});
        fn();
    }

    bool __cdecl Need_To_Reload_System_Display_(int16_t star_id);

    void __cdecl Set_System_Display_Loaded_Flag_(short star_idx);

    void __cdecl Load_New_System_Display_(int16_t star_idx);

    void __cdecl Init_System_Display_(int16_t star_id);

    void __cdecl Potential_Colony_Info_Popup_(int16_t planet_idx);

    void __cdecl Get_Rotating_Planet_Segment_(s_animation_header** out_ptr, int16_t planet_idx);

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

    void __cdecl N_Ships_By_Type_At_Location_(int16_t x, int16_t y, int16_t target_owner, int16_t *total_count, int16_t *bases_count, int16_t *colony_ships_count, int16_t *transports_count, int16_t *ship_sizes);

    inline void __cdecl Draw_System_Display_Popup_(int16_t system_id, uint8_t flag, const char* label, int32_t x, int32_t y, int32_t w, int32_t h, int32_t p8, int32_t p9) {
        using Fn = void(__cdecl*)(int16_t, uint8_t, const char*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00494050});
        fn(system_id, flag, label, x, y, w, h, p8, p9);
    }
}