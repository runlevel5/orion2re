#include "pch.h"

namespace struct_ {
    void __cdecl Set_Memory_(void* dest, int32_t size, int32_t value) {
        memset(dest, value, size);
    }

    void __cdecl Clear_Structure_(void* s, uint32_t len) {
        if (len == 0) {
            return;
        }

        // TODO: Just use memset
        uint8_t* ptr = static_cast<uint8_t*>(s);

        // Align to 2-byte boundary if pointer is odd
        if (reinterpret_cast<std::uintptr_t>(ptr) & 1) {
            *ptr++ = 0;
            len--;
        }

        // Fill memory in 4-byte chunks
        uint32_t* dword_ptr = reinterpret_cast<uint32_t*>(ptr);
        uint32_t dword_count = len >> 2;
        for (uint32_t i = 0; i < dword_count; ++i) {
            dword_ptr[i] = 0;
        }

        // Fill remaining bytes (0-3 bytes)
        uint8_t* tail_ptr = reinterpret_cast<uint8_t*>(&dword_ptr[dword_count]);
        uint32_t remaining_count = len & 3;
        for (uint32_t i = 0; i < remaining_count; ++i) {
            tail_ptr[i] = 0;
        }
    }

    void __cdecl Copy_Structure_Aligned_(void* dest, void* src, uint32_t size) {
        memcpy(dest, src, (size >> 2) * 4);
    }

    void __cdecl Copy_Structure_(void* dest, const void* src, uint32_t size) {
        // TODO: Just use memcpy
        uint8_t* d_ptr = static_cast<uint8_t*>(dest);
        const uint8_t* s_ptr = static_cast<const uint8_t*>(src);

        std::uintptr_t u_src = reinterpret_cast<std::uintptr_t>(s_ptr);
        std::uintptr_t u_dest = reinterpret_cast<std::uintptr_t>(d_ptr);

        if ((u_src & 1) == (u_dest & 1)) {
            if ((u_src & 1) != 0) {
                if (size > 0) {
                    *d_ptr++ = *s_ptr++;
                    size--;
                }
            }

            uint32_t dword_count = size >> 2;
            for (uint32_t i = 0; i < dword_count; ++i) {
                *reinterpret_cast<uint32_t*>(d_ptr) = *reinterpret_cast<const uint32_t*>(s_ptr);
                d_ptr += 4;
                s_ptr += 4;
            }

            uint32_t remaining_bytes = size & 3;
            for (uint32_t i = 0; i < remaining_bytes; ++i) {
                *d_ptr++ = *s_ptr++;
            }
        }
        else {
            for (uint32_t i = 0; i < size; ++i) {
                *d_ptr++ = *s_ptr++;
            }
        }
    }

    bool __cdecl Test_Bit_Field_(void* field, uint16_t bit_index) {
        const uint8_t* data = reinterpret_cast<const uint8_t*>(field);
        uint32_t byte_offset = bit_index >> 3;
        uint32_t bit_in_byte = bit_index & 7;

        return (data[byte_offset] >> bit_in_byte) & 1;
    }

    void __cdecl Set_Bit_Field_(void* field, uint16_t bit) {
        uint8_t* data = reinterpret_cast<uint8_t*>(field);
        // Calculate byte index and bit position within that byte
        data[bit >> 3] |= static_cast<uint8_t>(1 << (bit & 0x7));
    }

    void __cdecl Clear_Bit_Field_(void* bit_field, int16_t bit_index) {
        uint8_t* field_ptr = reinterpret_cast<uint8_t*>(bit_field);

        // Bit index is treated as unsigned 16-bit to calculate byte offset.
        uint16_t unsigned_index = static_cast<uint16_t>(bit_index);
        uint32_t byte_offset = unsigned_index >> 3;
        uint32_t bit_in_byte = unsigned_index & 7;

        field_ptr[byte_offset] &= ~(static_cast<uint8_t>(1 << bit_in_byte));
    }

    void __cdecl Swap_Structure_(void* left_ptr, void* right_ptr, uint32_t byte_count) {
        uint8_t* left = static_cast<uint8_t*>(left_ptr);
        uint8_t* right = static_cast<uint8_t*>(right_ptr);

        if ((reinterpret_cast<std::uintptr_t>(right_ptr) & 1U) != 0) {
            if ((reinterpret_cast<std::uintptr_t>(left_ptr) & 1U) == 0) {
                uint32_t count = byte_count;
                do {
                    uint8_t temp = *right;
                    *right++ = *left;
                    *left++ = temp;
                } while (--count != 0);
                return;
            }

            {
                uint8_t temp = *right;
                *right++ = *left;
                *left++ = temp;
            }

            byte_count -= 1;

            uint32_t dword_count = byte_count >> 2;
            if (dword_count != 0) {
                do {
                    uint32_t temp = *reinterpret_cast<uint32_t*>(right);
                    *reinterpret_cast<uint32_t*>(right) = *reinterpret_cast<uint32_t*>(left);
                    *reinterpret_cast<uint32_t*>(left) = temp;
                    right += 4;
                    left += 4;
                } while (--dword_count != 0);
            }

            uint32_t tail_count = byte_count & 3;
            if (tail_count != 0) {
                do {
                    uint8_t temp = *right;
                    *right++ = *left;
                    *left++ = temp;
                } while (--tail_count != 0);
            }

            return;
        }

        if ((reinterpret_cast<std::uintptr_t>(left_ptr) & 1U) != 0) {
            uint32_t count = byte_count;
            do {
                uint8_t temp = *right;
                *right++ = *left;
                *left++ = temp;
            } while (--count != 0);
            return;
        }

        uint32_t dword_count = byte_count >> 2;
        if (dword_count != 0) {
            do {
                uint32_t temp = *reinterpret_cast<uint32_t*>(right);
                *reinterpret_cast<uint32_t*>(right) = *reinterpret_cast<uint32_t*>(left);
                *reinterpret_cast<uint32_t*>(left) = temp;
                right += 4;
                left += 4;
            } while (--dword_count != 0);
        }

        uint32_t tail_count = byte_count & 3;
        if (tail_count != 0) {
            do {
                uint8_t temp = *right;
                *right++ = *left;
                *left++ = temp;
            } while (--tail_count != 0);
        }
    }
}