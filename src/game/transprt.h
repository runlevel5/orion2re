#pragma once

namespace transprt {
    using user_rec_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    inline uint32_t& _num_net_players = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00596200});

    inline int32_t& _tpSending =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00560A78});

    inline uint32_t& _tpRecNum =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A7C});

    inline uint32_t& _tpMessageIsProcessed = 
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A80});

    inline uint32_t& _tpSendImmediateBurst = 
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A84});

    inline uint32_t& _tpRecSrc =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A88});

    inline int32_t& _tpLastSendSuccessful =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00560A8C});

    inline int32_t& _tpInitialized = 
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00560A4C});

    inline uint8_t*& _tpFullSendBuffer = 
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00560A54});

    inline uint8_t*& _tpRecPacketBuffer = 
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00560A58});

    inline uint8_t*& _tpRecUserBuffer = 
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00560A5C});

    inline uint16_t& _tpBufferSize = 
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00560A60});

    inline uint8_t*& _tpMessageData = 
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00560A64});

    inline uint16_t& _tpSendDataLength = 
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00560A68});

    inline uint8_t& _tpForceBurstSend = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00560A48});

    inline uint32_t& _tpDestID = 
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A6C});

    inline uint32_t& _tpMessageStamp = 
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A74});

    inline uint32_t& DAT_00560a70 = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A70});

    inline uint16_t& _tpCurrentUserType = 
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00560A90});

    inline int32_t& _tpTotalPackets = 
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00560A98});

    inline int32_t& _tpBurstNum = 
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00560A9C});

    inline DWORD& _tpLastBurstTickCount = *reinterpret_cast<DWORD*>(std::uintptr_t{0x00560AA0});

    inline int32_t& _tpLastBurstStartPacket = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00560AAC});

    inline clock_t& _tpLastBurstClock = *reinterpret_cast<clock_t*>(std::uintptr_t{0x00592588});

    inline uint32_t& DAT_00560a94 = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A94});

    inline uint8_t& _tpSendImmediateBurstFlag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00560AA8});

    inline uint32_t& _queuedSenderCount = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A44});
    inline int32_t& _tpBlockReceiving = *reinterpret_cast<int32_t*>(std::uintptr_t{0x00560AA4});
    inline uint32_t (&_queuedSenders)[20] = *reinterpret_cast<uint32_t(*)[20]>(std::uintptr_t{0x00591F50});

    inline s_transport_packet& _tpSendBuffer =
        *reinterpret_cast<s_transport_packet*>(std::uintptr_t{0x00592428});

    inline uint8_t (&_tpAckFlags)[128] =
        *reinterpret_cast<uint8_t(*)[128]>(std::uintptr_t{0x005923A8});

    struct s_tp_receive_state {
        uint8_t packets_received[256];
        uint8_t unknown_100[768];
    };

    inline s_tp_receive_state& _tpReceiveState =
        *reinterpret_cast<s_tp_receive_state*>(std::uintptr_t{0x00591FA0});

    inline uint32_t (&_tpOldMessages)[128] = 
        *reinterpret_cast<uint32_t(*)[128]>(std::uintptr_t{0x00592590});

    void __cdecl ResetSending_();

    void __cdecl ResetReceiving_();

    bool __cdecl AllAcknowledged_();

    int32_t __cdecl SendAck_(uint8_t dest_player, uint32_t message_stamp, uint8_t burst_index);

    void __cdecl NotifyNextQueuedSender_();

    int32_t __cdecl NumUsers_();

    int32_t __cdecl SendLargeUserMessage_(uint16_t type, uint8_t* data, uint16_t len, uint32_t dest);

    inline user_rec_fn& _userRecFunc =
        *reinterpret_cast<user_rec_fn*>(std::uintptr_t{0x005923A0});

    int8_t __cdecl InitTransportLayer_(uint16_t buffer_size);

    void __cdecl ReceiveLargeUserMessage_(uint16_t packet_type, uint32_t packet_size, s_transport_packet* packet);

    void __cdecl UpdateTransportLayer_();

    void __cdecl SendPacket_(int32_t packet_index, uint32_t dest_id, uint16_t user_type);
}