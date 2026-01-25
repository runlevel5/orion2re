#pragma once
#include <cstdint>

namespace animate {
    inline void __cdecl Draw_(int32_t x, int32_t y, void* resource) {
        using Fn = void(__cdecl*)(int32_t, int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00524490});
        fn(x, y, resource);
    }
}