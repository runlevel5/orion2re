#include "pch.h"

#include "doctest/doctest.h"

DOCTEST_TEST_CASE("refit cost uses the corrected shared formulas") {
    int16_t saved_hull_cost = TECHDATA::_hull_data[HULL_FRIGATE].cost;
    int8_t saved_player_0_government =
        MOX::_player[0].traits[TRAIT_CURRENT_GOVERNMENT];
    int8_t saved_player_1_government =
        MOX::_player[1].traits[TRAIT_CURRENT_GOVERNMENT];
    TECHDATA::_hull_data[HULL_FRIGATE].cost = 100;
    MOX::_player[0].traits[TRAIT_CURRENT_GOVERNMENT] = GOVERNMENT_DICTATORSHIP;
    MOX::_player[1].traits[TRAIT_CURRENT_GOVERNMENT] = GOVERNMENT_FEUDAL;

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
    TECHDATA::_hull_data[HULL_FRIGATE].cost = saved_hull_cost;
    MOX::_player[0].traits[TRAIT_CURRENT_GOVERNMENT] = saved_player_0_government;
    MOX::_player[1].traits[TRAIT_CURRENT_GOVERNMENT] = saved_player_1_government;
}

DOCTEST_TEST_CASE("government cost reductions apply to normally neutral governments") {
    int8_t saved_government =
        MOX::_player[0].traits[TRAIT_CURRENT_GOVERNMENT];
    s_ratio16 saved_reduction =
        COLCALC::government_bonuses[GOVERNMENT_DICTATORSHIP].cost_reduction;
    int32_t cost = 101;

    MOX::_player[0].traits[TRAIT_CURRENT_GOVERNMENT] = GOVERNMENT_DICTATORSHIP;
    COLCALC::government_bonuses[GOVERNMENT_DICTATORSHIP].cost_reduction = { 1, 4 };
    DESIGN::Cost_Reduction_For_Govt_Type_(0, &cost);
    DOCTEST_CHECK(cost == 76);

    cost = 101;
    COLCALC::government_bonuses[GOVERNMENT_DICTATORSHIP].cost_reduction = { 0, 1 };
    DESIGN::Cost_Reduction_For_Govt_Type_(0, &cost);
    DOCTEST_CHECK(cost == 101);

    MOX::_player[0].traits[TRAIT_CURRENT_GOVERNMENT] = saved_government;
    COLCALC::government_bonuses[GOVERNMENT_DICTATORSHIP].cost_reduction =
        saved_reduction;
}