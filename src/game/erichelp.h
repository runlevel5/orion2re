#pragma once

namespace ERICHELP {
    inline void __cdecl Set_Tax_Rate_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004123F0});
        fn();
    }

    inline void __cdecl Set_Building_Queue_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00412350})();
    }

    inline void __cdecl Set_Colony_Summary_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412330});
        fn();
    }

    inline void __cdecl Set_Colony_Screen_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412320});
        fn();
    }

    inline void Set_New_Game_Screen_Help_List_(int16_t civ_var) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(0x00412380)(civ_var);
    }
    inline void __cdecl Set_Race_Select_Help_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004988B0)();
    }
}