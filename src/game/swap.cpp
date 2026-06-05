#include "pch.h"

namespace swap {

int32_t _swap_block_size;
uint8_t* _swap_block_address;

void __cdecl Save_Block_To_Drive_(uint8_t* block_address, int32_t block_size) {
    auto* block_header = (allocate::s_alloc_header*)block_address - 1;
    int32_t saved_block_size = block_size + (int32_t)sizeof(*block_header);
    FILE* swap_file;

    swap::_swap_block_address = block_address;
    swap::_swap_block_size = saved_block_size + (int32_t)sizeof(*block_header);

    if ((int32_t)block_header->size < swap::_swap_block_size) {
        exitm::Exit_With_Message(s_Drive_swap_reqd_too_large_for_bl_0055befc);
    }

    if (dos::Disk_Free_() < swap::_swap_block_size) {
        exitm::Exit_With_Message(s_Insufficient_disk_space_for_swap_0055bed8);
    }

    swap_file = fopen("TEMP.TMP", "wb");
    if (swap_file == nullptr) {
        exitm::Exit_With_Message(s_Error_in_swap_file_0055beb8);
    }

    fwrite(block_header, swap::_swap_block_size, 1, swap_file);
    fclose(swap_file);

    block_header->size = (uint32_t)saved_block_size;
}

void __cdecl Restore_Block_From_Drive_() {
    uint8_t* swap_block_address = swap::_swap_block_address;
    auto* block_header = (allocate::s_alloc_header*)swap_block_address - 1;
    FILE* swap_file = fopen("TEMP.TMP", "rb");

    if (swap_file == nullptr) {
        exitm::Exit_With_Message(s_Error_in_swap_file_0055beb8);
    }

    fread(block_header, (uint32_t)swap::_swap_block_size, 1, swap_file);
    fclose(swap_file);
}

}