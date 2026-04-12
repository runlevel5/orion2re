#pragma once
#include <cstdint>

namespace arith1e {
    void __cdecl Compress_Data_(uint8_t* in_data, uint32_t in_len, uint8_t* out_data, uint32_t* out_len);

    void __cdecl Uncompress_Data_(uint8_t* src, uint32_t src_len, uint8_t* dst, uint32_t* dst_len_ptr);
}