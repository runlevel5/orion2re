#pragma once

namespace ERICHELP {
    inline void __cdecl Set_Transport_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412360});
        fn();
    }

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

    inline void __cdecl Set_Occupation_Policy_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412370});
        fn();
    }

    inline void __cdecl Set_Dragon_New_Star_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412400});
        fn();
    }

    inline void __cdecl Set_Player_Attacked_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004123D0});
        fn();
    }

    inline void __cdecl Set_Planet_Selection_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412420});
        fn();
    }

    inline void __cdecl Set_New_System_Discovery_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412410});
        fn();
    }

    inline void __cdecl Set_Officer_Information_Popup_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412430});
        fn();
    }
}