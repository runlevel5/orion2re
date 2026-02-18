#pragma once

namespace aidudes {
    inline void __cdecl All_Colony_AI_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00446b20 });
        fn();
    }
}