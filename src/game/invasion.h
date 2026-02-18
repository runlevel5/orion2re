#pragma once

namespace invasion {
    inline void __cdecl Check_All_Rebellions_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0042c5f0 });
        fn();
    }
}