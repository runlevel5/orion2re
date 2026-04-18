#pragma once

#include <cstdint>
#include <cstdio>

struct s_lbx_header;

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

    s_animation_header* __cdecl Far_Reload_(const char* path, int32_t idx, s_animation_header* param3);

    // TYPE MISMATCH: originally returns void*
    inline void* __cdecl Farload_Data_Static_(const char* filename, int32_t arg2, void* dest, int32_t arg4, int32_t arg5, int32_t arg6) {
        using Fn = void*(__cdecl*)(const char*, int32_t, void*, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051C7B0});
        return fn(filename, arg2, dest, arg4, arg5, arg6);
    }

    inline void* __cdecl Far_Reload_Next_(const char* filename, int entry, void* buffer) {
        using Fn = void*(__cdecl*)(const char*, int, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0051C730 });
        return fn(filename, entry, buffer);
    }

    void* __cdecl Farload_Library_Data_(const char* fname, int32_t idx, void* out, int16_t alloc_type, uint32_t start_idx, uint16_t count, uint16_t expected_element_size);

    void* __cdecl Far_Reload_Data_(const char* path, int32_t idx, void* out, uint32_t size, uint16_t param_5, uint16_t param_6);

    void* __cdecl Far_Reload_Next_Data_(const char* filename, int32_t idx, void* buffer, uint32_t entry_num, uint16_t type, uint16_t size);

    void* __cdecl Farload_Library_(const char* lib_name, int idx, void* param_3, int alloc_type);

    inline uint16_t __cdecl Get_Farload_Data_Record_Count_(const char* lbx_name, int type) {
        using Fn = uint16_t(__cdecl*)(const char*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0051cd30});
        return fn(lbx_name, type);
    }
}