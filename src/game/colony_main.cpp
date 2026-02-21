#include "deps.h"

namespace COLONY {
    void __cdecl Draw_Colony_Bldg_(int16_t col, int16_t row) {
        if (col >= 0 && col < 6 && row >= 0 && row < 6) {
            int32_t building_id = _colony_bldgs[col].buildings[row];
            if (building_id != 0) {
                s_animation_header* seg = COLBCACH::Cache_Load_Bldg_(col, row, building_id);

                if (_full_draw) {
                    int16_t prev_frame = animate::Get_Current_Frame_(seg);
                    animate::Reset_Animation_Frame_(seg);

                    while (animate::Get_Current_Frame_(seg) < prev_frame) {
                        animate::Draw_(0, 0, seg);
                    }

                    if (_full_draw) {
                        animate::Draw_(0, 0, seg);
                        return;
                    }
                }

                if (_scanned_bldg_C == col && _scanned_bldg_R == row) {
                    if (ERIC::Global_Cycler_(30, 2)) {
                        Draw_Bldg_CR_Dark_(col, row);
                        return;
                    }
                }

                animate::Draw_No_Glass_(0, 0, seg);
            }
        }
    }

    void __cdecl Draw_Colony_Bldgs_() {
        bool is_scanning_valid = (_scanned_bldg_C != -1 && _scanned_bldg_R != -1);

        for (int16_t i = 0; i < 36 /* 0x24 */; ++i) {
            uint16_t row, col;
            Bldg_Back_To_Front_(i, &col, &row);

            if (_full_draw != 0 || (_scanned_bldg_C != -1 && _scanned_bldg_R != -1)) {
                COLDRAW::Draw_Veggie_CR_(col, row, !is_scanning_valid);

                int32_t building_id = _colony_bldgs[col].buildings[row];
                if (building_id != 0) {
                    s_animation_header* anim = COLBCACH::Cache_Load_Bldg_(col, row, building_id);
                    int16_t frame_count = animate::Get_Animation_Frame_Count_(anim);

                    if (frame_count > 1 || _full_draw != 0 || is_scanning_valid) {
                        Draw_Colony_Bldg_(col, row);
                    }
                }
            }
        }
    }

    void __cdecl Draw_Colony_Screen_() {
        video::Set_Page_Off_();

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            _full_draw = 1;
        }

        if (_full_draw != 0) {
            graphics::Fill_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, 0);

            s_animation_header* background_anim = C_Anims_(1);
            animate::Draw_(0, 0, background_anim);

            s_animation_header* base_anim = C_Anims_(0);
            animate::Draw_(0, 0, base_anim);

            COLDRAW::Draw_Road_List_();

            if (_valid_colony != 0) {
                Draw_Colony_Info_Background_();
            }

            if (_drawing_display == 0 || _drawing_display == 2) {
                if (MAINSCR::_debug_enabled != 0) {
                    char* msg = ESTRINGS::E_Strings_(274 /* ESTR_DRAW_SYSTEM */);
                    fonts::PSTRS_(0, 0, msg);
                }
                Set_Colony_Font_();
                COLSYSDI::Draw_Col_Sys_Disp_(7, 24 /* 0x18 */);
            }
            else if (_drawing_display == 1) {
                if (MAINSCR::_debug_enabled != 0) {
                    char* msg = ESTRINGS::E_Strings_(273 /* ESTR_DRAW_GALAXY */);
                    fonts::PSTRS_(0, 0, msg);
                }

                int16_t height = MOX::_galaxy_map_height;
                int16_t grid_y = MOX::_galaxy_map_grid_y;
                s_animation_header* anim_c = C_Anims_(12 /* 0xc */);
                s_animation_header* anim_b = C_Anims_(11 /* 0xb */);

                MOVEBOX::Draw_Galaxy_Map_Box_(anim_b, anim_c, 7, 24 /* 0x18 */, height, grid_y, 11 /* 0xb */, 31 /* 0x1f */, 0, 0, 3, 0);
            }

            if (_valid_colony != 0) {
                Draw_Colony_Bldgs_();
                Draw_Colony_Satellites_();
            }

