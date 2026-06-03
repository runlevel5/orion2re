#include "pch.h"

namespace comcore {

void* _sendBuffer;

void* _userReceiveBuffer;

s_comcore_poll_buf _pollReq;

s_comcore_open_req _openReq;

uint16_t _recBufferLength;

s_comcore_req_buf _sendReq;

uint32_t _commsInitTime3;

rec_fn _recFunc;

uint32_t _commsInitTime2;

uint32_t _commsInitTime;

uint8_t* _pollBuffer;

int16_t __cdecl ComSendUserMessage_(
    void* msg_data,
    uint16_t msg_length,
    DPID player_id,
    uint16_t msg_type,
    uint8_t flags,
    uint32_t transport_flag
) {
    if (msg_type < 100) {
        return (int16_t)0xFF33;
    }

    if (msg_length > 350 || flags == 2) {
        return transprt::SendLargeUserMessage_(transport_flag, (uint8_t*)msg_data, msg_length, player_id);
    }

    uint32_t packet_header;
    if (msg_type == 0x7B1) {
        packet_header = 0x71;
    } else if (msg_type == 0x7BF) {
        packet_header = 0x61;
    } else {
        packet_header = 0x51;
    }
    uint8_t* send_buffer = (uint8_t*)_sendBuffer;
    memcpy(send_buffer, &packet_header, sizeof(packet_header));

    memcpy(send_buffer + sizeof(packet_header), msg_data, msg_length);

    DPID target_player_id;
    if (player_id == 0x7FFF || player_id == 0x7FFE) {
        ::OutputDebugStringA("Sending Message To All....\n");
        target_player_id = 0;
    } else {
        ::OutputDebugStringA("Sending Message To Individual....\n");
        target_player_id = player_id;
    }

    HRESULT hr = netcode::_dplay->Send(
        WINNET::_local_player_id,
        target_player_id,
        0,
        _sendBuffer,
        msg_length + sizeof(packet_header)
    );

    if (hr != 0) {
        WINNET::Exit_With_Error_(hr);
    }

    return 1;
}

int32_t __cdecl ComInit_(uint16_t msg_size, char* player_name, recv_handler_fn receive_handler) {
    transprt::InitTransportLayer_(0x8000);

    if (receive_handler == nullptr) {
        return -203;
    }

    transprt::_userRecFunc = receive_handler;
    _recFunc = (comcore::rec_fn)transprt::ReceiveLargeUserMessage_;
    _recBufferLength = msg_size + 10;

    _userReceiveBuffer = malloc(_recBufferLength);
    if (_userReceiveBuffer == nullptr) {
        return -202;
    }

    _sendBuffer = malloc(_recBufferLength);
    if (_sendBuffer == nullptr) {
        return -202;
    }

    _sendReq.buf_size = 0;
    _sendReq.send_buf = (uint8_t*)_sendBuffer;

    _pollBuffer = (uint8_t*)malloc(_recBufferLength);
    if (_pollBuffer == nullptr) {
        return -202;
    }

    _pollReq.send_buf = _pollBuffer;

    _commsInitTime = time_::_time();

    if (strlen(player_name) > 10) {
        player_name[10] = '\0';
    }

    strncpy(_openReq.name, player_name, strlen(player_name));

    _openReq.msg_size = msg_size;
    net::_thisHost = WINNET::_local_player_id;

    _commsInitTime2 = time_::_time();
    _commsInitTime3 = _commsInitTime2;

    return 1;
}

int16_t __cdecl ComDoComms_() {
    transprt::UpdateTransportLayer_();
    GetMessages_();
    return 1;
}

void __cdecl GetMessages_() {
    int16_t attempts = 100;
    errno = 0;

    while (true) {
        if (net::NetPoll_(&_pollReq) != 3) {
            break;
        }

        attempts--;
        uint8_t* poll_data = _pollReq.send_buf;

        if (!HandleSessionMessage_(poll_data)) {
            uint32_t packet_header;
            memcpy(&packet_header, poll_data, sizeof(packet_header));

            if (packet_header == 0x51 || packet_header == 0x61 || packet_header == 0x71) {
                HandleControlMessage_(&_pollReq);
            }

            if (packet_header == 0x41) {
                WINNET::SetPlayerLastKeepAlive_(&_pollReq);
            }
        }

        if (attempts == 0) {
            break;
        }
    }
}

int32_t __cdecl HandleSessionMessage_(uint8_t* msg_data) {
    uint32_t packet_header;
    memcpy(&packet_header, msg_data, sizeof(packet_header));

    if (packet_header == 3) {
        WINNET::Handle_Create_Player_((s_net_pack_create_player*)msg_data);
        return 1;
    }

    if (packet_header == 5) {
        WINNET::Handle_Destroy_Player_((s_net_pack_destroy_player*)msg_data);
        return 1;
    }

    return 0;
}

int8_t __cdecl HandleControlMessage_(s_comcore_poll_buf* poll_buf) {
    if (poll_buf->id_from == WINNET::_local_player_id) {
        return 1;
    }

    if (_recFunc != nullptr) {
        uint32_t packet_header;
        memcpy(&packet_header, poll_buf->send_buf, sizeof(packet_header));
        uint8_t* payload = poll_buf->send_buf + sizeof(packet_header);

        if (packet_header == 0x51) {
            _recFunc(0x7B2, poll_buf->len - sizeof(packet_header), payload);
        }
        if (packet_header == 0x71) {
            _recFunc(0x7B1, poll_buf->len - sizeof(packet_header), payload);
        }
        if (packet_header == 0x61) {
            _recFunc(0x7BF, poll_buf->len - sizeof(packet_header), payload);
        }
    }

    return 0;
}

} // namespace comcore