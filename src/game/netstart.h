#pragma once
#include <cstdint>

namespace NETSTART {
    uint8_t __cdecl Start_Net_Screen_();
    uint8_t __cdecl Join_Net_Screen_();
    uint8_t __cdecl Load_Net_Screen_();

    extern char* _dialing_failed_error;
    extern char* _receiving_chunk_d_of_d;
    extern char* _really_begin_game_with_unused_slots_msg;
    extern char* _really_start_with_only_1_msg;
    extern char* _no_more_player_slots_error;
    extern char* _game_full_string;
    extern char* _answering_failed_error;
    extern char* _windows_string;

    extern char s_myname_005597dc[7];

    int32_t __cdecl Russ_Net_Create_Game_(GUID guid, char* game_name, int32_t max_players, char* str1, char* str2);

    bool __cdecl Do_Dial_(char* phone_number);

    char* __cdecl Load_Netstart_Strings_(char* buffer);
    int8_t __cdecl Request_Race_(uint8_t race_idx);
    int8_t __cdecl Request_Banner_Color_(uint8_t requested_color);

    void __cdecl Mox_Net_Uninit_();

    int __cdecl Dump_Users_();

    int8_t __cdecl Net_Select_Race_();

    int8_t __cdecl Net_Pick_Position_();

    int8_t __cdecl Do_Answer_();
}