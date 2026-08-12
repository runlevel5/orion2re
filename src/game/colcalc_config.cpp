#include "pch.h"

namespace COLCALC {
    s_lander_ship_cost lander_ship_cost = {
        500,
        100,
        100,
    };

    s_misc_costs misc_costs = {
        50,
        50,
        50,
    };

    s_growth_rate_bonus growth_rate_bonus = {
        0,
        25,
        50,
        100,
        -200,
    };

    uint8_t _pop_bonus_biospheres = 2;
    uint8_t _housing_creates_no_farmers = 0;
    uint8_t _planet_extinction = 0;
    uint8_t _zero_colony_lost = 0;
    uint8_t _keep_research_on_breakthrough = 0;
    uint8_t _keep_research_on_finds = 0;
    uint8_t scout_lab_research_bonus[SHIP_SIZE_COUNT] = {
        1, 2, 4, 8, 16, 32,
    };
    uint8_t _replicator_food_pp = 2;
    uint8_t _replicator_food_bc = 1;
    uint8_t _freighters_cash_bonus = 0;
}