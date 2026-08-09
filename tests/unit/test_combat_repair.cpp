#include "pch.h"

#include "doctest/doctest.h"
#include "game/combinit.h"

namespace TEST_COMBAT_REPAIR {
    void Reset_();
    void Set_Special_(int16_t ship_idx, int16_t special_id);
    void Damage_Special_(int16_t ship_idx, int16_t special_id);
    void Set_Cybernetic_(int16_t player_idx);
    void Set_Engineer_Bonus_(int16_t player_idx, int16_t amount);
    int Repair_Count_(int16_t ship_idx);
    int16_t Repair_Amount_(int16_t ship_idx);
}

DOCTEST_TEST_CASE("regeneration remains disabled by default") {
    TEST_COMBAT_REPAIR::Reset_();
    TEST_COMBAT_REPAIR::Set_Special_(0, SPECIAL_REGENERATION);

    COMBINIT::Repair_All_Combat_Ships_();

    DOCTEST_CHECK(TEST_COMBAT_REPAIR::Repair_Count_(0) == 0);
}

DOCTEST_TEST_CASE("working regeneration uses its configured repair rate") {
    TEST_COMBAT_REPAIR::Reset_();
    COMBINIT::_amoeba_regeneration = 1;
    COMBINIT::repair_rate.regen = 27;
    TEST_COMBAT_REPAIR::Set_Special_(0, SPECIAL_REGENERATION);

    COMBINIT::Repair_All_Combat_Ships_();

    DOCTEST_CHECK(TEST_COMBAT_REPAIR::Repair_Amount_(0) == 27);
}

DOCTEST_TEST_CASE("regeneration is additive with other repair sources") {
    TEST_COMBAT_REPAIR::Reset_();
    COMBINIT::_amoeba_regeneration = 1;
    COMBINIT::repair_rate = {7, 11, 13};
    TEST_COMBAT_REPAIR::Set_Special_(0, SPECIAL_AUTOMATED_REPAIR_UNIT);
    TEST_COMBAT_REPAIR::Set_Special_(0, SPECIAL_REGENERATION);
    TEST_COMBAT_REPAIR::Set_Cybernetic_(0);
    TEST_COMBAT_REPAIR::Set_Engineer_Bonus_(0, 4);

    COMBINIT::Repair_All_Combat_Ships_();

    DOCTEST_CHECK(TEST_COMBAT_REPAIR::Repair_Amount_(0) == 35);
}

DOCTEST_TEST_CASE("damaged regeneration does not repair") {
    TEST_COMBAT_REPAIR::Reset_();
    COMBINIT::_amoeba_regeneration = 1;
    TEST_COMBAT_REPAIR::Set_Special_(0, SPECIAL_REGENERATION);
    TEST_COMBAT_REPAIR::Damage_Special_(0, SPECIAL_REGENERATION);

    COMBINIT::Repair_All_Combat_Ships_();

    DOCTEST_CHECK(TEST_COMBAT_REPAIR::Repair_Count_(0) == 0);
}