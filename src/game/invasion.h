#pragma once

namespace invasion {
    void __cdecl Compute_Player_Ground_Combat_Bonuses_(int player_idx, s_invasion_player_bonuses* bonuses, char is_defender);

    inline void __cdecl Check_All_Rebellions_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0042c5f0 });
        fn();
    }
}