#pragma once
#include <cstdint>

namespace graphics {
    void Reset_Window_();
    void __cdecl Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    void __cdecl Set_Window_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
}