#pragma once
#include <cstdint>

namespace GENDRAW {
    inline void __cdecl Message_Box_(const char* text) {
        using Fn = void(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796b0});
        fn(text);
    }

    inline void __cdecl Warning_Box_(const char* msg) {
        using Fn = void(__cdecl*)(const char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004796E0})(msg);
    }
}