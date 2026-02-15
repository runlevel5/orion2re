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

    inline void __cdecl Draw_Galaxy_Map_Box_(void* header, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12) {
        using Fn = void(__cdecl*)(void*, int, int, int, int, int, int, int, int, int, int, int);
        reinterpret_cast<Fn>(std::uintptr_t{0x00512ab0})(header, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
    }

    inline void __cdecl Get_Galaxy_Map_Star_XY_(int16_t star_idx, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t u1, int16_t u2, int16_t u3, int16_t* out_x, int16_t* out_y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005135B0});
        fn(star_idx, x1, y1, x2, y2, u1, u2, u3, out_x, out_y);
    }
}