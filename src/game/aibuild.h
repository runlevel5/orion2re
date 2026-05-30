#pragma once
#include <cstdint>

namespace AIBUILD {
    extern uint8_t _difficulty_ship_design[12];
    extern int8_t _difficulty_intel_bias[8];
    extern uint8_t _player_ship_size_table[9];
    extern int _player_ship_sizes[8][14];
    extern int _player_total_strength_vs_colonies[8];
    extern uint8_t _player_n_transports[8][2];
    extern int8_t _player_n_freighter_groups_wanted[8];
    extern uint8_t _player_n_spies[8][2];
    extern uint8_t _player_n_colony_ships[8][3];
    extern int _player_total_strength_vs_ships[8][3];

    /** Returns the refit cost for replacing one ship design with another.
     * @param player_idx Player paying for the refit.
     * @param old_design Existing ship design.
     * @param new_design Replacement ship design.
     */
    int16_t __cdecl Refit_Cost_(int16_t player_idx, struct s_ship_design* old_design, struct s_ship_design* new_design);

    /** Returns an average strength estimate for a ship against relevant opponents.
     * @param ship Ship whose combat strength is being evaluated.
     */
    int16_t __cdecl Mean_Ship_Strength_(struct s_ship_data* ship);

    /** Computes build-related needs for one AI player.
     * @param player_idx Player index to evaluate.
     */
    void __cdecl Compute_Needs_(int16_t player_idx);

    /** Recomputes empire-wide build needs for all AI players. */
    void __cdecl Compute_Empire_Building_Needs_();

    /** Returns the highest hostile treaty state currently affecting a player.
     * @param player_idx Player index to inspect.
     */
    int16_t __cdecl Player_War_Level_(int16_t player_idx);

    /** Computes whether a player currently needs more colony ships.
     * @param player_idx Player index to evaluate.
     */
    void __cdecl Compute_Colony_Ship_Need_(int16_t player_idx);

    /** Computes how many freighter groups a player still needs.
     * @param player_idx Player index to evaluate.
     */
    void __cdecl Compute_Freighter_Need_(int16_t player_idx);

    /** Buys out eligible colony production for one player while funds allow.
     * @param player_idx Player index whose colonies may buy production outright.
     */
    void __cdecl Empire_Buy_Outright_(int16_t player_idx);

    /** Marks which buildings a colony is currently allowed to construct.
     * @param colony Colony to inspect.
     * @param buildable_flags Output flags indexed by product id.
     */
    void __cdecl Find_Buildable_Buildings_(struct s_colony* colony, uint16_t* buildable_flags);

    /** Scores one building choice for a colony's AI production logic.
     * @param colony Colony being evaluated.
     * @param building_id Building or product id to score.
     */
    short __cdecl Colony_Building_Score_(struct s_colony* colony, short building_id);

    /** Reassigns colony production when the current build should be replaced.
     * @param colony Colony whose production is being checked.
     */
    void __cdecl Assign_Colony_Building_(struct s_colony* colony);

    /** Chooses a new production target for a colony.
     * @param colony Colony whose production is being assigned.
     */
    void __cdecl Assign_Colony_New_Building_(struct s_colony* colony);

    /** Returns whether a colony contains foreign population from a treaty partner.
     * @param colony Colony to inspect.
     */
    bool __cdecl Colony_Is_Conquered_(struct s_colony* colony);

    /** Forces one listed colony to switch to a specific production item.
     * @param id_list List of colony ids to update.
     * @param list_index Index within id_list to process.
     * @param list_count Number of active ids in the list.
     * @param production_id Product id to assign.
     */
    void __cdecl Force_Produce_(uint8_t* id_list, int16_t list_index, int32_t* list_count, int16_t production_id);

    /** Returns a simple industry value estimate for a colony.
     * @param colony Colony to evaluate.
     */
    int16_t __cdecl Industry_Worth(struct s_colony* colony);

    /** Returns the display name for a colony production item.
     * @param product_id Building or project id to name.
     */
    char* __cdecl Product_Name_(int16_t product_id);

    /** Recomputes one colony and assigns a fresh autobuild target.
     * @param colony Colony to update.
     */
    void __cdecl Player_Colony_Autobuild_(s_colony* colony);
}