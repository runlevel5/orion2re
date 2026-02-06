#pragma once
#include <cstdint>

namespace line {
    int16_t __cdecl Clip_Line_(int16_t* x1, int16_t* y1, int16_t* x2, int16_t* y2);

    void __cdecl Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

    void __cdecl Draw_Vertical_Run_(uint8_t** buf, int16_t param_2, int16_t count, uint8_t color);
    void __cdecl Draw_Horizontal_Run_(uint8_t** buf, int16_t step_x, int16_t count, uint8_t color);
}