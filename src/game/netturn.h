#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct s_net_election_msg {
    int32_t rank;
    int8_t flag;
};
#pragma pack(pop)

namespace NETTURN {
    void __cdecl Net_Next_Turn_();
    extern const char s_craziness_00557680[10];

    void __cdecl Host_Next_Turn_();

    void __cdecl Client_Next_Turn_();

    char* __cdecl Load_Netturn_Strings_(char* input_strings);

    extern char* _waiting_fmt;

    extern char* _have_d_of_d_chunks_fmt;

    int8_t __cdecl Must_Broadcast_AI_Player_Data_();

    void __cdecl Net_Init_Host_Election_();
}