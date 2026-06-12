#pragma once

namespace transprt {
    using user_rec_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    extern uint32_t _num_net_players;

    extern int32_t _tpSending;

    extern uint32_t _tpRecNum;

    extern uint8_t _tpMessageIsProcessed;

    extern uint32_t _tpSendImmediateBurst;

    extern uint32_t _tpRecSrc;

    extern int32_t _tpLastSendSuccessful;

    extern int32_t _tpInitialized;

    extern uint8_t* _tpFullSendBuffer;

    extern uint8_t* _tpRecPacketBuffer;

    extern uint8_t* _tpRecUserBuffer;

    extern uint16_t _tpBufferSize;

    extern uint8_t* _tpMessageData;

    extern uint16_t _tpSendDataLength;

    extern uint8_t _tpForceBurstSend;

    extern uint32_t _tpDestID;

    extern uint32_t _tpMessageStamp;

    extern uint32_t _tpCurrentSendOffset;

    extern uint16_t _tpCurrentUserType;

    extern int32_t _tpTotalPackets;

    extern int32_t _tpBurstNum;

    extern uint32_t _tpLastBurstTickCount;

    extern int32_t _tpLastBurstStartPacket;

    extern clock_t _tpLastBurstClock;

    extern uint32_t _tpLastAckedBurstStartPacket;

    extern uint8_t _tpSendImmediateBurstFlag;

    extern uint32_t _queuedSenderCount;
    extern int32_t _tpBlockReceiving;
    extern uint32_t _queuedSenders[20];

    extern s_transport_packet _tpSendBuffer;

    extern uint8_t _tpAckFlags[128];

    struct s_tp_receive_state {
        uint8_t packets_received[256];
        uint8_t unknown_100[768];
    };

    extern s_tp_receive_state _tpReceiveState;

    extern uint32_t _tpOldMessages[128];

    /** Reset the current outbound transport state. */
    void __cdecl ResetSending_();

    /** Reset the current inbound transport state. */
    void __cdecl ResetReceiving_();

    /** Return whether the current burst has been acknowledged by every required recipient. */
    bool __cdecl AllAcknowledged_();

    /**
     * Send an acknowledgement for a received burst.
     * @param dest_player Destination player id.
     * @param message_stamp Message stamp being acknowledged.
     * @param burst_index Burst index being acknowledged.
     */
    int32_t __cdecl SendAck_(uint8_t dest_player, uint32_t message_stamp, uint8_t burst_index);

    /** Notify the next queued sender that this receiver is ready for another message. */
    void __cdecl NotifyNextQueuedSender_();

    /** Return the current number of active network users. */
    int32_t __cdecl NumUsers_();

    /**
     * Queue a large user message for chunked transport delivery.
     * @param user_type Application-defined message type.
     * @param data Message payload buffer.
     * @param data_len Payload length in bytes.
     * @param dest_id Destination network id.
     */
    int32_t __cdecl SendLargeUserMessage_(uint16_t user_type, uint8_t* data, uint16_t data_len, uint32_t dest_id);

    extern user_rec_fn _userRecFunc;

    /**
     * Allocate and initialize transport-layer buffers.
     * @param buffer_size Size of each transport buffer in bytes.
     */
    int8_t __cdecl InitTransportLayer_(uint16_t buffer_size);

    /**
     * Process an incoming transport-layer packet or acknowledgement.
     * @param packet_type Transport packet opcode.
     * @param packet_size_bytes Received packet size in bytes.
     * @param packet Received packet data.
     */
    void __cdecl ReceiveLargeUserMessage_(uint16_t packet_type, uint32_t packet_size_bytes, s_transport_packet* packet);

    /** Transmit the next burst of packets when the sender state allows it. */
    void __cdecl UpdateTransportLayer_();

    /**
     * Build and send one packet from the current outbound message.
     * @param packet_index Zero-based packet index within the message.
     * @param dest_id Destination network id.
     * @param user_type Application-defined message type.
     */
    void __cdecl SendPacket_(int32_t packet_index, uint32_t dest_id, uint16_t user_type);
}