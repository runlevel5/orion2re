#include "pch.h"

#include "doctest/doctest.h"
#include "fakes/fake_build_queue_runtime.h"
#include "game/build_queue.h"

namespace {
    void Set_List_(int preset, std::initializer_list<uint8_t> instructions) {
        memset(BUILD_QUEUE::presets[preset], BUILD_QUEUE::INSTRUCTION_END,
               BUILD_QUEUE::INSTRUCTION_CAPACITY);
        int index = 0;
        for (uint8_t instruction : instructions) {
            BUILD_QUEUE::presets[preset][index++] = instruction;
        }
    }
}

DOCTEST_TEST_SUITE("automatic build queues") {

DOCTEST_TEST_CASE("append rejects unavailable and duplicate buildings without overflowing") {
    TEST_BUILD_QUEUE::Reset_();
    MOX::_colony[0].producing[0] = BUILDING_AUTOMATED_FACTORY;
    TEST_BUILD_QUEUE::rejected_product = BUILDING_RESEARCH_LAB;
    Set_List_(0, {
        BUILD_QUEUE::INSTRUCTION_APPEND,
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_AUTOMATED_FACTORY),
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_RESEARCH_LAB),
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_SPACEPORT),
        BUILD_QUEUE::INSTRUCTION_END,
    });

    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 0));
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == BUILDING_AUTOMATED_FACTORY);
    DOCTEST_CHECK(MOX::_colony[0].producing[1] == BUILDING_SPACEPORT);
    DOCTEST_CHECK(MOX::_colony[0].producing[2] == COLONY_PRODUCTION_NONE);

    for (int slot = 0; slot < 7; ++slot) {
        MOX::_colony[0].producing[slot] = BUILDING_ALIEN_CONTROL_CENTER + slot;
    }
    Set_List_(0, {
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_SPACE_ACADEMY),
        BUILD_QUEUE::INSTRUCTION_END,
    });
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 0));
    DOCTEST_CHECK(MOX::_colony[0].producing[6] == BUILDING_ALIEN_CONTROL_CENTER + 6);
}

DOCTEST_TEST_CASE("prepend and merge preserve deterministic build-list order") {
    TEST_BUILD_QUEUE::Reset_();
    MOX::_colony[0].producing[0] = BUILDING_SPACEPORT;
    Set_List_(1, {
        BUILD_QUEUE::INSTRUCTION_PREPEND,
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_AUTOMATED_FACTORY),
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_RESEARCH_LAB),
        BUILD_QUEUE::INSTRUCTION_END,
    });
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 1));
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == BUILDING_AUTOMATED_FACTORY);
    DOCTEST_CHECK(MOX::_colony[0].producing[1] == BUILDING_RESEARCH_LAB);
    DOCTEST_CHECK(MOX::_colony[0].producing[2] == BUILDING_SPACEPORT);

    MOX::_colony[0].producing[0] = BUILDING_SPACEPORT;
    MOX::_colony[0].producing[1] = BUILDING_RESEARCH_LAB;
    MOX::_colony[0].producing[2] = BUILDING_AUTOMATED_FACTORY;
    for (int slot = 3; slot < 7; ++slot) {
        MOX::_colony[0].producing[slot] = COLONY_PRODUCTION_NONE;
    }
    Set_List_(1, {
        BUILD_QUEUE::INSTRUCTION_MERGE,
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_AUTOMATED_FACTORY),
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_RESEARCH_LAB),
        BUILD_QUEUE::INSTRUCTION_END,
    });
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 1));
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == BUILDING_SPACEPORT);
    DOCTEST_CHECK(MOX::_colony[0].producing[1] == BUILDING_AUTOMATED_FACTORY);
    DOCTEST_CHECK(MOX::_colony[0].producing[2] == BUILDING_RESEARCH_LAB);

    MOX::_colony[0].producing[0] = BUILDING_SPACEPORT;
    MOX::_colony[0].producing[1] = COLONY_PRODUCTION_NONE;
    MOX::_colony[0].producing[2] = COLONY_PRODUCTION_NONE;
    Set_List_(1, {
        BUILD_QUEUE::INSTRUCTION_MERGE,
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_AUTOMATED_FACTORY),
        BUILD_QUEUE::INSTRUCTION_END,
    });
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 1));
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == BUILDING_AUTOMATED_FACTORY);
    DOCTEST_CHECK(MOX::_colony[0].producing[1] == BUILDING_SPACEPORT);
}

DOCTEST_TEST_CASE("clear preserves bought products and refits while clear_refits removes refits") {
    TEST_BUILD_QUEUE::Reset_();
    MOX::_colony[0].producing[0] = BUILDING_AUTOMATED_FACTORY;
    MOX::_colony[0].bought_outright = 50;
    MOX::_ship[0].status = 4;
    MOX::_colony[0].producing[1] = COLBLDG::Colony_Production_From_Ship_Index_(0);
    MOX::_colony[0].producing[2] = BUILDING_RESEARCH_LAB;
    Set_List_(2, {BUILD_QUEUE::INSTRUCTION_CLEAR, BUILD_QUEUE::INSTRUCTION_END});
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 2));
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == BUILDING_AUTOMATED_FACTORY);
    DOCTEST_CHECK(MOX::_colony[0].producing[1] ==
                  COLBLDG::Colony_Production_From_Ship_Index_(0));
    DOCTEST_CHECK(MOX::_colony[0].producing[2] == COLONY_PRODUCTION_NONE);

    Set_List_(2, {BUILD_QUEUE::INSTRUCTION_CLEAR_REFITS, BUILD_QUEUE::INSTRUCTION_END});
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 2));
    DOCTEST_CHECK(MOX::_colony[0].producing[1] == COLONY_PRODUCTION_NONE);
    DOCTEST_CHECK(MOX::_ship[0].status == 5);
}

