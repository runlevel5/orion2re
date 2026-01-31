#pragma once
#include <cstdint>

namespace MOVEBOX {
    bool __cdecl Moveable_Box_Selected_(int16_t index);

    inline void __cdecl Vanish_Fleet_Movement_Box_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00513D30});
        fn();
    }

    void __cdecl Move_Box_To_Top_(int16_t box_id);
}