#pragma once

namespace CMBTDRW1 {
    inline void*& _k_save = *reinterpret_cast<void**>(std::uintptr_t{0x005960FC});

    void __cdecl Detailed_View_Ship_(int16_t ship_idx);

    inline void __cdecl Draw_Main_Combat_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004d0ca0 })();
    }

    inline void __cdecl Refresh_Combat_Screen_Full_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004cda90})();
    }

    inline void __cdecl Draw_Super_Fast_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004cd9e0})();
    }

    inline void __cdecl Move_Options_Background_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004d6e60})();
    }

    inline void __cdecl Display_Combat_View_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004d5b60})(ship_idx);
    }

    inline void __cdecl Scroll_Draw_Combat_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d5370});
        fn();
    }
}