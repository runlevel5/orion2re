#include "pch.h"

namespace allocate {
    int32_t _mark_stack_pos[2];

    s_mark_stack_entry _mark_stack[39];

    void* __cdecl Allocate_Space_(uint32_t size) {
        uint32_t padded_size = size;

        // Alignment logic: If size is odd, align it to the next 4-byte boundary.
        // Note: Even sizes (e.g., 2, 6) are not aligned to 4-byte boundaries here.
        if (size & 1) {
            padded_size = (size & 0xFFFFFFFC) + 4;
        }

        uint32_t total_alloc_size = padded_size + sizeof(s_alloc_header);

        auto* header = (s_alloc_header*)Allocate_Far_Space_(total_alloc_size);
        if (header == nullptr) {
            header = (s_alloc_header*)Allocate_Dos_Space_(total_alloc_size);

            if (header == nullptr) {
                Allocation_Error_(size);
            }
        }

        header->current = 0;
        header->size = size; // Stores the original requested size, not the padded size
        header->reserved2 = 0;

        // Return pointer to the data region following the header
        return header + 1;
    }

    void Allocate_Data_Space_() {
        MOX::_global_screen_seg = Allocate_Space_(0x1B5030);
        MOX::_techname_seg = Allocate_Space_(30000);
        MOX::_special_names_seg = Allocate_Space_(1000);

        MOX::_planet = (s_planet_data*)Allocate_Space_(MAX_PLANETS * sizeof(s_planet_data));
        struct_::Clear_Structure_(MOX::_planet, MAX_PLANETS * sizeof(s_planet_data));

        MOX::_player = (s_player*)Allocate_Space_(MAX_PLAYERS * sizeof(s_player));
        struct_::Clear_Structure_(MOX::_player, MAX_PLAYERS * sizeof(s_player));

        MOX::_colony = (s_colony*)Allocate_Space_(MAX_COLONIES * sizeof(s_colony));
        struct_::Clear_Structure_(MOX::_colony, MAX_COLONIES * sizeof(s_colony));

        MOX::_nebula_screen_seg = Allocate_Space_(0x3E800);

        // Global cache allocation
        MOX::_global_cache_seg = buffer::Allocate_Buffer_Space_(0x174508);
        MOX::_global_cache_size = 0x174508;

        // DOS-space allocations for low-level segments
        MOX::_global_data_seg = Allocate_Dos_Space_(0x2800);

        MOX::_nebula = (s_nebula*)Allocate_Dos_Space_(MAX_NEBULAS * sizeof(s_nebula));
        struct_::Clear_Structure_(MOX::_nebula, MAX_NEBULAS * sizeof(s_nebula));

        MOX::_star = (s_star_data*)Allocate_Dos_Space_(MAX_STARS * sizeof(s_star_data));
        struct_::Clear_Structure_(MOX::_star, MAX_STARS * sizeof(s_star_data));

        MOX::_ship = (s_ship_data*)Allocate_Dos_Space_(MAX_SHIPS * sizeof(s_ship_data));
        struct_::Clear_Structure_(MOX::_ship, MAX_SHIPS * sizeof(s_ship_data));

        MOX::_leaders = (s_leader_data*)Allocate_Dos_Space_(MAX_LEADERS * sizeof(s_leader_data));
        struct_::Clear_Structure_(MOX::_leaders, MAX_LEADERS * sizeof(s_leader_data));
    }

    void __cdecl Allocation_Error_(int32_t requested_bytes) {
        char message_buffer[100];

        sprintf(message_buffer, s_Couldn_t_Allocate_0055B668, requested_bytes);

        exitm::Exit_With_Message(message_buffer);
    }

    int32_t __cdecl Get_Remaining_Block_Space_(void* block) {
        const auto* header = (s_alloc_header*)block - 1;
        return header->size - header->current;
    }

    uint32_t __cdecl Get_Max_Block_Space_(void* block) {
        return ((s_alloc_header*)block - 1)->size;
    }

    void __cdecl Reallocation_Error_(uint32_t requested_size, int32_t bytes_short, int32_t block_capacity) {
        char buffer[100];

        sprintf(buffer, s_Unable_to_Allocate_bytes, requested_size, bytes_short, block_capacity);

        exitm::Exit_With_Message(buffer);
    }

