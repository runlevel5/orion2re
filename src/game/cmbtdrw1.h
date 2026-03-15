#pragma once

namespace CMBTDRW1 {
    inline void*& _k_save = *reinterpret_cast<void**>(std::uintptr_t{0x005960FC});

    void __cdecl Detailed_View_Ship_(int16_t ship_idx);

    inline void __cdecl Draw_Internals_Display_(int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(0x004d66d0)(x, y);
    }

    inline void __cdecl Full_Draw_Combat_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004cdb50)();
    }

    inline void __cdecl Draw_Weapon_Status_Display_(int16_t param_1, int16_t param_2) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(0x004cf810)(param_1, param_2);
    }

    inline void __cdecl Draw_Special_Status_Display_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004d0070)();
    }

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

    inline int16_t __cdecl Relative_Bearing_XY_(int16_t target_unit_idx, int16_t x, int16_t y) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004d4580})(target_unit_idx, x, y);
    }

    int16_t __cdecl Relative_Bearing_(int16_t ship_a_idx, int16_t ship_b_idx);

    inline int16_t __cdecl Ship_On_Screen_(uint16_t ship_idx) {
        using Fn = int16_t(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D59A0});
        return fn(ship_idx);
    }

    inline void __cdecl Draw_Ship_(int16_t ship_idx, int16_t x, int16_t y) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004D1980})(ship_idx, x, y);
    }

    inline void __cdecl Get_Draw_Ship_Offsets_(int16_t val, int16_t* out_x, int16_t* out_y) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D6520});
        fn(val, out_x, out_y);
    }

    inline void __cdecl Draw_Ship_To_Bitmap_(int16_t ship_idx, int16_t param_2, int16_t param_3, int16_t progress) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D4780});
        fn(ship_idx, param_2, param_3, progress);
    }

    inline void __cdecl Draw_Retreating_Ship_(uint16_t ship_idx) {
        using Fn = void(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D4A20});
        fn(ship_idx);
    }

    inline uint8_t __cdecl Get_Weapon_Arc_(int16_t bearing) {
        using Fn = uint8_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004D4730})(bearing);
    }
}