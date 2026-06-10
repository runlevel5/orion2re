#pragma once

#include "pch.h"

namespace comcore {
    using recv_handler_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    /**
     * Updates the transport layer and dispatches any newly received packets.
     */
    int16_t __cdecl ComDoComms_();

    /**
     * Polls the network layer until pending packets have been drained.
     */
    void __cdecl GetMessages_();

    /**
     * Handles session-management packets from the network layer.
     * @param msg_data Packet buffer whose first dword contains the session message code.
     */
    int32_t __cdecl HandleSessionMessage_(uint8_t* msg_data);

    /**
     * Dispatches a gameplay packet to the registered receive callback.
     * @param poll_buf Polled packet metadata and payload buffer.
     */
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

    /**
     * Sends a user packet directly or through the large-message transport path.
     * @param msg_data Payload buffer to send.
     * @param msg_length Payload size in bytes.
     * @param player_id Destination player ID, or a broadcast sentinel.
     * @param msg_type Application-defined message type.
     * @param flags Delivery flags that can force large-message handling.
     * @param transport_flag Transport flag forwarded to the large-message sender.
     */
    int16_t __cdecl ComSendUserMessage_(
        void* msg_data,
        uint16_t msg_length,
        uint32_t player_id,
        uint16_t msg_type,
        uint8_t flags,
        uint32_t transport_flag
    );

    /**
     * Initializes comcore buffers and installs the receive callback.
     * @param msg_size Maximum payload size expected for received messages.
     * @param player_name Local player name copied into the open request buffer.
     * @param receive_handler Callback used for received user messages.
     */
    int32_t __cdecl ComInit_(uint16_t msg_size, char* player_name, recv_handler_fn receive_handler);
}