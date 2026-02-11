#pragma once
#include <cstdint>

namespace FLT1 {
    inline void __cdecl Draw_Fltscrn_Big_Ship_Icons_(int16_t arg) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004b05c0})(arg);
    }

    inline void __cdecl Draw_Fltscrn_Ship_Destination_Lines_(int16_t arg) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004b0820})(arg);
    }

    inline void __cdecl Draw_Fltscrn_Small_Ship_Icons_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004b0b10})();
    }

    inline void __cdecl Draw_Flashing_Small_Ship_Icon_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004b01d0})();
    }

    inline void __cdecl Fill_FltScrn_Scroll_Bar_(s_scroll_bar* bar) {
        using Fn = void(__cdecl*)(s_scroll_bar*);
        reinterpret_cast<Fn>(std::uintptr_t{0x004b0c00})(bar);
    }

    inline void __cdecl Fleet_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004b0d20})();
    }

    inline void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* bar) {
        using Fn = void(__cdecl*)(s_scroll_bar*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004420D0 });
        fn(bar);
    }

    inline int __cdecl Scan_Small_Ship_Icons_(int16_t scanned, int16_t input, int16_t* out_scanned, int16_t* out_idx) {
        using Fn = int(__cdecl*)(int16_t, int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004B2340 });
        return fn(scanned, input, out_scanned, out_idx);
    }
}