#pragma once
#include <cstdint>

namespace clipped {
    inline void __cdecl Clipped_Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* data) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t*);
        reinterpret_cast<Fn>(0x00536040)(x, y, data);
    }

    inline void __cdecl Clipped_Draw_Glassed_Animated_Sprite_(int32_t x, int32_t y, uint8_t* data) {
        using Fn = void(__cdecl*)(int32_t, int32_t, uint8_t*);
        reinterpret_cast<Fn>(0x00535E50)(x, y, data);
    }

    inline void __cdecl Clipped_Draw_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* data) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t*);
        reinterpret_cast<Fn>(0x00535D20)(x, y, data);
    }

    inline void __cdecl Clipped_Draw_Glassed_Bitmap_Sprite_(int32_t x, int32_t y, uint8_t* data) {
        using Fn = void(__cdecl*)(int32_t, int32_t, uint8_t*);
        reinterpret_cast<Fn>(0x00535C00)(x, y, data);
    }

    inline void __cdecl Clipped_Draw_Stenciled_Animated_Sprite_(int16_t x, int16_t y, uint8_t* data) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t*);
        reinterpret_cast<Fn>(0x00536280)(x, y, data);
    }
}