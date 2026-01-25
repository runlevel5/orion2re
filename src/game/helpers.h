#pragma once
#include <cstddef>
#include <cstdint>

namespace helpers {
    inline size_t strlen(const char* str) {
        size_t len = 0;
        while (str[len]) len++;
        return len;
    }

    inline void strcpy(char* dest, const char* src) {
        while ((*dest++ = *src++));
    }

    inline void strcat(char* dest, const char* src) {
        while (*dest) dest++;
        while ((*dest++ = *src++));
    }

    inline void* memset(void* dst, int value, size_t size)
    {
        auto* p = static_cast<std::uint8_t*>(dst);
        std::uint8_t v8 = static_cast<std::uint8_t>(value);

        // 0xvvvvvvvv dword pattern
        std::uint32_t v32 = 0x01010101u * v8;

        // Align to 4 bytes
        while (size && (reinterpret_cast<std::uintptr_t>(p) & 3u)) {
            *p++ = v8;
            --size;
        }

        // Fill 4 bytes at a time
        auto* p32 = reinterpret_cast<std::uint32_t*>(p);
        while (size >= 4) {
            *p32++ = v32;
            size -= 4;
        }

        // Tail bytes
        p = reinterpret_cast<std::uint8_t*>(p32);
        while (size--) {
            *p++ = v8;
        }

        return dst;
    }
}
