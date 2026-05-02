#include "pch.h"

namespace shear {

void __cdecl Capture_Screen_Block_(uint8_t* src, int32_t width, int32_t height, uint8_t* dst)
{
    do {
        memcpy(dst, src, width);
        src += platform::g_screen_res_x;
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

void __cdecl Outline_Bitmap_Pixels_(uint8_t* src, uint32_t outline_color, uint32_t unused, uint8_t* dest, uint32_t width, uint32_t height) {
    _outline_color = outline_color;

    // BUG: width * height?
    uint32_t pixel_count = width * width;
    bitmap::_pixel_count = pixel_count;
    _temp = 0;

    uint32_t col = width;
    uint8_t out_c = _outline_color & 0xFF;

    bool was_solid = false;
    bool was_empty = false;

    uint8_t* s = src;
    uint8_t* d = dest;

    // Horizontal pass
    for (uint32_t i = 0; i < pixel_count; ++i) {
        uint8_t pixel = *s++;

        if (pixel == 0 || pixel == out_c) {
            if (was_solid) {
                *d = out_c;
            }
            was_solid = false;
            was_empty = true;
        } else {
            if (was_empty) {
                *(d - 1) = out_c;
            }
            was_empty = false;
            was_solid = true;
        }

        d++;
        col--;
        if (col == 0) {
            col = width;
            was_solid = false;
            was_empty = false;
        }
    }

    // BUG: width * height?
    pixel_count = width * width;
    bitmap::_pixel_count = pixel_count;
    _src_bitmap_end = src + pixel_count;

    s = src;
    d = dest;
    was_solid = false;
    was_empty = false;

    // Vertical pass
    for (uint32_t i = 0; i < pixel_count; ++i) {
        uint8_t pixel = *s;

        if (pixel == 0 || pixel == out_c) {
            if (was_solid) {
                *d = out_c;
            }
            was_solid = false;
            was_empty = true;
        } else {
            if (was_empty) {
                *(d - width) = out_c;
            }
            was_empty = false;
            was_solid = true;
        }

        s += width;
        d += width;

        if (s >= _src_bitmap_end) {
            s -= pixel_count;
            d -= pixel_count;
            s++;
            d++;
            was_solid = false;
            was_empty = false;
        }
    }
}

void __cdecl Outline_Bitmap_Pixels_No_Glass_(uint8_t* src, uint32_t outline_color, uint32_t unused, uint8_t* dest, uint32_t width, uint32_t height) {
    _outline_color = outline_color;
    // BUG: width * height?
    uint32_t pixel_count = width * width;
    bitmap::_pixel_count = pixel_count;
    _temp = 0;

    uint8_t color = static_cast<uint8_t>(outline_color);
    uint32_t x_count = width;
    bool was_glass = false;
    bool was_solid = false;

    uint8_t* current_src = src;
    uint8_t* current_dest = dest;

    // Pass 1: Horizontal outline
    for (uint32_t i = 0; i < pixel_count; ++i) {
        uint8_t pixel = *current_src++;

        if (pixel == 0 || pixel == color || pixel >= 0xF0) {
            // Glass pixel
            if (was_solid) {
                *current_dest = color;
            }
            was_solid = false;
            was_glass = true;
        } else {
            // Solid pixel
            if (was_glass) {
                *(current_dest - 1) = color;
            }
            was_glass = false;
            was_solid = true;
        }

        current_dest++;
        x_count--;

        if (x_count == 0) {
            x_count = width;
            was_glass = false;
            was_solid = false;
        }
    }

    // Pass 2: Vertical outline (column-major traversal)
    // BUG: width * height?
    pixel_count = width * width;
    bitmap::_pixel_count = pixel_count;
    _src_bitmap_end = src + pixel_count;

    current_src = src;
    current_dest = dest;
    was_glass = false;
    was_solid = false;

    for (uint32_t i = 0; i < pixel_count; ++i) {
        uint8_t pixel = *current_src;

        if (pixel == 0 || pixel == color || pixel >= 0xF0) {
            // Glass pixel
            if (was_solid) {
                *current_dest = color;
            }
            was_solid = false;
            was_glass = true;
        } else {
            // Solid pixel
            if (was_glass) {
                *(current_dest - width) = color;
            }
            was_glass = false;
            was_solid = true;
        }

        current_src += width;
        current_dest += width;

        // Wrap around to the next column when reaching the end
        if (current_src >= _src_bitmap_end) {
            current_src -= pixel_count;
            current_dest -= pixel_count;
            current_src++;
            current_dest++;
            was_glass = false;
            was_solid = false;
        }
    }
}

void __cdecl Interlace_Scale_Off_Page_ASM_(uint8_t* page, uint8_t* lut) {
    _row_temp = 240;
    uint8_t* ptr = page;

    do {
        for (int i = 0; i < 640; i++) {
            ptr[i] = lut[ptr[i]];
        }

        ptr += 1280;
        _row_temp--;
    } while (_row_temp != 0);
}

void __cdecl Mask_Bitmap_Pixels_(uint8_t* src, int32_t src_row_skip, int32_t dst_row_skip, uint8_t* dst, int32_t width, int32_t height) {
    _src_row_skip = src_row_skip;
    _dst_row_skip = dst_row_skip;

    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            if (*src != 0) {
                *dst = 0;
            }
            src++;
            dst++;
        }
        src += _src_row_skip;
        dst += _dst_row_skip;
    }
}

void __cdecl Mask_Bitmap_Pixels_Inverse_(uint8_t* src, int32_t src_row_skip, int32_t dst_row_skip, uint8_t* dst, int32_t width, int32_t height) {
    _src_row_skip = src_row_skip;
    _dst_row_skip = dst_row_skip;

    for (int32_t y = 0; y < height; ++y) {
        for (int32_t x = 0; x < width; ++x) {
            if (*src == 0) {
                *dst = 0;
            }
            src++;
            dst++;
        }
        src += _src_row_skip;
        dst += _dst_row_skip;
    }
}

void __cdecl Mask_Brighten_Bitmap_Pixels_(uint8_t* src, int32_t src_row_skip, int32_t dst_row_skip, uint8_t* dst, int32_t width, int32_t height) {
    _src_row_skip = src_row_skip;
    _dst_row_skip = dst_row_skip;

    for (int32_t h = height; h != 0; --h) {
        for (int32_t w = width; w != 0; --w) {
            uint8_t src_pixel = *src++;
            if (src_pixel != 0 && *dst != 0) {
                uint32_t color_offset = (static_cast<uint8_t>(src_pixel - bitmap::_brighten_start_color) << 8) + *dst;
                *dst = static_cast<uint8_t*>(bitmap::_brighten_palette_seg)[color_offset];
            }
            dst++;
        }
        src += _src_row_skip;
        dst += _dst_row_skip;
    }
}

void __cdecl Mask_Bitmap_Colors_Pixels_(uint8_t* pixels, uint32_t min_color, uint32_t max_color, uint32_t unused_param, uint32_t width, uint32_t height) {
    _col_temp = min_color;
    _row_temp = max_color;

    uint32_t count = (width * height) / 4;
    uint32_t* ptr = reinterpret_cast<uint32_t*>(pixels);

    uint8_t min_c = static_cast<uint8_t>(min_color);
    uint8_t max_c = static_cast<uint8_t>(max_color);

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t val = ptr[i];

        if (static_cast<uint8_t>(val) >= min_c && static_cast<uint8_t>(val) <= max_c) {
            val &= 0xFFFFFF00;
        }
        val = (val >> 8) | (val << 24);

        if (static_cast<uint8_t>(val) >= min_c && static_cast<uint8_t>(val) <= max_c) {
            val &= 0xFFFFFF00;
        }
        val = (val >> 8) | (val << 24);

        if (static_cast<uint8_t>(val) >= min_c && static_cast<uint8_t>(val) <= max_c) {
            val &= 0xFFFFFF00;
        }
        val = (val >> 8) | (val << 24);

        if (static_cast<uint8_t>(val) >= min_c && static_cast<uint8_t>(val) <= max_c) {
            val &= 0xFFFFFF00;
        }
        val = (val >> 8) | (val << 24);

        ptr[i] = val;
    }
}

