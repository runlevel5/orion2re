#include "pch.h"

namespace SCORE {
    namespace {
        constexpr std::uintptr_t kInvalidOutColonyIdxMarker = static_cast<std::uintptr_t>(-1);
    }

    s_hof _hof;
    const char LBX_0055a430[10] = "SCORE.LBX";

    char _default_names[10][20] = {
        "Moise",
        "Irma",
        "Justin",
        "Jalen",
        "Cereal",
        "Lan_Doan",
        "Cadfael",
        "J W R",
        "Ripping_Fang",
        "Chewy",
    };

    const char s_wb[3] = "wb";
    const char s_HOF_M2[7] = "HOF.M2";

    s_hiscore* _hi;

    const char s__d__d_005523a4[6] = "%d.%d";

    const char* PTR_s_SCORE_LBX_0055a440 = SCORE::LBX_0055a430;

    int16_t _entry_to_flash;

    char _difficulty_string[5][15];

    void __cdecl nullsub() {
        return;
    }

    int32_t __cdecl qsort_hof_scores_descending_(const void* p1, const void* p2) {
        const int16_t* a = (const int16_t*)p1;
        const int16_t* b = (const int16_t*)p2;
        if (b[0] < a[0]) {
            return -1;
        }
        if (a[0] < b[0]) {
            return 1;
        }
        if (b[1] < a[1]) {
            return -1;
        }
        if (a[1] < b[1]) {
            return 1;
        }
        return 0;
    }

    /**
     * @address 0x004e4980
     */
    int32_t __cdecl qsort_hof_scores_ascending_(const void* param_1, const void* param_2) {
        const int16_t* p1 = (const int16_t*)param_1;
        const int16_t* p2 = (const int16_t*)param_2;

        if (p2[0] < p1[0]) {
            return 1;
        }
        if (p1[0] < p2[0]) {
            return -1;
        }
        if (p2[0] == p1[0]) {
            if (p2[1] < p1[1]) {
                return 1;
            }
            if (p1[1] < p2[1]) {
                return -1;
            }
        }
        return 0;
    }

    /**
     * @address 004e3ad0
     */
    void __cdecl Load_Hall_Of_Fame_Screen_() {
        // Allocate space for the high score screen state and zero it out.
        // Size 0x125 (293 bytes) corresponds to sizeof(s_hiscore).
        _hi = (s_hiscore*)allocate::Allocate_First_Block_(MOX::_screen_seg, 0x125);
        memset(_hi, 0, 0x125);

        // Load specific background and cursor animations from SCORE.LBX.
        // Entry 15 (0xF) is the hall of fame background.
        _hi->background_anim = (s_animation_header*)farload::Far_Reload_Next_(
            SCORE::LBX_0055a430,
            0xF, 
            MOX::_screen_seg
        );

        // Entry 16 (0x10) is the custom mouse cursor for this screen.
        _hi->mouse_cursor_anim = (s_animation_header*)farload::Far_Reload_Next_(
            SCORE::LBX_0055a430,
            0x10, 
            MOX::_screen_seg
        );
    }

    void __cdecl Draw_Hi_Score_Screen_() {
        if (SCORE::_hi->is_first_draw != 0) {
            graphics::Fill_(0, 0, 639, 479, 0);
            SCORE::_hi->is_first_draw = 0;
        }

        animate::Draw_(0, 0, SCORE::_hi->main_background_anim);

        SCORE::Draw_Time_Score_();
        SCORE::Draw_Population_Score_();
        SCORE::Draw_Elimination_Score_();
        SCORE::Draw_Captured_Colonies_Score_();
        SCORE::Draw_Technology_Score_();
        SCORE::Draw_Orion_Score_();
        SCORE::Draw_Antares_Score_();
        SCORE::Draw_Council_Score_();
        SCORE::Draw_Total_Score_();
    }

    int16_t __cdecl Get_Elimination_Score_(int16_t player_idx) {
        int16_t score = 0;
        SCORE::_hi->num_opponents_defeated = 0;

        if (0 < MOX::_NUM_PLAYERS) {
            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                uint16_t defeated = MOX::_player[player_idx].player_defeated[i];
                if (defeated != 0) {
                    score += 50;
                    SCORE::_hi->num_opponents_defeated++;
                }
            }
        }

