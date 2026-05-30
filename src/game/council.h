#pragma once

struct s_colors;
struct s_animation_header;
struct s_mouse_list;

namespace council {
    extern const char lbx_00559a68[12];
    extern const char LBX_00559a74[12];

    extern s_colors _council_normal_colors;
    extern s_colors _council_highlight_colors;
    extern uint8_t _council_nominee_[2];
    extern int16_t _council_total_votes;
    extern int16_t _council_nominee_vote_total[2];
    extern uint8_t _council_members[8];
    extern uint8_t _council_state;
    extern char* _council_vote_message;
    extern s_animation_header* _council_dark_race_seg[8];
    extern int8_t _council_voter;
    extern char* _council_no_votes_message;
    extern int16_t _council_vote_cast;
    extern int16_t _council_member_votes[8];
    extern s_animation_header* _council_light_race_seg[8];
    extern char* _council_message;
    extern uint8_t _player_voted_winner_message_id[20];
    extern char* _council_votes_message;
    extern int16_t _council_nominee_votes[2];
    extern void* _council_mouse_bitmap;
    extern uint8_t _winning_player;
    extern s_animation_header* _council_nominees_seg[2];
    extern uint8_t _council_total_members;
    extern char* _council_votes_total_message;
    extern uint8_t _council_vote_for;

    void __cdecl Get_Player_Voted_Winner_Message_(int16_t unused);

    void __cdecl Draw_Council_Player_Voted_Winner_();

    extern s_mouse_list _council_blank_mouse;

    void Drive_Council_Player_Voted_Winner_(int16_t player_idx, int16_t unused);

    void Check_For_Council_Meeting_();

    void __cdecl Next_Council_State_();

    void __cdecl Main_Council_Screen_();

    void __cdecl Draw_Main_Council_Screen_();

    void __cdecl Control_Main_Council_Screen_();

    void Council_Player_Vote_();

    int16_t __cdecl Get_Num_Votes_(uint8_t player_idx);

    void __cdecl Council_Vote_Result_(int8_t vote_option, uint8_t player_idx);

    bool __cdecl Vote_Check_(int16_t candidate_idx, int16_t voter_idx, int16_t opponent_idx);

    void __cdecl Reload_Main_Council_Screen_();

    void __cdecl Load_Council_Text_Messages_();

    void __cdecl Get_Council_Message_();

    void __cdecl Council_Votes_();

    void __cdecl Council_Finished_Voting_();

    void Council_Accept_Ruling_();

    void Council_Player_Voted_Winner_();
}