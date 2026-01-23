#pragma once
#include <cstdint>

namespace random {
    inline uint32_t __cdecl Get_Random_Seed_() {
        using Fn = uint32_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x0052D4D0})();
    }

    inline void __cdecl Set_Random_Seed_(uint32_t seed) {
        using Fn = void(__cdecl*)(uint32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0052D4C0})(seed);
    }
}