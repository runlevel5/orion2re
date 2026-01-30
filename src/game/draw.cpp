#include "deps.h"

namespace draw {
    /**
     * @brief Draws a sprite to the current video page using the dimensions from the global animation header.
     * 
     * The function performs basic transparency (skips pixel value 0).
     */
    void __cdecl Draw_Bitmap_Sprite_(short x, short y, uint8_t* sprite) {
        if (!animate::_animation_header) {
            return;
        }

        int32_t width = static_cast<int32_t>(animate::_animation_header->width);
        int32_t height = static_cast<int32_t>(animate::_animation_header->height);
        uint8_t* video_page = reinterpret_cast<uint8_t*>(video::_current_video_page);

        // Screen is 640 pixels wide. Calculate starting destination.
        uint8_t* dest_row = video_page + (static_cast<int32_t>(y) * 640) + static_cast<int32_t>(x);

        for (int32_t h = 0; h < height; ++h) {
            for (int32_t w = 0; w < width; ++w) {
                uint8_t pixel = *sprite++;
                
                // Color 0 is treated as transparent.
                if (pixel != 0) {
                    dest_row[w] = pixel;
                }
            }
            // Move to the same X position on the next scanline.
            dest_row += 640;
        }
    }
}