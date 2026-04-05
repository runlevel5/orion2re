#pragma once

#include <cstdint>

namespace settings {
    inline int8_t& language = *reinterpret_cast<int8_t*>(std::uintptr_t{0x00597152});
}