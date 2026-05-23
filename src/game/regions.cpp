#include "pch.h"

namespace regions {
    void __cdecl Fill_Region_(uint8_t color, uint8_t* surface, uint32_t width_blocks, uint32_t height) {
        const uint32_t width_bytes = width_blocks << 2;
        const int32_t stride_skip = platform::g_screen_res_x - (int32_t)width_bytes;
        uint32_t remaining = height;

        do {
            memset(surface, color, width_bytes);
            surface += width_bytes;
            surface += stride_skip;
            --remaining;
        } while (remaining != 0);
    }

    void __cdecl Fill_Column_(uint8_t color, uint8_t* surface, uint32_t fill_width, uint32_t fill_height) {
        int32_t stride_skip = platform::g_screen_res_x - (int32_t)fill_width;
        uint32_t remaining = fill_height;

        do {
            memset(surface, color, fill_width);
            surface += fill_width;
            surface += stride_skip;
            --remaining;
        } while (remaining != 0);
    }

    void __cdecl Tint_Fill_Region_(uint8_t* tint_table, uint8_t* pixels, uint32_t width_dwords, int32_t row_count) {
        uint8_t* dst = pixels;
        uint8_t* src = pixels;

        _rows_remaining = row_count;

        uint32_t width_bytes = width_dwords << 2;
        int32_t row_skip = platform::g_screen_res_x - (int32_t)width_bytes;

        _saved_width = width_bytes >> 2;

        do {
            uint32_t remaining_dwords = _saved_width;

            do {
                *dst++ = tint_table[*src++];
                *dst++ = tint_table[*src++];
                *dst++ = tint_table[*src++];
                *dst++ = tint_table[*src++];
            } while (--remaining_dwords != 0);

            dst += row_skip;
            src += row_skip;
        } while (--_rows_remaining != 0);
    }

    void __cdecl Tint_Fill_Column_(uint8_t* tint_table, uint8_t* pixels, uint32_t width, uint32_t height) {
        uint8_t* dst = pixels;
        uint8_t* src = pixels;
        const int32_t row_skip = platform::g_screen_res_x - static_cast<int32_t>(width);

        _rows_remaining = height;
        _saved_width = width;

        do {
            uint32_t remaining = _saved_width;

            do {
                *dst++ = tint_table[*src++];
                --remaining;
            } while (remaining != 0);

            dst += row_skip;
            src += row_skip;
            --_rows_remaining;
        } while (_rows_remaining != 0);
    }

    void __cdecl Interlaced_Fill_Column_(uint32_t interlace_pattern, uint8_t* dst, int32_t span, int32_t row_count) {
        const uint32_t row_padding = static_cast<uint32_t>(platform::g_screen_res_x) - static_cast<uint32_t>(span);

        do {
            memset(dst, static_cast<int>(static_cast<uint8_t>(interlace_pattern)), static_cast<size_t>(span));
            dst += span;
            dst += row_padding;

            interlace_pattern =
                (interlace_pattern & 0xFFFF0000u) |
                ((interlace_pattern & 0x000000FFu) << 8) |
                ((interlace_pattern & 0x0000FF00u) >> 8);

            --row_count;
        } while (row_count != 0);
    }

