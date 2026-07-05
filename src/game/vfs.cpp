#include "pch.h"

#include "vfs.h"

#include "vfs_detail.h"

namespace vfs_detail {
    s_vfs_state g_vfs_state{};
}

namespace vfs {
    int8_t Init(const char* base_path) {
        char clean_base_path[vfs_detail::VFS_MAX_PATH];

        Shutdown();

        if (!vfs_detail::Build_Base_Path_(base_path, clean_base_path, sizeof(clean_base_path))) {
            return 0;
        }

        strlcpy(vfs_detail::g_vfs_state.base_path, clean_base_path, sizeof(vfs_detail::g_vfs_state.base_path));
        if (!vfs_detail::Scan_Zip_Archives_()) {
            Shutdown();
            return 0;
        }

        vfs_detail::g_vfs_state.initialized = true;
        return 1;
    }

    void Shutdown() {
        vfs_detail::Close_Zip_Archives_();
        vfs_detail::g_vfs_state.initialized = false;
        vfs_detail::g_vfs_state.base_path[0] = '\0';
        vfs_detail::g_vfs_state.fallback_path[0] = '\0';
    }

    int8_t Set_Raw_Fallback_Path(const char* path) {
        char clean_fallback_path[vfs_detail::VFS_MAX_PATH];

        if (!vfs_detail::Build_Base_Path_(path, clean_fallback_path, sizeof(clean_fallback_path))) {
            return 0;
        }

        strlcpy(vfs_detail::g_vfs_state.fallback_path, clean_fallback_path, sizeof(vfs_detail::g_vfs_state.fallback_path));
        return 1;
    }

    s_vfs_file* fopen(const char* path, const char* mode) {
        char asset_path[vfs_detail::VFS_MAX_PATH];
        char resolved_path[vfs_detail::VFS_MAX_PATH];
        vfs_detail::s_vfs_zip_entry zip_entry{};

        if (!vfs_detail::Is_Read_Only_Mode_(mode) || !vfs_detail::g_vfs_state.initialized) {
            return nullptr;
        }

        if (!vfs_detail::Normalize_Asset_Path_(path, asset_path, sizeof(asset_path))) {
            return nullptr;
        }

        if (vfs_detail::Resolve_Override_Asset_Path_(asset_path, resolved_path, sizeof(resolved_path))) {
            return vfs_detail::Open_Raw_File_(resolved_path, mode);
        }

        if (vfs_detail::Find_Zip_Entry_(asset_path, &zip_entry)) {
            return vfs_detail::Open_Zip_File_(zip_entry);
        }

        if (vfs_detail::Resolve_Raw_Asset_Path_(asset_path, resolved_path, sizeof(resolved_path))) {
            return vfs_detail::Open_Raw_File_(resolved_path, mode);
        }

        return nullptr;
    }

    size_t fread(void* ptr, size_t size, size_t count, s_vfs_file* file) {
        if (file == nullptr || file->ops == nullptr || file->ops->read == nullptr) {
            return 0;
        }

        return file->ops->read(ptr, size, count, file);
    }

    int32_t fsize(s_vfs_file* file) {
        if (file == nullptr || file->ops == nullptr || file->ops->size == nullptr) {
            return -1;
        }

        return file->ops->size(file);
    }

    int32_t fseek(s_vfs_file* file, int32_t offset, int32_t origin) {
        if (file == nullptr || file->ops == nullptr || file->ops->seek == nullptr) {
            return -1;
        }

        return file->ops->seek(file, offset, origin);
    }

    int32_t ftell(s_vfs_file* file) {
        if (file == nullptr || file->ops == nullptr || file->ops->tell == nullptr) {
            return -1;
        }

        return file->ops->tell(file);
    }

    int32_t fclose(s_vfs_file* file) {
        if (file == nullptr || file->ops == nullptr || file->ops->close == nullptr) {
            return -1;
        }

        return file->ops->close(file);
    }

    int8_t Resolve_Read_Path_No_Zip(const char* path, char* resolved_path, size_t resolved_path_size) {
        return vfs_detail::Resolve_Read_Path_No_Zip_(path, resolved_path, resolved_path_size) ? 1 : 0;
    }

    int8_t File_Exists(const char* path) {
        char asset_path[vfs_detail::VFS_MAX_PATH];
        char resolved_path[vfs_detail::VFS_MAX_PATH];

        if (!vfs_detail::g_vfs_state.initialized || !vfs_detail::Normalize_Asset_Path_(path, asset_path, sizeof(asset_path))) {
            return 0;
        }

        if (vfs_detail::Resolve_Override_Asset_Path_(asset_path, resolved_path, sizeof(resolved_path))) {
            return 1;
        }

        if (vfs_detail::Find_Zip_Entry_(asset_path, nullptr)) {
            return 1;
        }

        return vfs_detail::Resolve_Raw_Asset_Path_(asset_path, resolved_path, sizeof(resolved_path)) ? 1 : 0;
    }
}