#include "pch.h"

namespace transprt {

    uint32_t _num_net_players;

    int32_t _tpSending;

    uint32_t _tpRecNum;

    uint8_t _tpMessageIsProcessed;

    uint32_t _tpSendImmediateBurst;

    uint32_t _tpRecSrc;

    int32_t _tpLastSendSuccessful;

    int32_t _tpInitialized;

    uint8_t* _tpFullSendBuffer;

    uint8_t* _tpRecPacketBuffer;

    uint8_t* _tpRecUserBuffer;

    uint16_t _tpBufferSize;

    uint8_t* _tpMessageData;

    uint16_t _tpSendDataLength;

    uint8_t _tpForceBurstSend;

    uint32_t _tpDestID;

    uint32_t _tpMessageStamp = 1;

    uint32_t DAT_00560a70;

    uint16_t _tpCurrentUserType;

    int32_t _tpTotalPackets;

    int32_t _tpBurstNum;

    uint32_t _tpLastBurstTickCount;

    int32_t _tpLastBurstStartPacket;

    clock_t _tpLastBurstClock;

    uint32_t DAT_00560a94;

    uint8_t _tpSendImmediateBurstFlag = 1;

    uint32_t _queuedSenderCount;

    int32_t _tpBlockReceiving;

    uint32_t _queuedSenders[20];

    s_transport_packet _tpSendBuffer;

    uint8_t _tpAckFlags[128];

    s_tp_receive_state _tpReceiveState;

    uint32_t _tpOldMessages[128];

    user_rec_fn _userRecFunc;

    int8_t __cdecl InitTransportLayer_(uint16_t buffer_size) {
        if (buffer_size == 0) {
            return 0;
        }

        uint32_t buffer_size_bytes = buffer_size;

        _tpRecUserBuffer = (uint8_t*)malloc(buffer_size_bytes);
        _tpRecPacketBuffer = (uint8_t*)malloc(buffer_size_bytes);
        _tpFullSendBuffer = (uint8_t*)malloc(buffer_size_bytes);

        if (_tpRecUserBuffer != nullptr && 
            _tpRecPacketBuffer != nullptr && 
            _tpFullSendBuffer != nullptr) {
            
            _tpBufferSize = buffer_size;
            _tpInitialized = 1;
            
            memset(_tpOldMessages, 0, sizeof(_tpOldMessages));

            ResetSending_();
            ResetReceiving_();

            _tpLastBurstTickCount = (uint32_t)SDL_GetTicks();

            return 1;
        }

        return 0;
    }

    int32_t __cdecl SendLargeUserMessage_(uint16_t user_type, uint8_t* data, uint16_t data_len, uint32_t dest_id) {
        if (_tpInitialized == 0) {
            return -0xCC;
        }

        if (data_len > _tpBufferSize) {
            return -0xCC;
        }

        if (_tpSending != 0) {
            return -0xCC;
        }

        if (dest_id == net::_thisHost) {
            _tpLastSendSuccessful = 1;
            return 1;
        }

        if (NumUsers_() == 1) {
            _tpLastSendSuccessful = 1;
            return 1;
        }

        _tpMessageStamp++;

        if (data_len < 0x100) {
            memcpy(_tpFullSendBuffer, data, data_len);
            _tpMessageIsProcessed = 0;
            _tpMessageData = _tpFullSendBuffer;
        } else {
            uint32_t comp_len = 0;
            arith1e::Compress_Data_(data, data_len, _tpFullSendBuffer, &comp_len);
            data_len = (uint16_t)comp_len;
            _tpMessageIsProcessed = 1;
            _tpMessageData = _tpFullSendBuffer;
        }

        _tpSendDataLength = data_len;
        _tpSendImmediateBurst = 0;
        _tpDestID = dest_id;
        _tpBurstNum = 0;
        _tpSending = 1;
        _tpCurrentUserType = user_type;
        _tpTotalPackets =
            (data_len / sizeof(_tpSendBuffer.payload)) +
            ((data_len % sizeof(_tpSendBuffer.payload)) != 0 ? 1 : 0);

        UpdateTransportLayer_();

        return 1;
    }

    void __cdecl ResetSending_() {
        _tpSendDataLength = 0;
        _tpMessageData = nullptr;
        _tpDestID = 0;
        DAT_00560a70 = 0;
        _tpSending = 0;
        DAT_00560a94 = 0;
        _tpTotalPackets = 0;
        _tpBurstNum = 0;
        memset(_tpAckFlags, 0, sizeof(_tpAckFlags));
        _tpSendImmediateBurstFlag = 1;
    }

