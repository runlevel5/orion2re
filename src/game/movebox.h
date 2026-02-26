#pragma once
#include <cstdint>

namespace MOVEBOX {
    inline void __cdecl Clear_Moveable_Box_Fields_(int16_t order_id) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005129D0 });
        fn(order_id);
    }

    bool __cdecl Moveable_Box_Selected_(int16_t index);

    void __cdecl Vanish_Fleet_Movement_Box_();

    void __cdecl Vanish_System_Display_Box_();

    inline void __cdecl Save_And_Remove_Boxes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00513C90})();
    }

    inline void __cdecl Restore_Boxes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00513C00})();
    }

    void __cdecl Move_Box_To_Top_(int16_t box_id);

    bool __cdecl Mouse_Within_Fleet_Box_();

    inline void __cdecl Move_Moveable_Box_(int16_t box_id, int16_t mouse_btn) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00513740});
        fn(box_id, mouse_btn);
    }

    inline void __cdecl Draw_Moveable_Box_(int16_t box_id) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00513440});
        fn(box_id);
    }

    void __cdecl Box_Fleet_Screen_Scanned_Star_();

    inline void __cdecl Box_Officer_Screen_Scanned_Star_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005133A0});
        fn();
    }

    inline void __cdecl Box_Officer_Screen_Chosen_Star_(int16_t p1, int16_t p2, int16_t p3, int16_t p4) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005132B0});
        fn(p1, p2, p3, p4);
    }

    void __cdecl Box_Fleet_Screen_Scanned_Ship_(int16_t map_x_off, int16_t map_y_off, int32_t scale_x, int32_t scale_y);

    inline const char* s_geggggg_format = reinterpret_cast<const char*>(std::uintptr_t{0x005569C0}); // ">GEGGGGG%s (%s)"

    void __cdecl Do_Fleet_Screen_Stuff_(int16_t map_x_off, int16_t map_y_off, int32_t scale_x, int32_t scale_y);

    void __cdecl Do_Officer_Screen_Stuff_(int16_t map_x_off, int16_t map_y_off, int scale_x, int scale_y);

    void __cdecl Draw_Galaxy_Map_Box_(s_animation_header* anim1, s_animation_header* anim2,
                                    int16_t x_base, int16_t y_base,
                                    int16_t width, int16_t height,
                                    int16_t map_x_off, int16_t map_y_off,
                                    int16_t x_adj, int16_t y_adj,
                                    int16_t view_mode, int16_t exit_flag);

    inline void __cdecl Get_Galaxy_Map_Star_XY_(int16_t star_idx, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t u1, int16_t u2, int16_t u3, int16_t* out_x, int16_t* out_y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005135B0});
        fn(star_idx, x1, y1, x2, y2, u1, u2, u3, out_x, out_y);
    }

    inline void __cdecl Add_Galaxy_Map_Fields_2_(int16_t arg1, int16_t arg2, int16_t height, int16_t grid_y,
                                                 int16_t arg5, int16_t arg6, int16_t arg7, int16_t arg8, int16_t arg9,
                                                 int16_t* name_field, int16_t* star_field, int16_t* grid_field) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t*, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00512640});
        fn(arg1, arg2, height, grid_y, arg5, arg6, arg7, arg8, arg9, name_field, star_field, grid_field);
    }
}