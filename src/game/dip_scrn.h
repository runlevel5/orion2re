#pragma once
#include <cstdint>

namespace DIP_SCRN {
    inline uint16_t& _diplomacy_hand_mouse = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005569A8});
}