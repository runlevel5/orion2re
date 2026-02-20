#include "deps.h"

namespace aidudes {

    void All_Colony_AI_() {
        int16_t num_players = (int16_t)MOX::_NUM_PLAYERS;

        for (int16_t i = 0; i < num_players; ++i) {
            if (MOX::_player[i].objectives != 100) {
                MOX::Mox_Update_();
                Colony_AI_(i);
            }
        }

        MOX::Mox_Update_();

        int16_t num_colonies = (int16_t)MOX::_NUM_COLONIES;
        for (int i = 0; i < num_colonies; ++i) {
            if (MOX::_colony[i].producing[0] == -37) {
                MOX::_colony[i].producing[0] = -2;
            }
        }
    }
}