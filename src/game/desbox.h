#pragma once
#include <cstdint>

namespace DESBOX {
    inline uint8_t& _incr_value = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A61E0});
}