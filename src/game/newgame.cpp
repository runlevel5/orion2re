#include "deps.h"
#include <cstring>

namespace NEWGAME {

int16_t __cdecl Newgame_Screen_(void) {
    _color_ken.colors[0] = 0x79;
    _color_ken.colors[1] = 0x7B;
    _color_ken.colors[2] = 0x7A;
    *reinterpret_cast<uint32_t*>(&_color_ken.colors[3]) = 0x7C7C7C7C;
    _color_ken.colors[7] = 0x7C;

    int16_t prev_civ_btn = _civ_button_variable;

    struct_::Clear_Structure_(MOX::_star, 0x1FC8);
    struct_::Clear_Structure_(MOX::_planet, 0x17E8);
    struct_::Clear_Structure_(MOX::_colony, 0x1608A);
    struct_::Clear_Structure_(MOX::_ship, 0xFBF4);
    struct_::Clear_Structure_(MOX::_player, 0x7548);
    struct_::Clear_Structure_(MOX::_leaders, 0xF71);

    int16_t result = 0;

    mouse::Set_Mouse_List_(COLGCBT::_mouse_list_blank, 1);
    ERIC::Fast_Fade_Out_();
    fonts::Load_Palette_(9, 0, 0xFF);

    char local_8[8];
    for (int i = 0; i < 3; ++i) {
        local_8[i] = (char)(i * 2 - 0x59);
    }
    memset(&local_8[3], 0, 5);

    if (MOX::_game_type == 0 || MOX::_game_type == 1) {
        memset(RACESEL::_color_flag, 0, 8);
        memset(RACESEL::_race_flag, 0, 14);
    }

    Reload_Newgame_Screen_();
    Globals_To_Temps_();
    Update_Fields_();

    COLONY::_full_draw = 1;
    video::Set_Refresh_Stencil_();
    ERICHELP::Set_New_Game_Screen_Help_List_(_civ_button_variable);
    fields::Assign_Auto_Function_(Draw_Newgame_Screen_, 2);
    Set_Page_Off_();
    Draw_Newgame_Screen_();
    palstore::Darken_Palette_(100);
    palette::Clear_Palette_Changes_(0, 0xFF);

    bool exit_loop = false;
    ERIC::Fast_Fade_In_();
    mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);

    do {
        int16_t input = fields::Get_Input_();
        FAKEBUTS::Evaluate_Click_Thru_Fields_(input);

        if (input == _fields[6] || input == _fields[7]) {
            MOX::_current_screen = MOX::_return_screen;
            exit_loop = true;
        }

        if (input == _fields[8]) {
            Temps_To_Globals_();
            if (MOX::_game_type == 1 /* MULTIPLAYER_CLIENT */) {
                MOX::_current_screen = 16 /* SCREEN_MULTIPLAYER_WAIT */;
            } else if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                MOX::_current_screen = 21 /* SCREEN_NETWORK_GAME */;
            } else {
                int16_t selection_result = Race_Selection_Screen_(MOX::_player);
                if (selection_result == 1) {
                    exit_loop = true;
                    MOX::_player[0].objectives = 100 /* 'd' */;
                } else {
                    Reload_Newgame_Screen_();
                    video::Set_Refresh_Stencil_();
                    COLONY::_full_draw = 1;
                    Globals_To_Temps_();
                    Update_Fields_();
                    fields::Assign_Auto_Function_(Draw_Newgame_Screen_, 2);
                    ERICHELP::Set_New_Game_Screen_Help_List_(_civ_button_variable);
                    goto check_civ_update;
                }
            }
            result = 1;
            exit_loop = true;
        }

    check_civ_update:
        if (prev_civ_btn != _civ_button_variable) {
            prev_civ_btn = _civ_button_variable;
            ERICHELP::Set_New_Game_Screen_Help_List_(prev_civ_btn);
        }

        if (!exit_loop) {
            fonts::Set_Font_Style_(3, &_color_ken);
            Draw_Newgame_Screen_();
            fonts::Set_Font_Style_(3, &_color_ken);
            MOX::Mox_Sync_Update_();
        }
    } while (!exit_loop);

    if (result == 1) {
        Temps_To_Globals_();
    }

    fields::Clear_Fields_();
    FAKEBUTS::Clear_Click_Thru_Fields_();
    fields::Deactivate_Help_List_();
    fields::Deactivate_Auto_Function_();

    if (MOX::_game_type == 2 || MOX::_game_type == 3) {
        strcpy(MOX::_settings.net_game_name, DAT_0059532e);
    }

    return result;
}

} // namespace NEWGAME