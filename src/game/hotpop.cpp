#include "pch.h"
#include "enhanced_ux.h"

namespace HOTPOP {
    int16_t _hotseat_human_players;
    s_animation_header* _hot_pop_background_seg;
    s_animation_header* _grid_background[4];

    void __cdecl Fade_Into_Hotseat_Screen_() {
        ERIC::Fast_Fade_Out_();

        fonts::Load_Palette_(11, 0, 255);
        palette::Set_Palette_Changes_(0, 255);
        palette::Refresh_Palette_();
        palstore::Darken_Palette_(100);

        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

        video::Set_Page_Off_();
        animate::Draw_(0, 0, HOTPOP::_grid_background[0]);

        palette::Clear_Palette_Changes_(0, 255);
        RUSS::Mox_Sync_Update_();

        ERIC::Fast_Fade_In_();

        video::Set_Page_Back_();
        animate::Draw_(0, 0, HOTPOP::_grid_background[0]);

        video::Copy_Back_To_Off_();
    }

    void __cdecl Reload_Hotseat_Screen_() {
        _grid_background[0] = (s_animation_header*)farload::Far_Reload_(MULTPLAY::s_MULTIGM_LBX, 0, MOX::_screen_seg);
        _hot_pop_background_seg = (s_animation_header*)farload::Far_Reload_Next_(MULTPLAY::s_MULTIGM_LBX, 0x0B, MOX::_screen_seg);
        _grid_background[1] = (s_animation_header*)farload::Far_Reload_Next_(MULTPLAY::s_MULTIGM_LBX, 0x0C, MOX::_screen_seg);
        _grid_background[2] = (s_animation_header*)farload::Far_Reload_Next_(MULTPLAY::s_MULTIGM_LBX, 0x0D, MOX::_screen_seg);
        _grid_background[3] = (s_animation_header*)farload::Far_Reload_Next_(MULTPLAY::s_MULTIGM_LBX, 0x0E, MOX::_screen_seg);
    }

    void __cdecl Init_Hotseat_Screen_(int16_t* out_join_button_field, int16_t* out_accept_button_field, int16_t* out_cancel_button_field) {
        int16_t base_x;
        int16_t base_y;
        int16_t anim_w;
        int16_t anim_h;

        HOTPOP::Reload_Hotseat_Screen_();

        if (HOTPOP::_hotseat_human_players > 0) {
            HOTPOP::Fade_Into_Hotseat_Screen_();
        }

        fields::Clear_Fields_();

        anim_w = animate::Get_Width_(HOTPOP::_hot_pop_background_seg);
        base_x = (640 - anim_w) / 2;

        anim_h = animate::Get_Height_(HOTPOP::_hot_pop_background_seg);
        base_y = (480 - anim_h) / 2;

        // Enhancement: hotkeys for Add Human Player (A) and Begin Game (B).
        const bool enhanced_hotkeys = (ENHANCED_UX::enhanced_ux_settings.enabled != 0);

        if (HOTPOP::_hotseat_human_players < MAX_PLAYERS) {
            *out_join_button_field = fields::Add_Button_Field_(base_x + 125, base_y + 99, "", _grid_background[1], enhanced_hotkeys ? "A" : "", 40);
        } else {
            *out_join_button_field = -1000;
        }

        *out_accept_button_field = fields::Add_Button_Field_(base_x + 125, base_y + 136, "", _grid_background[2], enhanced_hotkeys ? "B" : "", 40);

        /* "\x1b" is a non-printing fmtpara ESC control byte reused here as a field-label sentinel. */
        *out_cancel_button_field = fields::Add_Button_Field_(base_x + 140, base_y + 200, "", _grid_background[3], "\x1b", 40);

        fields::Assign_Auto_Function_(HOTPOP::Draw_Hotseat_Screen_, 2);
    }

