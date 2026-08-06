#include "pch.h"

#include "doctest/doctest.h"
#include "game/settler.h"

namespace TEST_SETTLER {
    void Reset_();
    void Set_Pop_Limit_(int32_t race_idx, int16_t pop_limit);
}

static void Add_Resident_Pops_(s_colony* colony, int32_t race_idx, int32_t pop_count) {
    for (int32_t i = 0; i < pop_count; ++i) {
        POP::Set_Race(&colony->pop[(int32_t)(uint8_t)colony->n_pops], (uint32_t)race_idx);
        colony->n_pops++;
    }
}

static void Add_Inbound_Settler_(s_player* player, int32_t race_idx, int16_t dest_planet) {
    SETTLER::value_type settler_data = 0;
    SETTLER::Set_Race(&settler_data, (uint32_t)race_idx);
    SETTLER::Set_Dest_Planet(&settler_data, dest_planet);
    player->settlers[player->settlers_freighted].val = settler_data;
    player->settlers_freighted++;
}

DOCTEST_TEST_SUITE("settler population limits") {

DOCTEST_TEST_CASE("inbound settlers count only when scheduling another settler") {
    TEST_SETTLER::Reset_();
    TEST_SETTLER::Set_Pop_Limit_(0, 5);

    s_colony* colony = &MOX::_colony[0];
    colony->owner = 0;
    colony->planet = 0;
    Add_Resident_Pops_(colony, 0, 4);
    Add_Inbound_Settler_(&MOX::_player[0], 0, colony->planet);

    DOCTEST_CHECK(SETTLER::Room_For_Another_(colony, 0, 0));
    DOCTEST_CHECK_FALSE(SETTLER::Room_For_Another_(colony, 0, 1));
}

DOCTEST_TEST_CASE("settlers heading elsewhere do not consume destination capacity") {
    TEST_SETTLER::Reset_();
    TEST_SETTLER::Set_Pop_Limit_(0, 5);

    s_colony* colony = &MOX::_colony[0];
    colony->owner = 0;
    colony->planet = 0;
    Add_Resident_Pops_(colony, 0, 4);
    Add_Inbound_Settler_(&MOX::_player[0], 0, 1);

    DOCTEST_CHECK(SETTLER::Room_For_Another_(colony, 0, 1));
}

DOCTEST_TEST_CASE("higher mixed-race limit blocks an additional lower-limit settler") {
    TEST_SETTLER::Reset_();
    TEST_SETTLER::Set_Pop_Limit_(0, 5);
    TEST_SETTLER::Set_Pop_Limit_(1, 10);

    s_colony* colony = &MOX::_colony[0];
    colony->owner = 0;
    colony->planet = 0;
    Add_Resident_Pops_(colony, 0, 4);
    Add_Resident_Pops_(colony, 1, 6);

    DOCTEST_CHECK_FALSE(SETTLER::Room_For_Another_(colony, 0, 1));
}

DOCTEST_TEST_CASE("absolute colony population limit remains 42") {
    TEST_SETTLER::Reset_();
    TEST_SETTLER::Set_Pop_Limit_(0, 50);

    s_colony* colony = &MOX::_colony[0];
    colony->owner = 0;
    colony->planet = 0;
    Add_Resident_Pops_(colony, 0, 42);

    DOCTEST_CHECK_FALSE(SETTLER::Room_For_Another_(colony, 0, 0));
}

}