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
    uint32_t transprt_flag
) {
    if (msg_type < 100) {
        return static_cast<int16_t>(0xFF33);
    }

    if (msg_length > 350 || flags == 2) {
        return transprt::SendLargeUserMessage_(transprt_flag, (uint8_t*)msg_data, msg_length, player_id);
    }

    uint32_t header_val;
    if (msg_type == 0x7B1) {
        header_val = 0x71;
    } else if (msg_type == 0x7BF) {
        header_val = 0x61;
    } else {
        header_val = 0x51;
    }
    *static_cast<uint32_t*>(_sendBuffer) = header_val;

    memcpy(static_cast<uint8_t*>(_sendBuffer) + 4, msg_data, msg_length);

    DPID idTo;
    if (player_id == 0x7FFF || player_id == 0x7FFE) {
        ::OutputDebugStringA(s_Sending_Message_To_All);
        idTo = 0;
    } else {
        ::OutputDebugStringA(s_Sending_Message_To_Individual);
        idTo = player_id;
    }

    HRESULT hr = netcode::_dplay->Send(
        WINNET::_local_player_id,
        idTo,
        0,
        _sendBuffer,
        msg_length + 4
    );

    if (hr != 0) {
        WINNET::Exit_With_Error_(hr);
    }

    return 1;
}

int32_t __cdecl ComInit_(uint16_t msg_size, char* name, recv_handler_fn rec_fn) {
    transprt::InitTransportLayer_(0x8000);

    if (rec_fn == nullptr) {
        return -203;
    }

    transprt::_userRecFunc = rec_fn;
    _recFunc = reinterpret_cast<comcore::rec_fn>(transprt::ReceiveLargeUserMessage_);
    _recBufferLength = msg_size + 10;

    _userReceiveBuffer = _malloc(_recBufferLength);
    if (_userReceiveBuffer == nullptr) {
        return -202;
    }

    _sendBuffer = _malloc(_recBufferLength);
    if (_sendBuffer == nullptr) {
        return -202;
    }

    _sendReq.buf_size = 0;
    _sendReq.send_buf = static_cast<uint8_t*>(_sendBuffer);

    _pollBuffer = static_cast<uint8_t*>(_malloc(_recBufferLength));
    if (_pollBuffer == nullptr) {
        return -202;
    }

    _pollReq.send_buf = _pollBuffer;

    _commsInitTime = time_::_time();

    if (strlen(name) > 10) {
        name[10] = '\0';
    }

    strncpy(_openReq.name, name, strlen(name));

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
    *__errno() = 0;

    while (true) {
        if (net::NetPoll_(&_pollReq) != 3) {
            break;
        }

        attempts--;
        uint8_t* buf = _pollReq.send_buf;

        if (!HandleSessionMessage_(buf)) {
            uint32_t msg_type = *reinterpret_cast<uint32_t*>(buf);

            if (msg_type == 0x51 || msg_type == 0x61 || msg_type == 0x71) {
                HandleControlMessage_(&_pollReq);
            }

            if (*reinterpret_cast<uint32_t*>(buf) == 0x41) {
                WINNET::SetPlayerLastKeepAlive_(&_pollReq);
            }
        }

        if (attempts == 0) {
            break;
        }
    }
}

int32_t __cdecl HandleSessionMessage_(uint8_t* msg_data) {
    uint32_t msg_type = *reinterpret_cast<uint32_t*>(msg_data);

    if (msg_type == 3) {
        WINNET::Handle_Create_Player_(reinterpret_cast<s_net_pack_create_player*>(msg_data));
        return 1;
    }

    if (msg_type == 5) {
        WINNET::Handle_Destroy_Player_(reinterpret_cast<s_net_pack_destroy_player*>(msg_data));
        return 1;
    }

    return 0;
}

int8_t __cdecl HandleControlMessage_(s_comcore_poll_buf* poll_buf) {
    if (poll_buf->id_from == WINNET::_local_player_id) {
        return 1;
    }

    if (_recFunc != nullptr) {
        uint32_t msg_type = *reinterpret_cast<uint32_t*>(poll_buf->send_buf);

        if (msg_type == 0x51) {
            _recFunc(0x7B2, poll_buf->len - 4, poll_buf->send_buf + 4);
        }
        if (msg_type == 0x71) {
            _recFunc(0x7B1, poll_buf->len - 4, poll_buf->send_buf + 4);
        }
        if (msg_type == 0x61) {
            _recFunc(0x7BF, poll_buf->len - 4, poll_buf->send_buf + 4);
        }
    }

    return 0;
}

} // namespace comcore