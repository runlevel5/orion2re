#pragma once

namespace strings {
    inline void __cdecl Swap_Short_(int16_t* a, int16_t* b) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052CE60 });
        fn(a, b);
    }

    inline void Load_Strings() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00483F60 });
        fn();
    }
}