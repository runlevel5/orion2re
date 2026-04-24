#include "pch.h"

namespace EVANHELP {
    void __cdecl Set_Main_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&EVANHELP::_main_game_screen_help_list, 0xc);
    }

    void __cdecl Set_Multi_Player_Setup_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&EVANHELP::_multi_player_setup_screen_help_list, 10);
    }

    void Set_Load_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&EVANHELP::_load_game_screen_help_list, 7);
    }

    void __cdecl Set_Options_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&_options_game_screen_help_list, 0x12);
    }

    void __cdecl Set_Save_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&_save_game_screen_help_list, 7);
    }

    void __cdecl Set_PlntSum_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&_plntsum_screen_help_list, 15);
    }

    void __cdecl Set_Officer_Screen_Help_() {
        int32_t skills[2];
        skills[0] = 0;
        skills[1] = 0;

        OFFICER::Get_N_Skills_Per_Officer_Displayed_(skills);

        if (MOX::_officer_scrn_type == 1) {
            Set_ColOff_Screen_Help_List_(skills);
        } else {
            Set_ShipOff_Screen_Help_List_(skills, MOX::_n_fltscrn_big_icons_added);
        }
    }

    void __cdecl Set_Design_Screen_Help_List_(int16_t count_1, int16_t count_2) {
        _memcpy(&_weapon_replacement_screen_help_list[6][0], &_static_design_screen_help_list, 130);

        if (count_1 > 0) {
            int32_t val = _weapon_replacement_screen_help_list[9][2] + count_1 * 14 + 15;
            if (val >= _weapon_replacement_screen_help_list[9][4]) {
                val = _weapon_replacement_screen_help_list[9][4];
            }
            _weapon_replacement_screen_help_list[9][2] = static_cast<int16_t>(val);
        }

        if (count_2 > 0) {
            int32_t val = _weapon_replacement_screen_help_list[9][7] + count_2 * 14 + 15;
            if (val >= _weapon_replacement_screen_help_list[9][9]) {
                val = _weapon_replacement_screen_help_list[9][9];
            }
            _weapon_replacement_screen_help_list[9][7] = static_cast<int16_t>(val);
        }

        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_weapon_replacement_screen_help_list[6], 13);
    }

    void __cdecl Set_Null_Modem_Setup_Screen_Help_List_(int16_t x_offset, int16_t y_offset) {
        _memcpy(&_null_modem_setup_screen_help_list, &_static_null_modem_setup_screen_help_list, 40);
        Offset_Help_Field_XYs_(&_null_modem_setup_screen_help_list, 4, x_offset, y_offset);
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&_null_modem_setup_screen_help_list, 4);
    }

    void __cdecl Get_Application_Description_(char* dest, int16_t item_index) {
        char help_lbx_name[20];

        TEXTBOX::Get_Help_Lbx_Name_(help_lbx_name);
        allocate::Push_Block_(MOX::_screen_seg);

        void* buffer = allocate::Allocate_Next_Block_(MOX::_screen_seg, 2000);
        allocate::Allocate_Next_Block_(MOX::_screen_seg, 1403);

        uint8_t* data = (uint8_t*)farload::Far_Reload_Next_Data_(help_lbx_name, 0, buffer, item_index, 1, 1403);

        strcpy(dest, reinterpret_cast<char*>(data + 0x67));

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    void __cdecl Save_Help_List_() {
        _prev_help_struct_pointer = fields::_help_struct_pointer;
        _prev_help_list_count = fields::_help_list_count;
        _prev_help_list_active = fields::_help_list_active;
    }

    void __cdecl Set_Input_Box_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&_input_box_help_list, 1);
    }

    void __cdecl Restore_Help_List_() {
        fields::_help_struct_pointer = _prev_help_struct_pointer;
        fields::_help_list_count = _prev_help_list_count;
        fields::_help_list_active = _prev_help_list_active;

        if (_prev_help_list_active != 0) {
            fields::Set_Help_List_(_prev_help_struct_pointer, _prev_help_list_count);
        } else {
            fields::Deactivate_Help_List_();
        }
    }

    void __cdecl Set_Fleet_Screen_Help_List_(int16_t param_1) {
        int16_t help_list_count = 12;

        _memcpy(EVANHELP::_fleet_screen_help_list, EVANHELP::_static_fleet_screen_help_list, 0x50 * sizeof(uint32_t));

        if (param_1 == 0) {
            EVANHELP::Append_Help_List_(
                EVANHELP::_fleet_screen_help_list,
                &help_list_count,
                0x20, 0x168, 0x158, 0x35, 0x24f, 0x161
            );
        } else {
            for (int16_t i = param_1; i < 20; ++i) {
                int32_t x = (i % 4) * 0x40;
                int32_t y = (i / 4) * 0x40;

                EVANHELP::Append_Help_List_(
                    EVANHELP::_fleet_screen_help_list,
                    &help_list_count,
                    0x20, 0x168,
                    x + 0x156,
                    y + 0x34,
                    x + 0x196,
                    y + 0x72
                );
            }
        }

        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(EVANHELP::_fleet_screen_help_list, help_list_count);
    }

    void __cdecl Set_Scrap_Freighters_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(&_scrap_freighters_help_list, 1);
    }
}