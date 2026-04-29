#pragma once
#include <cstdint>

struct s_net_chat;

namespace CHAT {
    inline s_net_chat*& _chat_info = *reinterpret_cast<s_net_chat**>(std::uintptr_t{0x005A7B70});

    void __cdecl Send_Chat_Msg_(char* msg);

    void __cdecl Send_GNN_Chat_Msg_(char* msg);

    void __cdecl Receive_Chat_Msg_(uint8_t player_id, void* text);
}