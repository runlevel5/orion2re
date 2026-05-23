#include "pch.h"

namespace allocate {
    void* __cdecl Allocate_Space_(uint32_t size) {
        uint32_t padded_size = size;

        // Alignment logic: If size is odd, align it to the next 4-byte boundary.
        // Note: Even sizes (e.g., 2, 6) are not aligned to 4-byte boundaries here.
        if (size & 1) {
            padded_size = (size & 0xFFFFFFFC) + 4;
        }

        uint32_t total_alloc_size = padded_size + sizeof(s_alloc_header);

        auto* header = static_cast<s_alloc_header*>(Allocate_Far_Space_(total_alloc_size));
        if (header == nullptr) {
            return Allocate_Dos_Space_(size);
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

        MOX::_planet = reinterpret_cast<s_planet_data*>(Allocate_Space_(0x17E8));
        struct_::Clear_Structure_(MOX::_planet, 0x17E8);

        MOX::_player = reinterpret_cast<s_player*>(Allocate_Space_(0x7548));
        struct_::Clear_Structure_(MOX::_player, 0x7548);

        MOX::_colony = reinterpret_cast<s_colony*>(Allocate_Space_(0x1608A));
        struct_::Clear_Structure_(MOX::_colony, 0x1608A);

        MOX::_nebula_screen_seg = Allocate_Space_(0x3E800);

        // Global cache allocation
        MOX::_global_cache_seg = buffer::Allocate_Buffer_Space_(0x174508);
        MOX::_global_cache_size = 0x174508;

        // DOS-space allocations for low-level segments
        MOX::_global_data_seg = Allocate_Dos_Space_(0x2800);

        MOX::_nebula = reinterpret_cast<s_nebula*>(Allocate_Dos_Space_(0x14));
        struct_::Clear_Structure_(MOX::_nebula, 0x14);

        MOX::_star = reinterpret_cast<s_star_data*>(Allocate_Dos_Space_(0x1FC8));
        struct_::Clear_Structure_(MOX::_star, 0x1FC8);

        MOX::_ship = reinterpret_cast<s_ship_data*>(Allocate_Dos_Space_(0xFBF4));
        struct_::Clear_Structure_(MOX::_ship, 0xFBF4);

        MOX::_leaders = reinterpret_cast<s_leader_data*>(Allocate_Dos_Space_(0xF71));
        struct_::Clear_Structure_(MOX::_leaders, 0xF71);
    }

    void __cdecl Allocation_Error_(int bytes_requested) {
        char message_buffer[100];

        sprintf(message_buffer, s_Couldn_t_Allocate_0055B668, bytes_requested);

        exitm::Exit_With_Message(message_buffer);
    }

    int32_t __cdecl Get_Remaining_Block_Space_(void* addr) {
        if (!addr) {
            return 0;
        }

        const auto* header = static_cast<s_alloc_header*>(addr) - 1;
        return header->size - header->current;
    }

    uint32_t __cdecl Get_Max_Block_Space_(void* ptr) {
        return (static_cast<s_alloc_header*>(ptr) - 1)->size;
    }

    void __cdecl Reallocation_Error_(uint32_t size, int32_t arg2, int32_t arg3) {
        char buffer[100];

        // Format: "Unable to Allocate %d bytes (%d, %d)\n"
        sprintf(buffer, s_Unable_to_Allocate_bytes, size, arg2, arg3);

        exitm::Exit_With_Message(buffer);
    }

    void* __cdecl Allocate_Next_Block_(void* block, uint32_t size) {
        uint32_t aligned_size = size;

        // Custom alignment: if the requested size is odd, align it to the next 4-byte boundary.
        // Note: Even sizes (like 2 or 6) remain unaligned to 4-byte boundaries.
        if ((size & 1) != 0) {
            aligned_size = (size & 0xFFFFFFFC) + 4;
        }

        auto* pool_header = static_cast<s_alloc_header*>(block) - 1;

        int32_t current_offset = static_cast<int32_t>(pool_header->current);
        int32_t capacity = static_cast<int32_t>(pool_header->size);

        // Total required space for the new sub-block includes a 12-byte internal header
        int32_t total_required = current_offset + (int32_t)aligned_size + sizeof(s_alloc_header);

        if (total_required > capacity) {
            Reallocation_Error_(size, (current_offset - capacity) + (int32_t)size, capacity);
        }

        // Pointer to the start of the new sub-block (header starts here)
        uint8_t* pool_base = static_cast<uint8_t*>(block);
        auto* sub_block_header = reinterpret_cast<s_alloc_header*>(pool_base + current_offset);

        // Advance the pool's allocation pointer for future allocations
        pool_header->current = static_cast<uint32_t>(total_required);

        // Initialize the sub-block header.
        sub_block_header->current = 0;
        sub_block_header->size = size;
        sub_block_header->reserved2 = 0;

        // Return a pointer to the sub-block's data area, skipping the header.
        return sub_block_header + 1;
    }

    void* __cdecl Allocate_First_Block_(void* block, uint32_t size) {
        (static_cast<s_alloc_header*>(block) - 1)->current = 0;

        return Allocate_Next_Block_(block, size);
    }

    void* __cdecl Allocate_Dos_Space_(uint32_t size) {
        uint32_t allocation_size = size;

        if ((size & 1) != 0) {
            allocation_size = (size & 0xFFFFFFFC) + 4;
        }

        auto* header = static_cast<s_alloc_header*>(
            Allocate_Far_Space_(allocation_size + sizeof(s_alloc_header)));

        if (header == nullptr) {
            Allocation_Error_(size);
        }

        header->current = 0;
        header->size = size;
        header->reserved2 = 0;

        return header + 1;
    }

    void* __cdecl Allocate_Far_Space_(uint32_t size) {
        return ::_malloc(size);
    }

    void __cdecl Push_Block_(void* block) {
        if (_mark_stack_pos[0] >= 40) {
            exitm::Exit_With_Message(s_Mark_Block_Stack_Size_Exceeded_0055b6dc);
        }

        uint32_t pos = _mark_stack_pos[0];
        _mark_stack[pos][0] = reinterpret_cast<uint32_t>(block);

        _mark_stack[pos][1] = (static_cast<s_alloc_header*>(block) - 1)->current;

        _mark_stack_pos[0]++;
    }

    /**
     * @brief Pops a block marker from the stack and restores the original value at the target address.
     * The target address is stored at _mark_stack[idx][0], and the value to restore is at _mark_stack[idx][1].
     * The restoration occurs at (target_address - 12).
     */
    void __cdecl Pop_Block_(void* block) {
        if (static_cast<int32_t>(_mark_stack_pos[0]) < 1) {
            exitm::Exit_With_Message(s_Mark_Block_Stack_Empty_0055b6fc);
        }

        _mark_stack_pos[0]--;
        uint32_t index = _mark_stack_pos[0];

        uint32_t target_base = _mark_stack[index][0];
        uint32_t original_value = _mark_stack[index][1];

        // Restore the value relative to the block address
        (reinterpret_cast<s_alloc_header*>(target_base) - 1)->current = original_value;
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
        (static_cast<s_alloc_header*>(block_ptr) - 1)->current = 0;

        return Allocate_Next_Block_No_Header_(block_ptr, size);
    }

    int32_t __cdecl Mark_Block_(void* block_ptr) {
        auto* header = static_cast<s_alloc_header*>(block_ptr) - 1;
        header->reserved2 = header->current;
        return reinterpret_cast<int32_t>(header);
    }

    /**
     * @brief Allocates a block of memory from a pool, aligning the size if it's odd.
     * 
     * @param block_ptr Start address of the memory pool. A header exists 12 bytes before this address.
     * @param size The requested size of the block.
     * @return void* Pointer to the newly allocated block, or nullptr if capacity is exceeded.
     */
    void* __cdecl Allocate_Next_Block_No_Header_(void* block_ptr, uint32_t size) {
        uint8_t* base_ptr = reinterpret_cast<uint8_t*>(block_ptr);
        auto* pool_header = static_cast<s_alloc_header*>(block_ptr) - 1;

        uint32_t aligned_size = size;

        // Alignment logic: if size is odd, align it to the next 4-byte boundary.
        // Even sizes are used as-is.
        if ((size & 1) != 0) {
            aligned_size = (size & 0xFFFFFFFC) + 4;
        }

        uint32_t current_offset = pool_header->current;
        uint32_t pool_capacity = pool_header->size;
        uint32_t next_offset = current_offset + aligned_size;

        // Capacity check
        if (next_offset > pool_capacity) {
            return nullptr;
        }

        // Pointer to the start of the new block within the pool.
        uint8_t* new_block_ptr = base_ptr + current_offset;

        // Update the current offset in the pool header.
        pool_header->current = next_offset;

        // Initialize the new block's internal structure (12 bytes).
        auto* new_block_header = reinterpret_cast<s_alloc_header*>(new_block_ptr);
        new_block_header->current = 0;
        new_block_header->size = size;   // Stores the original requested size
        new_block_header->reserved2 = 0;

        return static_cast<void*>(new_block_ptr);
    }

    void* __cdecl Release_Block_(void* ptr) {
        auto* header = static_cast<s_alloc_header*>(ptr) - 1;
        header->current = header->reserved2;
        return header;
    }
}