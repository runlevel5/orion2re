#pragma once
#include <cstdint>

namespace regions {
    inline uint32_t& _rows_remaining =
            *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C094});

    inline uint32_t& _saved_width =
            *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C098});

    inline uint32_t& _interlaced_fill_next_pattern =
            *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C09C});

    void __cdecl Fill_Column_(uint8_t color, uint8_t* surface, uint32_t fill_width, uint32_t fill_height);
    void __cdecl Fill_Region_(uint8_t color, uint8_t* surface, uint32_t width_blocks, uint32_t height);

    void __cdecl Tint_Fill_Column_(uint8_t* tint_table, uint8_t* pixels, uint32_t width, uint32_t height);

    void __cdecl Tint_Fill_Region_(uint8_t* tint_table, uint8_t* pixels, uint32_t width_dwords, int32_t row_count);

    void __cdecl Interlaced_Fill_Column_(uint32_t interlace_pattern, uint8_t* dst, int32_t span, int32_t row_count);

    void __cdecl Interlaced_Fill_Region_(uint16_t pattern, uint8_t* dst, int32_t width_dwords, int32_t height);

    void __cdecl Tri_Interlaced_Fill_Column_(uint32_t color, uint8_t* dest, int32_t width, int32_t height);

    void __cdecl Tri_Interlaced_Fill_Region_(uint32_t next_pattern, uint8_t* dst, uint32_t width, uint32_t height);
}