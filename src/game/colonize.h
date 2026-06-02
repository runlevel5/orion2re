#pragma once
#include <cstdint>

namespace COLONIZE {
	// Allocates free colony slots to players based on pending colonization opportunities.
	void Allocate_New_Colony_Slots_();
    // Runs AI colonization for every active AI player.
    void __cdecl All_AI_Colonize_();

	// Determines which player, if any, has colonization priority at each star.
	void Find_Colonize_Players_();

	// Counts planets at a star that the specified player can colonize.
	int16_t Star_N_Colonizable_Planets_For_Player_(s_star_data* star, int16_t player_idx);

	// Counts uncolonized planets at a star that are valid outpost targets.
	int16_t Star_N_Outpostable_Planets_(s_star_data* star);

	// Counts uncolonized planets present at a star.
	int16_t Star_N_Uncolonized_Planets_(s_star_data* star);

    // Performs colonization decisions for one AI player.
    void __cdecl AI_Colonize_(int16_t player_idx);

    // Creates a new colony for the specified player on the target planet.
    void __cdecl Make_New_Colony_(int16_t planet_idx, int16_t player_idx);
    // Creates either a colony or an outpost for the specified player on the target planet.
    void __cdecl Make_New_Colony_Or_Outpost_(int16_t planet_idx, int16_t player_idx, int16_t is_outpost);
    // Creates a new outpost for the specified player on the target planet.
    void __cdecl Make_New_Outpost_(int16_t planet_idx, int16_t player_idx);
}