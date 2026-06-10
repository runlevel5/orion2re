#include "pch.h"

namespace net {
    uint32_t _thisHost;

    int32_t __cdecl NetPoll_(s_comcore_poll_buf* poll_buf) {
        return WINNET::Poll_Message_(poll_buf);
    }
}