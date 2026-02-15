#pragma once

namespace bitmap {
    void __cdecl Get_Bitmap_Actual_Size_(s_animation_header* header, int16_t* out_min_x, int16_t* out_min_y, int16_t* out_actual_width, int16_t* out_actual_height);

    int32_t __cdecl Get_Bitmap_Size_(int16_t width, int16_t height);

    void __cdecl Create_Bitmap_Header_(int16_t width, int16_t height, s_animation_header* h);

    void __cdecl Create_Blank_Bitmap_(int16_t w, int16_t h, void* sprite);

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

    inline void __cdecl Screen_Bitmap_Capture_(int x, int y, int w, int h, void* bmp) {
        using Fn = void(__cdecl*)(int, int, int, int, void*);
        reinterpret_cast<Fn>(0x0052F0C0)(x, y, w, h, bmp);
    }

    inline void __cdecl Mask_Out_Colors_(void* handle, uint8_t color_low, uint8_t color_high) {
        using Fn = void(__cdecl*)(void*, uint8_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052f8a0});
        fn(handle, color_low, color_high);
    }
}