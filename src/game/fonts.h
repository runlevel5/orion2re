#pragma once

namespace fonts {
    inline int32_t& _palette_block =
        *reinterpret_cast<int32_t*>(std::uintptr_t{ 0x005787B8 });

    inline char* const _font_name =
        reinterpret_cast<char*>(std::uintptr_t{ 0x005787C0 });

    void Load_Palette_(int16_t palette_id, int16_t start_index, int16_t end_index);

    inline void Set_PSTR_Style_(int32_t style) {
        using Fn = void(*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00526D00 });
        fn(style);
    }
    inline void Set_Font_Style_(int32_t font, intptr_t color_ptr) {
        using Fn = void(*)(int32_t, intptr_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005255A0 });
        fn(font, color_ptr);
    }
    inline void Print_(int32_t x, int32_t y, const char* str) {
        using Fn = void(*)(int32_t, int32_t, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00525910 });
        fn(x, y, str);
    }

    // Suggested name: fonts::Update_Palette_Internal
    inline void __cdecl FUN_00526f80() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00526F80 });
        fn();
    }
}
