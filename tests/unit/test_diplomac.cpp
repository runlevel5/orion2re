#include "pch.h"

#include "doctest/doctest.h"
#include "game/diplomac.h"

namespace TEST_DIPLOMAC {
    void Reset_();
}

DOCTEST_TEST_CASE("Meklar and Bulrathi starting relations are symmetric") {
    TEST_DIPLOMAC::Reset_();
    MOX::_player[0].race = STOCK_RACE_BULRATHI;
    MOX::_player[1].race = STOCK_RACE_MEKLAR;

    diplomac::Init_Diplomatic_Relations_();

    DOCTEST_CHECK(MOX::_player[0].relations[1] == -12);
    DOCTEST_CHECK(MOX::_player[1].relations[0] == -12);
    DOCTEST_CHECK(MOX::_player[0].base_relations[1] == -12);
    DOCTEST_CHECK(MOX::_player[1].base_relations[0] == -12);
}