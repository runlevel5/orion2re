#pragma once
#include <cstdint>

struct s_animation_header;

namespace ERIC {
    inline uint8_t& _fade_percent = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B40});

    inline uint8_t& fade_step = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B44});

    inline uint8_t& _hacky_need_to_fade_to_colony_palette_flag =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558B48});

    inline uint16_t& _saved_sfx_l = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00558B4C});

    inline uint16_t& _saved_music_l = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00558B50});

    inline void*& _cinematic_saved_autofunction = *reinterpret_cast<void**>(std::uintptr_t{0x005A4AD8});

    void __cdecl Eric_Init_();

    void __cdecl Eric_Defaults_();

    int16_t __cdecl Get_Sint_Index_From_List_(int16_t value, const int16_t* list, uint16_t count);

    uint16_t __cdecl Get_Fade_Percent_();

    void __cdecl Fast_Fade_In_();

    void __cdecl Fast_Fade_Out_();

    bool __cdecl Fast_Fade_Out_Needed_For_Colony_Palette_();

    inline void __cdecl Set_Fade_Speed_Perc(int32_t percentage) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ba650});
        fn(percentage);
    }

    void __cdecl Set_Fade_Percent_(int16_t percent);

    void __cdecl Print_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, const char* text);

    inline void __cdecl Print_Paragraph_Centered_Vertically_(int16_t x, int16_t y, int16_t w, int16_t h, void* text, int16_t unk) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, void*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004BA9A0})(x, y, w, h, text, unk);
    }

    int16_t __cdecl Global_Cycler_(int16_t divisor, int16_t modulus);

    void __cdecl Draw_Centered_(int16_t x, int16_t y, s_animation_header* anim);

    template <typename... Args>
    inline void __cdecl String_Builder2_(char* buffer, int index, char* format, Args... args) {
        using Fn = void(__cdecl*)(char*, int, char*, ...);
        reinterpret_cast<Fn>(std::uintptr_t{0x004ba550})(buffer, index, format, args...);
    }

    inline char* const s_2_00551108 = reinterpret_cast<char*>(std::uintptr_t{0x00551108});

    char* __cdecl Red_If_Negative_Fmt_String_(int16_t value);

    inline void __cdecl Pulsing_Box_(int16_t y, int16_t x, int32_t width, int32_t height, int32_t arg5, int32_t arg6, int32_t arg7) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int32_t, int32_t, int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BA950});
        fn(y, x, width, height, arg5, arg6, arg7);
    }

    inline void __cdecl Play_Loose_Flic_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004bb260})();
    }

    inline void __cdecl Play_Normal_Win_Flic_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004bb1b0})();
    }

    inline void __cdecl Play_All_Flics_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004bb1f0})();
    }

    inline void __cdecl Play_Take_Orion_Flic_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004bb110})();
    }

    inline void __cdecl Do_Cinematic_Popup_Setup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BAEE0});
        fn();
    }

    void __cdecl Do_Cinematic_Popup_Cleanup_();

    void __cdecl Play_Dimensional_Portal_Flic_();

    void __cdecl Play_Planetary_Destruction_Flic_();

    bool __cdecl Sint_Is_In_List_(int16_t value, int16_t* list, uint16_t count);

    inline void __cdecl Play_Diplomatic_Win_Flic_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004bb140})();
    }
}