#pragma once

#include "pch.h"

namespace comcore {
    using recv_handler_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);
    extern const char s_Sending_Message_To_Individual[35];
    extern const char s_Sending_Message_To_All[28];

    int16_t __cdecl ComDoComms_();

    void __cdecl GetMessages_();

    int32_t __cdecl HandleSessionMessage_(uint8_t* msg_data);

    int8_t __cdecl HandleControlMessage_(s_comcore_poll_buf* poll_buf);

    extern void* _sendBuffer;

    extern void* _userReceiveBuffer;

    extern s_comcore_poll_buf _pollReq;

    extern s_comcore_open_req _openReq;

    extern uint16_t _recBufferLength;

    extern s_comcore_req_buf _sendReq;

    extern uint32_t _commsInitTime3;

    using rec_fn = void(__cdecl*)(uint32_t, uint32_t, uint8_t*);
    extern rec_fn _recFunc;

    extern uint32_t _commsInitTime2;

    extern uint32_t _commsInitTime;

    extern uint8_t* _pollBuffer;

    int16_t __cdecl ComSendUserMessage_(
        void* msg_data,
        uint16_t msg_length,
        DPID player_id,
        uint16_t msg_type,
        uint8_t flags,
        uint32_t transprt_flag
    );

    int32_t __cdecl ComInit_(uint16_t msg_size, char* name, recv_handler_fn rec_fn);
}