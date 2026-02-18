#pragma once

namespace aimove {
    inline void __cdecl Move_All_AI_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004658a0 });
        fn();
    }
}