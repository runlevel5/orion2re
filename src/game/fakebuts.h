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

    inline void Evaluate_Click_Thru_Fields_(int16_t input) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(0x004b5640)(input);
    }

    inline void Clear_Click_Thru_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004b55b0)();
    }
}