#include "pch.h"

namespace dos {
    struct tm* _tmbuf;

    struct _finddata32_t _fileinfo;

    bool __cdecl DIR_(const char* pattern, char* output_name) {
        intptr_t find_result;
        // Binary behavior appears to call _findnext32 without preserving a valid find handle.
        // Keep a safe default here rather than reintroducing an uninitialized-handle use.
        intptr_t find_handle = 0;

        if (pattern[0] == '\0') {
            find_result = _findnext32(find_handle, &_fileinfo);
        } else {
            find_result = _findfirst32(pattern, &_fileinfo);
        }

        if (find_result == -1) {
            output_name[0] = '\0';
            return false;
        }

        strcpy(output_name, _fileinfo.name);

        return true;
    }

    void __cdecl Delete_File_(LPCSTR path) {
        remove(path);
    }

    int32_t __cdecl Disk_Free_(void) {
        _diskfree_t disk_free_info;

        if (_getdiskfree(0, &disk_free_info) != 0) {
            return 0;
        }

        return disk_free_info.avail_clusters *
               disk_free_info.sectors_per_cluster *
               disk_free_info.bytes_per_sector;
    }

    struct tm* __cdecl Time_Stamp_(LPCSTR pathname) {
        intptr_t find_handle;

        find_handle = _findfirst32(pathname, &_fileinfo);
        if (find_handle == -1) {
            return nullptr;
        }

        _ctime32(&_fileinfo.time_write);
        _tmbuf = _localtime32(&_fileinfo.time_write);
        return _tmbuf;
    }
}