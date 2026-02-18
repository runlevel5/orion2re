#pragma once

namespace aileader {
    inline void __cdecl Do_AI_Leaders_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0042a230 });
        fn();
    }
}