    void __cdecl Hotseat_Screen_(void) {
        int16_t btn_join;
        int16_t btn_accept;
        int16_t btn_cancel;
        int16_t input;
        int16_t player_idx;
        int16_t exit_code;
        s_player* temp_player;
        int16_t center_x;
        int16_t center_y;
        char msg_players_plural[20];
        char remaining_player_label[20];
        char warning_buf[200];

        MOX::_hot_seat_flag = 3;
        Init_Hotseat_Screen_(&btn_join, &btn_accept, &btn_cancel);

        center_x = (640 - animate::Get_Width_(HOTPOP::_hot_pop_background_seg)) / 2;
        center_y = (480 - animate::Get_Height_(HOTPOP::_hot_pop_background_seg)) / 2;

        HOTPOP::_hotseat_human_players = 0;
        player_idx = 0;
        allocate::Push_Block_(MOX::_global_data_seg);
        temp_player = (s_player*)allocate::Allocate_Next_Block_(MOX::_global_data_seg, sizeof(s_player));
        exit_code = 0;

        struct_::Clear_Structure_(temp_player, sizeof(s_player));

        do {
            input = fields::Get_Input_();

            if (HOTPOP::_hotseat_human_players == 0) {
                input = btn_join;
            }

            if (input == btn_cancel) {
                MOX::_current_screen = SCREEN_MULTI_PLAYER;
                exit_code = 1;
            }

            if (input == btn_accept) {
                if (HOTPOP::_hotseat_human_players == 1) {
                    if (GENDRAW::Confirmation_Box_((char*)HAROLD::H_Message_(162)) == 0) {
                        video::Set_Page_Back_();
                        animate::Draw_(0, 0, _grid_background[0]);
                        video::Copy_Back_To_Off_();
                    } else {
                        MOX::_game_type = 0;
                        exit_code = 2;
                    }
                } else {
                    strlcpy(msg_players_plural, HAROLD::H_Message_(165), sizeof(msg_players_plural));
                    
                    if ((MOX::_settings.number_of_players - HOTPOP::_hotseat_human_players) == 1) {
                        strlcpy(remaining_player_label, HAROLD::H_Message_(164), sizeof(remaining_player_label));
                    } else {
                        strlcpy(remaining_player_label, HAROLD::H_Message_(165), sizeof(remaining_player_label));
                    }

                    snprintf(warning_buf, sizeof(warning_buf), HAROLD::H_Message_(163), 
                            HOTPOP::_hotseat_human_players, msg_players_plural, 
                            (MOX::_settings.number_of_players - HOTPOP::_hotseat_human_players), 
                            remaining_player_label);

                    if (GENDRAW::Confirmation_Box_(warning_buf) == 0) {
                        video::Set_Page_Back_();
                        animate::Draw_(0, 0, _grid_background[0]);
                        video::Copy_Back_To_Off_();
                    } else {
                        exit_code = 2;
                    }
                }
            }

            if (input == btn_join) {
                MOX::_PLAYER_NUM = player_idx;
                if (RACESEL::Race_Selection_Screen_(temp_player) == 0) {
                    EVANHELP::Set_Hotseat_Setup_Screen_Help_List_(center_x, center_y);
                    if (HOTPOP::_hotseat_human_players == 0) {
                        MOX::_current_screen = SCREEN_MULTI_PLAYER;
                        exit_code = 1;
                    }
                } else {
                    EVANHELP::Set_Hotseat_Setup_Screen_Help_List_(center_x, center_y);
                    struct_::Copy_Structure_(&MOX::_player[player_idx], temp_player, sizeof(s_player));
                    MOX::_player[player_idx].objectives = 100;
                    player_idx++;
                    HOTPOP::_hotseat_human_players++;
                }

                Init_Hotseat_Screen_(&btn_join, &btn_accept, &btn_cancel);
                if (MOX::_settings.number_of_players == HOTPOP::_hotseat_human_players) {
                    exit_code = 2;
                }
            }

            if (exit_code != 0) {
                fields::Clear_Fields_();
                allocate::Pop_Block_(MOX::_global_data_seg);
                fields::Deactivate_Auto_Function_();

                if (exit_code == 2) {
                    INITGAME::Init_New_Game_(
                        MOX::_settings.game_difficulty,
                        MOX::_settings.galaxy_size,
                        MOX::_settings.galaxy_age,
                        MOX::_settings.number_of_players,
                        MOX::_settings.strategic_combat_flag,
                        MOX::_settings.starting_civilization_level
                    );
                    MOX::_current_screen = SCREEN_HOTSEAT_SELECT_PLAYER;
                }
                return;
            }

            video::Set_Page_Off_();
            HOTPOP::Draw_Hotseat_Screen_();
            RUSS::Mox_Sync_Update_();

        } while (true);
    }

void __cdecl Draw_Hotseat_Screen_(void)
{
    int16_t bg_x;
    int16_t bg_y;
    s_colors active_colors;
    s_colors inactive_colors;
    s_colors unused_colors;

    bg_x = (640 - animate::Get_Width_(HOTPOP::_hot_pop_background_seg)) / 2;
    bg_y = (480 - animate::Get_Height_(HOTPOP::_hot_pop_background_seg)) / 2;

    memset(&unused_colors, 0xA7, sizeof(unused_colors));

    animate::Draw_(bg_x, bg_y, HOTPOP::_hot_pop_background_seg);

    inactive_colors = *MISC::Font_Colors2_(1, 0x9a, 0x9c);
    active_colors = *MISC::Font_Colors2_(1, 0x9a, 0x9f);

    fonts::Set_Font_Style_Shadow_Down_(1, &active_colors);

    if ((int8_t)MOX::_settings.number_of_players > 0) {
        int16_t text_x = bg_x + 14;
        int16_t text_y = bg_y + 70;

        for (int16_t i = 0; i < (int8_t)MOX::_settings.number_of_players; ++i) {
            if (i < (int16_t)HOTPOP::_hotseat_human_players) {
                fonts::Set_Font_Style_Shadow_Down_(1, &active_colors);
            }
            else {
                fonts::Set_Font_Style_Shadow_Down_(1, &inactive_colors);
            }

            fonts::Print_Integer_(text_x, text_y, i + 1);
            text_y += 9;
        }
    }
}

}