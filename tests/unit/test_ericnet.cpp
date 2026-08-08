#include "pch.h"

#include "doctest/doctest.h"
#include "game/ericnet.h"

namespace {
    const int16_t default_reengage_buildings[] = {
        BUILDING_ARTEMIS_SYSTEM_NET,
        BUILDING_BATTLESTATION,
        BUILDING_MISSILE_BASE,
        BUILDING_GROUND_BATTERIES,
        BUILDING_STAR_BASE,
        BUILDING_STAR_FORTRESS,
        BUILDING_STELLAR_CONVERTER,
        BUILDING_FIGHTER_GARRISON,
    };

    const int16_t planetary_shields[] = {
        BUILDING_RADIATION_SHIELD,
        BUILDING_FLUX_SHIELD,
        BUILDING_BARRIER_SHIELD,
    };

    void Reset_Colony_() {
        memset(MOX::_colony, 0, sizeof(s_colony));
        memset(MOX::_planet, 0, sizeof(s_planet_data));
        memset(MOX::_star, 0, sizeof(s_star_data));
        memset(MOX::_ship, 0, sizeof(s_ship_data));
        MOX::_colony[0].planet = 0;
        MOX::_planet[0].star_index = 0;
    }

    void Restore_Default_Reengage_Buildings_() {
        memset(ERICNET::_combat_reengage_buildings, BUILDING_NO_BUILDING,
               sizeof(ERICNET::_combat_reengage_buildings));
        for (size_t index = 0;
             index < sizeof(default_reengage_buildings) / sizeof(default_reengage_buildings[0]);
             ++index) {
            ERICNET::_combat_reengage_buildings[index] =
                static_cast<uint8_t>(default_reengage_buildings[index]);
        }
    }
}

DOCTEST_TEST_SUITE("building combat re-engagement") {

DOCTEST_TEST_CASE("default defenses invalidate ignored combat when added or removed") {
    Restore_Default_Reengage_Buildings_();
    for (int16_t building_id : default_reengage_buildings) {
        Reset_Colony_();
        MOX::_star[0].ignore_combat_ships = 0x3F;
        ERICNET::Add_Building_(0, building_id);
        DOCTEST_CHECK(MOX::_star[0].ignore_combat_ships == 0);

        Reset_Colony_();
        MOX::_colony[0].buildings[building_id] = 1;
        MOX::_star[0].ignore_combat_ships = 0x3F;
        ERICNET::Remove_Building_(0, building_id);
        DOCTEST_CHECK(MOX::_star[0].ignore_combat_ships == 0);
    }
}

DOCTEST_TEST_CASE("planetary shields preserve ignored combat by default") {
    Restore_Default_Reengage_Buildings_();
    for (int16_t building_id : planetary_shields) {
        Reset_Colony_();
        MOX::_star[0].ignore_combat_ships = 0x3F;
        ERICNET::Add_Building_(0, building_id);
        DOCTEST_CHECK(MOX::_star[0].ignore_combat_ships == 0x3F);

        Reset_Colony_();
        MOX::_colony[0].buildings[building_id] = 1;
        MOX::_star[0].ignore_combat_ships = 0x3F;
        ERICNET::Remove_Building_(0, building_id);
        DOCTEST_CHECK(MOX::_star[0].ignore_combat_ships == 0x3F);
    }
}

DOCTEST_TEST_CASE("configured building list controls combat invalidation") {
    memset(ERICNET::_combat_reengage_buildings, BUILDING_NO_BUILDING,
           sizeof(ERICNET::_combat_reengage_buildings));
    ERICNET::_combat_reengage_buildings[0] = BUILDING_RADIATION_SHIELD;

    Reset_Colony_();
    MOX::_star[0].ignore_combat_ships = 0x3F;
    ERICNET::Add_Building_(0, BUILDING_RADIATION_SHIELD);
    DOCTEST_CHECK(MOX::_star[0].ignore_combat_ships == 0);

    Reset_Colony_();
    MOX::_star[0].ignore_combat_ships = 0x3F;
    ERICNET::Add_Building_(0, BUILDING_MISSILE_BASE);
    DOCTEST_CHECK(MOX::_star[0].ignore_combat_ships == 0x3F);

    Reset_Colony_();
    MOX::_colony[0].buildings[BUILDING_RADIATION_SHIELD] = 1;
    MOX::_star[0].ignore_combat_ships = 0x3F;
    ERICNET::Remove_Building_(0, BUILDING_RADIATION_SHIELD);
    DOCTEST_CHECK(MOX::_star[0].ignore_combat_ships == 0);

    Restore_Default_Reengage_Buildings_();
}

}