#pragma once
#include <cstdint>

namespace COLONY {
    inline void __cdecl Init_Colonies_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004dbfd0});
        fn();
    }
}