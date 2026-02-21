#pragma once
#include <cstdint>

namespace net {
    inline uint8_t& state = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005952EB});
}