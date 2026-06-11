#include "pch.h"

namespace sound {
    extern uint8_t _close_stream;
    extern uint8_t _musicActive;
}

namespace lbx {
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
                return;
            }
        }

        fread(header, sizeof(header->entry_count) + sizeof(header->magic) + sizeof(header->header_data), 1, header->file);

        if (header->magic != 0xFEAD) {
            fclose(header->file);
            header->file = nullptr;
            return;
        }

        uint32_t* raw_offsets = &header->entries[254].offset;
        fread(raw_offsets, sizeof(*raw_offsets), 510, header->file);

        header->first_offset = raw_offsets[0];

        int32_t entry_count = (int16_t)header->entry_count;
        for (int32_t entry_idx = 0; entry_idx < entry_count; ++entry_idx) {
            header->entries[entry_idx].offset = raw_offsets[entry_idx + 1];
            uint32_t prev_offset = (entry_idx == 0) ? header->first_offset : header->entries[entry_idx - 1].offset;
            header->entries[entry_idx].size = header->entries[entry_idx].offset - prev_offset;
        }

        strcpy(header->filename, filename);
    }

    uint32_t __cdecl LBX_getRecordLength_(s_lbx_stream_header* header, int32_t record_idx) {
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
                sound::_musicActive = 0;
                return;
            }
        }

        fread(stream, sizeof(stream->entry_count) + sizeof(stream->magic) + sizeof(stream->header_data), 1, stream->file);

        if (stream->magic != 0xFEAD) {
            fclose(stream->file);
            stream->file = nullptr;
            sound::_musicActive = 0;
            return;
        }

        uint32_t* raw_offsets = &stream->entries[254].offset;
        fread(raw_offsets, sizeof(*raw_offsets), 510, stream->file);

        stream->first_offset = raw_offsets[0];

        int32_t entry_count = (int16_t)stream->entry_count;
        for (int32_t entry_idx = 0; entry_idx < entry_count; ++entry_idx) {
            stream->entries[entry_idx].offset = raw_offsets[entry_idx + 1];
            uint32_t prev_offset = (entry_idx == 0) ? stream->first_offset : stream->entries[entry_idx - 1].offset;
            stream->entries[entry_idx].size = raw_offsets[entry_idx + 1] - prev_offset;
        }

        strcpy(stream->filename, filename);

        if (was_music_active == 1) {
            sound::_musicActive = 1;
        }

        sound::_close_stream = 0;
    }

    void __cdecl LBX_readRecord_(s_lbx_stream_header* header, int32_t record_idx, void* buffer) {
        if (header != nullptr && header->file != nullptr) {
            if ((int16_t)header->entry_count >= record_idx + 1) {
                uint32_t offset = (record_idx == 0)
                    ? header->first_offset
                    : header->entries[record_idx - 1].offset;
                fseek(header->file, offset, SEEK_SET);
                fread(buffer, header->entries[record_idx].size, 1, header->file);
            }
        }
    }
}