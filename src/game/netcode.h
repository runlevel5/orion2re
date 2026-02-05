#pragma once
#include <cstdint>

namespace netcode {
    inline void __cdecl Net_Uninit_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534B40});
        fn();
    }
}