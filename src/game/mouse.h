#pragma once

namespace mouse {
    inline void Set_Mouse_List_(uint8_t* mouse_data, int32_t count) {
        using Fn = void(*)(uint8_t*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005274D0 });
        fn(mouse_data, count);
    }
}