#pragma once

namespace bitmap {
    void __cdecl Get_Bitmap_Actual_Size_(s_animation_header* header, int16_t* out_min_x, int16_t* out_min_y, int16_t* out_actual_width, int16_t* out_actual_height);

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

    inline void __cdecl Draw_Picture_To_Bitmap_(s_animation_header* dest, void* src) {
        using Fn = void(__cdecl*)(s_animation_header*, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052F960});
        fn(dest, src);
    }

    inline void __cdecl Add_Picture_To_Bitmap_(int16_t x, int16_t y, s_animation_header* src, s_animation_header* dest) {
        using Fn = void(__cdecl*)(int16_t, int16_t, s_animation_header*, s_animation_header*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052FAC0});
        fn(x, y, src, dest);
    }
}