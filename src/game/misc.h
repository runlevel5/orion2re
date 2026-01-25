#pragma once

namespace MISC {
    inline uint8_t& _mouse_clock_image_num =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566578});

    inline uint8_t* _mouse_list_clock =
        reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558250});

    void __cdecl Increment_Mouse_Clock_Image_();

    struct ColorPair {
        uint32_t c1;
        uint32_t c2;
    };

    inline ColorPair* __cdecl Font_Colors2_(int32_t a1, int32_t a2, int32_t a3) {
        using Fn = ColorPair*(__cdecl*)(int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049ddb0});
        return fn(a1, a2, a3);
    }

    inline void __cdecl Draw_Advancing_Fill_Message_(int32_t a1, void* ctr, uint16_t max, ColorPair* c1, ColorPair* c2, int32_t x, int32_t y, char* msg, void* bitmap, int32_t flag) {
        using Fn = void(__cdecl*)(int32_t, void*, uint16_t, ColorPair*, ColorPair*, int32_t, int32_t, char*, void*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049e3f0});
        fn(a1, ctr, max, c1, c2, x, y, msg, bitmap, flag);
    }
}