#pragma once

#include <cstdint>

namespace sysfont {
    inline void __cdecl Load_System_Font_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005383b0});
        fn();
    }
}