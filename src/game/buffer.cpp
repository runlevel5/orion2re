#include "pch.h"

namespace buffer {
  int8_t __cdecl Flush_Buffer_Space_(void* buffer_context) {
    auto* file_buffer = static_cast<uint8_t*>(_file_buffer);

    for (int32_t outer_offset = 0; outer_offset < 0x374c4; outer_offset += 0x11b2) {
      for (int32_t inner_offset = outer_offset, remaining = 0x19a; remaining != 0; --remaining) {
        inner_offset += 0xb;
        *reinterpret_cast<uint32_t*>(file_buffer + inner_offset + 8) = 0;
      }
    }

    _global_load_count = 0;
    return 1;
  }

  void __cdecl Free_Buffer_Space_() {
    if (_buffer_space_ptr != nullptr) {
      _free(_buffer_space_ptr);
      _free(_file_buffer);
      _buffer_space_ptr = nullptr;
    }
  }

  uint8_t* __cdecl Allocate_Buffer_Space_(uint32_t requested_size) {
    uint32_t space_size = requested_size;

    if (static_cast<int32_t>(requested_size - 0x374b8) > 0) {
      space_size = requested_size - 0x374c4;
    } else {
      exitm::Exit_With_Message(s_Not_Enough_Space_For_Buffer_Spac_0055b310);
    }

    uint32_t allocation_size = space_size;
    if ((allocation_size & 1U) != 0) {
      allocation_size = (allocation_size & 0xFFFFFFFCu) + 4U;
    }

    uint8_t* buffer_space = static_cast<uint8_t*>(allocate::Allocate_Far_Space_(allocation_size + 0xC));
    if (buffer_space == nullptr) {
      allocate::Allocation_Error_(space_size);
    }

    _buffer_space_ptr = buffer_space;

    auto* header = reinterpret_cast<uint32_t*>(buffer_space);
    header[0] = 0;
    header[1] = space_size;
    header[2] = 0;

    uint8_t* buffer_space_segment = buffer_space + 0xC;
    _buffer_space_size = space_size;
    _buffer_space_seg = buffer_space_segment;

    _file_buffer = static_cast<uint8_t*>(allocate::Allocate_Far_Space_(0x374c4));
    _file_buffer_count = 0;

    uint8_t* file_buffer = _file_buffer;
    uint32_t chunk_offset = 0;
    do {
      uint32_t entry_offset = chunk_offset;
      for (uint32_t remaining = 0x19a; remaining != 0; --remaining) {
        entry_offset += 0xB;
        file_buffer[entry_offset + 5] = 0;
        *reinterpret_cast<uint32_t*>(file_buffer + entry_offset + 8) = 0;
        *reinterpret_cast<uint16_t*>(file_buffer + entry_offset + 6) = 0;
      }

      chunk_offset += 0x11B2;
      *reinterpret_cast<uint32_t*>(file_buffer + chunk_offset - 4) = 0;
    } while (chunk_offset < 0x374c4);

    return buffer_space_segment;
  }
}