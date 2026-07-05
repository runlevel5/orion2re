#include "pch.h"

#include "vfs_detail.h"

#include <algorithm>

namespace vfs_detail {
    const s_vfs_backend_ops g_vfs_zip_file_ops = {
        Zip_File_Read_,
        Zip_File_Size_,
        Zip_File_Seek_,
        Zip_File_Tell_,
        Zip_File_Close_,
    };

    bool Compare_Zip_Cache_Entries_(const s_vfs_zip_cache_entry& lhs, const s_vfs_zip_cache_entry& rhs) {
        const int path_compare = strcmp(lhs.asset_key, rhs.asset_key);
        if (path_compare != 0) {
            return path_compare < 0;
        }

        if (lhs.entry.archive_index != rhs.entry.archive_index) {
            return lhs.entry.archive_index < rhs.entry.archive_index;
        }

        return lhs.entry.file_index < rhs.entry.file_index;
    }

    void Destroy_Zip_Iter_State_(s_vfs_zip_file_stream* zip_stream) {
        if (zip_stream == nullptr || zip_stream->iter_state == nullptr) {
            return;
        }

        mz_zip_reader_extract_iter_free(zip_stream->iter_state);
        zip_stream->iter_state = nullptr;
    }

    bool Create_Zip_Iter_State_(s_vfs_zip_file_stream* zip_stream) {
        mz_zip_archive* archive;

        if (zip_stream == nullptr || zip_stream->archive_index >= g_vfs_state.zip_archives.size()) {
            return false;
        }

        archive = &g_vfs_state.zip_archives[zip_stream->archive_index].archive;
        zip_stream->iter_state = mz_zip_reader_extract_iter_new(archive, zip_stream->file_index, 0);
        return zip_stream->iter_state != nullptr;
    }

    bool Rewind_Zip_Stream_(s_vfs_zip_file_stream* zip_stream) {
        Destroy_Zip_Iter_State_(zip_stream);
        return Create_Zip_Iter_State_(zip_stream);
    }

    bool Skip_Zip_Stream_(s_vfs_zip_file_stream* zip_stream, int32_t byte_count) {
        uint8_t discard_buffer[4096];
        int32_t remaining = byte_count;

        while (remaining > 0) {
            const size_t chunk_size = std::min((size_t)remaining, sizeof(discard_buffer));
            const size_t bytes_read = mz_zip_reader_extract_iter_read(zip_stream->iter_state, discard_buffer, chunk_size);
            if (bytes_read != chunk_size) {
                return false;
            }

            remaining -= (int32_t)bytes_read;
        }

        return true;
    }

    bool Find_Zip_Entry_(const char* asset_path, s_vfs_zip_entry* zip_entry) {
        s_vfs_zip_cache_entry lookup_entry{};
        std::vector<s_vfs_zip_cache_entry>::const_iterator it;

        if (!Build_Asset_Key_(asset_path, lookup_entry.asset_key, sizeof(lookup_entry.asset_key))) {
            return false;
        }

        it = std::lower_bound(g_vfs_state.zip_entries.begin(), g_vfs_state.zip_entries.end(), lookup_entry, Compare_Zip_Cache_Entries_);
        if (it == g_vfs_state.zip_entries.end() || strcmp(it->asset_key, lookup_entry.asset_key) != 0) {
            return false;
        }

        if (zip_entry != nullptr) {
            *zip_entry = it->entry;
        }

        return true;
    }

    void Close_Zip_Archives_() {
        for (s_vfs_zip_archive& zip_archive : g_vfs_state.zip_archives) {
            mz_zip_reader_end(&zip_archive.archive);
            zip_archive.archive_path[0] = '\0';
        }

        g_vfs_state.zip_archives.clear();
        g_vfs_state.zip_entries.clear();
    }

