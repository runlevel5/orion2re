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
        uint8_t* video_page = video::_current_video_page;

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

    /**
     * @brief Copies a stream of colors from src to dst, skipping color 0 (transparency).
     */
    void __cdecl Color_Stream_Copy_(uint8_t* src, uint8_t* dst, int32_t count) {
        for (int32_t i = 0; i < count; ++i) {
            uint8_t color = src[i];
            if (color != 0x00) {
                dst[i] = color;
            }
        }
    }

    /**
     * @brief Draws a RLE-encoded animated sprite frame to the current video page.
     *
     * The sprite data format:
     * [0-1]  Unknown / Unused
     * [2-3]  Start Y offset (relative to provided y)
     * [4..]  Command Stream
     *
     * Command types:
     * - If length is 0: Skip 'skip' lines and reset cursor to start of row.
     * - If length > 0: Skip 'skip' pixels on current line, then draw 'length' pixels.
     */
    void __cdecl Draw_Animated_Sprite_(int16_t x, int16_t y, uint8_t* sprite) {
        uint16_t sprite_y_offset = *reinterpret_cast<uint16_t*>(sprite + 2);

        // Initial destination row start (including x/y and internal sprite y-offset)
        // Screen width is assumed to be 640 (0x280)
        uint8_t* dest_row_start = video::_current_video_page +
            (static_cast<int32_t>(y) + sprite_y_offset) * 640 + x;
        uint8_t* dest_ptr = dest_row_start;

        // Loop continues for the remaining height of the animation
        int32_t remaining_height = static_cast<int32_t>(animate::_animation_header->height) - sprite_y_offset;
        uint16_t* stream = reinterpret_cast<uint16_t*>(sprite + 4);

        while (remaining_height > 0) {
            uint16_t length = *stream++;
            uint16_t skip = *stream++;

            if (length == 0) {
                // Skip lines command
                remaining_height -= skip;
                dest_row_start += static_cast<int32_t>(skip) * 640;
                dest_ptr = dest_row_start;
            }
            else {
                // Draw pixels command
                dest_ptr += skip;

                uint8_t* src_pixels = reinterpret_cast<uint8_t*>(stream);

                // Manual memory copy (optimized as REP MOVSD/MOVSB in assembly)
                uint32_t dword_count = length >> 2;
                uint32_t byte_count = length & 3;

                for (uint32_t i = 0; i < dword_count; ++i) {
                    *reinterpret_cast<uint32_t*>(dest_ptr) = *reinterpret_cast<uint32_t*>(src_pixels);
                    dest_ptr += 4;
                    src_pixels += 4;
                }

                for (uint32_t i = 0; i < byte_count; ++i) {
                    *dest_ptr++ = *src_pixels++;
                }

                // Source stream maintains 2-byte alignment for command headers
                if (reinterpret_cast<std::uintptr_t>(src_pixels) & 1) {
                    src_pixels++;
                }
                stream = reinterpret_cast<uint16_t*>(src_pixels);
            }
        }
    }
}