void __cdecl Enlarge_Bitmap_(uint8_t* src_ptr, uint32_t scale_x, uint32_t scale_y, uint8_t* dst_ptr, uint32_t src_width, uint32_t src_height, uint32_t dst_width, uint32_t dst_height) {
    _src_width = scale_x;
    _src_height = scale_y;

    uint64_t src_size = (uint64_t)src_width * src_height;
    uint32_t src_offset = (uint32_t)src_size + ((uint32_t)(src_size >> 32) << 16) - 1;
    src_ptr += src_offset;

    _x_step_8_8 = (uint16_t)(0x6400 / (uint16_t)scale_x);

    uint32_t y_step_temp = (uint16_t)(0x6400 / (uint16_t)scale_y);
    _y_step_8_8 = (src_offset & 0xFFFF0000) | y_step_temp;

    uint64_t dst_size = (uint64_t)dst_width * dst_height;
    uint32_t dst_offset = (uint32_t)dst_size + ((uint32_t)(dst_size >> 32) << 16) - 1;
    dst_ptr += dst_offset;

    _dst_width = (uint16_t)dst_width;
    _y_accum_8_8 = 0;

    uint32_t x_step = _x_step_8_8;

    uint32_t y = dst_height;
    do {
        uint8_t* row_src_ptr = src_ptr;
        uint32_t x_accum = 0;

        uint32_t x = dst_width;
        do {
            *dst_ptr = *src_ptr;
            dst_ptr--;

            x_accum += x_step;
            uint32_t step_int = x_accum >> 8;
            src_ptr -= step_int;
            x_accum &= 0xFF;

            x--;
        } while (x != 0);

        src_ptr = row_src_ptr;
        _y_accum_8_8 += _y_step_8_8;

        if (((_y_accum_8_8 >> 8) & 0xFF) != 0) {
            src_ptr -= src_width;
            _y_accum_8_8 &= 0xFFFF00FF;
        }

        y--;
    } while (y != 0);
}

