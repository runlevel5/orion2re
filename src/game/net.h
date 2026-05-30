#pragma once
#include <cstdint>

namespace net {
    extern uint32_t _thisHost;

    int32_t __cdecl NetPoll_(s_comcore_poll_buf* req);
}