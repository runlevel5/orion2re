#pragma once

namespace council {
    inline void __cdecl Check_For_Council_Meeting_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004d9700 });
        fn();
    }
}