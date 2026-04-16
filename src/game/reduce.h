#pragma once
#include <cstdint>

struct s_animation_header;

namespace reduce {
    inline uint16_t& _ncubes =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00587FE0});

    void __cdecl Clear_Reduced_Palette_Groups_();

    inline void __cdecl Add_Reduced_Palette_Group_(s_animation_header* anim) {
        using Fn = void(__cdecl*)(s_animation_header*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534660});
        fn(anim);
    }

    inline void __cdecl Compress_Reduced_Palette_Groups_(int32_t p1, int32_t p2) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534710});
        fn(p1, p2);
    }
}