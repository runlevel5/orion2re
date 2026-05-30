#pragma once

#include <cstdint>

namespace palstore {
    int16_t Slow_Fade_In_();

    void __cdecl Darken_Palette_(int32_t darken_percent);

    void __cdecl Store_Palette_Block_(int32_t first_entry, int32_t entry_count);

    void __cdecl Store_Single_Palette_Color_(int32_t palette_index, uint8_t red, uint8_t green, uint8_t blue);
}