    void __cdecl ResetReceiving_() {
        _tpRecNum = 0;
        _tpRecSrc = 0;
        memset(&_tpReceiveState, 0, sizeof(_tpReceiveState));
    }

    int32_t __cdecl NumUsers_() {
        return WINNET::_player_count;
    }

    void __cdecl UpdateTransportLayer_() {
        if (_tpLastBurstStartPacket < _tpBurstNum || 
            _tpForceBurstSend == 1 || 
            _tpSendImmediateBurstFlag == 1 || 
            (uint32_t)(clock() - _tpLastBurstClock) >= 2000) {

            if (_tpSending != 0 && _tpBurstNum + 4 > _tpBurstNum) {
                for (int32_t packet_idx = _tpBurstNum; packet_idx < _tpBurstNum + 4; ++packet_idx) {
                    if (packet_idx >= _tpTotalPackets) {
                        break;
                    }
                    SendPacket_(packet_idx, _tpDestID, _tpCurrentUserType);
                }
            }

            _tpLastBurstTickCount = (uint32_t)SDL_GetTicks();
            _tpLastBurstClock = clock();
            _tpLastBurstStartPacket = _tpBurstNum;
            _tpSendImmediateBurstFlag = 0;
            _tpForceBurstSend = 0;
        }
    }

    void __cdecl SendPacket_(int32_t packet_index, uint32_t dest_id, uint16_t user_type) {
        const uint32_t payload_size_max = TRANSPORT_PAYLOAD_SIZE;
        uint16_t data_len = _tpSendDataLength;
        int32_t total_packets = (data_len / payload_size_max) + ((data_len % payload_size_max) != 0 ? 1 : 0);

        if (packet_index < total_packets && _tpSending != 0) {
            _tpSendBuffer.message_stamp = _tpMessageStamp;
            _tpSendBuffer.send_data_length = _tpSendDataLength;
            _tpSendBuffer.packet_index = (uint8_t)packet_index;
            _tpSendBuffer.total_packets = (uint8_t)total_packets;
            _tpSendBuffer.sender_host_id = (uint8_t)net::_thisHost;
            _tpSendBuffer.current_user_type = user_type;
            _tpSendBuffer.burst_index = (uint8_t)(packet_index / 4);
            _tpSendBuffer.message_is_processed = _tpMessageIsProcessed;
            _tpSendBuffer.send_immediate_burst = _tpSendImmediateBurst;

            uint32_t payload_size = payload_size_max;
            if (total_packets - packet_index == 1) {
                payload_size = _tpSendDataLength % payload_size_max;
                if (payload_size == 0) {
                    payload_size = payload_size_max;
                }
            }

            memcpy(_tpSendBuffer.payload, _tpMessageData + (packet_index * payload_size_max), payload_size);

            comcore::ComSendUserMessage_(
                &_tpSendBuffer,
                (uint16_t)(payload_size + (sizeof(_tpSendBuffer) - sizeof(_tpSendBuffer.payload))),
                dest_id,
                0x7B2,
                1,
                0
            );
        }
    }

