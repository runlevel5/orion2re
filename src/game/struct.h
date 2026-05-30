#pragma once

namespace struct_ {
    void __cdecl Clear_Structure_(void* s, uint32_t len);

    void __cdecl Set_Memory_(void* dest, uint32_t size, uint8_t fill);

    void __cdecl Copy_Structure_(void* dest, void* src, uint32_t size);
    void __cdecl Copy_Structure_Aligned_(void* dest, void* src, uint32_t size);
    int16_t __cdecl Test_Bit_Field_(void* field, uint16_t bit_index);
    void __cdecl Set_Bit_Field_(void* field, uint16_t bit);
    void __cdecl Clear_Bit_Field_(void* bit_field, uint16_t bit_index);

    void __cdecl Swap_Structure_(void* left_ptr, void* right_ptr, uint32_t byte_count);
}