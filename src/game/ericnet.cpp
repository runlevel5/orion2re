#include "deps.h"

namespace ERICNET {
    void __cdecl Init_Non_Homeworld_Colony_(int16_t colony_idx, int16_t player_idx) {
        Init_Homeworld_Colony2_(colony_idx, player_idx, 0);
    }
}