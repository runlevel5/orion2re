#include "pch.h"

namespace lbx {
    void __cdecl LBX_init_(s_lbx_stream_header* header, const char* filename) {
        memset(header, 0, sizeof(s_lbx_stream_header));

        header->file = _wfopen(filename, s_r_b_0055bf20);
        
        if (header->file == nullptr) {
            if (farload::_secondary_drive_path[0] != '\0') {
                char full_path[80];
                strcpy(full_path, farload::_secondary_drive_path);
                strcat(full_path, filename);
                header->file = _wfopen(full_path, s_r_b_0055bf20);
            }
            
            if (header->file == nullptr) {
                return;
            }
        }

        _fread(header, 8, 1, header->file);

        if (header->magic != 0xFEAD) {
            _fclose(header->file);
            header->file = nullptr;
            return;
        }

        uint32_t* temp_offsets = &header->entries[254].offset;
        _fread(temp_offsets, 4, 510, header->file);

        header->first_offset = temp_offsets[0];

        for (int i = 0; i < (int16_t)header->entry_count; ++i) {
            header->entries[i].offset = temp_offsets[i + 1];
            uint32_t prev_offset = (i == 0) ? header->first_offset : header->entries[i - 1].offset;
            header->entries[i].size = header->entries[i].offset - prev_offset;
        }

        strcpy(header->filename, filename);
    }

    int32_t __cdecl LBX_getRecordLength_(s_lbx_stream_header* header, uint16_t rec_idx) {
        return (int32_t)header->entries[rec_idx].size;
    }

    void __cdecl LBX_uninit_(s_lbx_stream_header* header) {
        if (header->file != nullptr) {
            _fclose(header->file);
        }
        memset(header, 0, sizeof(s_lbx_stream_header));
    }

    void __cdecl LBX_init_stream_(s_wave_stream_header* stream, const char* filename) {
        uint8_t music_active = sound::_musicActive;

        if (stream->file != nullptr && sound::_close_stream == 0) {
            mmioClose(stream->file, 0);
        }

        memset(stream, 0, sizeof(s_wave_stream_header));

        stream->file = mmioOpenA(filename, nullptr, 0x10000);

        if (stream->file == nullptr) {
            if (farload::_secondary_drive_path[0] != '\0') {
                char local_50[80];
                strcpy(local_50, farload::_secondary_drive_path);
                strcat(local_50, filename);
                stream->file = mmioOpenA(local_50, nullptr, 0x10000);
            }
            if (stream->file == nullptr) {
                sound::_musicActive = 0;
                return;
            }
        }

        mmioRead(stream->file, reinterpret_cast<char*>(stream), 8);

        if (stream->magic != 0xFEAD) {
            mmioClose(stream->file, 0);
            stream->file = nullptr;
            sound::_musicActive = 0;
            return;
        }

        uint32_t* raw_offsets = &stream->entries[254].offset;
        mmioRead(stream->file, reinterpret_cast<char*>(raw_offsets), 0x7F8);

        stream->first_offset = raw_offsets[0];

        int count = static_cast<int16_t>(stream->entry_count);
        for (int i = 0; i < count; ++i) {
            stream->entries[i].offset = raw_offsets[i + 1];
            uint32_t prev_offset = (i == 0) ? stream->first_offset : stream->entries[i - 1].offset;
            stream->entries[i].size = raw_offsets[i + 1] - prev_offset;
        }

        strcpy(stream->filename, filename);

        if (music_active == 1) {
            sound::_musicActive = 1;
        }

        sound::_close_stream = 0;
    }

    void __cdecl LBX_readRecord_(s_lbx_stream_header* header, int32_t record_idx, void* buffer) {
        if (header != nullptr && header->file != nullptr) {
            if ((int16_t)header->entry_count >= record_idx + 1) {
                // BUG: record_idx 0 leads to negative index entries[-1]
                _fseek(header->file, header->entries[record_idx - 1].offset, SEEK_SET);
                _fread(buffer, header->entries[record_idx].size, 1, header->file);
            }
        }
    }
}