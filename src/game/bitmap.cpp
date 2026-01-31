#include "deps.h"

namespace bitmap {
    void __cdecl Get_Bitmap_Actual_Size_(animate::s_animation_header* header, int16_t* out_min_x, int16_t* out_min_y, int16_t* out_actual_width, int16_t* out_actual_height) {
        animate::_animation_header = header;
        int16_t width = header->width;
        int16_t height = header->height;

        // Pixel data starts at offset 16 in this bitmap format
        uint8_t* pixels = reinterpret_cast<uint8_t*>(header) + 16;

        *out_min_x = 1000;
        *out_min_y = 1000;
        int16_t max_x = 0;
        int16_t max_y = 0;

        int16_t pixel_idx = 0;
        if (height > 0) {
            for (int16_t y = 0; y < height; ++y) {
                if (width > 0) {
                    for (int16_t x = 0; x < width; ++x) {
                        // Check if pixel is non-zero (not transparent)
                        if (pixels[pixel_idx] != 0) {
                            if (x < *out_min_x) {
                                *out_min_x = x;
                            }
                            if (y < *out_min_y) {
                                *out_min_y = y;
                            }
                            if (x > max_x) {
                                max_x = x;
                            }
                            if (y > max_y) {
                                max_y = y;
                            }
                        }
                        pixel_idx++;
                    }
                }
            }
        }

        *out_actual_height = (max_y - *out_min_y) + 1;
        *out_actual_width = (max_x - *out_min_x) + 1;
    }
}