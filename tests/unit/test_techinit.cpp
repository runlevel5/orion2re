#include "pch.h"

#include "doctest/doctest.h"
#include "game/techinit.h"

namespace TEST_TECHINIT {
    void Reset_();
}

DOCTEST_TEST_CASE("cybernetic races cannot receive advanced damage control") {
    TEST_TECHINIT::Reset_();
    MOX::_player[0].traits[TRAIT_CYBERNETIC] = 1;

    DOCTEST_CHECK(
        TECHINIT::Tech_Is_Legal_For_Player_(
            0, TECH_APP_ADVANCED_DAMAGE_CONTROL) == 0);
    DOCTEST_CHECK(
        TECHINIT::Tech_Is_Legal_For_Player_(
            0, TECH_APP_ADAMANTIUM_ARMOR) == 1);
}

DOCTEST_TEST_CASE("non-cybernetic races may receive advanced damage control") {
    TEST_TECHINIT::Reset_();

    DOCTEST_CHECK(
        TECHINIT::Tech_Is_Legal_For_Player_(
            0, TECH_APP_ADVANCED_DAMAGE_CONTROL) == 1);
}

DOCTEST_TEST_CASE("strategic combat uses the corrected technology tree") {
    TEST_TECHINIT::Reset_();
    MOX::_settings.strategic_combat_flag = 1;

    DOCTEST_CHECK(
        TECHINIT::Tech_Is_Legal_For_Player_(
            0, TECH_APP_ANTI_MISSILE_ROCKETS) == 0);
    DOCTEST_CHECK(
        TECHINIT::Tech_Is_Legal_For_Player_(
            0, TECH_APP_ENERGY_ABSORBER) == 0);
    DOCTEST_CHECK(
        TECHINIT::Tech_Is_Legal_For_Player_(
            0, TECH_APP_SECURITY_STATIONS) == 0);
    DOCTEST_CHECK(
        TECHINIT::Tech_Is_Legal_For_Player_(
            0, TECH_APP_WARP_DISSIPATER) == 0);
    DOCTEST_CHECK(
        TECHINIT::Tech_Is_Legal_For_Player_(
            0, TECH_APP_WARP_INTERDICTOR) == 1);
}

DOCTEST_TEST_CASE("Post-warp tactical games grant the common technology segment") {
    TEST_TECHINIT::Reset_();

    TECHINIT::Grant_Postwarp_Technologies_();

    DOCTEST_CHECK(
        MOX::_player[0].tech_applications[TECH_APP_REINFORCED_HULL] !=
        TECH_RESEARCH_STATUS_RESEARCHED);
    DOCTEST_CHECK(
        MOX::_player[0].tech_applications[TECH_APP_AUTOMATED_FACTORIES] ==
        TECH_RESEARCH_STATUS_RESEARCHED);
    DOCTEST_CHECK(
        MOX::_player[0].tech_applications[TECH_APP_FIGHTER_BAYS] ==
        TECH_RESEARCH_STATUS_RESEARCHED);
}

DOCTEST_TEST_CASE("Post-warp strategic games also grant the leading exceptions") {
    TEST_TECHINIT::Reset_();
    MOX::_settings.strategic_combat_flag = 1;

    TECHINIT::Grant_Postwarp_Technologies_();

    DOCTEST_CHECK(
        MOX::_player[0].tech_applications[TECH_APP_REINFORCED_HULL] ==
        TECH_RESEARCH_STATUS_RESEARCHED);
    DOCTEST_CHECK(
        MOX::_player[0].tech_applications[TECH_APP_ANTI_MISSILE_ROCKETS] ==
        TECH_RESEARCH_STATUS_RESEARCHED);
    DOCTEST_CHECK(
        MOX::_player[0].tech_applications[TECH_APP_FIGHTER_BAYS] ==
        TECH_RESEARCH_STATUS_RESEARCHED);
}
