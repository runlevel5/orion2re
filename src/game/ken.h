#pragma once

namespace KEN {
    inline void __cdecl Ken_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004AA050 })();
    }
}