DOCTEST_TEST_CASE("merge commands affect only cells selected by the merge list") {
    TEST_BUILD_QUEUE::Reset_();
    MOX::_colony[0].producing[0] = BUILDING_SPACEPORT;
    MOX::_colony[0].producing[1] = BUILDING_RESEARCH_LAB;
    MOX::_colony[0].production_spent = 100;
    Set_List_(2, {
        BUILD_QUEUE::INSTRUCTION_MERGE,
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_RESEARCH_LAB),
        BUILD_QUEUE::INSTRUCTION_BUY,
        BUILD_QUEUE::INSTRUCTION_CLEAR,
        BUILD_QUEUE::INSTRUCTION_END,
    });

    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 2));
    DOCTEST_CHECK(MOX::_colony[0].bought_outright == 0);
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == BUILDING_SPACEPORT);
    DOCTEST_CHECK(MOX::_colony[0].producing[1] == COLONY_PRODUCTION_NONE);
}

DOCTEST_TEST_CASE("stop ignores trailing products and buying respects thresholds") {
    TEST_BUILD_QUEUE::Reset_();
    Set_List_(3, {
        BUILD_QUEUE::INSTRUCTION_HOUSING,
        BUILD_QUEUE::INSTRUCTION_STOP,
        BUILD_QUEUE::INSTRUCTION_TRADE_GOODS,
        BUILD_QUEUE::INSTRUCTION_END,
    });
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 3));
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == COLONY_PRODUCTION_HOUSING);
    DOCTEST_CHECK(MOX::_colony[0].producing[1] == COLONY_PRODUCTION_NONE);

    MOX::_colony[0].producing[0] = BUILDING_AUTOMATED_FACTORY;
    MOX::_colony[0].production_spent = 9;
    Set_List_(3, {BUILD_QUEUE::INSTRUCTION_BUY10, BUILD_QUEUE::INSTRUCTION_END});
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 3));
    DOCTEST_CHECK(MOX::_colony[0].bought_outright == 0);
    MOX::_colony[0].production_spent = 10;
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 3));
    DOCTEST_CHECK(MOX::_colony[0].bought_outright == 90);
}

DOCTEST_TEST_CASE("repeat follows only repeatable products and ship allocation failure is harmless") {
    TEST_BUILD_QUEUE::Reset_();
    Set_List_(5, {
        static_cast<uint8_t>(BUILD_QUEUE::INSTRUCTION_BUILDING_BASE + BUILDING_RESEARCH_LAB),
        BUILD_QUEUE::INSTRUCTION_REPEAT,
        BUILD_QUEUE::INSTRUCTION_SPY,
        BUILD_QUEUE::INSTRUCTION_REPEAT,
        BUILD_QUEUE::INSTRUCTION_END,
    });
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 5));
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == BUILDING_RESEARCH_LAB);
    DOCTEST_CHECK(MOX::_colony[0].producing[1] == COLONY_PRODUCTION_SPY);
    DOCTEST_CHECK(MOX::_colony[0].producing[2] == COLONY_PRODUCTION_REPEAT);

    TEST_BUILD_QUEUE::next_ship = -1;
    Set_List_(5, {
        BUILD_QUEUE::INSTRUCTION_DESIGN1,
        BUILD_QUEUE::INSTRUCTION_END,
    });
    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 5));
    DOCTEST_CHECK(MOX::_colony[0].producing[3] == COLONY_PRODUCTION_NONE);
}

DOCTEST_TEST_CASE("unbuy refuses a treasury overflow") {
    TEST_BUILD_QUEUE::Reset_();
    MOX::_colony[0].producing[0] = BUILDING_AUTOMATED_FACTORY;
    MOX::_colony[0].production_spent = 50;
    MOX::_colony[0].bought_outright = 50;
    MOX::_player[0].bc = INT32_MAX;
    Set_List_(6, {BUILD_QUEUE::INSTRUCTION_UNBUY, BUILD_QUEUE::INSTRUCTION_END});

    DOCTEST_REQUIRE(BUILD_QUEUE::Apply_Preset_To_Colony_(0, 6));
    DOCTEST_CHECK(MOX::_player[0].bc == INT32_MAX);
    DOCTEST_CHECK(MOX::_colony[0].bought_outright == 50);
}

DOCTEST_TEST_CASE("empire application skips outposts and other players") {
    TEST_BUILD_QUEUE::Reset_();
    MOX::_NUM_COLONIES = 4;
    MOX::_colony[1].outpost_flag = 1;
    MOX::_colony[2].owner = 1;
    Set_List_(4, {BUILD_QUEUE::INSTRUCTION_HOUSING, BUILD_QUEUE::INSTRUCTION_END});

    BUILD_QUEUE::Apply_Preset_To_Empire_(0, 4);
    DOCTEST_CHECK(MOX::_colony[0].producing[0] == COLONY_PRODUCTION_HOUSING);
    DOCTEST_CHECK(MOX::_colony[1].producing[0] == COLONY_PRODUCTION_NONE);
    DOCTEST_CHECK(MOX::_colony[2].producing[0] == COLONY_PRODUCTION_NONE);
    DOCTEST_CHECK(MOX::_colony[3].producing[0] == COLONY_PRODUCTION_HOUSING);
    DOCTEST_CHECK(TEST_BUILD_QUEUE::player_calculations == 1);
}

}