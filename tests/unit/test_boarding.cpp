#include "pch.h"

#include "doctest/doctest.h"
#include "game/boarding.h"

namespace TEST_BOARDING {
    void Reset_();
}

DOCTEST_TEST_CASE("boarding rejects targets in stasis") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[1].stasis_source_idx = 0;

    DOCTEST_CHECK(
        BOARDING::Filter_Boarding_(0, 1) ==
        BOARDING::e_boarding_filter::target_in_stasis);
}

DOCTEST_TEST_CASE("classic boarding permits black-hole affected ships") {
    TEST_BOARDING::Reset_();
    MOX::_combat_data[0].black_hole_source_idx = 1;
    MOX::_combat_data[1].black_hole_source_idx = 0;

    DOCTEST_CHECK(
        BOARDING::Filter_Boarding_(0, 1) ==
        BOARDING::e_boarding_filter::allowed);
}

DOCTEST_TEST_CASE("disabled black-hole boarding filters both participants") {
    TEST_BOARDING::Reset_();
    BOARDING::_board_in_black_hole = 0;
    MOX::_combat_data[1].black_hole_source_idx = 0;

    DOCTEST_CHECK(
        BOARDING::Filter_Boarding_(0, 1) ==
        BOARDING::e_boarding_filter::target_in_black_hole);

    MOX::_combat_data[1].black_hole_source_idx = UINT8_MAX;
    MOX::_combat_data[0].black_hole_source_idx = 1;
    DOCTEST_CHECK(
        BOARDING::Filter_Boarding_(0, 1) ==
        BOARDING::e_boarding_filter::attacker_in_black_hole);
}
