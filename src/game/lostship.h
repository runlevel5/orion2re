#pragma once

namespace lostship {
    inline void __cdecl Lose_Out_Of_Range_Ships_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00470900 });
        fn();
    }
}