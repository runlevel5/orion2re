#pragma once
#include <cstdint>

namespace swap {
    extern int32_t _swap_block_size;
    extern uint8_t* _swap_block_address;

    void __cdecl Save_Block_To_Drive_(uint8_t* block_address, int32_t block_size);
    void __cdecl Restore_Block_From_Drive_();
}