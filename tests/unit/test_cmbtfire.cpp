#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtfire.h"

namespace TEST_STASIS {
    void Reset_();
    int16_t Last_Cleanup_Source_();
    int16_t Last_Missile_Hits_Owner_();
    int16_t Last_Retargeted_Missile_();
}

namespace TEST_CRYSTAL_FLEET {
    void Reset_();
    int16_t Cleanup_Count_();
    int16_t Cleanup_Source_(int16_t index);
}

DOCTEST_TEST_CASE("destroyed Crystal releases captures and invalidates effects") {
    TEST_CRYSTAL_FLEET::Reset_();

    MOX::_combat_data[1].owner = 11;
    MOX::_combat_data[1].previous_owner = 11;

    MOX::_combat_data[2].owner = 11;
    MOX::_combat_data[2].previous_owner = 0;
    MOX::_combat_data[2].ship_image_seg =
        reinterpret_cast<s_animation_header*>(1);

    CMBTFIRE::Destroy_Ship_(1, 0, 0, 1);

    DOCTEST_CHECK(MOX::_combat_data[2].owner == 0);
    DOCTEST_CHECK(MOX::_combat_data[2].ship_image_seg == nullptr);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Count_() == 2);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Source_(0) == 1);
    DOCTEST_CHECK(TEST_CRYSTAL_FLEET::Cleanup_Source_(1) == 2);
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