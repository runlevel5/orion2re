#pragma once
#include <cstdint>

namespace COLONIZE {
	void Allocate_New_Colony_Slots_();
    void __cdecl All_AI_Colonize_();

	void Find_Colonize_Players_();

	int16_t Star_N_Colonizable_Planets_For_Player_(s_star_data* star, int16_t player_idx);

	int16_t Star_N_Outpostable_Planets_For_Player_(s_star_data* star, int16_t player_idx);

	int16_t Star_N_Uncolonized_Planets_(s_star_data* star);

    void __cdecl AI_Colonize_(int16_t player_idx);

    inline void __cdecl Make_New_Colony_Or_Outpost_(int16_t planet_idx, int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b9820});
        fn(planet_idx, player_idx);
    }
}