#include "pch.h"

#include "doctest/doctest.h"
#include "game/cmbtmis.h"

DOCTEST_TEST_CASE("missile defense percentage rolls include their threshold") {
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Defense_Roll_Succeeds_(1, 0));

    DOCTEST_CHECK(CMBTMIS::Missile_Defense_Roll_Succeeds_(29, 30));
    DOCTEST_CHECK(CMBTMIS::Missile_Defense_Roll_Succeeds_(30, 30));
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Defense_Roll_Succeeds_(31, 30));

    DOCTEST_CHECK(CMBTMIS::Missile_Defense_Roll_Succeeds_(49, 50));
    DOCTEST_CHECK(CMBTMIS::Missile_Defense_Roll_Succeeds_(50, 50));
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Defense_Roll_Succeeds_(51, 50));
}

DOCTEST_TEST_CASE("expected missile damage applies only the active cloak defense mode") {
    CMBTMIS::missile_defense.cloaking_device_missile_defense = 0;
    CMBTMIS::missile_defense.cloaking_device_missile_miss_percent = 50;

    DOCTEST_CHECK(CMBTMIS::Expected_Damage_After_Cloak_(100, false) == 100);
    DOCTEST_CHECK(CMBTMIS::Expected_Damage_After_Cloak_(100, true) == 50);

    CMBTMIS::missile_defense.cloaking_device_missile_miss_percent = 30;
    DOCTEST_CHECK(CMBTMIS::Expected_Damage_After_Cloak_(100, true) == 70);

    CMBTMIS::missile_defense.cloaking_device_missile_defense = 40;
    DOCTEST_CHECK(CMBTMIS::Expected_Damage_After_Cloak_(100, true) == 100);

    CMBTMIS::missile_defense = { 0, 50, 30 };
}

DOCTEST_TEST_CASE("flyer targeting follows anti-missile chains to the final ship") {
    s_missile missiles[3] = {};

    missiles[0].is_anti_missile_rocket = 1;
    missiles[0].target_unit_idx = 1;
    missiles[1].is_anti_missile_rocket = 1;
    missiles[1].target_unit_idx = 2;
    missiles[2].target_unit_idx = 7;

    DOCTEST_CHECK(CMBTMIS::Final_Flyer_Target_(missiles, 0) == 7);
    DOCTEST_CHECK(CMBTMIS::Final_Flyer_Target_(missiles, 2) == 7);
}

DOCTEST_TEST_CASE("ship movement excludes flyers targeting another missile") {
    s_missile missile = {};
    missile.type = WEAPON_NUCLEAR_MISSILE;
    missile.target_unit_idx = 4;

    DOCTEST_CHECK(CMBTMIS::Missile_Targets_Ship_(&missile, 4));

    missile.is_anti_missile_rocket = 1;
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Targets_Ship_(&missile, 4));

    missile.is_anti_missile_rocket = 0;
    missile.type = WEAPON_NO_WEAPONS;
    DOCTEST_CHECK_FALSE(CMBTMIS::Missile_Targets_Ship_(&missile, 4));
}