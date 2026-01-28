#include "deps.h"
#include "regions.h"

namespace regions {
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