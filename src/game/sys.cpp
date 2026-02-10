#include "deps.h"

namespace SYS {
    /**
     * Determines if a system is visible/viewable for a specific player.
     * 
     * @param star_idx   The index of the system/star in MOX::_star array.
     * @param player_idx The index of the player.
     * @return true if the system can be viewed, false otherwise.
     */
    bool __cdecl Ok_To_View_System_(int16_t star_idx, int16_t player_idx) {
        // Collect visibility conditions
        bool has_visited = HAROLD::Player_Has_Visited_(player_idx, star_idx);
        bool is_omniscient = HAROLD::Player_Is_Omniscient_(player_idx);

        // System cannot be viewed if it is a black hole
        if (MOX::_star[star_idx].spectral_class == STAR_CLASS_BLACK_HOLE) {
            return false;
        }

        // Otherwise, it is viewable if the player has visited it or has omniscience
        return (has_visited || is_omniscient);
    }
}