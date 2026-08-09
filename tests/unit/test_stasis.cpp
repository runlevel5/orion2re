#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtfire.h"

namespace TEST_STASIS {
    void Reset_();
    int16_t Last_Cleanup_Source_();
    int16_t Last_Missile_Hits_Owner_();
    int16_t Last_Retargeted_Missile_();
}

DOCTEST_TEST_CASE("placing a ship in stasis cuts its continuing effects") {
    TEST_STASIS::Reset_();
    MOX::_missile[0].type = WEAPON_NUCLEAR_MISSILE;
    MOX::_missile[0].source_unit_idx = 0;
    MOX::_missile[0].target_unit_idx = 1;

    CMBTFIRE::Place_Ship_In_Stasis_(1, 0);

    DOCTEST_CHECK(MOX::_combat_data[1].stasis_source_idx == 0);
    DOCTEST_CHECK(TEST_STASIS::Last_Retargeted_Missile_() == 0);
    DOCTEST_CHECK(TEST_STASIS::Last_Cleanup_Source_() == 1);
}

DOCTEST_TEST_CASE("missile damage keeps launch armor after source ship capture") {
    TEST_STASIS::Reset_();
    MOX::_combat_data[1].owner = 2;
    MOX::_missile[0].type = WEAPON_INTERCEPTOR;
    MOX::_missile[0].owner = 1;
    MOX::_missile[0].source_unit_idx = 1;

    int16_t damage = 1;
    CMBTFIRE::Apply_Damage_To_Missile_(0, &damage, 0, 0);

    DOCTEST_CHECK(TEST_STASIS::Last_Missile_Hits_Owner_() == 1);
}