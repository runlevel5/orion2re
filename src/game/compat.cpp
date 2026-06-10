#include "pch.h"

bool File_Exists_(const char* path) {
    SDL_PathInfo path_info;

    if (path == nullptr || path[0] == '\0') {
        return false;
    }

    return SDL_GetPathInfo(path, &path_info) && path_info.type == SDL_PATHTYPE_FILE;
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