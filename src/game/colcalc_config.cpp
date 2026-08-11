#include "pch.h"

namespace COLCALC {
    s_growth_rate_bonus growth_rate_bonus = {
        0,
        25,
        50,
        100,
        -200,
    };

    uint8_t _housing_creates_no_farmers = 0;
    uint8_t _planet_extinction = 0;
    uint8_t _zero_colony_lost = 0;
    uint8_t _keep_research_on_breakthrough = 0;
    uint8_t _keep_research_on_finds = 0;
    uint8_t _replicator_food_pp = 2;
    uint8_t _replicator_food_bc = 1;
    uint8_t _freighters_cash_bonus = 0;
}