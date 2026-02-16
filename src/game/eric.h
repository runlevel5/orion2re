#pragma once
#include <cstdint>

namespace ERIC {
    inline uint8_t& _fade_percent = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B40});

    inline uint8_t& fade_step = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B44});

    inline uint8_t& _hacky_need_to_fade_to_colony_palette_flag =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B48});

    void __cdecl Eric_Init_();

    void __cdecl Eric_Defaults_();

    int16_t __cdecl Get_Sint_Index_From_List_(int16_t value, const int16_t* list, uint16_t count);

    uint16_t __cdecl Get_Fade_Percent_();

    void __cdecl Fast_Fade_In_();

    void __cdecl Fast_Fade_Out_();

    bool __cdecl Fast_Fade_Out_Needed_For_Colony_Palette_();

    void __cdecl Set_Fade_Percent_(int16_t percent);

    inline void __cdecl Print_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t line_height, char* text) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ba9f0});
        fn(x, y, width, line_height, text);
    }

    int16_t __cdecl Global_Cycler_(int16_t divisor, int16_t modulus);

    inline void __cdecl Draw_Centered_(int16_t x, int16_t y, void* bitmap) {
        using Fn = void(__cdecl*)(int16_t, int16_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ba840});
        fn(x, y, bitmap);
    }

    template <typename... Args>
    inline void __cdecl String_Builder2_(char* buffer, int index, char* format, Args... args) {
        using Fn = void(__cdecl*)(char*, int, char*, ...);
        reinterpret_cast<Fn>(std::uintptr_t{0x004ba550})(buffer, index, format, args...);
    }

    inline const char* __cdecl Red_If_Negative_Fmt_String_(int16_t value) {
        using Fn = const char*(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004bab20})(value);
    }
}