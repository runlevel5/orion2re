#pragma once
#include <cstdint>

namespace FLCQUEUE {
    inline void __cdecl Allocate_Flic_Queue_And_Pointers_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458250});
        fn();
    }

    inline void __cdecl Free_Flic_Queue_And_Pointers_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458240});
        fn();
    }
}