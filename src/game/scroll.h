#pragma once

namespace SCROLL {
    inline bool __cdecl Draw_Generic_Vertical_Scroll_Bar_(s_scroll_bar* bar, void* fill_fn) {
        using Fn = bool(__cdecl*)(s_scroll_bar*, void*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0048dc40})(bar, fill_fn);
    }

    inline void __cdecl Scroll_Down_One_Item_(s_scroll_bar* bar) {
        using Fn = void(__cdecl*)(s_scroll_bar*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0048DB60})(bar);
    }
    inline void __cdecl Scroll_Up_One_Item_(s_scroll_bar* bar) {
        using Fn = void(__cdecl*)(s_scroll_bar*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0048DBD0})(bar);
    }
}