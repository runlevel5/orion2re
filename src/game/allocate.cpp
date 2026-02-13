#include "deps.h"

namespace allocate {
    void* __cdecl Allocate_Space_(uint32_t size) {
        uint32_t padded_size = size;

        // Alignment logic: If size is odd, align it to the next 4-byte boundary.
        // Note: Even sizes (e.g., 2, 6) are not aligned to 4-byte boundaries here.
        if (size & 1) {
            padded_size = (size & 0xFFFFFFFC) + 4;
        }

        uint32_t total_alloc_size = padded_size + sizeof(s_alloc_header);

        uint32_t* block = static_cast<uint32_t*>(Allocate_Far_Space_(total_alloc_size));
        if (block == nullptr) {
            block = static_cast<uint32_t*>(Allocate_Dos_Space_(total_alloc_size));
            if (block == nullptr) {
                // Potential Bug: If Allocation_Error_ returns, the code will null-dereference 
                // when writing to the header fields.
                allocate::Allocation_Error_(size);
            }
        }

        s_alloc_header* header = reinterpret_cast<s_alloc_header*>(block);
        header->current = 0;
        header->size = size; // Stores the original requested size, not the padded size
        header->reserved2 = 0;

        // Return pointer to the data region following the header
        return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(block) + sizeof(s_alloc_header));
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
        MOX::_global_cache_seg = FUN_0052c200(0x174508);
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

        _sprintf(message_buffer, s_Couldn_t_Allocate_0055B668, bytes_requested);

        exitm::Exit_With_Message(message_buffer);
    }

    int32_t __cdecl Get_Remaining_Block_Space_(void* addr) {
        if (!addr) {
            return 0;
        }

        const auto block_addr = reinterpret_cast<std::uintptr_t>(addr);
        const auto block = reinterpret_cast<s_alloc_header*>(block_addr - 12);
        return block->size - block->current;
    }

    void __cdecl Reallocation_Error_(uint32_t size, const char* file, int line) {
        char buffer[100];

        // Format: "Unable to Allocate %d bytes (%s, %d)\n"
        _sprintf(buffer, s_Unable_to_Allocate_bytes, size, file, line);

        exitm::Exit_With_Message(buffer);
    }

    void* __cdecl Allocate_Next_Block_(void* block, uint32_t size) {
        uint32_t aligned_size = size;

        // Custom alignment: if the requested size is odd, align it to the next 4-byte boundary.
        // Note: Even sizes (like 2 or 6) remain unaligned to 4-byte boundaries.
        if ((size & 1) != 0) {
            aligned_size = (size & 0xFFFFFFFC) + 4;
        }

        uint8_t* pool_base = reinterpret_cast<uint8_t*>(block);

        // Pool metadata is stored in an AllocationHeader directly before the block pointer:
        // header->current : Current used offset within the pool
        // header->size    : Total capacity of the pool
        auto* pool_header = reinterpret_cast<s_alloc_header*>(pool_base - sizeof(s_alloc_header));

        int32_t current_offset = static_cast<int32_t>(pool_header->current);
        int32_t capacity = static_cast<int32_t>(pool_header->size);

        // Total required space for the new sub-block includes a 12-byte internal header
        // represented by AllocationHeader.
        int32_t total_required = current_offset + (int32_t)aligned_size + 12;

        if (total_required > capacity) {
            Reallocation_Error_(size, __FILE__, __LINE__);
        }

        // Pointer to the start of the new sub-block (header starts here)
        uint8_t* sub_block_start = pool_base + current_offset;

        // Advance the pool's allocation pointer for future allocations
        pool_header->current = static_cast<uint32_t>(total_required);

        // Initialize the sub-block header using AllocationHeader.
        auto* sub_block_header = reinterpret_cast<s_alloc_header*>(sub_block_start);
        sub_block_header->current = 0;
        sub_block_header->size = size;
        sub_block_header->reserved2 = 0;

        // Return a pointer to the sub-block's data area, skipping the 12-byte header.
        return reinterpret_cast<void*>(sub_block_start + 12);
    }

    void* __cdecl Allocate_First_Block_(void* block, uint32_t size) {
        // Zero out a field 12 bytes before the provided block pointer
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(block) - 0x0C) = 0;

        return Allocate_Next_Block_(block, size);
    }

    /**
     * @brief Allocates space with a 12-byte header, aligning odd sizes to the next 4-byte boundary.
     * @param size The requested memory size in bytes.
     * @return Pointer to the usable memory area (skipping the 12-byte header).
     */
    void* __cdecl Allocate_Dos_Space_(uint32_t size) {
        uint32_t allocation_size = size;

        // If the size is odd, align it to the next 4-byte boundary
        if ((size & 1) != 0) {
            allocation_size = (size & 0xFFFFFFFC) + 4;
        }

        // Allocate block with 12 bytes of overhead for metadata
        uint32_t* block = static_cast<uint32_t*>(Allocate_Far_Space_(allocation_size + 12));

        if (block == nullptr) {
            Allocation_Error_(size);
            // Allocation_Error_ is expected to terminate execution.
        }

        // Initialize metadata header
        block[0] = 0;
        block[1] = size;
        block[2] = 0;

        // Return pointer to memory after the 12-byte header
        return &block[3];
    }

    /**
     * Allocates a block of memory and terminates the application if allocation fails.
     * @param size Number of bytes to allocate.
     * @return Pointer to the allocated memory block.
     */
    void* __cdecl Allocate_Far_Space_(size_t size) {
        void* ptr = ::_malloc(size);

        if (ptr == nullptr) {
            exitm::Exit_With_Message(s_Null_Pointer_0055b658);
        }

        return ptr;
    }

    void __cdecl Push_Block_(void* block) {
        if (_mark_stack_pos[0] >= 40) {
            exitm::Exit_With_Message(s_Mark_Block_Stack_Size_Exceeded_0055b6dc);
        }

        uint32_t pos = _mark_stack_pos[0];
        _mark_stack[pos][0] = reinterpret_cast<uint32_t>(block);

        // Accessing metadata at block - 12 bytes
        uint32_t metadata = *reinterpret_cast<uint32_t*>(reinterpret_cast<uintptr_t>(block) - 0xC);
        _mark_stack[pos][1] = metadata;

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

        // Restore the value at the calculated offset relative to the block address
        *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(target_base) - 0x0C) = original_value;
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
        // Clear a field at offset -12 relative to the block pointer
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(block_ptr) - 0xc) = 0;

        return Allocate_Next_Block_No_Header_(block_ptr, size);
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
        auto* pool_header = reinterpret_cast<s_alloc_header*>(base_ptr - sizeof(s_alloc_header));

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
}