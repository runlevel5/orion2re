#pragma once
#include <cstdint>

namespace regions {
    void __cdecl Fill_Column_(uint8_t color, uint8_t* surface, uint32_t fill_width, uint32_t fill_height);
    void __cdecl Fill_Region_(uint8_t color, uint8_t* surface, uint32_t width_blocks, uint32_t height);

    inline void __cdecl Tint_Fill_Column_(uint8_t* color_map, uint8_t* dest, int32_t width, int32_t height) {
        using Fn = void(__cdecl*)(uint8_t*, uint8_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00537121 });
        fn(color_map, dest, width, height);
    }

    inline void __cdecl Tint_Fill_Region_(uint8_t* color_map, uint8_t* dest, int32_t blocks, int32_t height) {
        using Fn = void(__cdecl*)(uint8_t*, uint8_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0053717A });
        fn(color_map, dest, blocks, height);
    }

    inline void __cdecl Interlaced_Fill_Column_(uint32_t pattern, uint8_t* ptr, int32_t width, int32_t height) {
        using Fn = void(__cdecl*)(uint32_t, uint8_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00536F71});
        fn(pattern, ptr, width, height);
    }

    inline void __cdecl Interlaced_Fill_Region_(uint32_t pattern, uint8_t* ptr, int32_t width, int32_t height) {
        using Fn = void(__cdecl*)(uint32_t, uint8_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00536FAE});
        fn(pattern, ptr, width, height);
    }

    inline void __cdecl Tri_Interlaced_Fill_Column_(uint32_t color, uint8_t* dest, int32_t width, int32_t height) {
        using Fn = void(__cdecl*)(uint32_t, uint8_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0053701e});
        fn(color, dest, width, height);
    }

    inline void __cdecl Tri_Interlaced_Fill_Region_(uint32_t color, uint8_t* dest, int32_t blocks, int32_t height) {
        using Fn = void(__cdecl*)(uint32_t, uint8_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537076});
        fn(color, dest, blocks, height);
    }
}