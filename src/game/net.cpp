#include "pch.h"

namespace net {
    uint32_t _thisHost;

    int32_t __cdecl NetPoll_(s_comcore_poll_buf* poll_buf) {
        HRESULT receive_result;
        uint32_t id_from;
        uint32_t id_to;
        uint32_t buffer_length = comcore::_recBufferLength;

        receive_result = netcode::_dplay->Receive(
            (LPDPID)&id_from,
            (LPDPID)&id_to,
            9,
            poll_buf->send_buf,
            (LPDWORD)&buffer_length);

        if (receive_result != 0) {
            if (receive_result != static_cast<HRESULT>(0x887700BEu)) {
                WINNET::Exit_With_DPlay_Error_(receive_result);
            }
            return 0;
        }

        PTR_OutputDebugStringA_005aa3c0(s_Receiving_Message_005609F0);

        receive_result = netcode::_dplay->Receive(
            (LPDPID)&id_from,
            (LPDPID)&id_to,
            1,
            poll_buf->send_buf,
            (LPDWORD)&buffer_length);

        if (receive_result == 0) {
            poll_buf->id_from = id_from;
            poll_buf->id_to = id_to;
            poll_buf->len = buffer_length;
            return 3;
        }

        WINNET::Exit_With_DPlay_Error_(receive_result);
        return 0;
    }
}