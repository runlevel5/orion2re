#pragma once
#include <cstdint>

namespace draw {
    inline uint8_t (*_glass_lookup_table)[256] = reinterpret_cast<uint8_t(*)[256]>(std::uintptr_t{ 0x0056A998 });

    void __cdecl Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* sprite);

    void __cdecl Draw_Bitmap_Sprite_(short x, short y, uint8_t* sprite);

    void __cdecl Color_Stream_Copy_(uint8_t* src, uint8_t* dst, int32_t count);
}