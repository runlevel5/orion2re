#include "pch.h"

namespace EVANHELP {
    void __cdecl Set_Main_Screen_Help_List_() {
        int16_t local_2 = 15;

        if (MOX::_game_type == 2) {
            EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 303, 73, 0, 156, 19);
            EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 302, 418, 0, 519, 19);
        } else if (MOX::_game_type == 1) {
            EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 303, 73, 0, 156, 19);
        }

        if (MOVEBOX::Moveable_Box_Selected_(2) != 0) {
            int16_t height = animate::Get_Height_(MOX::_fleet_box_close_lg_button_seg);
            int16_t var_x = MOX::_fleet_box.x + 2;
            int16_t var_y = MOX::_fleet_box.y - height + MOX::_fleet_box.field8_0xe;
            int16_t var_w = MOX::_fleet_box.width + var_x;

            EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 310, var_x, var_y - 35, var_w, var_y);
            EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 304, var_x, var_y, var_w, var_y + height);

            int16_t offset = height;

            if (MOX::_fleet_box.outpost_btn_active != 0) {
                EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 309, var_x, var_y, var_w, var_y + offset + height);
                offset += height;
            }

            if (MOX::_fleet_box.colonize_btn_active != 0) {
                EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 306, var_x, var_y, var_w, var_y + offset + height);
                offset += height;
            }

            if (MOX::_fleet_box.engage_btn_active != 0) {
                EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 307, var_x, var_y, var_w, var_y + offset + height);
                offset += height;
            }

            if (MOX::_fleet_box.transport_btn_active != 0) {
                EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 308, var_x, var_y, var_w, var_y + offset + height);
                offset += height;
            }

            if (MOX::_fleet_box.attack_btn_active != 0) {
                EVANHELP::Append_Help_List_(EVANHELP::_main_screen_help_list, &local_2, 25, 305, var_x, var_y, var_w, var_y + offset + height);
            }
        }

        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(EVANHELP::_main_screen_help_list, local_2);
    }

    void __cdecl Set_Main_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(EVANHELP::_main_game_screen_help_list, 0xc);
    }

    void __cdecl Set_Multi_Player_Setup_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_multi_player_setup_screen_help_list, 10);
    }

    void Set_Load_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_load_game_screen_help_list, 7);
    }

    void __cdecl Set_Options_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_options_game_screen_help_list, 18);
    }

    void __cdecl Set_Save_Game_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_save_game_screen_help_list, 7);
    }

    void __cdecl Set_PlntSum_Screen_Help_List_() {
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_plntsum_screen_help_list, 15);
    }

    void __cdecl Set_Officer_Screen_Help_() {
        int16_t skills[4] = { 0 };

        OFFICER::Get_N_Skills_Per_Officer_Displayed_(skills);

        if (MOX::_officer_scrn_type == 1) {
            // TYPE MISMATCH: skills is int32_t[2], but Set_ColOff_Screen_Help_List_ expects int16_t*
            Set_ColOff_Screen_Help_List_(skills);
        } else {
            // TYPE MISMATCH: skills is int32_t[2], but Set_ShipOff_Screen_Help_List_ expects int16_t*
            Set_ShipOff_Screen_Help_List_(skills, MOX::_n_fltscrn_big_icons_added);
        }
    }

    void __cdecl Set_Design_Screen_Help_List_(int16_t lines_1, int16_t lines_2) {
        memcpy(_design_screen_help_list, _static_design_screen_help_list, sizeof(_design_screen_help_list));

        if (lines_1 > 0) {
            int32_t new_y1 = _design_screen_help_list[6].y1 + lines_1 * 14 + 15;
            if (new_y1 >= _design_screen_help_list[6].y2) {
                new_y1 = _design_screen_help_list[6].y2;
            }
            _design_screen_help_list[6].y1 = static_cast<int16_t>(new_y1);
        }

        if (lines_2 > 0) {
            int32_t new_y1 = _design_screen_help_list[7].y1 + lines_2 * 14 + 15;
            if (new_y1 >= _design_screen_help_list[7].y2) {
                new_y1 = _design_screen_help_list[7].y2;
            }
            _design_screen_help_list[7].y1 = static_cast<int16_t>(new_y1);
        }

        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_design_screen_help_list, 13);
    }

    void __cdecl Set_Null_Modem_Setup_Screen_Help_List_(int16_t x_offset, int16_t y_offset) {
        _memcpy(&_null_modem_setup_screen_help_list, &_static_null_modem_setup_screen_help_list, 40);
        Offset_Help_Field_XYs_(_null_modem_setup_screen_help_list, 4, x_offset, y_offset);
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_null_modem_setup_screen_help_list, 4);
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
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_input_box_help_list, 1);
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
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_scrap_freighters_help_list, 1);
    }

    void __cdecl Set_Hotseat_Setup_Screen_Help_List_(int16_t x_offset, int16_t y_offset) {
        _memcpy(_hotseat_setup_screen_help_list, _static_hotseat_setup_screen_help, sizeof(_static_hotseat_setup_screen_help));
        Offset_Help_Field_XYs_(_hotseat_setup_screen_help_list, 4, x_offset, y_offset);
        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_hotseat_setup_screen_help_list, 4);
    }

    void __cdecl Set_Weapon_Replacement_Screen_Help_List_(int16_t total_rows, int16_t x, int16_t y, uint8_t needs_racks) {
        int16_t help_count = 0;

        int16_t height = animate::Get_Height_(MOX::_weapons_mid_box_seg);
        if (total_rows < 7) {
            height = 0x66;
        }

        int16_t base_y = height + animate::Get_Height_(MOX::_weapons_top_box_seg) + y;

        memset(_weapon_replacement_screen_help_list, 0, sizeof(_weapon_replacement_screen_help_list));

        int16_t y1 = base_y + 10;

        int16_t h = animate::Get_Height_(MOX::_beam_filter_button_seg) + y1;
        int16_t w = animate::Get_Width_(MOX::_beam_filter_button_seg) + x + 0x65;
        Append_Help_List_(_weapon_replacement_screen_help_list, &help_count, 12, 0x188, x + 0x65, y1, w, h);

        h = animate::Get_Height_(MOX::_missile_filter_button_seg) + y1;
        w = animate::Get_Width_(MOX::_missile_filter_button_seg) + x + 0xcb;
        Append_Help_List_(_weapon_replacement_screen_help_list, &help_count, 12, 0x189, x + 0xcb, y1, w, h);

        h = animate::Get_Height_(MOX::_bomb_filter_button_seg) + y1;
        w = animate::Get_Width_(MOX::_bomb_filter_button_seg) + x + 0x146;
        Append_Help_List_(_weapon_replacement_screen_help_list, &help_count, 12, 0x18a, x + 0x146, y1, w, h);

        h = animate::Get_Height_(MOX::_special_filter_button_seg) + y1;
        w = animate::Get_Width_(MOX::_special_filter_button_seg) + x + 0x1ad;
        Append_Help_List_(_weapon_replacement_screen_help_list, &help_count, 12, 0x18b, x + 0x1ad, y1, w, h);

        int16_t btn_x = x + 0x21a;

        int16_t cancel_y = base_y + 0x52;
        h = animate::Get_Height_(MOX::_weapons_cancel_button_seg) + cancel_y;
        w = animate::Get_Width_(MOX::_weapons_cancel_button_seg) + btn_x;
        Append_Help_List_(_weapon_replacement_screen_help_list, &help_count, 12, 0x18c, btn_x, cancel_y, w, h);

        int16_t accept_y = base_y + 0x82;
        h = animate::Get_Height_(MOX::_weapons_accept_button_seg) + accept_y;
        w = animate::Get_Width_(MOX::_weapons_accept_button_seg) + btn_x;
        Append_Help_List_(_weapon_replacement_screen_help_list, &help_count, 12, 0x18d, btn_x, accept_y, w, h);

        int16_t conditional_id = (needs_racks == 0) ? 0x18e : 0x18f;
        Append_Help_List_(_weapon_replacement_screen_help_list, &help_count, 12, conditional_id, x + 0x18, base_y + 0x45, x + 0xd2, base_y + 0xaa);

        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_weapon_replacement_screen_help_list, help_count);
    }

    void __cdecl Offset_Help_Field_XYs_(s_help_box* help_boxes, int16_t count, int16_t offset_x, int16_t offset_y) {
        for (int16_t i = 0; i < count; ++i) {
            help_boxes[i].x1 += offset_x;
            help_boxes[i].y1 += offset_y;
            help_boxes[i].x2 += offset_x;
            help_boxes[i].y2 += offset_y;
        }
    }

    void __cdecl Set_ColOff_Screen_Help_List_(int16_t* skills) {
        memcpy(_coloff_screen_help_list, _static_coloff_screen_help_list, sizeof(_static_coloff_screen_help_list));

        for (int16_t i = 0; i < 4; ++i) {
            if (skills[i] > 0) {
                _coloff_screen_help_list[i + 3].y1 += skills[i] * 17 + 16;
            }
        }

        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_coloff_screen_help_list, 16);
    }

    void __cdecl Set_ShipOff_Screen_Help_List_(int16_t* help_y_offsets, int16_t start_index) {
        int16_t count = 18;

        _memcpy(_shipoff_screen_help_list, _static_shipoff_screen_help_list, sizeof(_shipoff_screen_help_list));

        for (int16_t i = 0; i < 4; i++) {
            if (help_y_offsets[i] > 0) {
                _shipoff_screen_help_list[i + 2].y1 += help_y_offsets[i] * 17 + 16;
            }
        }

        if (start_index == 0) {
            Append_Help_List_(_shipoff_screen_help_list, &count, 33, 333, 301, 16, 610, 198);
        } else {
            for (int16_t i = start_index; i < 15; i++) {
                int16_t x1 = (i % 5) * 63 + 301;
                int16_t y1 = (i / 5) * 62 + 16;
                Append_Help_List_(_shipoff_screen_help_list, &count, 33, 333, x1, y1, x1 + 63, y1 + 62);
            }
        }

        MAINMENU::Set_Main_Menu_Screen_Help_List_Aux_(_shipoff_screen_help_list, count);
    }

    void __cdecl Append_Help_List_(s_help_box* help_list, int16_t* list_count, int32_t unused, int16_t help_id, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        int16_t count = *list_count;
        help_list[count].help_id = help_id;
        help_list[count].x1 = x1;
        help_list[count].y1 = y1;
        help_list[count].x2 = x2;
        help_list[count].y2 = y2;
        *list_count = count + 1;
    }
}