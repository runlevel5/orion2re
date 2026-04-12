#pragma once
#include <cstdint>

namespace arith1e {
    inline void __cdecl Compress_Data_(uint8_t* in_data, uint32_t in_len, uint8_t* out_data, uint32_t* out_len) {
        using Fn = void(__cdecl*)(uint8_t*, uint32_t, uint8_t*, uint32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00538AB0});
        fn(in_data, in_len, out_data, out_len);
    }

    inline void __cdecl Uncompress_Data_(uint8_t* src, uint32_t src_len, uint8_t* dst, uint32_t* dst_len_ptr) {
        using Fn = void(__cdecl*)(uint8_t*, uint32_t, uint8_t*, uint32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00538B80});
        fn(src, src_len, dst, dst_len_ptr);
    }
}