#pragma once
#include <cstdint>

struct s_comcore_poll_buf;

namespace net {
    extern uint32_t _thisHost;

    /* Polls DirectPlay for an incoming message and fills the provided poll buffer on success.
     * @param poll_buf Output buffer that receives the message payload, source/target IDs, and byte count.
     */
    int32_t __cdecl NetPoll_(s_comcore_poll_buf* poll_buf);
}