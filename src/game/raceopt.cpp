#include "pch.h"

namespace raceopt {

    void __cdecl Setup_Evolutionary_Upgrade_(short player_idx) {
        s_player& plr = MOX::_player[player_idx];
        int8_t original_traits0 = plr.traits[0];

        if (plr.objectives == 100) {
            // Copy 30 bytes (traits index 1 to 30)
            struct_::Copy_Structure_(&plr.evolutionary_upgrade_traits[1], &plr.traits[1], 30);

            // Apply mask to trait 0 for the evolutionary upgrade
            plr.evolutionary_upgrade_traits[0] = original_traits0 & 0x0E;

            Evolutionary_Mutation_Screen_(plr.evolutionary_upgrade_traits);

            // Explicitly restore original traits[0]
            plr.traits[0] = original_traits0;
        }
        else {
            // Revert traits from evolutionary buffer if objectives condition not met
            struct_::Copy_Structure_(&plr.traits[1], &plr.evolutionary_upgrade_traits[1], 30);
        }

        plr.picked_evolutionary_upgrade = 1;
    }

}