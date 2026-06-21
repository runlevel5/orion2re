#include "pch.h"

#include "doctest/doctest.h"

namespace {
    uint32_t Legacy_Set_Pop_Race(uint32_t pop, uint32_t race) {
        return ((race ^ pop) & 0x0000000Fu) ^ pop;
    }

    uint32_t Legacy_Set_Pop_Original_Owner(uint32_t pop, uint32_t owner) {
        return (((owner << 4) ^ pop) & 0x00000070u) ^ pop;
    }

    uint32_t Legacy_Set_Pop_Prof(uint32_t pop, uint32_t prof) {
        return (((prof << 7) ^ pop) & 0x00000180u) ^ pop;
    }

    uint32_t Legacy_Set_Settler_Race(uint32_t settler_data, uint32_t race) {
        return (((race << 16) ^ settler_data) & 0x000F0000u) ^ settler_data;
    }

    uint32_t Legacy_Set_Settler_Prof(uint32_t settler_data, uint32_t prof) {
        return (((prof << 24) ^ settler_data) & 0x03000000u) ^ settler_data;
    }

    uint32_t Legacy_Set_Settler_Eta(uint32_t settler_data, uint32_t eta) {
        return (((eta << 20) ^ settler_data) & 0x00F00000u) ^ settler_data;
    }

    uint32_t Legacy_Set_Settler_Source(uint32_t settler_data, uint32_t source_colony) {
        return ((source_colony ^ settler_data) & 0x000000FFu) ^ settler_data;
    }

    uint32_t Legacy_Set_Settler_Dest(uint32_t settler_data, uint32_t dest_planet) {
        return (((dest_planet << 8) ^ settler_data) & 0x0000FF00u) ^ settler_data;
    }

    uint32_t Legacy_Add_Settler_To_Pop(uint32_t initial_pop, uint32_t settler_data) {
        uint32_t race_info = settler_data >> 16;
        uint32_t pop = initial_pop;

        pop = (pop ^ race_info) & 0x0000000F ^ pop;
        pop = ((race_info << 4) ^ pop) & 0x00000070 ^ pop;
        pop = ((settler_data >> 17) ^ pop) & 0x00000180 ^ pop;
        pop |= 0x00000200;
        pop &= 0xFFFFFBFF;

        return pop;
    }

    uint32_t Helper_Add_Settler_To_Pop(uint32_t initial_pop, uint32_t settler_data) {
        uint32_t pop = initial_pop;

        POP::Set_Race(&pop, SETTLER_DATA::Get_Race(settler_data));
        POP::Set_Original_Owner(&pop, SETTLER_DATA::Get_Race(settler_data));
        POP::Set_Prof(&pop, SETTLER_DATA::Get_Prof(settler_data));
        POP::Set_Assigned(&pop, true);
        POP::Set_Conquered(&pop, false);

        return pop;
    }

    uint32_t Legacy_Encode_Settler_From_Pop(uint32_t pop) {
        uint32_t encoded_settler = pop;

        encoded_settler ^= ((pop << 16) ^ encoded_settler) & 0x000F0000;
        encoded_settler ^= ((pop << 17) ^ encoded_settler) & 0x03000000;

        return encoded_settler;
    }

    uint32_t Helper_Encode_Settler_From_Pop(uint32_t pop) {
        uint32_t encoded_settler = 0;

        SETTLER_DATA::Set_Race(&encoded_settler, POP::Get_Race(pop));
        SETTLER_DATA::Set_Prof(&encoded_settler, POP::Get_Prof(pop));

        return encoded_settler;
    }
}

