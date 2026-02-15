#include "deps.h"

namespace COLSUM {
    void __cdecl Draw_Scan_Info_() {
        Draw_Colony_Scan_Info_();
        Set_Colony_Font_To_(1);

        if (MOX::_galaxy_map_scanned_star != -1) {
            const char* star_name = reinterpret_cast<const char*>(&MOX::_star[MOX::_galaxy_map_scanned_star]);
            fonts::Print_Centered_(444, 431, star_name);
        }
    }

    void __cdecl Colony_Summary_Screen_() {
        void* saved_auto_fn = nullptr;
        int16_t input_val = 0;
        int16_t scan_val = 0;
        short colony_list_buffer[250];

        COLSUM::_draw_planet_sum = 0;
        COLXPORT::_g_colony_list_ptr = colony_list_buffer;

        if (MOX::_return_screen != MOX::_current_screen &&
            COLBLDG::_return_to_build_popup_from_custom_design == 0 &&
            MOX::_return_screen != SCREEN_QUEUE_POPUP)
        {
            COLSUM::_colony_summary_saved_screen = MOX::_return_screen;
        }

        COLSUM::Load_Col_Sum_Anims_();
        COLSUM::Load_Colsum_Strings_();
        COLONY::Allocate_Mini_Planet_Cache_();
        COLXPORT::Build_Global_Colony_List_();
        COLSUM::Sort_Col_List_();
        COLSUM::Update_Col_List_();

        if (COLSUM::_list_col[9] == -1) {
            COLSUM::Reset_List_Col_Array_();
            COLSUM::Update_Col_List_();
        }

        fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
        MOX2::Get_Auto_Function_(&saved_auto_fn);
        mouse::Set_Mouse_List_(COLGCBT::_mouse_list_blank, 1);
        fields::Deactivate_Auto_Function_();

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            ERIC::Fast_Fade_Out_();
            graphics::Fill_(0, 0, 639, 479, 0);
        }

