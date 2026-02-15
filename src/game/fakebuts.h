#pragma once
#include <cstdint>

namespace FAKEBUTS {
    inline void __cdecl Clear_Fake_Buttons_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004B53F0});
        fn();
    }

    inline void __cdecl Draw_Fake_Buttons_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b5350 });
        fn();
    }
}