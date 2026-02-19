#include "deps.h"
#include <cstring>

namespace aitech {
    /**
     * @brief Applies evolutionary upgrades to players who have picked them but not yet applied them.
     * 
     * The function iterates through all players, checks if they have a pending evolutionary upgrade,
     * and if so, replaces their current traits with the upgrade traits while preserving the 
     * first trait (traits[0]).
     */
    void Apply_Evolutionary_Upgrades_() {
        for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];

            if (plr->picked_evolutionary_upgrade != 0 && plr->applied_evolutionary_upgrade == 0) {
                int8_t saved_trait_0 = plr->traits[0];
                
                memcpy(plr->traits, plr->evolutionary_upgrade_traits, sizeof(plr->traits));
                
                plr->traits[0] = saved_trait_0;
                
                plr->applied_evolutionary_upgrade = 1;
            }
        }
    }
}