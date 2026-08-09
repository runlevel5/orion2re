#include "pch.h"

#include "doctest/doctest.h"

namespace TEST_COLONY_POP_GROWTH {
    void Reset_();
    int32_t Destroy_Colony_Reason_();
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

DOCTEST_TEST_CASE("classic starvation preserves the final population") {
    s_colony* colony = Prepare_Starving_Colony_();

    COLCALC::Apply_Colony_Pop_Growth_(colony);

    DOCTEST_CHECK(colony->n_pops == 1);
}

DOCTEST_TEST_CASE("configured extinction allows a human colony to starve out") {
    s_colony* colony = Prepare_Starving_Colony_();
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