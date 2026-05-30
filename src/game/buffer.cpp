#include "pch.h"

namespace buffer {
  int32_t _global_load_count;
  int16_t _file_buffer_count;
  s_file_buffer_entry* _file_buffer;
  void* _buffer_space_seg;
  int32_t _buffer_space_size;
  void* _buffer_space_ptr;

  int8_t __cdecl Flush_Buffer_Space_(void* param_1) {
    (void)param_1;

    for (int32_t file_index = 0; file_index < 50; ++file_index) {
      for (int32_t segment_index = 0; segment_index < 410; ++segment_index) {
        _file_buffer[file_index].segments[segment_index].last_use_counter = 0;
      }
    }

    _global_load_count = 0;
    return (int8_t)1;
  }

  void __cdecl Free_Buffer_Space_() {
    if (_buffer_space_ptr != nullptr) {
      _free(_buffer_space_ptr);
      _free(_file_buffer);
      _buffer_space_ptr = nullptr;
    }
  }

  uint8_t* __cdecl Allocate_Buffer_Space_(uint32_t requested_size) {
    uint32_t allocation_error_size = requested_size;
    uint32_t space_size;

    if (static_cast<int32_t>(requested_size - 0x374b8) > 0) {
      allocation_error_size = requested_size - 0x374c4;
      space_size = allocation_error_size;
    } else {
      exitm::Exit_With_Message(s_Not_Enough_Space_For_Buffer_Spac_0055b310);
      space_size = requested_size;
    }

    uint32_t allocation_size = space_size;
    if ((allocation_size & 1U) != 0) {
      allocation_size = (allocation_size & 0xFFFFFFFCu) + 4U;
    }

    uint8_t* buffer_space = static_cast<uint8_t*>(allocate::Allocate_Far_Space_(allocation_size + 0xC));
    if (buffer_space == nullptr) {
      allocate::Allocation_Error_((int)allocation_error_size);
    }

    _buffer_space_ptr = buffer_space;

    auto* header = (allocate::s_alloc_header*)buffer_space;
    header->current = 0;
    header->size = space_size;
    header->reserved2 = 0;

    uint8_t* buffer_space_segment = buffer_space + 0xC;
    _buffer_space_size = space_size;
    _buffer_space_seg = buffer_space_segment;

    _file_buffer = reinterpret_cast<s_file_buffer_entry*>(allocate::Allocate_Far_Space_(0x374c4));
    _file_buffer_count = 0;

    for (int32_t file_index = 0; file_index < 0x32; ++file_index) {
      s_file_buffer_entry& file_entry = _file_buffer[file_index];
      file_entry.segment_count = 0;

      for (int32_t segment_index = 0; segment_index < 0x19A; ++segment_index) {
        s_buffer_cached_segment& segment = file_entry.segments[segment_index];
        segment.is_loaded = 0;
        segment.buffer_seg_id = 0;
        segment.last_use_counter = 0;
      }
    }

    return buffer_space_segment;
  }

  void __cdecl Get_Lowest_Priority_(int32_t* out_file_index, int32_t* out_segment_index) {
    uint32_t lowest_priority = 0xFFFFFFFFu;

    if (buffer::_file_buffer_count > 0) {
      for (int32_t file_index = 0; file_index < buffer::_file_buffer_count; ++file_index) {
        s_file_buffer_entry& file_entry = buffer::_file_buffer[file_index];

        if (file_entry.segment_count > 0) {
          for (int32_t segment_index = 0; segment_index < file_entry.segment_count; ++segment_index) {
            s_buffer_cached_segment& segment = file_entry.segments[segment_index];

            if (static_cast<uint32_t>(segment.last_use_counter) <= lowest_priority) {
              if (segment.is_loaded == 1) {
                *out_file_index = file_index;
                *out_segment_index = segment_index;
                lowest_priority = static_cast<uint32_t>(segment.last_use_counter);
              }
            }
          }
        }
      }
    }
  }

  void* __cdecl Buffer_Reload_(const char* file_name, int32_t buffer_seg_id, void* buffer_space_seg) {
    if (buffer_space_seg != buffer::_buffer_space_seg) {
      exitm::Exit_With_Message(s_Cache_Seg_Corrupted____0055b374);
    }

    if (*reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(buffer_space_seg) - 8) != buffer::_buffer_space_size) {
      exitm::Exit_With_Message(s_Cache_Seg_Corrupted____0055b374);
    }

    bool found_file = false;
    int32_t file_index = 0;

    if (buffer::_file_buffer_count > 0) {
      for (int32_t i = 0; i < static_cast<int32_t>(buffer::_file_buffer_count); ++i) {
        if (_strcmpi(file_name, buffer::_file_buffer[i].file_name) == 0) {
          found_file = true;
          file_index = i;
          break;
        }
      }
    }

