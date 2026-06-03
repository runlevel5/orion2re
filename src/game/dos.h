#pragma once

namespace dos {
    extern struct tm* _tmbuf;

    extern struct _finddata32_t _fileinfo;

    /**
     * Returns the available free space on the current drive in bytes.
     * @return Available free space, or 0 if the query fails.
     */
    int32_t __cdecl Disk_Free_(void);

    /**
     * Finds the first matching file or continues a prior directory search.
     * @param pattern Search pattern, or an empty string to continue the prior search.
     * @param output_name Receives the matched filename.
     * @return true if a matching filename was written to output_name, otherwise false.
     */
    bool __cdecl DIR_(const char* pattern, char* output_name);

    /**
     * Deletes the file at the specified path.
     * @param path Path to the file to remove.
     */
    void __cdecl Delete_File_(LPCSTR path);

    /**
     * Retrieves a file's last-write timestamp in local time.
     * @param pathname Path to the file to inspect.
     * @return Pointer to the cached local-time structure, or nullptr if the file is not found.
     */
    struct tm* __cdecl Time_Stamp_(LPCSTR pathname);
}