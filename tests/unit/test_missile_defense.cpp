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