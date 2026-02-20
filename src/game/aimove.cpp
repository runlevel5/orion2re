#include "deps.h"

namespace aimove {
    void __cdecl Move_All_AI_() {
        for (int32_t i = 0; i < (int32_t)MOX::_NUM_PLAYERS; ++i) {
            s_player& plr = MOX::_player[i];

            if (plr.objectives != 100 && plr.ship_speed != 0) {
                MOX::Mox_Update_();

                if (plr.traits[TRAIT_TRANS_DIMENSIONAL] != 0 || EVENTS::Event_Check_Hyperspace_Flux_() == 0) {
                    Move_AI_(i);
                }
            }
        }

        Move_NPCs_();
    }
}