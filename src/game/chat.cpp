#include "pch.h"

namespace CHAT {
    void __cdecl Receive_Chat_Msg_(uint8_t player_id, void* pack) {
        const char* text = (const char*)pack;

        if (_chat_info->count == 14) {
            memcpy(_chat_info->messages, &_chat_info->messages[1], 1066);
            _chat_info->count = 13;
        }

        _chat_info->messages[_chat_info->count].player_id = player_id;
        strcpy(_chat_info->messages[_chat_info->count].text, text);
        _chat_info->count++;
    }
}