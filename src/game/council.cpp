#include "pch.h"

namespace council {
    void Check_For_Council_Meeting_() {
        int16_t active_players = 0;
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].eliminated == 0) {
                active_players++;
            }
        }

        int16_t owned_stars = 0;
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (MOX::_star[i].owner != -1) {
                owned_stars++;
            }
        }

        int32_t current_turn_offset = MOX::_stardate - 35000;
        if (current_turn_offset >= 25) {
            bool time_for_meeting = ((int16_t)diplodef::_next_council_meeting_turn <= (int16_t)current_turn_offset) ||
                                     (diplodef::_COUNCIL_FLAG == 0);

            if (time_for_meeting && active_players > 2 && owned_stars >= (MOX::_NUM_STARS / 2)) {
                council::Main_Council_Screen_();
                diplodef::_next_council_meeting_turn = (int16_t)MOX::_stardate + 30561;
            }
        }
    }

    void __cdecl Control_Main_Council_Screen_() {
        int16_t field_id;
        int16_t input_id;
        int16_t done = 0;
        int16_t unused_flag = 0;

        fields::Assign_Auto_Function_(Draw_Main_Council_Screen_, 2);

        while (done == 0) {
            timer::Mark_Time_();
            fields::Clear_Fields_();

            field_id = fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551100, 0);
            input_id = fields::Get_Input_();

            if (field_id == input_id) {
                done = 1;
            }

            if (done == 0 && unused_flag == 0) {
                Draw_Main_Council_Screen_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(2);
            }

            unused_flag = 0;
        }

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
    }

    void __cdecl Main_Council_Screen_() {
        fields::Deactivate_Help_List_();
        fields::Enable_Draw_All_Fields_();
        graphics::Reset_Window_();
        video::Clip_Off_();
        mouse::Set_Mouse_List_(&DAT_00559a48, 1);
        JIM::Play_Streaming_Music_(0x77 /* MUSIC_COUNCIL */, -1, 1);
        ERIC::Set_Fade_Percent_(0);
        Set_Fade_Speed_Perc(10);
        ERIC::Fast_Fade_Out_();
        fields::Set_Input_Delay_(1);
        fields::Clear_Fields_();
        video::Set_Refresh_Stencil_();
        MOX2::Get_Auto_Function_(&EVENTS::_jim_saved_autofunction);
        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
        council::Reload_Main_Council_Screen_();
        council::_council_state = 0;
        diplodef::_COUNCIL_FLAG = 1;
        council::_council_nominee_vote_total[0] = 0;
        council::_council_nominee_vote_total[1] = 0;
        fields::Assign_Auto_Function_(council::Draw_Main_Council_Screen_, 2);

        while (council::_council_state != 0x63 /* 'c' */) {
            council::Get_Council_Message_();
            council::Control_Main_Council_Screen_();
            council::Next_Council_State_();
            if (council::_council_state == 2) {
                council::Council_Votes_();
                council::Council_Finished_Voting_();
                council::Council_Accept_Ruling_();
            }
        }

        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
        REPORT::_something_interesting_happened[MOX::_PLAYER_NUM] = 1;
        file_ani::Reset_File_Animation_Buffer_();
        JIM::Play_Background_Music_();
        ERIC::Set_Fade_Percent_(0);
        Set_Fade_Speed_Perc(10);
        ERIC::Fast_Fade_Out_();
        RUSS::Release_All_Blocks_(MOX::_screen_seg);
        MOX2::Put_Auto_Function_(&EVENTS::_jim_saved_autofunction);
        MISC::Fade_Into_Screen_(MAINSCR::Draw_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
    }

    void __cdecl Next_Council_State_() {
        switch (council::_council_state) {
        case 0:
            council::_council_state = 1;
            break;
        case 1:
            council::_council_state = 2;
            break;
        case 2:
        case 3:
            // No operation for these states
            break;
        case 4:
        case 5:
        case 6:
            council::_council_state = 99 /* 0x63 */;
            break;
        default:
            break;
        }
    }
}