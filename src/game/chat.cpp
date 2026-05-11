#include "pch.h"

namespace CHAT {
    void __cdecl Receive_Chat_Msg_(uint8_t player_id, void* pack) {
        const char* text = (const char*)pack;

        if (_chat_info->count == 14) {
            memmove(_chat_info, _chat_info->messages + 1, 0x42A);
            _chat_info->count = 13;
        }

        _chat_info->messages[_chat_info->count].player_id = player_id;
        strcpy(_chat_info->messages[_chat_info->count].text, text);
        _chat_info->count++;
    }

    void __cdecl Send_Chat_Msg_(char* param_1) {
        for (int32_t player_index = 0; player_index < MOX::_NUM_PLAYERS; ++player_index) {
            if (MOX::_player[player_index].objectives == 0x64) {
                int32_t message_len = static_cast<int32_t>(strlen(param_1) + 1);
                NETMOX::Mox_Send_Message_(MOX::_player[player_index].network_player_id, 0x27, param_1, (uint16_t)message_len);
                RUSS::Mox_Update_();
            }
        }
    }

    void __cdecl Send_GNN_Chat_Msg_(char* msg) {
        if (MOX::_NUM_PLAYERS <= 0) {
            return;
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives == 100) {
                int len = (int)strlen(msg);

                if (len > 80) {
                    msg[80] = '\0';
                    len = 80;
                }

                // TYPE MISMATCH: Mox_Send_Message_ returns int32_t and takes uint16_t for msg_type and len
                NETMOX::Mox_Send_Message_(MOX::_player[i].network_player_id, (uint16_t)0x2D, msg, (uint16_t)(len + 1));
                RUSS::Mox_Update_();
            }
        }
    }
}