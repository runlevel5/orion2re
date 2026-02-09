#pragma once
#include <cstdint>

namespace ERICNET {
    inline void __cdecl Init_Homeworld_Colony_(int16_t planet_idx, int16_t owner_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00408970});
        fn(planet_idx, owner_idx);
    }

    inline void __cdecl Mox_Play_Sound_(int32_t sound_id) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00409230});
        fn(sound_id);
    }
}