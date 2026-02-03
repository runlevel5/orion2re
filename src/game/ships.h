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

    inline void __cdecl Draw_Directional_Multi_Colored_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t* colors, int16_t line_type) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, uint8_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041f560});
        fn(x1, y1, x2, y2, colors, line_type);
    }

    inline void __cdecl Get_Ship_Icon_Coords_(int16_t icon_id, int16_t* x, int16_t* y) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041ee70});
        fn(icon_id, x, y);
    }

    inline s_animation_header* __cdecl Get_Ship_Icon_Pict_Seg_(int16_t owner, uint8_t flag) {
        using Fn = s_animation_header*(__cdecl*)(int16_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041f200});
        return fn(owner, flag);
    }
}