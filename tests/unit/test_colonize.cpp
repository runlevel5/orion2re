#include "pch.h"

#include "doctest/doctest.h"
#include "game/colonize.h"

namespace TEST_COLONIZE {
    void Reset_();
}

DOCTEST_TEST_CASE("same-owner outpost ship does not suppress colony ship priority") {
    TEST_COLONIZE::Reset_();

    MOX::_star[0].planet_index[0] = 0;
    MOX::_planet[0].star_index = 0;
    MOX::_planet[0].planet_type = PLANET_TYPE_PLANET;
    MOX::_planet[0].colony_index = 0;
    MOX::_colony[0].owner = 0;
    MOX::_colony[0].planet = 0;
    MOX::_colony[0].outpost_flag = 1;

    MOX::_ship[0].owner = 0;
    MOX::_ship[0].location = 0;
    MOX::_ship[0].d.ship_type = SHIP_TYPE_OUTPOST;
    MOX::_ship[1].owner = 0;
    MOX::_ship[1].location = 0;
    MOX::_ship[1].d.ship_type = SHIP_TYPE_COLONY;

    COLONIZE::Find_Colonize_Players_();

    DOCTEST_CHECK(MOX::_star[0].colonize_player == 0);
}