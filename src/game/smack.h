#pragma once

#include <cstdint>

struct s_mouse_list;

namespace SMACK {
    inline s_mouse_list& _smack_mouse =
        *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005599D8});

    inline void*& _smack_auto_function =
        *reinterpret_cast<void**>(std::uintptr_t{0x005960EC});

    inline int16_t& _smack_x =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005960F0});

    inline int16_t& _smack_y =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005960F2});

    inline void __cdecl Smack_Fade_(int32_t fade_percent) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D7780});
        fn(fade_percent);
    }

    void __cdecl Play_Cinematic_(const char* smacker_name, uint32_t play_flags, char use_fade);
}