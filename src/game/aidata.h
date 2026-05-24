#pragma once

namespace AIDATA {
    struct s_turns_from_to_intermediate_cache_entry {
        uint8_t turns;
        uint8_t intermediate_star;
        uint8_t extended_turns;
        uint8_t extended_intermediate_star;
    };
    static_assert(sizeof(s_turns_from_to_intermediate_cache_entry) == 4);

    struct s_ship_n_turns_to_intermediate_cache_entry {
        uint8_t turns;
        uint8_t intermediate_star;
    };
    static_assert(sizeof(s_ship_n_turns_to_intermediate_cache_entry) == 2);

    inline uint16_t(&_g_base_planet_values)[8][360] = *reinterpret_cast<uint16_t(*)[8][360]>(std::uintptr_t{ 0x0056ed28 });
    inline uint16_t(*&_ship_strengths)[500][8] = *reinterpret_cast<uint16_t(**)[500][8]>(std::uintptr_t{ 0x005953D4 });
    inline uint16_t(*&_ship_strengths_in_nebula)[500][8] = *reinterpret_cast<uint16_t(**)[500][8]>(std::uintptr_t{ 0x005953AC });
    inline uint16_t(*&_ship_bomb_strengths)[500][4] = *reinterpret_cast<uint16_t(**)[500][4]>(std::uintptr_t{ 0x005953D8 });
    inline int16_t(*&_planet_stage_weights)[360][8] = *reinterpret_cast<int16_t(**)[360][8]>(std::uintptr_t{ 0x0056ED20 });
    inline uint32_t(*&_base_star_stage_weights)[72][8] = *reinterpret_cast<uint32_t(**)[72][8]>(std::uintptr_t{ 0x0059539C });
    inline uint32_t(*&_star_stage_weights)[72][8] = *reinterpret_cast<uint32_t(**)[72][8]>(std::uintptr_t{ 0x005953E4 });
    inline uint8_t(*&_n_turns_from_to)[72][72][8] = *reinterpret_cast<uint8_t(**)[72][72][8]>(std::uintptr_t{ 0x005953D0 });
    inline s_turns_from_to_intermediate_cache_entry(*&_n_turns_from_to_intermediate)[72][72][8] =
        *reinterpret_cast<s_turns_from_to_intermediate_cache_entry(**)[72][72][8]>(std::uintptr_t{ 0x00571A28 });
    inline int8_t(*&_ship_n_turns_to)[500][72] = *reinterpret_cast<int8_t(**)[500][72]>(std::uintptr_t{ 0x005953A8 });
    inline s_ship_n_turns_to_intermediate_cache_entry(*&_ship_n_turns_to_intermediate)[500][72] =
        *reinterpret_cast<s_ship_n_turns_to_intermediate_cache_entry(**)[500][72]>(std::uintptr_t{ 0x00571A2C });
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
    inline int16_t(&_g_contextual_planet_values)[2880] =
        *reinterpret_cast<int16_t(*)[2880]>(std::uintptr_t{ 0x005703A8 });
    inline uint16_t (*&_contextual_planet_values)[2880] =
        *reinterpret_cast<uint16_t (**)[2880]>(std::uintptr_t{0x0055A6B8});
    inline uint16_t& _n_all_ships = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005953E0 });

    /*
     * Returns the player with the strongest population claim on a colony.
     * @param colony Colony to inspect.
     */
    int16_t __cdecl Colony_Dominant_Player_(s_colony* colony);

    /*
     * Updates per-star AI range summaries for each player.
     */
    void __cdecl Compute_Star_Player_Range_Info_();

    /*
     * Estimates how valuable a colony is to a specific player.
     * @param colony Colony being evaluated.
     * @param evaluating_player Player perspective used for the estimate.
     */
    int16_t __cdecl Colony_Worth_To_Player_(s_colony* colony, s_player* evaluating_player);

    /*
     * Estimates the strategic range value of a colony to a specific player.
     * @param colony Colony being evaluated.
     * @param evaluating_player Player perspective used for the estimate.
     */
    int16_t __cdecl Proximity_Worth_To_Player_(s_colony* colony, s_player* evaluating_player);

    /*
     * Computes the base per-player value table for all known planets.
     */
    void __cdecl Compute_Base_Planet_Values_();

    /*
     * Computes per-player planet values adjusted by same-system context.
     */
    void __cdecl Compute_Contextual_Planet_Values_();

    /*
     * Scores how worthwhile it is to refit a ship against current tech.
     * @param ship Ship to evaluate.
     */
    int32_t __cdecl Refit_Desirability_(s_ship_data* ship);

    /*
     * Returns cached travel turns between two stars and an optional intermediate stop.
     * @param player_idx Player whose travel rules are used.
     * @param source_star_idx Starting star.
     * @param dest_star_idx Destination star.
     * @param intermediate_star_idx_out Receives the chosen intermediate star, or -1.
     * @param use_extended_range Whether to use the extended-range result.
     */
    int16_t __cdecl Player_N_Turns_From_Star_To_Star_With_Intermediate_(
        int16_t player_idx, int16_t source_star_idx, int16_t dest_star_idx, int16_t* intermediate_star_idx_out, bool use_extended_range);

    /*
     * Returns cached travel turns from a ship to a destination star and an optional stop.
     * @param ship_idx Ship to evaluate.
     * @param dest_star_idx Destination star.
     * @param intermediate_star_idx_out Receives the chosen intermediate star, or -1.
     */
    int16_t __cdecl Ship_N_Turns_To_Star_With_Intermediate_(int16_t ship_idx, int16_t dest_star_idx, int16_t* intermediate_star_idx_out);

    /*
     * Compares refit entries by desirability for qsort.
     * @param lhs Left entry.
     * @param rhs Right entry.
     */
    int __cdecl airefit_cmp_(const void* lhs, const void* rhs);

    /*
     * Orders combat ships by owner and map position for qsort.
     * @param lhs Left ship index.
     * @param rhs Right ship index.
     */
    int __cdecl sort_ships_(const void* lhs, const void* rhs);

    /*
     * Releases the temporary AI data block allocated for turn analysis.
     */
    void __cdecl Deallocate_AI_Data_();

    /*
     * Checks whether a target star is reachable from a source star or its wormhole exit.
     * @param target_star_idx Star being tested.
     * @param source_star_idx Source star.
     * @param max_range_parsecs Maximum allowed distance in parsecs.
     */
    bool __cdecl Star_In_Range_Of_Given_Star_(int16_t target_star_idx, int16_t source_star_idx, int max_range_parsecs);

    /*
     * Builds AI caches for ships, colonies, stars, and per-player planning data.
     */
    void __cdecl Compute_AI_Data_();

    /*
     * Marks which players can reach a star within normal and extended range.
     * @param star_idx Star being tested.
     * @param in_normal_range_out Receives players within normal range.
     * @param in_extended_range_out Receives players within extended range.
     */
    void __cdecl Find_Players_In_Range_(int16_t star_idx, int8_t* in_normal_range_out, int8_t* in_extended_range_out);

    /*
     * Estimates how valuable settling an uncolonized planet is to a player.
     * @param planet Planet being evaluated.
     * @param evaluating_player Player perspective used for the estimate.
     */
    int32_t __cdecl Uncolonized_Planet_Worth_To_Player_(s_planet_data* planet, s_player* evaluating_player);
}