    void* __cdecl Allocate_Next_Block_(void* block, uint32_t size) {
        uint32_t aligned_size = size;

        // Custom alignment: if the requested size is odd, align it to the next 4-byte boundary.
        // Note: Even sizes (like 2 or 6) remain unaligned to 4-byte boundaries.
        if ((size & 1) != 0) {
            aligned_size = (size & 0xFFFFFFFCu) + 4u;
        }

        s_alloc_header* pool_header = (s_alloc_header*)block - 1;

        uint32_t current_offset = pool_header->current;
        int32_t capacity = (int32_t)pool_header->size;

        // Total required space for the new sub-block includes a 12-byte internal header.
        int32_t total_required =
            (int32_t)(current_offset + aligned_size + sizeof(s_alloc_header));

        if (total_required > capacity) {
            Reallocation_Error_(
                size,
                (int32_t)(current_offset - pool_header->size + size),
                capacity);
        }

        // Pointer to the start of the new sub-block (header starts here)
        uint8_t* pool_base = (uint8_t*)block;
        s_alloc_header* sub_block_header = (s_alloc_header*)(pool_base + current_offset);

        // Advance the pool's allocation pointer for future allocations
        pool_header->current = current_offset + aligned_size + sizeof(s_alloc_header);

        // Initialize the sub-block header.
        sub_block_header->current = 0;
        sub_block_header->size = size;
        sub_block_header->reserved2 = 0;

        // Return a pointer to the sub-block's data area, skipping the header.
        return sub_block_header + 1;
    }

    void* __cdecl Allocate_First_Block_(void* block, uint32_t size) {
        ((s_alloc_header*)block - 1)->current = 0;

        return Allocate_Next_Block_(block, size);
    }

    void* __cdecl Allocate_Dos_Space_(uint32_t size) {
        uint32_t allocation_size = size;

        if ((size & 1) != 0) {
            allocation_size = (size & 0xFFFFFFFC) + 4;
        }

        s_alloc_header* header = (s_alloc_header*)Allocate_Far_Space_(
            allocation_size + sizeof(s_alloc_header));

        if (header == nullptr) {
            Allocation_Error_(size);
        }

        header->current = 0;
        header->size = size;
        header->reserved2 = 0;

        return header + 1;
    }

    void* __cdecl Allocate_Far_Space_(uint32_t size) {
        void* ptr = ::malloc(size);

        if (ptr == nullptr) {
            exitm::Exit_With_Message(s_Null_Pointer_0055b658);
        }

        return ptr;
    }

    void __cdecl Push_Block_(void* block) {
        if (_mark_stack_pos[0] >= 40) {
            exitm::Exit_With_Message("Mark Block Stack Size Exceeded");
        }

        int32_t pos = _mark_stack_pos[0];
        _mark_stack[pos].block = block;

        _mark_stack[pos].current = ((s_alloc_header*)block - 1)->current;

        _mark_stack_pos[0]++;
    }

    void __cdecl Pop_Block_(void* unused_block) {
        (void)unused_block;

        if (_mark_stack_pos[0] < 1) {
            exitm::Exit_With_Message("Mark Block Stack Empty");
        }

        _mark_stack_pos[0]--;
        int32_t index = _mark_stack_pos[0];

        void* target_base = _mark_stack[index].block;
        uint32_t original_value = _mark_stack[index].current;

        ((s_alloc_header*)target_base - 1)->current = original_value;
    }

    void* __cdecl Allocate_Space_No_Header_(uint32_t size) {
        void* ptr = Allocate_Far_Space_(size);

        if (ptr == nullptr) {
            ptr = Allocate_Dos_Space_(size);

            if (ptr == nullptr) {
                Allocation_Error_(size);
            }
        }

        return ptr;
    }

    void* __cdecl Allocate_First_Block_No_Header_(void* block_ptr, uint32_t size) {
        ((s_alloc_header*)block_ptr - 1)->current = 0;

        return Allocate_Next_Block_No_Header_(block_ptr, size);
    }

    void __cdecl Mark_Block_(void* block_ptr) {
        auto* header = (s_alloc_header*)block_ptr - 1;
        header->reserved2 = header->current;
    }

    void* __cdecl Allocate_Next_Block_No_Header_(void* block_ptr, uint32_t size) {
        auto* pool_header = (s_alloc_header*)block_ptr - 1;

        uint32_t aligned_size = size;

        // Alignment logic: if size is odd, align it to the next 4-byte boundary.
        // Even sizes are used as-is.
        if ((size & 1) != 0) {
            aligned_size = (size & 0xFFFFFFFC) + 4;
        }

        uint32_t current_offset = pool_header->current;
        uint32_t next_offset = current_offset + aligned_size;

        if ((int32_t)pool_header->size < (int32_t)next_offset) {
            return nullptr;
        }

        void* new_block_ptr = (uint8_t*)block_ptr + current_offset;

        pool_header->current = next_offset;

        auto* sub_block_header = (s_alloc_header*)new_block_ptr;
        sub_block_header->current = 0;
        sub_block_header->size = size;
        sub_block_header->reserved2 = 0;

        return new_block_ptr;
    }

    void __cdecl Release_Block_(void* block) {
        auto* header = (s_alloc_header*)block - 1;
        header->current = header->reserved2;
    }
}