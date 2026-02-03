#pragma once
#include <cstdint>

namespace accss {
    inline int32_t __cdecl _access_(const char* path, int32_t mode) {
        using Fn = int32_t(__cdecl*)(const char*, int32_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0054ea70})(path, mode);
    }
}