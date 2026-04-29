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

    void __cdecl Host_Next_Turn_();

    void __cdecl Client_Next_Turn_();

    char* __cdecl Load_Netturn_Strings_(char* input_strings);

    inline char*& _waiting_fmt =
        *reinterpret_cast<char**>(std::uintptr_t{0x005A63E4});

    inline char*& _have_d_of_d_chunks_fmt =
        *reinterpret_cast<char**>(std::uintptr_t{0x005662A8});

    int8_t __cdecl Must_Broadcast_AI_Player_Data_();

    void __cdecl Net_Init_Host_Election_();
}