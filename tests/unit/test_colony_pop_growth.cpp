#include "pch.h"

#include "doctest/doctest.h"

namespace TEST_COLONY_POP_GROWTH {
    void Reset_();
    int32_t Destroy_Colony_Reason_();
    void Set_Add_Spy_Result_(int16_t result);
    int16_t Deleted_Product_();
}

namespace {
    s_colony* Prepare_Housing_Growth_() {
        TEST_COLONY_POP_GROWTH::Reset_();

        s_colony* colony = &MOX::_colony[0];
        colony->owner = 0;
        colony->planet = 0;
        colony->occupation_policy = 4;
        colony->climate = PLANET_CLIMATE_GAIA;
        colony->n_pops = 1;
        colony->pop[0] = 1u << 7;
        colony->pop_growth[0] = 1000;
        colony->food2_per_farmer = 2;
        colony->food2_needed_for_our_empire = 2;
        colony->maintenance[ECON_INDUSTRY] = 1;
        colony->producing[0] = COLONY_PRODUCTION_HOUSING;
        return colony;
    }

    s_colony* Prepare_Starving_Colony_() {
        TEST_COLONY_POP_GROWTH::Reset_();

        s_colony* colony = &MOX::_colony[0];
        colony->owner = 0;
        colony->planet = 0;
        colony->occupation_policy = 4;
        colony->n_pops = 1;
        colony->pop_growth[0] = -1000;
        return colony;
    }

    uint32_t New_Colonist_Job_(const s_colony* colony) {
        return (colony->pop[1] & 0x180u) >> 7;
    }

    s_colony* Prepare_Zero_Colony_With_Inbound_Settlers_() {
        TEST_COLONY_POP_GROWTH::Reset_();

        s_colony* colony = &MOX::_colony[0];
        colony->owner = 0;
        colony->planet = 0;
        SETTLER::Set_Dest_Planet(&MOX::_player[0].settlers[0].val, colony->planet);
        MOX::_player[0].settlers_freighted = 1;
        return colony;
    }

    s_colony* Prepare_Completed_Product_(int16_t product, int16_t cost) {
        TEST_COLONY_POP_GROWTH::Reset_();

        s_colony* colony = &MOX::_colony[0];
        colony->owner = 0;
        colony->planet = 0;
        colony->n_pops = 1;
        colony->producing[0] = product;
        colony->producing[1] = COLONY_PRODUCTION_NONE;
        colony->production_spent = cost;
        return colony;
    }
}

DOCTEST_TEST_CASE("classic Housing growth may create a farmer") {
    s_colony* colony = Prepare_Housing_Growth_();

    COLCALC::Apply_Colony_Pop_Growth_(colony);

    DOCTEST_REQUIRE(colony->n_pops == 2);
    DOCTEST_CHECK(New_Colonist_Job_(colony) == 0);
}

DOCTEST_TEST_CASE("configured Housing growth bypasses farmer assignment") {
    s_colony* colony = Prepare_Housing_Growth_();
    COLCALC::_housing_creates_no_farmers = 1;

    COLCALC::Apply_Colony_Pop_Growth_(colony);

    DOCTEST_REQUIRE(colony->n_pops == 2);
    DOCTEST_CHECK(New_Colonist_Job_(colony) == 1);
}

DOCTEST_TEST_CASE("reported population capacities do not exceed colony storage") {
    TEST_COLONY_POP_GROWTH::Reset_();

    const int8_t previous_max_population =
        MOX::_planet_max_population[PLANET_SIZE_HUGE];
    MOX::_planet_max_population[PLANET_SIZE_HUGE] = 50;

    s_planet_data* planet = &MOX::_planet[0];
    planet->colony_index = -1;
    planet->size = PLANET_SIZE_HUGE;
    planet->climate = PLANET_CLIMATE_GAIA;

    DOCTEST_CHECK(COLCALC::Planet_Max_Population_For_Player_(planet, 0) == 42);

    s_colony* colony = &MOX::_colony[0];
    colony->owner = 0;
    colony->planet = 0;
    colony->climate = PLANET_CLIMATE_GAIA;

    DOCTEST_CHECK(COLCALC::Colony_Race_Pop_Limit_(colony, 0) == 42);

    MOX::_planet_max_population[PLANET_SIZE_HUGE] =
        previous_max_population;
}

DOCTEST_TEST_CASE("classic starvation preserves the final population") {
    s_colony* colony = Prepare_Starving_Colony_();

    COLCALC::Apply_Colony_Pop_Growth_(colony);

    DOCTEST_CHECK(colony->n_pops == 1);
}

