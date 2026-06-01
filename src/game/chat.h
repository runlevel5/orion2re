#pragma once
#include <cstdint>

struct s_net_chat;

namespace CHAT {
    extern s_net_chat* _chat_info;

    /** Sends a regular chat message to each connected human player.
     *  @param msg Null-terminated chat text to broadcast.
     */
    void __cdecl Send_Chat_Msg_(char* msg);

    /** Sends a GNN chat message to each connected human player, truncating it to the network text limit.
     *  @param msg Null-terminated chat text buffer, truncated in place to 80 characters.
     */
    void __cdecl Send_GNN_Chat_Msg_(char* msg);

    /** Appends an incoming chat message to the rolling local chat buffer.
     *  @param player_id Sender player identifier.
     *  @param text Null-terminated message text to copy into the buffer.
     */
    void __cdecl Receive_Chat_Msg_(uint8_t player_id, const char* text);
}