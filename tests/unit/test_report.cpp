#include "pch.h"

#include "doctest/doctest.h"
#include "game/report.h"

namespace TEST_REPORT {
    extern int16_t colonizable_planet_count;

    void Reset_();
    void Add_Colony_Ship_();
}

DOCTEST_TEST_CASE("outpost prompt remains enabled by default") {
    TEST_REPORT::Reset_();

    DOCTEST_CHECK(REPORT::Colonization_(0, 0) == 1);
}

DOCTEST_TEST_CASE("outpost prompt can be skipped") {
    TEST_REPORT::Reset_();
    REPORT::_no_outpost_prompt = 1;

    DOCTEST_CHECK(REPORT::Colonization_(0, 0) == 0);
}

DOCTEST_TEST_CASE("skipping outpost prompt retains valid colony prompt") {
    TEST_REPORT::Reset_();
    TEST_REPORT::Add_Colony_Ship_();
    TEST_REPORT::colonizable_planet_count = 1;
    REPORT::_no_outpost_prompt = 1;

    DOCTEST_CHECK(REPORT::Colonization_(0, 0) == 1);
}

DOCTEST_TEST_CASE("skipping outpost prompt suppresses unusable colony prompt") {
    TEST_REPORT::Reset_();
    TEST_REPORT::Add_Colony_Ship_();
    REPORT::_no_outpost_prompt = 1;

    DOCTEST_CHECK(REPORT::Colonization_(0, 0) == 0);
}