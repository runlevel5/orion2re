#pragma once
#include <cstdint>

namespace regions {
    void __cdecl Fill_Column_(uint8_t color, uint8_t* surface, uint32_t fill_width, uint32_t fill_height);

    inline void __cdecl Fill_Region_(uint8_t color, uint8_t* surface, uint8_t width_blocks, uint32_t height) {
        using Fn = void(__cdecl*)(uint8_t, uint8_t*, uint8_t, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00536F23 });
        fn(color, surface, width_blocks, height);
    }
}