#pragma once

struct s_lbx_stream_entry {
    uint32_t size;
    uint32_t offset;
};

#pragma pack(push, 1)

struct s_lbx_disk_header {
    uint16_t entry_count;
    uint16_t magic;
    uint32_t header_data;
};

#pragma pack(pop)

struct s_lbx_stream_header {
    uint16_t entry_count;
    uint16_t magic;
    uint32_t header_data;
    FILE* file;
    uint32_t first_offset;
    s_lbx_stream_entry entries[509];
    uint32_t padding;
    char filename[80];
};

struct s_wave_stream_header {
    uint16_t entry_count;
    uint16_t magic;
    uint32_t header_data;
    FILE* file;
    uint32_t first_offset;
    s_lbx_stream_entry entries[509];
    uint32_t padding;
    char filename[80];
};