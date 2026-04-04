#pragma once
#include <cstdint>

namespace shear {
    inline void __cdecl Interlace_Scale_Off_Page_ASM_(uint8_t* video_page, uint8_t* palette_color_list) {
        using Fn = void(__cdecl*)(uint8_t*, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537EC9});
        fn(video_page, palette_color_list);
    }
}