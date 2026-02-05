#include "deps.h"

namespace COLXPORT {

/**
 * @brief Returns the number of non-outpost colonies belonging to a specific player.
 * 
 * If a "cheezy hack" state is active, it returns a cached value.
 * Otherwise, it iterates through the global colony list.
 */
short __cdecl N_Colonies_(short player_idx) {
    if (_cheezy_hack_col_count_state != 0) {
        return _cheezy_hack_col_count[player_idx];
    }

    short colony_count = 0;
    for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
        s_colony* colony = &MOX::_colony[i];

        // Only count colonies owned by player_idx that are NOT outposts
        if (colony->owner == (char)player_idx && colony->outpost_flag == 0) {
            colony_count++;
        }
    }

    return colony_count;
}

} // namespace COLXPORT