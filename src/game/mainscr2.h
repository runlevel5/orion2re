#pragma once
#include <cstdint>

namespace MAINSCR2 {
    inline uint8_t& _main_screen_fade_in_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596013});
    inline uint8_t& _force_main_screen_fade = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596014});
}