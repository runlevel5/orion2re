#include "pch.h"

#include "doctest/doctest.h"

DOCTEST_TEST_SUITE("colony production helpers") {

DOCTEST_TEST_CASE("building product classification excludes sentinels and upper boundary") {
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Building_(BUILDING_NO_BUILDING));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Building_(BUILDING_ALIEN_CONTROL_CENTER));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Building_(BUILDING_ARTIFICIAL_PLANET));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Building_(BUILDING_COUNT));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Building_(COLONY_PRODUCTION_NONE));
}

DOCTEST_TEST_CASE("ship product classification preserves queued and design ranges") {
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Queued_Ship_(-101));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Queued_Ship_(-100));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Queued_Ship_(-99));

    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Ship_Design_(-100));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_Design_(-99));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_Design_(-50));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Ship_Design_(-49));

    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(-101));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(-100));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(-99));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(-50));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Ship_(-49));
}

DOCTEST_TEST_CASE("ship product encodings round trip through their matching ranges") {
    for (int16_t ship_idx : {int16_t{0}, int16_t{1}, int16_t{MAX_SHIPS - 1}}) {
        const int16_t production_id = COLBLDG::Colony_Production_From_Ship_Index_(ship_idx);
        DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Queued_Ship_(production_id));
        DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(production_id));
        DOCTEST_CHECK(COLBLDG::Colony_Production_Ship_Index_(production_id) == ship_idx);
    }

    for (int16_t design_idx : {int16_t{0}, int16_t{1}, int16_t{49}}) {
        const int16_t production_id = COLBLDG::Colony_Production_From_Ship_Design_Index_(design_idx);
        DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_Design_(production_id));
        DOCTEST_CHECK(COLBLDG::Colony_Production_Ship_Design_Index_(production_id) == design_idx);
    }
}

}