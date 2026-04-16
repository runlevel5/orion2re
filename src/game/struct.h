#pragma once

namespace struct_ {
    void __cdecl Clear_Structure_(void* s, uint32_t len);

    inline void __cdecl Set_Memory_(void* dest, int32_t size, int32_t value) {
        using Fn = void(__cdecl*)(void*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052CED8});
        fn(dest, size, value);
    }

    void __cdecl Copy_Structure_(void* dest, const void* src, uint32_t size);
    bool __cdecl Test_Bit_Field_(void* field, uint16_t bit_index);
    void __cdecl Set_Bit_Field_(void* field, uint16_t bit);
    void __cdecl Clear_Bit_Field_(void* bit_field, int16_t bit_index);
}