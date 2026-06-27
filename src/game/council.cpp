#include "pch.h"

namespace council {
    const char lbx_00559a68[12] = "council.lbx";
    const char LBX_00559a74[12] = "council.LBX";

    s_mouse_list _council_blank_mouse = {0, 0, 0, 0, 639, 479};
    s_colors _council_normal_colors = {{150, 154, 155, 151, 152, 153, 154, 155}};
    s_colors _council_highlight_colors = {{150, 155, 155, 152, 153, 155, 155, 155}};
    uint8_t _council_nominee_[2];
    int16_t _council_total_votes;
    int16_t _council_nominee_vote_total[2];
    uint8_t _council_members[MAX_PLAYERS];
    uint8_t _council_state;
    char* _council_vote_message;
    s_animation_header* _council_dark_race_seg[MAX_PLAYERS];
    int8_t _council_voter;
    char* _council_no_votes_message;
    int16_t _council_vote_cast;
    int16_t _council_member_votes[MAX_PLAYERS];
    s_animation_header* _council_light_race_seg[MAX_PLAYERS];
    char* _council_message;
    uint8_t _player_voted_winner_message_id[20];
    char* _council_votes_message;
    int16_t _council_nominee_votes[2];
    void* _council_mouse_bitmap;
    uint8_t _winning_player;
    s_animation_header* _council_nominees_seg[2];
    uint8_t _council_total_members;
    char* _council_votes_total_message;
    uint8_t _council_vote_for;

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
        if (current_turn_offset < 25) {
            return;
        }

        if ((int32_t)diplodef::_next_council_meeting_turn > current_turn_offset &&
            diplodef::_COUNCIL_FLAG != 0) {
            return;
        }

        if (active_players <= 2) {
            return;
        }

        int32_t num_stars = MOX::_NUM_STARS;
        int16_t half_stars = (int16_t)((num_stars - (num_stars >> 31)) >> 1);
        if (owned_stars < half_stars) {
            return;
        }

