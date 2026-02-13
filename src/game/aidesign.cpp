#include "deps.h"

namespace aidesign {
    int16_t __cdecl Cost_Reduction_For_Govt_Hull_Type(int16_t govt_type, s_ship_design* design) {
        int32_t reduced_cost = (TECHDATA::_hull_data[design->size].hull_space * 3) / 2;

        DESIGN::Cost_Reduction_For_Govt_Type_(govt_type, &reduced_cost);

        return (int16_t)reduced_cost;
    }
}