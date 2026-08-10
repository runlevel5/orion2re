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
