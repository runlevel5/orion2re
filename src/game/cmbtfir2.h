#pragma once

#include <cstdint>

namespace CMBTFIR2 {
    int16_t __cdecl Helmsman_Bonus_(int16_t leader_idx);

    int16_t __cdecl Weaponry_Bonus_(int16_t leader_idx);

    inline int8_t __cdecl Apply_Combat_Artemis_Net_(int8_t flag) {
        using Fn = int8_t(__cdecl*)(int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004e88b0});
        return fn(flag);
    }
}