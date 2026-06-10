#include "pch.h"

namespace dos {
    namespace {
        enum {
            MAX_DOS_PATH = 512,
            NANOSECONDS_PER_SECOND = 1000000000,
        };

        const char* Get_File_Name_(const char* path) {
            const char* slash;
            const char* backslash;

            slash = strrchr(path, '/');
            backslash = strrchr(path, '\\');

            if (slash == nullptr) {
                slash = path;
            } else {
                ++slash;
            }

            if (backslash == nullptr) {
                return slash;
            }

            ++backslash;
            if (backslash > slash) {
                return backslash;
            }

            return slash;
        }

        void Split_Search_Pattern_(const char* pattern, char* directory, char* filename_pattern) {
            const char* file_name;
            size_t directory_length;

            file_name = Get_File_Name_(pattern);
            if (file_name == pattern) {
                strcpy(directory, ".");
                strcpy(filename_pattern, pattern);
                return;
            }

            directory_length = (size_t)(file_name - pattern);
            if (directory_length >= MAX_DOS_PATH) {
                directory_length = MAX_DOS_PATH - 1;
            }

            memcpy(directory, pattern, directory_length);
            directory[directory_length] = '\0';

            if (directory_length > 0 &&
                (directory[directory_length - 1] == '/' || directory[directory_length - 1] == '\\')) {
                directory[directory_length - 1] = '\0';
            }

            if (directory[0] == '\0') {
                strcpy(directory, ".");
            }

            strcpy(filename_pattern, file_name);
        }
    }

    bool __cdecl DIR_(const char* pattern, char* output_name) {
        char directory[MAX_DOS_PATH];
        char filename_pattern[MAX_DOS_PATH];
        char** matches;
        int count;

        if (pattern == nullptr || output_name == nullptr || pattern[0] == '\0') {
            output_name[0] = '\0';
            return false;
        }

        Split_Search_Pattern_(pattern, directory, filename_pattern);

        matches = SDL_GlobDirectory(directory, filename_pattern, SDL_GLOB_CASEINSENSITIVE, &count);
        if (matches == nullptr || count <= 0) {
            if (matches != nullptr) {
                SDL_free(matches);
            }
            output_name[0] = '\0';
            return false;
        }

        strcpy(output_name, Get_File_Name_(matches[0]));
        SDL_free(matches);
        return true;
    }

    void __cdecl Delete_File_(const char* path) {
        remove(path);
    }

    int32_t __cdecl Disk_Free_(void) {
        uint64_t available_bytes;
        SDL_Storage* storage;

        storage = SDL_OpenFileStorage(nullptr);
        if (storage == nullptr) {
            return 0;
        }

        if (!SDL_StorageReady(storage)) {
            SDL_CloseStorage(storage);
            return 0;
        }

        available_bytes = (uint64_t)SDL_GetStorageSpaceRemaining(storage);
        SDL_CloseStorage(storage);

        if (available_bytes > INT32_MAX) {
            return INT32_MAX;
        }

        return (int32_t)available_bytes;
    }

    struct tm* __cdecl Time_Stamp_(const char* pathname) {
        SDL_PathInfo path_info;
        time_t modified_time;

        if (pathname == nullptr) {
            return nullptr;
        }

        if (!SDL_GetPathInfo(pathname, &path_info) || path_info.type != SDL_PATHTYPE_FILE) {
            return nullptr;
        }

        modified_time = (time_t)(path_info.modify_time / NANOSECONDS_PER_SECOND);
        return localtime(&modified_time);
    }
}