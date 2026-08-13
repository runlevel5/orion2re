#include "pch.h"

#include "doctest/doctest.h"
#include "game/colbldg.h"

namespace TEST_COLBLDG {
    void Reset_();
}

DOCTEST_TEST_CASE("enemy outpost is not a colony base target") {
    TEST_COLBLDG::Reset_();

    for (int16_t i = 0; i < 5; ++i) {
        MOX::_star[0].planet_index[i] = i;
        MOX::_planet[i].planet_type = PLANET_TYPE_PLANET;
    }

    MOX::_planet[0].colony_index = 0;
    MOX::_colony[0].owner = 0;

    MOX::_planet[1].colony_index = -1;

    MOX::_planet[2].colony_index = 1;
    MOX::_colony[1].owner = 0;
    MOX::_colony[1].outpost_flag = 1;

    MOX::_planet[3].colony_index = 2;
    MOX::_colony[2].owner = 1;
    MOX::_colony[2].outpost_flag = 1;

    MOX::_planet[4].colony_index = -1;
    MOX::_planet[4].planet_type = PLANET_TYPE_ASTEROID;

    DOCTEST_CHECK(COLBLDG::Star_N_Uncolonized_Worlds_(0) == 2);
}