#pragma once
#include <cstdint>

namespace transprt {
    inline uint32_t& _num_net_players = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00596200});
}