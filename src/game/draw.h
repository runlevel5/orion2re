#pragma once
#include <cstdint>

namespace draw {
    inline void __cdecl Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* data) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t*);
        reinterpret_cast<Fn>(0x00536508)(x, y, data);
    }

    void __cdecl Draw_Bitmap_Sprite_(short x, short y, uint8_t* sprite);
}