DOCTEST_TEST_SUITE("pop helpers") {

DOCTEST_TEST_CASE("pop subfield setters match the legacy masked updates") {
    for (uint32_t pop : {0u, 0xFFFFFFFFu, 0x12345678u, 0x00000400u, 0x00000280u}) {
        for (uint32_t race = 0; race < 16; ++race) {
            uint32_t actual = pop;
            POP::Set_Race(&actual, race);
            DOCTEST_CHECK(actual == Legacy_Set_Pop_Race(pop, race));
        }

        for (uint32_t owner = 0; owner < 8; ++owner) {
            uint32_t actual = pop;
            POP::Set_Original_Owner(&actual, owner);
            DOCTEST_CHECK(actual == Legacy_Set_Pop_Original_Owner(pop, owner));
        }

        for (uint32_t prof = 0; prof < 4; ++prof) {
            uint32_t actual = pop;
            POP::Set_Prof(&actual, static_cast<POP::e_prof>(prof));
            DOCTEST_CHECK(actual == Legacy_Set_Pop_Prof(pop, prof));
        }
    }
}

DOCTEST_TEST_CASE("settler subfield setters match the legacy masked updates") {
    for (uint32_t settler_data : {0u, 0xFFFFFFFFu, 0x13579BDFu}) {
        for (uint32_t race = 0; race < 16; ++race) {
            uint32_t actual = settler_data;
            SETTLER_DATA::Set_Race(&actual, race);
            DOCTEST_CHECK(actual == Legacy_Set_Settler_Race(settler_data, race));
        }

        for (uint32_t prof = 0; prof < 4; ++prof) {
            uint32_t actual = settler_data;
            SETTLER_DATA::Set_Prof(&actual, static_cast<POP::e_prof>(prof));
            DOCTEST_CHECK(actual == Legacy_Set_Settler_Prof(settler_data, prof));
        }

        for (uint32_t eta = 0; eta < 16; ++eta) {
            uint32_t actual = settler_data;
            SETTLER_DATA::Set_Eta(&actual, eta);
            DOCTEST_CHECK(actual == Legacy_Set_Settler_Eta(settler_data, eta));
        }

        for (uint32_t source_colony : {0u, 1u, 0x7Fu, 0xFFu}) {
            uint32_t actual = settler_data;
            SETTLER_DATA::Set_Source_Colony(&actual, source_colony);
            DOCTEST_CHECK(actual == Legacy_Set_Settler_Source(settler_data, source_colony));
        }

        for (uint32_t dest_planet : {0u, 1u, 0x80u, 0xFFu}) {
            uint32_t actual = settler_data;
            SETTLER_DATA::Set_Dest_Planet(&actual, dest_planet);
            DOCTEST_CHECK(actual == Legacy_Set_Settler_Dest(settler_data, dest_planet));
        }
    }
}

DOCTEST_TEST_CASE("settler-to-pop rewrite matches the original bit twiddling") {
    const uint32_t initial_values[] = {
        0u,
        0xFFFFFFFFu,
        0x00000400u,
        0x13579BDFu,
    };

    for (uint32_t initial_pop : initial_values) {
        for (uint32_t race = 0; race < 16; ++race) {
            for (uint32_t prof = 0; prof < 4; ++prof) {
                uint32_t settler_data = 0;
                SETTLER_DATA::Set_Race(&settler_data, race);
                SETTLER_DATA::Set_Prof(&settler_data, static_cast<POP::e_prof>(prof));

                DOCTEST_CHECK(Helper_Add_Settler_To_Pop(initial_pop, settler_data) ==
                              Legacy_Add_Settler_To_Pop(initial_pop, settler_data));
            }
        }
    }
}

DOCTEST_TEST_CASE("pop-to-settler encoding keeps race and profession equivalent to the original code") {
    for (uint32_t race = 0; race < 16; ++race) {
        for (uint32_t prof = 0; prof < 4; ++prof) {
            uint32_t pop = 0xABCDEF00u;
            POP::Set_Race(&pop, race);
            POP::Set_Prof(&pop, static_cast<POP::e_prof>(prof));

            const uint32_t legacy = Legacy_Encode_Settler_From_Pop(pop);
            const uint32_t helper = Helper_Encode_Settler_From_Pop(pop);

            DOCTEST_CHECK(SETTLER_DATA::Get_Race(helper) == SETTLER_DATA::Get_Race(legacy));
            DOCTEST_CHECK(SETTLER_DATA::Get_Prof(helper) == SETTLER_DATA::Get_Prof(legacy));
        }
    }
}

DOCTEST_TEST_CASE("semantic getters expose profession assignment conquest and special-race behavior") {
    uint32_t pop = 0;
    POP::Set_Race(&pop, POP::RACE_ANDROID);
    POP::Set_Original_Owner(&pop, 3);
    POP::Set_Prof(&pop, POP::PROF_WORKER);
    POP::Set_Assigned(&pop, true);
    POP::Set_Conquered(&pop, true);

    DOCTEST_CHECK(POP::Get_Race(pop) == POP::RACE_ANDROID);
    DOCTEST_CHECK(POP::Get_Original_Owner(pop) == 3);
    DOCTEST_CHECK(POP::Get_Prof(pop) == POP::PROF_WORKER);
    DOCTEST_CHECK(POP::Is_Worker(pop));
    DOCTEST_CHECK(POP::Is_Assigned(pop));
    DOCTEST_CHECK(POP::Is_Conquered(pop));
    DOCTEST_CHECK(POP::Is_Android(pop));
    DOCTEST_CHECK(POP::Is_Android_Or_Native(pop));
    DOCTEST_CHECK(POP::Get_Effective_Player(pop, 5) == 5);

    POP::Set_Race(&pop, 2);

    DOCTEST_CHECK(!POP::Is_Android_Or_Native(pop));
    DOCTEST_CHECK(POP::Get_Effective_Player(pop, 5) == 2);
}

}