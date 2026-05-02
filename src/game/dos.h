#pragma once
#include <cstdint>

namespace dos {
    struct _diskfree_t {
        uint32_t total_clusters;
        uint32_t avail_clusters;
        uint32_t sectors_per_cluster;
        uint32_t bytes_per_sector;
    };

    inline uint32_t __cdecl __getdiskfree(uint32_t drive, _diskfree_t* disk_free_info) {
        using Fn = uint32_t(__cdecl*)(uint32_t, _diskfree_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00542680});
        return fn(drive, disk_free_info);
    }

    // Suggested name: _fileinfo
    inline uint8_t& _fileinfo =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0057B5D8});

    // Suggested name: _fileinfo_name
    inline char& DAT_0057b5ec =
        *reinterpret_cast<char*>(std::uintptr_t{0x0057B5EC});

    inline intptr_t __cdecl __findfirst(const char* pattern, void* data) {
        using Fn = intptr_t(__cdecl*)(const char*, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00541C70});
        return fn(pattern, data);
    }

    inline intptr_t __cdecl __findnext(intptr_t handle, void* data) {
        using Fn = intptr_t(__cdecl*)(intptr_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00541DC0});
        return fn(handle, data);
    }

    uint32_t __cdecl Disk_Free_(void);

    bool __cdecl DIR_(const char* pattern, char* output_name);

    void __cdecl Delete_File_(LPCSTR path);

    inline struct tm* __cdecl Time_Stamp_(const char* input) {
        using Fn = struct tm* (__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052D3D0});
        return fn(input);
    }
}