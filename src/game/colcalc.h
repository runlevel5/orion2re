#pragma once
#include <cstdint>

struct s_player;
struct s_colony;
struct s_star_data;

namespace COLCALC {
    void __cdecl Twiddle_Initial_Homeworlds_();

    inline void __cdecl Twiddle_Initial_Homeworld_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004fc4b0 });
        fn(colony);
    }

    void __cdecl Compute_All_Star_Colony_Stuff_();
    void __cdecl Compute_Star_Colony_Stuff_(s_star_data* star);

    void __cdecl Init_Surrenders_();

    inline void __cdecl No_Op1_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004FBEF0})();
    }

    inline int32_t __cdecl Player_Research_Cost_(s_player* plr, uint16_t field_idx) {
        using Fn = int32_t(__cdecl*)(s_player*, uint16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004f73e0})(plr, field_idx);
    }

    inline int32_t __cdecl Chance_For_Research_Breakthrough_Aux_(int accumulated, int produced, int cost) {
        using Fn = int32_t(__cdecl*)(int, int, int);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004f7470})(accumulated, produced, cost);
    }

    int16_t __cdecl Player_N_Turns_Until_Research_Complete_(s_player* plr);

    int32_t __cdecl Chance_For_Research_Breakthrough_(s_player* plr);
}