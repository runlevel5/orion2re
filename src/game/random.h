#pragma once
#include <cstdint>

namespace random {
    inline uint32_t& _random_seed = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0057B6F0});
    inline uint32_t& _saved_random_seed = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0057B6FC});

    uint32_t __cdecl Get_Random_Seed_();

    void __cdecl Save_Random_Seed_();

    void __cdecl Set_Random_Seed_(uint32_t seed);

    void __cdecl Randomize_();

    uint32_t __cdecl Random_(uint32_t max_val);

    void __cdecl Restore_Random_Seed_();
}