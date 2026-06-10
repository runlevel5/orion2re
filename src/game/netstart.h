#pragma once
#include <stdint.h>

#pragma pack(push, 1)
struct s_load_net_screen_packet {
    int16_t begin_state;
    uint8_t field2_0x2;
    uint8_t field3_0x3;
    uint8_t field4_0x4;
    uint8_t field5_0x5;
    int32_t network_ids[MAX_PLAYERS];
    uint8_t field38_0x26;
    uint8_t field39_0x27;
    int8_t number_of_players;
    uint8_t field41_0x29;
    uint8_t field42_0x2a;
    uint8_t field43_0x2b;
    uint8_t field44_0x2c;
    uint8_t field45_0x2d;
    char version_string[50];
};
#pragma pack(pop)

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

    int32_t __cdecl Russ_Net_Create_Game_(const char* game_name, int32_t max_players, const char* str1, const char* str2);

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