        council::Main_Council_Screen_();
        diplodef::_next_council_meeting_turn = (int16_t)MOX::_stardate + 30561;
    }

    void __cdecl Control_Main_Council_Screen_() {
        int16_t field_id;
        int16_t input_id;
        int16_t done = 0;

        fields::Assign_Auto_Function_(Draw_Main_Council_Screen_, 2);

        while (done == 0) {
            timer::Mark_Time_();
            fields::Clear_Fields_();

            field_id = fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0);
            input_id = fields::Get_Input_();

            if (field_id == input_id) {
                done = 1;
            }

            if (done == 0) {
                Draw_Main_Council_Screen_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(2);
            }
        }

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
    }

    void __cdecl Main_Council_Screen_() {
        fields::Deactivate_Help_List_();
        fields::Enable_Draw_All_Fields_();
        graphics::Reset_Window_();
        video::Clip_Off_();
        mouse::Set_Mouse_List_(&council::_council_blank_mouse, 1);
        JIM::Play_Streaming_Music_(0x77 /* MUSIC_COUNCIL */, -1, 1);
        ERIC::Set_Fade_Percent_(0);
        ERIC::Set_Fade_Speed_Percent_(10);
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
        ERIC::Set_Fade_Speed_Percent_(10);
        ERIC::Fast_Fade_Out_();
        RUSS::Release_All_Blocks_(MOX::_screen_seg);
        MOX2::Put_Auto_Function_(&EVENTS::_jim_saved_autofunction);
        MISC::Fade_Into_Screen_(MAINSCR::Draw_Main_Screen_, &MOX::_draw_main_screen_to_back, 0);
    }

    void __cdecl Draw_Main_Council_Screen_() {
        int16_t x_coords[6] = { 44, 520, 143, 444, 215, 377 };
        int16_t y_coords[6] = { 96, 100, 148, 149, 169, 168 };
        char buffer[40];

        video::Set_Page_Off_();
        fonts::Set_Outline_Color_(0, 0);

        if (MOX::_settings.animations_on == 1) {
            file_ani::Draw_Multi_File_Animation_(0);
            if (file_ani::Get_Multi_File_Animation_Frame_(0) == 0) {
                file_ani::Set_Multi_File_Animation_Frame_(0, 1);
            }
        }

        fonts::Set_Outline_Color_(0, 0);
        fonts::Set_Font_Style_Outline_(2, &council::_council_normal_colors);

        int16_t print_x = 30;
        for (int i = 0; i < 2; ++i) {
            if (council::_council_nominee_vote_total[i] < 1) {
                strlcpy(buffer, council::_council_no_votes_message, sizeof(buffer));
                JIM::Decode_Text_Race_(buffer, council::_council_nominee_[i], sizeof(buffer));
            } else {
                char* msg = (council::_council_nominee_vote_total[i] < 2) ? council::_council_vote_message : council::_council_votes_message;
                strlcpy(buffer, msg, sizeof(buffer));
                JIM::Decode_Text_Name_And_Stat_(buffer, council::_council_nominee_[i], council::_council_nominee_vote_total[i], sizeof(buffer));
            }
            fonts::Print_(print_x, 30, buffer);
            print_x += 421;
        }

        snprintf(buffer, sizeof(buffer), "%d %s", (int)council::_council_total_votes, council::_council_votes_total_message);
        fonts::Print_Centered_(320, 30, buffer);

        for (int i = 0; i < council::_council_total_members; ++i) {
            s_animation_header* anim;
            if ((council::_council_state == 2 || council::_council_state == 3) && council::_council_members[i] == council::_council_voter) {
                anim = council::_council_light_race_seg[i];
            } else {
                anim = council::_council_dark_race_seg[i];
            }
            animate::Draw_(x_coords[i], y_coords[i], anim);
        }

        int16_t nominee_x = 115;
        for (int i = 0; i < 2; ++i) {
            int16_t frame = animate::Get_Current_Frame_(council::_council_nominees_seg[i]);
            animate::Draw_(nominee_x, 225, council::_council_nominees_seg[i]);
            if (council::_council_nominee_[i] != council::_council_voter) {
                animate::Set_Animation_Frame_(council::_council_nominees_seg[i], frame);
            }
            nominee_x += 205;
        }

        fonts::Set_Outline_Color_(0, 0);
        fonts::Set_Font_Style_Outline_(4, &council::_council_normal_colors);
        
        int16_t msg_h = BILL::_Get_Paragraph_Max_Height_(550, council::_council_message);
        BILL::_Print_Paragraph_(55, 470 - msg_h, 550, council::_council_message, 2);

        JIM::Play_Streaming_Music_(119 /* 0x77 */, -1, 1);
        RUSS::Mox_Update_();
    }

    void Council_Player_Vote_() {
        char list_items[4][25];
        char title_msg[48];
        int16_t vote_choice = 0;

        int8_t list_metadata;
        uint8_t active_mask[4];

        fields::Assign_Auto_Function_(council::Draw_Main_Council_Screen_, 2);

        council::_council_vote_cast = council::Get_Num_Votes_((uint8_t)MOX::_PLAYER_NUM);
        council::_council_voter = (uint8_t)MOX::_PLAYER_NUM;

        for (int i = 0; i < 4; ++i) {
            list_items[i][0] = '\0';
        }

        JIM::Get_Text_Message_("jimtext.lbx", 23 /* COUNCIL_VOTE_CAST_INFO */, title_msg, sizeof(title_msg));
        JIM::Decode_Text_Stat_(title_msg, council::_council_vote_cast, sizeof(title_msg));

        if (MOX::_PLAYER_NUM == council::_council_nominee_[0]) {
            JIM::Get_Text_Message_("jimtext.lbx", 24 /* ME */, list_items[0], sizeof(list_items[0]));
        } else {
            snprintf(list_items[0], sizeof(list_items[0]), "  %s", MOX::_player[council::_council_nominee_[0]].name);
        }

        if (MOX::_PLAYER_NUM == council::_council_nominee_[1]) {
            JIM::Get_Text_Message_("jimtext.lbx", 24 /* ME */, list_items[1], sizeof(list_items[1]));
        } else {
            snprintf(list_items[1], sizeof(list_items[1]), "  %s", MOX::_player[council::_council_nominee_[1]].name);
        }

        JIM::Get_Text_Message_("jimtext.lbx", 25 /* ABSTAIN */, list_items[2], sizeof(list_items[2]));

        active_mask[0] = 1;
        active_mask[1] = 1;
        active_mask[2] = 1;
        active_mask[3] = 1;
        fonts::Set_Outline_Color_(0, 0);
        fonts::Set_Font_Style_(4, &council::_council_normal_colors);
        fonts::Set_Font_Highlight_Colors_(&council::_council_highlight_colors);
        fonts::Set_Font_LF_(4);

        council::_council_message[0] = '\0';
        list_metadata = 0;

        int16_t selection = fields::Get_List_Field_(
            214, 388, 180, title_msg, (char*)list_items, 25,
            &list_metadata, active_mask, 17, nullptr, 0, nullptr, 0, 0
        );

        if (selection == 0) {
            council::_winning_player = council::_council_nominee_[0];
            vote_choice = 1;
            MOX::_player[council::_council_voter].council_voted_for = council::_council_nominee_[0];
        } else if (selection == 1) {
            council::_winning_player = council::_council_nominee_[1];
            vote_choice = 2;
            MOX::_player[council::_council_voter].council_voted_for = council::_council_nominee_[1];
        } else if (selection == 2) {
            vote_choice = 0;
            MOX::_player[council::_council_voter].council_voted_for = -1;
        } else {
            vote_choice = 0;
        }

        council::Council_Vote_Result_((int8_t)vote_choice, (uint8_t)MOX::_PLAYER_NUM);
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        council::_council_voter = -1;
    }

    void __cdecl Council_Vote_Result_(int8_t vote_choice, uint8_t player_idx) {
        uint8_t nominee_id;

        if (vote_choice == 0) {
            council::_council_state = 3;
            diplomac::Change_Relations_(-6, player_idx, council::_council_nominee_[1], 0, 0, 0);
            diplomac::Change_Relations_(-6, player_idx, council::_council_nominee_[0], 0, 0, 0);
            MOX::_player[player_idx].council_voted_for = -1;
        } else if (vote_choice == 1) {
            council::_council_nominee_vote_total[0] += council::_council_vote_cast;
            council::_council_state = 2;
            council::_council_vote_for = 0;
            diplomac::Change_Relations_(24, player_idx, council::_council_nominee_[0], 16, 0, 0);
            diplomac::Change_Relations_(-12, player_idx, council::_council_nominee_[1], 9, 0, 0);
            nominee_id = council::_council_nominee_[0];
            MOX::_player[player_idx].council_voted_for = nominee_id;
        } else if (vote_choice == 2) {
            council::_council_nominee_vote_total[1] += council::_council_vote_cast;
            council::_council_state = 2;
            council::_council_vote_for = 1;
            diplomac::Change_Relations_(24, player_idx, council::_council_nominee_[1], 16, 0, 0);
            diplomac::Change_Relations_(-12, player_idx, council::_council_nominee_[0], 9, 0, 0);
            nominee_id = council::_council_nominee_[1];
            MOX::_player[player_idx].council_voted_for = nominee_id;
        }

        council::Get_Council_Message_();
        council::Control_Main_Council_Screen_();
    }

    int16_t __cdecl Get_Num_Votes_(uint8_t player_idx) {
        int16_t total_pop = MOX::_player[player_idx].total_pop;

        if ((total_pop % 10) == 0) {
            return total_pop / 10;
        }

        return (total_pop / 10) + 1;
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
            /* No operation for these states. */
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

    void __cdecl Council_Votes_() {
        if (_council_total_members != 0) {
            for (int i = 0; i < (int)_council_total_members; ++i) {
                uint8_t member_id = _council_members[i];

                if ((int16_t)member_id == MOX::_PLAYER_NUM) {
                    continue;
                }

                uint8_t nominee0 = _council_nominee_[0];
                uint8_t nominee1 = _council_nominee_[1];

                if (member_id == nominee0 || member_id == nominee1) {
                    continue;
                }

                int16_t vote0 = Vote_Check_(nominee0, member_id, nominee1);
                int16_t vote1 = Vote_Check_(nominee1, member_id, nominee0);

                int16_t vote_choice;
                if (vote0 == 1) {
                    /* If member votes for both nominees, they abstain. */
                    vote_choice = (vote1 == 1) ? 0 : 1;
                } else {
                    /* If member supports only nominee 1, cast the vote for nominee 1. */
                    vote_choice = (vote1 == 1) ? 2 : 0;
                }

                _council_vote_cast = _council_member_votes[i];
                _council_voter = member_id;
                Council_Vote_Result_((int8_t)vote_choice, member_id);
            }
        }

        /* Process automatic votes for nominees if they are not the player. */
        if ((int16_t)_council_nominee_[0] != MOX::_PLAYER_NUM) {
            int16_t votes = _council_nominee_votes[0];
            int16_t vote_choice = (votes >= 1) ? 1 : 0;

            _council_vote_cast = votes;
            _council_voter = _council_nominee_[0];
            Council_Vote_Result_((int8_t)vote_choice, _council_nominee_[0]);
        }

        if ((int16_t)_council_nominee_[1] != MOX::_PLAYER_NUM) {
            int16_t votes = _council_nominee_votes[1];
            int16_t vote_choice = (votes >= 1) ? 2 : 0;

            _council_vote_cast = votes;
            _council_voter = _council_nominee_[1];
            Council_Vote_Result_((int8_t)vote_choice, _council_nominee_[1]);
        }

        Council_Player_Vote_();
    }

    void __cdecl Reload_Main_Council_Screen_() {
        s_animation_header* mouse_h;
        int16_t max_votes;
        int16_t cur_votes;
        int16_t player_idx;

        _council_mouse_bitmap = allocate::Allocate_First_Block_(MOX::_screen_seg, 917 /* 0x395 */);
        mouse_h = (s_animation_header*)farload::Far_Reload_Next_(council::LBX_00559a74, 0, MOX::_screen_seg);
        bitmap::Clear_Mouse_Image_(0);
        bitmap::Create_Blank_Bitmap_(30, 30, _council_mouse_bitmap);
        bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)_council_mouse_bitmap, mouse_h);
        video::Set_Page_Off_();
        animate::Draw_Palette_(mouse_h);
        palette::Set_Palette_Changes_(0, 255);
        graphics::Fill_(0, 0, 639, 479, 0);
        bitmap::Draw_Bitmap_To_Mouse_(0, 0, (s_animation_header*)_council_mouse_bitmap, 0);

        max_votes = 0;
        player_idx = 0;
        if (MOX::_NUM_PLAYERS > 0) {
            do {
                if (MOX::_player[player_idx].eliminated == 0) {
                    cur_votes = Get_Num_Votes_((uint8_t)player_idx);
                    if (max_votes < cur_votes) {
                        max_votes = cur_votes;
                        _council_nominee_[0] = (uint8_t)player_idx;
                    }
                }
                player_idx++;
            } while (player_idx < MOX::_NUM_PLAYERS);
        }

        max_votes = 0;
        player_idx = 0;
        if (MOX::_NUM_PLAYERS > 0) {
            do {
                if (MOX::_player[player_idx].eliminated == 0 && (uint8_t)player_idx != _council_nominee_[0]) {
                    cur_votes = Get_Num_Votes_((uint8_t)player_idx);
                    if (max_votes < cur_votes) {
                        max_votes = cur_votes;
                        _council_nominee_[1] = (uint8_t)player_idx;
                    }
                }
                player_idx++;
            } while (player_idx < MOX::_NUM_PLAYERS);
        }

        _council_total_members = 0;
        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].eliminated == 0 && (uint8_t)i != _council_nominee_[0] && (uint8_t)i != _council_nominee_[1]) {
                _council_members[_council_total_members] = (uint8_t)i;
                _council_total_members++;
            }
        }

        RUSS::Mox_Update_();
        _council_total_votes = 0;
        for (int16_t i = 0; i < (int16_t)_council_total_members; ++i) {
            cur_votes = Get_Num_Votes_(_council_members[i]);
            _council_member_votes[i] = cur_votes;
            _council_total_votes += cur_votes;
        }

        cur_votes = Get_Num_Votes_(_council_nominee_[0]);
        _council_nominee_votes[0] = cur_votes;
        _council_total_votes += cur_votes;

        cur_votes = Get_Num_Votes_(_council_nominee_[1]);
        _council_nominee_votes[1] = cur_votes;
        _council_total_votes += cur_votes;

        if (_council_total_members != 0) {
            for (int16_t i = 0; i < (int16_t)_council_total_members; ++i) {
                for (int16_t j = 0; j < (int16_t)_council_total_members - 1; ++j) {
                    if (_council_member_votes[j] < _council_member_votes[j + 1]) {
                        strings::Swap_Short_(&_council_member_votes[j], &_council_member_votes[j + 1]);
                        strings::Swap_Char_(&_council_members[j], &_council_members[j + 1]);
                    }
                }
            }
        }

        for (int16_t i = 0; i < (int16_t)_council_total_members; ++i) {
            uint8_t race = (uint8_t)MOX::_player[_council_members[i]].race;
            int16_t resource_idx = (int16_t)race * 6 + i + 2;
            if (i == 0) {
                _council_light_race_seg[i] = (s_animation_header*)farload::Far_Reload_(council::lbx_00559a68, resource_idx, MOX::_screen_seg);
            } else {
                _council_light_race_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(council::lbx_00559a68, resource_idx, MOX::_screen_seg);
            }
            _council_dark_race_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(council::lbx_00559a68, (int16_t)race * 6 + i + 80 /* 0x50 */, MOX::_screen_seg);
        }

        RUSS::Mox_Update_();
        uint8_t nominee0_race = MOX::_player[_council_nominee_[0]].race;
        _council_nominees_seg[0] = (s_animation_header*)farload::Far_Reload_Next_(council::lbx_00559a68, (int16_t)nominee0_race * 2 + 158 /* 0x9E */, MOX::_screen_seg);
        uint8_t nominee1_race = MOX::_player[_council_nominee_[1]].race;
        _council_nominees_seg[1] = (s_animation_header*)farload::Far_Reload_Next_(council::lbx_00559a68, (int16_t)nominee1_race * 2 + 159 /* 0x9F */, MOX::_screen_seg);

        _council_no_votes_message = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30 /* 0x1E */);
        _council_vote_message = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30 /* 0x1E */);
        _council_votes_message = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30 /* 0x1E */);
        _council_votes_total_message = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 30 /* 0x1E */);
        _council_message = (char*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 210 /* 0xD2 */);

        int32_t ani_buffer_size = allocate::Get_Remaining_Block_Space_(MOX::_screen_seg) - 100;
        void* ani_buffer = allocate::Allocate_Next_Block_(MOX::_screen_seg, ani_buffer_size);
        file_ani::Set_File_Animation_Buffer_(ani_buffer);
        video::Set_Refresh_Stencil_();
        file_ani::Open_Multi_File_Animation_(council::LBX_00559a74, 1, 0);
        video::Set_Page_Off_();
        file_ani::Draw_Multi_File_Animation_(0);
        video::Copy_Off_To_Back_();
        RUSS::Mox_Update_();
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        Load_Council_Text_Messages_();
        RUSS::Mox_Update_();
        _council_voter = -1;
    }

    void __cdecl Get_Council_Message_() {
        char temp_buffer[80];

        switch (_council_state) {
        case 0:
            JIM::Get_Text_Message_("jimtext.lbx", 15, _council_message, 200);
            break;

        case 1:
            JIM::Get_Text_Message_("jimtext.lbx", 16, _council_message, 200);
            JIM::Decode_Text_Name_And_Name_(_council_message, _council_nominee_[0], _council_nominee_[1], 200);
            break;

        case 2:
            JIM::Get_Text_Message_("jimtext.lbx", 17, _council_message, 200);
            JIM::Decode_Text_Name_And_Stat_(_council_message, _council_nominee_[_council_vote_for], _council_vote_cast, 200);
            break;

        case 3:
            JIM::Get_Text_Message_("jimtext.lbx", 18, _council_message, 200);
            JIM::Decode_Text_Name_And_Stat_(_council_message, _council_voter, _council_vote_cast, 200);
            break;

        case 4:
            JIM::Get_Text_Message_("jimtext.lbx", 19, _council_message, 200);
            JIM::Decode_Text_Race_(_council_message, _council_nominee_[_council_vote_for], 200);
            break;

        case 5:
            JIM::Get_Text_Message_("jimtext.lbx", 20, _council_message, 200);
            break;

        case 6:
            JIM::Get_Text_Message_("jimtext.lbx", 21, _council_message, 200);
            if (diplodef::_COUNCIL_FLAG == 2) {
                JIM::Get_Text_Message_("jimtext.lbx", 22, temp_buffer, sizeof(temp_buffer));
                JIM::Decode_Text_Race_(temp_buffer, _winning_player, sizeof(temp_buffer));
                strcat(_council_message, temp_buffer);
            }
            break;

        default:
            break;
        }
    }

    void __cdecl Council_Finished_Voting_() {
        int16_t total_votes = council::_council_total_votes;
        int32_t threshold = (total_votes * 2 + 2) / 3;

        if (council::_council_nominee_vote_total[0] >= threshold) {
            council::_council_vote_for = 0;
            council::_council_state = 4;

            if ((int16_t)council::_council_nominee_[0] == MOX::_PLAYER_NUM) {
                diplodef::_COUNCIL_FLAG = 2;
            } else {
                diplodef::_COUNCIL_FLAG = 3;
            }
        } else if (council::_council_nominee_vote_total[1] >= threshold) {
            council::_council_vote_for = 1;
            council::_council_state = 4;

            if ((int16_t)council::_council_nominee_[1] == MOX::_PLAYER_NUM) {
                diplodef::_COUNCIL_FLAG = 2;
            } else {
                diplodef::_COUNCIL_FLAG = 3;
            }
        } else {
            council::_council_state = 5;
        }

        council::Get_Council_Message_();
        council::Control_Main_Council_Screen_();
        council::Next_Council_State_();
    }

    void Council_Accept_Ruling_() {
        char choice_labels[2][25];
        char prompt_text[40];
        int8_t list_metadata;
        uint8_t active_mask[4];

        fields::Assign_Auto_Function_(Draw_Main_Council_Screen_, 2);

        if (diplodef::_COUNCIL_FLAG != 1) {
            *council::_council_message = '\0';
            fonts::Set_Outline_Color_(0, 0);
            fonts::Set_Font_Style_(4, &council::_council_normal_colors);
            fonts::Set_Font_Highlight_Colors_(&council::_council_highlight_colors);
            fonts::Set_Font_LF_(4);

            JIM::Get_Text_Message_("jimtext.lbx", 0x1A, choice_labels[0], sizeof(choice_labels[0]));
            JIM::Get_Text_Message_("jimtext.lbx", 0x1B, choice_labels[1], sizeof(choice_labels[1]));
            JIM::Get_Text_Message_("jimtext.lbx", 0x1C, prompt_text, sizeof(prompt_text));

            list_metadata = 0;
            active_mask[0] = 1;
            active_mask[1] = 1;
            active_mask[2] = 1;
            active_mask[3] = 1;

            int16_t selection = fields::Get_List_Field_(
                0xD6, 0x184, 0xB4, 
                prompt_text, 
                choice_labels[0], 
                0x19, 
                &list_metadata, 
                active_mask, 
                0x11, 
                nullptr, 
                0, 
                nullptr, 
                0, 
                0
            );

            if (selection == 0) {
                council::_council_state = 0x63 /* 99 */;
                diplodef::_council_winner = council::_council_nominee_[council::_council_vote_for];
                
                if (MOX::_PLAYER_NUM == diplodef::_council_winner) {
                    Council_Player_Voted_Winner_();
                    ERIC::Play_Diplomatic_Win_Flic_();
                }
                
                MOX::_player[diplodef::_council_winner].won_council_vote = 1;
            }
            else if (selection == 1) {
                council::_council_state = 6;
                if (diplodef::_COUNCIL_FLAG == 2) {
                    council::_winning_player = council::_council_nominee_[0];
                    if ((int16_t)council::_council_nominee_[0] == MOX::_PLAYER_NUM) {
                        council::_winning_player = council::_council_nominee_[1];
                    }
                }
                else {
                    council::_winning_player = council::_council_nominee_[council::_council_vote_for];
                }

                council::Get_Council_Message_();
                council::Control_Main_Council_Screen_();
                council::Next_Council_State_();

                for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    if (council::_winning_player != i && MOX::_PLAYER_NUM != i) {
                        if (MOX::_player[i].eliminated == 0) {
                            if (COLCALC::May_Surrender_Empire_(i, council::_winning_player)) {
                                COLCALC::Surrender_Empire_(i, council::_winning_player);
                            }
                        }
                    }
                }
                
                COLCALC::Do_Surrenders_();

                diplomac::Break_Trade_(council::_winning_player, MOX::_PLAYER_NUM);
                diplomac::Break_Research_(council::_winning_player, MOX::_PLAYER_NUM);
                diplomac::Break_Tribute_(council::_winning_player, MOX::_PLAYER_NUM);
                diplomac::Break_Treaties_(council::_winning_player, MOX::_PLAYER_NUM);
                diplomac::Declare_War_(council::_winning_player, MOX::_PLAYER_NUM);
            }
        }

        fields::Deactivate_Auto_Function_();
        fields::Clear_Fields_();
    }

    void Council_Player_Voted_Winner_() {
        memset(_player_voted_winner_message_id, 0xff, sizeof(_player_voted_winner_message_id));

        fields::Assign_Auto_Function_(Draw_Council_Player_Voted_Winner_, 2);

        /* Select the nominee index opposite to the player's vote. */
        int16_t other_nominee_idx = _council_nominee_[_council_vote_for == 0 ? 1 : 0];
        Drive_Council_Player_Voted_Winner_(other_nominee_idx, 0);

        if (_council_total_members != 0) {
            for (int i = 0; i < (int)_council_total_members; ++i) {
                uint8_t member_idx = _council_members[i];
                _council_members[i] = member_idx + 100;
                Drive_Council_Player_Voted_Winner_((int16_t)member_idx, 0);
            }
        }
    }

    void __cdecl Load_Council_Text_Messages_() {
        JIM::Get_Text_Message_(DIP_SCRN::s_jimtext_lbx_00552360, 11, _council_no_votes_message, 30);
        JIM::Get_Text_Message_(DIP_SCRN::s_jimtext_lbx_00552360, 12, _council_vote_message, 30);
        JIM::Get_Text_Message_(DIP_SCRN::s_jimtext_lbx_00552360, 13, _council_votes_message, 30);
        JIM::Get_Text_Message_(DIP_SCRN::s_jimtext_lbx_00552360, 14, _council_votes_total_message, 30);
    }

    bool __cdecl Vote_Check_(int16_t candidate_idx, int16_t voter_idx, int16_t opponent_idx) {
        int16_t vote_score;
        int8_t treaty_type = MOX::_player[voter_idx].treaty[candidate_idx];

        if (treaty_type >= 4 /* TREATY_ALLIANCE */) {
            vote_score = -1;
        } else if (treaty_type == 2 /* TREATY_PEACE */) {
            vote_score = 200;
        } else {
            /* Base score calculated from diplomatic standing and trust. */
            vote_score = MOX::_player[voter_idx].broken_treaties_modifier[candidate_idx] +
                         ((MOX::_player[voter_idx].trust_worthiness[candidate_idx] -
                           MOX::_player[voter_idx].diplomacy_proposal_rejection[candidate_idx] * 10) * 2) +
                         MOX::_player[voter_idx].relations[candidate_idx];

            /* If the voter is allied with the opposing candidate, increase the score requirement. */
            if (MOX::_player[voter_idx].treaty[opponent_idx] >= 4 /* TREATY_ALLIANCE */) {
                vote_score += 100;
            }

            /* Favor the current Orion owner. */
            if (HACCESS::Orion_Owner_() == candidate_idx) {
                vote_score += 40;
            }

            /* Apply diplomatic trait modifiers. */
            if (MOX::_player[candidate_idx].traits[TRAIT_CHARISMATIC] == 1) {
                vote_score += 40;
            }
            if (MOX::_player[candidate_idx].traits[TRAIT_REPULSIVE] == 1) {
                vote_score -= 100;
            }

            /* Apply the relevant diplomacy tech modifier. */
            if (MOX::_player[candidate_idx].tech_applications[TECH_APP_XENO_PSYCHOLOGY] == 3) {
                vote_score += 30;
            }

            /* Favor the same candidate as in the previous council vote. */
            if (MOX::_player[voter_idx].council_voted_for == (int8_t)candidate_idx) {
                vote_score += 50;
            }

            /* Apply active treaty bonuses. */
            if (MOX::_player[voter_idx].treaty[candidate_idx] == 1 /* TREATY_NON_AGGRESSION */) {
                vote_score += 50;
            }
            if (MOX::_player[voter_idx].trade_treaty[candidate_idx] != 0) {
                vote_score += 25;
            }
            if (MOX::_player[voter_idx].research_treaty[candidate_idx] != 0) {
                vote_score += 25;
            }

            if (MOX::_player[candidate_idx].objectives == 100) {
                if (MOX::_settings.game_difficulty > 2) {
                    vote_score = (int16_t)((vote_score * 6) / (MOX::_settings.game_difficulty + 6));
                }
            }
        }

        /* Determine whether the vote is cast based on the final score. */
        return (int32_t)game_random::Random_(200) <= (int32_t)vote_score;
    }

    void __cdecl Get_Player_Voted_Winner_Message_(int16_t unused_callback_arg) {
        (void)unused_callback_arg;
        int16_t message_idx;
        do {
            /* BUG: message_idx can be -1 if Random_(14) returns 0. */
            message_idx = (int16_t)game_random::Random_(14) - 1;
        } while (_player_voted_winner_message_id[message_idx] != 0xFF);

        _player_voted_winner_message_id[message_idx] = (uint8_t)message_idx;

        /* Calculate the entry index for the current language block. */
        int32_t message_entry_idx = (int16_t)((int16_t)MOX::_settings.language * 14 + message_idx);

        farload::Farload_Data_Static_(
            "councmsg.lbx",
            message_entry_idx,
            _council_message,
            0,
            1,
            200
        );
    }

    void Drive_Council_Player_Voted_Winner_(int16_t player_idx, int16_t unused_callback_arg) {
        Get_Player_Voted_Winner_Message_(unused_callback_arg);

        int16_t race_anim_id = (int16_t)((uint8_t)MOX::_player[player_idx].race + 0xB8);
        file_ani::Open_Multi_File_Animation_("council.LBX", race_anim_id, 1);

        bool exit_loop = false;
        while (!exit_loop) {
            timer::Mark_Time_();
            fields::Clear_Fields_();

            int16_t hidden_field_id = fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0);
            int16_t input_id = fields::Get_Input_();

            if (hidden_field_id == input_id) {
                exit_loop = true;
            }

            if (!exit_loop) {
                Draw_Council_Player_Voted_Winner_();
                RUSS::Mox_Sync_Update_();
                timer::Release_Time_(2);
            }
        }
    }

    void __cdecl Draw_Council_Player_Voted_Winner_() {
        int16_t x_coords[6] = { 44, 520, 143, 444, 215, 377 };
        int16_t y_coords[6] = { 96, 100, 148, 149, 169, 168 };

        video::Set_Page_Off_();
        fonts::Set_Outline_Color_(0, 0);

        if (MOX::_settings.animations_on == 1) {
            file_ani::Draw_Multi_File_Animation_(0);
            if (file_ani::Get_Multi_File_Animation_Frame_(0) == 0) {
                file_ani::Set_Multi_File_Animation_Frame_(0, 1);
            }
            file_ani::Draw_Multi_File_Animation_(1);
        }

        for (int i = 0; i < (int)_council_total_members; ++i) {
            if (_council_members[i] < 100) {
                animate::Draw_(x_coords[i], y_coords[i], _council_dark_race_seg[i]);
            }
        }

        fonts::Set_Outline_Color_(0, 0);
        fonts::Set_Font_Style_Outline_(4, &_council_normal_colors);

        int16_t msg_height = BILL::_Get_Paragraph_Max_Height_(550, _council_message);
        BILL::_Print_Paragraph_(55, 470 - msg_height, 550, _council_message, 2);

        JIM::Play_Streaming_Music_(119 /* 0x77 */, -1, 1);
        RUSS::Mox_Update_();
    }
}