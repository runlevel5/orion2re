#include "pch.h"

#include "game/build_queue.h"
#include "doctest/doctest.h"

namespace TEST_COLCALC_BASE {
    void Reset_();
    void Set_Breakthrough_(int32_t chance, uint32_t roll, int32_t cost);
    int Give_Player_Field_Calls_();
}

DOCTEST_TEST_SUITE("colony production helpers") {

DOCTEST_TEST_CASE("building product classification excludes sentinels and upper boundary") {
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Building_(BUILDING_NO_BUILDING));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Building_(BUILDING_ALIEN_CONTROL_CENTER));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Building_(BUILDING_ARTIFICIAL_PLANET));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Building_(BUILDING_COUNT));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Building_(COLONY_PRODUCTION_NONE));
}

DOCTEST_TEST_CASE("ship product classification preserves queued and design ranges") {
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Queued_Ship_(-101));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Queued_Ship_(-100));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Queued_Ship_(-99));

    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Ship_Design_(-100));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_Design_(-99));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_Design_(-50));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Ship_Design_(-49));

    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(-101));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(-100));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(-99));
    DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(-50));
    DOCTEST_CHECK_FALSE(COLBLDG::Colony_Production_Is_Ship_(-49));
}

DOCTEST_TEST_CASE("ship product classification matches the original binary range") {
    for (int32_t production_id = INT16_MIN; production_id <= INT16_MAX; ++production_id) {
        const bool original_is_ship = production_id <= COLONY_PRODUCTION_SHIP_DESIGN_BASE;
        DOCTEST_CHECK(
            COLBLDG::Colony_Production_Is_Ship_(static_cast<int16_t>(production_id)) == original_is_ship);
    }
}

DOCTEST_TEST_CASE("ship product encodings round trip through their matching ranges") {
    for (int16_t ship_idx = 0; ship_idx < MAX_SHIPS; ++ship_idx) {
        const int16_t production_id = COLBLDG::Colony_Production_From_Ship_Index_(ship_idx);
        DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Queued_Ship_(production_id));
        DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_(production_id));
        DOCTEST_CHECK(COLBLDG::Colony_Production_Ship_Index_(production_id) == ship_idx);
    }

    for (int16_t design_idx : {int16_t{0}, int16_t{1}, int16_t{49}}) {
        const int16_t production_id = COLBLDG::Colony_Production_From_Ship_Design_Index_(design_idx);
        DOCTEST_CHECK(COLBLDG::Colony_Production_Is_Ship_Design_(production_id));
        DOCTEST_CHECK(COLBLDG::Colony_Production_Ship_Design_Index_(production_id) == design_idx);
    }
}

DOCTEST_TEST_CASE("base production reflects recalculated colony fields") {
    TEST_COLCALC_BASE::Reset_();

    s_colony colony = {};
    colony.owner = 0;
    colony.food2_per_farmer = 8;
    colony.industry_per_worker = 7;
    colony.research_per_scientist = 6;

    DOCTEST_CHECK(COLCALC::Colony_Empire_Base_Food2_Produced_(&colony, 0, 0, nullptr) == 8);
    DOCTEST_CHECK(COLCALC::Colony_Empire_Base_Industry_Produced_(&colony, 0, 0, nullptr) == 7);
    DOCTEST_CHECK(COLCALC::Colony_Empire_Base_Research_Produced_(&colony, 0, 0, nullptr) == 6);

    colony.food2_per_farmer = 4;
    colony.industry_per_worker = 3;
    colony.research_per_scientist = 2;

    DOCTEST_CHECK(COLCALC::Colony_Empire_Base_Food2_Produced_(&colony, 0, 0, nullptr) == 4);
    DOCTEST_CHECK(COLCALC::Colony_Empire_Base_Industry_Produced_(&colony, 0, 0, nullptr) == 3);
    DOCTEST_CHECK(COLCALC::Colony_Empire_Base_Research_Produced_(&colony, 0, 0, nullptr) == 2);
}

