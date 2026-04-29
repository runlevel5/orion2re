#pragma once
#include <cstdint>

namespace swap {
    inline int32_t& _swap_block_size =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00587FE4});
    inline uint8_t*& _swap_block_address =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00587FE8});

    void __cdecl Save_Block_To_Drive_(uint8_t* block_address, int32_t block_size);
    void __cdecl Restore_Block_From_Drive_();
}