#include "pch.h"

namespace regions {
    void __cdecl Fill_Region_(uint8_t color, uint8_t* surface, uint32_t width_blocks, uint32_t height) {
        uint32_t fill_pattern = color;
        fill_pattern |= (fill_pattern << 8);
        fill_pattern |= (fill_pattern << 16);

        uint8_t* current_row = surface;
        int32_t stride_skip = platform::g_screen_res_x - static_cast<int32_t>(width_blocks * 4);

        for (uint32_t y = 0; y < height; ++y) {
            uint32_t* dword_ptr = reinterpret_cast<uint32_t*>(current_row);

            for (uint32_t x = 0; x < width_blocks; ++x) {
                *dword_ptr++ = fill_pattern;
            }

            current_row = reinterpret_cast<uint8_t*>(dword_ptr) + stride_skip;
        }
    }

    void __cdecl Fill_Column_(uint8_t color, uint8_t* surface, uint32_t fill_width, uint32_t fill_height) {
        int32_t stride_skip = platform::g_screen_res_x - static_cast<int32_t>(fill_width);

        for (uint32_t y = 0; y < fill_height; ++y) {
            for (uint32_t x = 0; x < fill_width; ++x) {
                *surface = color;
                surface++;
            }
            surface += stride_skip;
        }
    }
}