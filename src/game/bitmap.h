#pragma once
#include <cstdint>

namespace bitmap {
    inline uint32_t __cdecl Get_Bitmap_Size_(int32_t width, int32_t height) {
        using Fn = uint32_t(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052EF80});
        return fn(width, height);
    }

    inline void __cdecl Create_Blank_Bitmap_(int32_t width, int32_t height, void* bitmap) {
        using Fn = void(__cdecl*)(int32_t, int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052EED0});
        fn(width, height, bitmap);
    }
}