#pragma once
#include <cstdint>

namespace dos {
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

    /**
     * Finds files matching a pattern or continues a previous search.
     * @param pattern The search pattern (e.g., "*.txt"). If empty, continues search with findnext.
     * @param output_name Buffer to receive the filename of the found file.
     * @return true if a file was found, false otherwise.
     */
    bool __cdecl DIR_(const char* pattern, char* output_name);
}