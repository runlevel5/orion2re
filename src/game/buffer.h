#pragma once
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

    inline void* __cdecl Farload_Buffer_Library_(const char* file_name, int32_t buffer_seg_id, void* buffer_space_seg, int32_t load_mode) {
        using Fn = void*(__cdecl*)(const char*, int32_t, void*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052C700});
        return fn(file_name, buffer_seg_id, buffer_space_seg, load_mode);
    }

    void* __cdecl Buffer_Reload_(const char* file_name, int32_t buffer_seg_id, void* buffer_space_seg);
}