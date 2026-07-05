#include "pch.h"

#include "vfs_detail.h"

namespace vfs_detail {
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

    bool Is_Read_Only_Mode_(const char* mode) {
        if (mode == nullptr || mode[0] != 'r') {
            return false;
        }

        for (size_t i = 1; mode[i] != '\0'; ++i) {
            if (mode[i] != 'b' && mode[i] != 't') {
                return false;
            }
        }

        return true;
    }

    bool Build_Base_Path_(const char* base_path, char* output_path, size_t output_path_size) {
        if (output_path == nullptr || output_path_size == 0) {
            return false;
        }

        if (base_path == nullptr || base_path[0] == '\0') {
            output_path[0] = '\0';
            return true;
        }

        return Cleanup_Path_(base_path, output_path, output_path_size, true);
    }

    bool Is_Absolute_Asset_Path_(const char* path) {
        if (path == nullptr || path[0] == '\0') {
            return false;
        }

        if (path[0] == '/' || path[0] == '\\') {
            return true;
        }

        return path[1] == ':';
    }

    bool Normalize_Asset_Path_(const char* path, char* output_path, size_t output_path_size) {
        char clean_path[VFS_MAX_PATH];
        const char* normalized_path;

        if (path == nullptr || output_path == nullptr || output_path_size == 0) {
            return false;
        }

        if (!Cleanup_Path_(path, clean_path, sizeof(clean_path), false)) {
            return false;
        }

        normalized_path = clean_path[0] != '\0' ? clean_path : path;
        if (normalized_path[0] == '\0' || Is_Absolute_Asset_Path_(normalized_path)) {
            return false;
        }

        if (strlcpy(output_path, normalized_path, output_path_size) >= output_path_size) {
            return false;
        }

        for (size_t i = 0; output_path[i] != '\0'; ++i) {
            if (output_path[i] == '\\') {
                output_path[i] = '/';
            }
        }

        return true;
    }

    bool Build_Asset_Key_(const char* asset_path, char* output_key, size_t output_key_size) {
        if (asset_path == nullptr || output_key == nullptr || output_key_size == 0) {
            return false;
        }

        if (strlcpy(output_key, asset_path, output_key_size) >= output_key_size) {
            return false;
        }

        for (size_t i = 0; output_key[i] != '\0'; ++i) {
            output_key[i] = (char)SDL_tolower((unsigned char)output_key[i]);
        }

        return true;
    }

    bool Build_Candidate_Path_(const char* base_path, const char* prefix, const char* asset_path, char* output_path, size_t output_path_size) {
        char combined_path[VFS_MAX_PATH];

        if (snprintf(combined_path, sizeof(combined_path), "%s%s%s", base_path, prefix, asset_path) >= (int)sizeof(combined_path)) {
            return false;
        }

        if (!Cleanup_Path_(combined_path, output_path, output_path_size, false)) {
            return false;
        }

        return true;
    }

    bool Resolve_Candidate_Path_(const char* candidate_path, char* resolved_path, size_t resolved_path_size) {
        if (Resolve_File_Path_Case_(candidate_path, resolved_path, resolved_path_size)) {
            return true;
        }

        if (!::File_Exists_(candidate_path)) {
            return false;
        }

        return strlcpy(resolved_path, candidate_path, resolved_path_size) < resolved_path_size;
    }

    bool Resolve_Override_Asset_Path_(const char* asset_path, char* resolved_path, size_t resolved_path_size) {
        char candidate_path[VFS_MAX_PATH];

        if (Build_Candidate_Path_(g_vfs_state.base_path, VFS_OVERRIDES_PATH, asset_path, candidate_path, sizeof(candidate_path)) &&
            Resolve_Candidate_Path_(candidate_path, resolved_path, resolved_path_size)) {
            return true;
        }

        return false;
    }

    bool Resolve_Raw_Asset_Path_(const char* asset_path, char* resolved_path, size_t resolved_path_size) {
        char candidate_path[VFS_MAX_PATH];

        if (Build_Candidate_Path_(g_vfs_state.base_path, "", asset_path, candidate_path, sizeof(candidate_path)) &&
            Resolve_Candidate_Path_(candidate_path, resolved_path, resolved_path_size)) {
            return true;
        }

        if (g_vfs_state.fallback_path[0] != '\0' &&
            Build_Candidate_Path_(g_vfs_state.fallback_path, "", asset_path, candidate_path, sizeof(candidate_path)) &&
            Resolve_Candidate_Path_(candidate_path, resolved_path, resolved_path_size)) {
            return true;
        }

        return false;
    }

    bool Resolve_Read_Path_No_Zip_(const char* path, char* resolved_path, size_t resolved_path_size) {
        char asset_path[VFS_MAX_PATH];

        if (!g_vfs_state.initialized) {
            return false;
        }

        if (!Normalize_Asset_Path_(path, asset_path, sizeof(asset_path))) {
            return false;
        }

        if (Resolve_Override_Asset_Path_(asset_path, resolved_path, resolved_path_size)) {
            return true;
        }

        return Resolve_Raw_Asset_Path_(asset_path, resolved_path, resolved_path_size);
    }
}