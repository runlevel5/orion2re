#pragma once

#include <cstdint>
#include <cstdio>

namespace farload {
#pragma pack(push, 1)
    struct s_lbx_header {
        uint16_t entry_count;
        uint16_t magic;
        uint32_t unknown4;
        uint32_t offsets[510];
    };
#pragma pack(pop)

    // Suggested name: farload::Error_Handler
    inline void __cdecl Error_Handler_(char* lib_name, int code, int idx) {
        using Fn = void(__cdecl*)(char*, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0051cf00 });
        fn(lib_name, code, idx);
    }

    inline uint32_t& _farload_file_flag = *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055AE34 });
    inline char* _farload_file_name = reinterpret_cast<char*>(std::uintptr_t{ 0x005723B8 });
    inline FILE*& _farload_fptr = *reinterpret_cast<FILE**>(std::uintptr_t{ 0x005723C8 });
    inline s_lbx_header& _lbx_header = *reinterpret_cast<s_lbx_header*>(std::uintptr_t{ 0x005723D0 });
    inline char* _secondary_drive_path = reinterpret_cast<char*>(std::uintptr_t{ 0x0055AE00 });

    void* __cdecl Far_Reload_(char* path, int32_t idx, void* param3);

    void* __cdecl Farload_Library_(char* lib_name, int idx, void* param_3, int alloc_type);
}