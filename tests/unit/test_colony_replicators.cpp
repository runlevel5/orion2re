#include "pch.h"

#include "doctest/doctest.h"

namespace TEST_COLONY_REPLICATORS {
    void Reset_();
}

DOCTEST_TEST_CASE("configured Food Replicator PP costs cap spending without negative production") {
    struct s_case {
        uint8_t pp_cost;
        int16_t expected_food;
        int16_t expected_industry;
    };

    const s_case cases[] = {
        {0, 2, 3},
        {1, 2, 1},
        {2, 1, 1},
        {3, 1, 0},
    };

    for (const s_case& test_case : cases) {
        TEST_COLONY_REPLICATORS::Reset_();
        COLCALC::_replicator_food_pp = test_case.pp_cost;

        s_colony* colony = &MOX::_colony[0];
        colony->buildings[BUILDING_FOOD_REPLICATORS] = 1;
        colony->production[ECON_INDUSTRY] = 3;
        colony->food2_needed_for_conquered = 3;

        COLCALC::Colony_Replicators_(colony);

        DOCTEST_CHECK((uint8_t)colony->n_food_replicated == test_case.expected_food);
        DOCTEST_CHECK(colony->production[ECON_FOOD] == test_case.expected_food);
        DOCTEST_CHECK(colony->production[ECON_INDUSTRY] == test_case.expected_industry);
    }
}

DOCTEST_TEST_CASE("Food Replicator BC cost is applied after climate-adjusted building maintenance") {
    struct s_case {
        uint8_t bc_cost;
        uint8_t expected_maintenance;
    };

    const s_case cases[] = {
        {0, 3},
        {1, 5},
        {3, 9},
    };

    for (const s_case& test_case : cases) {
        TEST_COLONY_REPLICATORS::Reset_();
        COLCALC::_replicator_food_bc = test_case.bc_cost;

        s_colony* colony = &MOX::_colony[0];
        colony->n_pops = 1;
        colony->climate = PLANET_CLIMATE_TOXIC;
        colony->buildings[BUILDING_AUTOMATED_FACTORY] = 1;
        colony->n_food_replicated = 2;

        COLCALC::Colony_BC_Maintenance_(colony);

        DOCTEST_CHECK((uint8_t)colony->maintenance[ECON_BC] == test_case.expected_maintenance);
    }
}