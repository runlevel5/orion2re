#include "pch.h"

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

    uint32_t __cdecl Random_(uint32_t max_val) {
        // Original assembly performed division before checking for zero, potentially causing a divide-by-zero trap.
        if (max_val == 0) {
            return 1;
        }

        uint32_t quotient = 0xFFFFFFFF / max_val;
        uint32_t limit = max_val * quotient;

        uint32_t seed = _random_seed;
        do {
            seed = seed * 0x41C64E6D + 0x3039;
        } while (limit <= seed);

        _random_seed = seed;
        return (seed / quotient) + 1;
    }

    void __cdecl Restore_Random_Seed_() {
        _random_seed = _saved_random_seed;
    }
}