        fonts::Load_Palette_(1, 0, 255);
        animate::Draw_Palette_(COLSUM::_anims[0]);
        remap::Update_Glass_Remap_Colors_();

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            palstore::Darken_Palette_(100);
            palette::Clear_Palette_Changes_(0, 255);
        }

        palette::Refresh_Palette_();

        COLONY::_g_colony_n = COLSUM::_list_col[0];
        COLONY::_using_colony_screen_palette = 1;
        MOX::_galaxy_map_scanned_star = (int16_t)MOX::_planet[MOX::_colony[COLONY::_g_colony_n].planet].star_index;
        COLONY::_full_draw = 1;

        ERICHELP::Set_Colony_Summary_Screen_Help_List_();
        fields::Assign_Auto_Function_(COLSUM::Draw_Colony_Summary_Screen_, 1);
        COLSUM::Draw_Colony_Summary_Screen_();
        palette::Clear_Palette_Changes_(0, 255);
        video::Set_Refresh_Stencil_();
        fields::Draw_Visible_Fields_();
        RUSS::Mox_Sync_Update_();
        COLONY::_full_draw = 0;
        JIM::Fast_Fade_In_();
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);

        while (true) {
            video::Set_Refresh_Stencil_();
            COLSUM::Update_Col_List_();
            timer::Mark_Time_();
            input_val = fields::Get_Input_();
            scan_val = fields::Scan_Input_();

            if (!COLSUM::Evaluate_Input_(&input_val, &scan_val)) {
                FAKEBUTS::Clear_Fake_Buttons_();
                COLONY::Clear_Galaxy_Map_Fields_();
                fields::Clear_Fields_();
                fields::Deactivate_Auto_Function_();
                COLONY::_using_colony_screen_palette = 0;
                return;
            }

            COLSUM::Draw_Colony_Summary_Screen_();
            if (COLONY::_full_draw != 0) {
                COLSUM::Update_Col_List_();
                COLONY::_full_draw = 0;
            }
            RUSS::Mox_Sync_Update_();
            timer::Release_Time_(1);

            if (input_val == COLSUM::_x_fields_3) {
                break;
            }
        }

        FAKEBUTS::Clear_Fake_Buttons_();
        COLONY::Clear_Galaxy_Map_Fields_();
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        fields::Deactivate_Help_List_();
        MOX2::Put_Auto_Function_(&saved_auto_fn);
        COLONY::_using_colony_screen_palette = 0;
    }

    void __cdecl Clear_List_Col_Array_() {
        memset(COLSUM::_list_col, 0xff, sizeof(COLSUM::_list_col));
    }

    void __cdecl Reset_List_Col_Array_() {
        _first = 0;
        Clear_List_Col_Array_();
        Update_Col_List_();
    }

    void __cdecl Init_Fields_() {
        fields::Clear_Fields_();
        FAKEBUTS::Clear_Fake_Buttons_();
        COLONY::Clear_Galaxy_Map_Fields_();
        Add_Fields_();
    }

    void __cdecl Update_Col_List_() {
        for (int i = 0; i < 10; ++i) {
            int16_t colony_idx = COLXPORT::_g_colony_list_ptr[COLSUM::_first + i];
            COLSUM::_list_col[i] = colony_idx;

            if (colony_idx == -1) {
                COLSUM::_list_buy_data[i] = 0;
            } else {
                COLSUM::_list_buy_data[i] = MOX::_colony[colony_idx].bought_outright;
            }
        }

        COLSUM::Init_Fields_();
    }

    void __cdecl Sort_Col_List_() {
        int16_t outer_count = 250;
        do {
            for (int16_t i = 0; i < 249; ++i) {
                int16_t val1 = COLXPORT::_g_colony_list_ptr[i];
                int16_t val2 = COLXPORT::_g_colony_list_ptr[i + 1];

                if (Switched_cmp_(val1, val2) > 0) {
                    COLXPORT::_g_colony_list_ptr[i] = val2;
                    COLXPORT::_g_colony_list_ptr[i + 1] = val1;
                }
            }
            outer_count--;
        } while (outer_count != 0);
    }

    void __cdecl Load_Col_Sum_Anims_() {
        COLBLDG::Load_Build_Anims_();

        for (int i = 0; i < 21; ++i) {
            _anims[i] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLSUM_LBX, i, MOX::_screen_seg);
        }

        COLONY::Load_Galaxy_Map_Anims_();
    }

    void __cdecl Draw_Colony_Summary_Screen_() {
        COLONY::_g_count++;
        video::Set_Page_Off_();
        graphics::Set_Window_(0, 0, 639, 479);
        video::Clip_On_();
        Update_First_(10, &_first, &_slider_bar_position);

        if (COLONY::_full_draw != 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
            graphics::Fill_(0, 0, 639, 479, 0);
            animate::Draw_(0, 0, _anims[0]);
            Clear_List_Col_Array_();
            Update_Col_List_();
            Draw_Empire_Info_();
        }

        Draw_Galaxy_Map_();
        for (int16_t i = 0; i < 10; ++i) {
            if (_list_col[i] != -1) {
                Draw_Colony_Summary_For_Colony_(i);
            }
        }

        if (COLONY::_full_draw != 0) {
            if (fields::_draw_fields_flag == 0) {
                fields::Enable_Draw_All_Fields_();
                fields::Draw_Visible_Fields_();
                fields::Disable_Draw_All_Fields_();
            } else {
                fields::Draw_Visible_Fields_();
            }
            video::Copy_Off_To_Back_();
        }

        Draw_Scan_Info_();

        if (COLMOVE::_cluster_colony_n != -1 && COLONY::_g_colony_n != -1) {
            uint8_t colors[6] = { 0xE8, 0xE9, 0xEA, 0xEB, 0xEA, 0xE9 };
            int16_t star1 = MOX::_planet[MOX::_colony[COLMOVE::_cluster_colony_n].planet].star_index;
            int16_t star2 = MOX::_planet[MOX::_colony[COLONY::_g_colony_n].planet].star_index;
            Colsum_Connect_Galaxy_Map_Stars_(star1, star2, colors);
        }

        if (_draw_planet_sum == 1) {
            int16_t index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
            COLONY::Draw_Planet_Summary_(229, 135, index);
        }

        Draw_Bar_Indicator_();
        FAKEBUTS::Draw_Fake_Buttons_();
        fields::Draw_Visible_Fields_();

        int16_t pointer_y = mouse::Pointer_Y_();
        int16_t pointer_x = mouse::Pointer_X_();
        COLMOVE::Draw_Cluster_(pointer_x, pointer_y);

        video::Clip_Off_();
    }

    void __cdecl Load_Colsum_Strings_() {
        if (_colsum_strings_been_loaded != 0) {
            return;
        }

        _colsum_strings_been_loaded = 1;

        _wee_event_strings[0] = ESTRINGS::E_Strings_(0x104);
        _wee_event_strings[1] = ESTRINGS::E_Strings_(0xdd);
        _wee_event_strings[2] = ESTRINGS::E_Strings_(0xf0);
        _wee_event_strings[3] = ESTRINGS::E_Strings_(0x227);
        _wee_event_strings[4] = ESTRINGS::E_Strings_(0xcc);
        _wee_event_strings[5] = ESTRINGS::E_Strings_(0x171);
        _wee_event_strings[6] = ESTRINGS::E_Strings_(0x10e);
        _wee_event_strings[7] = ESTRINGS::E_Strings_(0x117);
        _wee_event_strings[8] = ESTRINGS::E_Strings_(0xa7);
        _wee_event_strings[9] = ESTRINGS::E_Strings_(0x127);
        _wee_event_strings[10] = ESTRINGS::E_Strings_(0xa7);
        _wee_event_strings[11] = ESTRINGS::E_Strings_(0x102);
        _wee_event_strings[12] = ESTRINGS::E_Strings_(0x103);
        _wee_event_strings[13] = ESTRINGS::E_Strings_(0x17d);
        _wee_event_strings[14] = ESTRINGS::E_Strings_(0x19e);
        _wee_event_strings[15] = ESTRINGS::E_Strings_(0x19d);
        _wee_event_strings[16] = ESTRINGS::E_Strings_(0x19f);
        _wee_event_strings[17] = ESTRINGS::E_Strings_(0xce);
        _wee_event_strings[18] = ESTRINGS::E_Strings_(0x1d2);
        _wee_event_strings[19] = ESTRINGS::E_Strings_(0xad);
        _wee_event_strings[20] = ESTRINGS::E_Strings_(0xf6);
        _wee_event_strings[21] = ESTRINGS::E_Strings_(0x110);
        _wee_event_strings[22] = ESTRINGS::E_Strings_(0x119);
        _wee_event_strings[23] = ESTRINGS::E_Strings_(0x144);
        _wee_event_strings[24] = ESTRINGS::E_Strings_(0x18e);
        _wee_event_strings[25] = ESTRINGS::E_Strings_(0xb6);
        _wee_event_strings[26] = ESTRINGS::E_Strings_(0xc6);
        _wee_event_strings[27] = ESTRINGS::E_Strings_(0x12e);
        _wee_event_strings[28] = ESTRINGS::E_Strings_(0x22e);
        _wee_event_strings[29] = ESTRINGS::E_Strings_(0x1b6);
        _wee_event_strings[30] = ESTRINGS::E_Strings_(0x1a1);
        _wee_event_strings[31] = ESTRINGS::E_Strings_(0x135);
        _wee_event_strings[32] = ESTRINGS::E_Strings_(0x193);
        _wee_event_strings[33] = ESTRINGS::E_Strings_(0xff);
        _wee_event_strings[34] = ESTRINGS::E_Strings_(0x1f4);
        _wee_event_strings[35] = ESTRINGS::E_Strings_(0x186);
    }
}