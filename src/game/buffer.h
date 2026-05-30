#pragma once

#include <cstdint>

struct s_file_buffer_entry;

namespace buffer {
    extern int32_t _global_load_count;

    extern int16_t _file_buffer_count;

    extern s_file_buffer_entry* _file_buffer;

    extern void* _buffer_space_seg;

    extern int32_t _buffer_space_size;

    extern void* _buffer_space_ptr;

    void __cdecl Free_Buffer_Space_();

    int8_t __cdecl Flush_Buffer_Space_(void* param_1);

    uint8_t* __cdecl Allocate_Buffer_Space_(uint32_t requested_size);

    void* __cdecl Farload_Buffer_Library_(const char* file_name, int32_t entry_index, uint8_t* buffer_base);

    void __cdecl Get_Lowest_Priority_(int32_t* out_file_index, int32_t* out_segment_index);

    void __cdecl Get_Next_Buffer_Address_(int32_t current_buffer_index,
                                          int32_t current_segment_index,
                                          int32_t* next_buffer_index,
                                          int32_t* next_segment_index,
                                          uint8_t* buffer_data);

    void* __cdecl Buffer_Reload_(const char* file_name, int32_t buffer_seg_id, void* buffer_space_seg);
}