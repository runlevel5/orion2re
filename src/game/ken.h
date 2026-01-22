#pragma once

namespace KEN {
    inline void Ken_Init_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004C1D40 });
        fn();
    }
}