#pragma once

namespace remap {
    inline void __cdecl Update_Glass_Remap_Colors_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x0052BC90)();
    }

    inline void __cdecl Calculate_Picture_Remap_Colors_(s_animation_header* anim) {
        using Fn = void(__cdecl*)(s_animation_header*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052c020});
        fn(anim);
    }

    inline uint8_t __cdecl Get_Remap_Palette_Color_(int32_t index) {
        using Fn = uint8_t(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052c0e0});
        return fn(index);
    }
}