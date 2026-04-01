#pragma once

struct s_colors;
struct s_animation_header;

namespace council {
    inline s_colors& _council_normal_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00559A58});
    inline s_colors& _council_highlight_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00559A60});
    inline uint8_t (&_council_nominee_)[2] = *reinterpret_cast<uint8_t(*)[2]>(std::uintptr_t{0x00596016});
    inline int16_t& _council_total_votes = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596018});
    inline int16_t (&_council_nominee_vote_total)[2] = *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x0059601C});
    inline uint8_t* _council_members = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596020});
    inline uint8_t& _council_state =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596028});
    inline char*& _council_vote_message = *reinterpret_cast<char**>(std::uintptr_t{0x0059602C});
    inline s_animation_header** _council_dark_race_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596030});
    inline uint8_t& _council_voter = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596050});
    inline char*& _council_no_votes_message = *reinterpret_cast<char**>(std::uintptr_t{0x00596054});
    inline int16_t& _council_vote_cast = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00596080});
    inline int16_t (&_council_member_votes)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00596090});
    inline s_animation_header** _council_light_race_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00596060});
    inline char*& _council_message = *reinterpret_cast<char**>(std::uintptr_t{0x005960A0});
    inline uint8_t (&_player_voted_winner_message_id)[20] = *reinterpret_cast<uint8_t(*)[20]>(std::uintptr_t{0x005960B0});
    inline char*& _council_votes_message = *reinterpret_cast<char**>(std::uintptr_t{0x005960C4});
    inline int16_t (&_council_nominee_votes)[2] = *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x005960C8});
    inline void*& _council_mouse_bitmap = *reinterpret_cast<void**>(std::uintptr_t{0x005960CC});
    inline uint8_t& _winning_player = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005960D0});
    inline s_animation_header** _council_nominees_seg = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005960D8});
    inline uint8_t& _council_total_members = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005960E0});
    inline char*& _council_votes_total_message = *reinterpret_cast<char**>(std::uintptr_t{0x005960E4});
    inline uint8_t& _council_vote_for =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005960E8});

    inline const char* s_format_votes = reinterpret_cast<const char*>(std::uintptr_t{0x005569F0});

    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});

    void __cdecl Get_Player_Voted_Winner_Message_(int16_t player_idx);

    void __cdecl Draw_Council_Player_Voted_Winner_();

    inline const char* s_council_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00559A74});
    inline const char* s_empty_string = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});
    inline s_mouse_list& _council_blank_mouse = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{ 0x00559A48 });

    void Drive_Council_Player_Voted_Winner_(int16_t player_idx);

    void Check_For_Council_Meeting_();

    void __cdecl Next_Council_State_();

    void __cdecl Main_Council_Screen_();

    void __cdecl Draw_Main_Council_Screen_();

    void __cdecl Control_Main_Council_Screen_();

    void Council_Player_Vote_();

    int16_t __cdecl Get_Num_Votes_(uint8_t player_idx);

    void __cdecl Council_Vote_Result_(int16_t vote_option, int16_t player_idx);

    bool __cdecl Vote_Check_(int16_t candidate_idx, int16_t voter_idx, int16_t opponent_idx);

    void __cdecl Reload_Main_Council_Screen_();

    void __cdecl Load_Council_Text_Messages_();

    void __cdecl Get_Council_Message_();

    void __cdecl Council_Votes_();

    void __cdecl Council_Finished_Voting_();

    void Council_Accept_Ruling_();

    void Council_Player_Voted_Winner_();
}