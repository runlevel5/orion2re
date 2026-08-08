#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtfir2.h"
#include "game/combat1.h"

namespace TEST_STASIS_AREA_EFFECTS {
    void Reset_();
    void Set_Special_(int16_t ship_idx, int16_t special_id);
    void Set_In_Stasis_(int16_t ship_idx, bool in_stasis);
}

DOCTEST_TEST_CASE("stasis disables wide area jammer") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_combat_data[0].owner = 0;
    TEST_STASIS_AREA_EFFECTS::Set_Special_(0, SPECIAL_WIDE_AREA_JAMMER);

    int16_t base_bonus = 0;
    DOCTEST_CHECK(CMBTFIR2::Fleet_Defensive_Bonus_(0, 1, &base_bonus) == 70);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(0, true);
    base_bonus = 0;
    DOCTEST_CHECK(CMBTFIR2::Fleet_Defensive_Bonus_(0, 1, &base_bonus) == 0);
}

DOCTEST_TEST_CASE("stasis disables satellite attack bonus") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_combat_data[0].owner = 0;
    MOX::_combat_data[0].ftl_type = 0;
    MOX::_combat_data[0].size_class = 3;

    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 10);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(0, true);
    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 0);
}

DOCTEST_TEST_CASE("stasis disables scout lab") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_combat_attacker_player = 9;
    MOX::_combat_data[0].owner = 0;
    MOX::_combat_data[0].ftl_type = 1;
    TEST_STASIS_AREA_EFFECTS::Set_Special_(0, SPECIAL_SCOUT_LAB);

    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 20);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(0, true);
    DOCTEST_CHECK(CMBTFIR2::Fleet_Offensive_Bonus_(0) == 0);
}

DOCTEST_TEST_CASE("stasis disables warp dissipator") {
    TEST_STASIS_AREA_EFFECTS::Reset_();
    MOX::_combat_data[1].owner = 1;
    TEST_STASIS_AREA_EFFECTS::Set_Special_(1, SPECIAL_WARP_DISSIPATOR);

    DOCTEST_CHECK(COMBAT1::Enemy_Dissipator_Exists_(0) == 1);

    TEST_STASIS_AREA_EFFECTS::Set_In_Stasis_(1, true);
    DOCTEST_CHECK(COMBAT1::Enemy_Dissipator_Exists_(0) == 0);
}