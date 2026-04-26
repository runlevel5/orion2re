#include "pch.h"

namespace ERICHELP {

    void __cdecl Eric_Set_Help_List_(s_help_box* help_list, int16_t count) {
        fields::Deactivate_Help_List_();
        fields::Set_Help_List_(help_list, count);
    }

    void __cdecl Set_Transport_Popup_Help_List_() {
        Eric_Set_Help_List_(_colony_xport_pup_help_list, 7);
    }

    void __cdecl Set_Tax_Rate_Popup_Help_List_() {
        Eric_Set_Help_List_(_tax_rate_popup_help_list, 1);
    }

    void __cdecl Set_Building_Queue_Screen_Help_List_() {
        Eric_Set_Help_List_(_colony_building_queue_screen_help_list, 13);
    }

    void __cdecl Set_Colony_Summary_Screen_Help_List_() {
        Eric_Set_Help_List_(_colony_summary_screen_help_list, 22);
    }

    void __cdecl Set_Colony_Screen_Help_List_() {
        Eric_Set_Help_List_(_colony_screen_help_list, 17);
    }

    void __cdecl Set_New_Game_Screen_Help_List_(int16_t mode) {
        Eric_Set_Help_List_(_new_game_screen_help_list, 11);

        switch (mode) {
            case 0:
                _new_game_screen_help_list[0].help_id = 0x226;
                break;
            case 1:
                _new_game_screen_help_list[0].help_id = 0x225;
                break;
            case 2:
                _new_game_screen_help_list[0].help_id = 0x224;
                break;
            default:
                break;
        }
    }

    void __cdecl Set_Occupation_Policy_Popup_Help_List_() {
        Eric_Set_Help_List_(_occupation_policy_pup_help_list, 1);
    }

    void __cdecl Set_Dragon_New_Star_Popup_Help_List_() {
        Eric_Set_Help_List_(_dragon_new_star_popup_help_list, 1);
    }

    void __cdecl Set_Player_Attacked_Popup_Help_List_() {
        Eric_Set_Help_List_(_player_attacked_popup_help_list, 1);
    }

    void __cdecl Set_New_Colony_Confirmation_Box_Help_List_() {
        Eric_Set_Help_List_(_new_colony_confirmation_box_help_list, 1);
    }

    void __cdecl Set_Planet_Selection_Popup_Help_List_() {
        Eric_Set_Help_List_(_planet_selection_popup_help_list, 1);
    }

    void __cdecl Set_New_System_Discovery_Popup_Help_List_() {
    }

    void __cdecl Set_Officer_Information_Popup_Help_List_() {
        Eric_Set_Help_List_(_officer_information_popup_help_list, 1);
    }

}