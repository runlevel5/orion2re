#pragma once
#include <cstdint>

struct s_animation_header;

namespace BEAMS {
    inline s_animation_header*& _mauler_ball_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00564D64});

    inline void __cdecl Init_Beams_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420ca0});
        fn();
    }

    inline void __cdecl Load_Ship_Burst_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420bc0});
        fn();
    }

    inline void __cdecl Load_Ship_Explosion_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420c50});
        fn();
    }

    inline void __cdecl Load_Shield_Hit_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004230e0});
        fn();
    }

    s_animation_header* __cdecl Reflection_Field_Anims_(int32_t p1, int32_t p2);
    void __cdecl Load_Reflection_Field_Hit_(int8_t should_load);

    void __cdecl Load_Combat_Beam_Stuff_(int8_t reflection_hit_param);
}