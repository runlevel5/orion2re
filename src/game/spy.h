#pragma once

namespace spy {
    inline void __cdecl Resolve_Spies_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b8be0 });
        fn();
    }
}