    if (found_file) {
      s_file_buffer_entry& file_entry = buffer::_file_buffer[file_index];

      for (int32_t segment_index = 0; segment_index < file_entry.segment_count; ++segment_index) {
        s_buffer_cached_segment& segment = file_entry.segments[segment_index];

        if (segment.buffer_seg_id == buffer_seg_id) {
          if (segment.is_loaded == 1) {
            ++buffer::_global_load_count;
            segment.last_use_counter = buffer::_global_load_count;
            return segment.data_ptr;
          }

          ++buffer::_global_load_count;
          segment.last_use_counter = buffer::_global_load_count;
          segment.data_ptr = Farload_Buffer_Library_(file_name, buffer_seg_id, static_cast<uint8_t*>(buffer_space_seg));
          segment.is_loaded = 1;
          return segment.data_ptr;
        }
      }

      if (file_entry.segment_count + 1 >= 0x19A) {
        exitm::Exit_With_Message(s_max_buffer_segs_went_over___0055b358);
      }

      ++buffer::_global_load_count;

      s_buffer_cached_segment& segment = file_entry.segments[file_entry.segment_count];
      segment.buffer_seg_id = static_cast<int16_t>(buffer_seg_id);
      segment.last_use_counter = buffer::_global_load_count;
      segment.data_ptr = Farload_Buffer_Library_(file_name, buffer_seg_id, static_cast<uint8_t*>(buffer_space_seg));
      segment.is_loaded = 1;

      ++file_entry.segment_count;
      return file_entry.segments[file_entry.segment_count - 1].data_ptr;
    }

    if (static_cast<int32_t>(buffer::_file_buffer_count) + 1 >= 0x32) {
      exitm::Exit_With_Message(s_max_file_buffer_count_went_over__0055b334);
    }

    ++buffer::_global_load_count;

    s_file_buffer_entry& file_entry = buffer::_file_buffer[buffer::_file_buffer_count];
    memcpy(file_entry.file_name, file_name, strlen(file_name) + 1);

    file_entry.segment_count = 0;

    s_buffer_cached_segment& segment = file_entry.segments[file_entry.segment_count];
    segment.buffer_seg_id = static_cast<int16_t>(buffer_seg_id);
    segment.last_use_counter = buffer::_global_load_count;
    segment.data_ptr = Farload_Buffer_Library_(file_name, buffer_seg_id, static_cast<uint8_t*>(buffer_space_seg));
    segment.is_loaded = 1;

    ++file_entry.segment_count;
    ++buffer::_file_buffer_count;

