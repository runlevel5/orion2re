#include "deps.h"
#include "aibuild.h"

namespace AIBUILD {
    /**
     * Calculates the refit cost to upgrade from an old design to a new design.
     * The cost is generally twice the difference in production cost, but no less 
     * than half the hull space (adjusted for government bonuses).
     */
    int16_t __cdecl Refit_Cost_(int16_t player_idx, s_ship_design* old_design, s_ship_design* new_design) {
        // Minimum cost is half of the hull's available space
        int32_t min_cost = (int32_t)TECHDATA::_hull_data[(uint8_t)new_design->size].hull_space / 2;
        
        // Base refit cost is twice the difference in design costs
        int32_t refit_cost = ((int32_t)new_design->cost - (int32_t)old_design->cost) * 2;

        // Apply government-based cost reductions to the floor price
        DESIGN::Cost_Reduction_For_Govt_Type_(player_idx, &min_cost);

        // Result cannot be lower than the floor price
        if (refit_cost < min_cost) {
            refit_cost = min_cost;
        }

        return (int16_t)refit_cost;
    }
}