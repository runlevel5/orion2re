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

    extern uint32_t DAT_00560a70;

    extern uint16_t _tpCurrentUserType;

    extern int32_t _tpTotalPackets;

    extern int32_t _tpBurstNum;

    extern DWORD _tpLastBurstTickCount;

    extern int32_t _tpLastBurstStartPacket;

    extern clock_t _tpLastBurstClock;

    extern uint32_t DAT_00560a94;

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

    void __cdecl ResetSending_();

    void __cdecl ResetReceiving_();

    bool __cdecl AllAcknowledged_();

    int32_t __cdecl SendAck_(uint8_t dest_player, uint32_t message_stamp, uint8_t burst_index);

    void __cdecl NotifyNextQueuedSender_();

    int32_t __cdecl NumUsers_();

    int32_t __cdecl SendLargeUserMessage_(uint16_t type, uint8_t* data, uint16_t len, uint32_t dest);

    extern user_rec_fn _userRecFunc;

    int8_t __cdecl InitTransportLayer_(uint16_t buffer_size);

    void __cdecl ReceiveLargeUserMessage_(uint16_t packet_type, uint32_t packet_size, s_transport_packet* packet);

    void __cdecl UpdateTransportLayer_();

    void __cdecl SendPacket_(int32_t packet_index, uint32_t dest_id, uint16_t user_type);
}