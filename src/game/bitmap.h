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

    inline void __cdecl Tint_Bitmap_(void* dst_bitmap, void* src_bitmap, int32_t tint_type) {
        using Fn = void(__cdecl*)(void*, void*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052f560});
        fn(dst_bitmap, src_bitmap, tint_type);
    }

    inline void __cdecl Clear_Mouse_Image_(int32_t image_idx) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052F240});
        fn(image_idx);
    }

    inline void __cdecl Vanish_Bitmap_(s_animation_header* dest, s_animation_header* src, int16_t intensity) {
        using Fn = void(__cdecl*)(s_animation_header*, s_animation_header*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0052F400})(dest, src, intensity);
    }

    inline void __cdecl Flip_Bitmap_(s_animation_header* dst, s_animation_header* src, int32_t flag) {
        using Fn = void(__cdecl*)(s_animation_header*, s_animation_header*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052EFB0});
        fn(dst, src, flag);
    }

    inline void __cdecl Outline_Bitmap_(s_animation_header* dest, s_animation_header* src, int16_t color, int16_t unk) {
        using Fn = void(__cdecl*)(s_animation_header*, s_animation_header*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052F4C0});
        fn(dest, src, color, unk);
    }

    inline void __cdecl Mask_Out_Bitmap_With_Bitmap_(int16_t unk1, int16_t unk2, s_animation_header* dest, s_animation_header* src, int16_t unk3) {
        using Fn = void(__cdecl*)(int16_t, int16_t, s_animation_header*, s_animation_header*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052F690});
        fn(unk1, unk2, dest, src, unk3);
    }
}