#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtfir2.h"
#include "game/cmbtfire.h"

namespace TEST_ABSORBER_CLEANUP {
    void Reset_();
    void Set_Special_(int16_t ship_idx, int16_t special_id);
    int Warning_Count_();
    const char* Last_Warning_();
    int Combat_Refresh_Count_();
}

DOCTEST_TEST_CASE("full absorber charge destroys its ship") {
    TEST_ABSORBER_CLEANUP::Reset_();
    MOX::_combat_data[1].reflected_damage_pool = INT16_MAX;

    DOCTEST_CHECK(CMBTFIRE::Is_Combat_Ship_Dead_(1) == 4);
}

DOCTEST_TEST_CASE("absorber overload uses the overload explosion") {
    TEST_ABSORBER_CLEANUP::Reset_();
    MOX::_combat_data[1].reflected_damage_pool = INT16_MAX;
    TEST_ABSORBER_CLEANUP::Set_Special_(1, SPECIAL_QUANTUM_DETONATOR);

    DOCTEST_CHECK(
        CMBTFIR2::Damage_From_Ship_Exploding_(1) == 30 * 5 * 77);
}

DOCTEST_TEST_CASE("visible absorber overload displays its warning") {
    TEST_ABSORBER_CLEANUP::Reset_();
    MOX::_combat_data[1].reflected_damage_pool = INT16_MAX;
    MOX::_apply_damage_to_planet = 1;

    CMBTFIR2::Damage_From_Ship_Exploding_(1);

    DOCTEST_CHECK(TEST_ABSORBER_CLEANUP::Warning_Count_() == 1);
    DOCTEST_CHECK(
        std::string(TEST_ABSORBER_CLEANUP::Last_Warning_()) ==
        "Absorber Overloaded");
    DOCTEST_CHECK(TEST_ABSORBER_CLEANUP::Combat_Refresh_Count_() == 1);
}

DOCTEST_TEST_CASE("quantum detonator keeps its normal explosion multiplier") {
    TEST_ABSORBER_CLEANUP::Reset_();
    TEST_ABSORBER_CLEANUP::Set_Special_(1, SPECIAL_QUANTUM_DETONATOR);

    DOCTEST_CHECK(
        CMBTFIR2::Damage_From_Ship_Exploding_(1) == 30 * 5 * 3);
}

DOCTEST_TEST_CASE("computerless ships have no damageable computer subsystem") {
    TEST_ABSORBER_CLEANUP::Reset_();
    s_combat_data* ship = &MOX::_combat_data[1];
    TECHDATA::_hull_data[HULL_CRUISER].computer_hp = 5;

    ship->computer_type = COMPUTER_NO_COMPUTER;
    CMBTFIR2::Init_Internal_Space_(ship);

    DOCTEST_CHECK(ship->computer_max_hits == 0);
    DOCTEST_CHECK(ship->computer_current_hits == 0);

    ship->computer_type = COMPUTER_ELECTRONIC_COMPUTER;
    CMBTFIR2::Init_Internal_Space_(ship);

    DOCTEST_CHECK(ship->computer_max_hits == 5);
    DOCTEST_CHECK(ship->computer_current_hits == 5);
}

DOCTEST_TEST_CASE("configured systems repair percentage scales internal repair") {
    TEST_ABSORBER_CLEANUP::Reset_();
    s_combat_data* ship = &MOX::_combat_data[1];
    ship->ship_idx = 0;
    ship->structure_damage = 100;
    ship->drive_max_hits = 200;
    ship->drive_current_hits = 0;
    CMBTFIR2::_systems_repair_percent = 25;

    CMBTFIR2::Repair_Combat_Ship_(1, 20);

    DOCTEST_CHECK(ship->drive_current_hits == 5);
}