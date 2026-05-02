#pragma once

namespace dos {
    inline struct tm*& _tmbuf =
            *reinterpret_cast<struct tm**>(std::uintptr_t{0x0057B5D0});

    inline struct _finddata_t& _fileinfo =
            *reinterpret_cast<struct _finddata_t*>(std::uintptr_t{0x0057B5D8});

    inline time_t& _fileinfo_fi1_username_plus_2 =
            *reinterpret_cast<time_t*>(std::uintptr_t{0x0057B5E4});

    // Suggested name: _fileinfo_name
    inline char& DAT_0057b5ec =
        *reinterpret_cast<char*>(std::uintptr_t{0x0057B5EC});

    uint32_t __cdecl Disk_Free_(void);

    bool __cdecl DIR_(const char* pattern, char* output_name);

    void __cdecl Delete_File_(LPCSTR path);

    struct tm* __cdecl Time_Stamp_(LPCSTR pathname);
}