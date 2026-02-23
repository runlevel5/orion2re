#pragma once

namespace SCROLL {
    inline bool __cdecl Draw_Generic_Vertical_Scroll_Bar_(s_scroll_bar* bar, void* fill_fn) {
        using Fn = bool(__cdecl*)(s_scroll_bar*, void*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0048dc40})(bar, fill_fn);
    }

    void __cdecl Scroll_Down_One_Item_(s_scroll_bar* sb);
    void __cdecl Scroll_Up_One_Item_(s_scroll_bar* bar);

    int16_t __cdecl Get_Vertical_Scroll_Bar_Height_(s_scroll_bar* sb);
}