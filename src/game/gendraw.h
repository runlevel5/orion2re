#pragma once
#include <cstdint>

namespace GENDRAW {
    void __cdecl Message_Box_(const char* text);

    void __cdecl Help_(const char* message);

    // Suggested name: GENDRAW::Warning_Box_Prompt
    inline int16_t __cdecl Warning_Box_(const char* message) {
        using Fn = int16_t(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00479700});
        return fn(message);
    }

    inline int16_t __cdecl Warning_Box2_(const char* text) {
        return Warning_Box_(text);
    }

    // Suggested name: GENDRAW::Warning_Box_Alert
    inline void __cdecl Draw_Warning_Box_(const char* message) {
        using Fn = void(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796e0});
        fn(message);
    }

    // Suggested name: GENDRAW::Warning_Box_Notify
    inline void __cdecl Warning_Box_004796E0(char* text) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796e0});
        fn(text);
    }

    inline void __cdecl Message_Box_Exploding_(char* msg, int16_t star_idx) {
        using Fn = void(__cdecl*)(char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796D0});
        fn(msg, star_idx);
    }
}