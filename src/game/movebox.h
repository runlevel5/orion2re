#pragma once
#include <cstdint>

namespace MOVEBOX {
    inline bool __cdecl Moveable_Box_Selected_(int16_t index) {
        using Fn = bool(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00513B90})(index);
    }
}