#pragma once
#include <cstdint>

namespace ESTRINGS {
    inline void Load_E_Strings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a0060});
        fn();
    }
}