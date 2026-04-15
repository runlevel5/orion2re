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

    void __cdecl Clear_System_Display_Ship_Fields_();

    bool __cdecl Need_To_Reload_System_Display_(int16_t star_id);

    void __cdecl Set_System_Display_Loaded_Flag_(short star_idx);

    void __cdecl Load_New_System_Display_(int16_t star_idx);

    void __cdecl Init_System_Display_(int16_t star_id);

    void __cdecl Potential_Colony_Info_Popup_(int16_t planet_idx);

    void __cdecl Get_Rotating_Planet_Segment_(s_animation_header** out_ptr, int16_t planet_idx);

    inline int8_t (&_remap_gray_percentages)[10] =
        *reinterpret_cast<int8_t (*)[10]>(std::uintptr_t{0x00557EE0});

    inline uint8_t (&_remap_gray_mask_palette)[10][256] =
        *reinterpret_cast<uint8_t (*)[10][256]>(std::uintptr_t{0x005A57E0});

    void __cdecl Calculate_Remap_Gray_Palettes_();

    int __cdecl qsort_sys_disp_(const void* a, const void* b);

    bool __cdecl Ok_To_View_System_(int16_t star_idx, int16_t player_idx);

    void __cdecl N_Ships_By_Type_At_Location_(int16_t x, int16_t y, int16_t target_owner, int16_t *total_count, int16_t *bases_count, int16_t *colony_ships_count, int16_t *transports_count, int16_t *ship_sizes);

    void __cdecl Hide_System_Display_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset, int16_t y_offset, int16_t mode);

    void __cdecl Draw_System_Display_Border_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset, int16_t y_offset, int16_t mode);

    void __cdecl Draw_System_Special_Popup_(int16_t x, int16_t y, int16_t special_idx, int16_t anim_idx);

    void __cdecl Draw_Sun_Seg_(int16_t star_idx, int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset);

    void __cdecl Draw_System_Display_Planets_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset);

    inline void __cdecl Draw_Planet_With_Pierced_Ring_(int16_t planet_idx, int16_t orbit_pos, int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t x_offset) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00494C60 });
        fn(planet_idx, orbit_pos, base_x, base_y, content_w, content_h, y_offset, x_offset);
    }

    void __cdecl Draw_Colony_Marker_(int16_t x, int16_t y, int16_t owner);

    void __cdecl Draw_Outpost_Marker_(int16_t x, int16_t y, int16_t owner);

    void __cdecl Draw_System_Highlight_(int16_t planet_idx, int16_t rect_w, int16_t rect_h, int16_t x_offset, int16_t y_offset);

    void __cdecl Print_Planet_Info_Short_(int16_t planet_idx, int16_t* box_width, int16_t* box_height);

    inline void __cdecl Do_Print_Planet_Info_(int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7, int16_t* out_width, int16_t* out_height, int32_t p10) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t*, int16_t*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004953a0 });
        fn(p1, p2, p3, p4, p5, p6, p7, out_width, out_height, p10);
    }

    void __cdecl Print_Planet_Info_(int16_t p1, int16_t p2, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7);

    inline void __cdecl Draw_System_Ship_Icons_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t y_offset, int16_t
        x_offset) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004951B0 });
        fn(base_x, base_y, content_w, content_h, y_offset, x_offset);
    }

    inline void __cdecl Draw_Gate_Icons_(int16_t star_idx, int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, int16_t x_offset) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004949C0 });
        fn(star_idx, base_x, base_y, content_w, content_h, x_offset);
    }

    inline void __cdecl Print_Gate_Info_(int16_t star_idx, int16_t gate_idx, int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h,
        int16_t x_offset, int16_t y_offset) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00496E20 });
        fn(star_idx, gate_idx, base_x, base_y, content_w, content_h, x_offset, y_offset);
    }

    inline void __cdecl Print_System_Display_Box_Title_(int16_t base_x, int16_t base_y, int16_t content_w, int16_t content_h, const char*
        title_text) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004972B0 });
        fn(base_x, base_y, content_w, content_h, title_text);
    }

    inline void __cdecl Print_Empty_System_Data_(int16_t star_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00497E50 });
        fn(star_idx);
    }

    void __cdecl Draw_System_Display_Popup_(short star_idx, int8_t draw_border, char* title_text, short base_x, short base_y, short content_w, short content_h, short x_offset, short y_offset);
}