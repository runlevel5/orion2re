#pragma once

#include <cstdint>

struct s_help_box;

namespace ERICHELP {

    inline s_help_box (&_colony_xport_pup_help_list)[7] =
        *reinterpret_cast<s_help_box(*)[7]>(std::uintptr_t{0x00552120});

    inline s_help_box (&_tax_rate_popup_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00552208});

    inline s_help_box (&_colony_building_queue_screen_help_list)[13] =
        *reinterpret_cast<s_help_box(*)[13]>(std::uintptr_t{0x00552098});

    inline void __cdecl Eric_Set_Help_List_(s_help_box* list, int32_t count) {
        using Fn = void(__cdecl*)(s_help_box*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00412300});
        fn(list, count);
    }

    void __cdecl Set_Transport_Popup_Help_List_();
    void __cdecl Set_Tax_Rate_Popup_Help_List_();
    void __cdecl Set_Building_Queue_Screen_Help_List_();

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

    inline void __cdecl Set_New_Colony_Confirmation_Box_Help_List_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004123E0});
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