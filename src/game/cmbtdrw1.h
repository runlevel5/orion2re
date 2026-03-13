#pragma once

namespace CMBTDRW1 {
    inline void*& _k_save = *reinterpret_cast<void**>(std::uintptr_t{0x005960FC});

    void __cdecl Detailed_View_Ship_(int16_t ship_idx);

    inline void __cdecl Draw_Main_Combat_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004d0ca0 })();
    }
}