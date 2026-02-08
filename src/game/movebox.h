#pragma once
#include <cstdint>

namespace MOVEBOX {
    inline void __cdecl Clear_Moveable_Box_Fields_(int16_t order_id) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005129D0 });
        fn(order_id);
    }

    bool __cdecl Moveable_Box_Selected_(int16_t index);

    inline void __cdecl Vanish_Fleet_Movement_Box_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00513D30});
        fn();
    }

    inline void __cdecl Save_And_Remove_Boxes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00513C90})();
    }

    inline void __cdecl Restore_Boxes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00513C00})();
    }

    void __cdecl Move_Box_To_Top_(int16_t box_id);
}