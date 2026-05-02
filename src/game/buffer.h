#pragma once

#include <cstdint>

struct s_file_buffer_entry;

namespace buffer {
    inline int32_t& _global_load_count =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055B30C});

    inline int16_t& _file_buffer_count =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0057B5BC});

    inline s_file_buffer_entry*& _file_buffer =
        *reinterpret_cast<s_file_buffer_entry**>(std::uintptr_t{0x0057B5C0});

    inline void*& _buffer_space_seg =
        *reinterpret_cast<void**>(std::uintptr_t{0x0057B5C4});

    inline int32_t& _buffer_space_size =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0057B5C8});

    inline void*& _buffer_space_ptr =
        *reinterpret_cast<void**>(std::uintptr_t{0x0057B5CC});

    void __cdecl Free_Buffer_Space_();

    int8_t __cdecl Flush_Buffer_Space_(void* buffer_context);

    uint8_t* __cdecl Allocate_Buffer_Space_(uint32_t requested_size);

    void* __cdecl Farload_Buffer_Library_(const char* file_name, int32_t entry_index, uint8_t* buffer_base);

    inline void __cdecl Get_Lowest_Priority_(int32_t* out_file_buffer_index, int32_t* out_segment_index) {
        using Fn = void(__cdecl*)(int32_t*, int32_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052CD00});
        fn(out_file_buffer_index, out_segment_index);
    }

    inline void __cdecl Get_Next_Buffer_Address_(
            int32_t file_buffer_index,
            int32_t segment_index,
            int32_t* out_next_file_buffer_index,
            int32_t* out_next_segment_index,
            uint8_t* buffer_base) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t*, int32_t*, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052CC10});
        fn(file_buffer_index, segment_index, out_next_file_buffer_index, out_next_segment_index, buffer_base);
    }

    void* __cdecl Buffer_Reload_(const char* file_name, int32_t buffer_seg_id, void* buffer_space_seg);
}