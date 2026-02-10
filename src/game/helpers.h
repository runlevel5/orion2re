#pragma once
#include <cstddef>
#include <cstdint>

inline size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

inline char* strcpy(char* dest, const char* src) {
    char* ret = dest;
    while ((*dest++ = *src++));
    return ret;
}

inline char* __cdecl _strncpy(char* dest, const char* src, size_t n) {
    char* ret = dest;
    while (n && (*dest++ = *src++)) --n;
    while (n--) *dest++ = '\0';
    return ret;
}

#define strncpy _strncpy

inline char* strcat(char* dest, const char* src) {
    char* ret = dest;

    while (*dest) dest++;
    while ((*dest++ = *src++));

    return ret;
}

inline int strcmp(const char* s1, const char* s2)
{
    while (*s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
    }

    return static_cast<unsigned char>(*s1) - static_cast<unsigned char>(*s2);
}

inline void* memcpy(void* dst, const void* src, size_t size)
{
    auto* d = static_cast<std::uint8_t*>(dst);
    auto* s = static_cast<const std::uint8_t*>(src);

    // Align destination to 4 bytes for faster word copying
    while (size && (reinterpret_cast<std::uintptr_t>(d) & 3u)) {
        *d++ = *s++;
        --size;
    }

    // Copy 4 bytes at a time where possible
    auto* d32 = reinterpret_cast<std::uint32_t*>(d);
    auto* s32 = reinterpret_cast<const std::uint32_t*>(s);
    while (size >= 4) {
        *d32++ = *s32++;
        size -= 4;
    }

    // Tail bytes
    d = reinterpret_cast<std::uint8_t*>(d32);
    s = reinterpret_cast<const std::uint8_t*>(s32);
    while (size--) {
        *d++ = *s++;
    }

    return dst;
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
