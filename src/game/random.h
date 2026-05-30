#pragma once
#include <cstdint>

namespace random {
    extern uint32_t _random_seed;
    extern uint32_t _saved_random_seed;

    uint32_t __cdecl Get_Random_Seed_();

    void __cdecl Save_Random_Seed_();

    uint32_t __cdecl Set_Random_Seed_(uint32_t seed);

    uint32_t __cdecl Randomize_();

    uint32_t __cdecl Random_(uint32_t max_val);

    void __cdecl Restore_Random_Seed_();
}