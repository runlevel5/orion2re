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

    void __cdecl Make_New_Colony_(int16_t planet_idx, int16_t player_idx);
    void __cdecl Make_New_Colony_Or_Outpost_(short planet_idx, short player_idx, short is_outpost);
    void __cdecl Make_New_Outpost_(int16_t planet_idx, int16_t player_idx);
}