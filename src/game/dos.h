#pragma once

namespace dos {
    extern struct tm* _tmbuf;

    extern struct _finddata32_t _fileinfo;

    int32_t __cdecl Disk_Free_(void);

    bool __cdecl DIR_(const char* pattern, char* output_name);

    void __cdecl Delete_File_(LPCSTR path);

    struct tm* __cdecl Time_Stamp_(LPCSTR pathname);
}