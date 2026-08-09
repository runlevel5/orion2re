#include "pch.h"

#include "doctest/doctest.h"
#include "game/fleetpop.h"

namespace TEST_FLEETPOP {
    void Reset_();
}

DOCTEST_TEST_CASE("fleet at star exposes build outpost button") {
    TEST_FLEETPOP::Reset_();
    MOX::_ship[0].location = 1;

    DOCTEST_CHECK(FLEETPOP::Set_Build_Outpost_Button_Flag_(1) == 1);
}

DOCTEST_TEST_CASE("fleet en route does not expose build outpost button") {
    TEST_FLEETPOP::Reset_();
    MOX::_ship[0].location =
        static_cast<int16_t>(SHIP_LOCATION_WORMHOLE_OFFSET + 1);

    DOCTEST_CHECK(FLEETPOP::Set_Build_Outpost_Button_Flag_(1) == 0);
}

DOCTEST_TEST_CASE("fleet summary counts only ships linked into the visible stack") {
    TEST_FLEETPOP::Reset_();

    MOX::_ship_node[0].next_node = 1;
    MOX::_ship_node[1].ship_idx = 1;
    MOX::_ship_node[1].next_node = -1;

    MOX::_ship[0].d.ship_type = SHIP_TYPE_COMBAT;
    MOX::_ship[0].d.size = SHIP_SIZE_SMALL;
    MOX::_ship[1].d.ship_type = SHIP_TYPE_OUTPOST;
    MOX::_ship[2].d.ship_type = SHIP_TYPE_COLONY;

    int16_t total_count;
    int16_t colony_ships_count;
    int16_t outposts_count;
    int16_t transports_count;
    int16_t ship_sizes[SHIP_SIZE_DOOM_STAR + 1];

    SYS::N_Ships_By_Type_In_Stack_(
        0,
        &total_count,
        &colony_ships_count,
        &outposts_count,
        &transports_count,
        ship_sizes);

    DOCTEST_CHECK(total_count == 2);
    DOCTEST_CHECK(ship_sizes[SHIP_SIZE_SMALL] == 1);
    DOCTEST_CHECK(colony_ships_count == 0);
    DOCTEST_CHECK(outposts_count == 1);
    DOCTEST_CHECK(transports_count == 0);
}