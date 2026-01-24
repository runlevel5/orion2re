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

    inline void* __cdecl Far_Reload_Data_(const char* filename, int32_t arg2, void* buffer, int32_t arg3, int32_t count, int32_t size) {
        using Fn = void* (__cdecl*)(const char*, int32_t, void*, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0051C750 });
        return fn(filename, arg2, buffer, arg3, count, size);
    }

    inline void* __cdecl Far_Reload_Next_Data_(const char* filename, int type, void* buffer, int record, int count, int size) {
        using Fn = void* (__cdecl*)(const char*, int, void*, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0051C780 });
        return fn(filename, type, buffer, record, count, size);
    }

    void* __cdecl Farload_Library_(char* lib_name, int idx, void* param_3, int alloc_type);
}