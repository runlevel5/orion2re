#include "deps.h"

namespace ERICNET {
    void __cdecl Init_Homeworld_Colony_(int16_t colony_idx, int16_t player_idx) {
        Init_Homeworld_Colony2_(colony_idx, player_idx, 1);
    }

    void __cdecl Init_Non_Homeworld_Colony_(int16_t colony_idx, int16_t player_idx) {
        Init_Homeworld_Colony2_(colony_idx, player_idx, 0);
    }

    int __cdecl Mox_Play_Sound_(int16_t sound_id) {
        if (MOX::_settings.sound_fx_on != 0) {
            return sound::Play_Sound_(sound_id);
        }

        return 0;
    }
}