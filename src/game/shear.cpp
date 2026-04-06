#include "pch.h"

namespace shear {

void __cdecl Capture_Screen_Block_(uint8_t* src, int32_t width, int32_t height, uint8_t* dst)
{
    while (height != 0) {
        memcpy(dst, src, width);
        dst += width;
        src += g_screen_res_x;
        height--;
    }
}

void __cdecl Flip_Bitmap_Down_Different_(uint8_t* src, int width, int height, uint8_t* dst)
{
    uint8_t* current_dst = dst + width - 1;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            *current_dst = *src;
            current_dst--;
            src++;
        }
        current_dst += width * 2;
    }
}

void __cdecl Flip_Bitmap_Down_Same_(uint8_t* buffer, uint32_t width, uint32_t height) {
    uint8_t* row_start = buffer;
    uint8_t* row_end = buffer + width - 1;

    do {
        uint32_t half_width = width >> 1;
        uint8_t* left = row_start;
        uint8_t* right = row_end;

        while (half_width != 0) {
            uint8_t temp = *right;
            *right = *left;
            *left = temp;

            right--;
            left++;
            half_width--;
        }

        row_end += width;
        row_start += width;
        height--;
    } while (height != 0);
}

void __cdecl Flip_Bitmap_Left_Different_(uint8_t* src, uint32_t width, int32_t height, uint8_t* dst) {
    uint8_t* src_ptr = src + (height * width - width);

    do {
        memcpy(dst, src_ptr, width);
        dst += width;
        src_ptr -= width;
        height--;
    } while (height != 0);
}

void __cdecl Flip_Bitmap_Left_Same_(uint8_t* pixels, int width, uint32_t height) {
    uint8_t* bottom_row = pixels + (height * width - width);
    height >>= 1;
    uint8_t* top_row = pixels;

    do {
        uint8_t* cur_top = top_row;
        uint8_t* cur_bottom = bottom_row;
        int w = width;

        do {
            uint8_t temp = *cur_bottom;
            *cur_bottom = *cur_top;
            *cur_top = temp;

            cur_bottom++;
            cur_top++;
            w--;
        } while (w != 0);

        top_row += width;
        bottom_row -= width;
        height--;
    } while (height != 0);
}

} // namespace shear