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

    inline void __cdecl Create_Ship_Darken_Map_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004d6380 });
        fn();
    }

    inline int16_t __cdecl Ship_Completely_On_Screen_(int16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D5820});
        return fn(ship_idx);
    }

    inline uint32_t __cdecl Relative_Bearing_(int16_t attacker_idx, int16_t defender_idx) {
        using Fn = uint32_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D4630});
        return fn(attacker_idx, defender_idx);
    }

    inline int16_t __cdecl Ship_On_Screen_(uint16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D59A0});
        return fn(ship_idx);
    }

    inline void __cdecl Draw_Retreating_Ship_(uint16_t ship_idx) {
        using Fn = void(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D4A20});
        fn(ship_idx);
    }
}