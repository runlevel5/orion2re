#pragma once

namespace strings {
    inline void Load_Strings() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00483F60 });
        fn();
    }
}