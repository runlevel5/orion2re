#pragma once
#include <cstdint>

namespace GENDRAW {
    // Suggested name: GENDRAW::Message_Box
    inline void __cdecl Message_Box_(char* message) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796b0});
        fn(message);
    }

    // Suggested name: GENDRAW::Warning_Box_Prompt
    inline int16_t __cdecl Warning_Box_00479700(char* message) {
        using Fn = int16_t(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00479700});
        return fn(message);
    }

    // Suggested name: GENDRAW::Warning_Box_Alert
    inline void __cdecl Draw_Warning_Box_(const char* message) {
        using Fn = void(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796e0});
        fn(message);
    }

    inline void __cdecl Warning_Box_(const char* msg) {
        Draw_Warning_Box_(msg);
    }

    // Suggested name: GENDRAW::Warning_Box_Notify
    inline void __cdecl Warning_Box_004796E0(char* text) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004796e0});
        fn(text);
    }
}