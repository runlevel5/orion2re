#pragma once
#include <cstdint>

namespace ESTRINGS {
    inline char* (&_strings)[812] =
        *reinterpret_cast<char* (*)[812]>(std::uintptr_t{0x0056B788});
    inline const char*& s_0_0055110c = *reinterpret_cast<const char**>(std::uintptr_t{0x0055110C}); // " 0"
    inline const char*& s_1_00551110 = *reinterpret_cast<const char**>(std::uintptr_t{0x00551110}); // " 1"

    inline void Load_E_Strings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a0060});
        fn();
    }

    char* __cdecl E_Strings_(int16_t index);
}