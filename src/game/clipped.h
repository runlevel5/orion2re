#pragma once
#include <cstdint>

namespace clipped {
    void __cdecl Clipped_Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* sprite);

    void __cdecl Clipped_Draw_Glassed_Animated_Sprite_(int32_t x, int32_t y, uint8_t* data);

    void __cdecl Clipped_Draw_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* pixels);

    void __cdecl Clipped_Draw_Glassed_Bitmap_Sprite_(int32_t x, int32_t y, uint8_t* data);

    void __cdecl Clipped_Draw_Stenciled_Animated_Sprite_(int16_t x, int16_t y, uint8_t* data);
}