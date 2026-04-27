#pragma once

#include <cstdint>

struct s_help_box;

namespace ERICHELP {

    inline s_help_box (&_colony_xport_pup_help_list)[7] =
        *reinterpret_cast<s_help_box(*)[7]>(std::uintptr_t{0x00552120});

    inline s_help_box (&_occupation_policy_pup_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00552168});

    inline s_help_box (&_player_attacked_popup_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x005521E8});

    inline s_help_box (&_new_colony_confirmation_box_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x005521F8});

    inline s_help_box (&_new_game_screen_help_list)[11] =
        *reinterpret_cast<s_help_box(*)[11]>(std::uintptr_t{0x00552178});

    inline s_help_box (&_tax_rate_popup_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00552208});

    inline s_help_box (&_dragon_new_star_popup_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00552218});

    inline s_help_box (&_colony_building_queue_screen_help_list)[13] =
        *reinterpret_cast<s_help_box(*)[13]>(std::uintptr_t{0x00552098});

    inline s_help_box (&_colony_summary_screen_help_list)[22] =
        *reinterpret_cast<s_help_box(*)[22]>(std::uintptr_t{0x00551F70});

    inline s_help_box (&_colony_screen_help_list)[17] =
        *reinterpret_cast<s_help_box(*)[17]>(std::uintptr_t{0x00551EC0});

    inline s_help_box (&_planet_selection_popup_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00552238});

    inline s_help_box (&_officer_information_popup_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00552248});

    inline s_help_box (&_refit_ship_popup_help_list)[7] =
        *reinterpret_cast<s_help_box(*)[7]>(std::uintptr_t{0x00552050});

    void __cdecl Eric_Set_Help_List_(s_help_box* help_list, int16_t count);

    void __cdecl Set_Transport_Popup_Help_List_();
    void __cdecl Set_Tax_Rate_Popup_Help_List_();
    void __cdecl Set_Building_Queue_Screen_Help_List_();
    void __cdecl Set_Colony_Summary_Screen_Help_List_();
    void __cdecl Set_Colony_Screen_Help_List_();
    void __cdecl Set_New_Game_Screen_Help_List_(int16_t mode);
    void __cdecl Set_Occupation_Policy_Popup_Help_List_();
    void __cdecl Set_Dragon_New_Star_Popup_Help_List_();
    void __cdecl Set_Player_Attacked_Popup_Help_List_();
    void __cdecl Set_New_Colony_Confirmation_Box_Help_List_();
    void __cdecl Set_Planet_Selection_Popup_Help_List_();
    void __cdecl Set_New_System_Discovery_Popup_Help_List_();
    void __cdecl Set_Officer_Information_Popup_Help_List_();
    void __cdecl Set_Refit_Ship_Popup_Help_List_();
}