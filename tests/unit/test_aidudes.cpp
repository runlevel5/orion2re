#include "pch.h"

#include "doctest/doctest.h"

namespace TEST_AIDUDES {
    void Reset_();
    int Pre_Import_Calls_();
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