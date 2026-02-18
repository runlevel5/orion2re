#pragma once

namespace settler {
    inline void __cdecl Move_Settlers_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043be10 });
        fn();
    }
}