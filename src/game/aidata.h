#pragma once

namespace AIDATA {
    inline uint16_t(&_g_base_planet_values)[8][360] = *reinterpret_cast<uint16_t(*)[8][360]>(std::uintptr_t{ 0x0056ed28 });
    inline uint16_t(*&_ship_strengths)[500][8] = *reinterpret_cast<uint16_t(**)[500][8]>(std::uintptr_t{ 0x005953D4 });
    inline uint16_t(*&_ship_strengths_in_nebula)[500][8] = *reinterpret_cast<uint16_t(**)[500][8]>(std::uintptr_t{ 0x005953AC });
    inline int32_t(*&_ship_bomb_strengths)[500][2] = *reinterpret_cast<int32_t(**)[500][2]>(std::uintptr_t{ 0x005953D8 });
    inline int16_t(*&_planet_stage_weights)[360][8] = *reinterpret_cast<int16_t(**)[360][8]>(std::uintptr_t{ 0x0056ED20 });
    inline int32_t(*&_base_star_stage_weights)[72][8] = *reinterpret_cast<int32_t(**)[72][8]>(std::uintptr_t{ 0x0059539C });
    inline int32_t(*&_star_stage_weights)[72][8] = *reinterpret_cast<int32_t(**)[72][8]>(std::uintptr_t{ 0x005953E4 });
    inline uint8_t(*&_n_turns_from_to)[72][72][8] = *reinterpret_cast<uint8_t(**)[72][72][8]>(std::uintptr_t{ 0x005953D0 });
    inline int32_t(*&_n_turns_from_to_intermediate)[72][72][8] = *reinterpret_cast<int32_t(**)[72][72][8]>(std::uintptr_t{ 0x00571A28 });
    inline int8_t(*&_ship_n_turns_to)[500][72] = *reinterpret_cast<int8_t(**)[500][72]>(std::uintptr_t{ 0x005953A8 });
    inline int16_t(*&_ship_n_turns_to_intermediate)[500][72] = *reinterpret_cast<int16_t(**)[500][72]>(std::uintptr_t{ 0x00571A2C });
    inline int32_t(*&_possible_ship_strengths)[72][8][8][10] = *reinterpret_cast<int32_t(**)[72][8][8][10]>(std::uintptr_t{ 0x005953B0 });
    inline s_ai_colony_info(*&_ai_colony_info)[250] = *reinterpret_cast<s_ai_colony_info(**)[250]>(std::uintptr_t{ 0x005953E8 });
    inline s_ai_player_info(*&_ai_player_info)[8] = *reinterpret_cast<s_ai_player_info(**)[8]>(std::uintptr_t{ 0x005953B8 });
    inline s_ai_star_info(*&_ai_star_info)[72] = *reinterpret_cast<s_ai_star_info(**)[72]>(std::uintptr_t{ 0x005953C8 });
    inline uint8_t(*&_star_in_range_of_player)[72] = *reinterpret_cast<uint8_t(**)[72]>(std::uintptr_t{ 0x005953BC });
    inline uint8_t(*&_star_in_extended_range_of_player)[72] = *reinterpret_cast<uint8_t(**)[72]>(std::uintptr_t{ 0x005953EC });
    inline int32_t(*&_enroute_ship_strengths)[72][8][8][10] = *reinterpret_cast<int32_t(**)[72][8][8][10]>(std::uintptr_t{ 0x005953F0 });
    inline int8_t(*&_enroute_ships)[72][8][10] = *reinterpret_cast<int8_t(**)[72][8][10]>(std::uintptr_t{ 0x005953B4 });
    inline int8_t(*&_enroute_human_ships)[72][10] = *reinterpret_cast<int8_t(**)[72][10]>(std::uintptr_t{ 0x005953DC });
    inline int8_t(*&_enroute_nonstealthy_ships)[72][8] = *reinterpret_cast<int8_t(**)[72][8]>(std::uintptr_t{ 0x00595398 });
    inline int16_t(*&_all_ships)[500] = *reinterpret_cast<int16_t(**)[500]>(std::uintptr_t{ 0x005953C0 });
    inline int16_t(*&_i_all_combat_ships)[8][73] = *reinterpret_cast<int16_t(**)[8][73]>(std::uintptr_t{ 0x005953A0 });
    inline s_ai_tech_info*& _aitech = *reinterpret_cast<s_ai_tech_info**>(std::uintptr_t{ 0x005953CC });
    inline s_ai_refit_player(*&_airefit)[8] = *reinterpret_cast<s_ai_refit_player(**)[8]>(std::uintptr_t{ 0x005953A4 });
    inline int32_t(*&_ship_refit_desirabilities)[500] = *reinterpret_cast<int32_t(**)[500]>(std::uintptr_t{ 0x005953C4 });
    inline uint16_t(*&_base_planet_values)[8][360] = *reinterpret_cast<uint16_t(**)[8][360]>(std::uintptr_t{ 0x0055A6B4 });
    inline uint16_t(&_g_contextual_planet_values)[2880] =
        *reinterpret_cast<uint16_t(*)[2880]>(std::uintptr_t{ 0x0056ed28 });
    inline uint16_t (*&_contextual_planet_values)[2880] =
        *reinterpret_cast<uint16_t (**)[2880]>(std::uintptr_t{0x0055A6B8});
    inline uint16_t& _n_all_ships = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005953E0 });

    int16_t __cdecl Colony_Dominant_Player_(s_colony* colony);

    void __cdecl Compute_Star_Player_Range_Info_();

    int16_t __cdecl Colony_Worth_To_Player_(s_colony* colony, s_player* target_player);

    int16_t __cdecl Proximity_Worth_To_Player_(s_colony* colony, s_player* player);

    void __cdecl Compute_Base_Planet_Values_();

    void __cdecl Compute_Contextual_Planet_Values_();

    int32_t __cdecl Refit_Desirability_(s_ship_data* ship);

    short __cdecl Player_N_Turns_From_Star_To_Star_With_Intermediate_(int16_t player, int16_t star_a, int16_t star_b, int16_t* inter_star_out, bool use_extended);

    short __cdecl Ship_N_Turns_To_Star_With_Intermediate_(int16_t ship_idx, int16_t star_idx, int16_t* inter_star_out);

    int __cdecl airefit_cmp_(const void* a, const void* b);

    int __cdecl sort_ships_(const void* a, const void* b);

    void __cdecl Deallocate_AI_Data_();

    bool __cdecl Star_In_Range_Of_Given_Star_(int16_t target_star_idx, int16_t source_star_idx, int max_range);
    void __cdecl Compute_AI_Data_();
    void __cdecl Find_Players_In_Range_(int16_t star_idx, int8_t* in_range_900, int8_t* in_range_2025);

    int32_t __cdecl Uncolonized_Planet_Worth_To_Player_(s_planet_data* planet, s_player* plr);
}