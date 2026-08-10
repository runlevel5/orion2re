#include "pch.h"

#include "doctest/doctest.h"

namespace TEST_AIDUDES {
    void Reset_();
    void Reset_Maintenance_Scrap_(bool add_replacement);
    int Pre_Import_Calls_();
    int8_t Status_During_Recalculation_();
    int16_t Pooled_Officer_();
}

DOCTEST_TEST_CASE("blockaded colony stops assigning farmers when recalculation disables farming") {
    TEST_AIDUDES::Reset_();

    s_aidudes_colony_info colony_info = {};
    colony_info.colony_idx = 0;

    aidudes::Do_Blockaded_Colony_(&colony_info);

    DOCTEST_CHECK(TEST_AIDUDES::Pre_Import_Calls_() == 2);
    DOCTEST_CHECK(colony_info.cur_pop_idx == 1);
    DOCTEST_CHECK(POP::Is_Worker(MOX::_colony[0].pop[0]));
    DOCTEST_CHECK(POP::Is_Assigned(MOX::_colony[0].pop[0]));
}

DOCTEST_TEST_CASE("maintenance marks a ship scrapped before recalculation and reassigns its officer") {
    TEST_AIDUDES::Reset_Maintenance_Scrap_(true);

    maintain::Kill_Ships_(&MOX::_player[0], false, false, SHIP_TYPE_COMBAT);

    DOCTEST_CHECK(TEST_AIDUDES::Status_During_Recalculation_() == 7);
    DOCTEST_CHECK(MOX::_ship[0].status == 7);
    DOCTEST_CHECK(MOX::_ship[0].officer_index == -1);
    DOCTEST_CHECK(MOX::_ship[1].officer_index == 0);
    DOCTEST_CHECK(MOX::_leaders[0].location == 1);
    DOCTEST_CHECK(TEST_AIDUDES::Pooled_Officer_() == -1);
}

DOCTEST_TEST_CASE("maintenance pools an officer when the scrapped ship has no replacement") {
    TEST_AIDUDES::Reset_Maintenance_Scrap_(false);

    maintain::Kill_Ships_(&MOX::_player[0], false, false, SHIP_TYPE_COMBAT);

    DOCTEST_CHECK(TEST_AIDUDES::Status_During_Recalculation_() == 7);
    DOCTEST_CHECK(MOX::_ship[0].officer_index == -1);
    DOCTEST_CHECK(MOX::_leaders[0].location == -1);
    DOCTEST_CHECK(TEST_AIDUDES::Pooled_Officer_() == 0);
}