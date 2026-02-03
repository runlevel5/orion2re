#pragma once
#include <cstdint>

namespace GENDRAW {
    inline void __cdecl Message_Box_(const char* text) {
        using Fn = void(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796b0});
        fn(text);
    }

    inline bool __cdecl Warning_Box_00479700(const char* text) {
        using Fn = bool(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00479700});
        return fn(text);
    }

    inline void __cdecl Warning_Box_004796e0(const char* text) {
        using Fn = void(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796e0});
        fn(text);
    }

    inline void __cdecl Warning_Box_(const char* msg) {
        Warning_Box_004796e0(msg);
    }
}