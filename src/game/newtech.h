#pragma once
#include <cstdint>

namespace newtech {
    inline int16_t __cdecl Choose_Tech_Application_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004BBD20});
        return fn(player_idx);
    }
}