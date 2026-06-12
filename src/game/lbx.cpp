#include "pch.h"

namespace sound {
    extern uint8_t _close_stream;
    extern uint8_t _musicActive;
}

#define LBX_MAX_ENTRIES 509
#define LBX_OFFSET_COUNT 510

namespace lbx {
    namespace {
        bool Read_LBX_Offsets_(FILE* file, s_lbx_stream_entry* entries, uint16_t entry_count, uint32_t* first_offset, const char* filename) {
            uint32_t raw_offsets[LBX_OFFSET_COUNT] = {};

            if (fread(raw_offsets, sizeof(raw_offsets[0]), LBX_OFFSET_COUNT, file) != LBX_OFFSET_COUNT) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "lbx: failed to read offset table from %s", filename);
                return false;
            }

            *first_offset = raw_offsets[0];

            for (uint16_t entry_idx = 0; entry_idx < entry_count; ++entry_idx) {
                entries[entry_idx].offset = raw_offsets[entry_idx + 1];
                uint32_t prev_offset = (entry_idx == 0) ? *first_offset : entries[entry_idx - 1].offset;
                entries[entry_idx].size = entries[entry_idx].offset - prev_offset;
            }

            return true;
        }

        bool Read_LBX_Header_(FILE* file, uint16_t* entry_count, uint16_t* magic, uint32_t* header_data, const char* filename) {
            s_lbx_disk_header disk_header = {};

            if (fread(&disk_header, sizeof(disk_header), 1, file) != 1) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "lbx: failed to read header from %s", filename);
                return false;
            }

            if (disk_header.magic != 0xFEAD) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "lbx: invalid magic 0x%04x in %s", disk_header.magic, filename);
                return false;
            }

            if (disk_header.entry_count > LBX_MAX_ENTRIES) {
                SDL_LogError(
                    SDL_LOG_CATEGORY_APPLICATION,
                    "lbx: entry_count %u exceeds max %u in %s",
                    (unsigned int)disk_header.entry_count,
                    (unsigned int)LBX_MAX_ENTRIES,
                    filename
                );
                return false;
            }

            *entry_count = disk_header.entry_count;
            *magic = disk_header.magic;
            *header_data = disk_header.header_data;
            return true;
        }
    }

    void __cdecl LBX_init_(s_lbx_stream_header* header, const char* filename) {
        memset(header, 0, sizeof(s_lbx_stream_header));

        header->file = fopen_case(filename, "r+b");
        
        if (header->file == nullptr) {
            if (farload::_secondary_drive_path[0] != '\0') {
                char full_path[sizeof(header->filename)];
                strcpy(full_path, farload::_secondary_drive_path);
                strcat(full_path, filename);
                header->file = fopen_case(full_path, "r+b");
            }
            
            if (header->file == nullptr) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "lbx: unable to open %s", filename);
                return;
            }
        }

        if (!Read_LBX_Header_(header->file, &header->entry_count, &header->magic, &header->header_data, filename)) {
            fclose(header->file);
            header->file = nullptr;
            return;
        }

        if (!Read_LBX_Offsets_(header->file, header->entries, header->entry_count, &header->first_offset, filename)) {
            fclose(header->file);
            header->file = nullptr;
            return;
        }

        strcpy(header->filename, filename);
    }

    uint32_t __cdecl LBX_getRecordLength_(s_lbx_stream_header* header, int32_t record_idx) {
        if (header == nullptr || header->file == nullptr) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "lbx: get length on unopened stream");
            return 0;
        }

        if (record_idx < 0 || record_idx >= (int32_t)header->entry_count) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "lbx: get length invalid record %d for %s", record_idx, header->filename);
            return 0;
        }

        return header->entries[record_idx].size;
    }

    void __cdecl LBX_uninit_(s_lbx_stream_header* header) {
        if (header->file != nullptr) {
            fclose(header->file);
        }
        memset(header, 0, sizeof(s_lbx_stream_header));
    }

    void __cdecl LBX_init_stream_(s_wave_stream_header* stream, const char* filename) {
        uint8_t was_music_active = sound::_musicActive;

        if (stream->file != nullptr && sound::_close_stream == 0) {
            fclose(stream->file);
        }

        memset(stream, 0, sizeof(s_wave_stream_header));

        stream->file = fopen_case(filename, "rb");

        if (stream->file == nullptr) {
            if (farload::_secondary_drive_path[0] != '\0') {
                char full_path[sizeof(stream->filename)];
                strcpy(full_path, farload::_secondary_drive_path);
                strcat(full_path, filename);
                stream->file = fopen_case(full_path, "rb");
            }
            if (stream->file == nullptr) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "lbx: unable to open stream %s", filename);
                sound::_musicActive = 0;
                return;
            }
        }

        if (!Read_LBX_Header_(stream->file, &stream->entry_count, &stream->magic, &stream->header_data, filename)) {
            fclose(stream->file);
            stream->file = nullptr;
            sound::_musicActive = 0;
            return;
        }

        if (!Read_LBX_Offsets_(stream->file, stream->entries, stream->entry_count, &stream->first_offset, filename)) {
            fclose(stream->file);
            stream->file = nullptr;
            sound::_musicActive = 0;
            return;
        }

        strcpy(stream->filename, filename);

        if (was_music_active == 1) {
            sound::_musicActive = 1;
        }

        sound::_close_stream = 0;
    }

    void __cdecl LBX_readRecord_(s_lbx_stream_header* header, int32_t record_idx, void* buffer) {
        if (header == nullptr || header->file == nullptr) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "lbx: read on unopened stream");
            return;
        }

        if (record_idx < 0 || record_idx >= (int32_t)header->entry_count) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "lbx: read invalid record %d for %s", record_idx, header->filename);
            return;
        }

        uint32_t offset = (record_idx == 0)
            ? header->first_offset
            : header->entries[record_idx - 1].offset;

        if (fseek(header->file, offset, SEEK_SET) != 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "lbx: seek failed for %s record %d", header->filename, record_idx);
            return;
        }

        if (fread(buffer, header->entries[record_idx].size, 1, header->file) != 1) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "lbx: read failed for %s record %d", header->filename, record_idx);
        }
    }
}