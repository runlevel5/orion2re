#pragma once

#include <cstdint>

namespace alloc {
    inline uint32_t& _special_names_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00595390});
}