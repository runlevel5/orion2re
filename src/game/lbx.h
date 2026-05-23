#pragma once
#include <cstdint>

struct s_lbx_stream_header;
struct s_wave_stream_header;

namespace lbx {
    inline const char* s_r_b_0055bf20 = reinterpret_cast<const char*>(std::uintptr_t{0x0055BF20}); /* "r+b" */

    void __cdecl LBX_init_(s_lbx_stream_header* header, const char* filename);

    uint32_t __cdecl LBX_getRecordLength_(s_lbx_stream_header* header, int32_t rec_idx);

    void __cdecl LBX_uninit_(s_lbx_stream_header* header);

    void __cdecl LBX_init_stream_(s_wave_stream_header* stream, const char* filename);

    void __cdecl LBX_readRecord_(s_lbx_stream_header* header, int32_t record_idx, void* buffer);
}