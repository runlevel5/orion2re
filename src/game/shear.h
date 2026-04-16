#pragma once
#include <cstdint>

namespace shear {
    inline void __cdecl Outline_Bitmap_Pixels_(uint8_t* src, int16_t outline_color, int32_t param_4, uint8_t* dest, int16_t width, int16_t height) {
        using Fn = void(__cdecl*)(uint8_t*, int16_t, int32_t, uint8_t*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537701});
        fn(src, outline_color, param_4, dest, width, height);
    }

    inline void __cdecl Outline_Bitmap_Pixels_No_Glass_(uint8_t* src, int16_t outline_color, int32_t param_4, uint8_t* dest, int16_t width, int16_t height) {
        using Fn = void(__cdecl*)(uint8_t*, int16_t, int32_t, uint8_t*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005377F8});
        fn(src, outline_color, param_4, dest, width, height);
    }

    void __cdecl Capture_Screen_Block_(uint8_t* src, int32_t width, int32_t height, uint8_t* dst);
    void __cdecl Flip_Bitmap_Down_Different_(uint8_t* src, int32_t width, int32_t height, uint8_t* dst);
    void __cdecl Flip_Bitmap_Down_Same_(uint8_t* data, uint32_t width, uint32_t height);
    void __cdecl Flip_Bitmap_Left_Different_(uint8_t* src, uint32_t width, uint32_t height, uint8_t* dst);
    void __cdecl Flip_Bitmap_Left_Same_(uint8_t* pixels, uint32_t width, uint32_t height);

    inline void __cdecl Interlace_Scale_Off_Page_ASM_(uint8_t* video_page, uint8_t* palette_color_list) {
        using Fn = void(__cdecl*)(uint8_t*, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537EC9});
        fn(video_page, palette_color_list);
    }

    inline void __cdecl Mask_Bitmap_Pixels_(uint8_t* src_ptr, int src_skip, int dest_skip, uint8_t* dest_ptr, int width, int height) {
        using Fn = void(__cdecl*)(uint8_t*, int, int, uint8_t*, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537B62});
        fn(src_ptr, src_skip, dest_skip, dest_ptr, width, height);
    }

    inline void __cdecl Mask_Bitmap_Pixels_Inverse_(uint8_t* src_ptr, int src_skip, int dest_skip, uint8_t* dest_ptr, int width, int height) {
        using Fn = void(__cdecl*)(uint8_t*, int, int, uint8_t*, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537C2F});
        fn(src_ptr, src_skip, dest_skip, dest_ptr, width, height);
    }

    inline void __cdecl Mask_Brighten_Bitmap_Pixels_(uint8_t* src, int32_t src_skip, int32_t dst_skip, uint8_t* dst, int32_t width, int32_t height) {
        using Fn = void(__cdecl*)(uint8_t*, int32_t, int32_t, uint8_t*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537bb5});
        fn(src, src_skip, dst_skip, dst, width, height);
    }

    inline void __cdecl Mask_Bitmap_Colors_Pixels_(uint8_t* pixels, int16_t param_2, int16_t param_3, int16_t param_4, int16_t width, int16_t height) {
        using Fn = void(__cdecl*)(uint8_t*, int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537c82});
        fn(pixels, param_2, param_3, param_4, width, height);
    }

    inline void __cdecl Enlarge_Bitmap_(uint8_t* src_ptr, int scale_x, int scale_y, uint8_t* dst_ptr, int src_w, int src_h, int dst_w, int dst_h) {
        using Fn = void(__cdecl*)(uint8_t*, int, int, uint8_t*, int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537475});
        fn(src_ptr, scale_x, scale_y, dst_ptr, src_w, src_h, dst_w, dst_h);
    }

    inline void __cdecl Reduce_Bitmap_(uint8_t* src_ptr, int scale_x, int scale_y, uint8_t* dst_ptr, int src_w, int src_h, int dst_w, int dst_h) {
        using Fn = void(__cdecl*)(uint8_t*, int, int, uint8_t*, int, int, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537561});
        fn(src_ptr, scale_x, scale_y, dst_ptr, src_w, src_h, dst_w, dst_h);
    }

    inline void __cdecl Vanish_Bitmap_Pixels_(uint8_t* src, int32_t param_3, int32_t zero, void* dest, int32_t width, int32_t height) {
        using Fn = void(__cdecl*)(uint8_t*, int32_t, int32_t, void*, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00537620 });
        fn(src, param_3, zero, dest, width, height);
    }

    inline void __cdecl Jumble_Bitmap_Pixels_(uint8_t* src_pixels, int16_t param_4, int16_t param_3, uint8_t* dest_pixels, int16_t width, int16_t height) {
        using Fn = void(__cdecl*)(uint8_t*, int16_t, int16_t, uint8_t*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00537ab1});
        fn(src_pixels, param_4, param_3, dest_pixels, width, height);
    }
}