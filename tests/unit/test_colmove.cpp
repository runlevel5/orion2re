#include "pch.h"

#include "doctest/doctest.h"
#include "game/colmove.h"

namespace TEST_COLMOVE {
    extern int16_t help_call_count;
    extern int16_t settle_call_count;

    void Reset_();
}

DOCTEST_TEST_CASE("farmer job change enforces farm capacity outside transfer") {
    TEST_COLMOVE::Reset_();
    MOX::_colony[0].n_pops = 1;
    MOX::_colony[0].max_farms = 0;
    POP::Set_Prof(&MOX::_colony[0].pop[0], POP::PROF_WORKER);
    POP::Set_Assigned(&MOX::_colony[0].pop[0], true);

    DOCTEST_CHECK(COLMOVE::Give_Colonist_New_Job_(0, 0, ECON_FOOD, false) == 0);
    DOCTEST_CHECK(TEST_COLMOVE::help_call_count == 1);
    DOCTEST_CHECK(POP::Get_Prof(MOX::_colony[0].pop[0]) == POP::PROF_WORKER);
}

DOCTEST_TEST_CASE("farmer job change ignores source farm capacity during transfer") {
    TEST_COLMOVE::Reset_();
    MOX::_colony[0].n_pops = 1;
    MOX::_colony[0].max_farms = 0;
    POP::Set_Prof(&MOX::_colony[0].pop[0], POP::PROF_WORKER);
    POP::Set_Assigned(&MOX::_colony[0].pop[0], true);

    DOCTEST_CHECK(COLMOVE::Give_Colonist_New_Job_(0, 0, ECON_FOOD, true) == 1);
    DOCTEST_CHECK(TEST_COLMOVE::help_call_count == 0);
    DOCTEST_CHECK(POP::Get_Prof(MOX::_colony[0].pop[0]) == POP::PROF_FARMER);
}

DOCTEST_TEST_CASE("same-colony cluster reassignment enforces farm capacity") {
    TEST_COLMOVE::Reset_();
    MOX::_colony[0].n_pops = 1;
    MOX::_colony[0].max_farms = 0;
    POP::Set_Prof(&MOX::_colony[0].pop[0], POP::PROF_WORKER);
    COLMOVE::_cluster_colony_n = 0;

    COLMOVE::Send_Cluster_(0, ECON_FOOD);

    DOCTEST_CHECK(TEST_COLMOVE::help_call_count == 1);
    DOCTEST_CHECK(TEST_COLMOVE::settle_call_count == 0);
    DOCTEST_CHECK(POP::Get_Prof(MOX::_colony[0].pop[0]) == POP::PROF_WORKER);
}

DOCTEST_TEST_CASE("inter-colony cluster transfer ignores source farm capacity") {
    TEST_COLMOVE::Reset_();
    MOX::_colony[0].n_pops = 1;
    MOX::_colony[0].max_farms = 0;
    POP::Set_Prof(&MOX::_colony[0].pop[0], POP::PROF_WORKER);
    MOX::_colony[1].max_farms = UINT8_MAX;
    COLMOVE::_cluster_colony_n = 0;

    COLMOVE::Send_Cluster_(1, ECON_FOOD);

    DOCTEST_CHECK(TEST_COLMOVE::help_call_count == 0);
    DOCTEST_CHECK(TEST_COLMOVE::settle_call_count == 1);
    DOCTEST_CHECK(MOX::_colony[0].n_pops == 0);
    DOCTEST_CHECK(MOX::_colony[1].n_pops == 1);
    DOCTEST_CHECK(POP::Get_Prof(MOX::_colony[1].pop[0]) == POP::PROF_FARMER);
}