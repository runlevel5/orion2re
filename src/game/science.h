#pragma once
#include <cstdint>

namespace SCIENCE {
    // Suggested name: Show_Off_Stolen_Tech_
    inline void __cdecl Show_Off_Stolen_Tech_(int16_t tech_app_id, int16_t victim_player_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00410AF0});
        fn(tech_app_id, victim_player_idx);
    }
}