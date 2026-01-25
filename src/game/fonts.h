#pragma once
#include "misc.h"

namespace fonts {
    inline void*& _palette_block =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x005787B8 });

    inline char* const _font_name =
        reinterpret_cast<char*>(std::uintptr_t{ 0x005787C0 });

    void Load_Palette_(int16_t palette_id, int16_t start_index, int16_t end_index);

    inline void Set_PSTR_Style_(int32_t style) {
        using Fn = void(*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00526D00 });
        fn(style);
    }
    inline void __cdecl Set_Font_Style_(int32_t style, MISC::ColorPair* colors) {
        using Fn = void(__cdecl*)(int32_t, MISC::ColorPair*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005255a0});
        fn(style, colors);
    }

    inline int16_t __cdecl Get_String_Width_(const char* str) {
        using Fn = int16_t(__cdecl*)(const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00525470});
        return fn(str);
    }
    inline void Print_(int32_t x, int32_t y, const char* str) {
        using Fn = void(*)(int32_t, int32_t, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00525910 });
        fn(x, y, str);
    }

    inline void __cdecl Set_Font_Style_Outline_Heavy_(int32_t style, void* color_data) {
        using Fn = void(__cdecl*)(int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005256D0});
        fn(style, color_data);
    }

    inline void __cdecl Set_Outline_Color_(int32_t color1, int32_t color2) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005256F0});
        fn(color1, color2);
    }

    inline void __cdecl Print_To_Bitmap_(int32_t x, int32_t y, uint8_t* text, void* bitmap) {
        using Fn = void(__cdecl*)(int32_t, int32_t, uint8_t*, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00526610});
        fn(x, y, text, bitmap);
    }

    // Suggested name: fonts::Update_Palette_Internal
    inline void __cdecl FUN_00526f80() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00526F80 });
        fn();
    }
}
