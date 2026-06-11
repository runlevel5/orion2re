#include "pch.h"

namespace {
    const char* Get_File_Name_(const char* path) {
        const char* slash = strrchr(path, '/');
        const char* backslash = strrchr(path, '\\');
        const char* file_name = path;

        if (slash != nullptr && slash + 1 > file_name) {
            file_name = slash + 1;
        }

        if (backslash != nullptr && backslash + 1 > file_name) {
            file_name = backslash + 1;
        }

        return file_name;
    }

    bool Build_Uppercase_File_Name_(const char* path, char* upper_path, size_t upper_path_size) {
        if (SDL_strlcpy(upper_path, path, upper_path_size) >= upper_path_size) {
            return false;
        }

        char* file_name = const_cast<char*>(Get_File_Name_(upper_path));
        while (*file_name != '\0') {
            *file_name = (char)SDL_toupper((unsigned char)*file_name);
            ++file_name;
        }

        return true;
    }

    bool Mode_Creates_File_(const char* mode) {
        return mode[0] == 'w' || mode[0] == 'a';
    }
}

bool File_Exists_(const char* path) {
    SDL_PathInfo path_info;

    if (path == nullptr || path[0] == '\0') {
        return false;
    }

    return SDL_GetPathInfo(path, &path_info) && path_info.type == SDL_PATHTYPE_FILE;
}

FILE* fopen_case(const char* path, const char* mode) {
    char upper_path[512];
    char cwd[512];
    char* cwd_alloc;
    bool has_upper_path;
    int original_errno;

    if (path == nullptr || mode == nullptr) {
        errno = EINVAL;
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "fopen_case invalid args: path=%p mode=%p", (const void*)path, (const void*)mode);
        return nullptr;
    }

    has_upper_path = Build_Uppercase_File_Name_(path, upper_path, sizeof(upper_path)) &&
        strcmp(path, upper_path) != 0;

    cwd_alloc = SDL_GetCurrentDirectory();
    if (cwd_alloc != nullptr) {
        SDL_strlcpy(cwd, cwd_alloc, sizeof(cwd));
        SDL_free(cwd_alloc);
    } else {
        SDL_strlcpy(cwd, "<unavailable>", sizeof(cwd));
    }

    if (Mode_Creates_File_(mode)) {
        if (File_Exists_(path)) {
            return fopen(path, mode);
        }

        if (has_upper_path && File_Exists_(upper_path)) {
            return fopen(upper_path, mode);
        }

        return fopen(path, mode);
    }

    FILE* file = fopen(path, mode);
    if (file != nullptr) {
        return file;
    }

    original_errno = errno;

    if (has_upper_path) {
        file = fopen(upper_path, mode);
        if (file != nullptr) {
            SDL_LogInfo(
                SDL_LOG_CATEGORY_APPLICATION,
                "fopen_case fallback success: requested='%s' opened='%s' mode='%s' cwd='%s'",
                path,
                upper_path,
                mode,
                cwd);
            return file;
        }
    }

    SDL_LogWarn(
        SDL_LOG_CATEGORY_APPLICATION,
        "fopen_case failed: requested='%s' uppercase='%s' mode='%s' cwd='%s' exists=%d upper_exists=%d errno=%d (%s)",
        path,
        has_upper_path ? upper_path : "<none>",
        mode,
        cwd,
        File_Exists_(path) ? 1 : 0,
        (has_upper_path && File_Exists_(upper_path)) ? 1 : 0,
        original_errno,
        strerror(original_errno));

    return nullptr;
}

void Query_Memory_Status_(MEMORYSTATUS* memory_status) {
    uint64_t total_phys;
    uint32_t clamped_total_phys;

    if (memory_status == nullptr) {
        return;
    }

    SDL_zero(*memory_status);
    memory_status->dwLength = sizeof(*memory_status);

    total_phys = (uint64_t)SDL_GetSystemRAM() * 1024ull * 1024ull;
    if (total_phys > UINT32_MAX) {
        clamped_total_phys = UINT32_MAX;
    } else {
        clamped_total_phys = (uint32_t)total_phys;
    }

    memory_status->dwTotalPhys = clamped_total_phys;
    memory_status->dwAvailPhys = clamped_total_phys;
    memory_status->dwTotalPageFile = clamped_total_phys;
    memory_status->dwAvailPageFile = clamped_total_phys;
    memory_status->dwTotalVirtual = clamped_total_phys;
    memory_status->dwAvailVirtual = clamped_total_phys;
}

int Show_Message_Box_(void* window_handle, const char* text, const char* caption, bool is_error) {
    return SDL_ShowSimpleMessageBox(
        is_error ? SDL_MESSAGEBOX_ERROR : SDL_MESSAGEBOX_INFORMATION,
        caption != nullptr ? caption : "",
        text != nullptr ? text : "",
        (SDL_Window*)window_handle);
}