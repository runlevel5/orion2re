#pragma once

namespace buffer {
    inline uint32_t& _global_load_count =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055B30C});

    inline uint16_t& _file_buffer_count =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0057B5BC});

    inline uint8_t*& _file_buffer =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x0057B5C0});

    inline uint8_t*& _buffer_space_seg =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x0057B5C4});

    inline uint32_t& _buffer_space_size =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0057B5C8});

    inline uint8_t*& _buffer_space_ptr =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x0057B5CC});

    void __cdecl Free_Buffer_Space_();

    int8_t __cdecl Flush_Buffer_Space_(void* buffer_context);

    uint8_t* __cdecl Allocate_Buffer_Space_(uint32_t requested_size);
}