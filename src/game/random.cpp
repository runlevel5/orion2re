#include "deps.h"

namespace random {
    uint32_t __cdecl Get_Random_Seed_() {
        return _random_seed;
    }

    void __cdecl Set_Random_Seed_(uint32_t seed) {
        _random_seed = seed;
    }

    void __cdecl Randomize_() {
        uint32_t current_time = _time(nullptr);
        Set_Random_Seed_(current_time);
    }
}