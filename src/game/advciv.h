#pragma once
#include <cstdint>

namespace ADVCIV {
    inline int8_t*& _star_colony_owner =
        *reinterpret_cast<int8_t**>(std::uintptr_t{0x0056CD60});

    bool __cdecl Not_Star_Owner_(int16_t planet_idx, int16_t star_idx);

    inline void __cdecl Advanced_Civilization_Colonies_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004AB460})();
    }

    void __cdecl Init_Advanced_Civilization_Fleet_(int16_t player_index);

    void __cdecl Assign_Advanced_Civilization_Starting_Ships_();

    int32_t __cdecl Uncolonized_Planet_Worth_To_Player_(s_planet_data* planet, s_player* plr);
}