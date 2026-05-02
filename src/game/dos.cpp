#include "pch.h"

namespace dos {
    /**
     * Finds files matching a pattern or continues a previous search.
     * @param pattern The search pattern (e.g., "*.txt"). If empty, continues search with findnext.
     * @param output_name Buffer to receive the filename of the found file.
     * @return true if a file was found, false otherwise.
     */
    bool __cdecl DIR_(const char* pattern, char* output_name) {
        intptr_t result;
        intptr_t search_handle = NULL; // local_4: Allocated but uninitialized in original assembly

        if (pattern[0] == '\0') {
            // Note: search_handle is used uninitialized here in the original binary logic.
            result = __findnext(search_handle, &_fileinfo);
        } else {
            result = __findfirst(pattern, &_fileinfo);
        }

        // Check if findfirst or findnext failed (returns -1)
        if (result == -1) {
            if (output_name) {
                output_name[0] = '\0';
            }
            return false;
        }

        // Copy the found filename from the CRT fileinfo buffer to output_name.
        // The assembly performs an optimized string copy (strlen + memcpy).
        char* src = &DAT_0057b5ec;
        char* dst = output_name;
        while ((*dst++ = *src++) != '\0');
        *dst = '\0';

        return true;
    }

    void __cdecl Delete_File_(LPCSTR path) {
        remove(path);
    }

    uint32_t __cdecl Disk_Free_(void) {
        _diskfree_t disk_free_info;

        if (__getdiskfree(0, &disk_free_info) != 0) {
            return 0;
        }

        return disk_free_info.avail_clusters *
               disk_free_info.sectors_per_cluster *
               disk_free_info.bytes_per_sector;
    }

    struct tm* __cdecl Time_Stamp_(LPCSTR pathname) {
        intptr_t search_handle;

        search_handle = __findfirst(pathname, &_fileinfo);
        if (search_handle == -1) {
            return nullptr;
        }

        _ctime(&_fileinfo_fi1_username_plus_2);
        _tmbuf = _localtime(&_fileinfo_fi1_username_plus_2);
        return _tmbuf;
    }
}