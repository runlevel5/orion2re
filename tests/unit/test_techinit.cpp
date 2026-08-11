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
