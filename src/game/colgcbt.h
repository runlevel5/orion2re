#pragma once
#include <cstdint>

namespace COLGCBT {
    inline s_mouse_list &_mouse_list_blank = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005583A8});
}