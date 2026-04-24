#pragma once
#include <cstdint>

namespace GENDRAW {
    void __cdecl Message_Box_(const char* text);

    void __cdecl Help_(const char* message);

    inline int16_t __cdecl Confirmation_Box_(const char* message) {
        using Fn = int16_t(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00479700});
        return fn(message);
    }

    inline void __cdecl Warning_Box_(const char* message) {
        using Fn = void(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796e0});
        fn(message);
    }

    inline void __cdecl Message_Box_Exploding_Star_(char* msg, int16_t star_idx) {
        using Fn = void(__cdecl*)(char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796D0});
        fn(msg, star_idx);
    }
}