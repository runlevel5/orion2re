#pragma once

namespace MISC {
    inline uint8_t& _mouse_clock_image_num =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566578});

    inline uint8_t* _mouse_list_clock =
        reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558250});

    void __cdecl Increment_Mouse_Clock_Image_();

    void __cdecl Fade_Into_Screen_(void (*draw_callback)(), uint8_t* status_flag, int16_t palette_id);

    void __cdecl Change_Font_Style_(int16_t font_id);

    inline s_colors& _mox_font_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x005A5328});

    s_colors* __cdecl Font_Colors2_(int16_t mode, uint8_t param_2, uint8_t param_3);

    inline void __cdecl Set_Fitted_Font_Style_(const char* text, s_colors* colors, int16_t p3, int16_t p4, int16_t p5, int16_t p6, int16_t p7) {
        using Fn = void(__cdecl*)(const char*, s_colors*, int16_t, int16_t, int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049ede0})(text, colors, p3, p4, p5, p6, p7);
    }

    int16_t __cdecl Get_Font_Style_();

    void __cdecl Get_Mox_Font_Colors_(int16_t font_idx, uint8_t color_idx_1, uint8_t color_idx_2, s_colors* out_colors);

    void __cdecl Increase_Font_Color_Range_(int16_t increment);
    void __cdecl Decrease_Font_Color_Range_(int16_t amount);

    inline char* __cdecl Get_Comma_String_(int32_t value) {
        using Fn = char*(__cdecl*)(int32_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049df10})(value);
    }

    int16_t __cdecl Get_Font_Spacing_();

    inline void __cdecl Draw_Advancing_Fill_Message_(int8_t a1, void* ctr, uint16_t max, s_colors* c1, s_colors* c2, int16_t x, int16_t y, const char* msg, void* bitmap, uint8_t flag) {
        using Fn = void(__cdecl*)(int8_t, void*, uint16_t, s_colors*, s_colors*, int32_t, int32_t, const char*, void*, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049e3f0});
        fn(a1, ctr, max, c1, c2, x, y, msg, bitmap, flag);
    }

    void __cdecl Save_Window_Status_(int16_t* status);

    void __cdecl Restore_Window_Status_(int16_t clip_enabled);

    void __cdecl Get_LBX_Message_(int16_t message_id, char* buffer);

    inline void __cdecl Get_Bitmap_String_Range_(int16_t count, int16_t total, int16_t* out_width, int16_t* d1, int16_t* d2, int16_t* out_height, const char* str) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t*, int16_t*, int16_t*, int16_t*, const char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049E880});
        fn(count, total, out_width, d1, d2, out_height, str);
    }

    inline void __cdecl Swap_Bitmap_Colors_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors1, s_colors* colors2, int32_t count, void* bitmap) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, s_colors*, s_colors*, int32_t, void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049EEE0});
        fn(x1, y1, x2, y2, colors1, colors2, count, bitmap);
    }

    inline void __cdecl Set_Field_Outline_Color_(int color) {
        using Fn = void(__cdecl*)(int);
        reinterpret_cast<Fn>(0x00520000)(color);
    }

    void __cdecl _Darken_Fill_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    int16_t __cdecl Bit_Value_To_Num_(int32_t value, int16_t bit_limit);

    inline bool __cdecl Occluded_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ox1, int16_t oy1, int16_t ox2, int16_t oy2) {
        using Fn = bool(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049ebd0})(x1, y1, x2, y2, ox1, oy1, ox2, oy2);
    }
}