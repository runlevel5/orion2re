#pragma once
#include <cstdint>

namespace FLCQUEUE {
    inline void __cdecl Allocate_Flic_Queue_And_Pointers_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458250});
        fn();
    }

    inline void __cdecl Free_Flic_Queue_And_Pointers_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00458240});
        fn();
    }

    inline void __cdecl Draw_Flic_Queue_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004582C0});
        fn();
    }

    inline void __cdecl Add_Flic_To_End_Of_Queue_(int16_t x, int16_t y, s_animation_header *anim, uint8_t flag1, uint8_t flag2) {
        using Fn = void(__cdecl *)(int16_t, int16_t, s_animation_header *, uint8_t, uint8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00458260})(x, y, anim, flag1, flag2);
    }

    inline void __cdecl Add_Explosion_Anim_(int32_t x, int32_t y) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004a2800});
        fn(x, y);
    }
}