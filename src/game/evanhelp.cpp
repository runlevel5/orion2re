#include "pch.h"

namespace EVANHELP {
    void __cdecl Set_Main_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&EVANHELP::_main_game_screen_help_list, 0xc);
    }

    void __cdecl Set_Multi_Player_Setup_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&EVANHELP::_multi_player_setup_screen_help_list, 10);
    }
}