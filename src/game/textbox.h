#pragma once
#include <cstdint>

namespace TEXTBOX {
    inline int16_t& _bottom_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565EC8});
    inline uint32_t& _textbox_seg = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565ED8});
    inline const char* s_textbox_lbx_00557444 = reinterpret_cast<const char*>(std::uintptr_t{0x00557444});
    inline const char* s__00551120 = reinterpret_cast<const char*>(std::uintptr_t{0x00551120});

    void __cdecl Get_Help_Lbx_Name_(char* lbx_name);

    inline void __cdecl Get_Mox_Text_Box_Base_Colors_(int unknown, uint8_t* color_out_1, uint8_t* color_out_2) {
        using Fn = void(__cdecl*)(int, uint8_t*, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471790});
        fn(unknown, color_out_1, color_out_2);
    }

    inline void __cdecl Text_Box_Startup_(int size) {
        using Fn = void(__cdecl*)(int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471550});
        fn(size);
    }

    inline void __cdecl Box_Centered_On_XY_(int16_t* out_x, int16_t* out_y, int16_t height, bool has_title) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004713c0});
        fn(out_x, out_y, height, has_title);
    }

    inline void __cdecl Text_Box_Get_Input_(int16_t input_type) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471710});
        fn(input_type);
    }

    inline void __cdecl Text_Box_Cleanup_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00471690});
        fn();
    }

    void __cdecl Do_Text_Box_(char* title, char* text, int16_t input_type);

    void __cdecl Text_Box_(const char* param_1, const char* param_2);
}