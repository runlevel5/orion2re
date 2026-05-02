#include "pch.h"

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
        mouse::Set_Mouse_List_(&COLGCBT::_mouse_list_blank, 1);
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
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        Do_Informational_And_Decision_Popups_();

        while (true) {
            timer::Mark_Time_();
            _g_count++;

            if (input_received) {
                Update_Fields_();
                input_val = 0;
            }

            Draw_Colony_Screen_();
            RUSS::Mox_Sync_Update_();

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
        FAKEBUTS::Clear_Fake_Buttons_();
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
        RUSS::Mox_Update_();
        COLXPORT::Load_Xport_Anims_();
        RUSS::Mox_Update_();
        Load_Ship_Bitm_();
        RUSS::Mox_Update_();
        Load_Bldg_Bitm_();
        RUSS::Mox_Update_();
        Load_Officer_Anim_();
        RUSS::Mox_Update_();
        COLDRAW::Load_Road_List_Anims_();
        COLBLDG::Clear_Last_App_Desc_();
        RUSS::Mox_Update_();
        Load_Galaxy_Map_Anims_();
        RUSS::Mox_Update_();

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
            _c_anims[30] = OFFICER::Load_Officer_Picture_(
                (int32_t)MOX::_PLAYER_NUM,
                (int16_t)officer_idx,
                MOX::_screen_seg,
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
            return reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_(_RACEICON_LBX, entry_num, MOX::_global_cache_seg));
        }

        if (param_2 == 0) {
            int32_t entry_num = (static_cast<int32_t>(param_3) * 13) + (static_cast<int32_t>(param_1) * 2);
            return reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_(_RACEICON_LBX, entry_num, MOX::_global_cache_seg));
        }

        if (param_2 == 3) {
            return reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_(_RACEICON_LBX, 0xAA, MOX::_global_cache_seg));
        }

        if (param_2 == 4) {
            return reinterpret_cast<s_animation_header*>(buffer::Buffer_Reload_(_RACEICON_LBX, 0xA9, MOX::_global_cache_seg));
        }

        return nullptr;
    }

    s_animation_header* __cdecl Satellite_Anim_(int16_t colony_id) {
        int16_t pic_index = Satellite_Anim_Pic_(colony_id);
        return Buffer_Reload_Colony_(pic_index);
    }

    s_animation_header* __cdecl C_Anims_(int16_t anim_id) {
        RUSS::Mox_Update_();

        switch (anim_id) {
            case 0:
                return Cache_Load_Planet_(
                    static_cast<int16_t>(MOX::_planet[MOX::_colony[_colony_handle].planet].climate_bg_type +
                    static_cast<uint8_t>(MOX::_colony[_colony_handle].climate) * 3)
                );
            case 1:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x31, MOX::_global_cache_seg);
            case 2:
            case 11:  // 0x0B
            case 12:  // 0x0C
            case 16:  // 0x10
            case 17:  // 0x11
            case 18:  // 0x12
            case 19:  // 0x13
            case 20:  // 0x14
            case 21:  // 0x15
            case 22:  // 0x16
            case 23:  // 0x17
            case 24:  // 0x18
            case 25:  // 0x19
            case 26:  // 0x1A
            case 27:  // 0x1B
            case 28:  // 0x1C
            case 30:  // 0x1E
            case 31:  // 0x1F
            case 32:  // 0x20
                return _c_anims[anim_id];
            case 3:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLPUPS_LBX, 5, MOX::_global_cache_seg);
            case 4:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLPUPS_LBX, 6, MOX::_global_cache_seg);
            case 5:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x10, MOX::_global_cache_seg);
            case 6:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x11, MOX::_global_cache_seg);
            case 7:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x12, MOX::_global_cache_seg);
            case 8:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x2B, MOX::_global_cache_seg);
            case 9:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x2C, MOX::_global_cache_seg);
            case 10:
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x2D, MOX::_global_cache_seg);
            case 13:  // 0x0D
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x32, MOX::_global_cache_seg);
            case 14:  // 0x0E
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x33, MOX::_global_cache_seg);
            case 15:  // 0x0F
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY2_LBX, 0x34, MOX::_global_cache_seg);
            case 33:  // 0x21
                return (s_animation_header*)buffer::Buffer_Reload_(_COLPUPS_LBX, 0x16, MOX::_global_cache_seg);
            case 34:  // 0x22
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY_LBX, 0x11, MOX::_global_cache_seg);
            case 35:  // 0x23
                return (s_animation_header*)buffer::Buffer_Reload_(_COLONY_LBX, 0x12, MOX::_global_cache_seg);
            default:
                return nullptr;
        }
    }


    void __cdecl Make_Bldg_Array_For_Colony_(int16_t colony_idx, uint8_t is_ui_view) {
        uint32_t previous_seed = random::Get_Random_Seed_();
        random::Set_Random_Seed_(static_cast<uint32_t>(colony_idx));

        int16_t satellite_count = 0;
        Clear_Colony_Buidlings_();
        Clear_Satellite_Array_();

        int bldg_count = 0;
        for (int16_t bldg_id = 0; bldg_id < 49; ++bldg_id) {
            if (MOX::_colony[colony_idx].buildings[bldg_id] != 0) {
                if (is_ui_view == 0 || bldg_count < 18) {
                    if (TECHDATA::_buildings[bldg_id].type == 7 /* Satellite */) {
                        Insert_Satellite_Into_Array_(bldg_id, &satellite_count);
                    }
                    else {
                        Insert_Bldg_Into_Array_(colony_idx, bldg_id, bldg_count, is_ui_view);
                        bldg_count++;
                    }
                }
            }
        }

        int housing_to_add = (static_cast<uint8_t>(MOX::_colony[colony_idx].n_pops) / 3) + 1;
        for (int h = 0; h < housing_to_add; ++h) {
            if (bldg_count < 36) {
                if (is_ui_view == 0 || (is_ui_view == 1 && bldg_count < 18)) {
                    Insert_Bldg_Into_Array_(colony_idx, -3 /* ECON_HOUSING */, bldg_count, is_ui_view);
                    bldg_count++;
                }
            }
        }

        Sort_Bldg_Array_Columns_();

        if (is_ui_view == 0) {
            BUILDING target_jitter_ids[4] = {
                BUILDING_ARTEMIS_SYSTEM_NET,
                BUILDING_DIMENSIONAL_PORTAL,
                BUILDING_STAR_BASE,
                BUILDING_STAR_FORTRESS
            };
            for (int k = 0; k < 8; ++k) {
                int16_t bldg_id = target_jitter_ids[k % 4];
                int16_t row, col;
                if (Get_Bldg_CR_(bldg_id, &row, &col)) {
                    bool swapped = false;
                    for (int16_t si = 1; si >= -1 && !swapped; --si) {
                        for (int16_t di = 1; di >= -1 && !swapped; --di) {
                            int tr = row - si;
                            if (tr < 0) tr = 0;
                            if (tr > 5) tr = 5;
                            int tc = tr;
                            bool should_swap = false;
                            if (COLONY::_colony_bldgs[tr].buildings[tc] == 0) {
                                if (random::Random_(3) == 1) should_swap = true;
                            }
                            else {
                                if (random::Random_(3) == 1) {
                                    should_swap = true;
                                }
                                else if (random::Random_(3) == 1) {
                                    should_swap = true;
                                }
                            }
                            if (should_swap) {
                                int32_t temp_id = COLONY::_colony_bldgs[tr].buildings[tc];
                                COLONY::_colony_bldgs[tr].buildings[tc] = COLONY::_colony_bldgs[row].buildings[col];
                                COLONY::_colony_bldgs[row].buildings[col] = temp_id;
                                swapped = true;
                            }
                        }
                    }
                }
            }
            int16_t sp_row, sp_col;
            if (Get_Bldg_CR_(BUILDING_CAPITOL, &sp_row, &sp_col)) {
                if (COLONY::_colony_bldgs[1].buildings[1] != 0) {
                    int32_t temp_val = COLONY::_colony_bldgs[1].buildings[1];
                    COLONY::_colony_bldgs[1].buildings[1] = COLONY::_colony_bldgs[sp_row].buildings[sp_col];
                    COLONY::_colony_bldgs[sp_row].buildings[sp_col] = temp_val;
                }
            }
        }

        COLDRAW::Build_Road_List_Based_On_Bldg_List_();

        if (is_ui_view == 0) {
            COLDRAW::Build_Veggie_List_Based_On_Bldg_List_();
        }
        else {
            COLDRAW::Clear_Veggie_Draw_Flags_();
        }

        random::Set_Random_Seed_(previous_seed);
    }


    void __cdecl Insert_Bldg_Into_Array_(int16_t num_houses_delta, int16_t bldg_id, int16_t bldg_type, int8_t use_back_to_front) {
        int16_t row = -1;
        int16_t col = -1;
        int16_t n_houses = COLONY::N_Houses_In_Bldg_Array_();

        int total = (int)n_houses + (int)num_houses_delta;
        int16_t house_mod = (int16_t)(total % 4);

        if (bldg_type < 36 /* 0x24 */) {
            if (use_back_to_front == 1) {
                // TYPE MISMATCH: existing Bldg_Back_To_Front_ uses uint16_t*
                COLONY::Bldg_Back_To_Front_(bldg_type, (uint16_t*)&row, (uint16_t*)&col);
            }
            else {
                int16_t empty_count = 0;
                for (int16_t i = 0; i < 6; ++i) {
                    for (int16_t j = 0; j < 6; ++j) {
                        if (COLONY::_colony_bldgs[i].buildings[j] == 0) {
                            empty_count++;
                            if (random::Random_((int)empty_count) == 1) {
                                row = i;
                                col = j;
                            }
                        }
                    }
                }

                if (row == -1 || (col == -1 && bldg_id != -3)) {
                    COLONY::Find_Replacement_Slot_For_Building_(bldg_id, &row, &col);
                }
            }
        }
        else {
            COLONY::Find_Replacement_Slot_For_Building_(bldg_id, &row, &col);
        }

        if (row == -1 || col == -1) {
            return;
        }

        if (bldg_id >= 0) {
            COLONY::_colony_bldgs[row].buildings[col] = (int32_t)bldg_id;
        }
        else if (bldg_id == -3) {
            house_mod++;
            int switch_idx = (int)house_mod % 4;

            if (static_cast<uint32_t>(switch_idx) > 3) {
                COLONY::_colony_bldgs[row].buildings[col] = BUILDING_DIMENSIONAL_PORTAL /* 0x0E */;
            }
            else {
                switch (switch_idx) {
                case 0: COLONY::_colony_bldgs[row].buildings[col] = BUILDING_ARTEMIS_SYSTEM_NET /* 3 */; break;
                case 1: COLONY::_colony_bldgs[row].buildings[col] = BUILDING_DIMENSIONAL_PORTAL /* 0x0E */; break;
                case 2: COLONY::_colony_bldgs[row].buildings[col] = BUILDING_STAR_BASE /* 0x28 */; break;
                case 3: COLONY::_colony_bldgs[row].buildings[col] = BUILDING_STAR_FORTRESS /* 0x29 */; break;
                }
            }
        }
        else if (bldg_id == -1) {
            COLONY::_colony_bldgs[row].buildings[col] = BUILDING_BATTLESTATION /* 8 */;
        }
    }

    void Sort_Bldg_Array_Columns_() {
        int16_t iteration = 0;
        int16_t swapped_flag;

        do {
            swapped_flag = 0;

            for (int16_t row = 0; row < 5; ++row) {
                for (int16_t col = 0; col < 5; ++col) {
                    int32_t& b1 = _colony_bldgs[row].buildings[col];

                    int32_t& b_diag = _colony_bldgs[row + 1].buildings[col + 1];
                    if (b1 != 0 && b_diag != 0 &&
                        static_cast<uint8_t>(TECHDATA::_buildings[b_diag].type) < static_cast<uint8_t>(TECHDATA::_buildings[b1].type)) {
                        swapped_flag = static_cast<int16_t>(b1);
                        int32_t temp = b1;
                        b1 = b_diag;
                        b_diag = temp;
                    }

                    int32_t& b_below = _colony_bldgs[row + 1].buildings[col];
                    if (b1 != 0 && b_below != 0 &&
                        static_cast<uint8_t>(TECHDATA::_buildings[b_below].type) < static_cast<uint8_t>(TECHDATA::_buildings[b1].type)) {
                        swapped_flag = static_cast<int16_t>(b1);
                        int32_t temp = b1;
                        b1 = b_below;
                        b_below = temp;
                    }

                    int32_t& b_right = _colony_bldgs[row].buildings[col + 1];
                    if (b1 != 0 && b_right != 0 &&
                        static_cast<uint8_t>(TECHDATA::_buildings[b_right].type) < static_cast<uint8_t>(TECHDATA::_buildings[b1].type)) {
                        swapped_flag = static_cast<int16_t>(b1);
                        int32_t temp = b1;
                        b1 = b_right;
                        b_right = temp;
                    }
                }
            }
        } while (swapped_flag != 0 && ++iteration < 6);
    }


    void __cdecl Tested_Colony_Buys_Outright_(int16_t colony_idx) {
        s_colony* col = &MOX::_colony[colony_idx];
        char buf[500];

        if (col->bought_outright != 0) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_HAVE_ALREADY_BOUGHT_THIS_PRODUCT));
            COLONY::Show_Colony_Screen_();
            return;
        }

        int star_idx = MOX::_planet[col->planet].star_index;
        if ((MOX::_star[star_idx].blockaded >> (MOX::_PLAYER_NUM & 0x1f)) & 1) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_CANNOT_BUY_WHILE_THIS_STAR_IS_BLOCKADED));
            COLONY::Show_Colony_Screen_();
            return;
        }

        // -3 = ECON_HOUSING, -2 = ECON_TRADE_GOODS
        if (col->producing[0] == -3 || col->producing[0] == -2) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_CANNOT_BUY_HOUSING_OR_TRADE_GOODS_AS_THEY_ARE_SETTINGS_NOT_SPECIFIC_ITEMS));
            COLONY::Show_Colony_Screen_();
            return;
        }

        int buy_cost = COLCALC::Colony_Cost_To_Buy_Producing_Product_(col);
        if (buy_cost <= 0) {
            if (col->producing[0] != -1 /* ECON_NONE */) {
                GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_CANNOT_SPEND_BCS_TO_GET_THIS_FASTER_THAN_IN_A_SINGLE_TURN));
            }
            else {
                GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_HAVE_NO_PRODUCT_SELECTED_TO_BUY));
            }
            COLONY::Show_Colony_Screen_();
            return;
        }

        if (COLCALC::Colony_Can_Afford_To_Buy_Product_(colony_idx)) {
            if (COLONY::Colony_Can_Buy_Product_0_(colony_idx)) {
                int prod_remaining = (int)COLCALC::Colony_Cost_To_Buy_Producing_Product_(col) - (int)col->production_spent;
                int actual_buy_cost = COLCALC::Colony_Cost_To_Buy_Producing_Product_(col);

                sprintf(buf, ESTRINGS::E_Strings_(ESTR_DO_YOU_WANT_TO_SPEND_D_BC_TO_BUY_D_PRODUCTION),
                    actual_buy_cost, prod_remaining);

                if (GENDRAW::Confirmation_Box_(buf) != 0) {
                    COLCALC::Colony_Buy_Outright_(colony_idx);
                }
                COLONY::Col_Calc_Wrapper_(colony_idx);
            }
            else {
                GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_PURCHASING_FAILED_UNEXPECTEDLY_PLEASE_TELL_ERIC));
                COLONY::Show_Colony_Screen_();
            }
        }
        else {
            int short_by = (int)(short)COLCALC::Colony_Cost_To_Buy_Producing_Product_(col) - MOX::_player[col->owner].bc;
            sprintf(buf, ESTRINGS::E_Strings_(ESTR_YOU_CANNOT_AFFORD_THIS_INCREASE_IN_PRODUCTION_YOU_ARE_SHORT_BY_D_BC),
                short_by);
            GENDRAW::Help_(buf);
            COLONY::Show_Colony_Screen_();
        }
    }


    void Draw_Info_Name_And_Pop_() {
        char buffer[104];
        int32_t handle = _colony_handle;

        if (MOX::_colony[handle].occupation_policy == 0) {
            char* planet_name = HACCESS::Get_Planet_Name_(static_cast<int16_t>(_planet_handle));
            char* fmt = ESTRINGS::E_Strings_(ESTR_ANNIHILATING_S);
            sprintf(buffer, fmt, planet_name);
            int16_t cycle_val = ERIC::Global_Cycler_(30, 2);
            Set_Colony_Font_To_Red_(4, cycle_val);
        }
        else {
            char* planet_name = HACCESS::Get_Planet_Name_(static_cast<int16_t>(_planet_handle));
            char* colony_type = Colony_Type_(handle);
            char* fmt = ESTRINGS::E_Strings_(ESTR_SCOLONY_OF_S);
            sprintf(buffer, fmt, colony_type, planet_name);
            Set_Colony_Font_To_(4);
        }

        fonts::Set_Font_Spacing_(2);
        fonts::Print_Centered_(320, 0, buffer);

        int16_t colony_planet = MOX::_colony[handle].planet;
        int8_t star_idx = MOX::_planet[colony_planet].star_index;

        int16_t status_msg_id = -1;
        if ((MOX::_star[star_idx].blockaded >> (MOX::_PLAYER_NUM & 0x1F)) & 1) {
            int16_t cycle_val = ERIC::Global_Cycler_(30, 2);
            Set_Colony_Font_To_Red_(4, cycle_val);
            fonts::Set_Font_Spacing_(2);
            status_msg_id = ESTR_BLOCKADED1;
        }
        else if (EVENTS::Event_Check_Plague_(static_cast<int16_t>(handle))) {
            int16_t cycle_val = ERIC::Global_Cycler_(30, 2);
            Set_Colony_Font_To_Red_(4, cycle_val);
            fonts::Set_Font_Spacing_(2);
            status_msg_id = ESTR_PLAGUE;
        }
        else if (EVENTS::Event_Check_Population_Boom_(static_cast<int16_t>(handle))) {
            Set_Colony_Font_To_(4);
            fonts::Set_Font_Spacing_(2);
            status_msg_id = ESTR_POP_BOOM;
        }

        if (status_msg_id != -1) {
            char* status_text = ESTRINGS::E_Strings_(status_msg_id);
            fonts::Print_(0, 0, status_text);
        }

        Set_Colony_Font_To_(4);
        fonts::Set_Font_Spacing_(3);

        int sum_roundoff = 0;
        int sum_growth = 0;
        for (int i = 0; i < 10; ++i) {
            sum_roundoff += MOX::_colony[handle].pop_roundoff[i];
            sum_growth += MOX::_colony[handle].pop_growth[i];
        }

        int16_t n_base_pop = static_cast<int16_t>(Sum_Colonists_(handle, -1, -1, -1, -1, -1));
        int total_pop = n_base_pop + (sum_roundoff / 1000);
        int roundoff_rem = sum_roundoff % 1000;

        char* pop_fmt = ESTRINGS::E_Strings_(ESTR_POP_D03D_K_DK);
        sprintf(buffer, pop_fmt, total_pop, roundoff_rem, sum_growth);

        fonts::Set_Font_Style_Shadow_Down_(2, &_font_color_array);
        fonts::Print_Right_(638, 3, buffer);
        Set_Colony_Font_();
    }

    void __cdecl Draw_Current_Building2_(int16_t x, int16_t y, int16_t building_id) {
        if (building_id == -3) {
            graphics::Set_Window_(517, 17, 639, 158);
            video::Clip_On_();
            COLONY::Draw_Building_With_Bottom_Centered_(579, 106, 14, 5, 5);
            video::Clip_Off_();
            return;
        }

        if (TECHDATA::_buildings[building_id].type != 7) {
            graphics::Set_Window_(517, 17, 639, 158);
            video::Clip_On_();
            COLONY::Draw_Building_With_Bottom_Centered_(579, 106, building_id, 5, 5);
            video::Clip_Off_();
            return;
        }

        s_animation_header* animation = COLONY::Satellite_Anim_(building_id);
        ERIC::Draw_Centered_(x, (int16_t)(y - 70), animation);
    }


    void __cdecl Draw_Info_Build_() {
        char name_buffer[100];
        s_colony& colony = MOX::_colony[COLONY::_colony_handle];
        int16_t producing_id = colony.producing[0];

        switch (producing_id) {
        case -15:
            COLONY::Remap_Draw_Ship_Seg_N_Centered_(0x242 /* 578 */, 0x4d /* 77 */, 0x30 /* 48 */);
            break;
        case -12:
            COLONY::Remap_Draw_Ship_Seg_N_Centered_(0x242 /* 578 */, 0x39 /* 57 */, 0x2d /* 45 */);
            break;
        case -11:
            COLONY::Remap_Draw_Ship_Seg_N_Centered_(0x242 /* 578 */, 0x39 /* 57 */, 0x2f /* 47 */);
            break;
        case -8:
        case -2:
        case -1:
        case 0:
            // Do nothing
            break;
        case -7: {
            uint16_t race = (uint16_t)(uint8_t)MOX::_player[MOX::_PLAYER_NUM].race;
            s_animation_header* anim = COLONY::Spy_Anim_(race);
            ERIC::Draw_Centered_(0x242 /* 578 */, 0x4d /* 77 */, anim);
            break;
        }
        case -6:
        case -5:
        case -4: {
            uint16_t race = (uint16_t)(uint8_t)MOX::_player[MOX::_PLAYER_NUM].race;
            s_animation_header* anim = COLONY::People_Anim_(0, 4, race);
            ERIC::Draw_Centered_(0x242 /* 578 */, 0x4d /* 77 */, anim);
            break;
        }
        case -3:
            COLONY::Draw_Current_Building_(-3);
            break;
        default:
            COLONY::Set_Colony_Font_();
            if (producing_id >= 1 && producing_id < 49) {
                COLONY::Draw_Current_Building_(producing_id);
            }
            else if (producing_id <= -100) {
                COLONY::Draw_Current_Ship_(-100 - producing_id);
            }
            else {
                char* err_msg = ESTRINGS::E_Strings_(0xf7);
                RUSS::PFMT_(0, 0, err_msg);
                assert::hgetch_();
            }
            break;
        }

        COLONY::Draw_Current_Production_Graph_();

        char* name_str = COLBLDG::Selection_Name_(producing_id);
        strcpy(name_buffer, name_str);

        fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
        BILL::Squeeze_Print_Paragraph_(0x20a /* 522 */, 0x26 /* 38 */, 0x70 /* 112 */, 0x8d /* 141 */, name_buffer, 2);

        if (colony.auto_building != 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_bright_blue_font_color_array);
            char* auto_msg = ESTRINGS::E_Strings_(0xbd);
            fonts::Print_Centered_(0x242 /* 578 */, 0x1c /* 28 */, auto_msg);
        }

        COLONY::Draw_Turn_Count_();
    }


    void __cdecl Add_Screen_Fields_() {
        _colony_fields[10] = -1000;
        _colony_fields[9] = -1000;

        _colony_fields[2] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(0x87));
        _colony_fields[6] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(0x8b));
        _colony_fields[7] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(0x8c));

        _colony_fields[19] = fields::Add_Multi_Hot_Key_Field_(ESTRINGS::E_Strings_(0xf5));
        _colony_fields[20] = fields::Add_Multi_Hot_Key_Field_(ESTRINGS::E_Strings_(0x218));

        _colony_fields[1] = fields::Add_Button_Field_(0x22c, 0x1cb, ESTRINGS::E_Strings_(0xc), COLONY::C_Anims_(0x15), ESTRINGS::E_Strings_(0xc), 0x28);
        _colony_fields[5] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(0xd5));

        if (MOX::_star[_star_handle].officer_index[MOX::_PLAYER_NUM] == -1) {
            _colony_fields[17] = fields::Add_Button_Field_(0x22c, 0x1ab, ESTRINGS::E_Strings_(0xc), COLONY::C_Anims_(0x20), ESTRINGS::E_Strings_(0x158), 0x28);
        }
        else {
            _colony_fields[17] = fields::Add_Hidden_Field_(0x226, 0x165, 0x26f, 0x1bd, ESTRINGS::E_Strings_(0x158), 0);
        }

        _colony_fields[12] = fields::Add_Hidden_Field_(0x136, 0x20, 0x1fe, 0x3d, ESTRINGS::E_Strings_(0xc), 0);
        _colony_fields[13] = fields::Add_Hidden_Field_(0x80, 0x20, 0x12d, 0x3d, ESTRINGS::E_Strings_(0xc), 0);
        _colony_fields[14] = fields::Add_Hidden_Field_(0x80, 0x3e, 0x12d, 0x5b, ESTRINGS::E_Strings_(0xc), 0);
        _colony_fields[15] = fields::Add_Hidden_Field_(0x80, 0x5c, 0x12d, 0x79, ESTRINGS::E_Strings_(0xc), 0);
        _colony_fields[16] = fields::Add_Hidden_Field_(0x80, 0x7a, 0x12d, 0x97, ESTRINGS::E_Strings_(0xc), 0);

        if (_valid_colony != 0) {
            _colony_fields[4] = fields::Add_Button_Field_(0x207, 0x7b, ESTRINGS::E_Strings_(0xc), COLONY::C_Anims_(0x13), ESTRINGS::E_Strings_(0xd5), 0x28);
            _colony_fields[18] = fields::Add_Hidden_Field_(0x20d, 0x1a, 0x276, 0x25, ESTRINGS::E_Strings_(0xa6), 0x29);

            if (Colony_Can_Buy_Product_0_(_colony_handle)) {
                _colony_fields[3] = fields::Add_Button_Field_(0x24e, 0x7b, ESTRINGS::E_Strings_(0xc), COLONY::C_Anims_(0x14), ESTRINGS::E_Strings_(0xc1), 0x28);
            }
            else {
                s_animation_header* buy_anim = COLONY::C_Anims_(0x14);
                int16_t h = animate::Get_Height_(buy_anim);
                int16_t w = animate::Get_Width_(COLONY::C_Anims_(0x14));
                _colony_fields[3] = fields::Add_Hidden_Field_(0x24e, 0x7b, 0x24e + w, 0x7b + h, ESTRINGS::E_Strings_(0xc1), 0x28);
            }
        }
        else {
            _colony_fields[4] = -1000;
            _colony_fields[3] = -1000;
        }

        Add_Stacking_Fields_();
        Add_Satellite_Fields_();
        Add_Bldg_Fields_();

        _colony_fields[0] = fields::Add_Hidden_Field_(0, 0, 0x27f, 0x1df, ESTRINGS::E_Strings_(0xc), 0);
    }

    int8_t __cdecl Evaluate_Screen_Input_(int16_t* input_field) {
        int16_t field_val = *input_field;

        if (_colony_fields[8] == field_val) {
            OCPOLPUP::Occupation_Policy_Popup_();
            Show_Colony_Screen_();
            Col_Calc_Wrapper_(static_cast<int16_t>(_colony_handle));
            return 1;
        }

        if (_colony_fields[17] == field_val) {
            (&MOX::_screen_data)[4] = static_cast<int16_t>(_star_handle);
            MOX::_return_screen = MOX::_current_screen;
            (&MOX::_screen_data)[3] = 1;
            MOX::_current_screen = 29 /* 0x1d */;
            return 0;
        }

        if (_colony_fields[9] == field_val) {
            if (_drawing_display == 0) {
                _drawing_display = 1;
                _full_draw = 1;
                return 1;
            }
        }

        if (_colony_fields[10] == field_val) {
            if (_drawing_display == 1) {
                _drawing_display = 0;
                _full_draw = 1;
                _g_scanned_planet = -1;
                return 1;
            }
        }

        if (_colony_fields[11] == field_val) {
            int16_t transport_result = COLXPORT::Xport_Popup_();
            Show_Colony_Screen_();
            if (transport_result != -1) {
                COLMOVE::Send_Cluster_(transport_result, -1);
            }
            Show_Colony_Screen_();
            Col_Calc_Wrapper_(static_cast<int16_t>(_colony_handle));
            return 1;
        }

        if (_colony_fields[18] == field_val) {
            MOX::_colony[_colony_handle].auto_building = (MOX::_colony[_colony_handle].auto_building == 0);
            Show_Colony_Screen_();
            return 1;
        }

        if (_colony_fields[3] == field_val) {
            Tested_Colony_Buys_Outright_(static_cast<int16_t>(_colony_handle));
            Update_Fields_();
            Show_Colony_Screen_();
            *input_field = 0;
            fields::Clear_Mouse_Buffer_();
            fields::Set_Input_Delay_(3);
            return 1;
        }

        if (_colony_fields[5] == field_val) {
            if (_cheats != 0) {
                Do_Cheats_();
                Show_Colony_Screen_();
                return 1;
            }
            Col_Calc_Wrapper_(static_cast<int16_t>(_colony_handle));
            return 1;
        }

        if (_colony_fields[12] == field_val) {
            Show_Morale_(_colony_handle);
            Show_Colony_Screen_();
            return 1;
        }

        if (_colony_fields[13] == field_val) {
            Show_BC_Production_(_colony_handle);
            Show_Colony_Screen_();
            return 1;
        }

        if (_colony_fields[14] == field_val) {
            Show_Food_Production_(_colony_handle);
            Show_Colony_Screen_();
            return 1;
        }

        if (_colony_fields[15] == field_val) {
            COLCALC::Show_Industry_Production_(static_cast<int16_t>(_colony_handle));
            Show_Colony_Screen_();
            return 1;
        }

        if (_colony_fields[16] == field_val) {
            Show_Research_Production_(_colony_handle);
            Show_Colony_Screen_();
            return 1;
        }

        if (_colony_fields[4] == field_val) {
            MOX::_return_screen = MOX::_current_screen;
            MOX::_current_screen = 25 /* 0x19 */;
            return 0;
        }

        if (_colony_fields[6] == field_val) {
            int16_t new_colony = Get_Next_Colony_(_colony_handle, 0);
            if (new_colony != static_cast<int16_t>(_colony_handle)) {
                Set_Colony_Screen_To_New_Colony_(new_colony);
                return 0;
            }
        }
        else if (_colony_fields[20] == field_val) {
            if (capture::Check_Release_Version_() != 0) {
                _cheats = (_cheats == 0);
                MAINSCR::Set_Player_Cheated_Flag_(MOX::_PLAYER_NUM, 6);
                ERICNET::Mox_Play_Sound_(51 /* 0x33 */);
                return 1;
            }
        }
        else if (_colony_fields[19] == field_val) {
            s_colony* colony = &MOX::_colony[_colony_handle];
            colony->production_spent = static_cast<int16_t>(COLCALC::Colony_Producing_Product_Cost_(colony));
            MAINSCR::Set_Player_Cheated_Flag_(MOX::_PLAYER_NUM, 6);
            ERICNET::Mox_Play_Sound_(51 /* 0x33 */);
            return 1;
        }
        else if (_colony_fields[7] == field_val) {
            int16_t new_colony = Get_Next_Colony_(_colony_handle, 1);
            if (new_colony != static_cast<int16_t>(_colony_handle)) {
                Set_Colony_Screen_To_New_Colony_(new_colony);
                return 0;
            }
        }
        else if (_colony_fields[2] == field_val) {
            *input_field = _colony_fields[1];
            return 1;
        }
        else if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            Show_Colony_Screen_();
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        return 1;
    }

    int __cdecl Do_Colony_Info_Military_Stuff_For_(int military_index, int start_n, int param_3, int mode) {
        int military_count;
        int owner;
        int local_n;
        char buffer[256];
        s_animation_header* anim;
        int16_t height;

        military_count = MOX::_colony[_colony_handle].military[military_index];
        owner = MOX::_colony[_colony_handle].owner;
        _step_squish = 0;

        if (mode != 2) {
            Do_Colony_Info_Military_Stuff_For_(military_index, start_n, param_3, 2);
        }

        Set_Colony_Font_();

        if (_step_squish == 0) {
            local_n = 0;
            if (military_count > 0) {
                do {
                    if (mode == 0) {
                        int race = MOX::_player[owner].race;
                        anim = Military_Anim_(static_cast<int16_t>(military_index), static_cast<int16_t>(race));
                        height = animate::Get_Height_(anim);
                        animate::Draw_(static_cast<int16_t>((local_n + start_n) * (30 - _step_squish)), static_cast<int16_t>(0x1df - height), anim);
                    }
                    local_n++;
                } while (local_n < military_count);
            }
        }
        else {
            _step_squish = 0;
            local_n = 0;
            if (military_count > 0) {
                int race = MOX::_player[owner].race;
                anim = Military_Anim_(static_cast<int16_t>(military_index), static_cast<int16_t>(race));
                height = animate::Get_Height_(anim);

                animate::Draw_(static_cast<int16_t>(start_n * 30), static_cast<int16_t>(0x1df - height), anim);

                sprintf(buffer, ESTRINGS::E_Strings_(0x7c /* "x %d" */), military_count);
                Set_Colony_Font_To_(3);

                anim = Military_Anim_(static_cast<int16_t>(military_index), static_cast<int16_t>(race));
                height = animate::Get_Height_(anim);

                fonts::Print_(static_cast<int16_t>((start_n + 1) * (30 - _step_squish)), static_cast<int16_t>(0x1df - height), buffer);
                local_n = 3;
            }
        }

        if (mode == 1) {
            if (local_n > 0) {
                int step = 30 - _step_squish;
                int race = MOX::_player[owner].race;
                anim = Military_Anim_(static_cast<int16_t>(military_index), static_cast<int16_t>(race));
                height = animate::Get_Height_(anim);

                int x1 = start_n * step;
                int y1 = 0x1df - height;
                int x2 = (local_n + start_n) * step - 1;
                int y2 = 0x1df;

                _military_fields[military_index] = fields::Add_Hidden_Field_(static_cast<int16_t>(x1), static_cast<int16_t>(y1), static_cast<int16_t>(x2), static_cast<int16_t>(y2), ESTRINGS::E_Strings_(0xc), 0);
            }
            else {
                _military_fields[military_index] = -1000;
            }
        }
        else if (mode == 2) {
            int other_index = (military_index + 1) % 2;
            int total_military = military_count + MOX::_colony[_colony_handle].military[other_index];

            if (total_military < 2) {
                total_military = 1;
            }

            int step = 260 / total_military;
            if (step < 1) {
                step = 1;
            }

            if (step < 30) {
                _step_squish = static_cast<short>(30 - step);
            }
            else {
                _step_squish = 0;
            }
        }

        return start_n + local_n;
    }

}
