#pragma once
#include <cstdint>

namespace COLONIZE {
void Allocate_New_Colony_Slots_();

void Find_Colonize_Players_();

inline int16_t Star_N_Colonizable_Planets_For_Player_(s_star_data* star, int16_t player) {
    using Fn = int16_t(__cdecl*)(s_star_data*, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b9ae0});
    return fn(star, player);
}

inline int16_t Star_N_Outpostable_Planets_For_Player_(s_star_data* star, int16_t player) {
    using Fn = int16_t(__cdecl*)(s_star_data*, int16_t);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004b9b60});
    return fn(star, player);
}

inline int16_t Star_N_Uncolonized_Planets_(s_star_data* star) {
    using Fn = int16_t(__cdecl*)(s_star_data*);
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004b9aa0 });
    return fn(star);
}
}