    void __cdecl Interlaced_Fill_Region_(uint16_t pattern, uint8_t* dst, int32_t width_dwords, int32_t height)
    {
        uint32_t current_pattern = static_cast<uint32_t>(pattern);
        _interlaced_fill_next_pattern = current_pattern;

        {
            uint8_t fill_byte = static_cast<uint8_t>(current_pattern);
            uint32_t replicated_pattern = static_cast<uint32_t>(fill_byte);
            replicated_pattern |= replicated_pattern << 8;
            replicated_pattern |= replicated_pattern << 16;

            uint32_t previous_pattern = _interlaced_fill_next_pattern;
            _interlaced_fill_next_pattern = replicated_pattern;
            current_pattern = previous_pattern;
        }

        {
            uint8_t fill_byte = static_cast<uint8_t>(current_pattern >> 8);
            uint32_t replicated_pattern = static_cast<uint32_t>(fill_byte);
            replicated_pattern |= replicated_pattern << 8;
            replicated_pattern |= replicated_pattern << 16;

            uint32_t previous_pattern = _interlaced_fill_next_pattern;
            _interlaced_fill_next_pattern = replicated_pattern;
            current_pattern = previous_pattern;
        }

        uint32_t shifted_width_bytes = static_cast<uint32_t>(width_dwords);
        shifted_width_bytes <<= 2;
        uint32_t row_advance = platform::g_screen_res_x - shifted_width_bytes;
        uint32_t fill_count = shifted_width_bytes >> 2;
        uint32_t remaining_rows = static_cast<uint32_t>(height);

        do {
            uint32_t* fill_ptr = reinterpret_cast<uint32_t*>(dst);

            for (uint32_t i = 0; i < fill_count; ++i) {
                *fill_ptr++ = current_pattern;
            }

            dst = reinterpret_cast<uint8_t*>(reinterpret_cast<std::uintptr_t>(fill_ptr) + row_advance);

            uint32_t previous_pattern = _interlaced_fill_next_pattern;
            _interlaced_fill_next_pattern = current_pattern;
            current_pattern = previous_pattern;

            --remaining_rows;
        } while (remaining_rows != 0);
    }

    void __cdecl Tri_Interlaced_Fill_Column_(uint32_t color, uint8_t* dest, int32_t width, int32_t height) {
        const int32_t row_advance = platform::g_screen_res_x - width;

        do {
            memset(dest, (uint8_t)color, width);
            dest += width;
            dest += row_advance;
            color = (color >> 8) | (color << 24);
            if (--height == 0) {
                return;
            }

            memset(dest, (uint8_t)color, width);
            dest += width;
            dest += row_advance;
            color = (color >> 8) | (color << 24);
            if (--height == 0) {
                return;
            }

            memset(dest, (uint8_t)color, width);
            dest += width;
            dest += row_advance;
            color = (color >> 16) | (color << 16);
        } while (--height != 0);
    }

    void __cdecl Tri_Interlaced_Fill_Region_(
        uint32_t next_pattern,
        uint8_t* dst,
        uint32_t width,
        uint32_t height)
    {
        _interlaced_fill_next_pattern = next_pattern;

        const uint32_t fill_width = width & 0x3FFFFFFFu;
        const uint32_t fill_width_bytes = fill_width * 4;
        const uint32_t row_advance = platform::g_screen_res_x - fill_width_bytes;

        while (true) {
            memset(dst, static_cast<int>(static_cast<uint8_t>(_interlaced_fill_next_pattern)),
                   static_cast<size_t>(fill_width_bytes));
            dst += fill_width_bytes;
            dst += row_advance;
            _interlaced_fill_next_pattern =
                (_interlaced_fill_next_pattern >> 8) |
                (_interlaced_fill_next_pattern << 24);

            if (--height == 0) {
                break;
            }

            memset(dst, static_cast<int>(static_cast<uint8_t>(_interlaced_fill_next_pattern)),
                   static_cast<size_t>(fill_width_bytes));
            dst += fill_width_bytes;
            dst += row_advance;
            _interlaced_fill_next_pattern =
                (_interlaced_fill_next_pattern >> 8) |
                (_interlaced_fill_next_pattern << 24);

            if (--height == 0) {
                break;
            }

            memset(dst, static_cast<int>(static_cast<uint8_t>(_interlaced_fill_next_pattern)),
                   static_cast<size_t>(fill_width_bytes));
            dst += fill_width_bytes;
            dst += row_advance;
            _interlaced_fill_next_pattern =
                (_interlaced_fill_next_pattern >> 16) |
                (_interlaced_fill_next_pattern << 16);

            if (--height != 0) {
                continue;
            }

            break;
        }
    }
}