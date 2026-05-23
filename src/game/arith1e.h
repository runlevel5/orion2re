#pragma once
#include <cstdint>

namespace arith1e {
    inline const char* s_Failure_allocating_context__d_00560838 =
        reinterpret_cast<const char*>(std::uintptr_t{0x00560838}); // "Failure allocating context %d"
    inline const char* s_Fatal_error__00560ab0 =
        reinterpret_cast<const char*>(std::uintptr_t{0x00560AB0}); // "Fatal error: "

    void __cdecl Compress_Data_(uint8_t* in_data, uint32_t in_len, uint8_t* out_data, uint32_t* out_len);

    void __cdecl Uncompress_Data_(uint8_t* src, uint32_t src_len, uint8_t* dst, uint32_t* dst_len_ptr);
}