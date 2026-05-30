#pragma once
#include <cstdint>

struct s_net_chat;

namespace CHAT {
    extern s_net_chat* _chat_info;

    void __cdecl Send_Chat_Msg_(char* msg);

    void __cdecl Send_GNN_Chat_Msg_(char* msg);

    void __cdecl Receive_Chat_Msg_(uint8_t player_id, const char* text);
}