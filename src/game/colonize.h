#pragma once
#include <cstdint>

namespace COLONIZE {
	void Allocate_New_Colony_Slots_();
    void __cdecl All_AI_Colonize_();

	void Find_Colonize_Players_();

	int16_t Star_N_Colonizable_Planets_For_Player_(s_star_data* star, int16_t player_idx);

	int16_t Star_N_Outpostable_Planets_For_Player_(s_star_data* star, int16_t player_idx);

	int16_t Star_N_Uncolonized_Planets_(s_star_data* star);

	inline void __cdecl All_AI_Colonize_() {
		using Fn = void(__cdecl*)();
		auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004ba480 });
		fn();
	}
}