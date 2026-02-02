#include "deps.h"

namespace MAINMENU {
    void __cdecl Main_Menu_Fade_Function_() {
        animate::Draw_(0, 0, reinterpret_cast<animate::s_animation_header*>(_main_menu_background));
    }

    void __cdecl Add_Main_Menu_Fields_() {
        char filename_buf[16];
        char dir_buf[16];

        uint8_t game_type = FILEDEF::Get_Game_Type_From_Saved_Game_(9);

        _sprintf(filename_buf, s_save_percent_d_gam, 10);

        fields::Init_Fields_State_();

        // Check if continue game is available
        bool continue_avail = false;
        if (game_type == 0 || game_type == 1) {
            if (dos::DIR_(filename_buf, dir_buf)) {
                continue_avail = true;
            }
        }

        if (continue_avail) {
            _mainmenu->field_C = fields::Add_Hidden_Field_(0x19F, 0xAC, 0x237, 0xC1, s_C_00552420, 0x29);
        } else {
            _mainmenu->field_C = -1000;
        }

        // Check if loading is available
        if (MOX::_disallow_saves_code == 0 && dos::DIR_(s_save_question_gam, dir_buf)) {
            _mainmenu->field_L = fields::Add_Hidden_Field_(0x19F, 0xC3, 0x237, 0xD7, s_L_005560e8, 0x29);
        } else {
            _mainmenu->field_L = -1000;
        }

        // Mandatory fields
        _mainmenu->field_N = fields::Add_Hidden_Field_(0x19F, 0xD9, 0x237, 0xEE, s_N_0055765C, 0x29);
        _mainmenu->field_M = fields::Add_Hidden_Field_(0x19F, 0xF0, 0x237, 0x104, s_M_00557ED8, 0x29);
        _mainmenu->field_H = fields::Add_Hidden_Field_(0x19F, 0x106, 0x237, 0x11B, s_H_005569D4, 0x29);
        _mainmenu->field_Q = fields::Add_Hidden_Field_(0x19F, 0x11D, 0x237, 0x132, s_Q_005598f0, 0x29);
    }
}