#pragma once

#include <cstdint>
#include <cstdio>

namespace farload {
    // Suggested name: farload::Error_Handler
    void __cdecl Error_Handler_(const char* lib_name, int code, int idx);

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

    void* __cdecl Far_Reload_(const char* path, int32_t idx, void* param3);

    inline void* __cdecl Far_Reload_Next_(const char* filename, int entry, void* buffer) {
        using Fn = void*(__cdecl*)(const char*, int, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0051C730 });
        return fn(filename, entry, buffer);
    }

    inline void* __cdecl Farload_Library_Data_(const char* path, int32_t idx, void* out, int32_t mode, uint32_t size, uint16_t param_5, uint16_t param_6) {
        using Fn = void*(__cdecl*)(const char*, int32_t, void*, int32_t, uint32_t, uint16_t, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051ca30});
        return fn(path, idx, out, mode, size, param_5, param_6);
    }

    void* __cdecl Far_Reload_Data_(const char* path, int32_t idx, void* out, uint32_t size, uint16_t param_5, uint16_t param_6);

    void* __cdecl Far_Reload_Next_Data_(const char* path, int32_t idx, void* out, uint32_t size, uint16_t param_5, uint16_t param_6);

    void* __cdecl Farload_Library_(const char* lib_name, int idx, void* param_3, int alloc_type);
}