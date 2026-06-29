#include "pch.h"

namespace CHAT {
    s_net_chat* _chat_info;

    void __cdecl Receive_Chat_Msg_(uint8_t player_id, const char* text) {
        size_t len = strlen(text) + 1;

        if (_chat_info->count == 14) {
            memmove(_chat_info, _chat_info->messages + 1, sizeof(_chat_info->messages) - sizeof(_chat_info->messages[0]));
            _chat_info->count = 13;
        }

        _chat_info->messages[_chat_info->count].player_id = player_id;
        memcpy(_chat_info->messages[_chat_info->count].text, text, len);
        _chat_info->count++;
    }

    void __cdecl Send_Chat_Msg_(char* msg) {
        for (int32_t player_index = 0; player_index < MOX::_NUM_PLAYERS; ++player_index) {
            if (MOX::_player[player_index].objectives == PLAYER_OBJECTIVE_HUMAN) {
                int32_t message_len = (int32_t)(strlen(msg) + 1);
                NETMOX::Mox_Send_Message_(MOX::_player[player_index].network_player_id, NET_PACKET_CHAT, msg, (uint16_t)message_len);
                RUSS::Mox_Update_();
            }
        }
    }

    void __cdecl Send_GNN_Chat_Msg_(char* msg) {
        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int32_t player_index = 0; player_index < MOX::_NUM_PLAYERS; ++player_index) {
            if (MOX::_player[player_index].objectives == PLAYER_OBJECTIVE_HUMAN) {
                int32_t len = (int32_t)strlen(msg);

                if (len > 80) {
                    msg[80] = '\0';
                    len = 80;
                }

                NETMOX::Mox_Send_Message_(MOX::_player[player_index].network_player_id, NET_PACKET_GNN_CHAT, msg, (uint16_t)(len + 1));
                RUSS::Mox_Update_();
            }
        }
    }
}