        return score;
    }

    int16_t __cdecl Do_Calculate_Hi_Score_(int16_t player_idx) {
        int16_t final_score;

        allocate::Push_Block_(MOX::_global_data_seg);

        SCORE::_hi = reinterpret_cast<s_hiscore*>(allocate::Allocate_First_Block_(MOX::_global_data_seg, 0x125));

        SCORE::Calculate_Hi_Scores_(player_idx);

        final_score = SCORE::_hi->score_final;

        allocate::Pop_Block_(MOX::_global_data_seg);

        return final_score;
    }

    void __cdecl End_Of_Game_Hi_Score_(int16_t player_idx) {
        char buffer[500];
        int16_t current_score;
        int16_t max_hof_score = 0;
        int16_t rank_index = -1;
        bool cheated = false;

        current_score = Hi_Score_Screen_(player_idx);
        fields::Assign_Auto_Function_(SCORE::nullsub, 1);
        SCORE::_hi = (s_hiscore*)allocate::Allocate_First_Block_(MOX::_screen_seg, 0x125 /* 293 */);

        for (int i = 0; i < 10; ++i) {
            if (_hof.scores[i] > max_hof_score) {
                max_hof_score = _hof.scores[i];
            }
        }

        Sort_HOF_Scores_(SCORE::_hi->sort_list);

        for (int i = 0; i < 10; ++i) {
            if (SCORE::_hi->sort_list[i][0] <= current_score) {
                rank_index = SCORE::_hi->sort_list[i][1];
            }
        }

        if (rank_index > -1) {
            if (MOX::_player[player_idx].cheated != 0) {
                MISC::Get_LBX_Message_(0x17, buffer);
                GENDRAW::Warning_Box_(buffer);
                mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
                cheated = true;
                rank_index = -1;
            } else {
                if (current_score >= max_hof_score) {
                    sprintf(buffer, HAROLD::H_Message_(0x27), MOX::_player[player_idx].name);
                } else {
                    sprintf(buffer, HAROLD::H_Message_(0x28), MOX::_player[player_idx].name);
                }
                GENDRAW::Warning_Box_(buffer);
            }

            if (!cheated) {
                for (int i = 0; i < rank_index; ++i) {
                    strcpy(_hof.player_names[i], _hof.player_names[i + 1]);
                    _hof.scores[i] = _hof.scores[i + 1];
                    _hof.race_ids[i] = _hof.race_ids[i + 1];
                    _hof.difficulties[i] = _hof.difficulties[i + 1];
                    strcpy(_hof.race_names[i], _hof.race_names[i + 1]);
                }

                strcpy(_hof.player_names[rank_index], MOX::_player[player_idx].name);
                _hof.scores[rank_index] = current_score;
                _hof.race_ids[rank_index] = (int16_t)(uint8_t)MOX::_player[player_idx].race;
                _hof.difficulties[rank_index] = MOX::_settings.game_difficulty;
                strcpy(_hof.race_names[rank_index], MOX::_player[player_idx].race_name);
            }

            if (rank_index > -1 && !cheated) {
                Write_HOF_File_();
            }
        }

        Hall_Of_Fame_Screen_(rank_index);
        MOX::_current_screen = 10;
        MOX::_previous_screen = 14 /* 0xE */;
    }

    void __cdecl Create_Default_HOF_File_() {
        FILE* file = fopen(s_HOF_M2, s_wb);

        memset(&_hof, 0, sizeof(s_hof));

        *reinterpret_cast<uint16_t*>(&_hof) = 0x82;

        int16_t indices[13];
        for (int16_t i = 0; i < 13; ++i) {
            indices[i] = i;
        }

        for (int i = 0; i < 10; ++i) {
            int16_t idx1 = (int16_t)random::Random_(11) + 1;
            int16_t idx2 = (int16_t)random::Random_(11) + 1;
            if (idx2 != idx1) {
                int16_t temp = indices[idx1];
                indices[idx1] = indices[idx2];
                indices[idx2] = temp;
            }
        }

        for (int i = 0; i < 8; ++i) {
            int16_t idx1 = (int16_t)random::Random_(8) + 1;
            int16_t idx2 = (int16_t)random::Random_(8) + 1;
            char temp_name[20];
            strcpy(temp_name, _default_names[idx1]);
            strcpy(_default_names[idx1], _default_names[idx2]);
            strcpy(_default_names[idx2], temp_name);
        }

        int16_t score_base = 1500;
        int16_t rand_range = 10;

        for (int i = 0; i < 10; ++i) {
            int16_t r1 = (int16_t)random::Random_(rand_range + 1);
            int16_t r2 = (int16_t)random::Random_(5);

            _hof.scores[i] = score_base + (r2 * 10) + ((r1 - 1) * 15);
            _hof.difficulties[i] = (int8_t)(i % 5);
            strcpy(_hof.player_names[i], _default_names[i]);

            int16_t race_idx = indices[i];
            if (i < 2) {
                strcpy(_hof.race_names[i], MOX::_race_names[5]);
            } else {
                strcpy(_hof.race_names[i], MOX::_race_names[race_idx]);
            }

            score_base -= 125;
            rand_range--;
        }

        if (file) {
            fwrite(&_hof, sizeof(s_hof), 1, file);
            fclose(file);
        }
    }

    int8_t __cdecl Read_HOF_File_() {
        FILE* file;
        int16_t sort_buffer[10][2];

        file = fopen(s_HOF_M2, s_rb_0055117c);
        if (file == nullptr) {
            return 0;
        }

        fread(&_hof, sizeof(s_hof), 1, file);
        fclose(file);

        Sort_HOF_Scores_(sort_buffer);
        Move_Sorted_HOF_Data_(sort_buffer);

        return 1;
    }

    int16_t __cdecl HOF_Version_Number_() {
        if (!Read_HOF_File_()) {
            return 0;
        }

        return SCORE::_hof.version;
    }

    void __cdecl Hall_Of_Fame_Screen_(int16_t param_1) {
        int16_t input_val;
        int16_t scan_val;
        bool exit_screen = false;

        SCORE::_entry_to_flash = param_1;

        strcpy(SCORE::_difficulty_string[0], ESTRINGS::E_Strings_(0x221));
        strcpy(SCORE::_difficulty_string[1], ESTRINGS::E_Strings_(0x118));
        strcpy(SCORE::_difficulty_string[2], ESTRINGS::E_Strings_(0xbe));
        strcpy(SCORE::_difficulty_string[3], ESTRINGS::E_Strings_(0x13e));
        strcpy(SCORE::_difficulty_string[4], ESTRINGS::E_Strings_(0x148));

        if (SCORE::HOF_Version_Number_() != 0x82) {
            SCORE::Create_Default_HOF_File_();
        }

        fields::Disable_Cancel_();
        fields::Deactivate_Help_List_();
        SCORE::Read_HOF_File_();
        fields::Assign_Auto_Function_(SCORE::Draw_Hall_Of_Fame_Screen_, 1);
        SCORE::Load_Hall_Of_Fame_Screen_();
        SCORE::Init_Hall_Of_Fame_Screen_();

        SCORE::_hi->current_input_state = -1000;

        do {
            SCORE::_hi->prev_input_state = SCORE::_hi->current_input_state;
            input_val = fields::Get_Input_();
            scan_val = fields::Scan_Input_();
            SCORE::_hi->current_input_state = scan_val;

            if (SCORE::_hi->current_input_state != SCORE::_hi->prev_input_state) {
                SCORE::_hi->highlighted_entry_idx = -1;
            }

            if (SCORE::_hi->screen_field_handle == input_val) {
                exit_screen = true;
            } else if (SCORE::_hi->exit_hotkey_handle == input_val) {
                SCORE::Create_Default_HOF_File_();
                SCORE::_hi->timer_ticks = 0;
                SCORE::_entry_to_flash = -1;
                SCORE::Init_Hall_Of_Fame_Screen_();
            }

            for (int16_t i = 0; i < 10; ++i) {
                if (SCORE::_hi->entry_field_handles[i] == SCORE::_hi->current_input_state &&
                    SCORE::_hi->prev_input_state == SCORE::_hi->current_input_state &&
                    SCORE::_hi->timer_ticks > 1000) {
                    SCORE::_hi->highlighted_entry_idx = (int8_t)i;
                }
            }

            if (!exit_screen) {
                video::Set_Page_Off_();
                SCORE::Draw_Hall_Of_Fame_Screen_();
                RUSS::Mox_Sync_Update_();
            }

        } while (!exit_screen);

        ERIC::Fast_Fade_Out_();
        MOX::_current_screen = MOX::_return_screen;
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
    }

    void __cdecl Draw_Hall_Of_Fame_Screen_() {
        int16_t min_font_style = 4;
        int16_t min_race_font_spacing = 2;
        int16_t min_diff_font_spacing = 2;

        for (int16_t i = 0; i < 10; ++i) {
            int16_t record_idx = _hi->sort_list[i][1];

            MISC::Set_Fitted_Font_Style_(_hof.race_names[record_idx], &_hi->colors, 4, 2, -1, 67, 1);
            int16_t style = MISC::Get_Font_Style_();
            if (style < min_font_style) {
                min_font_style = style;
            }
            int16_t spacing = MISC::Get_Font_Spacing_();
            if (spacing < min_race_font_spacing) {
                min_race_font_spacing = spacing;
            }

            MISC::Set_Fitted_Font_Style_(_difficulty_string[_hof.difficulties[record_idx]], &_hi->colors, 4, 2, -1, 86, 1);
            style = MISC::Get_Font_Style_();
            if (style < min_font_style) {
                min_font_style = style;
            }
            spacing = MISC::Get_Font_Spacing_();
            if (spacing < min_diff_font_spacing) {
                min_diff_font_spacing = spacing;
            }
        }

        if (_hi->is_first_draw != 0 || _hi->highlighted_entry_idx < -2) {
            _hi->highlighted_entry_idx = -1;
            graphics::Fill_(0, 0, 639, 479, 0);
            animate::Draw_(0, 0, _hi->background_anim);
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        int16_t draw_y = 135;
        for (int16_t i = 0; i < 10; ++i) {
            int16_t record_idx = _hi->sort_list[i][1];
            int8_t flash_offset = 0;
            if (_entry_to_flash == record_idx) {
                flash_offset = _hi->flash_color_offset;
            }

            _hi->temp_colors.colors[0] = _hi->colors.colors[0];
            for (int j = 1; j < 8; ++j) {
                _hi->temp_colors.colors[j] = _hi->colors.colors[j] + flash_offset;
            }

            fonts::Set_Font_Style_(4, &_hi->temp_colors);
            fonts::Set_Font_Spacing_(1);
            graphics::Set_Window_(164, draw_y, 281, draw_y + 20);
            video::Clip_On_();
            fonts::Print_(164, draw_y, _hof.player_names[record_idx]);

            if (MOX::_settings.language != 0) {
                fonts::Set_Font_Style_(min_font_style, &_hi->temp_colors);
                fonts::Set_Font_Spacing_(min_race_font_spacing);
            }
            graphics::Set_Window_(292, draw_y, 354, draw_y + 20);
            video::Clip_On_();
            int16_t font_h = fonts::Get_Font_Height_();
            fonts::Print_(292, (draw_y - font_h) + 16, _hof.race_names[record_idx]);

            if (MOX::_settings.language != 0) {
                fonts::Set_Font_Style_(min_font_style, &_hi->temp_colors);
                fonts::Set_Font_Spacing_(min_diff_font_spacing);
            }
            graphics::Set_Window_(364, draw_y, 449, draw_y + 20);
            video::Clip_On_();
            font_h = fonts::Get_Font_Height_();
            fonts::Print_(364, (draw_y - font_h) + 16, _difficulty_string[_hof.difficulties[record_idx]]);

            if (MOX::_settings.language != 0) {
                fonts::Set_Font_Style_(4, &_hi->temp_colors);
                fonts::Set_Font_Spacing_(1);
            }
            graphics::Set_Window_(458, draw_y, 500, draw_y + 20);
            video::Clip_On_();
            fonts::Print_Integer_Right_(500, draw_y, _hof.scores[record_idx]);

            draw_y += 29;
        }

        graphics::Reset_Window_();
        video::Clip_Off_();

        if (_hi->is_first_draw == 0) {
            graphics::Set_Window_(0, 0, 640, 480);
            video::Clip_On_();
            int16_t mx = mouse::Pointer_X_();
            int16_t my = mouse::Pointer_Y_();
            if (mx > 636) mx = 636;
            if (my > 476) my = 476;
            animate::Draw_(mx, my, _hi->mouse_cursor_anim);
            graphics::Reset_Window_();
            video::Clip_Off_();
        }

        if (_entry_to_flash > -1 && _entry_to_flash < 10) {
            _hi->flash_timer_ticks++;
            if ((uint8_t)_hi->flash_timer_ticks % 6 == 0) {
                _hi->flash_color_offset += _hi->flash_direction;
                if (_hi->flash_color_offset == -1) {
                    _hi->flash_direction = 1;
                    _hi->flash_timer_ticks = 0;
                } else if (_hi->flash_color_offset == 6) {
                    _hi->flash_direction = -1;
                    _hi->flash_timer_ticks = 0;
                }
            }
        }

        _hi->is_first_draw = 0;
    }

    int16_t __cdecl Hi_Score_Screen_(int16_t player_idx) {
        bool finished = false;

        fields::Deactivate_Help_List_();

        if (SCORE::Read_HOF_File_() == 0) {
            SCORE::Create_Default_HOF_File_();
        }

        fields::Assign_Auto_Function_(SCORE::Draw_Hi_Score_Screen_, 1);
        fields::Clear_Fields_();
        SCORE::Load_Hi_Score_Screen_();

        SCORE::_hi->player_idx = (int8_t)player_idx;
        SCORE::Calculate_Hi_Scores_(player_idx);
        SCORE::_hi->highlighted_entry_idx = -1;

        SCORE::Get_Hi_Score_Colors_();
        SCORE::Add_Hi_Score_Screen_Fields_();
        SCORE::Init_Hi_Score_(player_idx);
        SCORE::Build_Captured_Colonists_Array_(player_idx);

        MISC::Fade_Into_Screen_(SCORE::Draw_Hi_Score_Screen_, &SCORE::_hi->is_first_draw, 12);
        remap::Update_Glass_Remap_Colors_();

        while (true) {
            int16_t input = fields::Get_Input_();
            if (SCORE::_hi->screen_field_handle == input) {
                finished = true;
            }

            if (finished) {
                break;
            }

            video::Set_Page_Off_();
            SCORE::Draw_Hi_Score_Screen_();
            RUSS::Mox_Sync_Update_();
        }

        video::Set_Page_Off_();
        SCORE::Draw_Hi_Score_Screen_();
        video::Copy_Off_To_Back_();
        video::Set_Refresh_Stencil_();

        MOX::_current_screen = MOX::_return_screen;
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();

        return SCORE::_hi->score_final;
    }

    /**
     * @address 0x004e45e0
     */
    void __cdecl Init_Hall_Of_Fame_Screen_() {
        fields::Clear_Fields_();

        _hi->highlighted_entry_idx = -2;
        _hi->is_first_draw = 1;

        for (int i = 0; i < 10; ++i) {
            _hi->sort_list[i][0] = _hof.scores[i];
            _hi->sort_list[i][1] = (int16_t)i;
        }

        qsort(_hi->sort_list, 10, 4, qsort_hof_scores_descending_);

        int16_t y = 131;
        for (int i = 0; i < 10; ++i) {
            int16_t handle = fields::Add_Hidden_Field_(139, y, 155, y + 22, s__00551100, 0);
            _hi->entry_field_handles[i] = handle;
            y += 29;
        }

        _hi->screen_field_handle = fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551120, 41);

        Get_Hall_Of_Fame_Colors_();

        _hi->exit_hotkey_handle = fields::Add_Multi_Hot_Key_Field_(s_C_00552420);

        fields::Disable_Draw_All_Fields_();
        mouse::Set_Mouse_List_(&MOX::_mouse_list_none, 1);
        ERIC::Fast_Fade_Out_();
        video::Set_Page_Off_();
        graphics::Fill_(0, 0, 639, 479, 0);
        RUSS::Mox_Sync_Update_();

        fonts::Load_Palette_(12, 0, 255);
        palette::Set_Palette_Changes_(0, 255);
        palette::Refresh_Palette_();
        palstore::Darken_Palette_(100);

        strcpy(_difficulty_string[0], ESTRINGS::E_Strings_(545));
        strcpy(_difficulty_string[1], ESTRINGS::E_Strings_(280));
        strcpy(_difficulty_string[2], ESTRINGS::E_Strings_(190));
        strcpy(_difficulty_string[3], ESTRINGS::E_Strings_(318));
        strcpy(_difficulty_string[4], ESTRINGS::E_Strings_(328));

        video::Set_Page_Off_();
        Draw_Hall_Of_Fame_Screen_();

        palette::Clear_Palette_Changes_(0, 255);
        ERIC::Fast_Fade_In_();
        video::Copy_Off_To_Back_();
        RUSS::Mox_Sync_Update_();
        fields::Enable_Draw_All_Fields_();

        animate::Draw_Palette_(_hi->background_anim);
    }

    void __cdecl Sort_HOF_Scores_(int16_t sort_list[10][2]) {
        for (int16_t i = 0; i < 10; ++i) {
            sort_list[i][0] = _hof.scores[i];
            sort_list[i][1] = i;
        }
        qsort(sort_list, 10, 4, qsort_hof_scores_ascending_);
    }

    void __cdecl Move_Sorted_HOF_Data_(int16_t sort_list[10][2]) {
        s_hof local_hof;

        local_hof.version = _hof.version;

        for (int16_t i = 0; i < 10; ++i) {
            int16_t idx = sort_list[i][1];

            strcpy(local_hof.player_names[i], _hof.player_names[idx]);
            local_hof.scores[i] = _hof.scores[idx];
            local_hof.race_ids[i] = _hof.race_ids[idx];
            local_hof.difficulties[i] = _hof.difficulties[idx];
            strcpy(local_hof.race_names[i], _hof.race_names[idx]);
        }

        _hof = local_hof;
    }

    /**
     * @address 0x004e45a0
     */
    void __cdecl Write_HOF_File_() {
        FILE* file;

        file = fopen(s_HOF_M2, s_wb);
        if (file != nullptr) {
            fwrite(&_hof, sizeof(s_hof), 1, file);
            fclose(file);
        }
    }

    /**
     * @address 0x004e33e0
     */
    void __cdecl Draw_Reg_Pop_() {
        SCORE::Draw_Pop_(0, 4, SCORE::_hi->reg_pop);
    }

    /**
     * @address 0x004e3380
     */
    void __cdecl Draw_Population_Score_() {
        char buffer[80];
        int raw_population = (int)SCORE::_hi->raw_population;
        char* format = (char*)HAROLD::H_Message_(0x15C /* HESTR_15C_D_POPULATION */);

        sprintf(buffer, format, raw_population);

        SCORE::Draw_Generic_Score_(
            SCORE::_hi->score_population,
            buffer,
            SCORE::Draw_Reg_Pop_,
            0x168
        );
    }

    /**
     * @address 0x004e2cf0
     */
    void __cdecl Draw_Elimination_Score_() {
        char message_buffer[80];
        int16_t num_defeated = SCORE::_hi->num_opponents_defeated;

        if (num_defeated == 0) {
            const char* msg = HAROLD::H_Message_(0x158 /* HESTR_158_PLAYERS_ELIMINATED */);
            strcpy(message_buffer, msg);
        } else {
            int16_t msg_id = (num_defeated == 1) ? 0x159 : 0x15a;
            const char* fmt = HAROLD::H_Message_(msg_id);
            sprintf(message_buffer, fmt, (int32_t)num_defeated);
        }

        SCORE::Draw_Generic_Score_(SCORE::_hi->score_elimination, message_buffer, SCORE::Draw_Eliminated_Race_Pictures_, 0x168);
    }

    /**
     * @address 0x004e2380
     */
    int32_t __cdecl Get_Captured_Colonist_Score_(int16_t player_idx) {
        int16_t captured_pop = MOX::_player[player_idx].captured_population;

        SCORE::_hi->raw_captured_colonies = captured_pop;

        int32_t product = (int32_t)((uint32_t)(uint16_t)captured_pop * 2);

        return product / (int32_t)((int8_t)MOX::_settings.galaxy_size + 1);
    }

    /**
     * @address 004e29c0
     */
    void __cdecl Draw_Captured_Colonies_Score_(void) {
        char buffer[80];
        int16_t message_id;
        const char* format;

        if (SCORE::_hi->raw_captured_colonies == 1) {
            message_id = 0x154 /* 340 */;
        } else {
            message_id = 0x155 /* 341 */;
        }

        format = HAROLD::H_Message_(message_id);
        sprintf(buffer, format, (int32_t)SCORE::_hi->raw_captured_colonies);

        SCORE::Draw_Generic_Score_(SCORE::_hi->score_captured_colonists, buffer, SCORE::Draw_Cap_Pop_, 0x168 /* 360 */);
    }

    /**
     * @address 0x004e2a20
     */
    void __cdecl Draw_Cap_Pop_() {
        SCORE::Draw_Pop_(1, 2, SCORE::_hi->raw_captured_population);
    }

    /**
     * @address 004e3430
     */
    void __cdecl Draw_Time_Score_() {
        char buffer[80];
        int16_t score = SCORE::_hi->score_time;
        int16_t stardate_calc = (int16_t)((uint16_t)MOX::_stardate + 30536 /* 0x7748 */);

        fonts::Set_Font_Style_(4, &SCORE::_hi->colors);
        fonts::Set_Font_Spacing_(1);
        int16_t font_height = fonts::Get_Font_Height_();

        const char* format = HAROLD::H_Message_(350 /* 0x15E */);
        sprintf(buffer, format, (int32_t)stardate_calc);

        SCORE::Print_Score_String_(140 /* 0x8C */, 139 /* 0x8B */, score, buffer);

        // next_draw_y update: font_height + 141 (0x8D) + 5 (BX calculation)
        SCORE::_hi->next_draw_y = font_height + 146 /* 0x92 */;
    }

    /**
     * @address 0x004e2d90
     */
    void __cdecl Draw_Eliminated_Race_Pictures_(void)
    {
        int8_t player_idx;
        int16_t base_y;
        int16_t current_x;
        int16_t icon_y;
        bool any_drawn;
        int16_t i;
        int16_t icon_width;
        int16_t icon_height;
        char buffer[40];

        player_idx = SCORE::_hi->player_idx;
        base_y = SCORE::_hi->next_draw_y;
        current_x = 0x8e;
        icon_y = base_y + 2;
        any_drawn = false;
        i = 0;

        if (MOX::_NUM_PLAYERS > 0) {
            do {
                uint16_t defeated_score = MOX::_player[player_idx].player_defeated[i];
                if (defeated_score != 0) {
                    uint8_t race = (uint8_t)MOX::_player[i].race;
                    s_animation_header* icon_anim = SCORE::_hi->race_icon_anims[race];

                    icon_width = animate::Get_Width_(icon_anim);
                    icon_height = animate::Get_Height_(icon_anim);
                    any_drawn = true;

                    animate::Draw_(current_x, icon_y, icon_anim);
                    graphics::Set_Window_(current_x, icon_y, current_x + icon_width, icon_height + icon_y);
                    video::Clip_On_();

                    fonts::Set_Font_Style_Outline_(1, &SCORE::_hi->colors);
                    fonts::Set_Outline_Color_(2, 1);
                    fonts::Set_Font_Spacing_(1);

                    sprintf(buffer, SCORE::s__d__d_005523a4, defeated_score / 10, defeated_score % 10);

                    int16_t centerX = current_x + (icon_width / 2);
                    fonts::Print_Centered_(centerX, base_y + 3, buffer);

                    fonts::Set_Font_Style_Outline_(1, &SCORE::_hi->colors);
                    fonts::Set_Outline_Color_(2, 1);

                    int16_t name_width = fonts::Get_String_Width_(MOX::_player[i].name);
                    int16_t font_height = fonts::Get_Font_Height_();
                    int16_t name_y = (icon_height - font_height) + icon_y - 1;

                    if (icon_width < name_width) {
                        fonts::Print_(current_x + 1, name_y, MOX::_player[i].name);
                    } else {
                        fonts::Print_Centered_(centerX, name_y, MOX::_player[i].name);
                    }

                    graphics::Reset_Window_();
                    video::Clip_Off_();

                    animate::Draw_(current_x - 1, base_y + 1, SCORE::_hi->race_icon_frame_anim);

                    // Draw background/border boxes
                    graphics::Box_(current_x - 2, base_y, icon_width + 3, icon_height + 3, 1, 0x4b /* 'K' */);
                    graphics::Box_(current_x - 1, base_y + 1, icon_width + 1, icon_height + 1, 1, 0x0f);

                    current_x = current_x + icon_width + 4;
                }
                i++;
            } while (i < MOX::_NUM_PLAYERS);
        }

        if (any_drawn) {
            SCORE::_hi->next_draw_y += icon_height + 4;
        }
    }

    /**
     * @address 004e3400
     */
    void __cdecl Draw_Technology_Score_() {
        char* label = (char*)HAROLD::H_Message_(349 /* HESTR_15D_TECHNOLOGY */);
        SCORE::Draw_Generic_Score_(SCORE::_hi->score_technology, label, nullptr, 360);
    }

    /**
     * @address 0x004e30e0
     */
    void __cdecl Draw_Orion_Score_() {
        char* label = (char*)HAROLD::H_Message_(0x15B /* HESTR_15B_CAPTURED_ORION */);
        Draw_Generic_Score_(_hi->score_orion, label, nullptr, 0);
    }

    /**
     * @address 004e2990
     */
    void __cdecl Draw_Antares_Score_() {
        char* message = (char*)HAROLD::H_Message_(0x153);
        Draw_Generic_Score_(_hi->score_antares, message, nullptr, 0);
    }

    /**
     * @address 0x004e2a40
     */
    void __cdecl Draw_Council_Score_(void) {
        int16_t council_score = SCORE::_hi->score_council;

        if (council_score > 0) {
            const char* message = HAROLD::H_Message_(342 /* HESTR_156_WON_COUNCIL_VOTE */);
            SCORE::Draw_Generic_Score_(council_score, (char*)message, 0, 0);
        }
    }

    /**
     * @address 0x004e21d0
     */
    void __cdecl Calculate_Hi_Scores_(int16_t player_idx) {
        char scor_buf[16];

        // This local string copy appears in the original binary but is not explicitly used.
        memcpy(scor_buf, s_scor_0055a510, 4);
        scor_buf[4] = s_scor_0055a510[4];

        SCORE::_hi->score_elimination = SCORE::Get_Elimination_Score_(player_idx);
        SCORE::_hi->score_technology = SCORE::Get_Technology_Score_(player_idx);
        SCORE::_hi->score_antares = SCORE::Get_Antares_Score_(player_idx);
        SCORE::_hi->score_orion = SCORE::Get_Orion_Score_(player_idx);
        SCORE::_hi->score_council = SCORE::Get_Won_Council_Score_(player_idx);
        SCORE::_hi->score_captured_colonists = SCORE::Get_Captured_Colonist_Score_(player_idx);
        SCORE::_hi->score_population = SCORE::Get_Population_Score_(player_idx);
        SCORE::_hi->score_time = SCORE::Get_Time_Score_();

        uint16_t raw_subtotal = (uint16_t)SCORE::_hi->score_time;
        raw_subtotal = (uint16_t)(raw_subtotal + (uint16_t)SCORE::_hi->score_antares);
        raw_subtotal = (uint16_t)(raw_subtotal + (uint16_t)SCORE::_hi->score_council);
        raw_subtotal = (uint16_t)(raw_subtotal + (uint16_t)SCORE::_hi->score_captured_colonists);
        raw_subtotal = (uint16_t)(raw_subtotal + (uint16_t)SCORE::_hi->score_orion);
        raw_subtotal = (uint16_t)(raw_subtotal + (uint16_t)SCORE::_hi->score_population);
        raw_subtotal = (uint16_t)(raw_subtotal + (uint16_t)SCORE::_hi->score_technology);
        raw_subtotal = (uint16_t)(raw_subtotal + (uint16_t)SCORE::_hi->score_elimination);
        SCORE::_hi->score_raw_subtotal = (int16_t)raw_subtotal;

        SCORE::_hi->score_modifier_pct = raceopt::Player_Score_Modifier_((int16_t)(uint8_t)SCORE::_hi->player_idx);

        int32_t product = (int32_t)SCORE::_hi->score_modifier_pct * (int32_t)SCORE::_hi->score_raw_subtotal;
        SCORE::_hi->score_final = (int16_t)((product + 50) / 100);
    }

    /**
     * @address 004e2a70
     */
    void __cdecl Draw_Total_Score_() {
        int16_t next_draw_y = _hi->next_draw_y;
        fonts::Set_Font_Style_(4, &_hi->colors);
        fonts::Set_Font_Spacing_(1);

        char label_buffer[80];
        strcpy(label_buffer, HAROLD::H_Message_(0x157));
        int16_t label_width = fonts::Get_String_Width_(label_buffer);

        char score_buffer[80];
        if (_hi->score_raw_subtotal == _hi->score_final) {
            sprintf(score_buffer, s_fmt_score_simple_00556104, (int)_hi->score_final);
        } else {
            char modifier_str[16];
            int16_t modifier = _hi->score_modifier_pct;
            if (modifier < 100) {
                sprintf(modifier_str, s_dot_d_0055a528, (int)modifier);
            } else {
                sprintf(modifier_str, s_fmt_modifier_full_005523a4, (int)(modifier / 100), (int)(modifier % 100));
            }
            sprintf(score_buffer, s_fmt_score_complex_0055a518, (int)_hi->score_raw_subtotal, modifier_str, (int)_hi->score_final);
        }

        int16_t score_width = fonts::Get_String_Width_(score_buffer);
        int16_t line_x_start = 500 - (score_width + 2);

        line::Line_(line_x_start, next_draw_y + 6, 498 /* 0x1f2 */, next_draw_y + 6, 0xD1);
        line::Line_(line_x_start, next_draw_y + 7, 498 /* 0x1f2 */, next_draw_y + 7, 0xD2);
        line::Line_(line_x_start, next_draw_y + 8, 498 /* 0x1f2 */, next_draw_y + 8, 0xD1);

        fonts::Get_Font_Height_();
        s_colors* dot_colors_ptr = MISC::Font_Colors2_(4, 0xD0, 0xD1);
        s_colors dot_colors = *dot_colors_ptr;

        char dot_str[4];
        strcpy(dot_str, s_dot_0055632c);
        int16_t dot_width = fonts::Get_String_Width_(dot_str);

        int16_t y = next_draw_y + 10;
        fonts::Set_Font_Style_(4, &_hi->colors);
        fonts::Set_Font_Spacing_(1);
        fonts::Print_(144 /* 0x90 */, y, label_buffer);

        int16_t current_x = label_width + dot_width + 144 /* 0x90 */;
        fonts::Set_Font_Style_(4, &dot_colors);
        fonts::Set_Font_Spacing_(1);

        while (current_x < line_x_start) {
            fonts::Print_(current_x, y, dot_str);
            current_x += dot_width;
        }

        fonts::Set_Font_Style_(4, &_hi->colors);
        fonts::Set_Font_Spacing_(1);
        fonts::Print_(current_x, y, score_buffer);
    }

    /**
     * @address 0x004e3680
     */
    void __cdecl Print_Score_String_(int16_t x, int16_t y, int16_t score, const char* label) {
        s_colors dot_colors;
        char dot_str[4];
        char label_buf[84];
        char score_buf[84];
        int16_t dot_width;
        int16_t label_width;
        int16_t score_width;
        s_colors* palette_colors;

        palette_colors = MISC::Font_Colors2_(4, 0xD0, 0xD1);
        dot_colors = *palette_colors;

        // Copy dot string "."
        strcpy(dot_str, s_dot_0055632c);
        dot_width = fonts::Get_String_Width_(dot_str);

        // Copy label string
        strcpy(label_buf, label);
        label_width = fonts::Get_String_Width_(label_buf);

        // Format score string
        sprintf(score_buf, s_fmt_score_simple_00556104, (int)score);
        score_width = fonts::Get_String_Width_(score_buf) + 3;

        // Print label
        fonts::Set_Font_Style_(4, &SCORE::_hi->colors);
        fonts::Set_Font_Spacing_(1);
        int16_t print_y = y + 4;
        fonts::Print_(x + 4, print_y, label_buf);

        // Print dots between label and score
        fonts::Set_Font_Style_(4, &dot_colors);
        fonts::Set_Font_Spacing_(1);

        int16_t limit_x = 499 /* 0x1F3 */ - score_width;
        int16_t current_x = x + label_width + dot_width + 4;

        while (current_x < limit_x) {
            fonts::Print_(current_x, print_y, dot_str);
            current_x += dot_width;
        }

        // Print score value aligned to the right
        fonts::Set_Font_Style_(4, &SCORE::_hi->colors);
        fonts::Set_Font_Spacing_(1);
        fonts::Print_Right_(499 /* 0x1F3 */, print_y, score_buf);
    }

    /**
     * @address 0x004e3070
     */
    void __cdecl Draw_Generic_Score_(int16_t value, const char* label, void (__cdecl *callback)(), int width) {
        int16_t current_y = SCORE::_hi->next_draw_y;

        fonts::Set_Font_Style_(4, &SCORE::_hi->colors);
        fonts::Set_Font_Spacing_(1);

        int16_t font_height = fonts::Get_Font_Height_();

        SCORE::Print_Score_String_(140, current_y, value, label);

        SCORE::_hi->next_draw_y = font_height + current_y + 2;

        if (callback != nullptr) {
            callback();
        }
    }

    void __cdecl Get_Hall_Of_Fame_Colors_() {
        s_colors* colors;

        MOX::_mox_font_colors_offset = 0xf0;

        colors = MISC::Font_Colors2_(0x4, 0xef, 0xf4);

        // Copy the 8-byte color structure
        memcpy(&SCORE::_hi->colors, colors, sizeof(s_colors));

        SCORE::_hi->flash_color_offset = 3;
        SCORE::_hi->flash_direction = 1;
        SCORE::_hi->flash_timer_ticks = 0;
    }

    /**
     * @address 0x004e2750
     */
    void __cdecl Add_Hi_Score_Screen_Fields_() {
        _hi->screen_field_handle = fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551120, 41);
    }

    /**
     * @address 0x004e28f0
     */
    void __cdecl Init_Hi_Score_(int16_t player_idx) {
        Get_Total_Population_(player_idx);
        uint32_t saved_seed = random::Get_Random_Seed_();
        Set_Hi_Score_Pop_Icon_Ids_(player_idx);
        random::Set_Random_Seed_(saved_seed);
    }

    /**
     * @address 0x004e24d0
     */
    int16_t __cdecl Get_Technology_Score_(int16_t player_idx) {
        int16_t tech_points;
        int16_t total_techs;

        SCORE::Get_Technology_(player_idx, &tech_points, &total_techs);

        return (tech_points * 3) + (total_techs * 5);
    }

    /**
     * @brief Calculates the score bonus for capturing Antares.
     * @param player_idx The index of the player.
     * @return 250 if Antares was captured, 0 otherwise.
     */
    int16_t __cdecl Get_Antares_Score_(int16_t player_idx) {
        if (SCORE::Captured_Antares_(player_idx)) {
            return 250 /* 0xFA */;
        }

        return 0;
    }

    /**
     * @address 004e2450
     */
    int16_t __cdecl Get_Orion_Score_(int16_t player_idx) {
        if (SCORE::Captured_Orion_(player_idx)) {
            return 100;
        }

        return 0;
    }

    /**
     * Calculates the time-based score component.
     * The score depends on the galaxy size, the number of players, and the current stardate.
     * Returns 0 if the player has no population.
     */
    int16_t __cdecl Get_Time_Score_() {
        int32_t time_score =
            (((int8_t)MOX::_settings.galaxy_size + 5) * MOX::_NUM_PLAYERS * 20) -
            (int16_t)((uint16_t)MOX::_stardate + 30536);

        return (SCORE::_hi->raw_population != 0) ? (int16_t)time_score : 0;
    }

    /**
     * Calculates and stores the total population score for a given player.
     * Updates the raw_population field in the high score structure.
     */
    int16_t __cdecl Get_Population_Score_(int16_t player_idx) {
        _hi->raw_population = SCORE::Get_Total_Population_(player_idx);
        return _hi->raw_population;
    }

    /**
     * @address 0x004e2470
     */
    int8_t __cdecl Captured_Orion_(int16_t player_idx) {
        return MOX::_player[player_idx].captured_orion;
    }

    /**
     * @address 0x004e2510
     */
    void __cdecl Get_Technology_(int16_t player_idx, int16_t* tech_count, int16_t* hyper_tech_count) {
        *tech_count = 0;
        *hyper_tech_count = 0;

        // Count completed technology fields (value 3 indicates known/researched)
        for (int16_t i = 0; i < 83 /* 0x53 */; ++i) {
            if (MOX::_player[player_idx].tech_fields[i] == 3) {
                *tech_count += 1;
            }
        }

        // Count active hyper advanced technologies
        for (int16_t i = 0; i < 8; ++i) {
            if (MOX::_player[player_idx].hyper_advanced_tech[i] != 0) {
                *hyper_tech_count += 1;
            }
        }
    }

    /**
     * @brief Checks if the specified player has captured Antares.
     * 
     * @param player_idx The index of the player to check.
     * @return int8_t Non-zero if Antares was captured, zero otherwise.
     */
    int8_t __cdecl Captured_Antares_(int16_t player_idx) {
        return MOX::_player[player_idx].captured_antares;
    }

    /**
     * @address 0x004e25e0
     */
    int16_t __cdecl Get_Total_Population_(int16_t player_idx) {
        int16_t total_pop = 0;

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
            if ((int16_t)MOX::_colony[i].owner == player_idx) {
                // Assembly zero-extends the n_pops byte into CX before adding to AX
                total_pop += (uint8_t)MOX::_colony[i].n_pops;
            }
        }

        return total_pop;
    }

    /**
     * @address 004e4ad0
     */
    void __cdecl Set_Hi_Score_Pop_Icon_Ids_(int16_t player_idx) {
        int16_t race_counts[14];
        bool limit_reached = false;
        int16_t collected_count = 0;

        int16_t total_pop_limit = SCORE::Get_Total_Population_(player_idx);
        memset(race_counts, 0, sizeof(race_counts));

        allocate::Push_Block_(MOX::_screen_seg);

        // s_hiscore_tmp_pop size is 5
        uint32_t alloc_size = total_pop_limit * 5;
        s_hiscore_tmp_pop* temp_buffer = (s_hiscore_tmp_pop*)allocate::Allocate_Next_Block_(MOX::_screen_seg, alloc_size);
        memset(temp_buffer, 0, alloc_size);

        for (int16_t col_idx = 0; col_idx < (int16_t)MOX::_NUM_COLONIES; ++col_idx) {
            if (limit_reached) break;

            if (MOX::_colony[col_idx].owner == player_idx) {
                for (int16_t pop_idx = 0; pop_idx < MOX::_colony[col_idx].n_pops; ++pop_idx) {
                    if (limit_reached) break;

                    uint32_t pop_entry = MOX::_colony[col_idx].pop[pop_idx];
                    int16_t p_idx = (int16_t)(pop_entry & 0xF);
                    int16_t race_id = (int16_t)(uint8_t)MOX::_player[p_idx].race;

                    if (race_id < 14) {
                        if (collected_count == total_pop_limit) {
                            limit_reached = true;
                        } else {
                            race_counts[race_id]++;
                            temp_buffer[collected_count].colony_idx = col_idx;
                            temp_buffer[collected_count].race_idx = (uint8_t)race_id;
                            temp_buffer[collected_count].pop_idx = (uint8_t)pop_idx;
                            collected_count++;
                        }
                    }
                }
            }
        }

        // Use the actual collected count for the shuffle and distribution phases
        int16_t final_count = total_pop_limit;

        if (final_count > 0) {
            for (int16_t i = final_count; i > 0; --i) {
                int16_t idx1 = (int16_t)random::Random_(final_count) - 1;
                int16_t idx2 = (int16_t)random::Random_(final_count) - 1;

                s_hiscore_tmp_pop swap_tmp = temp_buffer[idx1];
                temp_buffer[idx1] = temp_buffer[idx2];
                temp_buffer[idx2] = swap_tmp;
            }
        }

        for (int16_t race_id = 0; race_id < 14; ++race_id) {
            if (race_counts[race_id] > 0) {
                // Calculation: round( (race_counts / final_count) * 16 )
                int32_t num_to_draw = ((race_counts[race_id] * 160) / final_count + 5) / 10;

                // Enforce maximum of 16 icons total in high score report
                if (SCORE::_hi->reg_pop + num_to_draw > 16 && num_to_draw > 1) {
                    num_to_draw = 16 - SCORE::_hi->reg_pop;
                }

                for (int16_t j = 0; j < (int16_t)num_to_draw; ++j) {
                    if (SCORE::_hi->reg_pop >= 16) break;

                    SCORE::Get_From_Tmp_Pop_(
                        temp_buffer, 
                        race_id, 
                        final_count, 
                        &SCORE::_hi->pop_units[SCORE::_hi->reg_pop].race_id, 
                        &SCORE::_hi->pop_units[SCORE::_hi->reg_pop].icon_id
                    );

                    SCORE::_hi->reg_pop++;
                }
            }
        }

        allocate::Pop_Block_(MOX::_screen_seg);
    }

    int __cdecl ascending_qsort_uchar_(const void* a, const void* b) {
        return static_cast<int>(*static_cast<const uint8_t*>(a)) - static_cast<int>(*static_cast<const uint8_t*>(b));
    }

    void __cdecl Build_Captured_Colonists_Array_(int16_t player_idx) {
        uint32_t original_seed;
        uint16_t weights[8];
        int16_t captured_count;
        int16_t colonies;

        captured_count = 0;
        original_seed = random::Get_Random_Seed_();
        memset(weights, 0, sizeof(weights));

        for (int16_t i = 0; i < 8; ++i) {
            if (i != player_idx && MOX::_player[player_idx].first_contact[i] == 1) {
                weights[i] = 2;
            }

            if (MOX::_player[player_idx].player_defeated[i] != 0) {
                weights[i] = 5;
            }
        }

        colonies = SCORE::_hi->raw_captured_colonies;
        if (colonies > 16) {
            colonies = 16;
        }

        SCORE::_hi->raw_captured_population = colonies;

        if (SCORE::_hi->raw_captured_population > 0) {
            for (int16_t i = 0; i < SCORE::_hi->raw_captured_population; ++i) {
                int16_t choice = RUSS::Get_Weighted_Choice_Short_(weights, 8);
                if (choice >= 0 && choice < MOX::_NUM_PLAYERS && choice != player_idx) {
                    captured_count++;
                    SCORE::_hi->captured_pop_race_ids[i] = MOX::_player[choice].race;
                }
            }
        }

        qsort(SCORE::_hi->captured_pop_race_ids, 16, 1, SCORE::ascending_qsort_uchar_);

        SCORE::_hi->raw_captured_population = captured_count;
        random::Set_Random_Seed_(original_seed);
    }

    int8_t __cdecl Won_Council_Vote_(int16_t player_idx) {
        return static_cast<int8_t>(MOX::_player[player_idx].won_council_vote);
    }

    int16_t __cdecl Get_Won_Council_Score_(int16_t player_idx) {
        return SCORE::Won_Council_Vote_(player_idx) != 0 ? 100 : 0;
    }

    int16_t __cdecl Player_Score_(int16_t player_idx) {
        allocate::Push_Block_(MOX::_screen_seg);
        _hi = reinterpret_cast<s_hiscore*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x125));
        memset(_hi, 0, 0x125);
        _hi->player_idx = static_cast<int8_t>(player_idx);
        Calculate_Hi_Scores_(player_idx);
        allocate::Pop_Block_(MOX::_screen_seg);
        return _hi->score_final;
    }

    /**
     * @address 004e3540
     */
    void __cdecl Load_Hi_Score_Screen_() {
        uint32_t bitmap_size;
        int16_t lbx_index;

        _hi = static_cast<s_hiscore*>(allocate::Allocate_First_Block_(MOX::_screen_seg, sizeof(s_hiscore)));
        memset(_hi, 0, sizeof(s_hiscore));

        bitmap_size = bitmap::Get_Bitmap_Size_(600, 300);
        _hi->report_bitmap = static_cast<s_animation_header*>(allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size));
        bitmap::Create_Blank_Bitmap_(600, 300, _hi->report_bitmap);

        bitmap_size = bitmap::Get_Bitmap_Size_(50, 50);
        MAINSCR::_temp_bitmap = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, bitmap_size);

        _hi->main_background_anim = static_cast<s_animation_header*>(farload::Far_Reload_Next_(static_cast<const char*>(PTR_s_SCORE_LBX_0055a440), 0, MOX::_screen_seg));
        _hi->race_icon_frame_anim = static_cast<s_animation_header*>(farload::Far_Reload_Next_(static_cast<const char*>(PTR_s_SCORE_LBX_0055a440), 1, MOX::_screen_seg));

        lbx_index = 2;
        for (int i = 0; i < 13; ++i) {
            _hi->race_icon_anims[i] = static_cast<s_animation_header*>(farload::Far_Reload_Next_(static_cast<const char*>(PTR_s_SCORE_LBX_0055a440), lbx_index, MOX::_screen_seg));
            lbx_index++;
        }
    }

    void __cdecl Get_Hi_Score_Colors_() {
        // Set main high score colors
        SCORE::_hi->colors = *MISC::Font_Colors2_(4, 208 /* 0xD0 */, 210 /* 0xD2 */);

        // Set alternative colors for UI highlights
        SCORE::_hi->alt_colors = *MISC::Font_Colors2_(4, 208 /* 0xD0 */, 210 /* 0xD2 */);

        // Change global font color offset before fetching temporary colors
        MOX::_mox_font_colors_offset = 0xF3;
        SCORE::_hi->temp_colors = *MISC::Font_Colors2_(4, 208 /* 0xD0 */, 210 /* 0xD2 */);
    }

    /**
     * @address 0x004e3110
     */
    void __cdecl Draw_Pop_(uint8_t is_captured, int16_t max_pop, int16_t current_pop) {
        int16_t space;
        s_animation_header* anim;
        int16_t y_start;
        int16_t y_offset = 0;
        int16_t x_offset = 0;
        int16_t row_anim_count = 0;
        int16_t drawn_count = 0;

        y_start = SCORE::_hi->next_draw_y;

        anim = COLONY::C_Anims_(13);
        remap::Calculate_Picture_Remap_Colors_(anim);

        if (current_pop < max_pop * 48) {
            space = 12;
        } else if (current_pop < max_pop * 58) {
            space = 10;
        } else if (current_pop < max_pop * 72) {
            space = 8;
        } else if (current_pop < max_pop * 97) {
            space = 6;
        } else {
            space = 4;
        }

        if (current_pop > 0) {
            for (int16_t i = 0; i < current_pop; ++i) {
                if (i < max_pop * 97) {
                    if (is_captured == 0) {
                        anim = COLONY::Colony_Pop_Anim_(
                            SCORE::_hi->pop_units[i].race_id,
                            SCORE::_hi->pop_units[i].icon_id
                        );
                    } else {
                        anim = COLONY::Colony_Pop_Icon_(
                            static_cast<uint16_t>(static_cast<uint8_t>(SCORE::_hi->captured_pop_race_ids[i]))
                        );
                    }

                    if (current_pop > max_pop * 27) {
                        if ((row_anim_count & 1) == 0) {
                            animate::Remap_Draw_(x_offset + 140, y_start, anim);
                            y_offset = 0;
                        } else {
                            animate::Remap_Draw_(x_offset + 140, y_start + space, anim);
                            y_offset = space;
                        }
                        x_offset += space;
                        drawn_count++;

                        if (x_offset + 24 > 360) {
                            y_start += 30;
                            y_offset = 0;
                            row_anim_count = 0;
                            x_offset = 0;
                        }
                    } else {
                        x_offset = (drawn_count % 25) * 22;
                        y_offset = (drawn_count / 25) * 22;
                        animate::Remap_Draw_(x_offset + 140, y_start + y_offset, anim);
                        drawn_count++;
                    }
                    row_anim_count++;
                }
            }

            if (current_pop > 0) {
                SCORE::_hi->next_draw_y = y_start + y_offset + 28;
                return;
            }
        }

        SCORE::_hi->next_draw_y += 3;
    }

    /**
     * @address 004e4d70
     */
    void __cdecl Get_From_Tmp_Pop_(s_hiscore_tmp_pop* tmp_pop_list, uint16_t race_idx, int16_t count, int16_t* out_colony_idx, int16_t* out_pop_idx) {
        int16_t first_match_colony = -1;
        int16_t first_match_pop = -1;
        bool found_unused = false;
        int16_t unused_index = 0;

        for (int16_t i = 0; i < count; ++i) {
            s_hiscore_tmp_pop* entry = &tmp_pop_list[i];

            if (entry->race_idx == (uint8_t)race_idx) {
                if (first_match_colony == -1) {
                    first_match_colony = entry->colony_idx;
                    // Note: Compiler cleverness with register reuse results in this pattern
                    first_match_pop = (int16_t)entry->pop_idx;
                }

                if (entry->is_used == 0) {
                    *out_colony_idx = entry->colony_idx;
                    *out_pop_idx = (int16_t)entry->pop_idx;
                    found_unused = true;
                    unused_index = i;
                }
            }
        }

        if (found_unused) {
            tmp_pop_list[unused_index].is_used = 1;
        } else {
            // Potential bug: This branch performs a comparison and dereference of an 
            // address (0xFFFFFFFF) that is typically invalid in Win32.
            if (reinterpret_cast<std::uintptr_t>(out_colony_idx) == kInvalidOutColonyIdxMarker) {
                *out_colony_idx = first_match_colony;
                *out_pop_idx = first_match_pop;
            }
        }
    }
}