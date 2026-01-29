#pragma once
#include <cstdint>

namespace regions {
    void __cdecl Fill_Column_(uint8_t color, uint8_t* surface, uint32_t fill_width, uint32_t fill_height);

    void __cdecl Fill_Region_(uint8_t color, uint8_t* surface, uint32_t width_blocks, uint32_t height);
}