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
    void __cdecl Error_Handler_(char* lib_name, int code, int idx);

    inline char* s_vga_frames_error = reinterpret_cast<char*>(std::uintptr_t{0x0055AE38});
    inline char* s_error_defined_records = reinterpret_cast<char*>(std::uintptr_t{0x0055AE70});
    inline char* s_error_record_size = reinterpret_cast<char*>(std::uintptr_t{0x0055AE94});
    inline char* s_error_lbx_entries = reinterpret_cast<char*>(std::uintptr_t{0x0055AEB4});
    inline char* s_error_not_lbx = reinterpret_cast<char*>(std::uintptr_t{0x0055AED4});
    inline char* s_error_reload_failed = reinterpret_cast<char*>(std::uintptr_t{0x0055AEE8});
    inline char* s_error_improper_entry = reinterpret_cast<char*>(std::uintptr_t{0x0055AF10});
    inline char* s_error_insufficient_memory = reinterpret_cast<char*>(std::uintptr_t{0x0055AF30});
    inline char* s_error_corrupted = reinterpret_cast<char*>(std::uintptr_t{0x0055AF7C});
    inline char* s_error_not_found = reinterpret_cast<char*>(std::uintptr_t{0x0055AF94});
    inline char* s_entry_suffix = reinterpret_cast<char*>(std::uintptr_t{0x0055AFAC});
    inline char* s_entry_prefix = reinterpret_cast<char*>(std::uintptr_t{0x0055AFB0});

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