void __cdecl Reduce_Bitmap_(
    uint8_t* src_ptr, uint32_t src_width, uint32_t src_height,
    uint8_t* dst_ptr, uint32_t src_pitch, uint32_t unused_param,
    uint32_t dst_width, uint32_t dst_height)
{
    _src_width = src_width;
    _src_height = src_height;

    // The original inline assembly leaves the upper 16 bits of EAX intact,
    // which contains the src_ptr from its initialization block.
    uint32_t eax_reg = reinterpret_cast<uint32_t>(src_ptr);

    uint16_t x_step = 0x6400 / (src_width & 0xFFFF);
    _x_step_8_8 = x_step;

    uint16_t y_step = 0x6400 / (src_height & 0xFFFF);
    eax_reg = (eax_reg & 0xFFFF0000) | y_step;
    _y_step_8_8 = eax_reg;

    _dst_width = dst_width & 0xFFFF;
    _y_accum_8_8 = 0;

    uint32_t edx_reg = x_step;

    do {
        uint8_t* esi_reg = src_ptr;
        uint32_t ecx_reg = dst_width;
        uint32_t ebx_reg = 0;

        do {
            *dst_ptr++ = *esi_reg;
            ebx_reg += edx_reg;

            uint32_t eax_temp = ebx_reg;
            eax_temp >>= 8;
            esi_reg += eax_temp;

            ebx_reg &= 0xFF;

            ecx_reg--;
        } while (ecx_reg != 0);

        ecx_reg = _y_accum_8_8;
        ecx_reg += _y_step_8_8;

        eax_reg = src_pitch;

        // Emulates an 8-bit `MUL CH` bug where upper 16 bits of EAX are unchanged
        uint8_t ch = (ecx_reg >> 8) & 0xFF;
        uint8_t al = eax_reg & 0xFF;
        uint16_t ax = al * ch;

        eax_reg = (eax_reg & 0xFFFF0000) | ax;

        src_ptr += eax_reg;

        // Emulates `XOR CH, CH` which clears bits 8-15
        ecx_reg &= 0xFFFF00FF;

        _y_accum_8_8 = ecx_reg;

        dst_height--;
    } while (dst_height != 0);
}

