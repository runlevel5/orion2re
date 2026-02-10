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
}