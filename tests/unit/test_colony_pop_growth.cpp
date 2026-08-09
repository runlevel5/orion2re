#include "pch.h"

#include "doctest/doctest.h"

namespace TEST_COLONY_POP_GROWTH {
    void Reset_();
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

    uint32_t New_Colonist_Job_(const s_colony* colony) {
        return (colony->pop[1] & 0x180u) >> 7;
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

DOCTEST_TEST_CASE("zero research field is not advanced") {
    TEST_COLONY_POP_GROWTH::Reset_();

    s_player player = {};
    player.tech_fields[TECH_FIELD_STARTING_TECH] = 2;

    COLCALC::Advance_Player_Field_(&player, TECH_FIELD_STARTING_TECH, false);

    DOCTEST_CHECK(player.tech_fields[TECH_FIELD_STARTING_TECH] == 2);
}