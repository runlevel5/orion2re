#pragma once

#include <cstdint>

namespace settings {
    inline uint8_t& language = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00597152});
}