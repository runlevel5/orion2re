#pragma once

#include <stddef.h>
#include <stdint.h>

struct s_vfs_file;

namespace vfs {
    int8_t Init(const char* base_path);
    void Shutdown();
    int8_t Set_Raw_Fallback_Path(const char* path);

    s_vfs_file* fopen(const char* path, const char* mode);
    s_vfs_file* fopen_lbx(const char* lbx_name, int32_t entry_idx);

    size_t fread(void* ptr, size_t size, size_t count, s_vfs_file* file);
    int32_t fsize(s_vfs_file* file);
    int32_t fseek(s_vfs_file* file, int32_t offset, int32_t origin);
    int32_t ftell(s_vfs_file* file);
    int32_t fclose(s_vfs_file* file);

    int8_t Resolve_Read_Path_No_Zip(const char* path, char* resolved_path, size_t resolved_path_size);
    int8_t File_Exists(const char* path);
}