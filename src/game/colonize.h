#pragma once
#include <cstdint>

namespace COLONIZE {
	void Allocate_New_Colony_Slots_();

	void Find_Colonize_Players_();

	int16_t Star_N_Colonizable_Planets_For_Player_(s_star_data* star, int16_t player_idx);

	int16_t Star_N_Outpostable_Planets_For_Player_(s_star_data* star, int16_t player_idx);

	int16_t Star_N_Uncolonized_Planets_(s_star_data* star);
}