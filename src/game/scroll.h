#pragma once

namespace SCROLL {
    bool __cdecl Draw_Generic_Vertical_Scroll_Bar_(s_scroll_bar* scroll_bar, void(__cdecl* callback)());

    void __cdecl Scroll_Down_One_Item_(s_scroll_bar* sb);
    void __cdecl Scroll_Up_One_Item_(s_scroll_bar* bar);

    int16_t __cdecl Get_Vertical_Scroll_Bar_Height_(s_scroll_bar* sb);

    void __cdecl Update_Vertical_Scroll_Bar_State_(s_scroll_bar* scroll_bar);

    int8_t __cdecl Set_Vertical_Scroll_Bar_Maxima_(s_scroll_bar* scroll_bar);

    int16_t __cdecl Get_Vertical_Scroll_Bar_First_Item_(s_scroll_bar* sb);
}