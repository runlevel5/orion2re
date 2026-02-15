#pragma once
#include <cstdint>

namespace SHIPS {
    inline void __cdecl Build_Nonplayer_Ship_(int32_t ship_type, int32_t star_idx, int32_t p3) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0041F940})(ship_type, star_idx, p3);
    }

    inline void __cdecl Build_Ship_Icons_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0041e670})();
    }

    inline void __cdecl Set_Ship_Icon_XYs_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0041ec00})();
    }

    inline void __cdecl Draw_Directional_Multi_Colored_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors, int16_t line_type) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, s_colors*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041f560});
        fn(x1, y1, x2, y2, colors, line_type);
    }

    inline void __cdecl Get_Ship_Icon_Coords_(int16_t icon_id, int16_t* x, int16_t* y) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041ee70});
        fn(icon_id, x, y);
    }

    inline void __cdecl Get_Ship_Icon_Dimensions_(int32_t scale, int8_t* width, int8_t* height) {
        using Fn = void(__cdecl*)(int32_t, int8_t*, int8_t*);
        reinterpret_cast<Fn>(0x0041fef0)(scale, width, height);
    }

    inline s_animation_header* __cdecl Get_Ship_Icon_Pict_Seg_(int16_t owner, uint8_t flag) {
        using Fn = s_animation_header*(__cdecl*)(int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041f200});
        return fn(owner, flag);
    }

    inline void __cdecl Do_Ship_Destination_Lines_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041EC60});
        fn();
    }

    inline int16_t __cdecl Overlapped_Ship_Icon_Button_(int16_t* ship_icon_id, int16_t x_offset, int16_t y_offset) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0041f370})(ship_icon_id, x_offset, y_offset);
    }
}