DOCTEST_TEST_CASE("configured extinction allows a human colony to starve out") {
    s_colony* colony = Prepare_Starving_Colony_();
    colony->pop_growth[0] = -2000;
    MOX::_player[0].objectives = PLAYER_OBJECTIVE_HUMAN;
    COLCALC::_planet_extinction = 1;

    COLCALC::Apply_Colony_Pop_Growth_(colony);

    DOCTEST_CHECK(colony->n_pops == 0);
}

DOCTEST_TEST_CASE("configured extinction preserves the final AI population") {
    s_colony* colony = Prepare_Starving_Colony_();
    COLCALC::_planet_extinction = 1;

    COLCALC::Apply_Colony_Pop_Growth_(colony);

    DOCTEST_CHECK(colony->n_pops == 1);
}

DOCTEST_TEST_CASE("classic zero colony survives while settlers are inbound") {
    s_colony* colony = Prepare_Zero_Colony_With_Inbound_Settlers_();

    COLCALC::Ghost_Colony_Stuff_(colony);

    DOCTEST_CHECK(TEST_COLONY_POP_GROWTH::Destroy_Colony_Reason_() == -1);
}

DOCTEST_TEST_CASE("configured zero colony is lost despite inbound settlers") {
    s_colony* colony = Prepare_Zero_Colony_With_Inbound_Settlers_();
    COLCALC::_zero_colony_lost = 1;

    COLCALC::Ghost_Colony_Stuff_(colony);

    DOCTEST_CHECK(TEST_COLONY_POP_GROWTH::Destroy_Colony_Reason_() == 3);
}

DOCTEST_TEST_CASE("zero research field is not advanced") {
    TEST_COLONY_POP_GROWTH::Reset_();

    s_player player = {};
    player.tech_fields[TECH_FIELD_STARTING_TECH] = 2;

    COLCALC::Advance_Player_Field_(&player, TECH_FIELD_STARTING_TECH, false);

    DOCTEST_CHECK(player.tech_fields[TECH_FIELD_STARTING_TECH] == 2);
}

DOCTEST_TEST_CASE("Capitol can be built when the player has no homeworld") {
    TEST_COLONY_POP_GROWTH::Reset_();

    s_colony* colony = &MOX::_colony[0];
    colony->owner = 0;

    s_player* player = &MOX::_player[0];
    player->home_planet_id = -1;
    player->tech_applications[TECH_APP_CAPITOL] = TECH_RESEARCH_STATUS_RESEARCHED;

    DOCTEST_CHECK(COLCALC::Colony_Can_Build_Product_(colony, BUILDING_CAPITOL) == 1);
}

DOCTEST_TEST_CASE("completed building grants no maintenance cash") {
    s_colony* colony = Prepare_Completed_Product_(BUILDING_MARINE_BARRACKS, 60);
    TECHDATA::_buildings[BUILDING_MARINE_BARRACKS].cost = 60;
    TECHDATA::_buildings[BUILDING_MARINE_BARRACKS].maintenance = 3;
    MOX::_player[0].bc = 20;

    COLCALC::Apply_Production_(colony);

    DOCTEST_CHECK(MOX::_player[0].bc == 20);
}

DOCTEST_TEST_CASE("freighter completion uses configured cash bonus") {
    s_colony* colony = Prepare_Completed_Product_(COLONY_PRODUCTION_FREIGHTERS, 50);
    MOX::_player[0].bc = 20;

    COLCALC::Apply_Production_(colony);

    DOCTEST_CHECK(MOX::_player[0].bc == 20);

    colony = Prepare_Completed_Product_(COLONY_PRODUCTION_FREIGHTERS, 50);
    COLCALC::_freighters_cash_bonus = 5;
    MOX::_player[0].bc = 20;

    COLCALC::Apply_Production_(colony);

    DOCTEST_CHECK(MOX::_player[0].bc == 25);
}

DOCTEST_TEST_CASE("successful spy completion grants no maintenance cash") {
    s_colony* colony = Prepare_Completed_Product_(COLONY_PRODUCTION_SPY, 100);
    TEST_COLONY_POP_GROWTH::Set_Add_Spy_Result_(1);
    MOX::_player[0].bc = 20;

    COLCALC::Apply_Production_(colony);

    DOCTEST_CHECK(MOX::_player[0].bc == 20);
}

DOCTEST_TEST_CASE("failed spy completion refunds half its production cost") {
    s_colony* colony = Prepare_Completed_Product_(COLONY_PRODUCTION_SPY, 100);
    TEST_COLONY_POP_GROWTH::Set_Add_Spy_Result_(0);
    MOX::_player[0].bc = 20;

    COLCALC::Apply_Production_(colony);

    DOCTEST_CHECK(MOX::_player[0].bc == 70);
    DOCTEST_CHECK(TEST_COLONY_POP_GROWTH::Deleted_Product_() == COLONY_PRODUCTION_SPY);
}