            video::Copy_Off_To_Back_();
            video::Set_Refresh_Stencil_();
            _full_draw = 0;
        }
        else {
            if (_valid_colony != 0) {
                Draw_Colony_Bldgs_();
                Draw_Colony_Satellites_();
            }
        }

        if (_drawing_display == 1) {
            Box_Galaxy_Map_Star_(_star_handle, 0);
            if (_star_handle != _sys_disp_star_handle) {
                Box_Galaxy_Map_Star_(_sys_disp_star_handle, 1);
            }
        }

        if (_valid_colony != 0) {
            Draw_Colony_Info_();
        }
        else {
            No_Op1_();
        }

        FAKEBUTS::Draw_Fake_Buttons_();
        fields::Draw_Visible_Fields_();

        if (_field_mode == 1) {
            int16_t mouse_y = mouse::Pointer_Y_();
            int16_t mouse_x = mouse::Pointer_X_();
            COLMOVE::Draw_Cluster_(mouse_x, mouse_y);
        }

        palette::Cycle_Palette_Color_(255 /* 0xff */, 19 /* 0x13 */, 16 /* 0x10 */, 9, 32 /* 0x20 */, 40 /* 0x28 */, 21 /* 0x15 */, 2);
    }

    void __cdecl Show_Colony_Screen_() {
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        fields::Deactivate_Auto_Function_();
        _full_draw = 1;
        fields::Assign_Auto_Function_(Draw_Colony_Screen_, 1);
        ERICHELP::Set_Colony_Screen_Help_List_();
    }

    void __cdecl Draw_Colony_Info_() {
        Draw_Colony_Info_Military_();
        Draw_Colony_Info_Officer_();
        Draw_Colony_Info_Production_();
        Draw_Colony_Info_Pop_();
        Draw_Colony_Info_Morale_();
        Draw_Info_Name_And_Pop_();
        Draw_Info_Build_();
        Draw_Sys_Disp_Scanned_();
        Draw_Colony_Info_Scanned_();
    }

    void __cdecl Draw_Planet_Summary_(int16_t x, int16_t y, int16_t planet_idx) {
        char buffer[2000];

        fonts::Set_Font_Style_Shadow_Down_(1, &_font_color_array);

        graphics::Fill_(x + 5, y + 2, x + 0xb0, y + 0xc3, 0xe9);

        s_animation_header* planet_anim = Mini_Planet_Anim_(planet_idx);
        remap::Calculate_Picture_Remap_Colors_(planet_anim);

        s_animation_header* ui_anim = C_Anims_(0xe);
        animate::Draw_(x, y, ui_anim);

        MAINPUPS::Build_Generic_Planet_Or_Colony_Description_String_Formatted_(planet_idx, buffer);

        BILL::Squeeze_Print_Formatted_Paragraph_(x + 14, y + 45, 154, 140, buffer, 0);
    }

    int16_t __cdecl Bldg_Coords_To_Effective_Frame_(int16_t x, int32_t y) {
        if ((y % 2) == 0) {
            return (int16_t)y * 6 + x;
        }
        else {
            return ((int16_t)y * 6 - x) + 5;
        }
    }

    void __cdecl Colony_Screen_() {
        int16_t input_val;
        int16_t scanned_val;
        bool input_received = false;
        bool quit_flag = false;

        uint32_t saved_seed = random::Get_Random_Seed_();
        random::Set_Random_Seed_(static_cast<uint32_t>(_colony_handle));
        video::Clip_Off_();
        _using_colony_screen_palette = 1;

        _star_handle = (int32_t)MOX::_screen_data;
        _orbit_n = (int32_t)_orbit_temp;
        int16_t planet_idx = HAROLD::Planet_Index_(MOX::_screen_data, _orbit_temp);
        _planet_handle = (int32_t)planet_idx;

        _colony_handle = MOX::_planet[_planet_handle].colony_index;
        _g_scanned_star = -1;
        _g_scanned_planet = -1;
        _sys_disp_star_handle = _star_handle;

        if (_colony_handle == -1) {
            _valid_colony = 0;
            _drawing_display = 2;
            _planet_display_handle = static_cast<uint16_t>(planet_idx);
        }
        else {
            _valid_colony = 1;
            _drawing_display = 0;
            _g_scanned_planet = -1;
        }

        fields::Clear_Mouse_Buffer_();
        fields::Set_Input_Delay_(3);
        ERICHELP::Set_Colony_Screen_Help_List_();

        int16_t saved_player_num = MOX::_PLAYER_NUM;
        MOX::_PLAYER_NUM = static_cast<int16_t>(MOX::_colony[_colony_handle].owner);

        Load_Colony_Screen_Seg_();
        Load_Planet_Screen_();

        if (!_valid_colony) {
            MOX::_current_screen = MOX::_return_screen;
            return;
        }

        Make_Bldg_Array_For_Colony_(_colony_handle, 0);

        if (MOX::_return_screen != MOX::_current_screen) {
            _colony_saved_screen = MOX::_return_screen;
        }

        fields::Clear_Fields_();
        Set_Colony_Font_();
        mouse::Set_Mouse_List(COLGCBT::_mouse_list_blank, 1);
        video::Set_Page_Off_();
        video::Set_Refresh_Stencil_();

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            ERIC::Fast_Fade_Out_();
        }

        Update_Colony_Palette_();

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            palstore::Darken_Palette_(100);
            palette::Clear_Palette_Changes_(0, 255);
        }

        palette::Refresh_Palette_();
        Reset_Field_Arrays_();
        Update_Fields_();
        _full_draw = 1;

        fields::Assign_Auto_Function_(Draw_Colony_Screen_, 1);
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
        Draw_Colony_Screen_();
        ERIC::Fast_Fade_In_();
        video::Set_Refresh_Stencil_();
        mouse::Set_Mouse_List(MOX::_mouse_list_arrow, 1);
        Do_Informational_And_Decision_Popups_();

        while (true) {
            timer::Mark_Time_();
            _g_count++;

            if (input_received) {
                Update_Fields_();
                input_val = 0;
            }

            Draw_Colony_Screen_();
            MOX::Mox_Sync_Update_();

            input_val = fields::Get_Input_();
            scanned_val = fields::Scan_Input_();
            input_received = (input_val != 0);

            if (!Evaluate_Input_(&input_val, &scanned_val)) {
                quit_flag = true;
            }

            timer::Release_Time_(1);

            if (quit_flag) break;

            if (_valid_colony) {
                if (input_val == _colony_fields[1]) break;
            }
            else {
                if (input_val == _colony_fields[0] && input_val == _colony_fields[1]) break;
            }
        }

        Reset_Field_Mode_();
        fields::Clear_Fields_();
        Clear_Fake_Buttons_();
        fields::Deactivate_Auto_Function_();

        MOX::_PLAYER_NUM = saved_player_num;
        _using_colony_screen_palette = 0;

        if (!quit_flag) {
            Fix_Colony_Queue_(_colony_handle);
            if (MOX::_return_screen == MOX::_current_screen) {
                MOX::_current_screen = _colony_saved_screen;
                MOX::_return_screen = _colony_saved_screen;
            }
            else {
                MOX::_current_screen = MOX::_return_screen;
            }
        }

        fields::Deactivate_Help_List_();
        random::Set_Random_Seed_(saved_seed);
    }

    void __cdecl Load_Colony_Screen_Seg_() {
        COLBLDG::Load_Build_Anims_();
        MOX::Mox_Update_();
        COLXPORT::Load_Xport_Anims_();
        MOX::Mox_Update_();
        Load_Ship_Bitm_();
        MOX::Mox_Update_();
        Load_Bldg_Bitm_();
        MOX::Mox_Update_();
        Load_Officer_Anim_();
        MOX::Mox_Update_();
        COLDRAW::Load_Road_List_Anims_();
        COLBLDG::Clear_Last_App_Desc_();
        MOX::Mox_Update_();
        Load_Galaxy_Map_Anims_();
        MOX::Mox_Update_();

        _c_anims[19] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 1, MOX::_screen_seg);
        _c_anims[20] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 2, MOX::_screen_seg);
        _c_anims[21] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 4, MOX::_screen_seg);
        _c_anims[22] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 7, MOX::_screen_seg);
        _c_anims[23] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 8, MOX::_screen_seg);
        _c_anims[24] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 9, MOX::_screen_seg);
        _c_anims[32] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 21, MOX::_screen_seg);
    }

    void __cdecl Load_Officer_Anim_() {
        int8_t officer_idx = MOX::_star[COLONY::_star_handle].officer_index[MOX::_PLAYER_NUM];

        if (officer_idx != -1) {
            _c_anims[30] = (s_animation_header*)OFFICER::Load_Officer_Picture_(
                (int32_t)MOX::_PLAYER_NUM,
                (int16_t)officer_idx,
                (uint32_t)(uintptr_t)MOX::_screen_seg,
                0,
                1
            );
        }
    }


    void __cdecl Draw_Building_With_Bottom_Centered_(int16_t x, int16_t y, int16_t type, int16_t col, int16_t row) {
        s_animation_header* bldg_anim = COLBCACH::Cache_Load_Bldg_(col, row, type);

        int32_t cr_x = CR_To_XY_(col, row, 0);
        int32_t cr_y = CR_To_XY_(col, row, 1);

        bitmap::Create_Blank_Bitmap_(120, 120, COLBLDG::_bldg_bitm);

        int16_t offset_x = -(static_cast<int16_t>(cr_x) - 60);
        int16_t offset_y = -(static_cast<int16_t>(cr_y) - 120);

        bitmap::Add_Picture_To_Bitmap_(offset_x, offset_y, bldg_anim, (s_animation_header*)COLBLDG::_bldg_bitm);
        bitmap::Mask_Out_Colors_(COLBLDG::_bldg_bitm, 240, 255);

        int16_t min_x, min_y, actual_w, actual_h;
        bitmap::Get_Bitmap_Actual_Size_((s_animation_header*)COLBLDG::_bldg_bitm, &min_x, &min_y, &actual_w, &actual_h);

        int16_t draw_x = x - (actual_w / 2) - min_x;
        int16_t draw_y = y - actual_h - min_y;

        animate::Draw_(draw_x, draw_y, (s_animation_header*)COLBLDG::_bldg_bitm);
    }

    s_animation_header* __cdecl People_Anim_(int16_t param_1, int16_t param_2, int16_t param_3) {
        if (param_2 == 2) {
            int32_t entry_num = (static_cast<int32_t>(param_3) * 13) + (static_cast<int32_t>(param_1) * 2) + 1;
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_RACEICON_LBX, entry_num, MOX::_global_cache_seg));
        }

        if (param_2 == 0) {
            int32_t entry_num = (static_cast<int32_t>(param_3) * 13) + (static_cast<int32_t>(param_1) * 2);
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_RACEICON_LBX, entry_num, MOX::_global_cache_seg));
        }

        if (param_2 == 3) {
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_RACEICON_LBX, 0xAA, MOX::_global_cache_seg));
        }

        if (param_2 == 4) {
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_RACEICON_LBX, 0xA9, MOX::_global_cache_seg));
        }

        return nullptr;
    }

    s_animation_header* __cdecl Satellite_Anim_(int16_t colony_id) {
        int16_t pic_index = Satellite_Anim_Pic_(colony_id);
        return Buffer_Reload_Colony_(pic_index);
    }

    s_animation_header* __cdecl C_Anims_(int16_t index) {
        MOX::Mox_Update_();

        if (index > 0x23) {
            return nullptr;
        }

        switch (index) {
        case 0: {
            auto& colony = MOX::_colony[COLONY::_colony_handle];
            auto& planet = MOX::_planet[colony.planet];
            int16_t reload_arg = (int16_t)planet.climate_bg_type + (int16_t)colony.climate * 3;
            return Buffer_Reload_Planets_(reload_arg);
        }

        case 1:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x31, MOX::_global_cache_seg));

        case 2:
        case 11:
        case 12:
        case 16: case 17: case 18: case 19: case 20:
        case 21: case 22: case 23: case 24: case 25:
        case 26: case 27: case 28:
        case 30: case 31: case 32:
            return _c_anims[index];

        case 3:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 5, MOX::_global_cache_seg));

        case 4:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 6, MOX::_global_cache_seg));

        case 5:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x10, MOX::_global_cache_seg));

        case 6:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x11, MOX::_global_cache_seg));

        case 7:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x12, MOX::_global_cache_seg));

        case 8:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2b, MOX::_global_cache_seg));

        case 9:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2c, MOX::_global_cache_seg));

        case 10:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2d, MOX::_global_cache_seg));

        case 13:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x32, MOX::_global_cache_seg));

        case 14:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x33, MOX::_global_cache_seg));

        case 15:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x34, MOX::_global_cache_seg));

        case 33:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 0x16, MOX::_global_cache_seg));

        case 34:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY_LBX, 0x11, MOX::_global_cache_seg));

        case 35:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY_LBX, 0x12, MOX::_global_cache_seg));

        default:
            return nullptr;
        }
    }
}