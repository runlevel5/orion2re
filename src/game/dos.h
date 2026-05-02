#pragma once
#include <cstdint>
#include <ctime>

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

    inline struct tm*& _tmbuf =
            *reinterpret_cast<struct tm**>(std::uintptr_t{0x0057B5D0});

    inline struct file_info_1& _fileinfo =
            *reinterpret_cast<struct file_info_1*>(std::uintptr_t{0x0057B5D8});

    inline time_t& _fileinfo_fi1_username_plus_2 =
            *reinterpret_cast<time_t*>(std::uintptr_t{0x0057B5E4});

    // Suggested name: _fileinfo_name
    inline char& DAT_0057b5ec =
        *reinterpret_cast<char*>(std::uintptr_t{0x0057B5EC});

    inline intptr_t __cdecl __findnext(intptr_t handle, void* data) {
        using Fn = intptr_t(__cdecl*)(intptr_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00541DC0});
        return fn(handle, data);
    }

    uint32_t __cdecl Disk_Free_(void);

    bool __cdecl DIR_(const char* pattern, char* output_name);

    void __cdecl Delete_File_(LPCSTR path);

    struct tm* __cdecl Time_Stamp_(LPCSTR pathname);
}