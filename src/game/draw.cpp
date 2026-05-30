#include "pch.h"

namespace draw {
    /**
     * @brief Draws a sprite to the current video page using the dimensions from the global animation header.
     * 
     * The function performs basic transparency (skips pixel value 0).
     */
    void __cdecl Draw_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* sprite) {
        uint16_t width = (uint16_t)animate::_animation_header->width;
        uint16_t height = (uint16_t)animate::_animation_header->height;
        uint8_t* video_page = video::_current_video_page;

        uint8_t* dst = video_page + (uint16_t)x + (uint16_t)y * 640u - 640u + width;

        while (height != 0) {
            --height;
            dst += 640;
            dst -= width;
            --dst;

            uint32_t count = width;
            while (true) {
                ++dst;
                if (count == 0) {
                    break;
                }

                --count;
                uint8_t pixel = *sprite++;
                if (pixel != 0) {
                    *dst = pixel;
                }
            }
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
        uint16_t sprite_y_offset;
        memcpy(&sprite_y_offset, sprite + 2, sizeof(sprite_y_offset));

        uint32_t x_pos = (uint16_t)x;
        uint32_t y_pos = (uint16_t)y;
        uint32_t sprite_y_pos = sprite_y_offset;
        int32_t remaining_height =
            (int32_t)(uint16_t)animate::_animation_header->height - (int32_t)sprite_y_offset;

        // Initial destination row start (including x/y and internal sprite y-offset)
        // Screen width is assumed to be 640 (0x280)
        uint8_t* dest_row_start = video::_current_video_page +
            (y_pos + sprite_y_pos) * 640u + x_pos;
        uint8_t* dest_ptr = dest_row_start;

        // Loop continues for the remaining height of the animation
        uint8_t* stream = sprite + 4;

        while (remaining_height > 0) {
            uint16_t length;
            uint16_t skip;
            memcpy(&length, stream, sizeof(length));
            memcpy(&skip, stream + 2, sizeof(skip));
            stream += 4;

            if (length == 0) {
                // Skip lines command
                remaining_height -= skip;
                dest_row_start += (uint32_t)skip * 640u;
                dest_ptr = dest_row_start;
            }
            else {
                // Draw pixels command
                dest_ptr += skip;

                memcpy(dest_ptr, stream, length);
                dest_ptr += length;
                stream += length;

                // Source stream maintains 2-byte alignment for command headers
                if ((uintptr_t)stream & 1) {
                    stream++;
                }
            }
        }
    }
}