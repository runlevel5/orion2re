#pragma once
#include <cstdint>

struct s_animation_header;

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

    void __cdecl Print_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, const char* text);

    int16_t __cdecl Global_Cycler_(int16_t divisor, int16_t modulus);

    void __cdecl Draw_Centered_(int16_t x, int16_t y, s_animation_header* anim);

    template <typename... Args>
    inline void __cdecl String_Builder2_(char* buffer, int index, char* format, Args... args) {
        using Fn = void(__cdecl*)(char*, int, char*, ...);
        reinterpret_cast<Fn>(std::uintptr_t{0x004ba550})(buffer, index, format, args...);
    }

    inline char* const s_2_00551108 = reinterpret_cast<char*>(std::uintptr_t{0x00551108});

    char* __cdecl Red_If_Negative_Fmt_String_(int16_t value);
}