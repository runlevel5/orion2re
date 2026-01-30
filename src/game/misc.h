#pragma once

namespace MISC {
    inline uint8_t& _mouse_clock_image_num =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566578});

    inline uint8_t* _mouse_list_clock =
        reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558250});

    void __cdecl Increment_Mouse_Clock_Image_();

#pragma pack(push, 1)
    struct s_colors {
        uint8_t colors[8];
    };
#pragma pack(pop)

    inline s_colors* __cdecl Font_Colors2_(uint16_t a1, uint8_t a2, uint8_t a3) {
        using Fn = s_colors*(__cdecl*)(int16_t, uint8_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049ddb0});
        return fn(a1, a2, a3);
    }

    inline void __cdecl Set_Fitted_Font_Style_(const char* text, uint8_t* colors, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7) {
        using Fn = void(__cdecl*)(const char*, uint8_t*, int16_t, int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049ede0})(text, colors, p3, p4, p5, p6, p7);
    }

    inline int16_t __cdecl Get_Font_Style_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049e930})();
    }

    inline void __cdecl Get_Mox_Font_Colors_(int16_t style, char c1, char c2, void* out_colors) {
        using Fn = void(__cdecl*)(int16_t, char, char, void*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049e980})(style, c1, c2, out_colors);
    }

    inline void __cdecl Increase_Font_Color_Range_(int16_t value) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049ea00})(value);
    }

    inline char* __cdecl Get_Comma_String_(int32_t value) {
        using Fn = char*(__cdecl*)(int32_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049df10})(value);
    }

    inline int16_t __cdecl Get_Font_Spacing_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049e920})();
    }

    inline void __cdecl Draw_Advancing_Fill_Message_(int8_t a1, void* ctr, uint16_t max, s_colors* c1, s_colors* c2, int16_t x, int16_t y, const char* msg, void* bitmap, uint8_t flag) {
        using Fn = void(__cdecl*)(int8_t, void*, uint16_t, s_colors*, s_colors*, int32_t, int32_t, const char*, void*, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049e3f0});
        fn(a1, ctr, max, c1, c2, x, y, msg, bitmap, flag);
    }

    inline void __cdecl Save_Window_Status_(int16_t* status) {
        using Fn = void(__cdecl*)(int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049EDC0});
        fn(status);
    }

    inline void __cdecl Restore_Window_Status_(int16_t status) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049EDA0});
        fn(status);
    }
}