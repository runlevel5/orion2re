#include "pch.h"

namespace COLGCBT {

    void __cdecl Calc_Top_Line_() {
        if (COLONY::_colony_bldgs[0].buildings[0] > 0) {
            COLONY::_colony_bldgs[0].buildings[0] = 0;
            _top_line = 1;
            return;
        }

        if (COLONY::_colony_bldgs[1].buildings[0] > 0 ||
            COLONY::_colony_bldgs[0].buildings[1] > 0 ||
            COLONY::_colony_bldgs[1].buildings[1] > 0) {
            _top_line = 1;
            return;
        }

        if (COLONY::_colony_bldgs[2].buildings[0] > 0 ||
            COLONY::_colony_bldgs[0].buildings[2] > 0 ||
            COLONY::_colony_bldgs[2].buildings[1] > 0 ||
            COLONY::_colony_bldgs[1].buildings[2] > 0 ||
            COLONY::_colony_bldgs[2].buildings[2] > 0) {
            _top_line = 2;
            return;
        }

        _top_line = 3;
    }

    void __cdecl Replace_Colgcbt_Color_With_Player_Colors_(s_animation_header* anim, int16_t player_idx)
    {
        uint8_t color = static_cast<uint8_t>(MOX::_player[player_idx].color);

        if (color != 2) {
            replace::Replace_Color_Range_(anim, &_player_color_array + color * 8, 0xC0, 8);
            replace::Replace_Color_Range_(anim, &_short_player_color_array + color * 8, 0xE8, 4);
        }
    }

    void __cdecl Load_Combat_Anims_()
    {
        _explosion_anim = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLONY_LBX, 7, MOX::_screen_seg);