    bool Read_Zip_File_Entry_(mz_zip_archive* archive, uint32_t archive_index, mz_uint file_index, s_vfs_zip_cache_entry* cache_entry) {
        mz_zip_archive_file_stat file_stat;
        char normalized_path[VFS_MAX_PATH];

        if (archive == nullptr || cache_entry == nullptr) {
            return false;
        }

        if (!mz_zip_reader_file_stat(archive, file_index, &file_stat)) {
            return false;
        }

        if (file_stat.m_is_directory || file_stat.m_is_encrypted || !file_stat.m_is_supported || file_stat.m_uncomp_size > INT32_MAX) {
            return false;
        }

        if (!Normalize_Asset_Path_(file_stat.m_filename, normalized_path, sizeof(normalized_path))) {
            return false;
        }

        if (!Build_Asset_Key_(normalized_path, cache_entry->asset_key, sizeof(cache_entry->asset_key))) {
            return false;
        }

        cache_entry->entry.archive_index = archive_index;
        cache_entry->entry.file_index = file_index;
        cache_entry->entry.uncompressed_size = (int32_t)file_stat.m_uncomp_size;
        return true;
    }

    SDL_EnumerationResult SDLCALL Collect_Zip_Paths_(void* userdata, const char* dirname, const char* fname) {
        s_vfs_zip_scan_result* scan_result = (s_vfs_zip_scan_result*)userdata;
        char full_path[VFS_MAX_PATH];
        char extension[VFS_MAX_PATH];
        const char* file_extension;

        if (scan_result == nullptr || dirname == nullptr || fname == nullptr) {
            return SDL_ENUM_FAILURE;
        }

        file_extension = strrchr(fname, '.');
        if (file_extension == nullptr) {
            return SDL_ENUM_CONTINUE;
        }

        if (strlcpy(extension, file_extension, sizeof(extension)) >= sizeof(extension)) {
            scan_result->failed = true;
            return SDL_ENUM_FAILURE;
        }

        for (size_t i = 0; extension[i] != '\0'; ++i) {
            extension[i] = (char)SDL_tolower((unsigned char)extension[i]);
        }

        if (strcmp(extension, ".zip") != 0) {
            return SDL_ENUM_CONTINUE;
        }

        if (snprintf(full_path, sizeof(full_path), "%s/%s", dirname, fname) >= (int)sizeof(full_path)) {
            scan_result->failed = true;
            return SDL_ENUM_FAILURE;
        }

        scan_result->zip_paths.emplace_back(full_path);
        return SDL_ENUM_CONTINUE;
    }

    bool Scan_Zip_Archives_() {
        char data_directory[VFS_MAX_PATH];
        s_vfs_zip_scan_result scan_result{};

        if (!Build_Candidate_Path_(g_vfs_state.base_path, "", VFS_ZIP_DIRECTORY, data_directory, sizeof(data_directory))) {
            return true;
        }

        if (!SDL_EnumerateDirectory(data_directory, Collect_Zip_Paths_, &scan_result)) {
            SDL_PathInfo path_info;
            if (!SDL_GetPathInfo(data_directory, &path_info) || path_info.type != SDL_PATHTYPE_DIRECTORY) {
                return true;
            }

            return false;
        }

        if (scan_result.failed || scan_result.zip_paths.empty()) {
            if (scan_result.failed) {
                return false;
            }

            return true;
        }

        std::sort(scan_result.zip_paths.begin(), scan_result.zip_paths.end(), [](const std::string& lhs, const std::string& rhs) {
            const int file_name_compare = strcasecmp(Get_File_Name_(lhs.c_str()), Get_File_Name_(rhs.c_str()));
            if (file_name_compare != 0) {
                return file_name_compare < 0;
            }

            return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
        });

        g_vfs_state.zip_archives.reserve(scan_result.zip_paths.size());
        for (size_t i = 0; i < scan_result.zip_paths.size(); ++i) {
            mz_uint total_files;

            g_vfs_state.zip_archives.emplace_back();
            s_vfs_zip_archive& zip_archive = g_vfs_state.zip_archives.back();

            memset(&zip_archive.archive, 0, sizeof(zip_archive.archive));
            if (strlcpy(zip_archive.archive_path, scan_result.zip_paths[i].c_str(), sizeof(zip_archive.archive_path)) >= sizeof(zip_archive.archive_path)) {
                Close_Zip_Archives_();
                return false;
            }

            if (!mz_zip_reader_init_file(&zip_archive.archive, zip_archive.archive_path, 0)) {
                Close_Zip_Archives_();
                return false;
            }

            total_files = mz_zip_reader_get_num_files(&zip_archive.archive);
            for (mz_uint file_index = 0; file_index < total_files; ++file_index) {
                s_vfs_zip_cache_entry cache_entry{};

                if (Read_Zip_File_Entry_(&zip_archive.archive, (uint32_t)i, file_index, &cache_entry)) {
                    g_vfs_state.zip_entries.push_back(cache_entry);
                }
            }
        }

        std::sort(g_vfs_state.zip_entries.begin(), g_vfs_state.zip_entries.end(), Compare_Zip_Cache_Entries_);
        return true;
    }