    return file_entry.segments[file_entry.segment_count - 1].data_ptr;
  }

  void* __cdecl Farload_Buffer_Library_(const char* file_name, int32_t entry_index, uint8_t* buffer_base) {
    char secondary_file_name[60]; // BUG: secondary_file_name is too small and can overflow
    int32_t entry_size;
    int32_t* used_bytes_ptr;
    int32_t victim_file_buffer_index;
    int32_t victim_segment_index;
    int32_t next_file_buffer_index;
    int32_t next_segment_index;
    int32_t freed_size;
    void* loaded_data;
    bool reuse_current_file;

    if (entry_index < 0) {
      farload::Error_Handler_(file_name, 4, entry_index);
    }

    reuse_current_file = false;
    if (farload::_farload_file_flag != 0) {
      if (_strcmpi(file_name, farload::_farload_file_name) == 0) {
        reuse_current_file = true;
      } else if (farload::_farload_file_flag != 0) {
        fclose(farload::_farload_fptr);
      }
    }

    if (!reuse_current_file) {
      strcpy(farload::_farload_file_name, file_name);
      farload::_farload_fptr = fopen(file_name, s_rb_0055117c);

      if (farload::_farload_fptr == nullptr) {
        if (farload::_secondary_drive_path[0] == '\0') {
          farload::Error_Handler_(file_name, 1, entry_index);
        }

        strcpy(secondary_file_name, farload::_secondary_drive_path);
        strcat(secondary_file_name, farload::_farload_file_name);
        farload::_farload_fptr = fopen(secondary_file_name, s_rb_0055117c);

        if (farload::_farload_fptr == nullptr) {
          farload::Error_Handler_(file_name, 1, entry_index);
        }
      }

      fread(&farload::_lbx_header, 0x800, 1, farload::_farload_fptr);

      if (farload::_lbx_header.magic != 0xFEAD) {
        farload::Error_Handler_(file_name, 7, entry_index);
      }

      farload::_farload_file_flag = 1;
    }

    if (static_cast<int16_t>(farload::_lbx_header.entry_count) <= entry_index) {
      farload::Error_Handler_(file_name, 8, entry_index);
    }

    entry_size = static_cast<int32_t>(
        farload::_lbx_header.offsets[entry_index + 1] -
        farload::_lbx_header.offsets[entry_index]);

    fseek(
        farload::_farload_fptr,
        static_cast<long>(farload::_lbx_header.offsets[entry_index]),
        0);

    used_bytes_ptr = reinterpret_cast<int32_t*>(buffer_base - 0x0C);
    loaded_data = allocate::Allocate_Next_Block_No_Header_(buffer_base, entry_size);

    while (loaded_data == nullptr && *used_bytes_ptr > 0) {
      loaded_data = allocate::Allocate_Next_Block_No_Header_(buffer_base, entry_size);

      if (loaded_data == nullptr) {
        buffer::Get_Lowest_Priority_(&victim_file_buffer_index, &victim_segment_index);
        buffer::Get_Next_Buffer_Address_(
            victim_file_buffer_index,
            victim_segment_index,
            &next_file_buffer_index,
            &next_segment_index,
            buffer_base);

        uint8_t* used_end = buffer_base + *used_bytes_ptr;
        uint8_t* next_data_ptr = used_end;

        if (next_file_buffer_index != 1000) {
          next_data_ptr = static_cast<uint8_t*>(
              buffer::_file_buffer[next_file_buffer_index]
              .segments[next_segment_index]
              .data_ptr);
        }

        uint8_t* victim_data_ptr = static_cast<uint8_t*>(
            buffer::_file_buffer[victim_file_buffer_index]
            .segments[victim_segment_index]
            .data_ptr);

        freed_size = static_cast<int32_t>(next_data_ptr - victim_data_ptr);

        if (next_file_buffer_index != 1000) {
          const int32_t bytes_to_move =
              static_cast<int32_t>(used_end - next_data_ptr);

          struct_::Copy_Structure_(victim_data_ptr, next_data_ptr, bytes_to_move);

          if (buffer::_file_buffer_count > 0) {
            for (int32_t file_buffer_index = 0;
                file_buffer_index < static_cast<int16_t>(buffer::_file_buffer_count);
                ++file_buffer_index) {
              if (buffer::_file_buffer[file_buffer_index].segment_count > 0) {
                for (int32_t segment_index = 0;
                    segment_index < buffer::_file_buffer[file_buffer_index].segment_count;
                    ++segment_index) {
                  s_buffer_cached_segment& segment =
                      buffer::_file_buffer[file_buffer_index].segments[segment_index];

                  if (segment.is_loaded == 1) {
                    uint8_t* segment_data_ptr =
                        static_cast<uint8_t*>(segment.data_ptr);

                    if (victim_data_ptr < segment_data_ptr) {
                      segment.data_ptr = segment_data_ptr - freed_size;
                    }
                  }
                }
              }
            }
          }
        }

        buffer::_file_buffer[victim_file_buffer_index]
            .segments[victim_segment_index]
            .is_loaded = 0;

        *used_bytes_ptr -= freed_size;
      }
    }

    if (loaded_data == nullptr) {
      farload::Error_Handler_(file_name, 5, entry_index);
    }

    fread(loaded_data, static_cast<size_t>(entry_size), 1, farload::_farload_fptr);
    return loaded_data;
  }

  void __cdecl Get_Next_Buffer_Address_(int32_t current_buffer_index,
                                        int32_t current_segment_index,
                                        int32_t* next_buffer_index,
                                        int32_t* next_segment_index,
                                        uint8_t* buffer_data) {
    uint8_t* closest_address =
            buffer_data + *reinterpret_cast<int32_t*>(buffer_data - 0x0C);
    const std::uintptr_t current_segment_address =
            reinterpret_cast<std::uintptr_t>(
                    buffer::_file_buffer[current_buffer_index]
                            .segments[current_segment_index]
                            .data_ptr);

    *next_buffer_index = 1000;

    if (buffer::_file_buffer_count > 0) {
      for (int32_t file_buffer_index = 0;
           file_buffer_index < buffer::_file_buffer_count;
           ++file_buffer_index) {
        s_file_buffer_entry& file_buffer_entry =
                buffer::_file_buffer[file_buffer_index];

        if (file_buffer_entry.segment_count > 0) {
          for (int32_t segment_index = 0;
               segment_index < file_buffer_entry.segment_count;
               ++segment_index) {
            if (file_buffer_entry.segments[segment_index].is_loaded == 1) {
              uint8_t* segment_data = reinterpret_cast<uint8_t*>(
                      file_buffer_entry.segments[segment_index].data_ptr);

              if ((current_segment_address <
                   reinterpret_cast<std::uintptr_t>(segment_data)) &&
                  (reinterpret_cast<std::uintptr_t>(segment_data) <
                   reinterpret_cast<std::uintptr_t>(closest_address))) {
                *next_buffer_index = file_buffer_index;
                *next_segment_index = segment_index;
                closest_address = segment_data;
              }
            }
          }
        }
      }
    }
  }
}