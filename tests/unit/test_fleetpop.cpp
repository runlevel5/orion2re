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