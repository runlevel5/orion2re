#pragma once
#include <cstdint>

namespace sprite {
    inline uint16_t __cdecl Get_Sprite_Width_(void* sprite_data) {
        using Fn = uint16_t(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00524D20});
        return fn(sprite_data);
    }

    inline uint16_t __cdecl Get_Sprite_Height_(void* sprite_data) {
        using Fn = uint16_t(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00524D30});
        return fn(sprite_data);
    }
}