    void __cdecl ReceiveLargeUserMessage_(uint16_t packet_type, uint32_t packet_size_bytes, s_transport_packet* packet) {
        if (_tpInitialized == 0) {
            return;
        }

        uint8_t sender = packet->sender_host_id;
        if (sender == net::_thisHost) {
            return;
        }

        if (packet_type != 0x7B2 && packet_type != 0x7BF && packet_type != 0x7B1) {
            return;
        }

        if (packet_type == 0x7B1) {
            _tpForceBurstSend = 1;
            return;
        }

        if (packet_type == 0x7BF) {
            if (packet->message_stamp != _tpMessageStamp) {
                return;
            }

            if (packet->burst_index != (uint32_t)(_tpBurstNum / 4)) {
                return;
            }

            _tpAckFlags[sender] = 1;
            if (!AllAcknowledged_()) {
                return;
            }

            _tpBurstNum += 4;
            if (_tpBurstNum < _tpTotalPackets) {
                return;
            }

            _tpLastSendSuccessful = 1;
            ResetSending_();
            return;
        }

        if (_tpBlockReceiving != 0) {
            return;
        }

        if (packet->message_stamp <= _tpOldMessages[sender]) {
            SendAck_(sender, packet->message_stamp, packet->burst_index);
            return;
        }

        if (_tpRecSrc != 0 || _tpRecNum != 0) {
            if (_tpRecSrc != sender) {
                if (_queuedSenderCount >= 0x13) {
                    return;
                }

                bool found = false;
                for (uint32_t i = 0; i < _queuedSenderCount; ++i) {
                    if (_queuedSenders[i] == sender) {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    _queuedSenders[_queuedSenderCount++] = sender;
                }

                return;
            }
            if (_tpRecNum < packet->message_stamp) {
                ResetReceiving_();
                _tpRecSrc = sender;
                _tpRecNum = packet->message_stamp;
            }
        } else {
            ResetReceiving_();
            _tpRecSrc = sender;
            _tpRecNum = packet->message_stamp;
        }

        uint8_t packet_index = packet->packet_index;
        if (_tpReceiveState.packets_received[packet_index] == 1) {
            bool burst_complete = true;
            uint32_t burst_start = packet_index & ~3u;
            for (uint32_t i = burst_start; i < burst_start + 4 && i < (uint32_t)packet->total_packets; ++i) {
                if (_tpReceiveState.packets_received[i] == 0) {
                    burst_complete = false;
                }
            }
            if (burst_complete) {
                SendAck_(sender, packet->message_stamp, packet->burst_index);
                return;
            }
        } else {
            _tpReceiveState.packets_received[packet_index] = 1;
            bool burst_complete = true;
            uint32_t burst_start = packet_index & ~3u;
            for (uint32_t i = burst_start; i < burst_start + 4 && i < (uint32_t)packet->total_packets; ++i) {
                if (_tpReceiveState.packets_received[i] == 0) {
                    burst_complete = false;
                }
            }
            if (burst_complete) {
                SendAck_(sender, packet->message_stamp, packet->burst_index);
            }

            uint32_t payload_len =
                (packet_size_bytes & 0xFFFF) - (uint32_t)(sizeof(*packet) - sizeof(packet->payload));
            uint8_t* packet_dest = _tpRecPacketBuffer + (TRANSPORT_PAYLOAD_SIZE * packet_index);
            memcpy(packet_dest, packet->payload, payload_len);

            bool all_complete = true;
            for (uint32_t i = 0; i < (uint32_t)packet->total_packets; ++i) {
                if (_tpReceiveState.packets_received[i] == 0) {
                    all_complete = false;
                }
            }

            if (all_complete) {
                _tpBlockReceiving = 1;
                uint16_t len = packet->send_data_length;
                if (packet->message_is_processed == 1) {
                    uint32_t uncompressed_len;
                    arith1e::Uncompress_Data_(_tpRecPacketBuffer, len, _tpRecUserBuffer, &uncompressed_len);
                    packet->send_data_length = (uint16_t)uncompressed_len;
                } else {
                    memcpy(_tpRecUserBuffer, _tpRecPacketBuffer, len);
                }
                
                auto user_func = _userRecFunc;
                user_func(sender, packet->current_user_type, packet->send_data_length, _tpRecUserBuffer);

                ResetReceiving_();
                _tpBlockReceiving = 0;
                NotifyNextQueuedSender_();

                _tpOldMessages[sender] = packet->message_stamp;
            }
        }
    }

    int32_t __cdecl SendAck_(uint8_t dest_player, uint32_t message_stamp, uint8_t burst_index) {
        s_transport_pack_ack packet;
        packet.message_stamp = message_stamp;
        packet.host = (uint8_t)net::_thisHost;
        packet.burst_index = burst_index;

        return (int32_t)comcore::ComSendUserMessage_(&packet, sizeof(packet), dest_player, 0x7bf, 1, 0);
    }

    bool __cdecl AllAcknowledged_() {
        if (_tpDestID != 0x7FFE && _tpDestID != 0x7FFF) {
            return _tpAckFlags[_tpDestID] != 0;
        }

        bool all_acknowledged = true;
        for (uint32_t i = 0; i < WINNET::_player_count; ++i) {
            uint32_t player_id = WINNET::Get_Player_Network_Id_((int32_t)i);
            if (player_id != net::_thisHost) {
                if (all_acknowledged) {
                    all_acknowledged = true;
                    if (_tpAckFlags[player_id] != 0) {
                        continue;
                    }
                }
                all_acknowledged = false;
            }
        }

        return all_acknowledged;
    }

    void __cdecl NotifyNextQueuedSender_() {
        if (_queuedSenderCount != 0) {
            uint8_t msg[0x13] = {};
            msg[8] = (uint8_t)net::_thisHost;
            msg[11] = 0;

            // Send the wake-up message twice.
            comcore::ComSendUserMessage_(msg, sizeof(msg), _queuedSenders[0], 0x7B1, 1, 0);
            comcore::ComSendUserMessage_(msg, sizeof(msg), _queuedSenders[0], 0x7B1, 1, 0);

            memmove(&_queuedSenders[0], &_queuedSenders[1], sizeof(_queuedSenders) - sizeof(_queuedSenders[0]));

            _queuedSenderCount--;
        }
    }

}