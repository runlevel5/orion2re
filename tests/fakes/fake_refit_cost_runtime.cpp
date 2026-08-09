#include "pch.h"

namespace TECHDATA {
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
}

namespace DESIGN {
    void __cdecl Cost_Reduction_For_Govt_Type_(int16_t player_idx, int32_t* cost) {
        if (player_idx == 1) {
            *cost = (*cost * 2 + 2) / 3;
        }
    }
}