void __cdecl Vanish_Bitmap_Pixels_(uint8_t* src, uint32_t noise_threshold, uint32_t unused_param, uint8_t* dst, uint32_t width, uint32_t height) {
    _noise_threshold = noise_threshold;

    uint32_t rand_val = random::Random_(1000);
    _noise_start_index = (rand_val - 1) & 0xFFFE;

    uint32_t total_pixels = width * height;
    uint32_t dword_count = total_pixels >> 2;
    if ((total_pixels & 3) != 0) {
        dword_count++;
    }

    _row_dword_count = width >> 2;
    _row_dword_counter = _row_dword_count;

    uint32_t noise_idx = _noise_start_index & 0xFE;

    uint32_t* src32 = reinterpret_cast<uint32_t*>(src);
    uint32_t* dst32 = reinterpret_cast<uint32_t*>(dst);

    for (uint32_t ecx = dword_count; ecx > 0; ecx--) {
        uint32_t pixels = *src32++;
        uint32_t noise = *reinterpret_cast<uint32_t*>(_noise_table + noise_idx);
        noise_idx = (noise_idx + 4) & 0xFF;

        for (int i = 0; i < 4; ++i) {
            if (static_cast<uint8_t>(noise) < static_cast<uint8_t>(_noise_threshold)) {
                pixels &= 0xFFFFFF00;
            }
            pixels = (pixels >> 8) | (pixels << 24);
            noise = (noise >> 8) | (noise << 24);
        }

        *dst32++ = pixels;

        _row_dword_counter--;
        // Emulates a bug in original inline assembly: `JA` was used after `DEC`.
        // `DEC` doesn't set the Carry Flag, causing `JA` to evaluate the CF from 
        // the preceding `ROR EAX, 8` (which is the high bit of the 4th pixel).
        if (static_cast<int32_t>(pixels) < 0 || _row_dword_counter == 0) {
            _row_dword_counter = _row_dword_count;
            noise_idx = (noise_idx + ecx * ecx) & 0xFE;
        }
    }
}

void __cdecl Jumble_Bitmap_Pixels_(uint8_t* src_buf, uint32_t jumble_mask, uint32_t noise_threshold, uint8_t* dst_buf, uint32_t width, uint32_t height) {
    shear::_noise_threshold = noise_threshold;
    shear::_jumble_mask = jumble_mask;

    shear::_noise_start_index = (random::Random_(1000) - 1) & 0xFFFF;

    uint32_t pixel_count = (width * height) - shear::_jumble_mask - shear::_jumble_mask;

    src_buf += shear::_jumble_mask;
    dst_buf += shear::_jumble_mask;

    uint32_t noise_idx = shear::_noise_start_index;
    uint8_t threshold = static_cast<uint8_t>(shear::_noise_threshold);

    for (uint32_t i = 0; i < pixel_count; ++i) {
        uint8_t src_pixel = *src_buf++;
        uint32_t noise_val = shear::_noise_table[noise_idx];

        if (noise_val < threshold) {
            noise_val &= shear::_jumble_mask;
            noise_val = (noise_val + 1) & 0xFF;

            int32_t offset = static_cast<int32_t>(noise_val);
            if (shear::_jumble_direction_toggle != 0) {
                offset = -offset;
            }

            uint8_t adj_pixel = src_buf[offset - 1];

            *dst_buf = adj_pixel;
            dst_buf[offset] = src_pixel;

            shear::_jumble_direction_toggle = ~shear::_jumble_direction_toggle;
        }

        noise_idx++;
        noise_idx &= 0xFF;
        dst_buf++;
    }
}

void __cdecl Gray_Scale_Fill_Pixels_(
        uint8_t* pixels,
        uint8_t* gray_scale_table,
        int32_t y_step_8_8,
        uint32_t pixels_per_row,
        uint32_t row_count) {
    uint8_t* src = pixels;
    uint8_t* dst = pixels;

    shear::_y_step_8_8 = y_step_8_8;
    bitmap::_bitmap_temp1 = gray_scale_table;
    bitmap::_bitmap_temp2 = pixels_per_row;

    gray_scale_table = bitmap::_bitmap_temp1;

    do {
        uint32_t remaining = bitmap::_bitmap_temp2;

        do {
            const uint32_t index = *src++;
            *dst++ = gray_scale_table[index];
        } while (--remaining != 0);
    } while (--row_count != 0);
}

} // namespace shear