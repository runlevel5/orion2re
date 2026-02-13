#pragma once
#include <cstdint>

namespace ESTRINGS {
    inline void Load_E_Strings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a0060});
        fn();
    }

    inline char* __cdecl E_Strings_(int id) {
        using Fn = char*(__cdecl*)(int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004A0050});
        return fn(id);
    }
}