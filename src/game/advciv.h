#pragma once
#include <cstdint>

namespace ADVCIV {
    inline int8_t*& _star_colony_owner =
        *reinterpret_cast<int8_t**>(std::uintptr_t{0x0056CD60});

    bool __cdecl Enemy_Has_Colony_At_Star_(int16_t player_idx, int16_t star_idx);

    void __cdecl Advanced_Civilization_Colonies_();

    void __cdecl Init_Advanced_Civilization_Fleet_(int16_t player_index);

    void __cdecl Assign_Advanced_Civilization_Starting_Ships_();

    int32_t __cdecl Uncolonized_Planet_Worth_To_Player_(s_planet_data* planet, s_player* plr);

    inline int8_t*& _planet_selected_flag = *reinterpret_cast<int8_t**>(std::uintptr_t{0x0056CD54});
    inline s_adv_civ_player*& _adv_civ_player = *reinterpret_cast<s_adv_civ_player**>(std::uintptr_t{0x0056CD58});

    inline s_parsec*& _parsec =
        *reinterpret_cast<s_parsec**>(std::uintptr_t{0x0056CD5C});

    inline s_adv_planet_worth*& _planet_worth_to_player =
        *reinterpret_cast<s_adv_planet_worth**>(std::uintptr_t{0x0056CD50});

    void __cdecl Build_Parsec_Star_Distances_(s_parsec* parsec);

    void __cdecl Choose_Adv_Civ_Planets_();

    int16_t Get_AdvCiv_Planet_Picks_Cutoff();

    void __cdecl Build_Adv_Civ_Star_List_();

    void __cdecl Build_Adv_Civ_Planet_List_();

    int16_t __cdecl Get_Next_Adv_Civ_Planet_(int16_t player_idx, int8_t* availability_map);

    void __cdecl Twiddle_Selected_Adv_Civ_Planets_();

    void __cdecl Init_Non_Homeworld_Colonies_();

    int8_t __cdecl Twiddle_Planet_(int16_t planet_idx, int8_t* weight_choices);

    int __cdecl Get_Players_Planet_Worthiness_Average_(int16_t player_idx);

    int __cdecl qsort_planet_worth_(const void* a, const void* b);

    void __cdecl Get_Adv_Civ_Planet_Worthiness_(int16_t player_idx, int16_t planet_idx, int32_t* worth, int8_t* has_colony, int16_t* proximity_bonus);

    void __cdecl Get_Worth_For_All_Planets_();
}