DOCTEST_TEST_CASE("research production is banked when no field is selected") {
    struct s_research_case {
        int32_t accumulated;
        int16_t produced;
        int32_t expected;
    };

    const s_research_case cases[] = {
        {0, 6, 6},
        {17, 6, 23},
        {17, -6, 11},
    };

    for (const s_research_case& test_case : cases) {
        s_player player = {};
        player.current_research_field = 0;
        player.research_accumulated = test_case.accumulated;
        player.research_produced = test_case.produced;

        COLCALC::Check_For_Research_Breakthrough_(&player);

        DOCTEST_CHECK(player.research_accumulated == test_case.expected);
    }
}

DOCTEST_TEST_CASE("research completion preserves configured points and marks selection pending") {
    s_player player = {};
    player.research_accumulated = 275;

    COLCALC::Resolve_Research_Completion_(&player, true, 200);

    DOCTEST_CHECK(player.research_breakthrough == 1);
    DOCTEST_CHECK(player.research_accumulated == 75);

    COLCALC::Resolve_Research_Completion_(&player, true, 100);
    DOCTEST_CHECK(player.research_accumulated == 0);

    player.research_accumulated = 75;
    COLCALC::Resolve_Research_Completion_(&player, false, 0);
    DOCTEST_CHECK(player.research_accumulated == 0);
}

DOCTEST_TEST_CASE("configured breakthroughs retain research above twice the field cost") {
    TEST_COLCALC_BASE::Reset_();
    TEST_COLCALC_BASE::Set_Breakthrough_(100, 1, 100);
    COLCALC::_keep_research_on_breakthrough = 1;

    s_player player = {};
    player.current_research_field = 1;
    player.research_accumulated = 250;
    player.research_produced = 25;

    COLCALC::Check_For_Research_Breakthrough_(&player);

    DOCTEST_CHECK(player.research_breakthrough == 1);
    DOCTEST_CHECK(player.research_accumulated == 75);
    DOCTEST_CHECK(TEST_COLCALC_BASE::Give_Player_Field_Calls_() == 1);
}

DOCTEST_TEST_CASE("classic breakthroughs spend all accumulated research") {
    TEST_COLCALC_BASE::Reset_();
    TEST_COLCALC_BASE::Set_Breakthrough_(100, 1, 100);

    s_player player = {};
    player.current_research_field = 1;
    player.research_accumulated = 250;
    player.research_produced = 25;

    COLCALC::Check_For_Research_Breakthrough_(&player);

    DOCTEST_CHECK(player.research_breakthrough == 1);
    DOCTEST_CHECK(player.research_accumulated == 0);
}

DOCTEST_TEST_CASE("pending research selection survives turns without a breakthrough") {
    TEST_COLCALC_BASE::Reset_();
    TEST_COLCALC_BASE::Set_Breakthrough_(0, 100, 100);

    s_player player = {};
    player.current_research_field = 1;
    player.research_breakthrough = 1;
    player.research_accumulated = 25;
    player.research_produced = 5;

    COLCALC::Check_For_Research_Breakthrough_(&player);

    DOCTEST_CHECK(player.research_breakthrough == 1);
    DOCTEST_CHECK(player.research_accumulated == 30);
    DOCTEST_CHECK(TEST_COLCALC_BASE::Give_Player_Field_Calls_() == 0);
}

DOCTEST_TEST_CASE("build queue hotkeys map to all eleven numeric presets") {
    DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Hotkey_('q') == 0);
    DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Hotkey_('Q') == 0);
    for (char digit = '1'; digit <= '9'; ++digit) {
        DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Hotkey_(digit) == digit - '0');
    }
    DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Hotkey_('0') == 10);
    DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Hotkey_('x') == -1);
}

DOCTEST_TEST_CASE("shifted digits map to empire build queue presets") {
    DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Empire_Hotkey_('q') == 0);
    DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Empire_Hotkey_('Q') == 0);
    constexpr char shifted[] = "!@#$%^&*()";
    for (int index = 0; index < 9; ++index) {
        DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Empire_Hotkey_(shifted[index]) == index + 1);
    }
    DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Empire_Hotkey_(')') == 10);
    DOCTEST_CHECK(BUILD_QUEUE::Preset_For_Empire_Hotkey_('1') == -1);
}

}