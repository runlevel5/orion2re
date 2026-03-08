#pragma once

struct s_colony;

namespace invasion {
    inline int __cdecl Unload_Transports_(s_colony* col, int16_t* ship_array, int count) {
        using Fn = int(__cdecl*)(s_colony*, int16_t*, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0042cdc0 });
        return fn(col, ship_array, count);
    }

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

    inline void __cdecl Assign_New_Home_Planet_(int8_t player_idx, int16_t colony_idx) {
        using Fn = void(__cdecl*)(int8_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0042bf20});
        fn(player_idx, colony_idx);
    }
}