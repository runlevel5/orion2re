#pragma once
#include <cstdint>

namespace EVANHELP {
    inline uint8_t& _main_menu_screen_help_list = 
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552B48});
}