#pragma once

struct s_colony;

namespace invasion {
    void __cdecl Compute_Player_Ground_Combat_Bonuses_(int player_idx, s_invasion_player_bonuses* bonuses, char is_defender);

    inline void __cdecl Check_Rebellion_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042c650});
        fn(colony);
    }

    inline void __cdecl Enforce_Population_Limits_At_Colony_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042bcb0});
        fn(colony);
    }

    void __cdecl Check_All_Rebellions_();
}