#pragma once
#include <cstdint>

namespace INITSHIP {
    inline void __cdecl Init_Ship_Designs_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0047f0f0 })(player_idx);
    }
    inline int16_t __cdecl Best_Ship_Fuel_(int16_t player_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00481dd0 })(player_idx);
    }

    inline void __cdecl Update_Player_Ship_Designs_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00481e50});
        fn(player_idx);
    }
}