    s_vfs_file* Open_Zip_File_(const s_vfs_zip_entry& zip_entry) {
        s_vfs_zip_file_stream* zip_stream = new s_vfs_zip_file_stream{};
        s_vfs_file* vfs_file;

        zip_stream->archive_index = zip_entry.archive_index;
        zip_stream->file_index = zip_entry.file_index;
        if (!Create_Zip_Iter_State_(zip_stream)) {
            delete zip_stream;
            return nullptr;
        }

        vfs_file = new s_vfs_file{};
        vfs_file->backend_kind = VFS_BACKEND_ZIP_ENTRY;
        vfs_file->ops = &g_vfs_zip_file_ops;
        vfs_file->backend_state = zip_stream;
        vfs_file->data_size = zip_entry.uncompressed_size;
        vfs_file->position = 0;
        return vfs_file;
    }

    s_vfs_zip_file_stream* Get_Zip_File_Stream_(s_vfs_file* file) {
        if (file == nullptr || file->backend_kind != VFS_BACKEND_ZIP_ENTRY) {
            return nullptr;
        }

        return (s_vfs_zip_file_stream*)file->backend_state;
    }

    bool Seek_Zip_File_(s_vfs_file* file, int32_t new_position) {
        s_vfs_zip_file_stream* zip_stream = Get_Zip_File_Stream_(file);

        if (zip_stream == nullptr) {
            return false;
        }

        if (new_position < file->position) {
            if (!Rewind_Zip_Stream_(zip_stream)) {
                return false;
            }
            file->position = 0;
        }

        if (new_position > file->position && !Skip_Zip_Stream_(zip_stream, new_position - file->position)) {
            return false;
        }

        file->position = new_position;
        return true;
    }

    size_t Zip_File_Read_(void* ptr, size_t size, size_t count, s_vfs_file* file) {
        s_vfs_zip_file_stream* zip_stream;
        size_t remaining_bytes;
        size_t max_count;
        size_t requested_bytes;
        size_t bytes_read;

        if (ptr == nullptr || file == nullptr || size == 0 || count == 0) {
            return 0;
        }

        if (file->position >= file->data_size) {
            return 0;
        }

        zip_stream = Get_Zip_File_Stream_(file);
        if (zip_stream == nullptr) {
            return 0;
        }

        remaining_bytes = (size_t)(file->data_size - file->position);
        max_count = remaining_bytes / size;
        if (max_count == 0) {
            return 0;
        }

        if (max_count > count) {
            max_count = count;
        }

        requested_bytes = max_count * size;
        bytes_read = mz_zip_reader_extract_iter_read(zip_stream->iter_state, ptr, requested_bytes);
        if (bytes_read > 0) {
            file->position += (int32_t)bytes_read;
        }

        return bytes_read / size;
    }

    int32_t Zip_File_Size_(s_vfs_file* file) {
        if (file == nullptr) {
            return -1;
        }

        return file->data_size;
    }

    int32_t Zip_File_Seek_(s_vfs_file* file, int32_t offset, int32_t origin) {
        int64_t new_position;

        if (file == nullptr) {
            return -1;
        }

        switch (origin) {
            case SEEK_SET:
                new_position = offset;
                break;
            case SEEK_CUR:
                new_position = (int64_t)file->position + offset;
                break;
            case SEEK_END:
                new_position = (int64_t)file->data_size + offset;
                break;
            default:
                return -1;
        }

        if (new_position < 0 || new_position > file->data_size) {
            return -1;
        }

        return Seek_Zip_File_(file, (int32_t)new_position) ? 0 : -1;
    }

    int32_t Zip_File_Tell_(s_vfs_file* file) {
        if (file == nullptr) {
            return -1;
        }

        return file->position;
    }

    int32_t Zip_File_Close_(s_vfs_file* file) {
        s_vfs_zip_file_stream* zip_stream;

        if (file == nullptr) {
            return -1;
        }

        zip_stream = Get_Zip_File_Stream_(file);
        Destroy_Zip_Iter_State_(zip_stream);
        delete zip_stream;
        delete file;
        return 0;
    }
}