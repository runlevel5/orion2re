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
}