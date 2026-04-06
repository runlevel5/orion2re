#include "pch.h"

namespace shear {

void __cdecl Capture_Screen_Block_(uint8_t* src, int32_t width, int32_t height, uint8_t* dst)
{
    do {
        memcpy(dst, src, width);
        src += g_screen_res_x;
        dst += width;
        height--;
    } while (height != 0);
}

void __cdecl Flip_Bitmap_Down_Different_(uint8_t* src, int32_t width, int32_t height, uint8_t* dst)
{
    uint8_t* d = dst + width - 1;
    int32_t w = width;

    do {
        do {
            *d = *src;
            src++;
            d--;
            w--;
        } while (w != 0);

        d += width * 2;
        height--;
        w = width;
    } while (height != 0);
}

void __cdecl Flip_Bitmap_Down_Same_(uint8_t* data, uint32_t width, uint32_t height) {
    uint8_t* row_start = data;
    uint8_t* row_end = data + width - 1;

    for (uint32_t y = 0; y < height; ++y) {
        uint32_t half_width = width >> 1;
        uint8_t* left = row_start;
        uint8_t* right = row_end;

        for (uint32_t x = 0; x < half_width; ++x) {
            uint8_t temp = *right;
            *right = *left;
            *left = temp;
            right--;
            left++;
        }

        row_start += width;
        row_end += width;
    }
}

void __cdecl Flip_Bitmap_Left_Different_(uint8_t* src, uint32_t width, uint32_t height, uint8_t* dst) {
    uint8_t* src_ptr = src + (height * width - width);

    do {
        memcpy(dst, src_ptr, width);
        dst += width;
        src_ptr -= width;
        height--;
    } while (height != 0);
}

void __cdecl Flip_Bitmap_Left_Same_(uint8_t* pixels, uint32_t width, uint32_t height) {
    uint8_t* bottom_row = pixels + (height * width - width);
    height >>= 1;
    uint8_t* top_row = pixels;

    do {
        uint8_t* cur_top = top_row;
        uint8_t* cur_bottom = bottom_row;
        uint32_t w = width;

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