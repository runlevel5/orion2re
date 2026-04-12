#pragma once
#include <cstdint>

namespace transprt {
    inline uint32_t& _num_net_players = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00596200});

    inline int32_t& _tpSending =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00560A78});

    inline uint32_t& _tpRecNum =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A7C});

    inline uint32_t& _tpRecSrc =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A88});

    inline uint32_t& _tpLastSendSuccessful =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560A8C});
}