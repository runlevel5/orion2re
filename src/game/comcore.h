#pragma once

#include "pch.h"

namespace comcore {
    using recv_handler_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    int16_t __cdecl ComDoComms_();

    void __cdecl GetMessages_();

    int32_t __cdecl HandleSessionMessage_(uint8_t* msg_data);

    int8_t __cdecl HandleControlMessage_(s_comcore_poll_buf* poll_buf);

    inline void*& _sendBuffer = *reinterpret_cast<void**>(std::uintptr_t{0x00591F28});

    inline void*& _userReceiveBuffer =
        *reinterpret_cast<void**>(std::uintptr_t{0x00591ED0});

    inline s_comcore_poll_buf& _pollReq =
        *reinterpret_cast<s_comcore_poll_buf*>(std::uintptr_t{0x00591ED8});

    inline s_comcore_open_req& _openReq =
        *reinterpret_cast<s_comcore_open_req*>(std::uintptr_t{0x00591EE8});

    inline uint16_t& _recBufferLength =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00591F00});

    inline s_comcore_req_buf& _sendReq =
        *reinterpret_cast<s_comcore_req_buf*>(std::uintptr_t{0x00591F08});

    inline uint32_t& _commsInitTime3 =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00591F18});

    using rec_fn = void(__cdecl*)(uint32_t, uint32_t, uint8_t*);
    inline rec_fn& _recFunc =
        *reinterpret_cast<rec_fn*>(std::uintptr_t{0x00591F1C});

    inline uint32_t& _commsInitTime2 =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00591F38});

    inline uint32_t& _commsInitTime =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00591F3C});

    inline uint8_t*& _pollBuffer =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00591F48});

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