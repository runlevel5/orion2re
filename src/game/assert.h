#pragma once
#include <cstdint>

namespace assert {
    inline void __cdecl hgetch_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ab400});
        fn();
    }
}