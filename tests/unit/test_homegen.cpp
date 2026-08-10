#include "pch.h"

#include "doctest/doctest.h"

namespace TEST_HOMEGEN {
    void Reset_();
    int32_t Satellite_Generation_Count_();
}

DOCTEST_TEST_CASE("minimum-distance planet generation preserves existing satellite records") {
    TEST_HOMEGEN::Reset_();
    int16_t home_stars[1] = {0};

    HOMEGEN::Guarantee_A_Planet_At_Min_Dist_(0, 4, home_stars);

    DOCTEST_CHECK(MOX::_planet_count == 2);
    DOCTEST_CHECK(MOX::_star[1].planet_index[2] == 0);
    DOCTEST_CHECK(MOX::_planet[0].star_index == 1);
    DOCTEST_CHECK(MOX::_planet[0].planet_type == PLANET_TYPE_PLANET);
    DOCTEST_CHECK(TEST_HOMEGEN::Satellite_Generation_Count_() == 2);
}