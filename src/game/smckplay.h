#pragma once

#include <cstdint>

namespace smckplay {
    inline void __cdecl Open_Smacker_Animation_(const char* smacker_name, int8_t music_on, uint32_t play_flags) {
        using Fn = void(__cdecl*)(const char*, int8_t, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005358A0});
        fn(smacker_name, music_on, play_flags);
    }

    inline int32_t __cdecl Get_Smacker_Animation_Width_() {
        using Fn = int32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005359C0});
        return fn();
    }

    inline int32_t __cdecl Get_Smacker_Animation_Height_() {
        using Fn = int32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005359A0});
        return fn();
    }

    inline void __cdecl Draw_Smacker_Animation_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D77F0});
        fn();
    }

    inline int16_t __cdecl Get_Smacker_Animation_Total_Frames_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005359E0});
        return fn();
    }

    inline int16_t __cdecl Get_Smacker_Animation_Frame_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535A00});
        return fn();
    }

    inline void __cdecl Close_Smacker_Animation_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535BA0});
        fn();
    }
}