#pragma once
#include <cstdint>

namespace ERIC {
    inline uint8_t& _fade_percent = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B40});

    inline uint8_t& fade_step = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B44});

    void __cdecl Eric_Init_();

    void __cdecl Eric_Defaults_();

    int16_t __cdecl Get_Sint_Index_From_List_(int16_t value, const int16_t* list, uint16_t count);

    uint16_t __cdecl Get_Fade_Percent_();

    void Fast_Fade_Out_();

    inline bool __cdecl Fast_Fade_Out_Needed_For_Colony_Palette_() {
        using Fn = bool(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x004BA7A0})();
    }

    void __cdecl Set_Fade_Percent_(int16_t percent);

    inline void __cdecl Print_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t line_height, char* text) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ba9f0});
        fn(x, y, width, line_height, text);
    }

    inline int16_t __cdecl Global_Cycler_(int16_t range, int16_t step) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004BA930})(range, step);
    }

    inline void __cdecl Draw_Centered_(s_animation_header* anim) {
        using Fn = void(__cdecl*)(s_animation_header*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0050d220});
        fn(anim);
    }
}