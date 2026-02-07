#pragma once
#include <cstdint>

namespace DESBOX {
    inline int8_t& _incr_value = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A61E0});

    inline void __cdecl Generic_Replacement_Box_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0048dfb0})();
    }
    inline void __cdecl Weapons_Replacement_Box_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00490780});
        fn();
    }
    inline void __cdecl Special_Systems_Box_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0048f4d0});
        fn();
    }
    inline uint32_t __cdecl Application_Type_(int16_t app_idx) {
        using Fn = uint32_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0048DCF0})(app_idx);
    }
}