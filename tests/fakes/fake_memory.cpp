#include "pch.h"

#include <cstring>

namespace struct_ {
    void __cdecl Clear_Structure_(void* dest, uint32_t size) {
        if (dest != nullptr) {
            std::memset(dest, 0, size);
        }
    }

    void __cdecl Set_Memory_(void* dest, uint32_t size, uint8_t fill) {
        if (dest != nullptr) {
            std::memset(dest, fill, size);
        }
    }

    void __cdecl Copy_Structure_(void* dest, void* src, uint32_t size) {
        if (dest != nullptr && src != nullptr) {
            std::memcpy(dest, src, size);
        }
    }

    void __cdecl Copy_Structure_Aligned_(void* dest, void* src, uint32_t size) {
        if (dest != nullptr && src != nullptr) {
            std::memcpy(dest, src, size & ~3u);
        }
    }

    int16_t __cdecl Test_Bit_Field_(void* bit_field, uint16_t bit_index) {
        const auto* bytes = static_cast<const uint8_t*>(bit_field);
        return (bytes[bit_index / 8] >> (bit_index % 8)) & 1;
    }

    void __cdecl Set_Bit_Field_(void* bit_field, uint16_t bit_index) {
        auto* bytes = static_cast<uint8_t*>(bit_field);
        bytes[bit_index / 8] |= static_cast<uint8_t>(1u << (bit_index % 8));
    }

    void __cdecl Clear_Bit_Field_(void* bit_field, uint16_t bit_index) {
        auto* bytes = static_cast<uint8_t*>(bit_field);
        bytes[bit_index / 8] &= static_cast<uint8_t>(~(1u << (bit_index % 8)));
    }

    void __cdecl Swap_Structure_(void* left_ptr, void* right_ptr, uint32_t byte_count) {
        auto* left = static_cast<uint8_t*>(left_ptr);
        auto* right = static_cast<uint8_t*>(right_ptr);
        for (uint32_t i = 0; i < byte_count; ++i) {
            uint8_t tmp = left[i];
            left[i] = right[i];
            right[i] = tmp;
        }
    }
}