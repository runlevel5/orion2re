#pragma once
#include <cstdint>

namespace draw {
    void __cdecl Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* sprite);

    void __cdecl Draw_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* sprite);

    void __cdecl Color_Stream_Copy_(uint8_t* src, uint8_t* dst, int32_t count);
}