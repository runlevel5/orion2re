#pragma once
#include <cstdint>

namespace ERICNET {
    inline void __cdecl Init_Homeworld_Colony_(int16_t planet_idx, int16_t owner_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00408970});
        fn(planet_idx, owner_idx);
    }

    inline int32_t __cdecl Mox_Play_Sound_(int32_t sound_id) {
        using Fn = int32_t(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00409230});
        return fn(sound_id);
    }

    void __cdecl Init_Non_Homeworld_Colony_(int16_t colony_idx, int16_t player_idx);
}