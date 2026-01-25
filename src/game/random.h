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

    inline int16_t __cdecl Random_(int32_t max_val) {
        using Fn = int16_t(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D470 });
        return fn(max_val);
    }
}