#include "pch.h"

#include "doctest/doctest.h"

DOCTEST_TEST_CASE("refit cost uses the corrected shared formulas") {
    TECHDATA::_hull_data[HULL_FRIGATE].cost = 100;

    s_ship_design old_design = {};
    old_design.size = HULL_FRIGATE;
    old_design.cost = 100;

    s_ship_design small_upgrade = old_design;
    small_upgrade.cost = 105;
    s_ship_design large_upgrade = old_design;
    large_upgrade.cost = 130;
    s_ship_design downgrade = old_design;
    downgrade.cost = 80;

    DESIGN::_alternative_refit = 0;
    DOCTEST_CHECK(AIBUILD::Refit_Cost_(0, &old_design, &small_upgrade) == 25);
    DOCTEST_CHECK(AIBUILD::Refit_Cost_(1, &old_design, &small_upgrade) == 17);
    DOCTEST_CHECK(AIBUILD::Refit_Cost_(0, &old_design, &large_upgrade) == 60);
    DOCTEST_CHECK(AIBUILD::Refit_Cost_(0, &old_design, &downgrade) == 25);

    DESIGN::_alternative_refit = 1;
    DOCTEST_CHECK(AIBUILD::Refit_Cost_(0, &old_design, &small_upgrade) == 30);
    DOCTEST_CHECK(AIBUILD::Refit_Cost_(1, &old_design, &small_upgrade) == 22);
    DOCTEST_CHECK(AIBUILD::Refit_Cost_(0, &old_design, &large_upgrade) == 55);
    DOCTEST_CHECK(AIBUILD::Refit_Cost_(0, &old_design, &downgrade) == 25);

    DESIGN::_alternative_refit = 0;
}