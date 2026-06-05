#pragma once

namespace settler {
    /**
     * Returns how many freighters the player can scrap without stranding settlers already in transit.
     * @param player_idx Player index to inspect.
     */
    int32_t __cdecl N_Freighters_Player_Can_Scrap_(int16_t player_idx);

    /**
     * Checks whether the colony can accept one more pop of the specified race.
     * @param colony Colony being checked.
     * @param race_idx Race index of the incoming pop.
     * @param include_freighters Nonzero to count settlers already heading to the colony.
     */
    bool __cdecl Room_For_Another_(s_colony* colony, int16_t race_idx, int8_t include_freighters);

    /**
     * Adds an encoded settler record to the colony if capacity allows.
     * @param settler_data Packed settler data to decode into a colony pop slot.
     * @param colony Destination colony receiving the pop.
     */
    int8_t __cdecl Add_Settler_To_Colony_(uint32_t settler_data, s_colony* colony);

    /**
     * Advances all in-transit settlers by one turn and resolves arrivals.
     */
    void __cdecl Move_Settlers_();

    /**
     * Validates whether the selected pop can settle the destination colony and records the result flags.
     * @param source_colony Colony providing the pop.
     * @param selected_pop_idx Pop index selected in the source colony.
     * @param dest_colony Destination colony to receive the pop.
     * @param result Output structure filled with settlement validation flags.
     */
    void __cdecl Pop_Tries_To_Settle_(
            s_colony *source_colony,
            int16_t selected_pop_idx,
            s_colony *dest_colony,
            s_settler_settle_result *result);

    /**
     * Computes the number of turns needed to move a settler between two stars.
     * @param player Player whose ship speed determines travel time.
     * @param source_star Source star index.
     * @param dest_star Destination star index.
     */
    int32_t __cdecl Settler_ETA_(s_player* player, int16_t source_star, int16_t dest_star);

    /**
     * Moves one pop from the source colony to the destination colony or dispatches it into transit.
     * @param source_colony Colony losing the pop.
     * @param pop_index Pop index to move from the source colony.
     * @param dest_colony Destination colony receiving the pop.
     */
    int32_t __cdecl Settle_Pop_(s_colony* source_colony, int16_t pop_index, s_colony* dest_colony);
}