        _anims[8] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 0, MOX::_screen_seg);
        _anims[5] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 1, MOX::_screen_seg);
        _anims[7] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 2, MOX::_screen_seg);
        _anims[9] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 3, MOX::_screen_seg);
        _anims[6] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 4, MOX::_screen_seg);

        Replace_Colgcbt_Color_With_Player_Colors_(_anims[8], _g_attack_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[5], _g_attack_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[7], _g_attack_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[9], _g_attack_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[6], _g_attack_ptr->player_id);

        _anims[13] = _anims[8];
        _anims[10] = _anims[5];
        _anims[12] = _anims[7];
        _anims[14] = _anims[9];
        _anims[11] = _anims[6];

        _anims[18] = _anims[8];
        _anims[15] = _anims[5];
        _anims[17] = _anims[7];
        _anims[19] = _anims[9];
        _anims[16] = _anims[6];

        _anims[28] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 5, MOX::_screen_seg);
        _anims[25] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 6, MOX::_screen_seg);
        _anims[27] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 7, MOX::_screen_seg);
        _anims[29] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 8, MOX::_screen_seg);
        _anims[26] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 9, MOX::_screen_seg);

        Replace_Colgcbt_Color_With_Player_Colors_(_anims[28], _g_defend_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[25], _g_defend_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[27], _g_defend_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[29], _g_defend_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[26], _g_defend_ptr->player_id);

        _anims[33] = _anims[28];
        _anims[30] = _anims[25];
        _anims[32] = _anims[27];
        _anims[34] = _anims[29];
        _anims[31] = _anims[26];

        _anims[38] = _anims[28];
        _anims[35] = _anims[25];
        _anims[37] = _anims[27];
        _anims[39] = _anims[29];
        _anims[36] = _anims[26];

        if (MOX::_player[_g_defend_ptr->player_id].tech_applications[24] == 3) {
            _anims[23] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 15, MOX::_screen_seg);
            _anims[20] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 16, MOX::_screen_seg);
            _anims[22] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 17, MOX::_screen_seg);
            _anims[24] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 18, MOX::_screen_seg);
            _anims[21] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 19, MOX::_screen_seg);
        } else {
            _anims[23] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 10, MOX::_screen_seg);
            _anims[20] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 11, MOX::_screen_seg);
            _anims[22] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 12, MOX::_screen_seg);
            _anims[24] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 13, MOX::_screen_seg);
            _anims[21] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 14, MOX::_screen_seg);
        }

        Replace_Colgcbt_Color_With_Player_Colors_(_anims[23], _g_defend_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[20], _g_defend_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[22], _g_defend_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[24], _g_defend_ptr->player_id);
        Replace_Colgcbt_Color_With_Player_Colors_(_anims[21], _g_defend_ptr->player_id);

        _heart_anim = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 20, MOX::_screen_seg);
        _frame_anim = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLGCBT_LBX, 21, MOX::_screen_seg);
    }

    void __cdecl Colony_Combat_Screen_(s_invasion_side_info* attacker, s_invasion_side_info* defender, uint32_t seed, int16_t param_4) {
        bool is_exit = false;
        MOX2::action_func_t auto_function_ptr;

        MOX::_at_least_one_tactical_combat = 1;
        _start_combat = 0;
        _display_fast = 0;
        _title_changed_flag = 0;

        _g_attack_ptr = attacker;
        _g_defend_ptr = defender;

        _N_ATK_UNIT_ROWS = 3;
        _N_DEF_UNIT_ROWS = 3;

        COLONY::Set_Colony_Font_();
        COLONY::Load_Colony_Screen_Seg_();
        Load_Combat_Anims_();

        COLONY::_using_colony_screen_palette = 1;
        COLONY::Set_Colony_Globals_(param_4);
        COLONY::_sys_disp_star_handle = COLONY::_star_handle;
        COLONY::_drawing_display = 0;
        COLONY::_valid_colony = 1;

        COLONY::Load_Planet_Screen_();
        FLCQUEUE::Free_Flic_Queue_And_Pointers_();
        Set_Up_Troops_();

        if (COLONY::_valid_colony != 0) {
            COLONY::Make_Bldg_Array_For_Colony_(static_cast<int16_t>(COLONY::_colony_handle), 1);
        }

        mouse::Set_Mouse_List_(&_mouse_list_blank, 1);
        ERIC::Fast_Fade_Out_();
        COLONY::Update_Colony_Palette_();
        palstore::Darken_Palette_(100);
        palette::Clear_Palette_Changes_(0, 255);
        palette::Refresh_Palette_();

        COLONY::Reset_Field_Arrays_();
        Add_Global_Fields_();
        COLONY::_full_draw = 1;

        MOX2::Get_Auto_Function_(&auto_function_ptr);
        fields::Assign_Auto_Function_(Draw_Colony_Combat_Screen_, 1);
        Set_Help_List_();

        graphics::Fill_(0, 0, 639, 479, 0);
        Draw_Colony_Combat_Screen_();

        palstore::Darken_Palette_(100);
        palette::Clear_Palette_Changes_(0, 255);
        RUSS::Mox_Sync_Update_();
        ERIC::Fast_Fade_In_();

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        video::Set_Refresh_Stencil_();
        fields::Clear_Mouse_Buffer_();
        fields::Set_Input_Delay_(5);

        timer::Mark_Time_();
        timer::Release_Time_(16);

        random::Set_Random_Seed_(seed);

        do {
            timer::Mark_Time_();
            JIM::Play_Streaming_Music_(10, -1, 1);
            Add_Global_Fields_();
            Draw_Colony_Combat_Screen_();
            RUSS::Mox_Sync_Update_();

            int16_t input = fields::Get_Input_();
            fields::Scan_Input_();

            if (input != 0) {
                if (_display_fast == 0 &&
                    static_cast<uint8_t>(attacker->current_troop_type_idx) < 4 &&
                    static_cast<uint8_t>(defender->current_troop_type_idx) < 4) {
                    _display_fast = 1;
                }
                else {
                    is_exit = true;
                }
            }

            if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
                COLONY::_full_draw = 1;
                MOX::_FULL_DRAW_SCREEN_FLAG = 0;
                Set_Help_List_();
            }

            timer::Release_Time_(1);
        } while (!is_exit);

        JIM::Play_Background_Music_();
        sound::Stop_All_Sounds_();
        COLONY::Reset_Field_Mode_();
        fields::Clear_Fields_();
        fields::Deactivate_Help_List_();
        fields::Deactivate_Auto_Function_();

        MOX2::Put_Auto_Function_(&auto_function_ptr);
        FLCQUEUE::Allocate_Flic_Queue_And_Pointers_();

        MOX::_current_screen = MOX::_return_screen;
        COLONY::_using_colony_screen_palette = 0;
    }

    void __cdecl Set_Help_List_() {
        ERICHELP::Eric_Set_Help_List_(_colony_ground_combat_screen_help, 3);
    }

    void __cdecl Draw_Summary_Box_ATK_() {
        int16_t row = 0;

        if (COLONY::_full_draw != 0) {
            graphics::Darken_Fill_(2, 0x29, 0x103, 0xb8);
            animate::Draw_(1, 0x28, _frame_anim);
            COLONY::Set_Colony_Font_To_Player_(2, static_cast<int8_t>(_g_attack_ptr->player_id));
            char* str = ESTRINGS::E_Strings_(0xbb);
            fonts::Print_Centered_(0x82, row * 11 + 50, str);

            row++;
            Print_Best_Summary_(_g_attack_ptr, &row);
            Print_Race_Summary_(_g_attack_ptr, &row);

            row++;
            Print_Troop_Totals_(1, row * 11 + 50, _g_attack_ptr);
        }
    }

    int16_t __cdecl Get_Next_Troop_Type_(uint8_t side) {
        for (int16_t i = 0; i < 4; ++i) {
            if (side == 0) {
                if (N_Troop_Type_(i, side) < _g_attack_ptr->unit_counts[i]) {
                    return i;
                }
            } else {
                if (N_Troop_Type_(i, side) < _g_defend_ptr->unit_counts[i]) {
                    return i;
                }
            }
        }
        return -1;
    }

    s_animation_header* __cdecl Action_Anim_(int8_t side, uint8_t state, int8_t type) {
        switch (state) {
            case 0:
                return _explosion_anim;
            case 1:
                return _anims[(type + side * 4) * 5];
            case 2:
                return _anims[(type + side * 4) * 5 + 1];
            case 3:
                return _anims[(type + side * 4) * 5 + 2];
            case 4:
                return _anims[(type + side * 4) * 5 + 3];
            case 5:
                return _anims[(type + side * 4) * 5 + 4];
            default:
                return nullptr;
        }
    }

    void __cdecl Resort_Units_(uint8_t side_idx) {
        s_ground_unit temp;
        int16_t passes = 40;

        do {
            for (int16_t i = 0; i < 39; ++i) {
                s_animation_header* anim1 = Action_Anim_(
                    _g_units[side_idx][i].side,
                    _g_units[side_idx][i].state,
                    _g_units[side_idx][i].type
                );

                s_animation_header* anim2 = Action_Anim_(
                    _g_units[side_idx][i + 1].side,
                    _g_units[side_idx][i + 1].state,
                    _g_units[side_idx][i + 1].type
                );

                int16_t height1 = animate::Get_Height_(anim1);
                int16_t height2 = animate::Get_Height_(anim2);

                if (_g_units[side_idx][i + 1].y + height2 < _g_units[side_idx][i].y + height1) {
                    struct_::Copy_Structure_(&temp, &_g_units[side_idx][i], 25);
                    struct_::Copy_Structure_(&_g_units[side_idx][i], &_g_units[side_idx][i + 1], 25);
                    struct_::Copy_Structure_(&_g_units[side_idx][i + 1], &temp, 25);
                }
            }
            --passes;
        } while (passes != 0);
    }

    void __cdecl Draw_Summary_Box_DEF_() {
        int16_t line_y_offset = 0;

        if (COLONY::_full_draw != 0) {
            graphics::Darken_Fill_(0x17B, 0x29, 0x27E, 0xB8);
            animate::Draw_(0x17A, 0x28, _frame_anim);

            COLONY::Set_Colony_Font_To_Player_(2, static_cast<int8_t>(_g_defend_ptr->player_id));

            const char* str = ESTRINGS::E_Strings_(0x100);
            fonts::Print_Centered_(0x1FC, line_y_offset * 11 + 50, str);

            line_y_offset++;

            Print_Best_Summary_(_g_defend_ptr, &line_y_offset);
            Print_Race_Summary_(_g_defend_ptr, &line_y_offset);

            line_y_offset++;

            Print_Troop_Totals_(0x17A, line_y_offset * 11 + 50, _g_defend_ptr);
        }
    }

    int16_t __cdecl N_Slots_Left_(uint8_t side) {
        int16_t count = 0;
        for (int16_t i = 0; i < 40; ++i) {
            if (_g_units[side][i].state == 0) {
                count++;
            }
        }
        return count;
    }

    void __cdecl Spatter_Down_Starting_Troops_() {
        for (uint8_t side = 0; side < 2; ++side) {
            for (int16_t i = 0; i < 40; ++i) {
                _g_units[side][i].x = 0;
                _g_units[side][i].y = 0;
                _g_units[side][i].state = 0;
            }

            int16_t slots_left = N_Slots_Left_(side);
            bool done = (slots_left == 0);

            if (slots_left > 10) {
                slots_left = 10;
            }

            while (!done) {
                Add_Initial_Unit_(side);
                slots_left--;

                if (slots_left == 0 || Get_Next_Troop_Type_(side) == -1) {
                    done = true;
                }
            }

            Resort_Units_(side);
        }
    }

    void __cdecl Set_Up_Troops_() {
        memset(&_g_units, 0, sizeof(_g_units));
        Calc_Top_Line_();
        Spatter_Down_Starting_Troops_();
    }

    void __cdecl Add_Global_Fields_() {
        fields::Clear_Fields_();

        _fields[0] = fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, ESTRINGS::E_Strings_(0xc), 0);
        _fields[1] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(0x87));
    }

    void __cdecl Draw_Summary_Boxes_() {
        Draw_Summary_Box_ATK_();
        Draw_Summary_Box_DEF_();
    }

    void __cdecl Draw_Summary_() {
        bool attackers_defeated = true;

        for (int i = 0; i < 4; ++i) {
            if (_g_attack_ptr->unit_counts[i] > 0) {
                attackers_defeated = false;
            }
        }

        COLONY::Set_Colony_Font_To_(4);

        int16_t string_id;
        if (attackers_defeated) {
            string_id = 0x3D;
        } else {
            string_id = 0x3E;
        }

        char* race_name = MOX::_player[_g_attack_ptr->player_id].race_name;
        char* planet_name = HACCESS::Get_Planet_Name_(MOX::_colony[COLONY::_colony_handle].planet);
        char* format_str = ESTRINGS::E_Strings_(string_id);

        char buffer[500];
        _sprintf(buffer, format_str, planet_name, race_name);

        fonts::Print_Centered_(319, 10, buffer);
    }

    void __cdecl Draw_Troops_() {
        Verify_Legal_Totals_();

        for (uint8_t side = 0; side < 2; ++side) {
            for (int16_t unit_idx = 0; unit_idx < 40; ++unit_idx) {
                Do_Unit_Turn_(&_g_units[side][unit_idx]);
            }
            Force_Firing_Pairs_();
            Reinforce_Units_(side);
            Resort_Units_(side);
        }
    }

    void __cdecl Print_Best_Summary_(s_invasion_side_info* param_1, int16_t* param_2) {
        if (param_1->combat_modifiers.antigrav_bonus != 0) {
            Print_Data_(TECHDATA::_technology_applications[9].name,
                        static_cast<uint16_t>(static_cast<uint8_t>(param_1->combat_modifiers.antigrav_bonus)),
                        param_2,
                        param_1);
        }

        if (param_1->combat_modifiers.best_armor_tech_app_id != 0) {
            Print_Data_(TECHDATA::_technology_applications[param_1->combat_modifiers.best_armor_tech_app_id].name,
                        static_cast<uint16_t>(static_cast<uint8_t>(param_1->combat_modifiers.troop_bonus)),
                        param_2,
                        param_1);
        }

        if (param_1->combat_modifiers.personal_shield_tech_app_id != 0) {
            Print_Data_(TECHDATA::_technology_applications[param_1->combat_modifiers.personal_shield_tech_app_id].name,
                        static_cast<uint16_t>(static_cast<uint8_t>(param_1->combat_modifiers.personal_shield_bonus)),
                        param_2,
                        param_1);
        }

        if (param_1->combat_modifiers.rifle_tech_app_id != 0) {
            Print_Data_(TECHDATA::_technology_applications[param_1->combat_modifiers.rifle_tech_app_id].name,
                        static_cast<uint16_t>(static_cast<uint8_t>(param_1->combat_modifiers.rifle_bonus)),
                        param_2,
                        param_1);
        }
    }

    void __cdecl Draw_Colony_Combat_Screen_() {
        uint32_t random_seed = random::Get_Random_Seed_();
        video::Set_Page_Off_();

        if (COLONY::_full_draw != 0) {
            graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
            animate::Draw_(0, 0, COLONY::C_Anims_(1));
            animate::Draw_(0, 0, COLONY::C_Anims_(0));
        }

        COLONY::Set_Colony_Font_To_(1);
        Draw_Summary_Boxes_();
        COLONY::Set_Colony_Font_To_(1);
        COLONY::Draw_Colony_Bldgs_();

        if (COLONY::_full_draw != 0) {
            video::Copy_Off_To_Back_();
            video::Set_Refresh_Stencil_();
            COLONY::_full_draw = 0;
        }

        COLONY::Set_Colony_Font_To_(3);

        if (static_cast<uint8_t>(_g_attack_ptr->current_troop_type_idx) < 4 &&
            static_cast<uint8_t>(_g_defend_ptr->current_troop_type_idx) < 4) {

            char buffer[500];
            char* race_name = MOX::_player[_g_attack_ptr->player_id].race_name;
            char* planet_name = HACCESS::Get_Planet_Name_(MOX::_colony[COLONY::_colony_handle].planet);
            _sprintf(buffer, ESTRINGS::E_Strings_(0x2F), planet_name, race_name);
            fonts::Print_Centered_(0x13F, 10, buffer);

        } else {
            Draw_Summary_();
            if (_title_changed_flag == 0) {
                _title_changed_flag = 1;
                COLONY::_full_draw = 1;
            }
        }

        graphics::Set_Window_(0, 0, 0x27F, 0x1DF);
        video::Clip_On_();
        Draw_Troops_();
        FLCQUEUE::Draw_Flic_Queue_();
        video::Clip_Off_();

        random::Set_Random_Seed_(random_seed);

        do {
            _g_ctr++;

            bool combat_active = static_cast<uint8_t>(_g_attack_ptr->current_troop_type_idx) < 4 &&
                                 static_cast<uint8_t>(_g_defend_ptr->current_troop_type_idx) < 4;

            if (combat_active) {
                if (_start_combat == 0) {
                    _start_combat = 1;
                } else {
                    if (static_cast<int16_t>(_g_ctr) % 6 == 0 || _display_fast != 0) {
                        invasion::Ground_Combat_Round_(_g_attack_ptr, _g_defend_ptr);
                    }

                    if (_display_fast == 0) {
                        timer::Release_Time_(1);
                    }
                }
            } else {
                if (_start_combat == 0) {
                    _start_combat = 1;
                }
            }

            if (_display_fast == 0) {
                break;
            }

        } while (static_cast<uint8_t>(_g_attack_ptr->current_troop_type_idx) < 4 &&
                 static_cast<uint8_t>(_g_defend_ptr->current_troop_type_idx) < 4);
    }

    void __cdecl Print_Troop_Totals_(int16_t x, int16_t y, s_invasion_side_info* side_info) {
        int16_t active_unit_count = 0;
        int16_t valid_troop_idx = 0;
        char local_str[500];

        for (int i = 0; i < 4; i++) {
            if (side_info->unit_counts[i] != 0) {
                active_unit_count++;
            }
        }

        int16_t spacing = 261 / (active_unit_count + 1);
        int16_t cur_x = x + spacing;

        for (int j = 0; j < 4; j++) {
            if (side_info->unit_counts[j] != 0) {
                uint8_t race = static_cast<uint8_t>(MOX::_player[side_info->player_id].race);
                s_animation_header* anim = COLONY::Player_Troop_Anim_(side_info->player_id, j, race);

                COLONY::Set_Colony_Font_To_Player_(1, static_cast<int8_t>(_g_attack_ptr->player_id));

                int32_t strength = side_info->unit_strengths[j] - side_info->combat_modifiers.difficulty_bonus;
                _sprintf(local_str, ESTRINGS::E_Strings_(13), strength);
                fonts::Print_Centered_(cur_x, 176, local_str);

                uint8_t hp = static_cast<uint8_t>(side_info->unit_hps[j]);
                if (hp <= 1) {
                    _sprintf(local_str, ESTRINGS::E_Strings_(22), hp);
                } else {
                    _sprintf(local_str, ESTRINGS::E_Strings_(23), hp);
                }
                fonts::Print_Centered_(cur_x, 167, local_str);

                if (MOX::_settings.language == 4) {
                    _sprintf(local_str, s_percent_s_percent_d_00556d98,
                            COLONY::Player_Troop_Name_(side_info->player_id, j, race),
                            side_info->unit_counts[j]);
                } else {
                    _sprintf(local_str, ESTRINGS::E_Strings_(14),
                            side_info->unit_counts[j],
                            COLONY::Player_Troop_Name_(side_info->player_id, j, race));
                }
                fonts::Print_Centered_(cur_x, 158, local_str);

                ERIC::Draw_Bottom_Centered_(x + (valid_troop_idx + 1) * spacing, 157, anim);

                // This call effectively acts as a no-op since the result is never printed
                _sprintf(local_str, ESTRINGS::E_Strings_(124), side_info->unit_counts[j]);

                valid_troop_idx++;
                cur_x += spacing;
            }
        }

        if (valid_troop_idx != _old_ctr) {
            COLONY::_full_draw = 1;
            _old_ctr = valid_troop_idx;
        }
    }

} // namespace COLGCBT