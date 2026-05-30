#include "pch.h"

namespace dos {
    struct tm* _tmbuf;

    struct _finddata32_t _fileinfo;

    /**
     * Finds files matching a pattern or continues a previous search.
     * @param pattern The search pattern (e.g., "*.txt"). If empty, continues search with findnext.
     * @param output_name Buffer to receive the filename of the found file.
     * @return true if a file was found, false otherwise.
     */
    bool __cdecl DIR_(const char* pattern, char* output_name) {
        intptr_t result;
        // Binary note: the empty-pattern path passes an uninitialized stack local to _findnext32.
        // Keeping a safe default here avoids reintroducing that undefined behavior directly.
        intptr_t search_handle = 0;

        if (pattern[0] == '\0') {
            result = _findnext32(search_handle, &_fileinfo);
        } else {
            result = _findfirst32(pattern, &_fileinfo);
        }

        if (result == -1) {
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
        intptr_t search_handle;

        search_handle = _findfirst32(pathname, &_fileinfo);
        if (search_handle == -1) {
            return nullptr;
        }

        _ctime32(&_fileinfo.time_write);
        _tmbuf = _localtime32(&_fileinfo.time_write);
        return _tmbuf;
    }
}